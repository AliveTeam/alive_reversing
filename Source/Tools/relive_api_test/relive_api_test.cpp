#include "relive_api.hpp"

#include "TlvsAO.hpp"
#include "TypesCollectionAO.hpp"

#include "../../AliveLibAE/DebugHelpers.hpp"

#include "../../AliveLibCommon/stdafx_common.h"
#include "../../AliveLibCommon/logger.hpp"

#include "LvlReaderWriter.hpp"
#include "CamConverter.hpp"
#include "JsonModelTypes.hpp"
#include "ApiFg1Reader.hpp"

#include <gmock/gmock.h>

#include <array>
#include <memory>
#include <sstream>
#include <string_view>
#include <string>
#include <vector>

INITIALIZE_EASYLOGGINGPP

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

template<typename FnOnCam>
static void ForEachCamera(ReliveAPI::LvlReader& reader, FnOnCam onCam)
{
    std::vector<u8> tmpBuffer;
    for (s32 i = 0; i < reader.FileCount(); i++)
    {
        if (reader.FileNameAt(i).find(".CAM") != std::string::npos)
        {
            if (reader.ReadFileInto(tmpBuffer, reader.FileNameAt(i).c_str()))
            {
                ReliveAPI::ChunkedLvlFile camFile(tmpBuffer);
                onCam(reader.FileNameAt(i), camFile);
            }
        }
    }
}

static void TestCamAndFG1ExportImport(const std::string& strLvlName, bool allowFullFG1Blocks)
{
    // Save all CAM/FG1 data + reimport it to a copy of the LVL
    ReliveAPI::LvlReader reader(strLvlName.c_str());
    if (!reader.IsOpen())
    {
        throw ReliveAPI::IOReadException(strLvlName);
    }

    ReliveAPI::LvlWriter lvlWriter(strLvlName.c_str());
    if (!lvlWriter.IsOpen())
    {
        throw ReliveAPI::IOReadException(strLvlName);
    }

    LOG_INFO("Export + import LVL " << strLvlName);
    ForEachCamera(reader, [&](const std::string& camName, ReliveAPI::ChunkedLvlFile& camFile)
                  {
                      // Write out camera image and FG1 layers
                      ReliveAPI::CameraImageAndLayers convertedData;
                      ReliveAPI::CamConverter converter(camFile, convertedData);
                      ReliveAPI::ApiFG1Reader::DebugSave(camName, convertedData);

                      // Re-create the CAM file from new using the Base64 PNG data
                      std::vector<u8> importBuffer;
                      ReliveAPI::Detail::ImportCameraAndFG1(importBuffer, lvlWriter, camName, convertedData, allowFullFG1Blocks);
                  });
    // Close the reader first
    reader.Close();

    // Now write out the new LVL with the rewritten cam/FG1 data
    std::string newLvlName = strLvlName + std::string("_new.lvl");
    std::vector<u8> tmpBuffer;
    if (!lvlWriter.Save(tmpBuffer, newLvlName.c_str()))
    {
        throw ReliveAPI::IOWriteException(newLvlName.c_str());
    }
    lvlWriter.Close();

    // Export the re-saved copy
    ReliveAPI::LvlReader reader2(newLvlName.c_str());
    if (!reader2.IsOpen())
    {
        throw ReliveAPI::IOReadException(strLvlName);
    }
    LOG_INFO("Export and compare LVL " << newLvlName);
    ForEachCamera(reader2, [&](const std::string& camName, ReliveAPI::ChunkedLvlFile& camFile)
                  {
                      // Write out all cam and FG1s again (but this is our re-imported copy)
                      ReliveAPI::CameraImageAndLayers oldData;
                      ReliveAPI::CamConverter converter(camFile, oldData);
                      ReliveAPI::ApiFG1Reader::DebugSave("resaved_" + camName, oldData);

                      // Compare the resaved pngs are equal to the original exported data
                      ReliveAPI::CameraImageAndLayers newData;
                      ReliveAPI::ApiFG1Reader::DebugRead(camName, newData);

                      ASSERT_EQ(newData.mCameraImage, oldData.mCameraImage);
                      ASSERT_EQ(newData.mForegroundLayer, oldData.mForegroundLayer);
                      ASSERT_EQ(newData.mBackgroundLayer, oldData.mBackgroundLayer);
                      ASSERT_EQ(newData.mForegroundWellLayer, oldData.mForegroundWellLayer);
                      ASSERT_EQ(newData.mBackgroundWellLayer, oldData.mBackgroundWellLayer);

                  });
    reader2.Close();
}

TEST(alive_api, CAMAndFG1Conversion)
{
    for (const auto& lvl : kAOLvls)
    {
        TestCamAndFG1ExportImport(AOPath(lvl), false);
    }

    for (const auto& lvl : kAELvls)
    {
        TestCamAndFG1ExportImport(AEPath(lvl), true);
    }
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
    ReliveAPI::ImportPathJsonToBinary(getStaticFileBuffer(), "OutputAO.json", AOPath("R1.LVL"), "newAO.lvl", {}, true);

    const auto ogR1 = readFileIntoStaticFileBuffer(AOPath("R1.LVL"));
    ASSERT_NE(ogR1.size(), 0u);

    const auto rewrittenR1 = readFileIntoStaticFileBuffer("newAO.lvl");
    ASSERT_NE(rewrittenR1.size(), 0u);

    ASSERT_EQ(ogR1, rewrittenR1);
}

TEST(alive_api, ImportPathJsonToBinaryAE)
{
    ReliveAPI::ImportPathJsonToBinary(getStaticFileBuffer(), "OutputAE.json", AEPath(kAETestLvl), "newAE.lvl", {}, true);

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
    const std::vector<s32> paths{15, 16, 18, 19, 20};
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
            ReliveAPI::ImportPathJsonToBinary(getStaticFileBuffer(), jsonName, AOPath(lvl), lvlName, {}, true);

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
            ReliveAPI::ImportPathJsonToBinary(getStaticFileBuffer(), jsonName, AEPath(lvl), lvlName, {}, true);

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
    ReliveAPI::TypesCollectionAO types;

    AO::Path_Hoist tlv = {};
    std::unique_ptr<ReliveAPI::TlvObjectBase> pHoist = types.MakeTlvAO(AO::TlvTypes::Hoist_3, &tlv, 99);

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
    ArgsAdapter(s32 argc, char_type* argv[])
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

    [[nodiscard]] std::unique_ptr<char_type* []> ArgV() const
    {
        auto ptr = std::make_unique<char_type*[]>(mArgs.size());

        s32 i = 0;
        for (const auto& arg : mArgs)
        {
            ptr[i++] = const_cast<char_type*>(arg.c_str());
        }
        return ptr;
    }

    private : std::vector<std::string> mArgs;
};

} // namespace

// stop sdl_main messing with stuff
#undef main

s32 main(s32 argc, char_type* argv[])
{
    ArgsAdapter args(argc, argv);
    args.Add("--gtest_catch_exceptions=0");

    s32 newArgc = args.ArgC();
    auto newArgv = args.ArgV();

    ::testing::InitGoogleTest(&newArgc, newArgv.get());
    const auto ret = RUN_ALL_TESTS();
    return ret;
}
