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
{}

void SceneNode::addChild(const std::shared_ptr<SceneNode>& child)
{
    std::string childName = child->getName();
    if (getChild(childName) != nullptr)
    {
        JDL_ERROR("Node {} already has a child named {}", getName(), childName);
        return;
    }

    auto parent = child->getParent();
    if (parent != nullptr)
    {
        parent->removeChild(childName);
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
    if (it != m_children.end())
    {
        auto child = it->second;
        child->m_parent.reset();

        m_children.erase(it);
    }
}

void SceneNode::setParent(const std::shared_ptr<SceneNode>& parent)
{
    std::string nodeName = getName();

    // Check whether the parent can add the node as its child
    if (parent)
    {
        if (parent->getChild(nodeName))
        {
            JDL_ERROR("Parent {} already has a child named {}", parent->getName(), nodeName);
            return;
        }
    }

    auto oldParent = getParent();
    if (oldParent)
    {
        oldParent->removeChild(nodeName);
    }
    if (parent)
    {
        parent->addChild(std::static_pointer_cast<SceneNode>(getPointer()));
    }
}

std::string SceneNode::getPath() const
{
    std::vector<std::string> components {getName()};

    auto parent = getParent();
    while (parent)
    {
        components.push_back(parent->getName());
        parent = parent->getParent();
    }

    return utils::StringUtils::Join(components.rbegin(), components.rend(), kPathSeparator);
}

std::shared_ptr<SceneNode> SceneNode::find(const std::string& path)
{
    auto components = utils::StringUtils::Split(path, kPathSeparator);
    if (components.empty())
    {
        return nullptr;
    }

    auto node = std::static_pointer_cast<SceneNode>(getPointer());
    for (const std::string& childName : components)
    {
        node = node->getChild(childName);
        if (node == nullptr)
        {
            return nullptr;
        }
    }
    return node;
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

    // Update the parent transform
    auto parent = getParent();
    m_parentTransform = parent ? parent->getWorldTransform().inverse() * transform : m_worldTransform;

    // Set the descendents world transform as dirty
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

void SceneNode::render(core::RenderContext& context)
{
    if (m_visible)
    {
        doRender(context);
        for (const auto& it : m_children)
        {
            it.second->render(context);
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
