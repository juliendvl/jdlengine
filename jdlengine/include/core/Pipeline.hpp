#pragma once

#include "Core.hpp"

#include <unordered_map>

#include "resource/Shader.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

enum class ShaderStage
{
    kVertex   = VK_SHADER_STAGE_VERTEX_BIT,
    kFragment = VK_SHADER_STAGE_FRAGMENT_BIT
};

enum class PrimitiveType
{
    kPoints        = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    kLines         = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    kLineStrip     = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    kTriangles     = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    kTriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    kTriangleFan   = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN
};

enum class PolygonMode
{
    kPoints    = VK_POLYGON_MODE_POINT,
    kWireframe = VK_POLYGON_MODE_LINE,
    kFill      = VK_POLYGON_MODE_FILL
};


class JDL_API Pipeline : private NonCopyable<Pipeline>
{
public:
    /**
     * @brief Graphics pipeline constructor.
     * It does not create the pipeline.
     */
    Pipeline();

    /**
     * @brief Destroys the graphics pipeline.
     */
    ~Pipeline();

    /**
     * @brief Sets the shader for the specified stage.
     * @param stage  Shader stage
     * @param shader Shader resource
     */
    void setShader(ShaderStage stage, resource::Shader* shader) { m_shaders[stage] = shader; }

    /**
     * @brief Sets the pipeline primitive type.
     * @param type The new primitive type
     */
    void setPrimitiveType(PrimitiveType type) { m_primitiveType = type; }

    /**
     * @brief Sets the pipeline polygon mode (how fragments are generated).
     * @param mode The new polygon mode
     */
    void setPolygonMode(PolygonMode mode) { m_polygonMode = mode; }

    /**
     * @brief Creates the graphics pipeline.
     */
    void create();

    /**
     * @brief Returns the pipeline Vulkan object.
     */
    VkPipeline getObject() const { return m_pipeline; }

    /**
     * @brief Returns the render pass object.
     */
    VkRenderPass getRenderPass() const { return m_renderPass; }

    /**
     * @brief Returns the pipeline layout object.
     */
    VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; }

private:
    // Shaders
    std::unordered_map<ShaderStage, resource::Shader*> m_shaders;
    // Primitive type
    PrimitiveType m_primitiveType;
    // Polygon mode
    PolygonMode m_polygonMode;

    // Vulkan pipeline object
    VkPipeline m_pipeline;
    // Pipeline layout object
    VkPipelineLayout m_pipelineLayout;
    // Render pass object
    VkRenderPass m_renderPass;

    // Creates the pipeline layout
    void createPipelineLayout(VkDevice device);
    // CReates the render pass
    void createRenderPass(VkDevice device);
};

} // namespace core
} // namespace jdl
