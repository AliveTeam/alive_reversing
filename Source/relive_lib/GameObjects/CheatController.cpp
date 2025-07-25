#include "stdafx.h"
#include "CheatController.hpp"
#include "Function.hpp"
#include "../AliveLibAE/Game.hpp"
#include "MapWrapper.hpp"
#include "../AliveLibAE/QuikSave.hpp"
#include "data_conversion/file_system.hpp"
#include "data_conversion/AESaveSerialization.hpp"
#include "GameType.hpp"

CheatController* gCheatController = nullptr;

bool CheatController::gEnableFartGasCheat = false;
bool CheatController::gVoiceCheat = false;
bool CheatController::gEnableCheatFMV = false;
bool CheatController::gEnableCheatLevelSelect = false;

// AO/AE
static const InputCommands sCheatKeyArray_MovieSelect[8] = {
    InputCommands::eUp,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eDown};

// AO/AE
static const InputCommands sCheatKeyArray_LevelSelect[8] = {
    InputCommands::eDown,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eLeft,
    InputCommands::eUp};

// AE only
static const InputCommands sCheatKeyArray_PathSkip[6] = {
    InputCommands::eLeft,
    InputCommands::eRight,
    InputCommands::eUp,
    InputCommands::eDown,
    InputCommands::eLeft,
    InputCommands::eRight};

// AO only
static const InputCommands sCheatKeyArray_FartGas[] = {
    InputCommands::eThrowItem,
    InputCommands::eThrowItem,
    InputCommands::eThrowItem,
    InputCommands::eThrowItem,
    InputCommands::eThrowItem,
    InputCommands::eThrowItem};

// AO only
static const InputCommands sCheatKeyArray_VoiceLocks[] = {
    InputCommands::eSneak, // eCrouchOrRoll,
    InputCommands::eDown,
    InputCommands::eThrowItem,
    InputCommands::eDoAction,
    InputCommands::eDown,
    InputCommands::eSneak // eCrouchOrRoll
};

static void CheatController_Cheat_MovieSelect()
{
    if (GetMap().mCurrentCamera == 1)
    {
        CheatController::gEnableCheatFMV = !CheatController::gEnableCheatFMV;
    }
}

static void CheatController_Cheat_LevelSelect()
{
    if (GetMap().mCurrentCamera == 1)
    {
        CheatController::gEnableCheatLevelSelect = !CheatController::gEnableCheatLevelSelect;
    }
}

static void CheatController_Cheat_FartGas()
{
    CheatController::gEnableFartGasCheat = !CheatController::gEnableFartGasCheat;
}

static void CheatController_Cheat_VoiceLocks()
{
    CheatController::gVoiceCheat = !CheatController::gVoiceCheat;
}

static void CheatController_Cheat_PathSkip()
{
    char_type nameBuffer[20];

    DestroyObjects();
    sprintf(nameBuffer, "NXTP%04d.SAV.json", QuikSave::gActiveQuicksaveData.mWorldInfo.mSaveFileId);

    FileSystem fs;
    std::string jsonStr = fs.LoadToString(nameBuffer);

    if (jsonStr.empty())
    {
        ALIVE_FATAL("Save file is empty");
    }

    LOG_INFO("Loading path skip cheat save %s", nameBuffer);
    nlohmann::json j = nlohmann::json::parse(jsonStr);
    QuikSave::gActiveQuicksaveData = {};
    from_json(j, QuikSave::gActiveQuicksaveData);

    QuikSave::LoadActive();
}

struct CheatEntry final
{
    EReliveLevelIds mApplyToLevel;
    GameType mApplyToGame;
    s32 mCheatCodeLength;
    const InputCommands* mCheatCodeAry;
    s32 mSuccessIdx;
    void (*mCallBack)(void);
};

static CheatEntry sCheatArray[] = {
    {EReliveLevelIds::eMenu, GameType::eAe, ALIVE_COUNTOF(sCheatKeyArray_MovieSelect), sCheatKeyArray_MovieSelect, 0, &CheatController_Cheat_MovieSelect},
    {EReliveLevelIds::eMenu, GameType::eAo, ALIVE_COUNTOF(sCheatKeyArray_MovieSelect), sCheatKeyArray_MovieSelect, 0, &CheatController_Cheat_MovieSelect},
    {EReliveLevelIds::eMenu, GameType::eAe, ALIVE_COUNTOF(sCheatKeyArray_LevelSelect), sCheatKeyArray_LevelSelect, 0, &CheatController_Cheat_LevelSelect},
    {EReliveLevelIds::eMenu, GameType::eAo, ALIVE_COUNTOF(sCheatKeyArray_LevelSelect), sCheatKeyArray_LevelSelect, 0, &CheatController_Cheat_LevelSelect},
    {EReliveLevelIds::eNone, GameType::eAo, ALIVE_COUNTOF(sCheatKeyArray_FartGas), sCheatKeyArray_FartGas, 0, &CheatController_Cheat_FartGas},
    {EReliveLevelIds::eNone, GameType::eAo, ALIVE_COUNTOF(sCheatKeyArray_VoiceLocks), sCheatKeyArray_VoiceLocks, 0, &CheatController_Cheat_VoiceLocks},
    {EReliveLevelIds::eNone, GameType::eAe, ALIVE_COUNTOF(sCheatKeyArray_PathSkip), sCheatKeyArray_PathSkip, 0, &CheatController_Cheat_PathSkip}};

CheatController::CheatController()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true);
    SetType(ReliveTypes::eNone);
}

void CheatController::VScreenChanged()
{
    // Empty
}

CheatController::~CheatController()
{
    gCheatController = nullptr;
}

void CheatController::VUpdate()
{
    const u16 held = Input().GetPressed();

    if (held)
    {
        // Only do cheat code check if shift is held
        if (Input().IsAnyHeld(InputCommands::eRun))
        {
            for (auto& cheatEntry : sCheatArray)
            {
                if (cheatEntry.mApplyToGame == GetGameType())
                {
                    // None = apply to any level
                    if (cheatEntry.mApplyToLevel == EReliveLevelIds::eNone || (GetMap().mCurrentLevel == cheatEntry.mApplyToLevel && cheatEntry.mApplyToLevel != EReliveLevelIds::eNone))
                    {
                        if (held == cheatEntry.mCheatCodeAry[cheatEntry.mSuccessIdx])
                        {
                            cheatEntry.mSuccessIdx++;

                            // Check if we've successfully entered all cheat code keys.
                            if (cheatEntry.mSuccessIdx >= cheatEntry.mCheatCodeLength)
                            {
                                cheatEntry.mSuccessIdx = 0;
                                cheatEntry.mCallBack();
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
}
