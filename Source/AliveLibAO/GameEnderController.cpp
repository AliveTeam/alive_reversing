#include "stdafx_ao.h"
#include "Function.hpp"
#include "GameEnderController.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Input.hpp"
#include "ThrowableArray.hpp"
#include "SwitchStates.hpp"
#include "ResourceManager.hpp"
#include "Input.hpp"
#include "DDCheat.hpp"
#include "PauseMenu.hpp"
#include "Game.hpp"
#include "Grenade.hpp"
#include "GasCountDown.hpp"
#include "PathDataExtensions.hpp"

void GameEnderController_ForceLink()
{ }

namespace AO {

void GameEnderController::CreateGameEnderController()
{
    relive_new GameEnderController();
}

GameEnderController::GameEnderController()
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::eGameEnderController;
    field_14_state = GameEnderController_States::eInit_0;
}

void GameEnderController::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void GameEnderController::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_14_state)
    {
        case GameEnderController_States::eInit_0:
            if (sSwitchStates_505568.mData[70])
            {
                field_14_state = GameEnderController_States::eDetermineEnding_1;
                field_10_timer = sGnFrame + 50;
            }
            break;

        case GameEnderController_States::eDetermineEnding_1:
            if (field_10_timer <= static_cast<s32>(sGnFrame))
            {
                // Wait for murdering everyone to finish
                if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_162_ToShrykull_42F410 && sActiveHero->mCurrentMotion != eAbeMotions::Motion_163_ShrykullEnd_42F520)
                {
                    // Kill certain types of objects to prevent them appearing on the ending screen or crashing the game
                    // by trying to use the sControlledCharacter pointer.
                    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                    {
                        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
                        if (!pObjIter)
                        {
                            break;
                        }

                        if (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eParticleBurst || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eAlarm || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eUXB || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eMeatSaw || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eSlig || // EvenWurst Wieners fix - uses these extra objects that need to be deleted
                            pObjIter->mBaseGameObjectTypeId == ReliveTypes::eSligSpawner || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eSlog)
                        {
                            pObjIter->mBaseGameObjectFlags.Set(Options::eDead);
                        }
                    }

                    if (sRescuedMudokons_5076C0 >= Path_GoodEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                    {
                        // Stop the death timer
                        sGasTimer_507700 = 0;

                        gInfiniteGrenades_5076EC = 0;
                        gRestartRuptureFarmsKilledMuds_5076C4 = 0;
                        gRestartRuptureFarmsSavedMuds_5076C8 = 0;

                        if (pPauseMenu_5080E0)
                        {
                            pPauseMenu_5080E0->mBaseGameObjectFlags.Set(Options::eDead);
                            pPauseMenu_5080E0 = nullptr;
                        }

                        if (sRescuedMudokons_5076C0 >= Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Perfect ending
                            sActiveHero->mBaseGameObjectFlags.Set(Options::eDead);
                            gMap.SetActiveCam(EReliveLevelIds::eBoardRoom, 6, 11, CameraSwapEffects::eUnknown_11, 316, 0);
                            field_14_state = GameEnderController_States::ePerfectEnding_4;
                        }
                        else
                        {
                            // Meh good enough ending
                            gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, 1, CameraSwapEffects::eUnknown_11, 316, 0);
                            field_14_state = GameEnderController_States::eFinish_2;
                        }
                    }
                    else
                    {
                        if (sKilledMudokons_5076BC >= Path_BadEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Very bad ending
                            gInfiniteGrenades_5076EC = TRUE;

                            if (!gpThrowableArray_50E26C)
                            {
                                gpThrowableArray_50E26C = relive_new ThrowableArray();
                                if (gpThrowableArray_50E26C)
                                {
                                    gpThrowableArray_50E26C->Add(1);
                                }
                            }

                            sKilledMudokons_5076BC = gRestartRuptureFarmsKilledMuds_5076C4;
                            sRescuedMudokons_5076C0 = gRestartRuptureFarmsSavedMuds_5076C8;
                            sActiveHero->mBaseGameObjectFlags.Set(Options::eDead);

                            gMap.SetActiveCam(EReliveLevelIds::eBoardRoom, 6, 9, CameraSwapEffects::eUnknown_11, 304, 0);

                            field_14_state = GameEnderController_States::eBadEnding_3;
                        }
                        else
                        {
                            // Bad ending
                            sActiveHero->mBaseGameObjectFlags.Set(Options::eDead);

                            gInfiniteGrenades_5076EC = FALSE;

                            gMap.SetActiveCam(EReliveLevelIds::eBoardRoom, 6, 10, CameraSwapEffects::eUnknown_11, 304, 0);
                            field_14_state = GameEnderController_States::eBadEnding_3;
                            sRescuedMudokons_5076C0 = gRestartRuptureFarmsSavedMuds_5076C8;
                            sKilledMudokons_5076BC = gRestartRuptureFarmsKilledMuds_5076C4;
                        }
                    }
                }
            }
            break;

        case GameEnderController_States::eBadEnding_3:
            if (Input().IsAnyHeld(sInputKey_FartRoll))
            {
                gMap.SetActiveCam(EReliveLevelIds::eRuptureFarmsReturn, 19, 3, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_14_state = GameEnderController_States::eFinish_2;
            }
            break;

        case GameEnderController_States::ePerfectEnding_4:
            if (Input().IsAnyHeld(sInputKey_FartRoll))
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eFmvSelect_30, CameraSwapEffects::eInstantChange_0, 0, 0);
                gMap.field_DC_free_all_anim_and_palts = 1;
                field_14_state = GameEnderController_States::eFinish_2;
            }
            break;

        default:
            return;
    }
}

} // namespace AO
