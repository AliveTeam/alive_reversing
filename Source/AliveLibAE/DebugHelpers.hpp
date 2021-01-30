#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "DDCheat.hpp"

void DebugHelpers_Init();

extern char _devConsoleBuffer[1000];

#define DEV_CONSOLE_MESSAGE(message, duration) ShowDebugConsoleMessage(message, duration);
#define DEV_CONSOLE_MESSAGE_C(message, duration, r, g, b) ShowDebugConsoleMessage(message, duration, r, g, b);

#define DEV_CONSOLE_PRINTF(format, ...) memset(_devConsoleBuffer, 0, sizeof(_devConsoleBuffer));\
sprintf(_devConsoleBuffer, format, ##__VA_ARGS__);\
DEV_CONSOLE_MESSAGE_C(_devConsoleBuffer, 5, 127, 127, 0);

#define DEV_WORLD_PRINTF(x,y,ot,format, ...) memset(_devConsoleBuffer, 0, sizeof(_devConsoleBuffer));\
sprintf(_devConsoleBuffer, format, ##__VA_ARGS__);\
DEV::DebugDrawText(ot, 39, std::string(_devConsoleBuffer), x,y,255,255,255, true, false);

struct RaycastDebug
{
    FP x1;
    FP y1;
    FP x2;
    FP y2;
    FP hitX;
    FP hitY;
    PathLine * pLine;
    unsigned int type;
};

void DebugAddRaycast(RaycastDebug rc);

// File System
namespace FS
{
    std::vector<BYTE> ReadFile(std::string filePath);
    std::string GetPrefPath();
}

namespace DEV
{
    void DebugDrawLine(PrimHeader ** ot, int layer, int x1, int y1, int x2, int y2, BYTE r, BYTE g, BYTE b, bool worldspace, bool semiTransparent = false);
    void DebugDrawText(PrimHeader ** ot, int layer, std::string & text, int x, int y, BYTE r, BYTE g, BYTE b, bool worldspace, bool semiTransparent);
    void DebugFillRect(PrimHeader ** ot, int layer, int x, int y, int width, int height, BYTE r, BYTE g, BYTE b, bool worldspace, bool semiTransparent = false);
    void DebugDrawRect(PrimHeader ** ot, int layer, int x, int y, int width, int height, BYTE r, BYTE g, BYTE b, bool worldspace, bool semiTransparent = false);
    void DebugOnFrameDraw(PrimHeader** ppOt);
}

extern bool sDebugEnabled_VerboseEvents;

// Duration in seconds
void ShowDebugConsoleMessage(std::string message, float duration, BYTE r, BYTE g, BYTE b);
void ShowDebugConsoleMessage(std::string message, float duration);

std::vector<std::string> SplitString(const std::string& s, char seperator);
bool StringStartsWith(std::string mainStr, std::string toMatch);
std::string StringToLower(std::string s);
std::string IntToHexString(int v);
std::string EscapeUnknownCharacters(std::string text);
bool IsStringNumber(const std::string& s);

BaseGameObject * FindObjectOfType(int id);

// In game helper functions
void Cheat_OpenAllDoors();