#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

class JDL_API Renderer : private NonCopyable<Renderer>
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
     * @brief Sets the background color.
     * @param r, g, b, a New background color.
     */
    void setBackgroundColor(float r, float g, float b, float a = 1.0f);

    /**
     * @brief Renders a new frame.
     */
    void renderFrame();
};

} // namespace core
} // namespace jdl
