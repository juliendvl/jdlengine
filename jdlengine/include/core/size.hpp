#pragma once


namespace jdl
{
namespace core
{

struct Size
{
	int width = -1;
	int height = -1;

	/**
	 * @brief Creates an invalid size.
	 */
	Size() = default;

	/**
	 * @brief Creates a size.
	 * @param p_width The width.
	 * @param p_height The height.
	 */
	Size(int p_width, int p_height) : width(p_width), height(p_height) {}

	/**
	 * @brief Returns whether both dimensions are non-negative.
	 */
	bool is_valid() const {
		return width >= 0 && height >= 0;
	}
};

} // namespace core
} // namespace jdl
