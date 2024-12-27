#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace utils
{
namespace StringUtils
{

/**
 * @brief Joins the input sequence into a single string, using a specified separator.
 * @param start String sequence start
 * @param end   String sequence end
 * @param sep   Sequence separator
 */
template<typename Iter>
std::string Join(Iter start, Iter end, const std::string& sep)
{
    std::string result;
    for (Iter it = start; it != end; ++it)
    {
        result += *it;
        if (std::next(it) != end)
        {
            result += sep;
        }
    }
    return result;
}

/**
 * @brief Joins the input sequence into a single string, using a specified separator.
 * @param seq String sequence start
 * @param sep Sequence separator
 */
JDL_API std::string Join(const std::vector<std::string>& seq, const std::string& sep);

/**
 * @brief Splits the input string according to a specified separator.
 * @param str Input string
 * @param sep Separator
 */
JDL_API std::vector<std::string> Split(const std::string& str, const std::string& sep);

} // namespace StringUtils
} // namespace utils
} // namespace jdl
