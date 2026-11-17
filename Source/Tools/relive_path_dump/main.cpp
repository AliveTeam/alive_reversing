// relive_path_dump: dump every path of every Abe's Exoddus/Oddysee .LVL file
// in a game directory to JSON using ReliveAPI::ExportPathBinaryToJson.
//
// Usage: relive_path_dump <game_dir> <out_dir>
//
// Output: <out_dir>/<lvl stem>_path<id>.json for each path, plus
//         <out_dir>/manifest.json listing what was exported or failed.

#include "relive_api.hpp"

#include "../../AliveLibCommon/stdafx_common.h"
#include "../../AliveLibCommon/logger.hpp"

#include "file_api.hpp"
#include "ApiContext.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <string>
#include <vector>

INITIALIZE_EASYLOGGINGPP

namespace fs = std::filesystem;

static std::string Lower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

static std::string JsonEscape(const std::string& in)
{
    std::string out;
    for (const char c : in)
    {
        if (c == '"' || c == '\\')
        {
            out += '\\';
        }
        out += c;
    }
    return out;
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::fprintf(stderr, "Usage: %s <game_dir> <out_dir>\n", argv[0]);
        return 2;
    }

    const fs::path gameDir = argv[1];
    const fs::path outDir = argv[2];

    if (!fs::is_directory(gameDir))
    {
        std::fprintf(stderr, "Not a directory: %s\n", gameDir.string().c_str());
        return 2;
    }
    fs::create_directories(outDir);

    std::vector<fs::path> lvls;
    for (const auto& entry : fs::directory_iterator(gameDir))
    {
        if (entry.is_regular_file() && Lower(entry.path().extension().string()) == ".lvl")
        {
            lvls.push_back(entry.path());
        }
    }
    std::sort(lvls.begin(), lvls.end());

    if (lvls.empty())
    {
        std::fprintf(stderr, "No .lvl files found in %s\n", gameDir.string().c_str());
        return 1;
    }

    ReliveAPI::FileIO fileIo;
    std::string exported;
    std::string failed;
    int okCount = 0;

    for (const auto& lvl : lvls)
    {
        const std::string stem = Lower(lvl.stem().string());
        try
        {
            const auto result = ReliveAPI::EnumeratePaths(fileIo, lvl.string());
            for (const s32 pathId : result.paths)
            {
                const fs::path outFile = outDir / (stem + "_path" + std::to_string(pathId) + ".json");
                try
                {
                    ReliveAPI::Context context;
                    ReliveAPI::ExportPathBinaryToJson(fileIo, outFile.string(), lvl.string(), pathId, context);
                    std::printf("OK   %s path %d -> %s\n", stem.c_str(), pathId, outFile.string().c_str());
                    exported += std::string(exported.empty() ? "" : ",\n") + "    {\"lvl\": \"" + JsonEscape(stem) + "\", \"pathBnd\": \"" + JsonEscape(result.pathBndName) + "\", \"path\": " + std::to_string(pathId) + ", \"file\": \"" + JsonEscape(outFile.filename().string()) + "\"}";
                    okCount++;
                }
                catch (const std::exception& e)
                {
                    std::printf("FAIL %s path %d: %s\n", stem.c_str(), pathId, e.what());
                    failed += std::string(failed.empty() ? "" : ",\n") + "    {\"lvl\": \"" + JsonEscape(stem) + "\", \"path\": " + std::to_string(pathId) + ", \"error\": \"" + JsonEscape(e.what()) + "\"}";
                }
            }
        }
        catch (const std::exception& e)
        {
            std::printf("SKIP %s: %s\n", stem.c_str(), e.what());
            failed += std::string(failed.empty() ? "" : ",\n") + "    {\"lvl\": \"" + JsonEscape(stem) + "\", \"error\": \"" + JsonEscape(e.what()) + "\"}";
        }
    }

    if (FILE* manifest = std::fopen((outDir / "manifest.json").string().c_str(), "w"))
    {
        std::fprintf(manifest, "{\n  \"exported\": [\n%s\n  ],\n  \"failed\": [\n%s\n  ]\n}\n", exported.c_str(), failed.c_str());
        std::fclose(manifest);
    }

    std::printf("Done: %d paths exported.\n", okCount);
    return okCount > 0 ? 0 : 1;
}
