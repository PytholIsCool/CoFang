#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

class Path {
public:
	enum SpecialFolder {
		Temp,
		AppData,
		Local,
		LocalLow,
		Roaming
	};

	static std::string Combine(const std::string& path1, const std::string& path2);
	static std::filesystem::path Create(const std::string& path);
	static std::filesystem::path Create(const std::filesystem::path& path);
	static std::filesystem::path GetFolderPath(const SpecialFolder& folder);
private:
	static std::filesystem::path GetEnvironmentPath(const char* envVar);
};

#endif