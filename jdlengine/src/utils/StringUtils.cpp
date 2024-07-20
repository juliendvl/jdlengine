#include "utils/StringUtils.hpp"


namespace jdl
{
namespace utils
{
namespace StringUtils
{

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

} // namespace StringUtils
} // namespace utils
} // namespace jdl
