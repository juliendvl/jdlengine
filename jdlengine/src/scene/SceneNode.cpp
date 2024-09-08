#include "scene/SceneNode.hpp"

#include "utils/Logger.hpp"
#include "utils/StringUtils.hpp"


namespace jdl
{
namespace scene
{

const std::string SceneNode::kPathSeparator = "|";

SceneNode::SceneNode(const std::string& name)
    : core::Object(name)
    , m_visible(true)
    , m_worldTransformDirty(false)
{}

void SceneNode::addChild(const SceneNodePtr& child)
{
    const std::string& childName = child->getName();
    if (getChild(childName) != nullptr)
    {
        JDL_ERROR("Node '{}' already has a child named '{}'", getName(), childName);
        return;
    }

    // Remove the child from its old parent if any
    auto parent = child->getParent();
    if (parent != nullptr)
    {
        parent->removeChild(childName);
    }

    m_children[childName] = child;
    child->m_parent = std::static_pointer_cast<SceneNode>(getPointer());
}

SceneNodePtr SceneNode::getChild(const std::string& name) const
{
    auto it = m_children.find(name);
    return it != m_children.end() ? it->second : nullptr;
}

SceneNodePtr SceneNode::removeChild(const std::string& name)
{
    auto it = m_children.find(name);
    
    if (it != m_children.end())
    {
        auto child = it->second;

        m_children.erase(it);
        child->m_parent.reset();

        return child;
    }

    return nullptr;
}

void SceneNode::setParent(const SceneNodePtr& parent)
{
    if (parent)
    {
        const std::string& nodeName = getName();
        if (parent->getChild(nodeName) != nullptr)
        {
            JDL_ERROR("Cannot reparent: parent '{}' already has a child named '{}'", parent->getName(), nodeName);
            return;
        }
    }

    auto oldParent = getParent();
    if (oldParent)
    {
        oldParent->removeChild(getName());
    }

    if (parent)
    {
        parent->addChild(std::static_pointer_cast<SceneNode>(getPointer()));
    }
}

std::string SceneNode::getPath() const
{
    std::vector<std::string> pathNodes{getName()};

    auto current = getParent();
    while (current)
    {
        pathNodes.push_back(current->getName());
        current = current->getParent();
    }

    return utils::StringUtils::Join(pathNodes.rbegin(), pathNodes.rend(), kPathSeparator);
}

SceneNodePtr SceneNode::findNode(const std::string& path)
{
    std::vector<std::string> pathNodes = utils::StringUtils::Split(path, kPathSeparator);
    if (pathNodes.empty())
    {
        return nullptr;
    }

    auto current = std::static_pointer_cast<SceneNode>(getPointer());
    for (const std::string& nodeName : pathNodes)
    {
        current = current->getChild(nodeName);
        if (current == nullptr)
        {
            return nullptr;
        }
    }

    return current;
}

const math::SRTMatrix& SceneNode::getWorldTransform()
{
    if (m_worldTransformDirty)
    {
        m_worldTransformDirty = false;
        
        auto parent = getParent();
        m_worldTransform = parent ? parent->getWorldTransform() * m_parentTransform : m_parentTransform;
    }
    return m_worldTransform;
}

void SceneNode::setWorldTransform(const math::SRTMatrix& transform)
{
    m_worldTransform = transform;

    // Compute the new parent transform
    auto parent = getParent();
    m_parentTransform = parent ? parent->getWorldTransform().inverse() * m_worldTransform : m_worldTransform;

    setWorldTransformDirty();
}

void SceneNode::setParentTransform(const math::SRTMatrix& transform)
{
    m_parentTransform = transform;
    setWorldTransformDirty();
}

void SceneNode::render()
{
    if (m_visible)
    {
        doRender();
        for (const auto& [name, child] : m_children)
        {
            child->render();
        }
    }
}

void SceneNode::setWorldTransformDirty()
{
    m_worldTransformDirty = true;
    for (const auto& [name, child] : m_children)
    {
        child->setWorldTransformDirty();
    }
}

} // namespace scene
} // namespace jdl
