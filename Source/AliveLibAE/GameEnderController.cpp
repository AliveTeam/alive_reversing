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
#include "Function.hpp"
#include "PathDataExtensions.hpp"

s16 sFeeco_Restart_KilledMudCount_5C1BC6 = 0;
s16 sFeecoRestart_SavedMudCount_5C1BC8 = 0;

void CreateGameEnderController_43B7A0()
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
        pGameEnderController->mBaseGameObjectTlvInfo = pState->field_4_obj_id;
        pGameEnderController->field_20_timer = sGnFrame + pState->field_8_timer;
        pGameEnderController->field_24_state = pState->field_C_state;
    }
    return sizeof(GameEnderControllerSaveState);
}

GameEnderController::GameEnderController()
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eGameEnderController);
    field_24_state = GameEnderControllerStates::eInit_0;
}

void GameEnderController::VScreenChanged()
{
    if (field_24_state != GameEnderControllerStates::eInit_0)
    {
        if (sRescuedMudokons < 150)
        {
            SwitchStates_Set(100u, 0);
        }
    }

    if (gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s32 GameEnderController::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<GameEnderControllerSaveState*>(pSaveBuffer);

    pState->field_0_type = ReliveTypes::eGameEnderController;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;
    pState->field_8_timer = field_20_timer - sGnFrame;
    pState->field_C_state = field_24_state;
    return sizeof(GameEnderControllerSaveState);
}

void GameEnderController::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_24_state)
    {
        case GameEnderControllerStates::eInit_0:
            if (SwitchStates_Get(100u))
            {
                field_24_state = GameEnderControllerStates::eDetermineEnding_1;
            }
            break;

        case GameEnderControllerStates::eDetermineEnding_1:
        {
            auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(sActiveHero->mBirdPortalId));
            if (pBirdPortal)
            {
                if (pBirdPortal->mState == BirdPortal::PortalStates::CollapseTerminators_10)
                {
                    auto pAlarm = sObjectIds.Find_Impl(sAlarmObjId_550D70);
                    if (pAlarm)
                    {
                        pAlarm->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    }

                    pBirdPortal->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    sActiveHero->mBaseGameObjectFlags.Set(BaseGameObject::eDead);

                    // Good ending
                    if (sRescuedMudokons >= Path_GoodEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                    {
                        gAbeBulletProof_5C1BDA = 0;
                        sFeeco_Restart_KilledMudCount_5C1BC6 = 0;
                        sFeecoRestart_SavedMudCount_5C1BC8 = 0;

                        if (gPauseMenu)
                        {
                            gPauseMenu->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                            gPauseMenu = nullptr;
                        }

                        if (sRescuedMudokons >= Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Perfect ending
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 17, CameraSwapEffects::eUnknown_11, 17, 0);
                            field_24_state = GameEnderControllerStates::eAngelicEnding_5;
                        }
                        else
                        {
                            // Good enough ending
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 18, CameraSwapEffects::eUnknown_11, 17, 0);
                            field_24_state = GameEnderControllerStates::eGoodEnding_4;
                        }
                    }
                    else
                    {
                        if (sKilledMudokons >= Path_BadEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Very bad ending
                            gAbeBulletProof_5C1BDA = true;
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 15, CameraSwapEffects::eUnknown_11, 18, 0);
                            field_24_state = GameEnderControllerStates::eBadEnding_3;
                        }
                        else
                        {
                            // Bad ending
                            gAbeBulletProof_5C1BDA = false;
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 16, CameraSwapEffects::eUnknown_11, 18, 0);
                            field_24_state = GameEnderControllerStates::eBadEnding_3;
                            sRescuedMudokons = sFeecoRestart_SavedMudCount_5C1BC8;
                            sKilledMudokons = sFeeco_Restart_KilledMudCount_5C1BC6;
                        }
                        sVisitedBonewerks_5C1C02 = false;
                        sVisitedBarracks_5C1C04 = false;
                        sVisitedFeecoEnder_5C1C06 = false;
                    }
                }
            }
        }
        break;

        case GameEnderControllerStates::eBadEnding_3:
            if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm) || Input().isHeld(InputCommands::Enum::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eFeeCoDepot, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_24_state = GameEnderControllerStates::eFinish_2;
            }
            break;

        case GameEnderControllerStates::eGoodEnding_4:
            if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm) || Input().isHeld(InputCommands::Enum::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_24_state = GameEnderControllerStates::eFinish_2;
            }
            break;

        case GameEnderControllerStates::eAngelicEnding_5:
            if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm) || Input().isHeld(InputCommands::Enum::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 20, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_24_state = GameEnderControllerStates::eAngelicEndingCredits_6;
            }
            break;

        case GameEnderControllerStates::eAngelicEndingCredits_6:
            if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm) || Input().isHeld(InputCommands::Enum::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 2, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
                gMap.mFreeAllAnimAndPalts = true;
                field_24_state = GameEnderControllerStates::eFinish_2;
            }
            break;

        default:
            return;
    }
}
