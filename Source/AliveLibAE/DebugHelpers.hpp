#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "DDCheat.hpp"

void DebugHelpers_Init();

extern char_type g_devConsoleBuffer[1000];

#define DEV_CONSOLE_MESSAGE(message, duration) ShowDebugConsoleMessage(message, duration);
#define DEV_CONSOLE_MESSAGE_C(message, duration, r, g, b) ShowDebugConsoleMessage(message, duration, r, g, b);

#define DEV_CONSOLE_PRINTF(format, ...)                      \
    memset(g_devConsoleBuffer, 0, sizeof(g_devConsoleBuffer)); \
    sprintf(g_devConsoleBuffer, format, ##__VA_ARGS__);       \
    DEV_CONSOLE_MESSAGE_C(g_devConsoleBuffer, 5, 127, 127, 0);

#define DEV_WORLD_PRINTF(x, y, ot, format, ...)              \
    memset(g_devConsoleBuffer, 0, sizeof(g_devConsoleBuffer)); \
    sprintf(g_devConsoleBuffer, format, ##__VA_ARGS__);       \
    DEV::DebugDrawText(ot, 39, std::string(g_devConsoleBuffer), x, y, 255, 255, 255, true, false);

struct RaycastDebug final
{
    FP x1;
    FP y1;
    FP x2;
    FP y2;
    FP hitX;
    FP hitY;
    PathLine* pLine;
    u32 type;
};

void DebugAddRaycast(RaycastDebug rc);

// File System
namespace FS {
    // inline as its used by the API tests
    [[nodiscard]] inline bool ReadFileInto(std::vector<u8>& target, const std::string& filePath)
    {
        FILE* hFile = ::fopen(filePath.c_str(), "rb");
        if (!hFile)
        {
            return false;
        }

        ::fseek(hFile, 0, SEEK_END);                // seek to end of file
        const std::size_t fileLen = ::ftell(hFile); // get current file pointer
        ::fseek(hFile, 0, SEEK_SET);                // seek back to beginning of file

        target.resize(fileLen);
        ::fread(target.data(), 1, target.size(), hFile);

        ::fclose(hFile);
        return true;
    }

[[nodiscard]] std::vector<u8> ReadFile(const std::string& filePath);
[[nodiscard]] std::string GetPrefPath();
} // namespace FS

namespace DEV {
void DebugDrawLine(PrimHeader** ot, Layer layer, s32 x1, s32 y1, s32 x2, s32 y2, u8 r, u8 g, u8 b, bool worldspace, bool semiTransparent = false);
void DebugDrawText(PrimHeader** ot, Layer layer, std::string& text, s32 x, s32 y, u8 r, u8 g, u8 b, bool worldspace, bool semiTransparent);
void DebugFillRect(PrimHeader** ot, Layer layer, s32 x, s32 y, s32 width, s32 height, u8 r, u8 g, u8 b, bool worldspace, bool semiTransparent = false);
void DebugDrawRect(PrimHeader** ot, Layer layer, s32 x, s32 y, s32 width, s32 height, u8 r, u8 g, u8 b, bool worldspace, bool semiTransparent = false);
void DebugOnFrameDraw(PrimHeader** ppOt);
} // namespace DEV

extern bool sDebugEnabled_VerboseEvents;

// Duration in seconds
void ShowDebugConsoleMessage(std::string message, f32 duration, u8 r, u8 g, u8 b);
void ShowDebugConsoleMessage(std::string message, f32 duration);

std::vector<std::string> SplitString(const std::string& s, s8 seperator);
bool StringStartsWith(std::string mainStr, std::string toMatch);
std::string StringToLower(std::string s);
std::string IntToHexString(s32 v);
std::string EscapeUnknownCharacters(std::string text);
bool IsStringNumber(const std::string& s);

[[nodiscard]] BaseGameObject* FindObjectOfType(s32 id);

// In game helper functions
void Cheat_OpenAllDoors();
