#pragma once

#include "Core.hpp"
#include "Events.hpp"
#include "RenderContext.hpp"

#include "scene/Scene.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

class JDL_API Renderer : private utils::NonCopyable<Renderer>
{
public:
    /**
     * @brief Creates the renderer.
     */
    Renderer();

    /**
     * @brief Safely destroys the renderer.
     */
    ~Renderer();

    /**
     * @brief Sets the renderer background color.
     * @param r, g, b, a The new background color
     */
    void setBackgroundColor(float r, float g, float b, float a = 1.0f) {
        glClearColor(r, g, b, a);
    }

    /**
     * @brief Sets the active scene.
     */
    void setScene(const std::shared_ptr<scene::Scene>& scene) { m_scene = scene; }

    /**
     * @brief Renders a new frame.
     */
    void renderFrame();

    /**
     * @brief Mouse press event callback.
     * @param event Event data
     */
    void mousePressEvent(const MousePressEvent& event);

    /**
     * @brief Mouse move event callback.
     * @param event Event data
     */
    void mouseMoveEvent(const MouseMoveEvent& event);

    /**
     * @brief Mouse release event callback.
     * @param event Event data
     */
    void mouseReleaseEvent(const MouseReleaseEvent& event);

    /**
     * @brief Wheel event callback.
     * @param event Event data
     */
    void wheelEvent(const WheelEvent& event);

    /**
     * @brief Resize event callback.
     * @param event Event data
     */
    void resizeEvent(const ResizeEvent& event);

private:
    // Active scene
    std::shared_ptr<scene::Scene> m_scene;
    // Active render context
    RenderContext m_context;
};

} // namespace core
} // namespace jdl
