#include "File.h"
#include "Path.h"

bool File::Exists(const std::string& path) {
	return std::filesystem::exists(path);
}

/// <summary>
/// Put in the path and the name of the file. Eg.) C:\Path\File.txt
/// </summary>
/// <param name="path"></param>
/// <returns>std::fstream</returns>
std::fstream File::Create(const std::string& path) {
	Path::Create(path);

	std::fstream newFile(path, std::ios::out | std::ios::trunc);
	if (!newFile)					// Replace the file if it already exists
		return std::fstream();      // Return an empty stream
	return newFile;                 // Return the new file (which should be in the path you specified)
}

std::fstream File::GetFileFromPath(const std::string& path) {
	if (!File::Exists(path))
		return std::fstream();
	return std::fstream(path, std::ios::in | std::ios::out);
}

// What I've learned:
// 
// Returning std::fstream() returns an empty stream
// You gotta specify the purpose of the file when creating it