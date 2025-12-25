#pragma once

#include "core/Object.hpp"
#include "core/RenderContext.hpp"

#include "math/SRTMatrix.hpp"

#include <unordered_map>


namespace jdl
{
namespace scene
{

class JDL_API SceneNode : public core::Object
{
public:
    /**
     * @brief Creates the node.
     * @param name The node name.
     */
    SceneNode(const std::string& name);

    /**
     * @brief Adds a new child node.
     * @param child The child node.
     */
    void addChild(const std::shared_ptr<SceneNode>& child);

    /**
     * @brief Finds and returns a child node.
     * @param name Child node name.
     * @return The desired child node, or nullptr if no such node exists.
     */
    std::shared_ptr<SceneNode> getChild(const std::string& name) const;

    /**
     * @brief Removes a child node.
     * @param name Child node name.
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
     * @param parent The new parent. Set to nullptr to unparent the node.
     */
    void setParent(const std::shared_ptr<SceneNode>& parent);

    /**
     * @brief Returns the node path (= concatenation of all the nodes from the root node).
     */
    std::string getPath() const;

    /**
     * @brief Finds a node from its relative path.
     * @param path Target path, relative to the current node.
     * @return The desired node, or nullptr if no such node exists.
     */
    std::shared_ptr<SceneNode> findNode(const std::string& path);

    /**
     * @brief Returns whether the node and its descendents are visible or not.
     */
    bool isVisible() const {
        return m_visible;
    }

    /**
     * @brief Sets the node (and descendents) visibility state.
     */
    void setVisible(bool visible) {
        m_visible = visible;
    }

    /**
     * @brief Returns the node world transform.
     */
    const math::SRTMatrix& getWorldTransform();

    /**
     * @brief Sets the node world transform.
     * @param transform The new transform.
     */
    void setWorldTransform(const math::SRTMatrix& transform);
    void setWorldTransform(const glm::mat4& transform) { setWorldTransform(math::SRTMatrix(transform)); }

    /**
     * @brief Returns the node parent transform.
     */
    const math::SRTMatrix& getParentTransform() const {
        return m_parentTransform;
    }

    /**
     * @brief Sets the node parent transform.
     * @param transform The new transform.
     */
    void setParentTransform(const math::SRTMatrix& transform);
    void setParentTransform(const glm::mat4& transform) { setParentTransform(math::SRTMatrix(transform)); }

    /**
     * @brief Renders the node.
     * @param context The render context.
     */
    void render(core::RenderContext& context);

    /**
     * @brief Node path separator.
     */
    static const std::string PATH_SEPARATOR;

protected:
    /**
     * @brief Renders the node content. Must be reimplemented if necessary.
     * @param context The render context.
     */
    virtual void doRender(core::RenderContext& context) {}

    /**
     * @brief Marks the node world transform as dirty, and propagates the state to the descendents.
     */
    void setWorldTransformDirty();

private:
    // Node hierarchy
    std::unordered_map<std::string, std::shared_ptr<SceneNode>> m_children;
    std::weak_ptr<SceneNode> m_parent;

    // Node visibility
    bool m_visible = true;

    // Node transforms
    math::SRTMatrix m_worldTransform;
    math::SRTMatrix m_parentTransform;
    bool m_worldTransformDirty = false;
};


/**
 * @brief Creates and returns a new scene node.
 * @param name      Node name.
 * @param parent    Node parent (optional).
 * @param args      Node initialization arguments.
 * @return The created node.
 */
template<class N, typename... Args>
std::shared_ptr<N> CreateNode(const std::string& name, const std::shared_ptr<SceneNode>& parent, Args&&... args)
{
    std::string validName = name;
    if (parent)
    {
        size_t index = 1;
        while (parent->getChild(validName)) {
            validName = name + std::to_string(index++);
        }
    }

    auto node = std::make_shared<N>(name, std::forward<Args>(args)...);
    if (parent) {
        parent->addChild(node);
    }

    return node;
}

} // namespace scene
} // namespace jdl
