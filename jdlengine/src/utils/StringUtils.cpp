#include "utils/StringUtils.hpp"


namespace jdl
{
namespace utils
{
namespace StringUtils
{

std::string Join(const std::vector<std::string>& seq, const std::string& sep) {
    return Join(seq.cbegin(), seq.cend(), sep);
}

std::vector<std::string> Split(const std::string& str, const std::string& sep)
{
    std::vector<std::string> result;

    size_t start = 0U;
    size_t end = str.find(sep);

    while (end != std::string::npos)
    {
        result.push_back(str.substr(start, end - start));
        start = end + sep.length();
        end = str.find(sep, start);
    }

    result.push_back(str.substr(start, end));
    return result;
}

} // namespace StringUtils
} // namespace utils
} // namespace jdl
