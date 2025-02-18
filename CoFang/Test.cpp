#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "CoFang/API_Includes.h"
using SpecialFolder = Path::SpecialFolder;

static const std::string CfgPath = R"(C:\1TestFig\AdditionalFile\UserData.pyces)";
// Crude example of what can be done with the api
int main() {
    if (File::Exists(CfgPath)) {
        ConfigObject TestObj("User");
        ConfigObject Test("Other");

        Serialization::Deserialize(TestObj, CfgPath);
        Serialization::Deserialize(Test, CfgPath);

        /*std::cout << "AppData: " << Path::GetFolderPath(SpecialFolder::AppData) << "\n";
        std::cout << "Temp: " << Path::GetFolderPath(SpecialFolder::Temp) << "\n";
        std::cout << "Local: " << Path::GetFolderPath(SpecialFolder::Local) << "\n";
        std::cout << "LocalLow: " << Path::GetFolderPath(SpecialFolder::LocalLow) << "\n";
        std::cout << "Roaming: " << Path::GetFolderPath(SpecialFolder::Roaming) << "\n";
        std::cout << "Application: " << Path::GetFolderPath(SpecialFolder::Application) << "\n";*/

        std::cout << "Test is: " << Test.GetField<std::string>("Test") << "\n"; //Works

        std::cout << "CustomInt is: " << TestObj.GetField<int>("CustomInt") << "\n"; // Works
        std::cout << "CustomLong is: " << TestObj.GetField<long>("CustomLong") << "\n"; // Works

        std::cout << "CustomFloat is: " << TestObj.GetField<float>("CustomFloat") << "\n"; // Works
        std::cout << "CustomDouble is: " << TestObj.GetField<double>("CustomDouble") << "\n"; // Works

        std::cout << "CustomBool1 is: " << TestObj.GetField<bool>("CustomBool1") << "\n"; // Works
        std::cout << "CustomBool2 is: " << TestObj.GetField<bool>("CustomBool2") << "\n"; // Works

        std::cout << "CustomChar is: " << TestObj.GetField<char>("CustomChar") << "\n"; // Works
        std::cout << "CustomString is: " << TestObj.GetField<std::string>("CustomString") << "\n"; // Works

        return 0;
    }

    ConfigObject TestObj("User");
    ConfigObject Test("Other");

    Test << std::make_pair("Test", "TestVal") << std::make_pair("Test2", "TestVal");

    TestObj << std::make_pair("AnotherCustomString", "TestValueLol");
    TestObj << std::make_pair("TestInteger", 16);

    TestObj << std::make_pair("String1", "I really ") << std::make_pair("String2", "really ") << std::make_pair("String 3", "hate c++");

    TestObj.AddField("CustomInt", 16);
    TestObj.AddField("CustomLong", 999999999L);

    TestObj.AddField("CustomFloat", 1.72f);
    TestObj.AddField("Pi", std::numbers::pi);
    TestObj.AddField("CustomDouble", 5.4444444);

    TestObj.AddField("CustomBool1", true);
    TestObj.AddField("CustomBool2", false);

    TestObj.AddField("CustomChar", 'S');
    TestObj.AddField("CustomString", "Pythol");
    TestObj.AddField("StringWithFloat", "1.720000f");

    Serialization::Serialize(TestObj, CfgPath);
    Serialization::Serialize(Test, CfgPath);

    TestObj.RemoveField("StringWithFloat");

    Serialization::Serialize(TestObj, CfgPath);

    return 0;
}