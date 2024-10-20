#include "utils/FileUtils.hpp"

#include <filesystem>


namespace fs = std::filesystem;


namespace jdl
{
namespace utils
{
namespace FileUtils
{

std::string GetCurrentDir()
{
    return fs::current_path().string();
}

void SetCurrentDir(const std::string& path)
{
    fs::current_path(fs::path(path));
}

std::string GetDirName(const std::string& filePath)
{
    fs::path path(filePath);
    return path.parent_path().string();
}

std::string GetFileExtension(const std::string& filePath)
{
    fs::path path(filePath);
    return path.extension().string();
}

} // namespace FileUtils
} // namespace utils
} // namespace jdl
