#include "stdafx.h"
#include "LiftPoint.hpp"
#include "Function.hpp"
#include "Rope.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Abe.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "PathData.hpp"
#include "FixedPoint.hpp"

struct LiftPointData final
{
    AnimId field_0_platform_anim_id;
    s32 field_4_maxW_platform;
    s32 field_8_maxH_platform;
    AnimId field_C_lift_bottom_wheel_anim_id;
    AnimId field_10_lift_top_wheel_anim_id;
    s32 field_14_maxW_lift_wheels;
    s32 field_18_maxH_lift_wheels;
};
ALIVE_ASSERT_SIZEOF(LiftPointData, 0x1C);

const LiftPointData sLiftPointData_545AC8[18] = {
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24}};

const TintEntry sLiftTints_55BF50[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 107u, 107u, 107u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

void LiftPoint::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftPlatform_Mines));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftBottomWheel_Mines));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftTopWheel_Mines));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftPlatform_Necrum));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftBottomWheel_Necrum));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftTopWheel_Necrum));
}

LiftPoint::LiftPoint(relive::Path_LiftPoint* pTlv, const Guid& tlvId)
{
    mBaseGameObjectTlvInfo = tlvId;
    SetType(ReliveTypes::eLiftPoint);

    LoadAnimations();

    pTlv->mTlvSpecificMeaning = 3;

    field_27C_pTlv = sPathInfo->TLVInfo_From_TLVPtr(pTlv);

   // u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kLiftResID);
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    const LiftPointData& rPlatformData = sLiftPointData_545AC8[static_cast<u32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    AddDynamicCollision(
        rPlatformData.field_0_platform_anim_id,
        pTlv,
        tlvId);

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        mPlatformBaseCollisionLine->mLineType = eLineTypes::eBackgroundDynamicCollision_36;
    }

    SetTint(sLiftTints_55BF50, gMap.mCurrentLevel);

    const FP oldX = mXPos;
    MapFollowMe(true);
    const s16 xSnapDelta = FP_GetExponent(mXPos - oldX);
    mPlatformBaseXOffset -= xSnapDelta;
    mPlatformBaseWidthOffset -= xSnapDelta;

    GetAnimation().mFlags.Set(AnimFlags::eSemiTrans);


    const LiftPointData& rLiftWheelData = sLiftPointData_545AC8[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    if (field_13C_lift_wheel.Init(
            GetAnimRes(rLiftWheelData.field_C_lift_bottom_wheel_anim_id),
            this))
    {
        if (pTlv->mScale == relive::reliveScale::eHalf)
        {
            field_13C_lift_wheel.SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
            field_13C_lift_wheel.SetSpriteScale(GetSpriteScale());
        }
        else
        {
            field_13C_lift_wheel.SetRenderLayer(Layer::eLayer_BeforeShadow_25);
            field_13C_lift_wheel.SetSpriteScale(GetSpriteScale());
        }

        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eAnimate);
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eSemiTrans);
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eBlending);

        mMoving = false;
        mTopFloor = false;
        mMiddleFloor = false;
        mBottomFloor = false;
        mMoveToFloorLevel = false;

        // TODO: red is set to blue and vice versa - fix me
        //field_13C_lift_wheel.mGreen = static_cast<u8>(mRGB.g);
        //field_13C_lift_wheel.mRed = static_cast<u8>(mRGB.b);
        //field_13C_lift_wheel.mBlue = static_cast<u8>(mRGB.r);

        field_13C_lift_wheel.SetRGB(mRGB.r, mRGB.g, mRGB.b);
        field_13C_lift_wheel.SetRenderMode(TPageAbr::eBlend_0);

        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);

        const FP k25 = FP_FromInteger(25);
        const FP k13 = FP_FromInteger(13);
        const FP km10 = FP_FromInteger(-10);


        auto pRope1 = relive_new Rope(FP_GetExponent((k13 * GetSpriteScale() + mXPos)),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * GetSpriteScale()) + mYPos),
                                   GetSpriteScale());
        if (pRope1)
        {
            field_138_rope1_id = pRope1->mBaseGameObjectId;
        }

        auto pRope2 = relive_new Rope(FP_GetExponent((km10 * GetSpriteScale()) + mXPos),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * GetSpriteScale()) + mYPos),
                                   GetSpriteScale());
        if (pRope2)
        {
            field_134_rope2_id = pRope2->mBaseGameObjectId;
        }

        pRope2->field_106_bottom = FP_GetExponent((k25 * GetSpriteScale()) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y));
        pRope1->field_106_bottom = FP_GetExponent((k25 * GetSpriteScale()) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y));

        const FP v28 = mYPos * FP_FromDouble(1.5);
        const FP v29 = FP_FromRaw(FP_GetExponent(v28 * GetSpriteScale()) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);

        pRope2->mYPos = FP_FromInteger(FP_GetExponent(v29 + (k25 * GetSpriteScale()) + mYPos + FP_FromInteger(pRope2->field_F6_rope_length)));
        pRope1->mYPos = FP_FromInteger(FP_GetExponent((k25 * GetSpriteScale()) + mYPos + FP_FromInteger(pRope1->field_F6_rope_length) - v29));

        mHasPulley = false;
        CreatePulleyIfExists();

        field_278_lift_point_id = static_cast<s8>(pTlv->mLiftPointId);
        field_130_lift_point_stop_type = pTlv->mLiftPointStopType;

        switch (field_130_lift_point_stop_type)
        {
            case relive::Path_LiftPoint::LiftPointStopType::eTopFloor:
                mTopFloor = true;
                break;

            case relive::Path_LiftPoint::LiftPointStopType::eBottomFloor:
                mBottomFloor = true;
                break;

            case relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor:
                mMiddleFloor = true;
                break;

            default:
            case relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor: // Not used ??
            case relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly:
                field_130_lift_point_stop_type = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
                break;
        }

        mKeepOnMiddleFloor = false;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

s32 LiftPoint::CreateFromSaveState(const u8* pData)
{
    const LiftPointSaveState* pState = reinterpret_cast<const LiftPointSaveState*>(pData);

    relive::Path_LiftPoint* pTlv = static_cast<relive::Path_LiftPoint*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_C_tlvInfo));

    auto pLiftPoint = relive_new LiftPoint(pTlv, pState->field_C_tlvInfo);
    if (pLiftPoint)
    {
        pLiftPoint->mXPos = pState->field_4_xpos;
        pLiftPoint->mYPos = pState->field_8_ypos;
        pLiftPoint->SyncCollisionLinePosition();

        Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->field_134_rope2_id, ReliveTypes::eRope));
        Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->field_138_rope1_id, ReliveTypes::eRope));

        pRope2->field_106_bottom = FP_GetExponent(FP_FromInteger(pLiftPoint->mPlatformBaseCollisionLine->mRect.y) + (FP_FromInteger(25) * pLiftPoint->GetSpriteScale()));
        pRope1->field_106_bottom = FP_GetExponent(FP_FromInteger(pLiftPoint->mPlatformBaseCollisionLine->mRect.y) + (FP_FromInteger(25) * pLiftPoint->GetSpriteScale()));

        if (pLiftPoint->mHasPulley)
        {
            pRope2->field_102_top = FP_GetExponent(FP_FromInteger(pLiftPoint->field_26E_pulley_ypos) + FP_FromInteger(-19) * pLiftPoint->GetSpriteScale());
            pRope1->field_102_top = FP_GetExponent(FP_FromInteger(pLiftPoint->field_26E_pulley_ypos) + FP_FromInteger(-19) * pLiftPoint->GetSpriteScale());
        }

        pLiftPoint->mPlatformBaseTlvInfo = pState->field_C_tlvInfo;
        pLiftPoint->field_27C_pTlv = pState->field_10_pTlv;
        pLiftPoint->field_270_floorYLevel = pState->field_14_floorYLevel;
        pLiftPoint->field_130_lift_point_stop_type = pState->field_18_lift_point_stop_type;

        pLiftPoint->mMoving = pState->mMoving;
        pLiftPoint->mTopFloor = pState->mTopFloor;
        pLiftPoint->mMiddleFloor = pState->mMiddleFloor;
        pLiftPoint->mBottomFloor = pState->mBottomFloor;
        pLiftPoint->mMoveToFloorLevel = pState->mMoveToFloorLevel;
        pLiftPoint->mKeepOnMiddleFloor = pState->mKeepOnMiddleFloor;
    }

    if (pState->field_10_pTlv == pState->field_C_tlvInfo)
    {
        return sizeof(LiftPointSaveState);
    }

    pTlv->mTlvSpecificMeaning = 1;
    if (pState->field_10_pTlv == Guid{})
    {
        return sizeof(LiftPointSaveState);
    }

    relive::Path_TLV* pTlv2 = sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_10_pTlv);
    pTlv2->mTlvSpecificMeaning = 3;
    return sizeof(LiftPointSaveState);
}

void LiftPoint::vKeepOnMiddleFloor()
{
    mKeepOnMiddleFloor = true;
}

bool LiftPoint::vOnTopFloor()
{
    return mTopFloor && !mMoveToFloorLevel;
}

bool LiftPoint::vOnMiddleFloor()
{
    return mMiddleFloor && !mMoveToFloorLevel;
}

bool LiftPoint::vOnBottomFloor()
{
    return mBottomFloor && !mMoveToFloorLevel;
}

bool LiftPoint::vOnAnyFloor()
{
    return vOnBottomFloor() || vOnTopFloor() || vOnMiddleFloor();
}

bool LiftPoint::vOnAFloorLiftMoverCanUse()
{
    // Top or bottom floor can still be activated by the lift mover?
    return (vOnMiddleFloor() && !mIgnoreLiftMover) || vOnBottomFloor() || vOnTopFloor();
}

bool LiftPoint::vMovingToFloorLevel()
{
    return mMoveToFloorLevel;
}

void LiftPoint::vMove_4626A0(FP xSpeed, FP ySpeed, s32 /*not_used*/)
{
    mVelX = xSpeed * GetSpriteScale();
    mVelY = ySpeed * GetSpriteScale();

    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        mMoving = true;
    }
}

void LiftPoint::VUpdate()
{
    if (mMoving)
    {
        if (mMoveToFloorLevel)
        {
            const bool bOnFloor = field_270_floorYLevel == mYPos;
            const FP distToFloor = field_270_floorYLevel - mYPos;

            FP absDistToFloor = {};
            if (distToFloor < FP_FromInteger(0) || bOnFloor)
            {
                absDistToFloor = -distToFloor;
            }
            else
            {
                absDistToFloor = distToFloor;
            }

            if (absDistToFloor >= FP_FromInteger(2) * GetSpriteScale())
            {
                if (distToFloor >= FP_FromInteger(0))
                {
                    mVelY = FP_FromInteger(2) * GetSpriteScale();
                }
                else
                {
                    mVelY = -(FP_FromInteger(2) * GetSpriteScale());
                }
                mMoving |= 1u;
            }
            else
            {
                mYPos = field_270_floorYLevel;
                mVelY = FP_FromInteger(0);
                mMoving = false;
                mMoveToFloorLevel = false;
                SfxPlayMono(relive::SoundEffects::LiftStop, 0);
                SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -2000);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            field_130_lift_point_stop_type = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
            const FP lineY = FP_FromInteger(mPlatformBaseCollisionLine->mRect.y);
            relive::Path_TLV* pTlvIter = sPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                lineY,
                mXPos,
                (GetSpriteScale() * FP_FromInteger(30)) + lineY);

            if (pTlvIter)
            {
                while (pTlvIter->mTlvType != ReliveTypes::eLiftPoint)
                {
                    pTlvIter = sPathInfo->TlvGetAt(
                        pTlvIter,
                        mXPos,
                        lineY,
                        mXPos,
                        lineY + (GetSpriteScale() * FP_FromInteger(30)));

                    if (!pTlvIter)
                    {
                        break;
                    }
                }
            }

            // TODO: Bugged if lift point not found - last TLV will get casted which could be anything
            auto pLiftTlv = static_cast<relive::Path_LiftPoint*>(pTlvIter);
            if (pLiftTlv)
            {
                field_130_lift_point_stop_type = pLiftTlv->mLiftPointStopType;

                mIgnoreLiftMover = false;
                if (pLiftTlv->mIgnoreLiftMover == relive::reliveChoice::eYes)
                {
                    mIgnoreLiftMover = true;
                }
            }

            // TODO: Also bugged because will always be true
            if (pLiftTlv)
            {
                sub_461000(pLiftTlv);
                field_270_floorYLevel = FP_FromInteger(pLiftTlv->mTopLeftY + -mPlatformBaseYOffset);
            }
            else
            {
                field_270_floorYLevel = FP_FromInteger(0);
                field_130_lift_point_stop_type = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
            }

            const FP distanceToFloor = field_270_floorYLevel - mYPos;
            const FP kMinus25Scaled = GetSpriteScale() * FP_FromInteger(-25);
            const FP k30Scaled = GetSpriteScale() * FP_FromInteger(30);

            switch (field_130_lift_point_stop_type)
            {
                case relive::Path_LiftPoint::LiftPointStopType::eTopFloor:
                    if (mVelY >= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;
                            mTopFloor = false;
                            field_27C_pTlv = Guid{};
                        }
                        else
                        {
                            mMoveToFloorLevel = true;
                            mMoving = true;
                            mYPos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            field_27C_pTlv = sPathInfo->TLVInfo_From_TLVPtr(pLiftTlv);
                            pLiftTlv->mLiftPointId = field_278_lift_point_id;
                            mTopFloor = true;
                        }
                    }
                    else if (mVelY + lineY <= FP_FromInteger(pLiftTlv->mTopLeftY))
                    {
                        vStayOnFloor(mTopFloor, pLiftTlv);
                        mTopFloor = true;
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eBottomFloor:
                    if (mVelY <= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;

                            mBottomFloor = false;
                            field_27C_pTlv = Guid{};
                        }
                        else
                        {
                            mMoving = true;
                            mMoveToFloorLevel = true;
                            mYPos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            field_27C_pTlv = sPathInfo->TLVInfo_From_TLVPtr(pLiftTlv);
                            pLiftTlv->mLiftPointId = field_278_lift_point_id;
                            mBottomFloor = true;
                        }
                    }
                    else if (lineY + mVelY >= FP_FromInteger(pLiftTlv->mTopLeftY))
                    {
                        vStayOnFloor(mBottomFloor, pLiftTlv);
                        mBottomFloor = true;
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor:
                    if (distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                    {
                        pLiftTlv->mTlvSpecificMeaning = 1;
                        field_27C_pTlv = Guid{};
                    }
                    else
                    {
                        if (mKeepOnMiddleFloor)
                        {
                            vStayOnFloor(mMiddleFloor, pLiftTlv);
                            mKeepOnMiddleFloor = false;
                        }

                        if (mVelY == FP_FromInteger(0))
                        {
                            mMoving = true;
                            mMoveToFloorLevel = true;
                            mYPos = field_270_floorYLevel - distanceToFloor;
                        }

                        pLiftTlv->mTlvSpecificMeaning = 3;
                        field_27C_pTlv = sPathInfo->TLVInfo_From_TLVPtr(pLiftTlv);
                        pLiftTlv->mLiftPointId = field_278_lift_point_id;
                        mMiddleFloor = true;
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly:
                    if (pLiftTlv)
                    {
                        pLiftTlv->mTlvSpecificMeaning = 1;
                        field_27C_pTlv = Guid{};
                    }
                    mTopFloor = false;
                    mMiddleFloor = false;
                    mBottomFloor = false;
                    break;
                default:
                    break;
            }
        }

        mXPos += mVelX;
        mYPos += mVelY;

        if (mPlatformBaseCollisionLine)
        {
            SyncCollisionLinePosition();
        }

        MoveObjectsOnLift(mVelX);
    }

    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(field_134_rope2_id, ReliveTypes::eRope));
    pRope2->field_106_bottom = FP_GetExponent((FP_FromInteger(mPlatformBaseCollisionLine->mRect.y) + (FP_FromInteger(25) * GetSpriteScale())));

    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(field_138_rope1_id, ReliveTypes::eRope));
    pRope1->field_106_bottom = FP_GetExponent((FP_FromInteger(mPlatformBaseCollisionLine->mRect.y) + (FP_FromInteger(25) * GetSpriteScale())));

    if (mHasPulley)
    {
        pRope2->field_102_top = FP_GetExponent((FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * GetSpriteScale())));
        pRope1->field_102_top = FP_GetExponent((FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * GetSpriteScale())));
    }

    const FP v39 = mYPos * FP_FromDouble(1.5);
    const FP v40 = FP_FromRaw(FP_GetExponent(v39 * GetSpriteScale()) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);
    pRope2->mYPos = FP_NoFractional(v40 + (FP_FromInteger(25) * GetSpriteScale()) + mYPos + FP_FromInteger(pRope2->field_F6_rope_length));
    pRope1->mYPos = FP_NoFractional((FP_FromInteger(25) * GetSpriteScale()) + mYPos + FP_FromInteger(pRope1->field_F6_rope_length) - v40);

    field_13C_lift_wheel.mFlags.Set(AnimFlags::eAnimate);
    field_1D4_pulley_anim.mFlags.Set(AnimFlags::eAnimate);

    if (mVelY == FP_FromInteger(0))
    {
        // Wheels are stopped if not moving
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eAnimate);
        field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eAnimate);
    }
    else if (mVelY > FP_FromInteger(0))
    {
        // Pulley/lift wheels spin opposite ways for up/down
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eLoopBackwards);
        field_1D4_pulley_anim.mFlags.Set(AnimFlags::eLoopBackwards);
    }
    else
    {
        field_13C_lift_wheel.mFlags.Set(AnimFlags::eLoopBackwards);
        field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eLoopBackwards);
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eNecrum || gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault || gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault_Ender || gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault || gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault_Ender)
    {
        if (field_13C_lift_wheel.GetCurrentFrame() == 1 && field_13C_lift_wheel.mFlags.Get(AnimFlags::eAnimate))
        {
            SfxPlayMono(relive::SoundEffects::WheelSqueak, 0);
        }
    }
    else if (field_13C_lift_wheel.GetCurrentFrame() == 1 && field_13C_lift_wheel.mFlags.Get(AnimFlags::eAnimate) && sGnFrame & 1)
    {
        SfxPlayMono(relive::SoundEffects::WheelSqueak, 0);
    }

    if ((mCurrentLevel != gMap.mCurrentLevel || mCurrentPath != gMap.mCurrentPath || EventGet(kEventDeathReset))
        && mPlatformBaseCount <= 0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void LiftPoint::VRender(PrimHeader** ppOt)
{
    // Renders the pulley, lift platform and lift platform wheel

    // In the current level/map?
    if (mCurrentLevel == gMap.mCurrentLevel && mCurrentPath == gMap.mCurrentPath)
    {
        // Within the current camera X bounds?
        PSX_Point camPos = {};
        gMap.GetCurrentCamCoords(&camPos);

        if (mXPos >= FP_FromInteger(camPos.x) && mXPos <= FP_FromInteger(camPos.x + 640))
        {
            s16 r = mRGB.r;
            s16 g = mRGB.g;
            s16 b = mRGB.b;

            const PSX_RECT bRect = VGetBoundingRect();
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mXPos),
                (bRect.h + bRect.y) / 2,
                GetScale(),
                &r,
                &g,
                &b);

            field_13C_lift_wheel.SetRGB(r, g, b);

            if (gMap.mCurrentLevel != EReliveLevelIds::eNecrum && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                field_13C_lift_wheel.VRender(
                    FP_GetExponent(mXPos - pScreenManager->CamXPos() + (FP_FromInteger(3) * GetSpriteScale())),
                    FP_GetExponent(mYPos - pScreenManager->CamYPos() + (FP_FromInteger(-5) * GetSpriteScale())),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                field_13C_lift_wheel.Get_Frame_Rect(&frameRect);
            }

            if (mHasPulley)
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        FP_FromInteger(field_26C_pulley_xpos),
                        FP_FromInteger(field_26E_pulley_ypos),
                        0))
                {
                    r = mRGB.r;
                    g = mRGB.g;
                    b = mRGB.b;

                    ShadowZone::ShadowZones_Calculate_Colour(
                        field_26C_pulley_xpos,
                        field_26E_pulley_ypos,
                        GetScale(),
                        &r,
                        &g,
                        &b);

                    field_1D4_pulley_anim.SetRGB(r, g, b);

                    field_1D4_pulley_anim.VRender(
                        FP_GetExponent(FP_FromInteger(field_26C_pulley_xpos) - pScreenManager->CamXPos()),
                        FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) - pScreenManager->CamYPos()),
                        ppOt,
                        0,
                        0);

                    PSX_RECT frameRect = {};
                    field_1D4_pulley_anim.Get_Frame_Rect(&frameRect);
                }
            }

            // The base animation is the actual lift/platform itself
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

            if (gMap.mCurrentLevel == EReliveLevelIds::eNecrum && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                field_13C_lift_wheel.VRender(
                    FP_GetExponent(mXPos - pScreenManager->CamXPos() + (FP_FromInteger(3) * GetSpriteScale())),
                    FP_GetExponent(mYPos - pScreenManager->CamYPos() + (FP_FromInteger(-5) * GetSpriteScale())),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                field_13C_lift_wheel.Get_Frame_Rect(&frameRect);
            }
        }
    }
}

void LiftPoint::MoveObjectsOnLift(FP xVelocity)
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        BaseGameObject* pObjectsLiftPoint = sObjectIds.Find_Impl(pObj->BaseAliveGameObject_PlatformId);
        if (pObjectsLiftPoint == this)
        {
            if (pObj == sControlledCharacter)
            {
                // Keep the player in the screen
                const FP oldVelY = pObj->mVelY;
                pObj->mVelY = mVelY;
                pObj->SetActiveCameraDelayedFromDir();

                // And keep the old velY
                pObj->mVelY = oldVelY;
            }

            // Do platforms ever move horizontally? This is always 0 it seems
            pObj->mXPos += xVelocity;

            // Keep ypos on the platform
            pObj->mYPos = FP_FromInteger(mPlatformBaseCollisionLine->mRect.y);

            if (pObj->GetInteractive())
            {
                pObj->mCollectionRect.x = pObj->mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                pObj->mCollectionRect.y = pObj->mYPos - ScaleToGridSize(GetSpriteScale());
                pObj->mCollectionRect.w = pObj->mXPos + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                pObj->mCollectionRect.h = pObj->mYPos;
            }
        }
    }
}

void LiftPoint::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}


void LiftPoint::sub_461000(relive::Path_TLV* pTlv)
{
    pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
    pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
    pTlv->mTlvSpecificMeaning |= 1;
}

void LiftPoint::vStayOnFloor(bool floor, relive::Path_LiftPoint* pTlv)
{
    if (!floor)
    {
        mYPos = FP_FromInteger(pTlv->mTopLeftY + -mPlatformBaseYOffset);
        SfxPlayMono(relive::SoundEffects::LiftStop, 0);
        SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -2000);
    }

    mMoving = false;
    pTlv->mTlvSpecificMeaning = 3;
    field_27C_pTlv = sPathInfo->TLVInfo_From_TLVPtr(pTlv);
    pTlv->mLiftPointId = field_278_lift_point_id;
    mVelY = FP_FromInteger(0);

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}

s32 LiftPoint::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<LiftPointSaveState*>(pSaveBuffer);

    pState->field_0_type = ReliveTypes::eLiftPoint;
    pState->field_4_xpos = mXPos;
    pState->field_8_ypos = mYPos;
    pState->field_C_tlvInfo = mPlatformBaseTlvInfo;
    pState->field_10_pTlv = field_27C_pTlv;
    pState->field_14_floorYLevel = field_270_floorYLevel;
    pState->field_18_lift_point_stop_type = field_130_lift_point_stop_type;

    pState->mMoving = mMoving;
    pState->mTopFloor = mTopFloor;
    pState->mMiddleFloor = mMiddleFloor;
    pState->mBottomFloor = mBottomFloor;
    pState->mMoveToFloorLevel = mMoveToFloorLevel;
    pState->mKeepOnMiddleFloor = mKeepOnMiddleFloor;

    return sizeof(LiftPointSaveState);
}

void LiftPoint::CreatePulleyIfExists()
{
    relive::Path_TLV* pFound = nullptr;

    const PathData* pPathData = sPathInfo->mPathData;
    s16 yCamIdx = FP_GetExponent(mYPos) / pPathData->field_C_grid_height;
    // If we are in the top row of cameras then there can't be a pulley in the screen above because there are no more screens above!
    while (yCamIdx >= 0)
    {
        const s16 xCamIdx = (FP_GetExponent(mXPos) / pPathData->field_A_grid_width) - gMap.mCamIdxOnX;
        // Keep looking up 1 camera for any camera that has TLVs in it.
        relive::Path_TLV* pTlvIter = sPathInfo->Get_First_TLV_For_Offsetted_Camera(xCamIdx, yCamIdx - gMap.mCamIdxOnY);
        while (pTlvIter)
        {
            if (pTlvIter->mTlvType == ReliveTypes::ePulley)
            {
                const FP left = FP_FromInteger(mPlatformBaseCollisionLine->mRect.x) + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                if (left <= FP_FromInteger(pTlvIter->mTopLeftX))
                {
                    const FP right = FP_FromInteger(mPlatformBaseCollisionLine->mRect.w) - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                    if (FP_FromInteger(pTlvIter->mTopLeftX) <= right)
                    {
                        pFound = pTlvIter;
                        break;
                    }
                }
            }
            pTlvIter = Path::TLV_Next_Of_Type(pTlvIter, ReliveTypes::ePulley);
        }

        if (pFound)
        {
            // Found a pulley, stop searching
            break;
        }

        // Go 1 more camera up
        yCamIdx--;
    }

    if (!pFound)
    {
        return;
    }

    // Calculate pulley position
    const FP k13_scaled = FP_FromInteger(13) * GetSpriteScale();
    const FP kM10_scaled = FP_FromInteger(-10) * GetSpriteScale();

    field_26C_pulley_xpos = FP_GetExponent(((kM10_scaled + k13_scaled) / FP_FromInteger(2)) + FP_NoFractional(mXPos));
    field_26E_pulley_ypos = pFound->mTopLeftY;

    const LiftPointData& data = sLiftPointData_545AC8[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    field_1D4_pulley_anim.Init(
        GetAnimRes(data.field_10_lift_top_wheel_anim_id),
        this);

    field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eAnimate);
    field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eSemiTrans);
    field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eBlending);

    mHasPulley = true;

    field_1D4_pulley_anim.SetRenderLayer(GetAnimation().GetRenderLayer());
    field_1D4_pulley_anim.SetSpriteScale(GetSpriteScale());

    field_1D4_pulley_anim.SetRGB(mRGB.r, mRGB.g, mRGB.b);

    field_1D4_pulley_anim.SetRenderMode(TPageAbr::eBlend_0);

    // Set the top of the ropes to be the bottom of the pulley
    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(field_134_rope2_id, ReliveTypes::eRope));
    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(field_138_rope1_id, ReliveTypes::eRope));

    pRope1->field_102_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * GetSpriteScale()));
    pRope2->field_102_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * GetSpriteScale()));
}

LiftPoint::~LiftPoint()
{
    BaseGameObject* pRope2 = sObjectIds.Find(field_134_rope2_id, ReliveTypes::eRope);
    BaseGameObject* pRope1 = sObjectIds.Find(field_138_rope1_id, ReliveTypes::eRope);
    if (pRope2)
    {
        pRope2->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_134_rope2_id = Guid{};
    }

    if (pRope1)
    {
        pRope1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_138_rope1_id = Guid{};
    }

    Path::TLV_Reset(mPlatformBaseTlvInfo, -1, 0, 0);

    relive::Path_TLV* pTlv = sPathInfo->TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(GetSpriteScale() * FP_FromInteger(30)),
        FP_GetExponent(mXPos),
        FP_GetExponent(FP_FromInteger(mPlatformBaseCollisionLine->mRect.y) + (GetSpriteScale() * FP_FromInteger(30))),
        ReliveTypes::eLiftPoint);

    if (pTlv)
    {
        pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
        pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
    }

    field_13C_lift_wheel.VCleanUp();

    if (mHasPulley)
    {
        field_1D4_pulley_anim.VCleanUp();
    }
}
