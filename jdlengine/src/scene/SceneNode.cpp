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
        JDL_ERROR("Node {} already has a child named {}", getName(), childName);
        return;
    }

    auto parent = child->getParent();
    if (parent)
    {
        parent->removeChild(childName);
    }

    m_children[childName] = child;
    child->m_parent = std::static_pointer_cast<SceneNode>(getPointer());
    child->setWorldTransformDirty();
}

SceneNodePtr SceneNode::getChild(const std::string& name) const
{
    auto it = m_children.find(name);
    return it != m_children.end() ? it->second : nullptr;
}

void SceneNode::removeChild(const std::string& name)
{
    auto it = m_children.find(name);
    if (it != m_children.end())
    {
        m_children.erase(it);
        it->second->m_parent.reset();
        it->second->setWorldTransformDirty();
    }
}

void SceneNode::setParent(const SceneNodePtr& parent)
{
    const std::string& nodeName = getName();

    if (parent)
    {
        if (parent->getChild(nodeName) != nullptr)
        {
            JDL_ERROR("Parent {} already has a child named {}", parent->getName(), nodeName);
            return;
        }
    }

    auto oldParent = getParent();
    if (oldParent != nullptr)
    {
        oldParent->m_children.erase(nodeName);
        m_parent.reset();
    }

    if (parent)
    {
        parent->addChild(std::static_pointer_cast<SceneNode>(getPointer()));
    }
    else
    {
        setWorldTransformDirty();
    }
}

std::string SceneNode::getPath() const
{
    std::vector<std::string> nodes { getName() };

    auto current = getParent();
    while (current != nullptr)
    {
        nodes.push_back(current->getName());
        current = current->getParent();
    }

    return utils::StringUtils::Join(nodes.rbegin(), nodes.rend(), kPathSeparator);
}

SceneNodePtr SceneNode::findNode(const std::string& path)
{
    auto nodes = utils::StringUtils::Split(path, kPathSeparator);

    auto current = std::static_pointer_cast<SceneNode>(getPointer());
    for (const std::string& nodeName : nodes)
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

    // The world transform for the node descendents must be marked as dirty
    for (const auto& it : m_children)
    {
        it.second->setWorldTransformDirty();
    }
}

void SceneNode::setParentTransform(const math::SRTMatrix& transform)
{
    m_parentTransform = transform;
    setWorldTransformDirty();
}

void SceneNode::render(core::RenderContext& context, bool recursive)
{
    if (m_visible)
    {
        doRender(context);
        if (recursive)
        {
            for (const auto& it : m_children)
            {
                it.second->render(context);
            }
        }
    }
}

void SceneNode::setWorldTransformDirty()
{
    m_worldTransformDirty = true;
    for (const auto& it : m_children)
    {
        it.second->setWorldTransformDirty();
    }
}

} // namespace scene
} // namespace jdl
