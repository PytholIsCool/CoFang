#include "Serialization.h"
#include "FileSystemWrapper/File.h"
#include <string>
#include <regex>

#pragma region Serialization
void Serialization::Serialize(ConfigObject& obj, const std::string& path) {
	if (obj.Serialized)													// If the object has already been serialized (and is up to date), return
		return;															
	                                                                    
	if (File::Exists(path)) {	                                        // If the file exists:
		std::ifstream configFileIn(path);                               // Read the contents of the file
		std::stringstream buffer;                                       
		buffer << configFileIn.rdbuf();                                 // Write the contents of the file to the buffer
		std::string fileContent = buffer.str();                         // Conver the contents of the buffer to a regular string
		configFileIn.close();                                           // Close the file as it's no longer needed

		if (fileContent.contains("  ConfigID: " + obj.GetID())) {		// Notes at the bottom of the file
			std::regex configRegex(R"(\{\s*ConfigID:\s*)" + obj.GetID() + R"(\s*[^}]*\}\s*)");
			fileContent = std::regex_replace(fileContent, configRegex, "");
																		// Remove the old contents of the file corrosponding with the object parameter
			std::ofstream configFileOut(path, std::ios::trunc);         // Open the file for writing (overwrite/truncate mode)
			configFileOut << fileContent;								// Write the cleaned content back
		}
	}
	else
		File::Create(path);											    // If the file doesn't exist, create it

	std::ofstream configFile(path, std::ios::app);                      // Open soley for appending
	configFile << "{\n" << "  ConfigID: " << obj.GetID() << "\n";

	for (const auto& [fieldName, fieldValue] : obj.Fields) {
		configFile << "  " << fieldName << ": ";
		std::visit([&](auto& value) {
			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, int>)
				configFile << std::to_string(value) << "\n";
			else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, long>)
				configFile << std::to_string(value) << "L\n";

			else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, float>)
				configFile << std::to_string(value) << "f\n";
			else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, double>)
				configFile << std::to_string(value) << "D\n";

			else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>)
				configFile << (value == 1 ? "true" : "false") << "\n";

			else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, char>)
				configFile << '\'' << std::string(1, value) << "'\n";
			else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>)
				configFile << "\"" << value << "\"" << "\n";
			else
				throw std::runtime_error("Invalid data type found. Please review your fields.");
			}, fieldValue);
	}
	configFile << "}\n\n";

	configFile.close();

	obj.Serialized = true;
}
#pragma endregion

#pragma region Deserialization
void Serialization::Deserialize(ConfigObject& obj, const std::string& path) {
	std::fstream configFile(path, std::ios::in);
	if (!configFile.is_open())
		throw std::runtime_error("Failed to open file: \"" + path + "\"");

	std::string line;
	std::string fieldName;
	std::string fieldValue;
	bool aligned = false;

	while (std::getline(configFile, line)) {

		if (line.empty())
			continue;
		if (line == "{") {
			aligned = false;
			continue;
		}
		if (line == "}") {
			if (aligned)
				break;
			continue;
		}

		if (!aligned && line.starts_with("  ConfigID: ")) {
			if (line.substr(12) == obj.GetID())
				aligned = true;                                             // Found the correct id
			continue;
		}
		if (line.substr(2).starts_with("ConfigID: "))
			continue;
		if (!aligned)
			continue;

		line = line.substr(2);

		size_t colonPos = line.find(':');
		fieldName = line.substr(0, colonPos);
		fieldValue = line.substr(fieldName.size() + 2);

		std::variant<int, long, float, double, bool, char, std::string> parsedValue;

		if (fieldValue.front() == '"' && fieldValue.back() == '"')
			parsedValue = fieldValue.substr(1, fieldValue.size() - 2);      // Strings
		else if (fieldValue.front() == '\'' && fieldValue.back() == '\'')
			parsedValue = (fieldValue.substr(1, fieldValue.size() - 2))[0]; // Chars
		else {
			std::string potNum = fieldValue;                                // Parse numbers and other types as before
			if (!potNum.empty() && !::isdigit(potNum.back()))
				potNum.pop_back();                                          // Remove type indicator

			if (std::all_of(potNum.begin(), potNum.end(), ::isdigit))
				if (fieldValue.back() == 'L')
					parsedValue = std::stol(potNum);                        // Longs
				else
					parsedValue = std::stoi(potNum);                        // Ints
			else if (potNum.find('.') != std::string::npos) {
				std::string potF = potNum;
				potF.erase(std::remove(potF.begin(), potF.end(), '.'), potF.end());
				if (std::all_of(potF.begin(), potF.end(), ::isdigit))
					if (fieldValue.back() == 'f')
						parsedValue = std::stof(potNum);                    // Floats
					else if (fieldValue.back() == 'D')
						parsedValue = std::stod(potNum);                    // Doubles
			}
			else if (fieldValue == "true" || fieldValue == "false")
				parsedValue = (fieldValue == "true");                       // Bools
		}
		obj.AddField(fieldName, parsedValue);
	}
	line.clear();
	line.shrink_to_fit();
	fieldName.clear();
	fieldName.shrink_to_fit();
	fieldValue.clear();
	fieldValue.shrink_to_fit();

	configFile.close();

	if (!aligned)
		throw std::runtime_error("ConfigObject with ID \"" + obj.GetID() + "\" not found.");

	obj.Serialized = true;
}
#pragma endregion

// What I've learned:
//
// using constexpr marks stuff as immutable meaning they won't or may never change. This helps compile times, performance and even helps memory usage
// Templates are sexy
// std::decay_t<decltype(value goes here)> is an efficient way of getting a variable's type
// std::is_same_v is good for comparing types
// 
// std::regex notation stuff:
// 
// { is a special character in regex so we escape it as "\{"
// same with }
// \s* represents white space or empty space. This can be a new line or series of actual spaces
// [^}]* represents any and all characters that aren't "}". That's how we can find the fields without explicitly looking for them