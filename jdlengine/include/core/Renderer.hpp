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

class JDL_API Renderer : private NonCopyable<Renderer>
{
public:
    /**
     * @brief Creates a renderer.
     */
    Renderer();

    /**
     * @brief Destroys the renderer.
     */
    ~Renderer();

    /**
     * @brief Sets the background color.
     * @param r, g, b, a The new background color.
     */
    void setBackgroundColor(float r, float g, float b, float a = 1.0f) {
        glClearColor(r, g, b, a);
    }

    /**
     * @brief Returns the rendered scene.
     */
    const scene::ScenePtr& getScene() const {
        return m_scene;
    }

    /**
     * @brief Sets the rendered scene.
     * @param scene The new scene.
     */
    void setScene(const scene::ScenePtr& scene) {
        m_scene = scene;
    }

    /**
     * @brief Renders a frame.
     */
    void renderFrame();

    /**
     * @brief Mouse press event.
     * @param event Event data.
     */
    void mousePressEvent(const MousePressEvent& event);

    /**
     * @brief Mouse move event.
     * @param event Event data.
     */
    void mouseMoveEvent(const MouseMoveEvent& event);

    /**
     * @brief Mouse release event.
     * @param event Event data.
     */
    void mouseReleaseEvent(const MouseReleaseEvent& event);

    /**
     * @brief Wheel event.
     * @param event Event data.
     */
    void wheelEvent(const WheelEvent& event);

    /**
     * @brief Resize event.
     * @param event Event data.
     */
    void resizeEvent(const ResizeEvent& event);

private:
    // Render context
    RenderContext m_context;
    // Rendered scene
    scene::ScenePtr m_scene;
};

} // namespace core
} // namespace jdl
