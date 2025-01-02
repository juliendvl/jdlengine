#pragma once

#include "core/Object.hpp"
#include "core/Events.hpp"
#include "core/RenderContext.hpp"

#include "resource/Cameras.hpp"


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
     * @param name Scene name
     */
    Scene(const std::string& name);

    /**
     * @brief Returns the scene root node.
     */
    const std::shared_ptr<SceneNode> getRootNode() const { return m_rootNode; }

    /**
     * @brief Sets the scene root node.
     */
    void setRootNode(const std::shared_ptr<SceneNode>& rootNode) { m_rootNode = rootNode; }

    /**
     * @brief Returns the active camera.
     */
    resource::Camera* getCamera() const { return m_camera; }

    /**
     * @brief Sets the scene camera.
     * @param camera The new camera
     */
    void setCamera(resource::Camera* camera) { m_camera = camera; }

    /**
     * @brief Renders the scene content.
     * @param context Render context
     */
    void render(core::RenderContext& context);

    /**
     * @brief Mouse press event callback.
     * @param event Event data
     */
    void mousePressEvent(const core::MousePressEvent& event);

    /**
     * @brief Mouse move event callback.
     * @param event Event data
     */
    void mouseMoveEvent(const core::MouseMoveEvent& event);

    /**
     * @brief Mouse release event callback.
     * @param event Event data
     */
    void mouseReleaseEvent(const core::MouseReleaseEvent& event);

    /**
     * @brief Wheel event callback.
     * @param event Event data
     */
    void wheelEvent(const core::WheelEvent& event);

    /**
     * @brief Resize event callback.
     * @param event Event data
     */
    void resizeEvent(const core::ResizeEvent& event);

private:
    // Root node
    std::shared_ptr<SceneNode> m_rootNode;
    // Active camera
    resource::Camera* m_camera;
};

} // namespace scene
} // namespace jdl
