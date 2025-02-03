#include "Path.h"
#include <sstream>
#include <fstream>
#include <string>

std::string Path::Combine(const std::string& path1, const std::string& path2) {
    if (path1.empty())
        return path2;
    if (path2.empty())
        return path1;

    std::string combinedPath;                                          // I saw Saya do comments like this and I like it so I'm taking it
    combinedPath.reserve(path1.size() + path2.size() + 1);             // Reserving the amount of space needed to avoid dynamic allocations
    combinedPath = path1;                                              // To my knowledge, they can still happen if the strings are big enough

    if (!combinedPath.empty() && (combinedPath.back() == '\\' || combinedPath.back() == '/'))
        combinedPath.pop_back();
    if (!path2.empty() && path2.front() != '\\' && path2.front() != '/')
        combinedPath.append("\\");
    combinedPath.append(path2);

    return combinedPath;
}
/// <summary>
/// Only include the path, not the file
/// </summary>
/// <param name="path"></param>
/// <returns>std::filesystem::path</returns>
std::filesystem::path Path::Create(const std::string& path) {
    std::filesystem::path newPath(path);
    if (newPath.has_extension()) {                                     // Making sure that the path doesn't have any extensions
        std::filesystem::create_directories(newPath.parent_path());
        return newPath.parent_path();                                  // Returns to parent path if an extension is included
    }                                                                  
    std::filesystem::create_directories(newPath);
    return newPath;
}
std::filesystem::path Path::Create(const std::filesystem::path& path) {
    std::filesystem::create_directories(path);
    return path;
}

std::filesystem::path Path::GetFolderPath(const SpecialFolder& folder) {
    switch (folder) {
    case SpecialFolder::AppData:
        return GetEnvironmentPath("APPDATA").parent_path();         // Appdata
    case SpecialFolder::Temp:
        return std::filesystem::temp_directory_path();              // Temp
    case SpecialFolder::Local:
        return GetEnvironmentPath("LOCALAPPDATA");                  // %LOCALAPPDATA%
    case SpecialFolder::LocalLow:
        return GetEnvironmentPath("LOCALAPPDATA").parent_path() / "LocalLow";
    case SpecialFolder::Roaming:
        return GetEnvironmentPath("APPDATA");                       // Roaming
    case SpecialFolder::Application:
        return std::filesystem::current_path();                     // Application Path
    default:
        throw std::invalid_argument("Unknown SpecialFolder type.");
    }
}

std::filesystem::path Path::GetEnvironmentPath(const char* envVar) {
    char* buffer = nullptr;
    size_t size = 0;
    errno_t err = _dupenv_s(&buffer, &size, envVar);
    if (err != 0 || buffer == nullptr)
        throw std::runtime_error(std::string("Failed to get environment variable: ") + envVar);
    std::unique_ptr<char, decltype(&free)> envValue(buffer, &free); // Lets me free the buffer instantly
    return std::filesystem::path(envValue.get());                 
}
// What I've learned:
// 
// This shit is cancer
// No real way to do this without potentially making copies
//
// std::filesystem::create_directories has an internal check for if the file exists or not.
//