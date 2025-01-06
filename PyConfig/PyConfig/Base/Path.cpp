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

// What I've learned:
// 
// This shit is cancer
// No real way to do this without potentially making copies
// Namespaces must be stacked
// (I think)
//
// std::filesystem::create_directories has an internal check for if the file exists or not.
//