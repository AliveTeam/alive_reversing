#include "../AliveLibCommon/stdafx_common.h"
#include "alive_api.hpp"
#include "SDL.h"
#include "logger.hpp"
#include "AOTlvs.hpp"
#include <gmock/gmock.h>
#include "../AliveLibAE/DebugHelpers.hpp"

const std::string kAEDir = "C:\\GOG Games\\Abes Exoddus\\";
const std::string kAODir = "C:\\GOG Games\\Abes Oddysee\\";

static std::string AEPath(const std::string& fileName)
{
    return kAEDir + fileName;
}

static std::string AOPath(const std::string& fileName)
{
    return kAODir + fileName;
}

TEST(alive_api, ExportPathBinaryToJsonAE)
{
    auto ret = AliveAPI::ExportPathBinaryToJson("OutputAE.json", AEPath("BA.LVL"), 16);
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);
}


TEST(alive_api, ExportPathBinaryToJsonAO)
{
    auto ret = AliveAPI::ExportPathBinaryToJson("OutputAO.json", AOPath("R1.LVL"), 19);
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);
}

TEST(alive_api, ImportPathJsonToBinaryAO)
{
    auto ret =  AliveAPI::ImportPathJsonToBinary("OutputAO.json", AOPath("R1.LVL"), {});
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);

    const auto ogR1 = FS::ReadFile(AOPath("R1.LVL"));
    const auto rewrittenR1 = FS::ReadFile("new.lvl");
    ASSERT_EQ(ogR1, rewrittenR1);
}

TEST(alive_api, ImportPathJsonToBinaryAE)
{
    auto ret = AliveAPI::ImportPathJsonToBinary("OutputAE.json", AEPath("BA.LVL"), {});
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);

    const auto ogR1 = FS::ReadFile(AEPath("BA.LVL"));
    const auto rewrittenR1 = FS::ReadFile("new.lvl");
    ASSERT_EQ(ogR1, rewrittenR1);
}

TEST(alive_api, EnumeratePathsAO)
{
    auto ret = AliveAPI::EnumeratePaths(AOPath("R1.LVL"));
    ASSERT_EQ(ret.pathBndName, "R1PATH.BND");
    const std::vector<int> paths {15, 16, 18, 19};
    ASSERT_EQ(ret.paths, paths);
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);
}

// Get version

// Upgrade

TEST(alive_api, tlv_reflection)
{
    TypesCollection types(Game::AO);

    AO::Path_Hoist tlv = {};
    std::unique_ptr<TlvObjectBase> pHoist = types.MakeTlvAO(AO::TlvTypes::Hoist_3, &tlv);

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
