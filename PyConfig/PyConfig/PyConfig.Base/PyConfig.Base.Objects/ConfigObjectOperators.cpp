#include "ConfigObject.h"

ConfigObject& operator<<(ConfigObject& obj, const std::pair<std::string, std::variant<int, long, float, double, bool, char, std::string>>& field) {
    obj.AddField(field.first, field.second);
    return obj;
}
const int operator>>(ConfigObject& obj, const std::pair<std::string, int>& fieldInfo) {
    return obj.GetField<int>(fieldInfo.first);
}
const long operator>>(ConfigObject& obj, const std::pair<std::string, long>& fieldInfo) {
    return obj.GetField<long>(fieldInfo.first);
}

const float operator>>(ConfigObject& obj, const std::pair<std::string, float>& fieldInfo) {
    return obj.GetField<float>(fieldInfo.first);
}
const double operator>>(ConfigObject& obj, const std::pair<std::string, double>& fieldInfo) {
    return obj.GetField<double>(fieldInfo.first);
}

const bool operator>>(ConfigObject& obj, const std::pair<std::string, bool>& fieldInfo) {
    return obj.GetField<bool>(fieldInfo.first);
}

const char operator>>(ConfigObject& obj, const std::pair<std::string, char>& fieldInfo) {
    return obj.GetField<char>(fieldInfo.first);
}
const std::string operator>>(ConfigObject& obj, const std::pair<std::string, std::string>& fieldInfo) {
    return obj.GetField<std::string>(fieldInfo.first);
}