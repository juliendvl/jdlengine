#pragma once

#include "core/Core.hpp"

#include "scene/SceneNode.hpp"

#include "utils/NonCopyable.hpp"


namespace tinygltf
{
    class Model;
} // namespace tinygltf


namespace jdl
{

namespace resource
{
    class Material;
    class Mesh;
    class Texture;
} // namespace scene

namespace io
{

class JDL_API ModelImporter : public NonCopyable<ModelImporter>
{
public:
    /**
     * @brief Imports a GLTF model.
     * @param path   Model file path
     * @param parent Model root parent (optional)
     * @return The model root node
     */
    static scene::SceneNodePtr Import(const std::string& path, const scene::SceneNodePtr& parent = nullptr) {
        return Importer.doImport(path, parent);
    }

private:
    // Unique instance
    static ModelImporter Importer;

    // Internal data
    std::vector<resource::Texture*>           m_textures;
    std::vector<resource::Material*>          m_materials;
    std::vector<std::vector<resource::Mesh*>> m_meshes;
    std::vector<scene::SceneNodePtr>          m_nodes;

    scene::SceneNodePtr doImport(const std::string& path, const scene::SceneNodePtr& parent);

    // Imports the model textures
    void importTextures(const tinygltf::Model& model);
    // Imports the model materials
    void importMaterials(const tinygltf::Model& model);
    // Imports the model meshes
    void importMeshes(const tinygltf::Model& model);
    // Imports the model nodes
    void importNodes(const tinygltf::Model& model);
    // Clears the importer internal data
    void clear();
};

} // namespace io
} // namespace jdl
