#include "../AliveLibCommon/stdafx_common.h"
#include "alive_api.hpp"
#include "SDL.h"
#include "logger.hpp"
#include "AOTlvs.hpp"
#include <gmock/gmock.h>
#include "../AliveLibAE/DebugHelpers.hpp"

TEST(alive_api, ExportPathBinaryToJsonAO)
{

    auto ret = AliveAPI::ExportPathBinaryToJson("Output.json", "C:\\GOG Games\\Abes Oddysee\\R1.LVL", 19);
 
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);

}

TEST(alive_api, ImportPathJsonToBinaryAO)
{
    auto ret =  AliveAPI::ImportPathJsonToBinary("Output.json", "C:\\GOG Games\\Abes Oddysee\\R1.LVL", {});
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);

    const auto ogR1 = FS::ReadFile("C:\\GOG Games\\Abes Oddysee\\R1.LVL");
    const auto rewrittenR1 = FS::ReadFile("new.lvl");
    ASSERT_EQ(ogR1, rewrittenR1);
}

TEST(alive_api, EnumeratePathsAO)
{
    auto ret = AliveAPI::EnumeratePaths("C:\\GOG Games\\Abes Oddysee\\R1.LVL");
    ASSERT_EQ(ret.pathBndName, "R1PATH.BND");
    const std::vector<int> paths {15, 16, 18, 19};
    ASSERT_EQ(ret.paths, paths);
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);
}

// Get version

// Upgrade

TEST(alive_api, tlv_reflection)
{
    TypesCollection types;

    AO::Path_Hoist tlv = {};
    std::unique_ptr<TlvObjectBase> pHoist = types.MakeTlv(AO::TlvTypes::Hoist_3, &tlv);

    auto obj = pHoist->InstanceToJson(types);
    pHoist->InstanceFromJson(types, obj);
    pHoist->StructureToJson();
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    const auto ret = RUN_ALL_TESTS();
    return ret;
}
