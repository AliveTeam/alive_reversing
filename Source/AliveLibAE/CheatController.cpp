#include "stdafx.h"
#include "CheatController.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "QuikSave.hpp"

CheatController* gCheatController = nullptr;

static const InputCommands sCheatKeyArray_MovieSelect[8] = {
    InputCommands::eUp,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eDown};

static const InputCommands sCheatKeyArray_LevelSelect[8] = {
    InputCommands::eDown,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eUp};

static const InputCommands sCheatKeyArray_PathSkip[6] = {
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eUp,
    InputCommands::eDown,
    InputCommands::eLeft,
    InputCommands::eRight};

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

    DestroyObjects();
    sprintf(nameBuffer, "NXTP%04d.SAV", QuikSave::gActiveQuicksaveData.mWorldInfo.mSaveFileId);

    // TODO: Fix path skip with json saves - need to load the converted json save here
    //memcpy(&gActiveQuicksaveData, res.data(), sizeof(gActiveQuicksaveData));
    QuikSave::LoadActive();
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
    auto held = Input().mPads[sCurrentControllerIndex].mPressed;

    if (held)
    {
        // Only do cheat code check if shift is held
        if (Input().mPads[sCurrentControllerIndex].mRawInput & InputCommands::eRun)
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

void CheatController::VRender(OrderingTable& /*ot*/)
{
    // NULL
}

void CheatController::VScreenChanged()
{
    // NULL
}
