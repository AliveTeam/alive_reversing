#include "relive_api.hpp"

#include "AOTlvs.hpp"
#include "TypesCollectionAO.hpp"

#include "../AliveLibAE/DebugHelpers.hpp"

#include "../AliveLibCommon/stdafx_common.h"
#include "../AliveLibCommon/logger.hpp"

#include "SDL.h"

#include <gmock/gmock.h>

#include <array>
#include <memory>
#include <sstream>
#include <string_view>
#include <string>
#include <vector>

namespace {

using namespace std::literals::string_view_literals;

constexpr std::string_view kAEDir = "C:\\GOG Games\\Abes Exoddus\\"sv;
constexpr std::string_view kAODir = "C:\\GOG Games\\Abes Oddysee\\"sv;
constexpr std::string_view kAETestLvl = "pv.lvl"sv;

constexpr std::array kAELvls{
    "ba.lvl"sv,
    "bm.lvl"sv,
    "br.lvl"sv,
    "bw.lvl"sv,
    "cr.lvl"sv,
    "fd.lvl"sv,
    "mi.lvl"sv,
    "ne.lvl"sv,
    "pv.lvl"sv,
    "st.lvl"sv,
    "sv.lvl"sv};

constexpr std::array kAOLvls{
    "c1.lvl"sv,
    "d1.lvl"sv,
    "d2.lvl"sv,
    "d7.lvl"sv,
    "e1.lvl"sv,
    "e2.lvl"sv,
    "f1.lvl"sv,
    "f2.lvl"sv,
    "f4.lvl"sv,
    "l1.lvl"sv,
    "r1.lvl"sv,
    "r2.lvl"sv,
    "r6.lvl"sv,
    "s1.lvl"sv};

template <typename... Ts>
[[nodiscard]] std::string concat(const Ts&... xs)
{
    static std::ostringstream oss;
    oss.str("");

    (oss << ... << xs);

    return oss.str();
}

[[nodiscard]] std::string AEPath(std::string_view fileName)
{
    return concat(kAEDir, fileName);
}

[[nodiscard]] std::string AOPath(std::string_view fileName)
{
    return concat(kAODir, fileName);
}

[[nodiscard]] std::vector<u8>& getStaticFileBuffer()
{
    static std::vector<u8> result;
    return result;
}

[[nodiscard]] std::vector<u8>& readFileIntoStaticFileBuffer(const std::string& filePath)
{
    std::vector<u8>& buf = getStaticFileBuffer();

    if (!FS::ReadFileInto(buf, filePath))
    {
        std::cerr << "Failure reading file '" << filePath << "'\n";
        std::abort();
    }

    return buf;
}

TEST(alive_api, ExportPathBinaryToJsonAE)
{
    ReliveAPI::ExportPathBinaryToJson(getStaticFileBuffer(), "OutputAE.json", AEPath(kAETestLvl), 14);
}

TEST(alive_api, ExportPathBinaryToJsonAO)
{
    ReliveAPI::ExportPathBinaryToJson(getStaticFileBuffer(), "OutputAO.json", AOPath("R1.LVL"), 19);
}

TEST(alive_api, ImportPathJsonToBinaryAO)
{
    ReliveAPI::ImportPathJsonToBinary(getStaticFileBuffer(), "OutputAO.json", AOPath("R1.LVL"), "newAO.lvl", {});

    const auto ogR1 = readFileIntoStaticFileBuffer(AOPath("R1.LVL"));
    ASSERT_NE(ogR1.size(), 0u);

    const auto rewrittenR1 = readFileIntoStaticFileBuffer("newAO.lvl");
    ASSERT_NE(rewrittenR1.size(), 0u);

    ASSERT_EQ(ogR1, rewrittenR1);
}

TEST(alive_api, ImportPathJsonToBinaryAE)
{
    ReliveAPI::ImportPathJsonToBinary(getStaticFileBuffer(), "OutputAE.json", AEPath(kAETestLvl), "newAE.lvl", {});

    const auto ogLVL = readFileIntoStaticFileBuffer(AEPath(kAETestLvl));
    ASSERT_NE(ogLVL.size(), 0u);

    const auto rewrittenLVL = readFileIntoStaticFileBuffer("newAE.lvl");
    ASSERT_NE(rewrittenLVL.size(), 0u);

    if (ogLVL != rewrittenLVL)
    {
        ReliveAPI::DebugDumpTlvs("old/", AEPath(kAETestLvl), 14);
        ReliveAPI::DebugDumpTlvs("new/", "newAE.lvl", 14);
    }
    ASSERT_EQ(ogLVL, rewrittenLVL);
}

TEST(alive_api, EnumeratePathsAO)
{
    auto ret = ReliveAPI::EnumeratePaths(getStaticFileBuffer(), AOPath("R1.LVL"));
    ASSERT_EQ(ret.pathBndName, "R1PATH.BND");
    const std::vector<s32> paths{15, 16, 18, 19};
    ASSERT_EQ(ret.paths, paths);
}

TEST(alive_api, ReSaveAllPathsAO)
{
    for (const auto& lvl : kAOLvls)
    {
        auto ret = ReliveAPI::EnumeratePaths(getStaticFileBuffer(), AOPath(lvl));

        for (s32 path : ret.paths)
        {
            const std::string jsonName = concat("OutputAO_", lvl, '_', path, ".json");
            LOG_INFO("Save " << jsonName);
            ReliveAPI::ExportPathBinaryToJson(getStaticFileBuffer(), jsonName, AOPath(lvl), path);

            const std::string lvlName = concat("OutputAO_", lvl, '_', path, ".lvl");
            LOG_INFO("Resave " << lvlName);
            ReliveAPI::ImportPathJsonToBinary(getStaticFileBuffer(), jsonName, AOPath(lvl), lvlName, {});

            const auto originalLvlBytes = readFileIntoStaticFileBuffer(AOPath(lvl));
            ASSERT_NE(originalLvlBytes.size(), 0u);
            const auto resavedLvlBytes = readFileIntoStaticFileBuffer(lvlName);
            ASSERT_NE(resavedLvlBytes.size(), 0u);

            if (originalLvlBytes != resavedLvlBytes)
            {
                ReliveAPI::DebugDumpTlvs("old/", AOPath(lvl), path);
                ReliveAPI::DebugDumpTlvs("new/", lvlName, path);
            }
            ASSERT_EQ(originalLvlBytes, resavedLvlBytes);
        }
    }
}

TEST(alive_api, ReSaveAllPathsAE)
{
    for (const auto& lvl : kAELvls)
    {
        auto ret = ReliveAPI::EnumeratePaths(getStaticFileBuffer(), AEPath(lvl));

        for (s32 path : ret.paths)
        {
            const std::string jsonName = concat("OutputAE_", lvl, '_', path, ".json");
            LOG_INFO("Save " << jsonName);
            ReliveAPI::ExportPathBinaryToJson(getStaticFileBuffer(), jsonName, AEPath(lvl), path);

            const std::string lvlName = concat("OutputAE_", lvl, '_', path, ".lvl");
            LOG_INFO("Resave " << lvlName);
            ReliveAPI::ImportPathJsonToBinary(getStaticFileBuffer(), jsonName, AEPath(lvl), lvlName, {});

            const auto originalLvlBytes = readFileIntoStaticFileBuffer(AEPath(lvl));
            ASSERT_NE(originalLvlBytes.size(), 0u);

            const auto resavedLvlBytes = readFileIntoStaticFileBuffer(lvlName);
            ASSERT_NE(resavedLvlBytes.size(), 0u);

            if (originalLvlBytes != resavedLvlBytes)
            {
                ReliveAPI::DebugDumpTlvs("old/", AEPath(lvl), path);
                ReliveAPI::DebugDumpTlvs("new/", lvlName, path);
            }
            ASSERT_EQ(originalLvlBytes, resavedLvlBytes);
        }
    }
}

// Get version

// Upgrade

TEST(alive_api, tlv_reflection)
{
    TypesCollectionAO types;

    AO::Path_Hoist tlv = {};
    std::unique_ptr<TlvObjectBase> pHoist = types.MakeTlvAO(AO::TlvTypes::Hoist_3, &tlv, 99);

    auto obj = pHoist->InstanceToJson(types);
    (void) pHoist->InstanceFromJson(types, obj); // TODO: check return value?
    (void) pHoist->StructureToJson();            // TODO: check return value?
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

    [[nodiscard]] s32 ArgC() const
    {
        return static_cast<s32>(mArgs.size());
    }

    [[nodiscard]] std::unique_ptr<s8* []> ArgV() const
    {
        auto ptr = std::make_unique<s8*[]>(mArgs.size());

        s32 i = 0;
        for (const auto& arg : mArgs)
        {
            ptr[i++] = const_cast<s8*>(arg.c_str());
        }
        return ptr;
    }

    private : std::vector<std::string> mArgs;
};

} // namespace

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
