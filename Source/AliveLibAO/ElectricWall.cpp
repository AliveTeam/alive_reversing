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
    mTypeId = Types::eElectricWall_25;

    const AnimRecord& rec = AO::AnimRec(AnimId::Electric_Wall);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mAnim.mRenderMode = TPageAbr::eBlend_1;
    mAnim.mRenderLayer = Layer::eLayer_Foreground_36;

    if (mAnim.Get_Frame_Count() > 0)
    {
        mAnim.SetFrame(Math_NextRandom() % mAnim.Get_Frame_Count());
    }

    mBlue = 80;
    mGreen = 80;
    mRed = 80;
    field_E4_tlv = tlvInfo;
    mXPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = 0;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = 1;
    }

    field_E8_switch_id = pTlv->field_1A_switch_id;
    field_EA_start_state = pTlv->field_1C_start_state;

    if (SwitchStates_Get(pTlv->field_1A_switch_id) == field_EA_start_state)
    {
        mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    }

    field_EC_sound_timer = 0;
}

ElectricWall::~ElectricWall()
{
    gMap.TLV_Reset(field_E4_tlv, -1, 0, 0);
}

void ElectricWall::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || gMap.GetDirection(mLvlNumber, mPathNumber, mXPos, mYPos) == CameraPos::eCamInvalid_m1)
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ElectricWall::VUpdate()
{
    const CameraPos soundDirection = gMap.GetDirection(
        mLvlNumber,
        mPathNumber,
        mXPos,
        mYPos);

    if (Event_Get(kEventDeathReset_4))
    {
        mGameObjectFlags.Set(Options::eDead);
    }

    if (SwitchStates_Get(field_E8_switch_id) == field_EA_start_state)
    {
        mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render);

        // Keep flipping direction
        if (!(gnFrameCount_507670 % 8))
        {
            mAnim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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
        bRectBigger.x = FP_GetExponent(mXPos - FP_FromInteger(4));
        bRectBigger.y = static_cast<s16>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(mXPos + FP_FromInteger(4));
        bRectBigger.h = static_cast<s16>(bRect.h + 5);

        for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
        {
            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            // Don't kill nades
            if (pObjIter->mTypeId != Types::eGrenade_40)
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

                        if (RectsOverlap(bRectBigger, objRect) && pObjIter->mHealth > FP_FromInteger(0))
                        {
                            SFX_Play_Camera(SoundEffect::ElectricGateLoud_47, 45, soundDirection);
                        }
                    }
                }
                else
                {
                    // Touching the wall, rip
                    if (!pObjIter->mAliveGameObjectFlags.Get(Flags_10A::e10A_Bit5_Electrocuted))
                    {
                        pObjIter->mAliveGameObjectFlags.Set(Flags_10A::e10A_Bit5_Electrocuted);
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
