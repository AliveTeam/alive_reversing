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

const FallingItem_Data sFallingItemData_4BAB20[16] = {
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // menu
    {AnimId::FallingMeat_Falling, AnimId::FallingMeat_Waiting, 66, 42},       // rupture farms
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // lines
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // forest
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // forest temple
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // stock yards
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // stock yards return
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // removed
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // desert
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // desert temple
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // credits
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // removed
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // board room
    {AnimId::FallingMeat_Falling, AnimId::FallingMeat_Waiting, 66, 42},       // rupture farms return
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // forest chase
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}};// desert escape

FallingItem::FallingItem(Path_FallingItem* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mFlags.Set(Options::eCanExplode_Bit7);

    field_4_typeId = Types::eRockSpawner_32;

    field_10C_tlvInfo = tlvInfo;

    const s32 lvlIdx = static_cast<s32>(gMap.mCurrentLevel);
    const AnimRecord& rec = AO::AnimRec(sFallingItemData_4BAB20[lvlIdx].field_0_falling_animId);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(
        rec.mFrameTableOffset,
        rec.mMaxW,
        rec.mMaxH,
        ppRes,
        1);

    field_10_anim.field_C_layer = Layer::eLayer_DoorFlameRollingBallPortalClip_Half_31;
    if (gMap.mCurrentLevel == LevelIds::eLines_2)
    {
        field_C0_r = 77;
        field_C2_g = 120;
        field_C4_b = 190;
    }

    field_112_switch_id = pTlv->field_18_switch_id;
    if (pTlv->field_1A_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }


    field_118_fall_interval = pTlv->field_1C_fall_interval;
    field_114_max_falling_items = pTlv->field_1E_max_falling_items;
    field_116_remaining_falling_items = pTlv->field_1E_max_falling_items;

    field_120_reset_switch_id_after_use = pTlv->field_20_reset_switch_id_after_use;
    field_122_do_sound_in_state_falling = 1;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_128_xpos = FP_FromInteger((pTlv->field_14_bottom_right.field_0_x + pTlv->field_10_top_left.field_0_x) / 2);
    field_12C_ypos = FP_FromInteger(pTlv->field_14_bottom_right.field_2_y);

    field_124_yPosStart = field_AC_ypos;
    field_110_state = State::eWaitForIdEnable_0;
    field_130_sound_channels = 0;

    // Not sure why this rupture farms primary item hack is required
    if (!pPrimaryFallingItem_4FFA54 && (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13))
    {
        pPrimaryFallingItem_4FFA54 = this;
        field_134_created_gnFrame = gnFrameCount_507670;
    }

    field_D0_pShadow = ao_new<Shadow>();
}

FallingItem::~FallingItem()
{
    if (pPrimaryFallingItem_4FFA54 == this)
    {
        pPrimaryFallingItem_4FFA54 = nullptr;
    }
    gMap.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
}

void FallingItem::DamageHitItems_41A6D0()
{
    for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj != this)
        {
            if (pObj->mFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
            {
                BaseAnimatedWithPhysicsGameObject* pAliveObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                PSX_RECT fallingItemRect = {};
                VGetBoundingRect(&fallingItemRect, 1);

                PSX_RECT objRect = {};
                pAliveObj->VGetBoundingRect(&objRect, 1);

                if (fallingItemRect.x <= objRect.w && fallingItemRect.w >= objRect.x && fallingItemRect.y <= objRect.h && fallingItemRect.h >= objRect.y)
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
        mFlags.Set(BaseGameObject::eDead);
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
        case State::eWaitForIdEnable_0:
            if (!SwitchStates_Get(field_112_switch_id))
            {
                return;
            }
            [[fallthrough]];

        case State::eGoWaitForDelay_1:
        {
            mFlags.Clear(Options::eCanExplode_Bit7);
            field_110_state = State::eWaitForFallDelay_2;
            field_B4_velx = FP_FromInteger(0);
            field_B8_vely = FP_FromInteger(0);
            const AnimRecord& rec = AO::AnimRec(sFallingItemData_4BAB20[static_cast<s32>(gMap.mCurrentLevel)].field_4_waiting_animId);
            field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
            field_11C_delay_timer = gnFrameCount_507670 + field_118_fall_interval;
            break;
        }

        case State::eWaitForFallDelay_2:
            if (static_cast<s32>(gnFrameCount_507670) >= field_11C_delay_timer)
            {
                field_110_state = State::eFalling_3;
                field_122_do_sound_in_state_falling = TRUE;
                field_130_sound_channels = SFX_Play_43AE60(SoundEffect::AirStream_28, 50, -2600, 0);
            }
            break;

        case State::eFalling_3:
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
                    field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 16)
                == 1)
            {
                field_AC_ypos = hitY;
                field_110_state = State::eSmashed_4;

                ao_new<ScreenShake>(0);

                if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
                {
                    ao_new<ParticleBurst>(
                        field_A8_xpos,
                        field_AC_ypos,
                        25,
                        field_BC_sprite_scale,
                        BurstType::eMeat_4);
                }
                else
                {
                    ao_new<ParticleBurst>(
                        field_A8_xpos,
                        field_AC_ypos,
                        25,
                        field_BC_sprite_scale,
                        BurstType::eFallingRocks_0);
                }

                ao_new<ParticleBurst>(
                    field_A8_xpos,
                    field_AC_ypos,
                    25,
                    field_BC_sprite_scale,
                    BurstType::eSticks_1);
            }
            else
            {
                field_AC_ypos += field_B8_vely;
            }
            break;
        }

        case State::eSmashed_4:
        {
            if (field_130_sound_channels)
            {
                SND_Stop_Channels_Mask_4774A0(field_130_sound_channels);
                field_130_sound_channels = 0;
            }

            if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
            {
                if (gMap.Is_Point_In_Current_Camera_4449C0(
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

            if (field_112_switch_id)
            {
                if (field_120_reset_switch_id_after_use == Choice_short::eYes_1)
                {
                    SwitchStates_Do_Operation_436A10(field_112_switch_id, SwitchOp::eSetFalse_1);
                }
            }

            field_116_remaining_falling_items--;

            if ((field_114_max_falling_items && field_116_remaining_falling_items <= 0) || !gMap.Is_Point_In_Current_Camera_4449C0(field_B2_lvl_number, field_B0_path_number, field_128_xpos, field_12C_ypos, 0))
            {
                mFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                const AnimRecord& rec = AO::AnimRec(sFallingItemData_4BAB20[static_cast<s32>(gMap.mCurrentLevel)].field_0_falling_animId);
                field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
                mFlags.Set(Options::eCanExplode_Bit7);
                field_B8_vely = FP_FromInteger(0);
                field_B4_velx = FP_FromInteger(0);
                field_AC_ypos = field_124_yPosStart;
                field_110_state = State::eWaitForIdEnable_0;
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
    if (gMap.mCurrentLevel != gMap.mLevel
        || gMap.mCurrentPath != gMap.mPath
        || field_110_state != State::eFalling_3)
    {
        mFlags.Set(BaseGameObject::eDead);
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

} // namespace AO
