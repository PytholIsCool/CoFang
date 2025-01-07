#ifndef CONFIGOBJECT_H
#define CONFIGOBJECT_H

#include <iostream>
#include <string>
#include <optional>
#include <format>
#include <memory>
#include <unordered_map>
#include <variant>
// Entirely reworked this class as I wasn't happy with how over-engineered it was
class ConfigObject {
private:
    std::string Identifier;
    static std::unordered_map<std::string, ConfigObject*> ConfigRegistry;
public:
    std::unordered_map<std::string, std::variant<int, long, float, double, bool, char, std::string>> Fields;

#pragma region Constructors
    // Constructor
    ConfigObject(const std::string& id) : Identifier(id) {
        if (ConfigRegistry.find(id) != ConfigRegistry.end())
            throw std::runtime_error("Identifier already exists.");
        ConfigRegistry[id] = this;
    }
    // Destructor
    ~ConfigObject() {
        Fields.clear();
        ConfigRegistry.erase(Identifier);
    }
    // Copy handler
    ConfigObject(const ConfigObject& other) : Identifier(other.Identifier + "_copy"), Fields(other.Fields) {
        while (ConfigRegistry.contains(Identifier))                    // If the registry already contains the ID
            Identifier = Identifier.append("_copy");                   // Keep edding the _copy prefix until it can copy the object again
        ConfigRegistry[Identifier] = this;                             // Updates the ID so that the registry can differenciate between the 2
    }
    // Copy assignment handler
    ConfigObject& operator=(const ConfigObject& other) {               // Handles any copy assignments
        if (this == &other) return *this;                              // Prevents self assignments

        ConfigRegistry.erase(Identifier);                              // Erases the original object

        Identifier = other.Identifier + "_copy";                       // Gives a new ID to the copy
        while (ConfigRegistry.contains(Identifier))
            Identifier = Identifier.append("_copy");                   // Keep edding the _copy prefix until it can copy the object again

        Fields = other.Fields;
        ConfigRegistry[Identifier] = this;                             // Updates the registry

        return *this;
    }
    // Move handler
    ConfigObject(ConfigObject&& other) noexcept
        : Identifier(std::move(other.Identifier)), Fields(std::move(other.Fields)) {
        ConfigRegistry[Identifier] = this;
        other.Identifier = "";
    }
#pragma endregion

#pragma region Operators
    ConfigObject& operator=(ConfigObject&& other) noexcept {           // =
        if (this == &other) return *this;

        ConfigRegistry.erase(Identifier);

        Identifier = std::move(other.Identifier);
        Fields = std::move(other.Fields);

        ConfigRegistry[Identifier] = this;

        return *this;
    }
    ConfigObject* operator=(ConfigObject* other) noexcept {            // =
        if (this == other) return this;

        if (other == nullptr) return this;

        ConfigRegistry.erase(Identifier);

        Identifier = std::move(other->Identifier);
        Fields = std::move(other->Fields);

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
    ConfigObject& AddField(const std::string& name, const std::variant<int, long, float, double, bool, char, std::string>& type);
    void RemoveField(const std::string& name);
    bool FieldExists(const std::string& name) const;
    template <typename T>
    const T GetField(const std::string& fieldName) const {
        if (auto value = std::get_if<T>(&this->Fields.at(fieldName)))
            return *value;
        else
            throw std::runtime_error("Field type mismatch or field not found.");
    }
#pragma endregion
};

class ConfigObject;

ConfigObject& operator<<(ConfigObject& obj, const std::pair<std::string, std::variant<int, long, float, double, bool, char, std::string>>& field);
//template <typename T>
const int operator>>(ConfigObject& obj, const std::pair<std::string, int>& fieldInfo);
const long operator>>(ConfigObject& obj, const std::pair<std::string, long>& fieldInfo);

const float operator>>(ConfigObject& obj, const std::pair<std::string, float>& fieldInfo);
const double operator>>(ConfigObject& obj, const std::pair<std::string, double>& fieldInfo);

const bool operator>>(ConfigObject& obj, const std::pair<std::string, bool>& fieldInfo);

const char operator>>(ConfigObject& obj, const std::pair<std::string, char>& fieldInfo);
const std::string operator>>(ConfigObject& obj, const std::pair<std::string, std::string>& fieldInfo);


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
