#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace utils
{
namespace FileUtils
{

/**
 * @brief Returns the current working directory.
 */
JDL_API std::string GetCurrentDir();

/**
 * @brief Sets the current working directory.
 * @param path The new working directory.
 */
JDL_API void SetCurrentDir(const std::string& path);

/**
 * @brief  Returns the directory name of the input file path (i.e. parent path).
 * @param  filePath Input file path
 * @return The directory name.
 */
JDL_API std::string GetDirName(const std::string& filePath);

/**
 * @brief Returns the extension of the input file.
 * If the input file contains multiple extensions (ex: toto.tar.gz) then only the last extension will be returned.
 * 
 * @param filePath Input file path
 * @return File extension, with the "dot" marker.
 */
JDL_API std::string GetFileExtension(const std::string& filePath);

} // namespace FileUtils
} // namespace utils
} // namespace jdl
