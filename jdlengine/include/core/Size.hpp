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
    Size(int _width, int _height) : width(_width), height(_height) {}

    /**
     * @brief Returns whether the size is valid or not.
     */
    bool isValid() const {
        return width >= 0 && height >= 0;
    }
};

} // namespace core
} // namespace jdl
