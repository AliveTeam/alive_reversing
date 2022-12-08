#include "stdafx.h"
#include "CheatController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "MainMenu.hpp"
#include "QuikSave.hpp"

CheatController* gCheatController = nullptr;

static const InputCommands::Enum sCheatKeyArray_MovieSelect[8] = {
    InputCommands::Enum::eUp,
    InputCommands::Enum::eLeft,
    InputCommands::Enum::eRight,
    InputCommands::Enum::eLeft,
    InputCommands::Enum::eRight,
    InputCommands::Enum::eLeft,
    InputCommands::Enum::eRight,
    InputCommands::Enum::eDown};

static const InputCommands::Enum sCheatKeyArray_LevelSelect[8] = {
    InputCommands::Enum::eDown,
    InputCommands::Enum::eRight,
    InputCommands::Enum::eLeft,
    InputCommands::Enum::eRight,
    InputCommands::Enum::eLeft,
    InputCommands::Enum::eRight,
    InputCommands::Enum::eLeft,
    InputCommands::Enum::eUp};

static const InputCommands::Enum sCheatKeyArray_PathSkip[6] = {
    InputCommands::Enum::eLeft,
    InputCommands::Enum::eRight,
    InputCommands::Enum::eUp,
    InputCommands::Enum::eDown,
    InputCommands::Enum::eLeft,
    InputCommands::Enum::eRight};

void CheatController_Cheat_FMV()
{
    if (gMap.mCurrentCamera == 1)
    {
        gEnableCheatFMV = !gEnableCheatFMV;
    }
}

void CheatController_Cheat_LevelSelect()
{
    if (gMap.mCurrentCamera == 1)
    {
        gEnableCheatLevelSelect = !gEnableCheatLevelSelect;
    }
}

void CheatController_Cheat_PathSkip()
{
    char_type nameBuffer[20];

    DestroyObjects_4A1F20();
    sprintf(nameBuffer, "NXTP%04d.SAV", sActiveQuicksaveData.field_204_world_info.mSaveFileId);

    //memcpy(&sActiveQuicksaveData, res.data(), sizeof(sActiveQuicksaveData));
    sActiveQuicksaveData.field_200_accumulated_obj_count = 1024;
    Quicksave_LoadActive();
}

static CheatEntry sCheatArray[3] = {
    {1u, ALIVE_COUNTOF(sCheatKeyArray_MovieSelect), sCheatKeyArray_MovieSelect, 0, &CheatController_Cheat_FMV},
    {1u, ALIVE_COUNTOF(sCheatKeyArray_LevelSelect), sCheatKeyArray_LevelSelect, 0, &CheatController_Cheat_LevelSelect},
    {0xFFFFFFFE, ALIVE_COUNTOF(sCheatKeyArray_PathSkip), sCheatKeyArray_PathSkip, 0, &CheatController_Cheat_PathSkip}};


CheatController::CheatController()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true);
    SetType(ReliveTypes::eNone);
}

CheatController::~CheatController()
{
    gCheatController = nullptr;
}

void CheatController::VUpdate()
{
    auto held = Input().mPads[sCurrentControllerIndex].mHeld;

    if (held)
    {
        // Only do cheat code check if shift is held
        if (Input().mPads[sCurrentControllerIndex].mPressed & InputCommands::Enum::eRun)
        {
            for (auto& cheatEntry : sCheatArray)
            {
                // Bit shift current level for level mask.
                if ((1 << static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))) & cheatEntry.mLevelMask)
                {
                    if (held == cheatEntry.mCheatCodeAry[cheatEntry.mSuccessIdx])
                    {
                        cheatEntry.mSuccessIdx++;

                        // Check if we've successfully entered all cheat code keys.
                        if (cheatEntry.mSuccessIdx >= cheatEntry.mCheatCodeLength)
                        {
                            cheatEntry.mSuccessIdx = 0;
                            cheatEntry.mCallback();
                        }
                    }
                    else
                    {
                        cheatEntry.mSuccessIdx = 0;
                    }
                }
            }
        }
    }
}

void CheatController::VRender(PrimHeader** /*ppOt*/)
{
    // NULL
}

void CheatController::VScreenChanged()
{
    // NULL
}
