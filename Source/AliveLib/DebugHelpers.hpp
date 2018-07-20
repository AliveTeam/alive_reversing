#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "DDCheat.hpp"

void DebugHelpers_Init();

#define DEV_CONSOLE_MESSAGE(message, duration) ShowDebugConsoleMessage(message, duration);
#define DEV_CONSOLE_MESSAGE_C(message, duration, r, g, b) ShowDebugConsoleMessage(message, duration, r, g, b);

// File System
namespace FS
{
    std::vector<BYTE> ReadFile(std::string filePath);
}

extern bool sDebugEnabled_VerboseEvents;

// Duration in seconds
void ShowDebugConsoleMessage(std::string message, float duration, char r, char g, char b);
void ShowDebugConsoleMessage(std::string message, float duration);

std::vector<std::string> SplitString(const std::string& s, char seperator);
bool StringStartsWith(std::string mainStr, std::string toMatch);
std::string StringToLower(std::string s);
std::string IntToHexString(int v);

BaseGameObject * FindObjectOfType(int id);

// In game helper functions
void Cheat_OpenAllDoors();