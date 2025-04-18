#pragma once

#include "core/Object.hpp"
#include "core/Renderer.hpp"

#include "math/SRTMatrix.hpp"


namespace jdl
{
namespace scene
{

class JDL_API SceneNode : public core::Object
{
public:
    /**
     * @brief Creates the node.
     * @param name Node name
     */
    SceneNode(const std::string& name);

    /**
     * @brief Adds a child node.
     * @param child The child node
     */
    void addChild(const std::shared_ptr<SceneNode>& child);

    /**
     * @brief  Finds and returns a child node.
     * @param  name Child name
     * @return Child node, or nullptr if it does not exist.
     */
    std::shared_ptr<SceneNode> getChild(const std::string& name) const;

    /**
     * @brief Removes a child node.
     * @param name Child name
     */
    void removeChild(const std::string& name);

    /**
     * @brief Returns the node parent.
     */
    std::shared_ptr<SceneNode> getParent() const {
        return m_parent.lock();
    }

    /**
     * @brief Sets the node parent.
     * @param parent The new parent
     */
    void setParent(const std::shared_ptr<SceneNode>& parent);

    /**
     * @brief Returns the node path, from the scene node hierarchy root.
     */
    std::string getPath() const;

    /**
     * @brief  Finds a node according to its relative path from the current node.
     * @param  path Required node path (relative to the current node)
     * @return Queried node, or nullptr if no such node exists.
     */
    std::shared_ptr<SceneNode> findNode(const std::string& path);

    /**
     * @brief Returns the node world transform.
     */
    const math::SRTMatrix& getWorldTransform();

    /**
     * @brief Sets the node world transform.
     * @param transform The new transform
     */
    void setWorldTransform(const math::SRTMatrix& transform);
    void setWorldTransform(const glm::mat4& transform) { setWorldTransform(math::SRTMatrix(transform)); }

    /**
     * @brief Returns the node parent transform.
     */
    const math::SRTMatrix& getParentTransform() const { return m_parentTransform; }

    /**
     * @brief Sets the node parent transform.
     * @param transform The new transform
     */
    void setParentTransform(const math::SRTMatrix& transform);
    void setParentTransform(const glm::mat4& transform) { setParentTransform(math::SRTMatrix(transform)); }

    /**
     * @brief Returns whether the node (and its descendents) are visible or not.
     */
    bool isVisible() const { return m_visible; }

    /**
     * @brief Sets whether the node (and its descendents) should be visible or not.
     * @param visible The new visibility state
     */
    void setVisible(bool visible) { m_visible = visible; }

    /**
     * @brief Renders the node content.
     * @param context   Render context
     * @param recursive Render the node descendents or not
     */
    void render(core::RenderContext& context, bool recursive = true);

    /**
     * @brief Node path separator.
     */
    static const std::string kPathSeparator;

protected:
    /**
     * @brief Renders the node content. Can be reimplemented if necessary
     * @param context Render context
     */
    virtual void doRender(core::RenderContext& context) {}

private:
    std::unordered_map<std::string, std::shared_ptr<SceneNode>> m_children;
    std::weak_ptr<SceneNode> m_parent;

    math::SRTMatrix m_worldTransform;
    math::SRTMatrix m_parentTransform;
    bool m_worldTransformDirty;

    bool m_visible;

    // Sets the node (and its descendents) world transforms as dirty
    void setWorldTransformDirty();
};

using SceneNodePtr = std::shared_ptr<SceneNode>;


/**
 * @brief  Creates and returns a new scene node.
 * @param  name   Node name
 * @param  parent Node parent
 * @param  args   Node initialization arguments
 */
template<class N, typename... Args>
std::shared_ptr<N> CreateNode(const std::string& name, const std::shared_ptr<SceneNode>& parent, Args&&... args)
{
    // Find a valid name for the new node
    std::string validName = name;
    if (parent)
    {
        size_t index = 1;
        while (parent->getChild(validName) != nullptr)
        {
            validName = name + std::to_string(index++);
        }
    }

    auto node = std::make_shared<N>(validName, std::forward<Args>(args)...);
    if (parent)
    {
        parent->addChild(node);
    }
    return node;
}

} // namespace scene
} // namespace jdl
