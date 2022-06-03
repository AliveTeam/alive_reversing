#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneySack.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "BeeSwarm.hpp"
#include "HoneyDrip.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"

namespace AO {

HoneySack::HoneySack(Path_HoneySack* pTlv, s32 tlvInfo)
{
    mTypeId = Types::eHoneySack_45;

    const AnimRecord& hangingRec = AO::AnimRec(AnimId::HoneySack_Hanging);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, hangingRec.mResourceId, 1, 0);
    Animation_Init_417FD0(hangingRec.mFrameTableOffset, hangingRec.mMaxW, hangingRec.mMaxH, ppRes, 1);

    mGameObjectFlags.Set(Options::eCanExplode_Bit7);
    field_E4_tlvInfo = tlvInfo;

    field_100_chase_ticks = pTlv->field_18_chase_ticks;
    mAnim.mRenderLayer = Layer::eLayer_DoorFlameRollingBallPortalClip_Half_31;

    mXPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_FC_ypos2 = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_EA_bHit_ground = 0;

    if (pTlv->field_1A_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = 0;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = 1;
    }

    if (pTlv->field_1_unknown)
    {
        mYPos += FP_FromInteger(pTlv->field_1_unknown);

        field_E8_state = State::eUpdateHoneySackOnGround_3;
        const AnimRecord& groundRec = AO::AnimRec(AnimId::HoneySack_OnGround);
        mAnim.Set_Animation_Data(groundRec.mFrameTableOffset, 0);
        field_F0_pBee = nullptr;
    }
    else
    {
        mGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

        field_E8_state = State::eDripHoney_0;
        field_EC_timer = gnFrameCount_507670 + 90;

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_455270("WASP.BAN", nullptr);
        }

        field_F0_pBee = ao_new<BeeSwarm>(mXPos, mYPos, FP_FromInteger(0), 5, 0);
        if (field_F0_pBee)
        {
            field_F0_pBee->field_C_refCount++;
            field_F0_pBee->mSpriteScale = mSpriteScale;
        }

        field_F4_drip_target_x = FP_FromInteger(0);
        field_F8_drip_target_y = FP_FromInteger(0);

        Path_TLV* pHoneyDripTarget = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::HoneyDripTarget_42, 0);
        if (pHoneyDripTarget)
        {
            field_F4_drip_target_x = FP_FromInteger(pHoneyDripTarget->field_10_top_left.field_0_x);
            field_F8_drip_target_y = FP_FromInteger(pHoneyDripTarget->field_10_top_left.field_2_y);
        }
    }
}

HoneySack::~HoneySack()
{
    mGameObjectFlags.Clear(Options::eCanExplode_Bit7);

    if (field_E8_state == State::eDripHoney_0)
    {
        gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_E4_tlvInfo, FP_GetExponent(mYPos - field_FC_ypos2), 0, 0);
    }

    if (field_F0_pBee)
    {
        field_F0_pBee->field_C_refCount--;
        field_F0_pBee = nullptr;
    }
}

void HoneySack::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void HoneySack::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    mGameObjectFlags.Clear(Options::eCanExplode_Bit7);
    field_E8_state = State::eSetFallAnimation_1;
}

void HoneySack::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mGameObjectFlags.Set(Options::eDead);
    }

    if (field_F0_pBee)
    {
        if (field_F0_pBee->mGameObjectFlags.Get(BaseGameObject::eDead))
        {
            field_F0_pBee->field_C_refCount--;
            field_F0_pBee = nullptr;
        }
    }

    switch (field_E8_state)
    {
        case State::eDripHoney_0:
            if (static_cast<s32>(gnFrameCount_507670) > field_EC_timer)
            {
                ao_new<HoneyDrip>(field_F4_drip_target_x, field_F8_drip_target_y);

                field_EC_timer = gnFrameCount_507670 + 90;
            }
            if (!gMap.Is_Point_In_Current_Camera_4449C0(
                    mLvlNumber,
                    mPathNumber,
                    mXPos,
                    mYPos,
                    0))
            {
                mGameObjectFlags.Set(Options::eDead);
            }
            break;

        case State::eSetFallAnimation_1:
            if (static_cast<s32>(gnFrameCount_507670) > field_EC_timer - 68)
            {
                const AnimRecord& rec = AO::AnimRec(AnimId::HoneySack_Falling);
                mAnim.Set_Animation_Data(rec.mFrameTableOffset, 0);
                field_E8_state = State::eFallOnGround_2;
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);
            }
            break;

        case State::eFallOnGround_2:
        {
            if (mVelY < FP_FromInteger(18))
            {
                mVelY += FP_FromInteger(1);
            }

            const FP oldY = mYPos;
            mYPos += mVelY;

            if (field_F0_pBee)
            {
                field_F0_pBee->field_D70_chase_target_x = mXPos;
                field_F0_pBee->field_D74_chase_target_y = mYPos;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            if (sCollisions_DArray_504C6C->RayCast(
                    mXPos,
                    oldY,
                    mXPos,
                    mYPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    mSpriteScale == FP_FromInteger(1) ? 0x01 : 0x10))
            {
                SFX_Play_Mono(SoundEffect::MountingElum_38, 90, 0);
                Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 90, -1000, nullptr);
                mYPos = hitY;
                field_E8_state = State::eUpdateHoneySackOnGround_3;
                const AnimRecord& rec = AO::AnimRec(AnimId::HoneySack_FallingToSmashed);
                mAnim.Set_Animation_Data(rec.mFrameTableOffset, 0);

                auto pNewBee = ao_new<BeeSwarm>(
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    24,
                    field_100_chase_ticks);
                if (pNewBee)
                {
                    pNewBee->Chase(sActiveHero);
                }

                if (field_F0_pBee)
                {
                    field_F0_pBee->field_C_refCount--;
                    field_F0_pBee->mGameObjectFlags.Set(Options::eDead);
                    field_F0_pBee = nullptr;
                }

                for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->mTypeId == Types::eHoney_47)
                    {
                        pObj->mGameObjectFlags.Set(Options::eDead);
                        field_EA_bHit_ground = 1;
                        return;
                    }
                }
            }
        }
        break;

        case State::eUpdateHoneySackOnGround_3:
            if (!field_EA_bHit_ground)
            {
                for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->mTypeId == Types::eHoney_47)
                    {
                        pObj->mGameObjectFlags.Set(Options::eDead);
                        field_EA_bHit_ground = 1;
                        break;
                    }
                }
            }

            if (!gMap.Is_Point_In_Current_Camera_4449C0(
                    mLvlNumber,
                    mPathNumber,
                    mXPos,
                    mYPos,
                    0))
            {
                mGameObjectFlags.Set(Options::eDead);
            }
            break;

        default:
            return;
    }
}

} // namespace AO
