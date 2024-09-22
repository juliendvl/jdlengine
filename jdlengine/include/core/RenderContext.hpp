#pragma once

#include "resource/ShaderProgram.hpp"


namespace jdl
{
namespace core
{

struct RenderContext
{
    // Active shader program
    resource::ShaderProgram* shaderProgram;
};

} // namespace core
} // namespace jdl
