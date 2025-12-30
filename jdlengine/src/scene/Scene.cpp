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

void Scene::mousePressEvent(const core::MousePressEvent& event) {}

void Scene::mouseReleaseEvent(const core::MouseReleaseEvent& event) {}

void Scene::mouseMoveEvent(const core::MouseMoveEvent& event)
{
    if (event.getButton() != core::MouseButton::eLeftButton) {
        return;
    }

    auto camera = dynamic_cast<resource::OrbitCamera*>(m_camera);
    if (camera)
    {
        camera->updateRotation(event.getDeltaX(), event.getDeltaY());
        m_cameraDirty = true;
    }
}

void Scene::wheelEvent(const core::WheelEvent& event)
{
    auto camera = dynamic_cast<resource::OrbitCamera*>(m_camera);
    if (camera)
    {
        camera->setRadius(camera->getRadius() - event.getDeltaY());
        m_cameraDirty = true;
    }
}

void Scene::resizeEvent(const core::ResizeEvent& event)
{
    auto camera = dynamic_cast<resource::PerspectiveCamera*>(m_camera);
    if (camera)
    {
        camera->setAspectRatio(static_cast<float>(event.getWidth()) / event.getHeight());
        m_cameraDirty = true;
    }
}

} // namespace scene
} // namespace jdl
