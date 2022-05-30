#include "stdafx_ao.h"
#include "Function.hpp"
#include "GameEnderController.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Events.hpp"
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
    ao_new<GameEnderController>();
}

GameEnderController::GameEnderController()
    : BaseGameObject(1)
{
    field_4_typeId = Types::eGameEnderController_37;
    field_14_state = GameEnderController_States::eInit_0;
}

void GameEnderController::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void GameEnderController::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    switch (field_14_state)
    {
        case GameEnderController_States::eInit_0:
            if (sSwitchStates_505568.mData[70])
            {
                field_14_state = GameEnderController_States::eDetermineEnding_1;
                field_10_timer = gnFrameCount_507670 + 50;
            }
            break;

        case GameEnderController_States::eDetermineEnding_1:
            if (field_10_timer <= static_cast<s32>(gnFrameCount_507670))
            {
                // Wait for murdering everyone to finish
                if (sActiveHero_507678->field_FC_current_motion != eAbeMotions::Motion_162_ToShrykull_42F410 && sActiveHero_507678->field_FC_current_motion != eAbeMotions::Motion_163_ShrykullEnd_42F520)
                {
                    // Kill certain types of objects to prevent them appearing on the ending screen or crashing the game
                    // by trying to use the sControlledCharacter_50767C pointer.
                    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                    {
                        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
                        if (!pObjIter)
                        {
                            break;
                        }

                        if (pObjIter->field_4_typeId == Types::eParticleBurst_19 || pObjIter->field_4_typeId == Types::eAlarm_1 || pObjIter->field_4_typeId == Types::eUXB_99 || pObjIter->field_4_typeId == Types::eMeatSaw_56 || pObjIter->field_4_typeId == Types::eSlig_88 || // EvenWurst Wieners fix - uses these extra objects that need to be deleted
                            pObjIter->field_4_typeId == Types::eSligSpawner_91 || pObjIter->field_4_typeId == Types::eSlog_89)
                        {
                            pObjIter->mFlags.Set(Options::eDead);
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
                            pPauseMenu_5080E0->mFlags.Set(Options::eDead);
                            pPauseMenu_5080E0 = nullptr;
                        }

                        if (sRescuedMudokons_5076C0 >= Path_GetTotalMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Perfect ending
                            sActiveHero_507678->mFlags.Set(Options::eDead);
                            gMap.SetActiveCam_444660(LevelIds::eBoardRoom_12, 6, 11, CameraSwapEffects::eUnknown_11, 316, 0);
                            field_14_state = GameEnderController_States::ePerfectEnding_4;
                        }
                        else
                        {
                            // Meh good enough ending
                            gMap.SetActiveCam_444660(LevelIds::eCredits_10, 1, 1, CameraSwapEffects::eUnknown_11, 316, 0);
                            field_14_state = GameEnderController_States::eFinish_2;
                        }
                    }
                    else
                    {
                        if (sKilledMudokons_5076BC >= Path_BadEndingMuds(gMap.mCurrentLevel, gMap.mCurrentPath))
                        {
                            // Very bad ending
                            gInfiniteGrenades_5076EC = TRUE;

                            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID, 1, 0))
                            {
                                ResourceManager::LoadResourceFile_455270("ABEPICK.BAN", 0);
                                ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID, 1, 0);
                            }
                            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID, 1, 0))
                            {
                                ResourceManager::LoadResourceFile_455270("ABETHROW.BAN", 0);
                                ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID, 1, 0);
                            }
                            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kGrenadeAOResID, 1, 0))
                            {
                                ResourceManager::LoadResourceFile_455270("GRENADE.BAN", 0);
                                ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kGrenadeAOResID, 1, 0);
                            }
                            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, 1, 0))
                            {
                                ResourceManager::LoadResourceFile_455270("EXPLO2.BAN", 0);
                                ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, 1, 0);
                            }
                            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0))
                            {
                                ResourceManager::LoadResourceFile_455270("ABEBLOW.BAN", 0);
                                ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0);
                            }
                            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, 1, 0))
                            {
                                ResourceManager::LoadResourceFile_455270("METAL.BAN", 0);
                                ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, 1, 0);
                            }

                            if (!gpThrowableArray_50E26C)
                            {
                                gpThrowableArray_50E26C = ao_new<ThrowableArray>();
                                if (gpThrowableArray_50E26C)
                                {
                                    gpThrowableArray_50E26C->Add_453F70(1);
                                }
                            }

                            sKilledMudokons_5076BC = gRestartRuptureFarmsKilledMuds_5076C4;
                            sRescuedMudokons_5076C0 = gRestartRuptureFarmsSavedMuds_5076C8;
                            sActiveHero_507678->mFlags.Set(Options::eDead);

                            gMap.SetActiveCam_444660(LevelIds::eBoardRoom_12, 6, 9, CameraSwapEffects::eUnknown_11, 304, 0);

                            field_14_state = GameEnderController_States::eBadEnding_3;
                        }
                        else
                        {
                            // Bad ending
                            sActiveHero_507678->mFlags.Set(Options::eDead);

                            gInfiniteGrenades_5076EC = FALSE;

                            gMap.SetActiveCam_444660(LevelIds::eBoardRoom_12, 6, 10, CameraSwapEffects::eUnknown_11, 304, 0);
                            field_14_state = GameEnderController_States::eBadEnding_3;
                            sRescuedMudokons_5076C0 = gRestartRuptureFarmsSavedMuds_5076C8;
                            sKilledMudokons_5076BC = gRestartRuptureFarmsKilledMuds_5076C4;
                        }
                    }
                }
            }
            break;

        case GameEnderController_States::eBadEnding_3:
            if (Input().IsAnyHeld(sInputKey_FartRoll_4C65B0))
            {
                gMap.SetActiveCam_444660(LevelIds::eRuptureFarmsReturn_13, 19, 3, CameraSwapEffects::eInstantChange_0, 0, 0);
                field_14_state = GameEnderController_States::eFinish_2;
            }
            break;

        case GameEnderController_States::ePerfectEnding_4:
            if (Input().IsAnyHeld(sInputKey_FartRoll_4C65B0))
            {
                gMap.SetActiveCam_444660(LevelIds::eMenu_0, 1, CameraIds::Menu::eFmvSelect_30, CameraSwapEffects::eInstantChange_0, 0, 0);
                gMap.field_DC_free_all_anim_and_palts = 1;
                field_14_state = GameEnderController_States::eFinish_2;
            }
            break;

        default:
            return;
    }
}

} // namespace AO
