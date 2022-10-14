#include "stdafx_ao.h"
#include "Function.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Rope.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "../relive_lib/ScreenManager.hpp"

namespace AO {

struct LiftPointCoord final
{
    s32 field_0;
    s32 field_4;
};

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

const LiftPointData sLiftPointData_4BB480[16] = {
    {AnimId::LiftPlatform_Forest, 122, 12, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest, 62, 46},                    // menu
    {AnimId::LiftPlatform_RuptureFarms, 117, 19, AnimId::LiftBottomWheel_RuptureFarms, AnimId::LiftTopWheel_RuptureFarms, 124, 37}, // ruputure farms
    {AnimId::LiftPlatform_Lines, 114, 25, AnimId::LiftBottomWheel_Lines, AnimId::LiftTopWheel_Lines, 62, 46},                       // lines
    {AnimId::LiftPlatform_Forest, 122, 12, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest, 62, 46},                    // forest
    {AnimId::LiftPlatform_Forest, 122, 12, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest, 62, 46},                    // forest temple
    {AnimId::LiftPlatform_Forest, 122, 12, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest, 62, 46},                    // stock yards
    {AnimId::LiftPlatform_Forest, 122, 12, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest, 62, 46},                    // stock yards return
    {AnimId::LiftPlatform_Lines, 114, 25, AnimId::LiftBottomWheel_Lines, AnimId::LiftTopWheel_Lines, 62, 46},                       // removed
    {AnimId::LiftPlatform_Desert, 116, 20, AnimId::LiftBottomWheel_Desert, AnimId::LiftTopWheel_Desert, 73, 55},                    // desert
    {AnimId::LiftPlatform_Desert2, 116, 20, AnimId::LiftBottomWheel_Desert2, AnimId::LiftTopWheel_Desert2, 73, 55},                 // desert temple
    {AnimId::LiftPlatform_Forest, 122, 12, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest, 62, 46},                    // credits
    {AnimId::LiftPlatform_Forest, 122, 12, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest, 62, 46},                    // removed
    {AnimId::LiftPlatform_RuptureFarms, 117, 19, AnimId::LiftBottomWheel_RuptureFarms, AnimId::LiftTopWheel_RuptureFarms, 124, 37}, // board room
    {AnimId::LiftPlatform_RuptureFarms, 117, 19, AnimId::LiftBottomWheel_RuptureFarms, AnimId::LiftTopWheel_RuptureFarms, 124, 37}, // rupture farms return
    {AnimId::LiftPlatform_Forest, 122, 12, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest, 62, 46},                    // forest chase
    {AnimId::LiftPlatform_Desert2, 116, 20, AnimId::LiftBottomWheel_Desert2, AnimId::LiftTopWheel_Desert2, 73, 55}};                // desert escape

const LiftPointCoord stru_4BB640[16] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {2, 1},
    {2, 1},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {2, 1}};

void LiftPoint::LoadAnimations()
{
    for (u32 i = 0; i < ALIVE_COUNTOF(sLiftPointData_4BB480); i++)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sLiftPointData_4BB480[i].field_0_platform_anim_id));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sLiftPointData_4BB480[i].field_10_lift_top_wheel_anim_id));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sLiftPointData_4BB480[i].field_C_lift_bottom_wheel_anim_id));
    }
}

LiftPoint::LiftPoint(relive::Path_LiftPoint* pTlv, Map* pPath, const Guid& tlvId)
    : PlatformBase()
{
    SetType(ReliveTypes::eLiftPoint);

    LoadAnimations();

    pTlv->mTlvSpecificMeaning = 3;

    const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
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

    AddDynamicCollision(
        sLiftPointData_4BB480[lvl_idx].field_0_platform_anim_id,
        pTlv,
        pPath,
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

    const FP oldX = mXPos;
    MapFollowMe(1);
    const FP newX = mXPos;

    GetAnimation().mFlags.Set(AnimFlags::eSemiTrans);

    const auto xMovedBy = FP_GetExponent(newX - oldX);
    mPlatformBaseXOffset -= xMovedBy;
    mPlatformBaseWidthOffset -= xMovedBy;

    if (field_13C_lift_wheel.Init(
            GetAnimRes(sLiftPointData_4BB480[lvl_idx].field_C_lift_bottom_wheel_anim_id),
            this))
    {
        if (pTlv->mScale == relive::reliveScale::eHalf)
        {
            field_13C_lift_wheel.SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        }
        else
        {
            field_13C_lift_wheel.SetRenderLayer(Layer::eLayer_BeforeShadow_25);
        }

        field_13C_lift_wheel.SetSpriteScale(GetSpriteScale());

        field_27A_flags.Clear(Flags::eBit6_bMoveToFloorLevel);
        field_27A_flags.Clear(Flags::eBit4_bBottomFloor);
        field_27A_flags.Clear(Flags::eBit3_bMiddleFloor);
        field_27A_flags.Clear(Flags::eBit2_bTopFloor);

        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eSemiTrans);
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eBlending);
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eAnimate);

        field_12C_bMoving &= ~1u;
        mVelX = FP_FromInteger(0);
        field_13C_lift_wheel.SetRGB(128, 128, 128);
        field_13C_lift_wheel.SetRenderMode(TPageAbr::eBlend_0);
        field_10C = 1;

        mVelY = FP_FromInteger(0);
        mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;

        auto pRopeMem = relive_new Rope(
            FP_GetExponent(mXPos + (FP_FromInteger(13) * GetSpriteScale()) + FP_FromInteger(stru_4BB640[lvl_idx].field_4)),
            0,
            FP_GetExponent(mYPos + (FP_FromInteger(25) * GetSpriteScale())),
            GetSpriteScale());
        if (pRopeMem)
        {
            pRopeMem->mBaseGameObjectRefCount++;
            field_138_pRope1 = pRopeMem;
        }

        auto pRopeMem2 = relive_new Rope(
            FP_GetExponent(mXPos + (FP_FromInteger(-10) * GetSpriteScale()) + FP_FromInteger(stru_4BB640[lvl_idx].field_0)),
            0,
            FP_GetExponent(mYPos + (FP_FromInteger(25) * GetSpriteScale())),
            GetSpriteScale());;
        if (pRopeMem2)
        {
            pRopeMem2->mBaseGameObjectRefCount++;
            field_134_pRope2 = pRopeMem2;
        }

        field_134_pRope2->field_F2_bottom = FP_GetExponent((FP_FromInteger(25) * GetSpriteScale()) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y));
        field_138_pRope1->field_F2_bottom = FP_GetExponent((FP_FromInteger(25) * GetSpriteScale()) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y));

        const FP v29 = FP_FromRaw(FP_GetExponent((mYPos * FP_FromDouble(1.5)) * GetSpriteScale()) % FP_FromInteger(field_134_pRope2->field_E6_rope_length).fpValue);
        field_134_pRope2->mYPos = FP_NoFractional(mYPos + v29 + (FP_FromInteger(25) * GetSpriteScale()) + FP_FromInteger(field_134_pRope2->field_E6_rope_length));
        field_138_pRope1->mYPos = FP_NoFractional(mYPos + v29 - (FP_FromInteger(25) * GetSpriteScale()) + FP_FromInteger(field_138_pRope1->field_E6_rope_length));

        field_27A_flags.Clear(Flags::eBit5_bHasPulley);

        CreatePulleyIfExists(0, 0);

        field_278_lift_point_id = pTlv->mLiftPointId;
        mLiftPointStopType = pTlv->mLiftPointStopType;

        switch (mLiftPointStopType)
        {
            case relive::Path_LiftPoint::LiftPointStopType::eTopFloor:
                field_27A_flags.Set(Flags::eBit2_bTopFloor);
                break;

            case relive::Path_LiftPoint::LiftPointStopType::eBottomFloor:
                field_27A_flags.Set(Flags::eBit4_bBottomFloor);
                break;

            case relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor:
                field_27A_flags.Set(Flags::eBit3_bMiddleFloor);
                break;
        }

        field_27A_flags.Clear(Flags::eBit8_KeepOnMiddleFloor);
        field_27A_flags.Set(Flags::eBit7_bIgnoreLiftMover);
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

void LiftPoint::Move(FP xSpeed, FP ySpeed, s32 /*not_used*/)
{
    mVelX = xSpeed;
    mVelY = ySpeed;
    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        field_12C_bMoving |= 1u;
    }
}


void LiftPoint::StayOnFloor(s16 floor, relive::Path_LiftPoint* pLiftTlv)
{
    if (!floor)
    {
        mYPos = FP_FromInteger(pLiftTlv->mTopLeftY - mPlatformBaseYOffset);
        SfxPlayMono(relive::SoundEffects::LiftStop, 0);
        SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -2000);
    }

    field_12C_bMoving &= ~1u;
    pLiftTlv->mTlvSpecificMeaning = 3;
    pLiftTlv->mLiftPointId = field_278_lift_point_id;
    mVelY = FP_FromInteger(0);

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}


void LiftPoint::Sub_Unknown(relive::Path_TLV* pTlv)
{
    pTlv->mTlvFlags.Clear(relive::eBit1_Created);
    pTlv->mTlvFlags.Clear(relive::eBit2_Destroyed);
    pTlv->mTlvSpecificMeaning |= 1;
}

bool LiftPoint::OnTopFloor() const
{
    return field_27A_flags.Get(Flags::eBit2_bTopFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnBottomFloor() const
{
    return field_27A_flags.Get(Flags::eBit4_bBottomFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnMiddleFloor() const
{
    return field_27A_flags.Get(Flags::eBit3_bMiddleFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnAnyFloor() const
{
    return OnBottomFloor() || OnTopFloor() || OnMiddleFloor();
}


bool LiftPoint::OnAFloorLiftMoverCanUse() const
{
    return (OnMiddleFloor() && !field_27A_flags.Get(Flags::eBit7_bIgnoreLiftMover)) || OnBottomFloor() || OnTopFloor();
}

void LiftPoint::VUpdate()
{
    if (field_12C_bMoving & 1)
    {
        if (field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel))
        {
            const bool bOnFloor = field_270_floorYLevel == mYPos;
            const FP distToFloor = field_270_floorYLevel - mYPos;
            FP absDistToFloor = distToFloor;
            if (distToFloor < FP_FromInteger(0) || bOnFloor)
            {
                absDistToFloor = -distToFloor;
            }
            else
            {
                absDistToFloor = distToFloor;
            }

            if (absDistToFloor >= (FP_FromInteger(2) * GetSpriteScale()))
            {
                if (distToFloor >= FP_FromInteger(0))
                {
                    mVelY = (FP_FromInteger(2) * GetSpriteScale());
                }
                else
                {
                    mVelY = -(FP_FromInteger(2) * GetSpriteScale());
                }
                field_12C_bMoving |= 1u;
            }
            else
            {
                mYPos = field_270_floorYLevel;
                mVelY = FP_FromInteger(0);
                field_12C_bMoving &= ~1u;
                field_27A_flags.Clear(Flags::eBit6_bMoveToFloorLevel);
                SfxPlayMono(relive::SoundEffects::LiftStop, 0);
                SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -2000);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
            const FP lineY = FP_FromInteger(mPlatformBaseCollisionLine->mRect.y);

            relive::Path_LiftPoint* pLiftTlv = nullptr;
            relive::Path_TLV* pTlvIter = gMap.TLV_Get_At(
                nullptr,
                mXPos,
                lineY,
                mXPos,
                (GetSpriteScale() * FP_FromInteger(30)) + lineY);
            while (pTlvIter)
            {
                if (pTlvIter->mTlvType == ReliveTypes::eLiftPoint)
                {
                    pLiftTlv = static_cast<relive::Path_LiftPoint*>(pTlvIter);
                    mLiftPointStopType = pLiftTlv->mLiftPointStopType;
                    break;
                }
                pTlvIter = gMap.TLV_Get_At(
                    pTlvIter,
                    mXPos,
                    lineY,
                    mXPos,
                    lineY + (GetSpriteScale() * FP_FromInteger(30)));
            }

            if (pLiftTlv && pLiftTlv->mIsStartPoint == relive::reliveChoice::eYes)
            {
                field_27A_flags.Set(Flags::eBit7_bIgnoreLiftMover);
            }
            else
            {
                field_27A_flags.Clear(Flags::eBit7_bIgnoreLiftMover);
            }

            if (pLiftTlv)
            {
                Sub_Unknown(pLiftTlv);

                field_270_floorYLevel = FP_FromInteger(pTlvIter->mTopLeftY + -mPlatformBaseYOffset);
            }
            else
            {
                field_270_floorYLevel = FP_FromInteger(0);
                mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
            }

            const auto distanceToFloor = field_270_floorYLevel - mYPos;
            const FP kMinus25Scaled = (GetSpriteScale() * FP_FromInteger(-25));
            const FP k30Scaled = (GetSpriteScale() * FP_FromInteger(30));

            switch (mLiftPointStopType)
            {
                case relive::Path_LiftPoint::LiftPointStopType::eTopFloor:
                    if (mVelY >= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || (distanceToFloor <= kMinus25Scaled) || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;
                            field_27A_flags.Clear(Flags::eBit2_bTopFloor);
                        }
                        else
                        {
                            field_27A_flags.Set(Flags::eBit6_bMoveToFloorLevel);
                            field_12C_bMoving |= 1u;
                            mYPos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            pLiftTlv->mLiftPointId = field_278_lift_point_id;
                            field_27A_flags.Set(Flags::eBit2_bTopFloor);
                        }
                    }
                    else if (mVelY + lineY <= FP_FromInteger(pTlvIter->mTopLeftY))
                    {
                        StayOnFloor(field_27A_flags.Get(Flags::eBit2_bTopFloor), pLiftTlv);

                        field_27A_flags.Set(Flags::eBit2_bTopFloor);
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eBottomFloor:
                    if (mVelY <= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || (distanceToFloor <= kMinus25Scaled) || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;
                            field_27A_flags.Clear(Flags::eBit4_bBottomFloor);
                        }
                        else
                        {
                            field_12C_bMoving |= 1u;
                            field_27A_flags.Set(Flags::eBit6_bMoveToFloorLevel);
                            mYPos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            pLiftTlv->mLiftPointId = field_278_lift_point_id;
                            field_27A_flags.Set(Flags::eBit4_bBottomFloor);
                        }
                    }
                    else if (mVelY + lineY >= FP_FromInteger(pTlvIter->mTopLeftY))
                    {
                        StayOnFloor(field_27A_flags.Get(Flags::eBit4_bBottomFloor), pLiftTlv);
                        field_27A_flags.Set(Flags::eBit4_bBottomFloor);
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor:
                    if (distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                    {
                        pLiftTlv->mTlvSpecificMeaning = 1;
                    }
                    else
                    {
                        if (field_27A_flags.Get(Flags::eBit8_KeepOnMiddleFloor))
                        {
                            StayOnFloor(field_27A_flags.Get(Flags::eBit3_bMiddleFloor), pLiftTlv);
                            field_27A_flags.Clear(Flags::eBit8_KeepOnMiddleFloor);
                        }

                        if (mVelY == FP_FromInteger(0))
                        {
                            field_12C_bMoving |= 1u;
                            field_27A_flags.Set(Flags::eBit6_bMoveToFloorLevel);
                            mYPos = field_270_floorYLevel - distanceToFloor;
                        }

                        pLiftTlv->mTlvSpecificMeaning = 3;
                        pLiftTlv->mLiftPointId = field_278_lift_point_id;
                        field_27A_flags.Set(Flags::eBit3_bMiddleFloor);
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly:
                    if (pLiftTlv)
                    {
                        pLiftTlv->mTlvSpecificMeaning = 1;
                    }

                    field_27A_flags.Clear(Flags::eBit2_bTopFloor);
                    field_27A_flags.Clear(Flags::eBit3_bMiddleFloor);
                    field_27A_flags.Clear(Flags::eBit4_bBottomFloor);
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
        KeepThingsOnPlatform(mVelX);
    }

    const FP FP_25xScale = FP_FromInteger(25) * GetSpriteScale();
    const FP FP_m19xScale = FP_FromInteger(-19) * GetSpriteScale();

    Rope* pRope2 = field_134_pRope2;
    Rope* pRope1 = field_138_pRope1;

    const FP rope2_rope_length = FP_FromInteger(pRope2->field_E6_rope_length);
    const FP rope1_rope_length = FP_FromInteger(pRope1->field_E6_rope_length);

    const FP pColliRectY = FP_FromInteger(mPlatformBaseCollisionLine->mRect.y);

    pRope2->field_F2_bottom = FP_GetExponent(pColliRectY + FP_25xScale);
    pRope1->field_F2_bottom = FP_GetExponent(pColliRectY + FP_25xScale);

    if (field_27A_flags.Get(Flags::eBit5_bHasPulley))
    {
        const FP pulley_ypos = FP_FromInteger(field_26E_pulley_ypos);

        pRope2->field_EE_top = FP_GetExponent(pulley_ypos + FP_m19xScale);
        pRope1->field_EE_top = FP_GetExponent(pulley_ypos + FP_m19xScale);
    }

    const FP new_ypos = (mYPos * FP_FromDouble(1.5));
    const FP remaining_rope = FP_FromRaw(FP_GetExponent(new_ypos * GetSpriteScale()) % rope2_rope_length.fpValue);

    pRope2->mYPos = FP_NoFractional((mYPos + remaining_rope) + (FP_25xScale + rope2_rope_length));
    pRope1->mYPos = FP_NoFractional((mYPos + FP_25xScale) + (rope1_rope_length - remaining_rope));


    field_13C_lift_wheel.mFlags.Set(AnimFlags::eAnimate);
    field_1D4_pulley_anim.mFlags.Set(AnimFlags::eAnimate);

    if (mVelY == FP_FromInteger(0))
    {
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eAnimate);
        field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eAnimate);
    }
    else if (mVelY > FP_FromInteger(0))
    {
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eLoopBackwards);
        field_1D4_pulley_anim.mFlags.Set(AnimFlags::eLoopBackwards);
    }
    else if (mVelY < FP_FromInteger(0))
    {
        field_13C_lift_wheel.mFlags.Set(AnimFlags::eLoopBackwards);
        field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eLoopBackwards);
    }

    if (field_13C_lift_wheel.GetCurrentFrame() == 1)
    {
        if (field_13C_lift_wheel.mFlags.Get(AnimFlags::eAnimate))
        {
            SfxPlayMono(relive::SoundEffects::WheelSqueak, 0);
        }
    }

    if (mCurrentLevel != gMap.mCurrentLevel || mCurrentPath != gMap.mCurrentPath || EventGet(kEventDeathReset))
    {
        if (mPlatformBaseCount <= 0)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void LiftPoint::VRender(PrimHeader** ppOt)
{
    PSX_Point mapCoord = {};
    gMap.GetCurrentCamCoords(&mapCoord);
    if (mCurrentLevel == gMap.mCurrentLevel)
    {
        if (mCurrentPath == gMap.mCurrentPath)
        {
            if (mXPos >= FP_FromInteger(mapCoord.x) && mXPos <= FP_FromInteger(mapCoord.x + 1024))
            {
                BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

                s16 liftWheelR = mRGB.r;
                s16 liftWheelG = mRGB.g;
                s16 liftWheelB = mRGB.b;

                const PSX_RECT boundingRect = VGetBoundingRect();

                ShadowZone::ShadowZones_Calculate_Colour(
                    FP_GetExponent(mXPos),
                    (boundingRect.y + boundingRect.h) / 2,
                    GetScale(),
                    &liftWheelR,
                    &liftWheelG,
                    &liftWheelB);

                field_13C_lift_wheel.SetRGB(liftWheelR, liftWheelG, liftWheelB);

                const FP xOff = (FP_FromInteger(3) * GetSpriteScale());
                const FP yOff = (FP_FromInteger(-5) * GetSpriteScale());

                field_13C_lift_wheel.VRender(
                    FP_GetExponent(mXPos
                                   + xOff
                                   + FP_FromInteger(pScreenManager->mCamXOff) - pScreenManager->mCamPos->x),
                    FP_GetExponent(mYPos
                                   + yOff
                                   + FP_FromInteger(pScreenManager->mCamYOff) - pScreenManager->mCamPos->y),
                    ppOt,
                    0,
                    0);

                PSX_RECT liftWheelRect = {};
                field_13C_lift_wheel.Get_Frame_Rect(&liftWheelRect);

                if (field_27A_flags.Get(Flags::eBit5_bHasPulley))
                {
                    const FP pulley_xpos = FP_FromInteger(field_26C_pulley_xpos);
                    const FP pulley_ypos = FP_FromInteger(field_26E_pulley_ypos);
                    if (gMap.Is_Point_In_Current_Camera(
                            mCurrentLevel,
                            mCurrentPath,
                            pulley_xpos,
                            pulley_ypos,
                            0))
                    {
                        s16 r = mRGB.r;
                        s16 g = mRGB.g;
                        s16 b = mRGB.b;

                        ShadowZone::ShadowZones_Calculate_Colour(
                            field_26C_pulley_xpos,
                            field_26E_pulley_ypos,
                            GetScale(),
                            &r,
                            &g,
                            &b);

                        field_1D4_pulley_anim.SetRGB(r, g, b);

                        field_1D4_pulley_anim.VRender(
                            FP_GetExponent(pulley_xpos + FP_FromInteger(pScreenManager->mCamXOff) - pScreenManager->mCamPos->x),
                            FP_GetExponent(pulley_ypos + FP_FromInteger(pScreenManager->mCamYOff) - pScreenManager->mCamPos->y),
                            ppOt,
                            0,
                            0);
                        PSX_RECT pulleyRect = {};
                        field_1D4_pulley_anim.Get_Frame_Rect(&pulleyRect);
                    }
                }
            }
        }
    }
}

void LiftPoint::VScreenChanged()
{
    if (!field_27A_flags.Get(Flags::eBit5_bHasPulley))
    {
        CreatePulleyIfExists(0, -1);
    }

    if (gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

LiftPoint::~LiftPoint()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        return;
    }

    if (field_134_pRope2)
    {
        field_134_pRope2->mBaseGameObjectRefCount--;
        field_134_pRope2->mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (field_138_pRope1)
    {
        field_138_pRope1->mBaseGameObjectRefCount--;
        field_138_pRope1->mBaseGameObjectFlags.Set(Options::eDead);
    }

    field_134_pRope2 = nullptr;
    field_138_pRope1 = nullptr;

    Path::TLV_Reset(mPlatformBaseTlvInfo, -1, 0, 0);

    auto pLiftPointTlv = gMap.TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(FP_FromInteger(mPlatformBaseCollisionLine->mRect.y)),
        FP_GetExponent(mXPos),
        FP_GetExponent((GetSpriteScale() * FP_FromInteger(30)) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y)),
        ReliveTypes::eLiftPoint);

    if (pLiftPointTlv)
    {
        pLiftPointTlv->mTlvFlags.Clear(relive::eBit1_Created);
        pLiftPointTlv->mTlvFlags.Clear(relive::eBit2_Destroyed);
    }

    field_13C_lift_wheel.VCleanUp();

    if (field_27A_flags.Get(Flags::eBit5_bHasPulley))
    {
        field_1D4_pulley_anim.VCleanUp();
    }
}

void LiftPoint::CreatePulleyIfExists(s16 camX, s16 camY)
{
    auto pTlv = gMap.Get_First_TLV_For_Offsetted_Camera(camX, camY);
    if (pTlv)
    {
        while (1)
        {
            field_26E_pulley_ypos = pTlv->mTopLeftY;
            field_26C_pulley_xpos = pTlv->mTopLeftX;

            if (pTlv->mTlvType == ReliveTypes::ePulley)
            {
                if (mPlatformBaseCollisionLine->mRect.x <= pTlv->mTopLeftX && pTlv->mTopLeftX <= mPlatformBaseCollisionLine->mRect.w)
                {
                    break;
                }
            }

            pTlv = gMap.TLV_Get_At(pTlv, FP_FromInteger(-1), FP_FromInteger(-1), FP_FromInteger(-1), FP_FromInteger(-1));
            if (!pTlv)
            {
                return;
            }
        }

        const FP k13_scaled = FP_FromInteger(13) * GetSpriteScale();
        const FP kM10_scaled = FP_FromInteger(-10) * GetSpriteScale();

        field_26C_pulley_xpos = FP_GetExponent(((k13_scaled + kM10_scaled) / FP_FromInteger(2)) + FP_NoFractional(mXPos));

        const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

        field_1D4_pulley_anim.Init(GetAnimRes(sLiftPointData_4BB480[lvl_idx].field_10_lift_top_wheel_anim_id), this);

        field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eSemiTrans);
        field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eBlending);
        field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eAnimate);

        field_27A_flags.Set(Flags::eBit5_bHasPulley);

        field_1D4_pulley_anim.SetRGB(128, 128, 128);

        field_1D4_pulley_anim.SetRenderLayer(GetAnimation().GetRenderLayer());
        field_1D4_pulley_anim.SetSpriteScale(GetSpriteScale());
        field_1D4_pulley_anim.SetRenderMode(TPageAbr::eBlend_0);

        field_134_pRope2->field_EE_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * field_1D4_pulley_anim.GetSpriteScale()));
        field_138_pRope1->field_EE_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * field_1D4_pulley_anim.GetSpriteScale()));
    }
}

} // namespace AO
