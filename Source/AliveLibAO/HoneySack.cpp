#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneySack.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "BeeSwarm.hpp"
#include "HoneyDrip.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"

namespace AO {

HoneySack::HoneySack(Path_HoneySack* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eHoneySack;

    const AnimRecord hangingRec = AO::AnimRec(AnimId::HoneySack_Hanging);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, hangingRec.mResourceId, 1, 0);
    Animation_Init(AnimId::HoneySack_Hanging, ppRes);

    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    field_E4_tlvInfo = tlvInfo;

    field_100_chase_ticks = pTlv->field_18_chase_ticks;
    mAnim.mRenderLayer = Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31;

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);
    field_FC_ypos2 = FP_FromInteger(pTlv->mTopLeft.y);

    field_EA_bHit_ground = 0;

    if (pTlv->field_1A_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    if (pTlv->field_1_unknown)
    {
        mYPos += FP_FromInteger(pTlv->field_1_unknown);

        field_E8_state = State::eUpdateHoneySackOnGround_3;
        mAnim.Set_Animation_Data(AnimId::HoneySack_OnGround, nullptr);
        field_F0_pBee = nullptr;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

        field_E8_state = State::eDripHoney_0;
        field_EC_timer = sGnFrame + 90;

        if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_455270("WASP.BAN", nullptr);
        }

        field_F0_pBee = relive_new BeeSwarm(mXPos, mYPos, FP_FromInteger(0), 5, 0);
        if (field_F0_pBee)
        {
            field_F0_pBee->mBaseGameObjectRefCount++;
            field_F0_pBee->mSpriteScale = mSpriteScale;
        }

        field_F4_drip_target_x = FP_FromInteger(0);
        field_F8_drip_target_y = FP_FromInteger(0);

        Path_TLV* pHoneyDripTarget = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::HoneyDripTarget_42, 0);
        if (pHoneyDripTarget)
        {
            field_F4_drip_target_x = FP_FromInteger(pHoneyDripTarget->mTopLeft.x);
            field_F8_drip_target_y = FP_FromInteger(pHoneyDripTarget->mTopLeft.y);
        }
    }
}

HoneySack::~HoneySack()
{
    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);

    if (field_E8_state == State::eDripHoney_0)
    {
        Path::TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_E4_tlvInfo, FP_GetExponent(mYPos - field_FC_ypos2), 0, 0);
    }

    if (field_F0_pBee)
    {
        field_F0_pBee->mBaseGameObjectRefCount--;
        field_F0_pBee = nullptr;
    }
}

void HoneySack::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void HoneySack::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
    field_E8_state = State::eSetFallAnimation_1;
}

void HoneySack::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (field_F0_pBee)
    {
        if (field_F0_pBee->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            field_F0_pBee->mBaseGameObjectRefCount--;
            field_F0_pBee = nullptr;
        }
    }

    switch (field_E8_state)
    {
        case State::eDripHoney_0:
            if (static_cast<s32>(sGnFrame) > field_EC_timer)
            {
                relive_new HoneyDrip(field_F4_drip_target_x, field_F8_drip_target_y);

                field_EC_timer = sGnFrame + 90;
            }
            if (!gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            break;

        case State::eSetFallAnimation_1:
            if (static_cast<s32>(sGnFrame) > field_EC_timer - 68)
            {
                mAnim.Set_Animation_Data(AnimId::HoneySack_Falling, nullptr);
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

            if (sCollisions->Raycast(
                    mXPos,
                    oldY,
                    mXPos,
                    mYPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    mSpriteScale == FP_FromInteger(1) ? kFgFloor : kBgFloor))
            {
                SfxPlayMono(SoundEffect::MountingElum_38, 90, 0);
                Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 90, -1000, nullptr);
                mYPos = hitY;
                field_E8_state = State::eUpdateHoneySackOnGround_3;
                mAnim.Set_Animation_Data(AnimId::HoneySack_FallingToSmashed, nullptr);

                auto pNewBee = relive_new BeeSwarm(
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
                    field_F0_pBee->mBaseGameObjectRefCount--;
                    field_F0_pBee->mBaseGameObjectFlags.Set(Options::eDead);
                    field_F0_pBee = nullptr;
                }

                for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->mBaseGameObjectTypeId == ReliveTypes::eHoney)
                    {
                        pObj->mBaseGameObjectFlags.Set(Options::eDead);
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

                    if (pObj->mBaseGameObjectTypeId == ReliveTypes::eHoney)
                    {
                        pObj->mBaseGameObjectFlags.Set(Options::eDead);
                        field_EA_bHit_ground = 1;
                        break;
                    }
                }
            }

            if (!gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            break;

        default:
            return;
    }
}

} // namespace AO
