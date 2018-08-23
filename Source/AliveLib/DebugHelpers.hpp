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

// File System
namespace FS
{
    std::vector<BYTE> ReadFile(std::string filePath);
}

namespace DEV
{
    void DebugDrawLine(int ** ot, int layer, int x1, int y1, int x2, int y2, char r, char g, char b, bool worldspace, bool semiTransparent = false);
    void DebugFillRect(int ** ot, int layer, int x, int y, int width, int height, char r, char g, char b, bool worldspace, bool semiTransparent = false);
    void DebugDrawRect(int ** ot, int layer, int x, int y, int width, int height, char r, char g, char b, bool worldspace, bool semiTransparent = false);
    void DebugOnFrameDraw();
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