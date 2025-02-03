#include "ConfigObject.h"

std::unordered_map<std::string, ConfigObject*> ConfigObject::ConfigRegistry;

#pragma region Constructors
// Constructor
ConfigObject::ConfigObject(const std::string& id) : Identifier(id) {
    if (ConfigRegistry.find(id) != ConfigRegistry.end())
        throw std::runtime_error("Identifier already exists.");
    Serialized = false;
    ConfigRegistry[id] = this;
}

// Destructor
ConfigObject::~ConfigObject() {
    Serialized = false;
    Fields.clear();
    ConfigRegistry.erase(Identifier);
}

// Copy Constructor
ConfigObject::ConfigObject(const ConfigObject& other) : Identifier(other.Identifier + "_copy"), Fields(other.Fields) {
    while (ConfigRegistry.contains(Identifier))
        Identifier = Identifier.append("_copy");
    ConfigRegistry[Identifier] = this;
    Serialized = false;
}

// Copy Assignment Operator
ConfigObject& ConfigObject::operator=(const ConfigObject& other) {
    if (this == &other) return *this;

    ConfigRegistry.erase(Identifier);

    Identifier = other.Identifier + "_copy";
    while (ConfigRegistry.contains(Identifier))
        Identifier = Identifier.append("_copy");

    Fields = other.Fields;
    ConfigRegistry[Identifier] = this;
    Serialized = false;

    return *this;
}

// Move Constructor
ConfigObject::ConfigObject(ConfigObject&& other) noexcept
    : Identifier(std::move(other.Identifier)), Fields(std::move(other.Fields)) {
    ConfigRegistry[Identifier] = this;
    Serialized = other.Serialized;
    ConfigRegistry.erase(other.Identifier);
}
#pragma endregion

#pragma region Operators
// Move Assignment Operator
ConfigObject& ConfigObject::operator=(ConfigObject&& other) noexcept {
    if (this == &other) return *this;

    ConfigRegistry.erase(Identifier);

    Identifier = std::move(other.Identifier);
    Fields = std::move(other.Fields);
    Serialized = other.Serialized;

    ConfigRegistry[Identifier] = this;

    return *this;
}

// Pointer Assignment Operator
ConfigObject* ConfigObject::operator=(ConfigObject* other) noexcept {
    if (this == other || other == nullptr) return this;

    ConfigRegistry.erase(Identifier);

    Identifier = std::move(other->Identifier);
    Fields = std::move(other->Fields);
    Serialized = other->Serialized;

    ConfigRegistry[Identifier] = this;

    return this;
}

bool ConfigObject::operator==(const ConfigObject& other) const {
    return this->Identifier == other.Identifier;
}

bool ConfigObject::operator==(const ConfigObject* other) const {
    if (other == nullptr) return false;
    return this->Identifier == other->Identifier;
}
#pragma endregion

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

    this->Serialized = false;

    return *this;
}
void ConfigObject::RemoveField(const std::string& fieldName) {
    if (this->FieldExists(fieldName)) {
        this->Fields.erase(fieldName);
        this->Serialized = false;
    }
}
bool ConfigObject::FieldExists(const std::string& fieldName) const {
    return this->Fields.contains(fieldName);
}

// What I've learned:
//
// std::format is sex
// std::optional is used for values that aren't guarenteed to see any use but should still exist in case they are used
// std::nullopt (null optional) is a way of removing any and all value from a variable that's been marked as optional