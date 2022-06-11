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
    mBaseGameObjectTypeId = ReliveTypes::eElectricWall;

    const AnimRecord& rec = AO::AnimRec(AnimId::Electric_Wall);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Count() > 0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(Math_NextRandom() % mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Count());
    }

    mBaseAnimatedWithPhysicsGameObject_Blue = 80;
    mBaseAnimatedWithPhysicsGameObject_Green = 80;
    mBaseAnimatedWithPhysicsGameObject_Red = 80;
    field_E4_tlv = tlvInfo;
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    field_E8_switch_id = pTlv->field_1A_switch_id;
    field_EA_start_state = pTlv->field_1C_start_state;

    if (SwitchStates_Get(pTlv->field_1A_switch_id) == field_EA_start_state)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    }

    field_EC_sound_timer = 0;
}

ElectricWall::~ElectricWall()
{
    gMap.TLV_Reset(field_E4_tlv, -1, 0, 0);
}

void ElectricWall::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos) == CameraPos::eCamInvalid_m1)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ElectricWall::VUpdate()
{
    const CameraPos soundDirection = gMap.GetDirection(
        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
        mBaseAnimatedWithPhysicsGameObject_PathNumber,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos);

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (SwitchStates_Get(field_E8_switch_id) == field_EA_start_state)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

        // Keep flipping direction
        if (!(sGnFrame % 8))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        }

        // Play sound every so often
        if (static_cast<s32>(sGnFrame) >= field_EC_sound_timer)
        {
            // set a random starting frame
            SFX_Play_Camera(SoundEffect::BirdPortalSpark_48, 45, soundDirection);
            field_EC_sound_timer = sGnFrame + Math_RandomRange_450F20(24, 40);
        }


        const PSX_RECT bRect = VGetBoundingRect();

        PSX_RECT bRectBigger = {};
        bRectBigger.x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(4));
        bRectBigger.y = static_cast<s16>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(4));
        bRectBigger.h = static_cast<s16>(bRect.h + 5);

        for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
        {
            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            // Don't kill nades
            if (pObjIter->mBaseGameObjectTypeId != ReliveTypes::eGrenade)
            {
                PSX_RECT objRect = pObjIter->VGetBoundingRect();

                if (!RectsOverlap(bRectBigger, objRect))
                {
                    // Not touching, so every so often check if we are near
                    if (!(sGnFrame % 3))
                    {
                        // Make each side of the rect wider
                        objRect.x -= 50;
                        objRect.w += 50;

                        if (RectsOverlap(bRectBigger, objRect) && pObjIter->mHealth > FP_FromInteger(0))
                        {
                            SFX_Play_Camera(SoundEffect::ElectricGateLoud_47, 45, soundDirection);
                        }
                    }
                }
                else
                {
                    // Touching the wall, rip
                    if (!pObjIter->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit5_Electrocuted))
                    {
                        pObjIter->mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit5_Electrocuted);
                        relive_new Electrocute(pObjIter, 1);

                        pObjIter->VTakeDamage(this);

                        SFX_Play_Camera(SoundEffect::ElectricZap_46, 127, soundDirection);

                        relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
                    }
                }
            }
        }
    }
}

} // namespace AO
