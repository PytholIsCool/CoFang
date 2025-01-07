#include "Serialization.h"
#include "PyConfig.Base.FileSystemWrapper/File.h"
#include <string>

#pragma region Serialization
void Serialization::Serialize(ConfigObject& obj, const std::string& path) {
	std::fstream configFile;
	if (File::Exists(path)) {
		configFile.open(path, std::ios::out | std::ios::app);
	}
	else
		configFile = File::Create(path);
		

	if (!obj.Fields.empty()) {
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
					configFile << (value == 1 ? R"('true')" : R"('false')") << "\n";

				else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, char>)
					configFile << std::string(1, value) << "\n";
				else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>)
					configFile << "\"" << value << "\"" << "\n";
				else
					throw std::runtime_error("Invalid data type found. Please review your fields.");  
				}, fieldValue);
		}
		configFile << "}\n\n";
	}

	configFile.close();
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
				aligned = true;                                         // Found the correct id
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
			parsedValue = fieldValue.substr(1, fieldValue.size() - 2);  // Strings
		else if (fieldValue.size() == 1 && !::isdigit(fieldValue[0]))
			parsedValue = fieldValue[0];                                // Chars
		else {
			std::string potNum = fieldValue;                            // Parse numbers and other types as before
			if (!potNum.empty() && !::isdigit(potNum.back()))
				potNum.pop_back();                                      // Remove type indicator

			if (std::all_of(potNum.begin(), potNum.end(), ::isdigit))
				if (fieldValue.back() == 'L')
					parsedValue = std::stol(potNum);                    // Longs
				else
					parsedValue = std::stoi(potNum);                    // Ints
			else if (potNum.find('.') != std::string::npos) {
				std::string potF = potNum;
				potF.erase(std::remove(potF.begin(), potF.end(), '.'), potF.end());
				if (std::all_of(potF.begin(), potF.end(), ::isdigit))
					if (fieldValue.back() == 'f')
						parsedValue = std::stof(potNum);                // Floats
					else if (fieldValue.back() == 'D')
						parsedValue = std::stod(potNum);                // Doubles
			}
			else if (fieldValue == R"('true')" || fieldValue == R"('false')")
				parsedValue = (fieldValue == R"('true')");              // Bools
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
}
#pragma endregion

// What I've learned:
//
// using constexpr marks stuff as immutable meaning they won't or may never change. This helps compile times, performance and even helps memory usage
// Templates are sexy
// std::decay_t<decltype(value goes here)> is an efficient way of getting a variable's type
// std::is_same_v is good for comparing types