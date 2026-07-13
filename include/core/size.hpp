#pragma once


namespace jdl
{
namespace core
{

struct Size
{
    int width = -1;
    int height = -1;

    Size() = default;
    Size(int t_width, int t_height)
        : width(t_width)
        , height(t_height)
    {}

    bool is_valid() const { return width >= 0 && height >= 0; }
};

} // namespace core
} // namespace jdl
