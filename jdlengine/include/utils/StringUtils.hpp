#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace utils
{
namespace StringUtils
{

/**
 * @brief Joins a sequence of string characters using a specified separator.
 * @param start String sequence start iterator.
 * @param end   String sequence end iterator.
 * @param sep   Sequence separator.
 * @return The resulting joined string.
 */
template<typename Iter>
std::string Join(Iter start, Iter end, const std::string& sep)
{
    if (start == end) {
        return "";
    }

    std::string result = *start;
    for (auto it = std::next(start); it != end; ++it) {
        result += sep + *it;
    }

    return result;
}

/**
 * @brief Joins a sequence of string characters using a specified separator.
 * @param seq String sequence.
 * @param sep Sequence separator.
 * @return The resulting joined string.
 */
JDL_API std::string Join(const std::vector<std::string>& seq, const std::string& sep);

/**
 * @brief Splits the input string according to the specified separator.
 * @param str Input string to be splitted.
 * @param sep Separator.
 * @return Resulting string sequence.
 */
JDL_API std::vector<std::string> Split(const std::string& str, const std::string& sep);


} // namespace StringUtils
} // namespace utils
} // namespace jdl
