#include "Serialization.h"
#include "File.h"
#include <string>

#pragma region Serialization
void Serialization::Serialize(ConfigObject& obj, const std::string& path) {
	std::fstream configFile = File::Create(path);

	if (ConObjMan::IsIntInit(obj))
		configFile << ConObjMan::FormatIntGroup(obj) << "\n";
	if (ConObjMan::IsLongInit(obj))
		configFile << ConObjMan::FormatLongGroup(obj) << "\n";
	if (ConObjMan::IsFloatInit(obj))
		configFile << ConObjMan::FormatFloatGroup(obj) << "\n";
	if (ConObjMan::IsDoubleInit(obj))
		configFile << ConObjMan::FormatDoubleGroup(obj) << "\n";
	if (ConObjMan::IsBoolInit(obj))
		configFile << ConObjMan::FormatBoolGroup(obj) << "\n";
	if (ConObjMan::IsCharInit(obj))
		configFile << ConObjMan::FormatCharGroup(obj) << "\n";
	if (ConObjMan::IsStringInit(obj))
		configFile << ConObjMan::FormatStringGroup(obj) << "\n";
	if (!obj.CustomFields.empty())
		Serialization::CustomSerialize(obj, configFile);

	configFile.close();
}
void Serialization::Serialize(ConfigObject& obj, std::fstream& configFile) {
	if (ConObjMan::IsIntInit(obj))
		configFile << ConObjMan::FormatIntGroup(obj) << "\n";
	if (ConObjMan::IsLongInit(obj))
		configFile << ConObjMan::FormatLongGroup(obj) << "\n";
	if (ConObjMan::IsFloatInit(obj))
		configFile << ConObjMan::FormatFloatGroup(obj) << "\n";
	if (ConObjMan::IsDoubleInit(obj))
		configFile << ConObjMan::FormatDoubleGroup(obj) << "\n";
	if (ConObjMan::IsBoolInit(obj))
		configFile << ConObjMan::FormatBoolGroup(obj) << "\n";
	if (ConObjMan::IsCharInit(obj))
		configFile << ConObjMan::FormatCharGroup(obj) << "\n";
	if (ConObjMan::IsStringInit(obj))
		configFile << ConObjMan::FormatStringGroup(obj) << "\n";
	if (!obj.CustomFields.empty())
		Serialization::CustomSerialize(obj, configFile);

	configFile.close();
}
#pragma endregion

#pragma region Deserialization
void Serialization::Deserialize(ConfigObject& obj, const std::string& path) {
	std::fstream configFile(path, std::ios::in);
	if (!configFile.is_open())
		throw std::runtime_error("Failed to open file: \"" + path + "\"");
	ParseLines(obj, configFile);
}
void Serialization::Deserialize(ConfigObject& obj, std::fstream& configFile) {
	ParseLines(obj, configFile);
}
#pragma endregion

#pragma region Native
void Serialization::ParseLines(ConfigObject& obj, std::fstream& configFile) {
	std::string cfgL;
	std::string cfgN;
	std::string cfgV;
	bool AllNative = true;

	while (std::getline(configFile, cfgL)) {
		if (cfgL.empty() || cfgL == "{" || cfgL == "}" || cfgL.contains(NullIndicator))
			continue;

		bool matched = false;

		if (cfgL.starts_with("  ConfigID: "))
			if (cfgL.substr(12) != obj.GetID())
				throw std::runtime_error("ConfigObject IDs do not match.");

		if (cfgL.starts_with("  Int")) {
			cfgV = (cfgN = cfgL.substr(5)).substr(2);
			Serialization::AssignValue(obj, cfgN, cfgV, obj.CfgInt1, obj.CfgInt2, obj.CfgInt3);
			matched = true;
		}
		if (cfgL.starts_with("  Long")) {
			cfgV = (cfgN = cfgL.substr(6)).substr(2);
			Serialization::AssignValue(obj, cfgN, cfgV, obj.CfgLong1, obj.CfgLong2, obj.CfgLong3);
			matched = true;
		}

		if (cfgL.starts_with("  Float")) {
			cfgV = (cfgN = cfgL.substr(7)).substr(2);
			Serialization::AssignValue(obj, cfgN, cfgV, obj.CfgFloat1, obj.CfgFloat2, obj.CfgFloat3);
			matched = true;
		}
		if (cfgL.starts_with("  Double")) {
			cfgV = (cfgN = cfgL.substr(8)).substr(2);
			Serialization::AssignValue(obj, cfgN, cfgV, obj.CfgDouble1, obj.CfgDouble2, obj.CfgDouble3);
			matched = true;
		}

		if (cfgL.starts_with("  Bool")) {
			cfgV = (cfgN = cfgL.substr(6)).substr(3);
			switch (atoi(cfgN.c_str())) {
			case 1: obj.CfgBool1 = cfgV == "true" ? true : false;
				break;
			case 2: obj.CfgBool2 = cfgV == "true" ? true : false;
				break;
			case 3: obj.CfgBool3 = cfgV == "true" ? true : false;
				break;
			default: throw std::runtime_error("Invalid index: " + cfgN[0]);
				break;
			}
			matched = true;
		}

		if (cfgL.starts_with("  Char")) {
			cfgV = (cfgN = cfgL.substr(6)).substr(3);
			switch (atoi(cfgN.c_str())) {
			case 1: obj.CfgChar1 = cfgV[0];
				break;
			case 2: obj.CfgChar2 = cfgV[0];
				break;
			case 3: obj.CfgChar3 = cfgV[0];
				break;
			default: throw std::runtime_error("Invalid index: " + cfgN[0]);
				break;
			}
			matched = true;
		}
		if (cfgL.starts_with("  String")) {
			cfgV = (cfgN = cfgL.substr(8)).substr(3);
			switch (atoi(cfgN.c_str())) {
			case 1: obj.CfgString1 = cfgV;
				break;
			case 2: obj.CfgString2 = cfgV;
				break;
			case 3: obj.CfgString3 = cfgV;
				break;
			default: throw std::runtime_error("Invalid index: " + cfgN[0]);
				break;
			}
			matched = true;
		}

		if (matched == false) {
			if (cfgL.substr(2).starts_with("ConfigID: "))
				continue;
			CustomParse(obj, cfgL.substr(2));
		}
	}
	cfgL.clear();
	cfgL.shrink_to_fit();
	cfgN.clear();
	cfgN.shrink_to_fit();
	cfgV.clear();
	cfgV.shrink_to_fit();
}
#pragma endregion

#pragma region Custom
void Serialization::CustomParse(ConfigObject& obj, const std::string& line) {
	std::string fieldName;
	std::string fieldValue;

	size_t colonPos = line.find(':');
	fieldName = line.substr(0, colonPos);
	fieldValue = line.substr(fieldName.size() + 2);

	fieldName = fieldName.substr(7);

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
		else if (fieldValue == C_TrueIndicator || fieldValue == C_FalseIndicator)
			parsedValue = (fieldValue == C_TrueIndicator);          // Bools
	}
	obj.AddField(fieldName, parsedValue);

	fieldName.clear();
	fieldName.shrink_to_fit();
	fieldValue.clear();
	fieldValue.shrink_to_fit();
}
void Serialization::CustomSerialize(const ConfigObject& obj, std::fstream& configFile) {
	for (const auto& [fieldName, fieldValue] : obj.CustomFields) {
		configFile << "{\n" << "  ConfigID: " << obj.GetID() << "\n  " << fieldName << ": ";
		std::visit([&](auto& value) {
			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, int>)
				configFile << std::to_string(value) << "\n";
			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, long>)
				configFile << std::to_string(value) << "L\n";

			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, float>)
				configFile << std::to_string(value) << "f\n";
			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, double>)
				configFile << std::to_string(value) << "D\n";

			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>)
				configFile << (value == 1 ? C_TrueIndicator : C_FalseIndicator) << "\n";

			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, char>)
				configFile << std::string(1, value) << "\n";
			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>)
				configFile << "\"" << value << "\"" << "\n";
			configFile << "}\n\n";
			}, fieldValue);
	}
}
#pragma endregion

// What I've learned:
//
// using constexpr marks stuff as immutable meaning they won't or may never change. This helps compile times, performance and even helps memory usage
// Templates are sexy
// std::decay_t<decltype(value goes here)> is an efficient way of getting a variable's type
// std::is_same_v is good for comparing types