#include "alive_api.hpp"
#include "SDL.h"
#include "logger.hpp"
#include "AOTlvs.hpp"
#include <gmock/gmock.h>

TEST(alive_api, ExportPathBinaryToJson)
{

    auto ret = AliveAPI::ExportPathBinaryToJson("Output.json", "C:\\GOG Games\\Abes Oddysee\\R6.LVL", 6);
 
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);

}

TEST(alive_api, EnumeratePaths)
{
    auto ret = AliveAPI::EnumeratePaths("C:\\GOG Games\\Abes Oddysee\\R1.LVL");
    ASSERT_EQ(ret.pathBndName, "R1PATH.BND");
    const std::vector<int> paths {15, 16, 18, 19};
    ASSERT_EQ(ret.paths, paths);
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);
}

// Get version

// Import

// Upgrade

TEST(alive_api, tlv_reflection)
{
    TypesCollection types;

    AO::Path_Hoist tlv = {};
    std::unique_ptr<TlvObjectBaseAO> pHoist = types.MakeTlv(AO::TlvTypes::Hoist_3, &tlv);

    auto obj = pHoist->InstanceToJson(types);
    pHoist->InstanceFromJson(types, obj);
    pHoist->StructureToJson();
}

int main(int argc, char* argv[])
{
#if _WIN32
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::SetConsoleTitleA("Debug Console");
    ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);
#endif

    ::testing::InitGoogleTest(&argc, argv);
    const auto ret = RUN_ALL_TESTS();
    return ret;
}
