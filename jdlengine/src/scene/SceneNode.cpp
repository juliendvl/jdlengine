#include "scene/SceneNode.hpp"

#include "utils/Logger.hpp"
#include "utils/StringUtils.hpp"


namespace jdl
{
namespace scene
{

const std::string SceneNode::PATH_SEPARATOR = "|";

SceneNode::SceneNode(const std::string& name) : core::Object(name) {}

void SceneNode::addChild(const std::shared_ptr<SceneNode>& child)
{
    const std::string& childName = child->getName();
    if (getChild(childName) != nullptr)
    {
        JDL_ERROR("Node {} already has a child named {}", getName(), childName);
        return;
    }

    auto oldParent = child->getParent();
    if (oldParent) {
        oldParent->removeChild(childName);
    }

    m_children[childName] = child;
    child->m_parent = std::static_pointer_cast<SceneNode>(getPointer());
}

std::shared_ptr<SceneNode> SceneNode::getChild(const std::string& name) const
{
    auto it = m_children.find(name);
    return it != m_children.end() ? it->second : nullptr;
}

void SceneNode::removeChild(const std::string& name)
{
    auto it = m_children.find(name);
    if (it == m_children.end()) {
        return;
    }

    it->second->m_parent.reset();
    m_children.erase(it);
}

void SceneNode::setParent(const std::shared_ptr<SceneNode>& parent)
{
    const std::string& nodeName = getName();
    if (parent && parent->getChild(nodeName))
    {
        JDL_ERROR("Node {} already has a child named {}", parent->getName(), nodeName);
        return;
    }

    auto oldParent = getParent();
    if (oldParent) {
        oldParent->removeChild(nodeName);
    }

    parent->m_children[nodeName] = std::static_pointer_cast<SceneNode>(getPointer());
    m_parent = parent;
}

std::string SceneNode::getPath() const
{
    std::vector<std::string> nodes { getName() };

    auto current = getParent();
    while (current)
    {
        nodes.push_back(current->getName());
        current = current->getParent();
    }

    return utils::StringUtils::Join(nodes.rbegin(), nodes.rend(), PATH_SEPARATOR);
}

std::shared_ptr<SceneNode> SceneNode::findNode(const std::string& path)
{
    if (path == "") {
        return nullptr;
    }
    auto nodes = utils::StringUtils::Split(path, PATH_SEPARATOR);

    std::shared_ptr<SceneNode> current = std::static_pointer_cast<SceneNode>(getPointer());
    for (const auto& node : nodes)
    {
        current = current->getChild(node);
        if (!current) {
            return nullptr;
        }
    }

    return current;
}

const math::SRTMatrix& SceneNode::getWorldTransform()
{
    if (m_worldTransformDirty)
    {
        auto parent = getParent();
        m_worldTransform = parent ? parent->getWorldTransform() * m_parentTransform : m_parentTransform;
        m_worldTransformDirty = false;
    }

    return m_worldTransform;
}

void SceneNode::setWorldTransform(const math::SRTMatrix& transform)
{
    m_worldTransform = transform;

    // Update the parent transform
    auto parent = getParent();
    m_parentTransform = parent ? parent->getWorldTransform().getInverse() * m_worldTransform : m_worldTransform;

    // The descendents world transform are now dirty...
    for (const auto& [name, child] : m_children) {
        child->setWorldTransformDirty();
    }
}

void SceneNode::setParentTransform(const math::SRTMatrix& transform)
{
    m_parentTransform = transform;
    setWorldTransformDirty();
}

void SceneNode::render(core::RenderContext& context)
{
    if (!m_visible) {
        return;
    }

    doRender(context);
    for (const auto& [name, child] : m_children) {
        child->render(context);
    }
}

void SceneNode::setWorldTransformDirty()
{
    m_worldTransformDirty = true;
    for (const auto& [name, child] : m_children) {
        child->setWorldTransformDirty();
    }
}

} // namespace scene
} // namespace jdl
