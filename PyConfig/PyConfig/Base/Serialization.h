#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "ConfigObject.h"
#include <numbers>
#include <list>

class Serialization {
public:
	static void Serialize(ConfigObject& obj, const std::string& path);
	static void Serialize(ConfigObject& obj, std::fstream& configFile);

	static void Deserialize(ConfigObject& obj, const std::string& path);
	static void Deserialize(ConfigObject& obj, std::fstream& configFile);
private:
	template <typename T>
	static void AssignValue(ConfigObject& obj, const std::string& cfgN, const std::string& cfgV, T& cfgValue1, T& cfgValue2, T& cfgValue3) {
		int index = std::stoi(cfgN);
		T value = static_cast<T>(std::stod(cfgV));
		switch (index) {
		case 1: cfgValue1 = value;
			break;
		case 2: cfgValue2 = value;
			break;
		case 3: cfgValue3 = value;
			break;
		default: throw std::runtime_error("Invalid index: " + cfgN[0]);
			break;
		}
	}
	static void ParseLines(ConfigObject& obj, std::fstream& configFile);
	static void CustomParse(ConfigObject& obj, const std::string& line);
	static void CustomSerialize(const ConfigObject& obj, std::fstream& configFile);
};

#endif

