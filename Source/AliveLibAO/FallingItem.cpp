#include "stdafx_ao.h"
#include "Function.hpp"
#include "FallingItem.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "ScreenShake.hpp"
#include "ParticleBurst.hpp"
#include "Midi.hpp"
#include "SwitchStates.hpp"

namespace AO {

ALIVE_VAR(1, 0x4FFA54, FallingItem*, pPrimaryFallingItem_4FFA54, nullptr);

const FallingItem_Data sFallingItemData_4BAB20[16] =
{
  { 11504, 11544, 76, 38 },
  { 12136, 12148, 66, 42 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 12136, 12148, 66, 42 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 }
};

FallingItem* FallingItem::ctor_419F30(Path_FallingItem* pTlv, int tlvInfo)
{
    ctor_401090();
    field_6_flags.Set(Options::eCanExplode_Bit7);
    SetVTable(this, 0x4BABE0);

    field_4_typeId = Types::eRockSpawner_32;

    field_10C_tlvInfo = tlvInfo;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kF2rockResID, 1, 0);
    const int lvlIdx = static_cast<int>(gMap_507BA8.field_0_current_level);
    Animation_Init_417FD0(
        sFallingItemData_4BAB20[lvlIdx].field_0,
        sFallingItemData_4BAB20[lvlIdx].field_8,
        sFallingItemData_4BAB20[lvlIdx].field_A,
        ppRes,
        1);

    field_10_anim.field_C_layer = 31;
    if (gMap_507BA8.field_0_current_level == LevelIds::eLines_2)
    {
        field_C0_r = 77;
        field_C2_g = 120;
        field_C4_b = 190;
    }

    field_112_id = pTlv->field_18_id;
    if (pTlv->field_1A_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }


    field_118_delay_time = pTlv->field_1C_delay_time;
    field_114_num_items = pTlv->field_1E_number_of_items;
    field_116_num_items_remaining = pTlv->field_1E_number_of_items;

    field_120_reset_id = pTlv->field_20_reset_id;
    field_122_do_sound_in_state_falling = 1;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    field_128_xpos = FP_FromInteger((pTlv->field_14_bottom_right.field_0_x + pTlv->field_10_top_left.field_0_x) / 2);
    field_12C_ypos = FP_FromInteger(pTlv->field_14_bottom_right.field_2_y);

    field_124_yPosStart = field_AC_ypos;
    field_110_state = State::eState_0_WaitForIdEnable;
    field_130_sound_channels = 0;

    // Not sure why this rupture farms primary item hack is required
    if (!pPrimaryFallingItem_4FFA54 && (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13))
    {
        pPrimaryFallingItem_4FFA54 = this;
        field_134_created_gnFrame = gnFrameCount_507670;
    }

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseGameObject* FallingItem::dtor_41A660()
{
    SetVTable(this, 0x4BABE0);
    if (pPrimaryFallingItem_4FFA54 == this)
    {
        pPrimaryFallingItem_4FFA54 = nullptr;
    }
    gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    return dtor_401000();
}

FallingItem* FallingItem::Vdtor_41A7F0(signed int flags)
{
    dtor_41A660();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void FallingItem::DamageHitItems_41A6D0()
{
    for (int idx = 0; idx < gBaseGameObject_list_9F2DF0->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj != this)
        {
            if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
            {
                BaseAnimatedWithPhysicsGameObject* pAliveObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                PSX_RECT fallingItemRect = {};
                VGetBoundingRect(&fallingItemRect, 1);

                PSX_RECT objRect = {};
                pAliveObj->VGetBoundingRect(&objRect, 1);

                if (fallingItemRect.x <= objRect.w && fallingItemRect.w >= objRect.x &&
                    fallingItemRect.y <= objRect.h && fallingItemRect.h >= objRect.y)
                {
                    if (pAliveObj->field_BC_sprite_scale == field_BC_sprite_scale)
                    {
                        static_cast<BaseAliveGameObject*>(pAliveObj)->VTakeDamage(this);
                    }
                }
            }
        }
    }
}

void FallingItem::VUpdate_41A120()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (pPrimaryFallingItem_4FFA54 == this)
    {
        if (!((gnFrameCount_507670 - field_134_created_gnFrame) % 87))
        {
            SFX_Play_43AD70(SoundEffect::MeatsawOffscreen_88, 45, 0);
        }

        if (!((gnFrameCount_507670 - field_134_created_gnFrame) % 25))
        {
            SFX_Play_43AD70(SoundEffect::MeatsawIdle_89, 45, 0);

        }
    }

    switch (field_110_state)
    {
    case State::eState_0_WaitForIdEnable:
        if (SwitchStates_Get(field_112_id))
        {
            field_6_flags.Clear(Options::eCanExplode_Bit7);
            field_110_state = State::eState_1_GoWaitForDelay;
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
            field_10_anim.Set_Animation_Data_402A40(sFallingItemData_4BAB20[static_cast<int>(gMap_507BA8.field_0_current_level)].field_4, nullptr);
            field_11C_delay_timer = gnFrameCount_507670 + field_118_delay_time;
        }
        break;

    case State::eState_1_GoWaitForDelay:
        field_6_flags.Clear(Options::eCanExplode_Bit7);
        field_110_state = State::eState_2_WaitForFallDelay;
        field_B4_velx = FP_FromInteger(0);
        field_B8_vely = FP_FromInteger(0);
        field_10_anim.Set_Animation_Data_402A40(sFallingItemData_4BAB20[static_cast<int>(gMap_507BA8.field_0_current_level)].field_4, nullptr);
        field_11C_delay_timer = gnFrameCount_507670 + field_118_delay_time;
        break;

    case State::eState_2_WaitForFallDelay:
        if (static_cast<int>(gnFrameCount_507670) >= field_11C_delay_timer)
        {
            field_110_state = State::eState_3_Falling;
            field_122_do_sound_in_state_falling = TRUE;
            field_130_sound_channels = SFX_Play_43AE60(SoundEffect::AirStream_28, 50, -2600, 0);
        }
        break;

    case State::eState_3_Falling:
    {
        if (field_122_do_sound_in_state_falling)
        {
            if (field_AC_ypos >= sActiveHero_507678->field_AC_ypos - FP_FromInteger(120))
            {
                field_122_do_sound_in_state_falling = 0;
                SFX_Play_43AE60(SoundEffect::AirStream_28, 127, -1300, 0);
            }
        }

        DamageHitItems_41A6D0();

        if (field_B8_vely < FP_FromInteger(20))
        {
            field_B8_vely += field_BC_sprite_scale * FP_FromDouble(1.8);
        }

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        if (sCollisions_DArray_504C6C->RayCast_40C410(
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_B8_vely + field_AC_ypos,
            &pLine,
            &hitX,
            &hitY,
            field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 16) == 1)
        {
            field_AC_ypos = hitY;
            field_110_state = State::eState_4_Smashed;

            ScreenShake* pScreenShake = ao_new<ScreenShake>();
            if (pScreenShake)
            {
                pScreenShake->ctor_4624D0(0);
            }

            if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 ||
                gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
            {
                ParticleBurst* pParticleBurst = ao_new<ParticleBurst>();
                if (pParticleBurst)
                {
                    pParticleBurst->ctor_40D0F0(
                        field_A8_xpos,
                        field_AC_ypos,
                        25,
                        field_BC_sprite_scale,
                        BurstType::eType_4);
                }
            }
            else
            {
                ParticleBurst* pParticleBurst = ao_new<ParticleBurst>();
                if (pParticleBurst)
                {
                    pParticleBurst->ctor_40D0F0(
                        field_A8_xpos,
                        field_AC_ypos,
                        25,
                        field_BC_sprite_scale,
                        BurstType::eType_0);
                }
            }

            ParticleBurst* pParticleBurst = ao_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_40D0F0(
                    field_A8_xpos,
                    field_AC_ypos,
                    25,
                    field_BC_sprite_scale,
                    BurstType::eType_1);
            }
        }
        else
        {
            field_AC_ypos += field_B8_vely;
        }
        break;
    }

    case State::eState_4_Smashed:
    {
        if (field_130_sound_channels)
        {
            SND_Stop_Channels_Mask_4774A0(field_130_sound_channels);
            field_130_sound_channels = 0;
        }

        if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 ||
            gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
        {
            if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0))
            {
                SFX_Play_43AE60(SoundEffect::KillEffect_78, 127, -700, 0);
                SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 110, 0);
            }
            else
            {
                SND_SEQ_Play_477760(SeqId::Unknown_25, 1, 65, 65);
                SFX_Play_43AE60(SoundEffect::KillEffect_78, 90, -700, 0);
                SFX_Play_43AD70(SoundEffect::FallingItemHit_53, 33, 0);
                SFX_Play_43AE60(SoundEffect::MeatsawUp_90, 80, -400, 0);
            }
        }
        else
        {
            SFX_Play_43AD70(SoundEffect::FallingItemLand_73, 0, 0);
            SFX_Play_43AE60(SoundEffect::FallingItemHit_53, 110, -1536, 0);
        }

        if (field_112_id)
        {
            if (field_120_reset_id)
            {
                SwitchStates_Do_Operation_436A10(field_112_id, SwitchOp::eSetFalse_1);
            }
        }

        field_116_num_items_remaining--;

        if (field_114_num_items && field_116_num_items_remaining <= 0 || !gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_128_xpos,
                field_12C_ypos,
                0))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        else
        {
            field_10_anim.Set_Animation_Data_402A40(sFallingItemData_4BAB20[static_cast<int>(gMap_507BA8.field_0_current_level)].field_0, nullptr);
            field_6_flags.Set(Options::eCanExplode_Bit7);
            field_B8_vely = FP_FromInteger(0);
            field_B4_velx = FP_FromInteger(0);
            field_AC_ypos = field_124_yPosStart;
            field_110_state = State::eState_0_WaitForIdEnable;
        }
        break;
    }

    default:
        return;
    }
}

void FallingItem::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    // Empty
}

void FallingItem::VScreenChanged_41A7C0()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path
        || field_110_state != State::eState_3_Falling)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}


void FallingItem::VUpdate()
{
    VUpdate_41A120();
}

void FallingItem::VScreenChanged()
{
    VScreenChanged_41A7C0();
}

BaseGameObject* FallingItem::VDestructor(signed int flags)
{
    return Vdtor_41A7F0(flags);
}

}
