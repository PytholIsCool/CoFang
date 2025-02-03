#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "Objects/ConfigObject.h"
#include <numbers>
#include <list>

class Serialization {
public:
	static void Serialize(ConfigObject& obj, const std::string& path);
	static void Deserialize(ConfigObject& obj, const std::string& path);
};

#endif