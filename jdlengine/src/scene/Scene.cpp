#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"

#include "core/Renderer.hpp"


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
    if (m_rootNode)
    {
        m_rootNode->render(context);
    }
}

void Scene::resizeEvent(const core::ResizeEvent& event)
{
}

} // namespace scene
} // namespace jdl
