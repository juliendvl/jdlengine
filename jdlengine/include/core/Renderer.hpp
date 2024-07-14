#pragma once

#include "Core.hpp"
#include "Events.hpp"

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
     * @brief Renders a frame.
     */
    void renderFrame();

    /**
     * @brief Resize event.
     * @param event Event data.
     */
    void resizeEvent(const ResizeEvent& event);
};

} // namespace core
} // namespace jdl
