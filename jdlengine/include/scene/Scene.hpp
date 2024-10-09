#pragma once

#include "core/Object.hpp"

#include "SceneNode.hpp"

#include "core/Events.hpp"
#include "core/RenderContext.hpp"

#include "resource/Camera.hpp"


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
     * @brief Sets the scene camera.
     * @param camera The new camera
     */
    void setCamera(resource::Camera* camera);

    /**
     * @brief Renders the scene content.
     * @param context Render context
     */
    void render(const core::RenderContext& context);

    /**
     * @brief Mouse press event.
     * @param event Event data.
     */
    void mousePressEvent(const core::MousePressEvent& event);

    /**
     * @brief Mouse move event.
     * @param event Event data.
     */
    void mouseMoveEvent(const core::MouseMoveEvent& event);

    /**
     * @brief Mouse release event.
     * @param event Event data.
     */
    void mouseReleaseEvent(const core::MouseReleaseEvent& event);

    /**
     * @brief Wheel event.
     * @param event Event data.
     */
    void wheelEvent(const core::WheelEvent& event);

    /**
     * @brief Resize event.
     * @param event Event data.
     */
    void resizeEvent(const core::ResizeEvent& event);

private:
    // Root node
    SceneNodePtr m_rootNode;
    // Camera
    resource::Camera* m_camera;
    bool m_reloadCamera;
    // Events internal data
    int m_lastX;
    int m_lastY;
};

using ScenePtr = std::shared_ptr<Scene>;

} // namespace scene
} // namespace jdl
