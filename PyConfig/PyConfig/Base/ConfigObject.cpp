#include "ConfigObject.h"

std::unordered_map<std::string, ConfigObject*> ConfigObject::ConfigRegistry;

ConfigObject* ConfigObject::FindByID(const std::string& Identifier) {
    if (ConfigRegistry.find(Identifier) != ConfigRegistry.end())
        return ConfigRegistry.find(Identifier)->second;
    else
        throw std::runtime_error("ConfigObject could not be found. Perhaps the identifier is invalid?");
    return nullptr;
}

const std::string& ConfigObject::GetID() {
    return this->Identifier;
}
const std::string& ConfigObject::GetID() const {
    return this->Identifier;
}
void ConfigObject::AddField(const std::string& fieldName, const std::variant<int, long, float, double, bool, char, std::string>& type) {
    std::string prefixedName = "Custom_" + fieldName;
    if (this->FieldExists(prefixedName))
        throw std::runtime_error("Field already exists. Please assign your new field a different name.");
    if (fieldName.contains(':'))
        throw std::runtime_error("Colons are not permitted for field names.");
    this->CustomFields.emplace(prefixedName, type);
}
void ConfigObject::RemoveField(const std::string& fieldName) {
    std::string prefixedName = "Custom_" + fieldName;
    if (this->FieldExists(prefixedName))
        this->CustomFields.erase(prefixedName);
}
bool ConfigObject::FieldExists(const std::string& fieldName) const {
    return this->CustomFields.contains(fieldName);
}

// ConObjMan

std::string ConObjMan::FormatIntGroup(ConfigObject& obj) {
    return std::format("{{\n  ConfigID: {}\n  Int1: {}\n  Int2: {}\n  Int3: {}\n}}",
        obj.GetID(),
        obj.CfgInt1 ? std::to_string(obj.CfgInt1.value()) : NullIndicator,
        obj.CfgInt2 ? std::to_string(obj.CfgInt2.value()) : NullIndicator,
        obj.CfgInt3 ? std::to_string(obj.CfgInt3.value()) : NullIndicator)
        .append("\n");
}
bool ConObjMan::IsIntInit(ConfigObject& obj) {
    return !(obj.CfgInt1 == std::nullopt && obj.CfgInt2 == std::nullopt && obj.CfgInt3 == std::nullopt);
}
std::string ConObjMan::FormatLongGroup(ConfigObject& obj) {
    return std::format("{{\n  ConfigID: {}\n  Long1: {}\n  Long2: {}\n  Long3: {}\n}}",
        obj.GetID(),
        obj.CfgLong1 ? std::to_string(obj.CfgLong1.value()) : NullIndicator,
        obj.CfgLong2 ? std::to_string(obj.CfgLong2.value()) : NullIndicator,
        obj.CfgLong3 ? std::to_string(obj.CfgLong3.value()) : NullIndicator)
        .append("\n");
}
bool ConObjMan::IsLongInit(ConfigObject& obj) {
    return !(obj.CfgLong1 == std::nullopt && obj.CfgLong2 == std::nullopt && obj.CfgLong3 == std::nullopt);
}

std::string ConObjMan::FormatFloatGroup(ConfigObject& obj) {
    return std::format("{{\n  ConfigID: {}\n  Float1: {}\n  Float2: {}\n  Float3: {}\n}}",
        obj.GetID(),
        obj.CfgFloat1 ? std::to_string(obj.CfgFloat1.value()) : NullIndicator,
        obj.CfgFloat2 ? std::to_string(obj.CfgFloat2.value()) : NullIndicator,
        obj.CfgFloat3 ? std::to_string(obj.CfgFloat3.value()) : NullIndicator)
        .append("\n");
}
bool ConObjMan::IsFloatInit(ConfigObject& obj) {
    return !(obj.CfgFloat1 == std::nullopt && obj.CfgFloat2 == std::nullopt && obj.CfgFloat3 == std::nullopt);
}
std::string ConObjMan::FormatDoubleGroup(ConfigObject& obj) {
    return std::format("{{\n  ConfigID: {}\n  Double1: {}\n  Double2: {}\n  Double3: {}\n}}",
        obj.GetID(),
        obj.CfgDouble1 ? std::to_string(obj.CfgDouble1.value()) : NullIndicator,
        obj.CfgDouble2 ? std::to_string(obj.CfgDouble2.value()) : NullIndicator,
        obj.CfgDouble3 ? std::to_string(obj.CfgDouble3.value()) : NullIndicator)
        .append("\n");
}
bool ConObjMan::IsDoubleInit(ConfigObject& obj) {
    return !(obj.CfgDouble1 == std::nullopt && obj.CfgDouble2 == std::nullopt && obj.CfgDouble3 == std::nullopt);
}

std::string ConObjMan::FormatBoolGroup(ConfigObject& obj) {
    return std::format("{{\n  ConfigID: {}\n  Bool1: {}\n  Bool2: {}\n  Bool3: {}\n}}",
        obj.GetID(),
        obj.CfgBool1 ? (obj.CfgBool1.value() == true ? "true" : "false") : NullIndicator,
        obj.CfgBool2 ? (obj.CfgBool2.value() == true ? "true" : "false") : NullIndicator,
        obj.CfgBool3 ? (obj.CfgBool3.value() == true ? "true" : "false") : NullIndicator)
        .append("\n");
}
bool ConObjMan::IsBoolInit(ConfigObject& obj) {
    return !(obj.CfgBool1 == std::nullopt && obj.CfgBool2 == std::nullopt && obj.CfgBool3 == std::nullopt);
}

std::string ConObjMan::FormatCharGroup(ConfigObject& obj) {
    return std::format("{{\n  ConfigID: {}\n  Char1: {}\n  Char2: {}\n  Char3: {}\n}}",
        obj.GetID(),
        obj.CfgChar1.has_value() ? std::string(1, obj.CfgChar1.value()) : NullIndicator,
        obj.CfgChar2.has_value() ? std::string(1, obj.CfgChar2.value()) : NullIndicator,
        obj.CfgChar3.has_value() ? std::string(1, obj.CfgChar3.value()) : NullIndicator)
        .append("\n");
}
bool ConObjMan::IsCharInit(ConfigObject& obj) {
    return !(obj.CfgChar1 == std::nullopt && obj.CfgChar2 == std::nullopt && obj.CfgChar3 == std::nullopt);
}
std::string ConObjMan::FormatStringGroup(ConfigObject& obj) {
    return std::string("{\n  ConfigID: ")
        .append(obj.GetID())
        .append("\n  String1: ")
        .append(obj.CfgString1 ? obj.CfgString1.value() : NullIndicator)
        .append("\n  String2: ")
        .append(obj.CfgString2 ? obj.CfgString2.value() : NullIndicator)
        .append("\n  String3: ")
        .append(obj.CfgString3 ? obj.CfgString3.value() : NullIndicator)
        .append("\n}\n");
}
bool ConObjMan::IsStringInit(ConfigObject& obj) {
    return !(obj.CfgString1 == std::nullopt && obj.CfgString2 == std::nullopt && obj.CfgString3 == std::nullopt);
}

// What I've learned:
//
// std::format is sex
// std::optional is used for values that aren't guarenteed to see any use but should still exist in case they are used
// std::nullopt (null optional) is a way of removing any and all value from a variable that's been marked as optional