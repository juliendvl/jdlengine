#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"


namespace jdl
{
namespace scene
{

Scene::Scene(const std::string& name) : core::Object(name) {}

void Scene::render(core::RenderContext& context)
{
    if (m_rootNode) {
        m_rootNode->render(context);
    }
}

} // namespace scene
} // namespace jdl
