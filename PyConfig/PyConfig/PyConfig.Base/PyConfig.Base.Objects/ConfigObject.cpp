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
ConfigObject& ConfigObject::AddField(const std::string& fieldName, const std::variant<int, long, float, double, bool, char, std::string>& type) {
    if (this->FieldExists(fieldName))
        throw std::runtime_error("Field already exists. Please assign your new field a different name.");
    if (fieldName.contains(':'))
        throw std::runtime_error("Colons are not permitted for field names.");
    this->Fields.emplace(fieldName, type);
    return *this;
}
void ConfigObject::RemoveField(const std::string& fieldName) {
    if (this->FieldExists(fieldName))
        this->Fields.erase(fieldName);
}
bool ConfigObject::FieldExists(const std::string& fieldName) const {
    return this->Fields.contains(fieldName);
}

// What I've learned:
//
// std::format is sex
// std::optional is used for values that aren't guarenteed to see any use but should still exist in case they are used
// std::nullopt (null optional) is a way of removing any and all value from a variable that's been marked as optional