#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"

void DebugHelpers_Init();

// File System
namespace FS
{
    std::vector<BYTE> ReadFile(std::string filePath);
}