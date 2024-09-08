#pragma once

#include "core/Object.hpp"

#include <unordered_map>

#include "math/SRTMatrix.hpp"


namespace jdl
{
namespace scene
{

class JDL_API SceneNode : public core::Object
{
public:
    /**
     * @brief Base constructor.
     * @param name Node name
     */
    SceneNode(const std::string& name);

    /**
     * @brief Adds a new child node.
     * @param child The new child node
     */
    void addChild(const std::shared_ptr<SceneNode>& child);

    /**
     * @brief Finds and returns a child node.
     * @param name Child name
     * @return Child node, or nullptr if it does not exist. 
     */
    std::shared_ptr<SceneNode> getChild(const std::string& name) const;

    /**
     * @brief Removes a child node.
     * @param name Child name
     * @return The deleted node if any
     */
    std::shared_ptr<SceneNode> removeChild(const std::string& name);

    /**
     * @brief Returns the parent node if any.
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
     * @brief Returns a string representation of the path from the root node to this node.
     */
    std::string getPath() const;

    /**
     * @brief Finds and returns a node from its path relative to the current node.
     * @param path Node path relative to the current node
     * @return Target node, or nullptr if it does not exist.
     */
    std::shared_ptr<SceneNode> findNode(const std::string& path);

    /**
     * @brief Returns whether the node and its descendents are visible or not.
     */
    bool isVisible() const {
        return m_visible;
    }

    /**
     * @brief Sets the node visibility state.
     * @param visible New state
     */
    void setVisible(bool visible) {
        m_visible = visible;
    }

    /**
     * @brief Returns the node world transform.
     */
    const math::SRTMatrix& getWorldTransform();

    /**
     * @brief Returns the node parent transform.
     */
    const math::SRTMatrix& getParentTransform() const {
        return m_parentTransform;
    }

    /**
     * @brief Sets the node world transform.
     * @param transform The new world transform
     */
    void setWorldTransform(const math::SRTMatrix& transform);
    void setWorldTransform(const glm::mat4& transform) {
        setWorldTransform(math::SRTMatrix(transform));
    }

    /**
     * @brief Sets the node parent transform.
     * @param transform The new parent transform
     */
    void setParentTransform(const math::SRTMatrix& transform);
    void setParentTransform(const glm::mat4& transform) {
        setParentTransform(math::SRTMatrix(transform));
    }

    /**
     * @brief Renders the node.
     */
    void render();

    /**
     * @brief Node path separator.
     */
    static const std::string kPathSeparator;

protected:
    /**
     * @brief Renders the node content. Must be reimplemented if necessary.
     */
    virtual void doRender() {}

private:
    // Children nodes
    std::unordered_map<std::string, std::shared_ptr<SceneNode>> m_children;
    // Parent node (weak reference to avoid circular dependencies)
    std::weak_ptr<SceneNode> m_parent;
    // Visibility state
    bool m_visible;
    // Transforms (world and parent)
    math::SRTMatrix m_worldTransform;
    math::SRTMatrix m_parentTransform;
    // World transform validity state
    bool m_worldTransformDirty;

    // Propagates the world transform dirtiness state to the node and its children
    void setWorldTransformDirty();
};

using SceneNodePtr = std::shared_ptr<SceneNode>;

/**
 * @brief Creates and returns a new scene node.
 * @param name   Node name
 * @param parent Node parent
 * @param args   Node initialization arguments
 */
template<class N, typename... Args>
std::shared_ptr<N> CreateNode(const std::string& name, const std::shared_ptr<SceneNode>& parent, Args&&... args)
{
    std::string nodeName = name;

    // Find a valid name for the new node
    if (parent)
    {
        size_t index = 1;

        while (parent->getChild(nodeName) != nullptr)
        {
            nodeName = name + "_" + std::to_string(index++);
        }
    }

    auto node = std::make_shared<N>(nodeName, std::forward<Args>(args)...);
    if (parent)
    {
        parent->addChild(node);
    }

    return node;
}

} // namespace scene
} // namespace jdl
