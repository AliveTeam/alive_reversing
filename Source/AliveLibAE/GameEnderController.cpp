#include "stdafx.h"
#include "GameEnderController.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "DDCheat.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "BirdPortal.hpp"
#include "Map.hpp"
#include "PauseMenu.hpp"
#include "ObjectIds.hpp"
#include "Abe.hpp"
#include "Alarm.hpp"
#include "Function.hpp"
#include "PathDataExtensions.hpp"

ALIVE_VAR(1, 0x5C1BC6, s16, sFeeco_Restart_KilledMudCount_5C1BC6, 0);
ALIVE_VAR(1, 0x5C1BC8, s16, sFeecoRestart_SavedMudCount_5C1BC8, 0);

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
    auto pState = reinterpret_cast<const GameEnderController_State*>(pBuffer);
    auto pGameEnderController = relive_new GameEnderController();
    if (pGameEnderController)
    {
        pGameEnderController->mBaseGameObjectTlvInfo = pState->field_4_obj_id;
        pGameEnderController->field_20_timer = sGnFrame + pState->field_8_timer;
        pGameEnderController->field_24_state = pState->field_C_state;
    }
    return sizeof(GameEnderController_State);
}

GameEnderController::GameEnderController()
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eGameEnderController);
    field_24_state = GameEnderController_States::eInit_0;
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kDeathFlareResID);
}

void GameEnderController::VScreenChanged()
{
    if (field_24_state != GameEnderController_States::eInit_0)
    {
        if (sRescuedMudokons_5C1BC2 < 150)
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
    auto pState = reinterpret_cast<GameEnderController_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eGameEnderController_57;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;
    pState->field_8_timer = field_20_timer - sGnFrame;
    pState->field_C_state = field_24_state;
    return sizeof(GameEnderController_State);
}

void GameEnderController::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_24_state)
    {
        case GameEnderController_States::eInit_0:
            if (SwitchStates_Get(100u))
            {
                field_24_state = GameEnderController_States::eDetermineEnding_1;
            }
            break;

        case GameEnderController_States::eDetermineEnding_1:
        {
            auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(sActiveHero->field_1A8_portal_id));
            if (pBirdPortal)
            {
                if (pBirdPortal->field_28_state == BirdPortal::PortalStates::CollapseTerminators_10)
                {
                    auto pAlarm = sObjectIds.Find_Impl(sAlarmObjId_550D70);
                    if (pAlarm)
                    {
                        pAlarm->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    }

                    pBirdPortal->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    sActiveHero->mBaseGameObjectFlags.Set(BaseGameObject::eDead);

                    // Good ending
                    if (sRescuedMudokons_5C1BC2 >= Path_GoodEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                    {
                        gAbeBulletProof_5C1BDA = 0;
                        sFeeco_Restart_KilledMudCount_5C1BC6 = 0;
                        sFeecoRestart_SavedMudCount_5C1BC8 = 0;

                        if (pPauseMenu_5C9300)
                        {
                            pPauseMenu_5C9300->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                            pPauseMenu_5C9300 = nullptr;
                        }

                        if (sRescuedMudokons_5C1BC2 >= Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Perfect ending
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 17, CameraSwapEffects::eUnknown_11, 17, 0);
                            field_24_state = GameEnderController_States::eAngelicEnding_5;
                        }
                        else
                        {
                            // Good enough ending
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 18, CameraSwapEffects::eUnknown_11, 17, 0);
                            field_24_state = GameEnderController_States::eGoodEnding_4;
                        }
                    }
                    else
                    {
                        if (sKilledMudokons_5C1BC0 >= Path_BadEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Very bad ending
                            gAbeBulletProof_5C1BDA = TRUE;
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 15, CameraSwapEffects::eUnknown_11, 18, 0);
                            field_24_state = GameEnderController_States::eBadEnding_3;
                        }
                        else
                        {
                            // Bad ending
                            gAbeBulletProof_5C1BDA = FALSE;
                            gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 16, CameraSwapEffects::eUnknown_11, 18, 0);
                            field_24_state = GameEnderController_States::eBadEnding_3;
                            sRescuedMudokons_5C1BC2 = sFeecoRestart_SavedMudCount_5C1BC8;
                            sKilledMudokons_5C1BC0 = sFeeco_Restart_KilledMudCount_5C1BC6;
                        }
                        sVisitedBonewerks_5C1C02 = FALSE;
                        sVisitedBarracks_5C1C04 = FALSE;
                        sVisitedFeecoEnder_5C1C06 = FALSE;
                    }
                }
            }
        }
        break;

        case GameEnderController_States::eBadEnding_3:
            if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm) || Input().isHeld(InputCommands::Enum::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eFeeCoDepot, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_24_state = GameEnderController_States::eFinish_2;
            }
            break;

        case GameEnderController_States::eGoodEnding_4:
            if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm) || Input().isHeld(InputCommands::Enum::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_24_state = GameEnderController_States::eFinish_2;
            }
            break;

        case GameEnderController_States::eAngelicEnding_5:
            if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm) || Input().isHeld(InputCommands::Enum::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eBrewery_Ender, 1, 20, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_24_state = GameEnderController_States::eAngelicEndingCredits_6;
            }
            break;

        case GameEnderController_States::eAngelicEndingCredits_6:
            if (Input().isHeld(InputCommands::Enum::eUnPause_OrConfirm) || Input().isHeld(InputCommands::Enum::eBack))
            {
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 2, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
                gMap.mFreeAllAnimAndPalts = TRUE;
                field_24_state = GameEnderController_States::eFinish_2;
            }
            break;

        default:
            return;
    }
}
