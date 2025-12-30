#pragma once

#include "core/Object.hpp"
#include "core/Events.hpp"
#include "core/GlobalUniformBuffer.hpp"
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
     * @param name The scene name.
     */
    Scene(const std::string& name);

    /**
     * @brief Returns the scene root node.
     */
    const std::shared_ptr<SceneNode>& getRootNode() const {
        return m_rootNode;
    }

    /**
     * @brief Sets the scene root node.
     * @param rootNode The new root node.
     */
    void setRootNode(const std::shared_ptr<SceneNode>& rootNode) {
        m_rootNode = rootNode;
    }

    /**
     * @brief Returns the current camera.
     */
    resource::Camera* getCamera() const { return m_camera; }

    /**
     * @brief Sets the scene camera.
     * @param camera The new camera.
     */
    void setCamera(resource::Camera* camera);

    /**
     * @brief Renders the scene.
     * @param context Render context.
     */
    void render(core::RenderContext& context);

    /**
     * @brief Mouse press event handler.
     * @param event Event data.
     */
    void mousePressEvent(const core::MousePressEvent& event);

    /**
     * @brief Mouse release event handler.
     * @param event Event data.
     */
    void mouseReleaseEvent(const core::MouseReleaseEvent& event);

    /**
     * @brief Mouse move event handler.
     * @param event Event data.
     */
    void mouseMoveEvent(const core::MouseMoveEvent& event);

    /**
     * @brief Wheel event handler.
     * @param event Event data.
     */
    void wheelEvent(const core::WheelEvent& event);

    /**
     * @brief Resize event handler.
     * @param event Event data.
     */
    void resizeEvent(const core::ResizeEvent& event);

private:
    // Root node
    std::shared_ptr<SceneNode> m_rootNode;
    // Current camera
    resource::Camera* m_camera = nullptr;
    bool m_cameraDirty = true;
    // Scene gloabl uniform buffer
    core::GlobalUniformBuffer m_uniformBuffer;
};

} // namespace scene
} // namespace jdl
