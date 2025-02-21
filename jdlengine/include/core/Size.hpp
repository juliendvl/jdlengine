#pragma once


namespace jdl
{
namespace core
{

struct Size
{
    int width;
    int height;

    Size() : width(-1), height(-1) {}
    Size(int width, int height) : width(width), height(height) {}

    bool isValid() const { return width >= 0 && height >= 0; }
};

} // namespace core
} // namespace jdl
