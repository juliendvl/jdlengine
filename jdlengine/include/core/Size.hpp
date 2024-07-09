#pragma once


namespace jdl
{
namespace core
{

struct Size
{
    int width  = -1;
    int height = -1;

    Size() = default;
    Size(int width, int height) : width(width), height(height) {}

    bool isValid() const {
        return width >= 0 && height >= 0;
    }
};

} // namespace core
} // namespace jdl
