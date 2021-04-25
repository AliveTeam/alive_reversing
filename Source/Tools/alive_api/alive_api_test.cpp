#include "../AliveLibCommon/stdafx_common.h"
#include "alive_api.hpp"
#include "SDL.h"
#include "logger.hpp"
#include "AOTlvs.hpp"
#include <gmock/gmock.h>
#include "../AliveLibAE/DebugHelpers.hpp"

const std::string kAEDir = "C:\\GOG Games\\Abes Exoddus\\";
const std::string kAODir = "C:\\GOG Games\\Abes Oddysee\\";
const std::string kAETestLvl = "pv.lvl";

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
    AliveAPI::ExportPathBinaryToJson("OutputAE.json", AEPath(kAETestLvl), 14);
}


TEST(alive_api, ExportPathBinaryToJsonAO)
{
    AliveAPI::ExportPathBinaryToJson("OutputAO.json", AOPath("R1.LVL"), 19);
}

TEST(alive_api, ImportPathJsonToBinaryAO)
{
    AliveAPI::ImportPathJsonToBinary("OutputAO.json", AOPath("R1.LVL"), "newAO.lvl", {});

    const auto ogR1 = FS::ReadFile(AOPath("R1.LVL"));
    ASSERT_NE(ogR1.size(), 0u);

    const auto rewrittenR1 = FS::ReadFile("newAO.lvl");
    ASSERT_NE(rewrittenR1.size(), 0u);

    ASSERT_EQ(ogR1, rewrittenR1);
}

TEST(alive_api, ImportPathJsonToBinaryAE)
{
    AliveAPI::ImportPathJsonToBinary("OutputAE.json", AEPath(kAETestLvl), "newAE.lvl", {});

    const auto ogLVL = FS::ReadFile(AEPath(kAETestLvl));
    ASSERT_NE(ogLVL.size(), 0u);

    const auto rewrittenLVL = FS::ReadFile("newAE.lvl");
    ASSERT_NE(rewrittenLVL.size(), 0u);

    if (ogLVL != rewrittenLVL)
    {
        AliveAPI::DebugDumpTlvs("old/", AEPath(kAETestLvl), 14);
        AliveAPI::DebugDumpTlvs("new/", "newAE.lvl", 14);
    }
    ASSERT_EQ(ogLVL, rewrittenLVL);
}

TEST(alive_api, EnumeratePathsAO)
{
    auto ret = AliveAPI::EnumeratePaths(AOPath("R1.LVL"));
    ASSERT_EQ(ret.pathBndName, "R1PATH.BND");
    const std::vector<s32> paths {15, 16, 18, 19};
    ASSERT_EQ(ret.paths, paths);
}

TEST(alive_api, ReSaveAllPathsAO)
{
    for (const auto& lvl : kAOLvls)
    {
        auto ret = AliveAPI::EnumeratePaths(AOPath(lvl));

        for (s32 path : ret.paths)
        {
            const std::string jsonName = "OutputAO_" + lvl + "_" + std::to_string(path) + ".json";
            LOG_INFO("Save " << jsonName);
            AliveAPI::ExportPathBinaryToJson(jsonName, AOPath(lvl), path);

            const std::string lvlName = "OutputAO_" + lvl + "_" + std::to_string(path) + ".lvl";
            LOG_INFO("Resave " << lvlName);
            AliveAPI::ImportPathJsonToBinary(jsonName, AOPath(lvl), lvlName, {});

            const auto originalLvlBytes = FS::ReadFile(AOPath(lvl));
            ASSERT_NE(originalLvlBytes.size(), 0u);
            const auto resavedLvlBytes = FS::ReadFile(lvlName);
            ASSERT_NE(resavedLvlBytes.size(), 0u);

            if (originalLvlBytes != resavedLvlBytes)
            {
                AliveAPI::DebugDumpTlvs("old/", AOPath(lvl), path);
                AliveAPI::DebugDumpTlvs("new/", lvlName, path);
            }
            ASSERT_EQ(originalLvlBytes, resavedLvlBytes);
        }
    }
}

TEST(alive_api, ReSaveAllPathsAE)
{
    for (const auto& lvl : kAELvls)
    {
        auto ret = AliveAPI::EnumeratePaths(AEPath(lvl));

        for (s32 path : ret.paths)
        {
            const std::string jsonName = "OutputAE_" + lvl + "_" + std::to_string(path) + ".json";
            LOG_INFO("Save " << jsonName);
            AliveAPI::ExportPathBinaryToJson(jsonName, AEPath(lvl), path);

            const std::string lvlName = "OutputAE_" + lvl + "_" + std::to_string(path) + ".lvl";
            LOG_INFO("Resave " << lvlName);
            AliveAPI::ImportPathJsonToBinary(jsonName, AEPath(lvl), lvlName, {});

            const auto originalLvlBytes = FS::ReadFile(AEPath(lvl));
            ASSERT_NE(originalLvlBytes.size(), 0u);

            const auto resavedLvlBytes = FS::ReadFile(lvlName);
            ASSERT_NE(resavedLvlBytes.size(), 0u);

            if (originalLvlBytes != resavedLvlBytes)
            {
                AliveAPI::DebugDumpTlvs("old/", AEPath(lvl), path);
                AliveAPI::DebugDumpTlvs("new/", lvlName, path);
            }
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
    std::unique_ptr<TlvObjectBase> pHoist = types.MakeTlvAO(AO::TlvTypes::Hoist_3, &tlv, 99);

    auto obj = pHoist->InstanceToJson(types);
    pHoist->InstanceFromJson(types, obj);
    pHoist->StructureToJson();
    ASSERT_EQ(pHoist->InstanceNumber(), 99);
}

/*
TEST(json_upgrade, upgrade_rename_structure)
{
    AliveAPI::JsonUpgradeResult r = AliveAPI::UpgradePathJson("rename_field.json");
    ASSERT_EQ(r.mResult, AliveAPI::UpgradeError::None);
}
*/

class ArgsAdapter
{
public:
    ArgsAdapter(s32 argc, s8* argv[])
    {
        for (s32 i = 0; i < argc; i++)
        {
            mArgs.push_back(argv[i]);
        }

    }

    void Add(const std::string& arg)
    {
        mArgs.push_back(arg);
    }

    s32 ArgC() const
    {
        return static_cast<s32>(mArgs.size());
    }

    std::unique_ptr<s8*[]> ArgV() const
    {
        auto ptr = std::make_unique<s8* []>(mArgs.size());

        s32 i = 0;
        for (const auto& arg : mArgs)
        {
            ptr[i++] = const_cast<s8*>(arg.c_str());
        }
        return ptr;
    }

private:
    std::vector<std::string> mArgs;
};

s32 main(s32 argc, s8* argv[])
{
    ArgsAdapter args(argc, argv);
    args.Add("--gtest_catch_exceptions=0");

    s32 newArgc = args.ArgC();
    auto newArgv = args.ArgV();

    ::testing::InitGoogleTest(&newArgc, newArgv.get());
    const auto ret = RUN_ALL_TESTS();
    return ret;
}
