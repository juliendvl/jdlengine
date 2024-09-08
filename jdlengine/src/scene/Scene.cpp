#include "scene/Scene.hpp"


namespace jdl
{
namespace scene
{

Scene::Scene(const std::string& name)
    : core::Object(name)
    , m_rootNode(nullptr)
{}

void Scene::render()
{
    if (m_rootNode)
    {
        m_rootNode->render();
    }
}

void Scene::resizeEvent(const core::ResizeEvent& event) {}

} // namespace scene
} // namespace jdl
