#include "stdafx_ao.h"
#include "Function.hpp"
#include "MeatSaw.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "Math.hpp"
#include "Blood.hpp"
#include "ScreenManager.hpp"

namespace AO {

MeatSaw::~MeatSaw()
{
    if (field_1A8_flags.Get(flags_1A8::eBit1_ResetOffscreen) && SwitchStates_Get(field_EE_switch_id) != field_F2_switch_value)
    {
        gMap.TLV_Reset(field_100_tlvInfo, 1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_100_tlvInfo, 0, 0, 0);
    }

    field_110_anim.VCleanUp();
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
}

MeatSaw::MeatSaw(Path_MeatSaw* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eMeatSaw;

    const AnimRecord rec = AO::AnimRec(AnimId::MeatSaw_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    
    field_10_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.mRenderMode = TPageAbr::eBlend_0;

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.mRenderLayer = Layer::eLayer_RopeWebMeatSaw_Half_5;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.mRenderLayer = Layer::eLayer_RopeWebMeatSaw_24;
        field_C6_scale = 1;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 8);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_F6_switch_min_time_off = pTlv->field_1A_switch_min_time_off;
    field_F8_switch_max_time_off = pTlv->field_1C_switch_max_time_off;

    field_E6_max_rise_time = pTlv->field_1E_max_rise_time;
    field_EE_switch_id = pTlv->field_20_switch_id;

    field_C8_yOffset = 0;
    field_F4 = 0;

    if (pTlv->field_22_type == Path_MeatSaw::Type::eAutomatic_1)
    {
        field_1A8_flags.Set(flags_1A8::eBit1_ResetOffscreen);
        field_1A8_flags.Clear(flags_1A8::eBit2_SwitchIdMeatSaw);
    }
    else if (pTlv->field_22_type == Path_MeatSaw::Type::eSwitchId_2)
    {
        field_1A8_flags.Set(flags_1A8::eBit1_ResetOffscreen);
        field_1A8_flags.Set(flags_1A8::eBit2_SwitchIdMeatSaw);
    }
    else // eAutomaticPersistOffscreen_0
    {
        field_1A8_flags.Clear(flags_1A8::eBit1_ResetOffscreen);
        field_1A8_flags.Clear(flags_1A8::eBit2_SwitchIdMeatSaw);
    }

    field_EA_speed1 = pTlv->field_24_speed;
    field_E8_speed2 = pTlv->field_24_speed;
    if (pTlv->field_26_start_state == Path_MeatSaw::StartState::eOff_0)
    {
        field_F0_switch_value = SwitchStates_Get(pTlv->field_20_switch_id) == 0;
    }
    else
    {
        if (pTlv->field_20_switch_id)
        {
            field_F0_switch_value = SwitchStates_Get(pTlv->field_20_switch_id);
        }
        else
        {
            field_F0_switch_value = 0;
        }
    }

    if (pTlv->field_20_switch_id)
    {
        field_F2_switch_value = SwitchStates_Get(pTlv->field_20_switch_id);
    }
    else
    {
        field_F2_switch_value = 0;
    }

    field_FA_automatic_min_time_off = pTlv->field_2A_automatic_min_time_off;

    field_EC_off_speed = pTlv->field_28_off_speed;
    field_FC_automatic_max_time_off = pTlv->field_2C_automatic_max_time_off;

    if (pTlv->field_1_unknown)
    {
        field_F0_switch_value = field_F0_switch_value == 0;
        field_F2_switch_value = field_F2_switch_value == 0;
    }

    field_104_idle_timer = 0;
    field_E4_state = MeatSawStates::eIdle_0;
    field_10C_FrameCount = 0;
    mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromInteger(pTlv->field_1E_max_rise_time);
    field_100_tlvInfo = tlvInfo;

    if (pTlv->field_2E_inital_position != 0)
    {
        field_E4_state = MeatSawStates::eGoingUp_2;
        field_F4 = pTlv->field_1E_max_rise_time + pTlv->field_24_speed - pTlv->field_1E_max_rise_time % pTlv->field_24_speed;
    }

    const AnimRecord& motorRec = AO::AnimRec(AnimId::MeatSawMotor);
    u8** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, motorRec.mResourceId, 1, 0);
    if (field_110_anim.Init(
            motorRec.mFrameTableOffset,
            gObjList_animations_505564,
            this,
            motorRec.mMaxW,
            motorRec.mMaxH,
            ppRes2,
            1,
            0,
            0))
    {
        field_110_anim.mRenderLayer = field_10_anim.mRenderLayer;
        field_110_anim.field_14_scale = field_BC_sprite_scale;

        field_110_anim.mRed = static_cast<u8>(field_C0_r);
        field_110_anim.mGreen = static_cast<u8>(field_C2_g);
        field_110_anim.mBlue = static_cast<u8>(field_C4_b);

        field_110_anim.mRenderMode = TPageAbr::eBlend_0;

        field_110_anim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
        field_110_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

        field_D0_pShadow = ao_new<Shadow>();
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0);
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

void MeatSaw::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || !sControlledCharacter_50767C || // Can be nullptr during the game ender
        FP_Abs(sControlledCharacter_50767C->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) > FP_FromInteger(1024))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void MeatSaw::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    GrindUpObjects_439CD0();

    const CameraPos direction = gMap.GetDirection_444A40(
        field_B2_lvl_number,
        field_B0_path_number,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos);

    if (!(field_10C_FrameCount % 87))
    {
        SFX_Play_Camera(SoundEffect::MeatsawOffscreen_88, 45, direction);
    }

    if (!(field_10C_FrameCount % 25))
    {
        SFX_Play_Camera(SoundEffect::MeatsawIdle_89, 45, direction);
    }

    field_10C_FrameCount++;

    switch (field_E4_state)
    {
        case MeatSawStates::eIdle_0:
            if ((field_104_idle_timer <= static_cast<s32>(sGnFrame) || (field_1A8_flags.Get(flags_1A8::eBit2_SwitchIdMeatSaw))) &&
                (!field_1A8_flags.Get(flags_1A8::eBit1_ResetOffscreen) || SwitchStates_Get(field_EE_switch_id) == field_F0_switch_value))
            {
                field_E4_state = MeatSawStates::eGoingDown_1;
                const AnimRecord& rec = AO::AnimRec(AnimId::MeatSaw_Moving);
                field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                field_1A8_flags.Clear(flags_1A8::eBit3_AutomaticMeatSawIsDown);
                field_E8_speed2 = field_EA_speed1;
                field_108_SFX_timer = sGnFrame + 2;
            }
            else
            {
                if (field_1A8_flags.Get(flags_1A8::eBit1_ResetOffscreen))
                {
                    if (!field_1A8_flags.Get(flags_1A8::eBit2_SwitchIdMeatSaw))
                    {
                        if (field_EC_off_speed)
                        {
                            if (field_104_idle_timer <= static_cast<s32>(sGnFrame))
                            {
                                field_E4_state = MeatSawStates::eGoingDown_1;
                                const AnimRecord& rec = AO::AnimRec(AnimId::MeatSaw_Moving);
                                field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                                field_1A8_flags.Set(flags_1A8::eBit3_AutomaticMeatSawIsDown);
                                field_E8_speed2 = field_EC_off_speed;
                                field_108_SFX_timer = sGnFrame + 2;
                            }
                        }
                    }
                }
            }
            break;

        case MeatSawStates::eGoingDown_1:
            field_F4 += field_E8_speed2;

            if (!((sGnFrame - field_108_SFX_timer) % 8))
            {
                SFX_Play_Camera(SoundEffect::MeatsawDown_91, 50, direction);
            }

            if (field_F4 >= field_E6_max_rise_time)
            {
                field_E4_state = MeatSawStates::eGoingUp_2;
                field_108_SFX_timer = sGnFrame + 2;
            }
            break;

        case MeatSawStates::eGoingUp_2:
            if (!((sGnFrame - field_108_SFX_timer) % 10))
            {
                field_108_SFX_timer = sGnFrame;
                SFX_Play_Camera(SoundEffect::MeatsawUp_90, 50, direction);
            }

            field_F4 -= field_E8_speed2;

            if (field_F4 <= 0)
            {
                field_E4_state = MeatSawStates::eIdle_0;
                s16 minRnd = 0;
                s16 maxRnd = 0;
                if (field_1A8_flags.Get(flags_1A8::eBit3_AutomaticMeatSawIsDown))
                {
                    maxRnd = field_FC_automatic_max_time_off;
                    minRnd = field_FA_automatic_min_time_off;
                }
                else
                {
                    maxRnd = field_F8_switch_max_time_off;
                    minRnd = field_F6_switch_min_time_off;
                }

                field_104_idle_timer = sGnFrame + Math_RandomRange_450F20(minRnd, maxRnd);
                field_10_anim.Set_Animation_Data(15200, 0);
                if (field_1A8_flags.Get(flags_1A8::eBit2_SwitchIdMeatSaw))
                {
                    SwitchStates_Set(field_EE_switch_id, field_F0_switch_value == 0 ? 1 : 0);
                }
            }
            break;
    }
}

void MeatSaw::GrindUpObjects_439CD0()
{
    PSX_RECT ourRect = {};
    VGetBoundingRect(&ourRect, 1);

    ourRect.y += field_C8_yOffset;
    ourRect.h += field_C8_yOffset;

    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
            {
                // Can't grind meat with a meat saw, that would be grindception
                if (pObjIter->mBaseGameObjectTypeId != ReliveTypes::eMeat)
                {
                    PSX_RECT objRect = {};
                    pObjIter->VGetBoundingRect(&objRect, 1);

                    if (RectsOverlap(ourRect, objRect) && pObjIter->field_BC_sprite_scale == field_BC_sprite_scale && pObjIter->mHealth > FP_FromInteger(0))
                    {
                        if (pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger(ourRect.x) && pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos <= FP_FromInteger(ourRect.w))
                        {
                            if (!pObjIter->VTakeDamage(this))
                            {
                                return;
                            }

                            ao_new<Blood>(
                                pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos,
                                FP_FromInteger(ourRect.h - 10),
                                FP_FromInteger(-5),
                                FP_FromInteger(5),
                                field_BC_sprite_scale,
                                50);

                            ao_new<Blood>(
                                pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos,
                                FP_FromInteger(ourRect.h - 10),
                                FP_FromInteger(0),
                                FP_FromInteger(5),
                                field_BC_sprite_scale,
                                50);

                            ao_new<Blood>(
                                pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos,
                                FP_FromInteger(ourRect.h - 10),
                                FP_FromInteger(5),
                                FP_FromInteger(5),
                                field_BC_sprite_scale,
                                50);

                            SFX_Play_Mono(SoundEffect::KillEffect_78, 127, 0);
                            SFX_Play_Pitch(SoundEffect::KillEffect_78, 127, -700, 0);
                            return;
                        }
                    }
                }
            }
        }
    }
}

void MeatSaw::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        field_C8_yOffset = field_F4;
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

        field_110_anim.VRender(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos
                           + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)
                           - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos
                           + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + field_E6_max_rise_time))
                           - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        field_110_anim.Get_Frame_Rect(&rect);
        pScreenManager_4FF7C8->InvalidateRect(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager_4FF7C8->field_2E_idx);
    }
}

} // namespace AO
