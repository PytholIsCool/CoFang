#include "ConfigObject.h"

ConfigObject& operator<<(ConfigObject& obj, const std::pair<std::string, std::variant<int, long, float, double, bool, char, std::string>>& field) {
    obj.AddField(field.first, field.second);
    return obj;
}