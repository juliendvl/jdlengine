#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace utils
{
namespace StringUtils
{

/**
 * @brief Joins the strings in the input sequence, using a given separator.
 * @param start String sequence start iterator
 * @param end   String sequence end iterator
 * @param sep   Separator
 * @return Resulting string
 */
template<typename Iter>
std::string Join(Iter start, Iter end, const std::string& sep)
{
    std::string result = "";
    for (auto it = start; it != end; ++it)
    {
        result += *it + sep;
    }
    // Remove the extra added separator from the resulting string
    return result.substr(0, result.length() - sep.length());
}

/**
 * @brief Joins the strings in the input sequence, using a given separator.
 * @param seq String array
 * @param sep Separator
 * @return Resulting string
 */
JDL_API std::string Join(const std::vector<std::string>& seq, const std::string& sep);

/**
 * @brief Splits the input string according to the given separator.
 * @param str Input string
 * @param sep Separator
 * @return Output string array
 */
JDL_API std::vector<std::string> Split(const std::string& str, const std::string& sep);

/**
 * @brief Converts the input string to lowercase.
 * @param str Input string to be converted
 * @return The converted lowercase string.
 */
JDL_API std::string ToLower(std::string str);

/**
 * @brief Converts the input string to uppercase.
 * @param str Input string to be converted
 * @return The converted uppercase string.
 */
JDL_API std::string ToUpper(std::string str);

} // namespace StringUtils
} // namespace utils
} // namespace jdl
