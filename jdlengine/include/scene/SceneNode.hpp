#pragma once

#include "core/Object.hpp"
#include "core/RenderContext.hpp"

#include "math/SRTMatrix.hpp"


namespace jdl
{
namespace scene
{

class JDL_API SceneNode : public core::Object
{
public:
    /**
     * @brief Creates the scene node.
     * @param name Node name
     */
    SceneNode(const std::string& name);

    /**
     * @brief Adds a new child node.
     * @param child Child node
     */
    void addChild(const std::shared_ptr<SceneNode>& child);

    /**
     * @brief  Finds and returns a child node.
     * @param  name Child node name
     * @return Child node, or nullptr if it does not exist.
     */
    std::shared_ptr<SceneNode> getChild(const std::string& name) const;

    /**
     * @brief Removes a child node.
     * @param name Child node name
     */
    void removeChild(const std::string& name);

    /**
     * @brief Returns the node parent.
     */
    std::shared_ptr<SceneNode> getParent() const { return m_parent.lock(); }

    /**
     * @brief Sets the node parent.
     * @param parent The new parent
     */
    void setParent(const std::shared_ptr<SceneNode>& parent);

    /**
     * @brief Returns the node path, i.e. the concatenation of the nodes names from the root.
     */
    std::string getPath() const;

    /**
     * @brief  Finds and returns a node from its path.
     * @param  path Node path, from the root
     * @return Queried node or nullptr if it does not exist
     */
    std::shared_ptr<SceneNode> find(const std::string& path);

    /**
     * @brief Returns whether the node is visible or not.
     */
    bool isVisible() const { return m_visible; }

    /**
     * @brief Sets whether the node should be visible or not. This also affects the node descendents.
     */
    void setVisible(bool visible) { m_visible = visible; }

    /**
     * @brief Returns the node world transform.
     */
    const math::SRTMatrix& getWorldTransform();

    /**
     * @brief Returns the node parent transform.
     */
    const math::SRTMatrix& getParentTransform() const { return m_parentTransform; }

    /**
     * @brief Sets the node world transform.
     * @param transform The new transform
     */
    void setWorldTransform(const math::SRTMatrix& transform);
    void setWorldTransform(const glm::mat4& transform) { setWorldTransform(math::SRTMatrix(transform)); }

    /**
     * @brief Sets the node parent transform.
     * @param transform The new transform
     */
    void setParentTransform(const math::SRTMatrix& transform);
    void setParentTransform(const glm::mat4& transform) { setParentTransform(math::SRTMatrix(transform)); }

    /**
     * @brief Renders the node and its descendents.
     * @param context Render context
     */
    void render(core::RenderContext& context);

    /**
     * @brief Node path separator.
     */
    static const std::string kPathSeparator;

protected:
    /**
     * @brief Renders the node content. Must be reimplemented if necessary.
     * @param context Render context
     */
    virtual void doRender(core::RenderContext& context) {}

private:
    // Node Hierarchy
    std::unordered_map<std::string, std::shared_ptr<SceneNode>> m_children;
    std::weak_ptr<SceneNode> m_parent;

    // Visibility state
    bool m_visible;

    // Node transforms
    math::SRTMatrix m_worldTransform;
    math::SRTMatrix m_parentTransform;
    bool m_worldTransformDirty;

    // Sets the node world transform as dirty
    void setWorldTransformDirty();
};

using SceneNodePtr = std::shared_ptr<SceneNode>;


/**
 * @brief  Creates a new node.
 * @param  name   Node name
 * @param  parent Node parent (can be nullptr)
 * @param  args   Node initialization arguments
 * @return The created node
 */
template<class N, typename... Args>
std::shared_ptr<N> CreateNode(const std::string& name, const std::shared_ptr<SceneNode>& parent, Args&&... args)
{
    // Find a valid name for the new node
    std::string validName = name;
    if (parent)
    {
        size_t index = 1;
        while (parent->getChild(validName))
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
