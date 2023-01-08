#include "stdafx.h"
#include "GameEnderController.hpp"
#include "stdlib.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "BirdPortal.hpp"
#include "Map.hpp"
#include "PauseMenu.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Abe.hpp"
#include "Alarm.hpp"
#include "../relive_lib/Function.hpp"
#include "PathDataExtensions.hpp"
#include "Input.hpp"

s16 gFeeco_Restart_KilledMudCount = 0;
s16 gFeecoRestart_SavedMudCount = 0;

void CreateGameEnderController()
{
    // Exit if it already exists
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eGameEnderController)
        {
            return;
        }
    }

    // Otherwise create one
    relive_new GameEnderController();
}


s32 GameEnderController::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const GameEnderControllerSaveState*>(pBuffer);
    auto pGameEnderController = relive_new GameEnderController();
    if (pGameEnderController)
    {
        pGameEnderController->mBaseGameObjectTlvInfo = pState->mObjId;
        pGameEnderController->mTimer = sGnFrame + pState->mTimer;
        pGameEnderController->mState = pState->mState;
    }
    return sizeof(GameEnderControllerSaveState);
}

GameEnderController::GameEnderController()
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eGameEnderController);
    mState = GameEnderControllerStates::eInit_0;
}

void GameEnderController::VScreenChanged()
{
    if (mState != GameEnderControllerStates::eInit_0)
    {
        if (gRescuedMudokons < 150)
        {
            SwitchStates_Set(100u, 0);
        }
    }

    if (gMap.mCurrentLevel != gMap.mNextLevel)
    {
        SetDead(true);
    }
}

s32 GameEnderController::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<GameEnderControllerSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eGameEnderController;
    pState->mObjId = mBaseGameObjectTlvInfo;
    pState->mTimer = mTimer - sGnFrame;
    pState->mState = mState;
    return sizeof(GameEnderControllerSaveState);
}

void GameEnderController::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mState)
    {
        case GameEnderControllerStates::eInit_0:
            if (SwitchStates_Get(100u))
            {
                mState = GameEnderControllerStates::eDetermineEnding_1;
            }
            break;

        case GameEnderControllerStates::eDetermineEnding_1:
        {
            auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(sActiveHero->mBirdPortalId));
            if (pBirdPortal)
            {
                if (pBirdPortal->mState == BirdPortal::PortalStates::CollapseTerminators_10)
                {
                    auto pAlarm = sObjectIds.Find_Impl(gAlarmObjId);
                    if (pAlarm)
                    {
                        pAlarm->SetDead(true);
                    }

                    pBirdPortal->SetDead(true);
                    sActiveHero->SetDead(true);

                    // Good ending
                    if (gRescuedMudokons >= Path_GoodEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                    {
                        gAbeInvincible = false;
                        gFeeco_Restart_KilledMudCount = 0;
                        gFeecoRestart_SavedMudCount = 0;

                        if (gPauseMenu)
                        {
                            gPauseMenu->SetDead(true);
                            gPauseMenu = nullptr;
                        }

                        if (gRescuedMudokons >= Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Perfect ending
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 17, CameraSwapEffects::eUnknown_11, 17, 0);
                            mState = GameEnderControllerStates::eAngelicEnding_5;
                        }
                        else
                        {
                            // Good enough ending
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 18, CameraSwapEffects::eUnknown_11, 17, 0);
                            mState = GameEnderControllerStates::eGoodEnding_4;
                        }
                    }
                    else
                    {
                        if (gKilledMudokons >= Path_BadEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Very bad ending
                            gAbeInvincible = true;
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 15, CameraSwapEffects::eUnknown_11, 18, 0);
                            mState = GameEnderControllerStates::eBadEnding_3;
                        }
                        else
                        {
                            // Bad ending
                            gAbeInvincible = false;
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 16, CameraSwapEffects::eUnknown_11, 18, 0);
                            mState = GameEnderControllerStates::eBadEnding_3;
                            gRescuedMudokons = gFeecoRestart_SavedMudCount;
                            gKilledMudokons = gFeeco_Restart_KilledMudCount;
                        }
                        gVisitedBonewerkz = false;
                        gVisitedBarracks = false;
                        gVisitedFeecoEnder = false;
                    }
                }
            }
        }
        break;

        case GameEnderControllerStates::eBadEnding_3:
            if (Input().IsAnyPressed(InputCommands::eUnPause_OrConfirm) || Input().IsAnyPressed(InputCommands::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eFeeCoDepot, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
                mState = GameEnderControllerStates::eFinish_2;
            }
            break;

        case GameEnderControllerStates::eGoodEnding_4:
            if (Input().IsAnyPressed(InputCommands::eUnPause_OrConfirm) || Input().IsAnyPressed(InputCommands::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
                mState = GameEnderControllerStates::eFinish_2;
            }
            break;

        case GameEnderControllerStates::eAngelicEnding_5:
            if (Input().IsAnyPressed(InputCommands::eUnPause_OrConfirm) || Input().IsAnyPressed(InputCommands::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 20, CameraSwapEffects::eInstantChange_0, 0, 0);
                mState = GameEnderControllerStates::eAngelicEndingCredits_6;
            }
            break;

        case GameEnderControllerStates::eAngelicEndingCredits_6:
            if (Input().IsAnyPressed(InputCommands::eUnPause_OrConfirm) || Input().IsAnyPressed(InputCommands::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 2, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
                gMap.mFreeAllAnimAndPalts = true;
                mState = GameEnderControllerStates::eFinish_2;
            }
            break;

        default:
            return;
    }
}
