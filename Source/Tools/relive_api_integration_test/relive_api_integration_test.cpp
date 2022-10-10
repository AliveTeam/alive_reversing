#include "relive_api.hpp"

#include "TlvsAO.hpp"
#include "TypesCollectionAO.hpp"

#include "../../AliveLibCommon/stdafx_common.h"
#include "../../AliveLibCommon/logger.hpp"

#include "LvlReaderWriter.hpp"
#include "CamConverter.hpp"
#include "JsonModelTypes.hpp"
#include "ApiFG1Reader.hpp"

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
constexpr std::string_view kAETestLvl = "mi.lvl"sv;

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

[[maybe_unused]] [[nodiscard]] std::vector<u8>& readFileIntoStaticFileBuffer(const std::string& filePath)
{
    std::vector<u8>& buf = getStaticFileBuffer();

    FileSystem fs;
    if (!fs.LoadToVec(filePath.c_str(), buf))
    {
        std::cerr << "Failure reading file '" << filePath << "'\n";
        std::abort();
    }

    return buf;
}

TEST(alive_api, AddFileToLvl)
{
    // Note: testData needed to be bigger than the size of a lvl header to detect a bug where new files got wrote
    // to the start of the file.
    std::string testData("One form of rory, and one form only. This is definitely a gobbins moment");
    ReliveAPI::FileIO fileIo;

    {
        ReliveAPI::LvlWriter w(fileIo, AOPath("R1.LVL").c_str());
        ASSERT_TRUE(w.IsOpen());

        std::vector<u8> data(testData.begin(), testData.end());
        w.AddFile("PLOP.DAT", data);

        ASSERT_TRUE(w.Save(fileIo, getStaticFileBuffer(), "CraigDavid.lvl"));
    }

    {
        ReliveAPI::LvlReader r(fileIo, "CraigDavid.lvl");
        ASSERT_TRUE(r.IsOpen());

        ASSERT_GT(r.FileCount(), 1);

        std::vector<u8> fileData;
        ASSERT_TRUE(r.ReadFileInto(fileData, "PLOP.DAT"));

        std::string s(fileData.begin(), fileData.end());
        ASSERT_EQ(s, testData);
    }
}

template <typename FnOnCam>
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
    ReliveAPI::FileIO fileIo;

    ReliveAPI::LvlReader reader(fileIo, strLvlName.c_str());
    if (!reader.IsOpen())
    {
        throw ReliveAPI::IOReadException(strLvlName);
    }

    ReliveAPI::LvlWriter lvlWriter(fileIo, strLvlName.c_str());
    if (!lvlWriter.IsOpen())
    {
        throw ReliveAPI::IOReadException(strLvlName);
    }

    LOG_INFO("+ import LVL " << strLvlName);
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
    if (!lvlWriter.Save(fileIo, tmpBuffer, newLvlName.c_str()))
    {
        throw ReliveAPI::IOWriteException(newLvlName.c_str());
    }
    lvlWriter.Close();

    // the re-saved copy
    ReliveAPI::LvlReader reader2(fileIo, newLvlName.c_str());
    if (!reader2.IsOpen())
    {
        throw ReliveAPI::IOReadException(strLvlName);
    }
    LOG_INFO("and compare LVL " << newLvlName);
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
    ReliveAPI::FileIO fileIo;
    ReliveAPI::Context context;
    ReliveAPI::Detail::ExportPathBinaryToJson(getStaticFileBuffer(), fileIo, "OutputAE.json", AEPath(kAETestLvl), 1, context);
}

TEST(alive_api, ExportPathBinaryToJsonAO)
{
    ReliveAPI::FileIO fileIo;
    ReliveAPI::Context context;
    ReliveAPI::Detail::ExportPathBinaryToJson(getStaticFileBuffer(), fileIo, "OutputAO.json", AOPath("R1.LVL"), 19, context);
}

static void SaveVec(const std::string& fileName, const std::vector<u8>& vec)
{
    FILE* hFile = ::fopen(fileName.c_str(), "wb");
    if (hFile)
    {
        ::fwrite(vec.data(), 1, vec.size(), hFile);
        ::fclose(hFile);
    }
}

static bool PathChunksAreEqual(ReliveAPI::IFileIO& fileIo, const std::string& leftSideLvl, const std::string& rightSideLvl)
{
    auto leftPathBnd = ReliveAPI::Detail::OpenPathBnd(fileIo, leftSideLvl, getStaticFileBuffer());
    std::vector<u8> tmp;
    auto rightPathBnd = ReliveAPI::Detail::OpenPathBnd(fileIo, rightSideLvl, tmp);

    for (u32 i = 0; i < leftPathBnd->ChunkCount(); i++)
    {
        auto leftPath = leftPathBnd->ChunkAt(i);
        if (leftPath.Header().mResourceType == ResourceManagerWrapper::Resource_Path)
        {
            auto rightPath = rightPathBnd->ChunkById(leftPath.Id());
            if (leftPath.Data() != rightPath->Data())
            {
                SaveVec("LeftPath.dat", leftPath.Data());
                SaveVec("RightPath.dat", rightPath->Data());

                return false;
            }
        }
    }
    return true;
}

TEST(alive_api, ImportPathJsonToBinaryAO)
{
    ReliveAPI::FileIO fileIo;
    ReliveAPI::Context context;

    ReliveAPI::Detail::ExportPathBinaryToJson(getStaticFileBuffer(), fileIo, "OutputAO.json", AOPath("R1.LVL"), 19, context);

    ReliveAPI::Detail::ImportPathJsonToBinary(getStaticFileBuffer(), fileIo, "OutputAO.json", AOPath("R1.LVL"), "newAO.lvl", {}, true, context);
    
    ReliveAPI::Detail::ExportPathBinaryToJson(getStaticFileBuffer(), fileIo, "OutputAO2.json", "newAO.lvl", 19, context);

    ASSERT_TRUE(PathChunksAreEqual(fileIo, AOPath("R1.LVL"), "newAO.lvl"));
}

TEST(alive_api, ImportPathJsonToBinaryAE)
{
    ReliveAPI::FileIO fileIo;
    ReliveAPI::Context context;

    ReliveAPI::Detail::ExportPathBinaryToJson(getStaticFileBuffer(), fileIo, "OutputAE.json", AEPath(kAETestLvl), 1, context);

    ReliveAPI::Detail::ImportPathJsonToBinary(getStaticFileBuffer(), fileIo, "OutputAE.json", AEPath(kAETestLvl), "newAE.lvl", {}, true, context);

    ReliveAPI::Detail::ExportPathBinaryToJson(getStaticFileBuffer(), fileIo, "OutputAE2.json", "newAE.lvl", 1, context);

    ASSERT_TRUE(PathChunksAreEqual(fileIo, AEPath(kAETestLvl), "newAE.lvl"));
}

TEST(alive_api, EnumeratePathsAO)
{
    ReliveAPI::FileIO fileIo;
    auto ret = ReliveAPI::Detail::EnumeratePaths(getStaticFileBuffer(), fileIo, AOPath("R1.LVL"));
    ASSERT_EQ(ret.pathBndName, "R1PATH.BND");
    const std::vector<s32> paths{15, 16, 18, 19, 20};
    ASSERT_EQ(ret.paths, paths);
}

TEST(alive_api, ReSaveAllPathsAO)
{
    ReliveAPI::FileIO fileIo;
    ReliveAPI::Context context;

    for (const auto& lvl : kAOLvls)
    {
        auto ret = ReliveAPI::Detail::EnumeratePaths(getStaticFileBuffer(), fileIo, AOPath(lvl));

        for (s32 path : ret.paths)
        {
            const std::string jsonName = concat("OutputAO_", lvl, '_', path, ".json");
            LOG_INFO("Save " << jsonName);
            ReliveAPI::Detail::ExportPathBinaryToJson(getStaticFileBuffer(), fileIo, jsonName, AOPath(lvl), path, context);

            const std::string lvlName = concat("OutputAO_", lvl, '_', path, ".lvl");
            LOG_INFO("Resave " << lvlName);
            ReliveAPI::Detail::ImportPathJsonToBinary(getStaticFileBuffer(), fileIo, jsonName, AOPath(lvl), lvlName, {}, true, context);

            ASSERT_TRUE(PathChunksAreEqual(fileIo, AOPath(lvl), lvlName));
        }
    }
}

TEST(alive_api, ReSaveAllPathsAE)
{
    ReliveAPI::FileIO fileIo;
    ReliveAPI::Context context;

    for (const auto& lvl : kAELvls)
    {
        auto ret = ReliveAPI::Detail::EnumeratePaths(getStaticFileBuffer(), fileIo, AEPath(lvl));

        for (s32 path : ret.paths)
        {
            const std::string jsonName = concat("OutputAE_", lvl, '_', path, ".json");
            LOG_INFO("Save " << jsonName);
            ReliveAPI::Detail::ExportPathBinaryToJson(getStaticFileBuffer(), fileIo, jsonName, AEPath(lvl), path, context);

            const std::string lvlName = concat("OutputAE_", lvl, '_', path, ".lvl");
            LOG_INFO("Resave " << lvlName);
            ReliveAPI::Detail::ImportPathJsonToBinary(getStaticFileBuffer(), fileIo, jsonName, AEPath(lvl), lvlName, {}, true, context);

            ASSERT_TRUE(PathChunksAreEqual(fileIo, AEPath(lvl), lvlName));
        }
    }
}

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
