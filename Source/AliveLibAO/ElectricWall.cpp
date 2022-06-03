#include "stdafx_ao.h"
#include "Function.hpp"
#include "ElectricWall.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "Electrocute.hpp"
#include "Flash.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

ElectricWall::ElectricWall(Path_ElectricWall* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = Types::eElectricWall_25;

    const AnimRecord& rec = AO::AnimRec(AnimId::Electric_Wall);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;
    field_10_anim.field_C_layer = Layer::eLayer_Foreground_36;

    if (field_10_anim.Get_Frame_Count() > 0)
    {
        field_10_anim.SetFrame(Math_NextRandom() % field_10_anim.Get_Frame_Count());
    }

    field_C4_b = 80;
    field_C2_g = 80;
    field_C0_r = 80;
    field_E4_tlv = tlvInfo;
    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    field_E8_switch_id = pTlv->field_1A_switch_id;
    field_EA_start_state = pTlv->field_1C_start_state;

    if (SwitchStates_Get(pTlv->field_1A_switch_id) == field_EA_start_state)
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_EC_sound_timer = 0;
}

ElectricWall::~ElectricWall()
{
    gMap.TLV_Reset(field_E4_tlv, -1, 0, 0);
}

void ElectricWall::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || gMap.GetDirection(field_B2_lvl_number, field_B0_path_number, field_A8_xpos, field_AC_ypos) == CameraPos::eCamInvalid_m1)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ElectricWall::VUpdate()
{
    const CameraPos soundDirection = gMap.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos);

    if (Event_Get(kEventDeathReset_4))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (SwitchStates_Get(field_E8_switch_id) == field_EA_start_state)
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

        // Keep flipping direction
        if (!(gnFrameCount_507670 % 8))
        {
            field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }

        // Play sound every so often
        if (static_cast<s32>(gnFrameCount_507670) >= field_EC_sound_timer)
        {
            // set a random starting frame
            SFX_Play_Camera(SoundEffect::BirdPortalSpark_48, 45, soundDirection);
            field_EC_sound_timer = gnFrameCount_507670 + Math_RandomRange_450F20(24, 40);
        }


        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        PSX_RECT bRectBigger;
        bRectBigger.x = FP_GetExponent(field_A8_xpos - FP_FromInteger(4));
        bRectBigger.y = static_cast<s16>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(field_A8_xpos + FP_FromInteger(4));
        bRectBigger.h = static_cast<s16>(bRect.h + 5);

        for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
        {
            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            // Don't kill nades
            if (pObjIter->mBaseGameObjectTypeId != Types::eGrenade_40)
            {
                PSX_RECT objRect = {};
                pObjIter->VGetBoundingRect(&objRect, 1);

                if (!RectsOverlap(bRectBigger, objRect))
                {
                    // Not touching, so every so often check if we are near
                    if (!(gnFrameCount_507670 % 3))
                    {
                        // Make each side of the rect wider
                        objRect.x -= 50;
                        objRect.w += 50;

                        if (RectsOverlap(bRectBigger, objRect) && pObjIter->field_100_health > FP_FromInteger(0))
                        {
                            SFX_Play_Camera(SoundEffect::ElectricGateLoud_47, 45, soundDirection);
                        }
                    }
                }
                else
                {
                    // Touching the wall, rip
                    if (!pObjIter->field_10A_flags.Get(Flags_10A::e10A_Bit5_Electrocuted))
                    {
                        pObjIter->field_10A_flags.Set(Flags_10A::e10A_Bit5_Electrocuted);
                        ao_new<Electrocute>(pObjIter, 1);

                        pObjIter->VTakeDamage(this);

                        SFX_Play_Camera(SoundEffect::ElectricZap_46, 127, soundDirection);

                        ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
                    }
                }
            }
        }
    }
}

} // namespace AO
