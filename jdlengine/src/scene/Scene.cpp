#include "scene/Scene.hpp"


namespace jdl
{
namespace scene
{

Scene::Scene(const std::string& name)
    : core::Object(name)
    , m_rootNode(nullptr)
    , m_camera(nullptr)
    , m_reloadCamera(true)
    , m_lastX(0)
    , m_lastY(0)
{}

void Scene::setCamera(resource::Camera* camera)
{
    m_camera = camera;
    m_reloadCamera = true;
}

void Scene::render(const core::RenderContext& context)
{
    if (m_reloadCamera && m_camera != nullptr)
    {
        context.shaderProgram->setUniform("V", m_camera->getViewMatrix());
        context.shaderProgram->setUniform("P", m_camera->getProjectionMatrix());
    }
    if (m_rootNode)
    {
        m_rootNode->render(context);
    }
}

void Scene::mousePressEvent(const core::MousePressEvent& event)
{
    m_lastX = event.getX();
    m_lastY = event.getY();
}

void Scene::mouseMoveEvent(const core::MouseMoveEvent& event)
{
    if (m_camera && event.getButton() == core::MouseButton::kLeftButton)
    {
        int deltaX = event.getX() - m_lastX;
        int deltaY = event.getY() - m_lastY;

        if (auto orbitCamera = dynamic_cast<resource::OrbitCamera*>(m_camera))
        {
            orbitCamera->setTheta(orbitCamera->getTheta() + 0.01f * deltaY);
            orbitCamera->setPhi(orbitCamera->getPhi() + 0.01f * deltaX);
        }
    }

    // Update the last X/Y position
    m_lastX = event.getX();
    m_lastY = event.getY();
}

void Scene::mouseReleaseEvent(const core::MouseReleaseEvent& event) {}

void Scene::wheelEvent(const core::WheelEvent& event)
{
    if (auto orbitCamera = dynamic_cast<resource::OrbitCamera*>(m_camera))
    {
        orbitCamera->setRadius(orbitCamera->getRadius() - event.getDelta());
        m_reloadCamera = true;
    }
}

void Scene::resizeEvent(const core::ResizeEvent& event)
{
    if (auto perspCamera = dynamic_cast<resource::PerspectiveCamera*>(m_camera))
    {
        perspCamera->setAspectRatio(static_cast<float>(event.getWidth()) / event.getHeight());
        m_reloadCamera = true;
    }
}

} // namespace scene
} // namespace jdl
