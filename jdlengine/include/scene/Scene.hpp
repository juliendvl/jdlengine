#pragma once

#include "core/Object.hpp"

#include "SceneNode.hpp"

#include "core/Events.hpp"
#include "core/RenderContext.hpp"


namespace jdl
{
namespace scene
{

class JDL_API Scene : public core::Object
{
public:
    /**
     * @brief Creates the scene.
     * @param name Scene name.
     */
    Scene(const std::string& name);

    /**
     * @brief Returns the scene root node.
     */
    const SceneNodePtr& getRootNode() {
        return m_rootNode;
    }

    /**
     * @brief Sets the scene root node.
     * @param rootNode The new root node
     */
    void setRootNode(const SceneNodePtr& rootNode) {
        m_rootNode = rootNode;
    }

    /**
     * @brief Renders the scene content.
     * @param context Render context
     */
    void render(const core::RenderContext& context);

    /**
     * @brief Resize event.
     * @param event Event data.
     */
    void resizeEvent(const core::ResizeEvent& event);

private:
    SceneNodePtr m_rootNode;
};

using ScenePtr = std::shared_ptr<Scene>;

} // namespace scene
} // namespace jdl
