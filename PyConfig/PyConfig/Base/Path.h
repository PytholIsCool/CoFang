#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

class Path {
public:
	static std::string Combine(const std::string& path1, const std::string& path2);
	static std::filesystem::path Create(const std::string& path);
	static std::filesystem::path Create(const std::filesystem::path& path);
};

#endif