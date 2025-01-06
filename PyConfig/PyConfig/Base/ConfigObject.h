#ifndef CONFIGOBJECT_H
#define CONFIGOBJECT_H

#include <iostream>
#include <string>
#include <optional>
#include <format>
#include <memory>
#include <unordered_map>
#include <variant>

static const std::string NullIndicator = R"('RoQ)E%1=-TG$a9XpSg~BNPhIA[=/v)";
static const std::string C_TrueIndicator = R"(A3r!k9Lp$w2Xz7%UqB8v@M4yJ1Nz6)";
static const std::string C_FalseIndicator = R"(T5fG@8ZxR#2wQj6Lk9Y1mV&3A0Pn7)";

class ConfigObject {
private:
    std::string Identifier;
    static std::unordered_map<std::string, ConfigObject*> ConfigRegistry;
public:
    std::unordered_map<std::string, std::variant<int, long, float, double, bool, char, std::string>> CustomFields;

    std::optional<int> CfgInt1, CfgInt2, CfgInt3;
    std::optional<long> CfgLong1, CfgLong2, CfgLong3;

    std::optional<float> CfgFloat1, CfgFloat2, CfgFloat3;
    std::optional<double> CfgDouble1, CfgDouble2, CfgDouble3;

    std::optional<bool> CfgBool1, CfgBool2, CfgBool3;

    std::optional<char> CfgChar1, CfgChar2, CfgChar3;
    std::optional<std::string> CfgString1, CfgString2, CfgString3;
public:
#pragma region Constructors
    // Constructor
    ConfigObject(const std::string& id) : Identifier(id) {
        if (ConfigRegistry.find(id) != ConfigRegistry.end())
            throw std::runtime_error("Identifier already exists.");
        ConfigRegistry[id] = this;
    }
    // Destructor
    ~ConfigObject() {
        ConfigRegistry.erase(Identifier);
    }
    // Copy handler
    ConfigObject(const ConfigObject& other)                            // Handles any copies
        : Identifier(other.Identifier + "_copy"),                      // Updates the ID so the registry doesn't get all fucky wucky
        CfgInt1(other.CfgInt1), CfgInt2(other.CfgInt2), CfgInt3(other.CfgInt3),
        CfgLong1(other.CfgLong1), CfgLong2(other.CfgLong2), CfgLong3(other.CfgLong3),
        CfgFloat1(other.CfgFloat1), CfgFloat2(other.CfgFloat2), CfgFloat3(other.CfgFloat3),
        CfgDouble1(other.CfgDouble1), CfgDouble2(other.CfgDouble2), CfgDouble3(other.CfgDouble3),
        CfgBool1(other.CfgBool1), CfgBool2(other.CfgBool2), CfgBool3(other.CfgBool3),
        CfgChar1(other.CfgChar1), CfgChar2(other.CfgChar2), CfgChar3(other.CfgChar3),
        CfgString1(other.CfgString1), CfgString2(other.CfgString2), CfgString3(other.CfgString3) {
        if (ConfigRegistry.find(Identifier) != ConfigRegistry.end())
            throw std::runtime_error("Identifier already exists in the registry.");
        ConfigRegistry[Identifier] = this;
    }
    // Copy assignment handler
    ConfigObject& operator=(const ConfigObject& other) {               // Handles any copy assignments
        if (this == &other) return *this;                              // Prevents self assignments

        ConfigRegistry.erase(Identifier);                              // Erases the original object

        Identifier = other.Identifier + "_copy";                       // Gives a new ID to the copy
        CfgInt1 = other.CfgInt1; CfgInt2 = other.CfgInt2; CfgInt3 = other.CfgInt3;
        CfgLong1 = other.CfgLong1; CfgLong2 = other.CfgLong2; CfgLong3 = other.CfgLong3;
        CfgFloat1 = other.CfgFloat1; CfgFloat2 = other.CfgFloat2; CfgFloat3 = other.CfgFloat3;
        CfgDouble1 = other.CfgDouble1; CfgDouble2 = other.CfgDouble2; CfgDouble3 = other.CfgDouble3;
        CfgBool1 = other.CfgBool1; CfgBool2 = other.CfgBool2; CfgBool3 = other.CfgBool3;
        CfgChar1 = other.CfgChar1; CfgChar2 = other.CfgChar2; CfgChar3 = other.CfgChar3;
        CfgString1 = other.CfgString1; CfgString2 = other.CfgString2; CfgString3 = other.CfgString3;

        ConfigRegistry[Identifier] = this;                             // Updates the registry I made

        return *this;
    }
    // Move handler
    ConfigObject(ConfigObject&& other) noexcept
        : Identifier(std::move(other.Identifier)),
        CfgInt1(other.CfgInt1), CfgInt2(other.CfgInt2), CfgInt3(other.CfgInt3),
        CfgLong1(other.CfgLong1), CfgLong2(other.CfgLong2), CfgLong3(other.CfgLong3),
        CfgFloat1(other.CfgFloat1), CfgFloat2(other.CfgFloat2), CfgFloat3(other.CfgFloat3),
        CfgDouble1(other.CfgDouble1), CfgDouble2(other.CfgDouble2), CfgDouble3(other.CfgDouble3),
        CfgBool1(other.CfgBool1), CfgBool2(other.CfgBool2), CfgBool3(other.CfgBool3),
        CfgChar1(other.CfgChar1), CfgChar2(other.CfgChar2), CfgChar3(other.CfgChar3),
        CfgString1(std::move(other.CfgString1)), CfgString2(std::move(other.CfgString2)), CfgString3(std::move(other.CfgString3)) {
        ConfigRegistry[Identifier] = this;
        other.Identifier = "";
    }
#pragma endregion

#pragma region Operators
    ConfigObject& operator=(ConfigObject&& other) noexcept {           // =
        if (this == &other) return *this;

        ConfigRegistry.erase(Identifier);

        Identifier = std::move(other.Identifier);
        CfgInt1 = other.CfgInt1; CfgInt2 = other.CfgInt2; CfgInt3 = other.CfgInt3;
        CfgLong1 = other.CfgLong1; CfgLong2 = other.CfgLong2; CfgLong3 = other.CfgLong3;
        CfgFloat1 = other.CfgFloat1; CfgFloat2 = other.CfgFloat2; CfgFloat3 = other.CfgFloat3;
        CfgDouble1 = other.CfgDouble1; CfgDouble2 = other.CfgDouble2; CfgDouble3 = other.CfgDouble3;
        CfgBool1 = other.CfgBool1; CfgBool2 = other.CfgBool2; CfgBool3 = other.CfgBool3;
        CfgChar1 = other.CfgChar1; CfgChar2 = other.CfgChar2; CfgChar3 = other.CfgChar3;
        CfgString1 = std::move(other.CfgString1); CfgString2 = std::move(other.CfgString2); CfgString3 = std::move(other.CfgString3);

        ConfigRegistry[Identifier] = this;

        return *this;
    }
    ConfigObject* operator=(ConfigObject* other) noexcept {            // =
        if (this == other) return this;

        if (other == nullptr) return this;

        ConfigRegistry.erase(Identifier);

        Identifier = std::move(other->Identifier);
        CfgInt1 = other->CfgInt1; CfgInt2 = other->CfgInt2; CfgInt3 = other->CfgInt3;
        CfgLong1 = other->CfgLong1; CfgLong2 = other->CfgLong2; CfgLong3 = other->CfgLong3;
        CfgFloat1 = other->CfgFloat1; CfgFloat2 = other->CfgFloat2; CfgFloat3 = other->CfgFloat3;
        CfgDouble1 = other->CfgDouble1; CfgDouble2 = other->CfgDouble2; CfgDouble3 = other->CfgDouble3;
        CfgBool1 = other->CfgBool1; CfgBool2 = other->CfgBool2; CfgBool3 = other->CfgBool3;
        CfgChar1 = other->CfgChar1; CfgChar2 = other->CfgChar2; CfgChar3 = other->CfgChar3;
        CfgString1 = std::move(other->CfgString1); CfgString2 = std::move(other->CfgString2); CfgString3 = std::move(other->CfgString3);

        ConfigRegistry[Identifier] = this;

        return this;
    }
    
    bool operator==(const ConfigObject& other) const {                 // ==
        return this->Identifier == other.Identifier;
    }

    bool operator==(const ConfigObject* other) const {                 // ==
        if (other == nullptr) return false;
        return this->Identifier == other->Identifier;
    }

#pragma endregion

#pragma region Functions
    // Static
    static ConfigObject* FindByID(const std::string& Identifier);
    // Instance
    const std::string& GetID();
    const std::string& GetID() const;
    void AddField(const std::string& name, const std::variant<int, long, float, double, bool, char, std::string>& type);
    void RemoveField(const std::string& name);
    bool FieldExists(const std::string& name) const;
    template <typename T>
    const std::optional<T> GetField(const std::string& fieldName) {
        std::string prefixedName = "Custom_" + fieldName;
        if (!this->FieldExists(prefixedName))
            throw std::runtime_error("Field does not exist. Perhaps the field name is invalid?");            // Another possible cause is that the names being looked for aren't in the
        if (auto value = std::get_if<T>(&this->CustomFields[prefixedName]))                                  // config anymore in which case, the config should be regenerated
            return *value;
        else
            throw std::runtime_error("Field type mismatch. Requested type does not match the stored type."); // This means that you're requesting a type other than your requested field's type
    }                                                                                                        // Try switching the type you request
#pragma endregion
};

class ConObjMan {
public:
    static std::string FormatIntGroup(ConfigObject& obj);
    static bool IsIntInit(ConfigObject& obj);
    static std::string FormatLongGroup(ConfigObject& obj);
    static bool IsLongInit(ConfigObject& obj);

    static std::string FormatFloatGroup(ConfigObject& obj);
    static bool IsFloatInit(ConfigObject& obj);
    static std::string FormatDoubleGroup(ConfigObject& obj);
    static bool IsDoubleInit(ConfigObject& obj);

    static std::string FormatBoolGroup(ConfigObject& obj);
    static bool IsBoolInit(ConfigObject& obj);

    static std::string FormatCharGroup(ConfigObject& obj);
    static bool IsCharInit(ConfigObject& obj);
    static std::string FormatStringGroup(ConfigObject& obj);
    static bool IsStringInit(ConfigObject& obj);
};

#endif

// What I've learned:
//
// Conventionally, the constructor and destructor for any new objects that you create should be defined in the header
// There are cases where conventionally speaking, it is okay to define a function in the header
// std::unordered_map is very similar to C#'s dictionaries
// std::format is a godsend and I will forever love it
//
// Moving an object means transfering its data to a different object without copying the object itself
// Unlike c#, all different types of constructors need to be accounted for when dealing with semi-complex objects
//
// Copies are entirely new objects based off an existing one
// When a copy is made, the copy is initialized with all the values of its reference object
//
// On the other hand, copy assignments are when you take the properties of an existing object and copy them onto another existing object.
// Because of that, there's always the possibility that self assignment is done
// That's why I've gotta handle that in the copy assignment constructor in the first few lines
// Self assignment isn't practical and should be avoided
//
// Then there's move constructors which are similar to copy assignments but instead of 2 normal objects, it works with temporary ones.
// More specifically, it takes the properties from a temporary object and moves it to the other specified object
//
// Although there's a bunch of massive code blocks and they're hard to understand at first glance, 
// all that they do is handle all of the properties tied to each instance of each the object
// as well as the ID which is needed to effectively find the object
//
// This is all really weird
//
// Alongside a large variety of constructors, you also have to include operators or else they won't work.
// For example, if I have 2 of this object and I wanna check if they're the same, there has to be something that handles operators in the header file
// More specifically, in this case, I'd need smth that would take care of == operators (Although I'll be handling other ones too.)
// In operators (and really anything for that matter), you should always check for nullptrs
//
// Const member functions are used when you want to make a member function that can't modify the values of member variables
// "bool IsIntInit() const;" (Member variables cannot be modified in this method)
// "bool IsIntInit();" (Member variables can be modified in this method)
//
//