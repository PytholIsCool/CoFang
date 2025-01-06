#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <filesystem>

class File {
public:
	static bool Exists(const std::string& path);
	static std::fstream Create(const std::string& path);
	static std::fstream GetFileFromPath(const std::string& path);
};

#endif