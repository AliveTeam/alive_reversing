#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "GameEnderController.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Input.hpp"
#include "ThrowableArray.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Input.hpp"
#include "DDCheat.hpp"
#include "PauseMenu.hpp"
#include "Game.hpp"
#include "Grenade.hpp"
#include "GasCountDown.hpp"
#include "PathDataExtensions.hpp"

namespace AO {

void GameEnderController::CreateGameEnderController()
{
    relive_new GameEnderController();
}

GameEnderController::GameEnderController()
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eGameEnderController);
    mState = GameEnderControllerStates::eInit_0;
}

void GameEnderController::VScreenChanged()
{
    if (gMap.LevelChanged())
    {
        SetDead(true);
    }
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
            if (gSwitchStates.mData[70])
            {
                mState = GameEnderControllerStates::eDetermineEnding_1;
                mTimer = MakeTimer(50);
            }
            break;

        case GameEnderControllerStates::eDetermineEnding_1:
            if (mTimer <= static_cast<s32>(sGnFrame))
            {
                // Wait for murdering everyone to finish
                if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_162_ToShrykull && sActiveHero->mCurrentMotion != eAbeMotions::Motion_163_ShrykullEnd)
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

                        if (pObjIter->Type() == ReliveTypes::eParticleBurst || pObjIter->Type() == ReliveTypes::eAlarm || pObjIter->Type() == ReliveTypes::eUXB || pObjIter->Type() == ReliveTypes::eMeatSaw || pObjIter->Type() == ReliveTypes::eSlig || // EvenWurst Wieners fix - uses these extra objects that need to be deleted
                            pObjIter->Type() == ReliveTypes::eSligSpawner || pObjIter->Type() == ReliveTypes::eSlog)
                        {
                            pObjIter->SetDead(true);
                        }
                    }

                    if (sRescuedMudokons >= Path_GoodEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                    {
                        // Stop the death timer
                        sGasTimer = 0;

                        gInfiniteGrenades = false;
                        gRestartRuptureFarmsKilledMuds = 0;
                        gRestartRuptureFarmsSavedMuds = 0;

                        if (gPauseMenu)
                        {
                            gPauseMenu->SetDead(true);
                            gPauseMenu = nullptr;
                        }

                        if (sRescuedMudokons >= Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Perfect ending
                            sActiveHero->SetDead(true);
                            gMap.SetActiveCam(EReliveLevelIds::eBoardRoom, 6, 11, CameraSwapEffects::eUnknown_11, 316, 0);
                            mState = GameEnderControllerStates::ePerfectEnding_4;
                        }
                        else
                        {
                            // Meh good enough ending
                            gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, 1, CameraSwapEffects::eUnknown_11, 316, 0);
                            mState = GameEnderControllerStates::eFinish_2;
                        }
                    }
                    else
                    {
                        if (sKilledMudokons >= Path_BadEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Very bad ending
                            gInfiniteGrenades = true;

                            if (!gThrowableArray)
                            {
                                gThrowableArray = relive_new ThrowableArray();
                                if (gThrowableArray)
                                {
                                    gThrowableArray->Add(1);
                                }
                            }

                            sKilledMudokons = gRestartRuptureFarmsKilledMuds;
                            sRescuedMudokons = gRestartRuptureFarmsSavedMuds;
                            sActiveHero->SetDead(true);

                            gMap.SetActiveCam(EReliveLevelIds::eBoardRoom, 6, 9, CameraSwapEffects::eUnknown_11, 304, 0);

                            mState = GameEnderControllerStates::eBadEnding_3;
                        }
                        else
                        {
                            // Bad ending
                            sActiveHero->SetDead(true);

                            gInfiniteGrenades = false;

                            gMap.SetActiveCam(EReliveLevelIds::eBoardRoom, 6, 10, CameraSwapEffects::eUnknown_11, 304, 0);
                            mState = GameEnderControllerStates::eBadEnding_3;
                            sRescuedMudokons = gRestartRuptureFarmsSavedMuds;
                            sKilledMudokons = gRestartRuptureFarmsKilledMuds;
                        }
                    }
                }
            }
            break;

        case GameEnderControllerStates::eBadEnding_3:
            if (Input().IsAnyPressed(InputCommands::eCrouchOrRoll))
            {
                gMap.SetActiveCam(EReliveLevelIds::eRuptureFarmsReturn, 19, 3, CameraSwapEffects::eInstantChange_0, 0, 0);
                mState = GameEnderControllerStates::eFinish_2;
            }
            break;

        case GameEnderControllerStates::ePerfectEnding_4:
            if (Input().IsAnyPressed(InputCommands::eCrouchOrRoll))
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, CameraIds::Menu::eFmvSelect_30, CameraSwapEffects::eInstantChange_0, 0, 0);
                gMap.mFreeAllAnimAndPalts = true;
                mState = GameEnderControllerStates::eFinish_2;
            }
            break;

        default:
            return;
    }
}

} // namespace AO
