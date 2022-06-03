#include "stdafx_ao.h"
#include "Function.hpp"
#include "ZBall.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

ALIVE_VAR(1, 0x9F1DCC, ZBall*, gCenter_ZBall_9F1DCC, nullptr);
ALIVE_VAR(1, 0x9F1DD0, ZBall*, gOutZBall_9F1DD0, nullptr);

s16* Animation_OnFrame_ZBallSmacker(void* pObj, s16* pData)
{
    auto pZBall = static_cast<ZBall*>(pObj);
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pBase = gBaseGameObjects->ItemAt(i);
        if (!pBase)
        {
            break;
        }

        if (pBase->mGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // If the object is within the ZBall rect then smack it
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pBase);

            PSX_RECT bRect = {};
            pAliveObj->VGetBoundingRect(&bRect, 1);

            if (bRect.x <= (FP_GetExponent(pZBall->mXPos) + pData[2]) && bRect.w >= (FP_GetExponent(pZBall->mXPos) + pData[0]) && bRect.h >= (FP_GetExponent(pZBall->mYPos) + pData[1]) && bRect.y <= (FP_GetExponent(pZBall->mYPos) + pData[3]))
            {
                pAliveObj->VTakeDamage(pZBall);
            }
        }
    }

    return pData + 4; // A rect I guess ?
}

ZBall::ZBall(Path_ZBall* pTlv, s32 tlvInfo)
{
    mTypeId = Types::eZBall_92;

    mBlue = 128;
    mGreen = 128;
    mRed = 128;

    switch (pTlv->field_1C_speed)
    {
        case Path_ZBall::Speed::eNormal_0:
        {
            const AnimRecord& rec1 = AO::AnimRec(AnimId::Swinging_Ball_Normal);
            u8** ppRes1 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec1.mResourceId, 1, 0);
            Animation_Init_417FD0(rec1.mFrameTableOffset, rec1.mMaxW, rec1.mMaxH, ppRes1, 1);
            break;
        }

        case Path_ZBall::Speed::eFast_1:
        {
            const AnimRecord& rec2 = AO::AnimRec(AnimId::Swinging_Ball_Fast);
            u8** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
            Animation_Init_417FD0(rec2.mFrameTableOffset, rec2.mMaxW, rec2.mMaxH, ppRes2, 1);
            break;
        }

        case Path_ZBall::Speed::eSlow_2:
        {
            const AnimRecord& rec3 = AO::AnimRec(AnimId::Swinging_Ball_Slow);
            u8** ppRes3 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec3.mResourceId, 1, 0);
            Animation_Init_417FD0(rec3.mFrameTableOffset, rec3.mMaxW, rec3.mMaxH, ppRes3, 1);
            break;
        }

    }

    mXPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    if (gMap.mCurrentLevel == LevelIds::eForestTemple_4)
    {
        switch (pTlv->field_18_start_pos)
        {
            case Path_ZBall::StartPos::eCenter_0:
                mAnim.SetFrame(6u);
                gCenter_ZBall_9F1DCC = this;
                field_EA_sound_pitch = -800;
                break;

            case Path_ZBall::StartPos::eOut_1:
                mAnim.SetFrame(0);
                gOutZBall_9F1DD0 = this;
                field_EA_sound_pitch = -400;
                break;

            case Path_ZBall::StartPos::eIn_2:
                mAnim.SetFrame(13u);
                field_EA_sound_pitch = 0;
                break;
        }

        mAnim.VDecode();
    }

    if (pTlv->field_1A_scale != Scale_short::eFull_0)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = 0;
    }

    field_E4_tlvInfo = tlvInfo;
    mAnim.field_1C_fn_ptr_array = kZBall_Anim_Frame_Fns_4CEBF8;
}

void ZBall::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mGameObjectFlags.Set(Options::eDead);
    }

    if (gCenter_ZBall_9F1DCC == this || gOutZBall_9F1DD0 == this)
    {
        if (mAnim.field_92_current_frame == 0 || mAnim.field_92_current_frame == 13)
        {
            SFX_Play_Pitch(SoundEffect::ZBall_62, 50, field_EA_sound_pitch, nullptr);
        }
    }

    if (gCenter_ZBall_9F1DCC == this)
    {
        if (mAnim.field_92_current_frame == 3 || mAnim.field_92_current_frame == 16)
        {
            SFX_Play_Pitch(SoundEffect::SackWobble_34, 40, field_EA_sound_pitch - 2400, nullptr);
        }
    }

    if (mAnim.field_92_current_frame <= 6 || mAnim.field_92_current_frame >= 19)
    {
        if (mSpriteScale == FP_FromInteger(1))
        {
            mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }
    }
    else if (mSpriteScale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeWell_22;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeWell_Half_3;
    }

    // Pointless because never seems to be read
    field_E8_bFrameAbove12 = mAnim.field_92_current_frame >= 13;

    if (!gMap.Is_Point_In_Current_Camera_4449C0(
            mLvlNumber,
            mPathNumber,
            mXPos,
            mYPos,
            0))
    {
        mGameObjectFlags.Set(Options::eDead);
        if (field_E4_tlvInfo != -1)
        {
            gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
        }
    }
}

} // namespace AO
