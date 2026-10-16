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
	Size(int p_width, int p_height) : width(p_width), height(p_height) {}

	bool is_valid() const {
		return width >= 0 && height >= 0;
	}
};

} // namespace core
} // namespace jdl
