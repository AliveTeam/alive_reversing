#include "../AliveLibCommon/stdafx_common.h"
#include "alive_api.hpp"
#include "SDL.h"
#include "logger.hpp"
#include "AOTlvs.hpp"
#include <gmock/gmock.h>
#include "../AliveLibAE/DebugHelpers.hpp"

const std::string kAEDir = "C:\\GOG Games\\Abes Exoddus\\";
const std::string kAODir = "C:\\GOG Games\\Abes Oddysee\\";
const std::string kAETestLvl = "ba.lvl";

const std::vector<std::string> kAELvls = 
{
    "ba.lvl",
    "bm.lvl",
    "br.lvl",
    "bw.lvl",
    "cr.lvl",
    "fd.lvl",
    "mi.lvl",
    "ne.lvl",
    "pv.lvl",
    "st.lvl",
    "sv.lvl",
};

const std::vector<std::string> kAOLvls =
{
    "c1.lvl",
    "d1.lvl",
    "d2.lvl",
    "d7.lvl",
    "e1.lvl",
    "e2.lvl",
    "f1.lvl",
    "f2.lvl",
    "f4.lvl",
    "l1.lvl",
    "r1.lvl",
    "r2.lvl",
    "r6.lvl",
    "s1.lvl",
};


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
    // finished lvl's: mi.lvl, ne.lvl, cr.lvl
    // remaining lvl's: ba.lvl, bm.lvl, bw.lvl, fd.lvl, br.lvl, pv.lvl st.lvl, sv.lvl
    auto ret = AliveAPI::ExportPathBinaryToJson("OutputAE.json", AEPath(kAETestLvl), 4); // 16 paths
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);
}


TEST(alive_api, ExportPathBinaryToJsonAO)
{
    auto ret = AliveAPI::ExportPathBinaryToJson("OutputAO.json", AOPath("R1.LVL"), 19);
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);
}

TEST(alive_api, ImportPathJsonToBinaryAO)
{
    auto ret =  AliveAPI::ImportPathJsonToBinary("OutputAO.json", AOPath("R1.LVL"), "newAO.lvl", {});
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);

    const auto ogR1 = FS::ReadFile(AOPath("R1.LVL"));
    const auto rewrittenR1 = FS::ReadFile("newAO.lvl");
    ASSERT_EQ(ogR1, rewrittenR1);
}

TEST(alive_api, ImportPathJsonToBinaryAE)
{
    auto ret = AliveAPI::ImportPathJsonToBinary("OutputAE.json", AEPath(kAETestLvl), "newAE.lvl", {});
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);

    const auto ogLVL = FS::ReadFile(AEPath(kAETestLvl));
    const auto rewrittenLVL = FS::ReadFile("newAE.lvl");
    ASSERT_EQ(ogLVL, rewrittenLVL);
}

TEST(alive_api, EnumeratePathsAO)
{
    auto ret = AliveAPI::EnumeratePaths(AOPath("R1.LVL"));
    ASSERT_EQ(ret.pathBndName, "R1PATH.BND");
    const std::vector<int> paths {15, 16, 18, 19};
    ASSERT_EQ(ret.paths, paths);
    ASSERT_EQ(ret.mResult, AliveAPI::Error::None);
}

TEST(alive_api, ReSaveAllPathsAO)
{
    for (const auto& lvl : kAOLvls)
    {
        auto ret = AliveAPI::EnumeratePaths(AOPath(lvl));
        ASSERT_EQ(ret.mResult, AliveAPI::Error::None);

        for (int path : ret.paths)
        {
            const std::string jsonName = "OutputAO_" + lvl + "_" + std::to_string(path) + ".json";
            LOG_INFO("Save " << jsonName);
            auto exportRet = AliveAPI::ExportPathBinaryToJson(jsonName, AOPath(lvl), path);
            ASSERT_EQ(exportRet.mResult, AliveAPI::Error::None);

            const std::string lvlName = "OutputAO_" + lvl + "_" + std::to_string(path) + ".lvl";
            LOG_INFO("Resave " << lvlName);
            auto importRet = AliveAPI::ImportPathJsonToBinary(jsonName, AOPath(lvl), lvlName, {});
            ASSERT_EQ(importRet.mResult, AliveAPI::Error::None);

            const auto originalLvlBytes = FS::ReadFile(AOPath(lvl));
            const auto resavedLvlBytes = FS::ReadFile(lvlName);
            ASSERT_EQ(originalLvlBytes, resavedLvlBytes);
        }
    }
}


TEST(alive_api, ReSaveAllPathsAE)
{
    for (const auto& lvl : kAELvls)
    {
        auto ret = AliveAPI::EnumeratePaths(AEPath(lvl));
        ASSERT_EQ(ret.mResult, AliveAPI::Error::None);

        for (int path : ret.paths)
        {
            const std::string jsonName = "OutputAE_" + lvl + "_" + std::to_string(path) + ".json";
            LOG_INFO("Save " << jsonName);
            auto exportRet = AliveAPI::ExportPathBinaryToJson(jsonName, AEPath(lvl), path);
            ASSERT_EQ(exportRet.mResult, AliveAPI::Error::None);

            const std::string lvlName = "OutputAE_" + lvl + "_" + std::to_string(path) + ".lvl";
            LOG_INFO("Resave " << lvlName);
            auto importRet = AliveAPI::ImportPathJsonToBinary(jsonName, AEPath(lvl), lvlName, {});
            ASSERT_EQ(importRet.mResult, AliveAPI::Error::None);

            const auto originalLvlBytes = FS::ReadFile(AEPath(lvl));
            const auto resavedLvlBytes = FS::ReadFile(lvlName);
            ASSERT_EQ(originalLvlBytes, resavedLvlBytes);
        }
    }
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
