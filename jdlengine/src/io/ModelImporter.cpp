#include "io/ModelImporter.hpp"

#include <glm/gtc/type_ptr.hpp>

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>

#include "core/Vertex.hpp"

#include "resource/Material.hpp"
#include "resource/Mesh.hpp"
#include "resource/Texture.hpp"

#include "scene/RenderableNode.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"

#include "utils/Logger.hpp"
#include "utils/FileUtils.hpp"
#include "utils/StringUtils.hpp"


namespace jdl
{
namespace io
{

// Supported GLTF extensions
static const std::string kASCIIExtension = ".gltf";
static const std::string kBinaryExtension = ".glb";

// Generates a name for the input GLTF object
template<typename T>
static std::string s_GetName(const T& object, const std::string& objectType, size_t objectIndex)
{
    if (!object.name.empty())
    {
        return object.name;
    }
    return objectType + std::to_string(objectIndex);
}

// Returns the pointer to the buffer data for the input attribute
static const float* s_GetAttributePtr(const tinygltf::Model& model,
                                      const tinygltf::Primitive& primitive,
                                      const std::string& attribute)
{
    auto it = primitive.attributes.find(attribute);
    if (it == primitive.attributes.end())
    {
        return nullptr;
    }

    const auto& accessor = model.accessors[it->second];
    const auto& view = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[view.buffer];

    return reinterpret_cast<const float*>(&buffer.data[view.byteOffset + accessor.byteOffset]);
}


ModelImporter ModelImporter::Importer;

scene::SceneNodePtr ModelImporter::doImport(const std::string& path, const scene::SceneNodePtr& parent)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;

    std::string errors;
    std::string warnings;
    bool loaded = true;

    std::string extension = utils::StringUtils::ToLower(utils::FileUtils::GetFileExtension(path));
    if (extension == kASCIIExtension)
    {
        loaded = loader.LoadASCIIFromFile(&model, &errors, &warnings, path);
    }
    else if (extension == kBinaryExtension)
    {
        loaded = loader.LoadBinaryFromFile(&model, &errors, &warnings, path);
    }
    else
    {
        JDL_ERROR("Invalid GLTF extension: {}", extension);
        return nullptr;
    }

    if (warnings.length() > 0)
    {
        JDL_WARN(warnings);
    }
    if (errors.length() > 0)
    {
        JDL_ERROR(errors);
    }
    if (!loaded)
    {
        JDL_ERROR("Failed to load model {}", path);
        return nullptr;
    }

    scene::SceneNodePtr rootNode = nullptr;

    std::string currentDir = utils::FileUtils::GetCurrentDir();
    {
        utils::FileUtils::SetCurrentDir(utils::FileUtils::GetDirName(path));

        importTextures(model);
        importMaterials(model);
        importMeshes(model);
        importNodes(model);

        // If the model has multiple scenes and/or nodes, group them under one single root node
        std::vector<scene::SceneNodePtr> rootNodes;
        for (const auto& scene : model.scenes)
        {
            for (int nodeIndex : scene.nodes)
            {
                rootNodes.push_back(m_nodes[nodeIndex]);
            }
        }
        if (rootNodes.size() > 1)
        {
            rootNode = scene::CreateNode<scene::SceneNode>("Root", nullptr);
            JDL_DEBUG("[IMPORTER] Import node {}", rootNode->getName());

            for (const auto& node : rootNodes)
            {
                rootNode->addChild(node);
            }
        }
        else
        {
            rootNode = rootNodes[0];
        }

        clear();
    }
    utils::FileUtils::SetCurrentDir(currentDir);

    return rootNode;
}

void ModelImporter::importTextures(const tinygltf::Model& model)
{
    m_textures.reserve(model.textures.size());

    for (auto i = 0; i < model.textures.size(); ++i)
    {
        const auto& modelTexture = model.textures[i];
        const auto& source = model.images[modelTexture.source];

        if (source.uri.find("data:image/") != std::string::npos)
        {
            JDL_WARN("Embedded images are not supported for now");
            m_textures.push_back(nullptr);
            continue;
        }

        std::string textureName = s_GetName(modelTexture, "Texture", i);
        auto texture = resource::ResourceManager::Create<resource::Texture>(textureName, source.uri);

        JDL_DEBUG("[IMPORTER] Import texture {}", texture->getName());
        m_textures.push_back(texture);
    }
}

void ModelImporter::importMaterials(const tinygltf::Model& model)
{
    m_materials.reserve(model.materials.size());

    for (auto i = 0; i < model.materials.size(); ++i)
    {
        const auto& modelMaterial = model.materials[i];
        const auto& pbrMaterial = modelMaterial.pbrMetallicRoughness;

        std::string materialName = s_GetName(modelMaterial, "Material", i);
        auto material = resource::ResourceManager::Create<resource::Material>(materialName);

        JDL_DEBUG("[IMPORTER] Import material {}", material->getName());
        m_materials.push_back(material);

        material->setBaseColorFactor(glm::make_vec4(pbrMaterial.baseColorFactor.data()));
        if (pbrMaterial.baseColorTexture.index >= 0)
        {
            material->setBaseColorTexture(m_textures[pbrMaterial.baseColorTexture.index]);
        }
    }
}

void ModelImporter::importMeshes(const tinygltf::Model& model)
{
    m_meshes.reserve(model.meshes.size());

    for (auto i = 0; i < model.meshes.size(); ++i)
    {
        const auto& modelMesh = model.meshes[i];
        std::string meshName = s_GetName(modelMesh, "Mesh", i);

        std::vector<resource::Mesh*> primitives;
        primitives.reserve(modelMesh.primitives.size());

        // Import the model mesh primitives
        for (auto j = 0; j < modelMesh.primitives.size(); ++j)
        {
            const auto& modelPrimitive = modelMesh.primitives[j];
            std::string primitiveName = meshName + std::to_string(j);

            auto mesh = resource::ResourceManager::Create<resource::Mesh>(
                primitiveName,
                static_cast<resource::PrimitiveType>(modelPrimitive.mode)
            );
            primitives.push_back(mesh);
            JDL_DEBUG("[IMPORTER] Import mesh {}", mesh->getName());

            // Retrieve the vertex attributes pointers
            const float* posPtr = s_GetAttributePtr(model, modelPrimitive, "POSITION");
            const float* normalPtr = s_GetAttributePtr(model, modelPrimitive, "NORMAL");
            const float* uvPtr = s_GetAttributePtr(model, modelPrimitive, "TEXCOORD_0");
           
            // Create the mesh vertices
            size_t nbVertices = model.accessors[modelPrimitive.attributes.at("POSITION")].count;
            std::vector<core::Vertex> vertices(nbVertices);
            for (auto v = 0; v < nbVertices; ++v)
            {
                vertices[v].position = glm::make_vec3(&posPtr[3 * v]);
                if (normalPtr != nullptr)
                {
                    vertices[v].normal = glm::make_vec3(&normalPtr[3 * v]);
                }
                if (uvPtr != nullptr)
                {
                    vertices[v].uv = glm::make_vec2(&uvPtr[2 * v]);
                }
            }
            mesh->addVertices(vertices);

            // Read and create the mesh indices
            if (modelPrimitive.indices >= 0)
            {
                const auto& accessor = model.accessors[modelPrimitive.indices];
                const auto& view = model.bufferViews[accessor.bufferView];
                const auto& buffer = model.buffers[view.buffer];

                std::vector<uint32_t> indices;
                indices.reserve(accessor.count);

                const void* dataPtr = &buffer.data[view.byteOffset + accessor.byteOffset];
                switch (accessor.componentType)
                {
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
                    {
                        const uint32_t* indexPtr = static_cast<const uint32_t*>(dataPtr);
                        indices.insert(indices.end(), indexPtr, indexPtr + accessor.count);
                        break;
                    }
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
                    {
                        const uint16_t* indexPtr = static_cast<const uint16_t*>(dataPtr);
                        indices.insert(indices.end(), indexPtr, indexPtr + accessor.count);
                        break;
                    }
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
                    {
                        const uint8_t* indexPtr = static_cast<const uint8_t*>(dataPtr);
                        indices.insert(indices.end(), indexPtr, indexPtr + accessor.count);
                        break;
                    }
                    default:
                        break;
                }
                mesh->addIndices(indices);
            }

            // Set the mesh material
            if (modelPrimitive.material >= 0)
            {
                mesh->setMaterial(m_materials[modelPrimitive.material]);
            }
        }
        m_meshes.emplace_back(std::move(primitives));
    }
}

void ModelImporter::importNodes(const tinygltf::Model& model)
{
    m_nodes.reserve(model.nodes.size());

    // Import nodes
    for (auto i = 0; i < model.nodes.size(); ++i)
    {
        const auto& modelNode = model.nodes[i];
        std::string nodeName = s_GetName(modelNode, "Node", i);

        scene::SceneNodePtr node = nullptr;
        if (modelNode.mesh >= 0)
        {
            auto renderable = scene::CreateNode<scene::RenderableNode>(nodeName, nullptr);
            for (resource::Mesh* mesh : m_meshes[modelNode.mesh])
            {
                renderable->addMesh(mesh);
            }
            node = renderable;
        }
        else
        {
            node = scene::CreateNode<scene::SceneNode>(nodeName, nullptr);
        }

        JDL_DEBUG("[IMPORTER] Import node {}", node->getName());
        m_nodes.push_back(node);
    }

    // Setup the parent/child relationships
    for (auto i = 0; i < model.nodes.size(); ++i)
    {
        const scene::SceneNodePtr& node = m_nodes[i];
        for (int childIndex : model.nodes[i].children)
        {
            node->addChild(m_nodes[childIndex]);
        }
    }
}

void ModelImporter::clear()
{
    m_materials.clear();
    m_textures.clear();
    m_meshes.clear();
    m_nodes.clear();
}

} // namespace io
} // namespace jdl
