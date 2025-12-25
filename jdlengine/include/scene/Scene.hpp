#pragma once

#include "core/Object.hpp"
#include "core/RenderContext.hpp"


namespace jdl
{
namespace scene
{

class SceneNode;

class JDL_API Scene : public core::Object
{
public:
    /**
     * @brief Creates the scene.
     * @param name The scene name.
     */
    Scene(const std::string& name);

    /**
     * @brief Returns the scene root node.
     */
    const std::shared_ptr<SceneNode>& getRootNode() const {
        return m_rootNode;
    }

    /**
     * @brief Sets the scene root node.
     * @param rootNode The new root node.
     */
    void setRootNode(const std::shared_ptr<SceneNode>& rootNode) {
        m_rootNode = rootNode;
    }

    /**
     * @brief Renders the scene.
     * @param context Render context.
     */
    void render(core::RenderContext& context);

private:
    std::shared_ptr<SceneNode> m_rootNode;
};

} // namespace scene
} // namespace jdl
