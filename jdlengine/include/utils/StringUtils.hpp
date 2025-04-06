#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace utils
{
namespace StringUtils
{

/**
 * @brief  Joins a sequence of strings using a specified separator.
 * @param  start String sequence start
 * @param  end   String sequence end
 * @param  sep   Separator string
 * @return The resulting string
 */
template<typename Iter>
std::string Join(Iter start, Iter end, const std::string& sep)
{
    std::string result = "";
    for (auto it = start; it != end; ++it)
    {
        result += *it;
        result += sep;
    }

    return result.substr(0, result.size() - sep.size());
}

/**
 * @brief  Joins a sequence of strings using a specified separator.
 * @param  seq String sequence
 * @param  sep Separator string
 * @return The resulting string
 */
JDL_API std::string Join(const std::vector<std::string>& seq, const std::string& sep);

/**
 * @brief  Splits the input string according to the specified separator.
 * @param  str Input string
 * @param  sep Separator string
 * @return The resulting string sequence
 */
JDL_API std::vector<std::string> Split(const std::string& str, const std::string& sep);

} // namespace StringUtils
} // namespace utils
} // namespace jdl
