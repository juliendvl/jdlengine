#include "utils/StringUtils.hpp"

#include <algorithm>


namespace jdl
{
namespace utils
{
namespace StringUtils
{

std::string Join(const std::vector<std::string>& seq, const std::string& sep)
{
    return Join(seq.cbegin(), seq.cend(), sep);
}

std::vector<std::string> Split(const std::string& str, const std::string& sep)
{
    std::vector<std::string> result;

    size_t start = 0;
    size_t end = str.find(sep);
    
    while (end != std::string::npos)
    {
        result.push_back(str.substr(start, end - start));
        start = end + sep.length();
        end = str.find(sep, start);
    }
    // Add the last string to the result array
    result.push_back(str.substr(start, end));

    return result;
}

std::string ToLower(std::string str)
{
    std::transform(
        str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    return str;
}

std::string ToUpper(std::string str)
{
    std::transform(
        str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::toupper(c); }
    );
    return str;
}

} // namespace StringUtils
} // namespace utils
} // namespace jdl
