#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"


namespace jdl
{
namespace scene
{

Scene::Scene(const std::string& name)
    : core::Object(name)
    , m_rootNode(nullptr)
{}

void Scene::render(core::RenderContext& context)
{
    if (m_rootNode != nullptr)
    {
        if (m_camera)
        {
            context.shaderProgram->setUniform("V", m_camera->getViewMatrix());
            context.shaderProgram->setUniform("P", m_camera->getProjectionMatrix());
        }
        m_rootNode->render(context);
    }
}

void Scene::mousePressEvent(const core::MousePressEvent& event) {}

void Scene::mouseMoveEvent(const core::MouseMoveEvent& event)
{
    if (event.getButton() == core::MouseButton::kLeft)
    {
        auto orbitCamera = dynamic_cast<resource::OrbitCamera*>(m_camera);
        if (orbitCamera)
        {
            const float increment = glm::radians(1.0f);

            orbitCamera->setAzimuth(orbitCamera->getAzimuth() + increment * event.getDeltaX());
            orbitCamera->setPolar(orbitCamera->getPolar() + increment * event.getDeltaY());
        }
    }
}

void Scene::mouseReleaseEvent(const core::MouseReleaseEvent& event) {}

void Scene::wheelEvent(const core::WheelEvent& event)
{
    auto orbitCamera = dynamic_cast<resource::OrbitCamera*>(m_camera);
    if (orbitCamera)
    {
        orbitCamera->setRadius(orbitCamera->getRadius() - event.getDeltaY());
    }
}

void Scene::resizeEvent(const core::ResizeEvent& event)
{
    auto perspCamera = dynamic_cast<resource::PerspectiveCamera*>(m_camera);
    if (perspCamera)
    {
        float ratio = static_cast<float>(event.getWidth()) / event.getHeight();
        perspCamera->setAspectRatio(ratio);
    }
}

} // namespace scene
} // namespace jdl
