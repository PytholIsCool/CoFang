#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "PyConfig/API_Includes.h"

static const std::string CfgPath = R"(C:\1TestFig\AdditionalFile\UserData.pycfg)";

int main() {
    if (File::Exists(CfgPath)) {
        ConfigObject TestObj("User");

        Serialization::Deserialize(TestObj, CfgPath);

        std::cout << "CustomInt is: " << *TestObj.GetField<int>("CustomInt") << "\n"; // Works
        std::cout << "CustomLong is: " << *TestObj.GetField<long>("CustomLong") << "\n"; // Works

        std::cout << "CustomFloat is: " << *TestObj.GetField<float>("CustomFloat") << "\n"; // Works
        std::cout << "CustomDouble is: " << *TestObj.GetField<double>("CustomDouble") << "\n"; // Works

        std::cout << "CustomBool1 is: " << *TestObj.GetField<bool>("CustomBool1") << "\n"; // Works
        std::cout << "CustomBool2 is: " << *TestObj.GetField<bool>("CustomBool2") << "\n"; // Works

        std::cout << "CustomChar is: " << *TestObj.GetField<char>("CustomChar") << "\n"; // Works
        std::cout << "CustomString is: " << *TestObj.GetField<std::string>("CustomString") << "\n"; // Works
        std::cout << "StringWithFloat is: " << *TestObj.GetField<std::string>("StringWithFloat") << "\n"; // Works

        std::cout << "Int1 is: " << std::to_string(*TestObj.CfgInt1) << "\n";

        std::cout << "Long1 is: " << std::to_string(*TestObj.CfgLong1) << "\n";

        std::cout << "Float1 is: " << std::to_string(*TestObj.CfgFloat1) << "\n";

        std::cout << "Double1 is: " << std::to_string(*TestObj.CfgDouble1) << "\n";

        std::cout << "Bool1 is: " << std::to_string(*TestObj.CfgBool1) << "\n";

        std::cout << "Char1 is: " << std::string(1, *TestObj.CfgChar1) << "\n";

        std::cout << "String1 is: " << *TestObj.CfgString1 << "\n";

        return 0;
    }

    ConfigObject TestObj("User");
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

    TestObj.CfgInt1 = 16;
    TestObj.CfgLong1 = 17;

    TestObj.CfgFloat1 = 1.72;
    TestObj.CfgDouble1 = 5.4;

    TestObj.CfgBool1 = true;

    TestObj.CfgChar1 = 'S';
    TestObj.CfgString1 = "TestVal";

    Serialization::Serialize(TestObj, CfgPath);

    return 0;
}