#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"


namespace jdl
{
namespace scene
{

Scene::Scene(const std::string& name) : core::Object(name)
{
    // Create a default camera
    m_camera = resource::ResourceManager::Create<resource::OrbitCamera>(name + "_CAM");
}

void Scene::setCamera(resource::Camera* camera)
{
    m_camera = camera;
    m_cameraDirty = true;
}

void Scene::render(core::RenderContext& context)
{
    if (m_cameraDirty)
    {
        // Update the uniform buffer with the camera new data.
        m_uniformBuffer.viewMatrix = m_camera->getViewMatrix();
        m_uniformBuffer.projectionMatrix = m_camera->getProjectionMatrix();
        m_uniformBuffer.cameraPosition = m_camera->getPosition();

        m_cameraDirty = false;
    }
    context.globalUniformBuffer.lock()->setData(&m_uniformBuffer);

    // Bind the global uniform buffer set
    vkCmdBindDescriptorSets(
        context.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        context.pipelineLayout,
        0, 1, &context.globalDescriptorSet,
        0, nullptr
    );

    if (m_rootNode) {
        m_rootNode->render(context);
    }
}

} // namespace scene
} // namespace jdl
