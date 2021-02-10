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

void GameEnderController_ForceLink() {}

namespace AO {

void CC GameEnderController::CreateGameEnderController_41C7D0()
{
    auto pGameEnderController = ao_new<GameEnderController>();
    if (pGameEnderController)
    {
        pGameEnderController->ctor_487E10(1);
        SetVTable(pGameEnderController, 0x4BAEF0);
        pGameEnderController->field_4_typeId = Types::eGameEnderController_37;
        pGameEnderController->field_14_state = 0;
    }
}

BaseGameObject* GameEnderController::dtor_41C850()
{
    SetVTable(this, 0x4BAEF0);
    return dtor_487DF0();
}

BaseGameObject* GameEnderController::VDestructor(signed int flags)
{
    return Vdtor_41CD00(flags);
}

GameEnderController* GameEnderController::Vdtor_41CD00(signed int flags)
{
    dtor_41C850();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void GameEnderController::VScreenChanged()
{
    VScreenChanged_41CCE0();
}

void GameEnderController::VScreenChanged_41CCE0()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void GameEnderController::VUpdate()
{
    VUpdate_41C860();
}

void GameEnderController::VUpdate_41C860()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    switch (field_14_state)
    {
    case 0:
        if (sSwitchStates_505568.mData[70])
        {
            field_14_state = 1;
            field_10_timer = gnFrameCount_507670 + 50;
        }
        break;

    case 1:
        if (field_10_timer <= static_cast<int>(gnFrameCount_507670))
        {
            // Wait for murdering everyone to finish
            if (sActiveHero_507678->field_FC_current_motion != eAbeStates::State_162_ToShrykull_42F410 &&
                sActiveHero_507678->field_FC_current_motion != eAbeStates::State_163_ShrykullEnd_42F520)
            {
                // Kill certain types of objects to prevent them appearing on the ending screen or crashing the game
                // by trying to use the sControlledCharacter_50767C pointer.
                for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
                {
                    BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
                    if (!pObjIter)
                    {
                        break;
                    }

                    if (pObjIter->field_4_typeId == Types::eParticleBurst_19 ||
                        pObjIter->field_4_typeId == Types::eAlarm_1 ||
                        pObjIter->field_4_typeId == Types::eUXB_99 ||
                        pObjIter->field_4_typeId == Types::eMeatSaw_56 ||
                        pObjIter->field_4_typeId == Types::eSlig_88 || // EvenWurst Wieners fix - uses these extra objects that need to be deleted
                        pObjIter->field_4_typeId == Types::eSligSpawner_91 ||
                        pObjIter->field_4_typeId == Types::eSlog_89
                        )
                    {
                        pObjIter->field_6_flags.Set(Options::eDead_Bit3);
                    }
                }

                if (sRescuedMudokons_5076C0 < 50)
                {
                    if (sKilledMudokons_5076BC < 75)
                    {
                        // Bad ending
                        sActiveHero_507678->field_6_flags.Set(Options::eDead_Bit3);

                        gInfiniteGrenades_5076EC = FALSE;

                        gMap_507BA8.SetActiveCam_444660(LevelIds::eBoardRoom_12, 6, 10, CameraSwapEffects::eEffect11, 304, 0);
                        field_14_state = 3;
                        sRescuedMudokons_5076C0 = gRestartRuptureFarmsSavedMuds_5076C8;
                        sKilledMudokons_5076BC = gRestartRuptureFarmsKilledMuds_5076C4;
                    }
                    else
                    {
                        // Very bad ending
                        gInfiniteGrenades_5076EC = TRUE;

                        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbepickResID, 1, 0))
                        {
                            ResourceManager::LoadResourceFile_455270("ABEPICK.BAN", 0);
                            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbepickResID, 1, 0);
                        }
                        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbethrowResID, 1, 0))
                        {
                            ResourceManager::LoadResourceFile_455270("ABETHROW.BAN", 0);
                            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbethrowResID, 1, 0);
                        }
                        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kGrenadeResID, 1, 0))
                        {
                            ResourceManager::LoadResourceFile_455270("GRENADE.BAN", 0);
                            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kGrenadeResID, 1, 0);
                        }
                        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, 1, 0))
                        {
                            ResourceManager::LoadResourceFile_455270("EXPLO2.BAN", 0);
                            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, 1, 0);
                        }
                        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 1, 0))
                        {
                            ResourceManager::LoadResourceFile_455270("ABEBLOW.BAN", 0);
                            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 1, 0);
                        }
                        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMetalGib, 1, 0))
                        {
                            ResourceManager::LoadResourceFile_455270("METAL.BAN", 0);
                            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMetalGib, 1, 0);
                        }

                        if (!gpThrowableArray_50E26C)
                        {
                            gpThrowableArray_50E26C = ao_new<ThrowableArray>();
                            if (gpThrowableArray_50E26C)
                            {
                                gpThrowableArray_50E26C->ctor_453EE0();
                            }
                        }
                        gpThrowableArray_50E26C->Add_453F70(1);

                        sKilledMudokons_5076BC = gRestartRuptureFarmsKilledMuds_5076C4;
                        sRescuedMudokons_5076C0 = gRestartRuptureFarmsSavedMuds_5076C8;
                        sActiveHero_507678->field_6_flags.Set(Options::eDead_Bit3);

                        gMap_507BA8.SetActiveCam_444660(LevelIds::eBoardRoom_12, 6, 9, CameraSwapEffects::eEffect11, 304, 0);

                        field_14_state = 3;
                    }
                }
                else
                {
                    // Stop the death timer
                    sGasTimer_507700 = 0;

                    gInfiniteGrenades_5076EC = 0;
                    gRestartRuptureFarmsKilledMuds_5076C4 = 0;
                    gRestartRuptureFarmsSavedMuds_5076C8 = 0;

                    if (pPauseMenu_5080E0)
                    {
                        pPauseMenu_5080E0->field_6_flags.Set(Options::eDead_Bit3);
                        pPauseMenu_5080E0 = nullptr;
                    }

                    if (sRescuedMudokons_5076C0 < 99)
                    {
                        // Meh good enough ending
                        gMap_507BA8.SetActiveCam_444660(LevelIds::eCredits_10, 1, 1, CameraSwapEffects::eEffect11, 316, 0);
                        field_14_state = 2;
                    }
                    else
                    {
                        // Perfect ending
                        sActiveHero_507678->field_6_flags.Set(Options::eDead_Bit3);
                        gMap_507BA8.SetActiveCam_444660(LevelIds::eBoardRoom_12, 6, 11, CameraSwapEffects::eEffect11, 316, 0);
                        field_14_state = 4;
                    }
                }

            }
        }
        break;

    case 3:
        if (Input().IsAnyHeld(sInputKey_FartRoll_4C65B0))
        {
            gMap_507BA8.SetActiveCam_444660(LevelIds::eRuptureFarmsReturn_13, 19, 3, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            field_14_state = 2;
        }
        break;

    case 4:
        if (Input().IsAnyHeld(sInputKey_FartRoll_4C65B0))
        {
            gMap_507BA8.SetActiveCam_444660(LevelIds::eMenu_0, 1, 30, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            gMap_507BA8.field_DC_free_all_anim_and_palts = 1;
            field_14_state = 2;
        }
        break;

    default:
        return;
    }
}

}
