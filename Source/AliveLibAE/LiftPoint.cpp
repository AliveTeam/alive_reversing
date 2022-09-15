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
#include "ResourceManager.hpp"
#include "Map.hpp"

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

const TintEntry sLiftTints_55BF50[18] = {
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
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

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
    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    const LiftPointData& rPlatformData = sLiftPointData_545AC8[static_cast<u32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    AddDynamicCollision(
        rPlatformData.field_0_platform_anim_id,
        pTlv,
        tlvId);

    if (mSpriteScale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        field_124_pCollisionLine->mLineType = eLineTypes::eBackgroundDynamicCollision_36;
    }

    SetTint(sLiftTints_55BF50, gMap.mCurrentLevel);

    const FP oldX = mXPos;
    MapFollowMe(TRUE);
    const s16 xSnapDelta = FP_GetExponent(mXPos - oldX);
    mPlatformBaseXOffset -= xSnapDelta;
    mPlatformBaseWidthOffset -= xSnapDelta;

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);


    const LiftPointData& rLiftWheelData = sLiftPointData_545AC8[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    if (field_13C_lift_wheel.Init(
            GetAnimRes(rLiftWheelData.field_C_lift_bottom_wheel_anim_id),
            this))
    {
        if (pTlv->mScale != relive::reliveScale::eFull)
        {
            field_13C_lift_wheel.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
            field_13C_lift_wheel.field_14_scale = mSpriteScale;
        }
        else
        {
            field_13C_lift_wheel.mRenderLayer = Layer::eLayer_BeforeShadow_25;
            field_13C_lift_wheel.field_14_scale = mSpriteScale;
        }

        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eBit2_Animate);
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eBit16_bBlending);

        field_12C_bMoving &= ~1u;

        field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
        field_280_flags.Clear(LiftFlags::eBit2_bMiddleFloor);
        field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
        field_280_flags.Clear(LiftFlags::eBit5_bMoveToFloorLevel);

        field_13C_lift_wheel.mGreen = static_cast<u8>(mRGB.g);
        field_13C_lift_wheel.mRed = static_cast<u8>(mRGB.b);
        field_13C_lift_wheel.mBlue = static_cast<u8>(mRGB.r);
        field_13C_lift_wheel.mRenderMode = TPageAbr::eBlend_0;

        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);

        const FP k25 = FP_FromInteger(25);
        const FP k13 = FP_FromInteger(13);
        const FP km10 = FP_FromInteger(-10);


        auto pRope1 = relive_new Rope(FP_GetExponent((k13 * mSpriteScale + mXPos)),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * mSpriteScale) + mYPos),
                                   mSpriteScale);
        if (pRope1)
        {
            field_138_rope1_id = pRope1->mBaseGameObjectId;
        }

        auto pRope2 = relive_new Rope(FP_GetExponent((km10 * mSpriteScale) + mXPos),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * mSpriteScale) + mYPos),
                                   mSpriteScale);
        if (pRope2)
        {
            field_134_rope2_id = pRope2->mBaseGameObjectId;
        }

        pRope2->field_106_bottom = FP_GetExponent((k25 * mSpriteScale) + FP_FromInteger(field_124_pCollisionLine->mRect.y));
        pRope1->field_106_bottom = FP_GetExponent((k25 * mSpriteScale) + FP_FromInteger(field_124_pCollisionLine->mRect.y));

        const FP v28 = mYPos * FP_FromDouble(1.5);
        const FP v29 = FP_FromRaw(FP_GetExponent(v28 * mSpriteScale) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);

        pRope2->mYPos = FP_FromInteger(FP_GetExponent(v29 + (k25 * mSpriteScale) + mYPos + FP_FromInteger(pRope2->field_F6_rope_length)));
        pRope1->mYPos = FP_FromInteger(FP_GetExponent((k25 * mSpriteScale) + mYPos + FP_FromInteger(pRope1->field_F6_rope_length) - v29));

        field_280_flags.Clear(LiftFlags::eBit4_bHasPulley);
        CreatePulleyIfExists();

        field_278_lift_point_id = static_cast<s8>(pTlv->mLiftPointId);
        field_130_lift_point_stop_type = pTlv->mLiftPointStopType;

        switch (field_130_lift_point_stop_type)
        {
            case relive::Path_LiftPoint::LiftPointStopType::eTopFloor:
                field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                break;

            case relive::Path_LiftPoint::LiftPointStopType::eBottomFloor:
                field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
                break;

            case relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor:
                field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor);
                break;

            default:
            case relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor: // Not used ??
            case relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly:
                field_130_lift_point_stop_type = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
                break;
        }

        field_280_flags.Clear(LiftFlags::eBit7_KeepOnMiddleFloor);
        field_280_flags.Set(LiftFlags::eBit6);
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

s32 LiftPoint::CreateFromSaveState(const u8* pData)
{
    const LiftPoint_State* pState = reinterpret_cast<const LiftPoint_State*>(pData);

    relive::Path_LiftPoint* pTlv = static_cast<relive::Path_LiftPoint*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_C_tlvInfo));

    auto pLiftPoint = relive_new LiftPoint(pTlv, pState->field_C_tlvInfo);
    if (pLiftPoint)
    {
        pLiftPoint->mXPos = pState->field_4_xpos;
        pLiftPoint->mYPos = pState->field_8_ypos;
        pLiftPoint->SyncCollisionLinePosition();

        Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->field_134_rope2_id, ReliveTypes::eRope));
        Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->field_138_rope1_id, ReliveTypes::eRope));

        pRope2->field_106_bottom = FP_GetExponent(FP_FromInteger(pLiftPoint->field_124_pCollisionLine->mRect.y) + (FP_FromInteger(25) * pLiftPoint->mSpriteScale));
        pRope1->field_106_bottom = FP_GetExponent(FP_FromInteger(pLiftPoint->field_124_pCollisionLine->mRect.y) + (FP_FromInteger(25) * pLiftPoint->mSpriteScale));

        if (pLiftPoint->field_280_flags.Get(LiftPoint::eBit4_bHasPulley))
        {
            pRope2->field_102_top = FP_GetExponent(FP_FromInteger(pLiftPoint->field_26E_pulley_ypos) + FP_FromInteger(-19) * pLiftPoint->mSpriteScale);
            pRope1->field_102_top = FP_GetExponent(FP_FromInteger(pLiftPoint->field_26E_pulley_ypos) + FP_FromInteger(-19) * pLiftPoint->mSpriteScale);
        }

        pLiftPoint->mPlatformBaseTlvInfo = pState->field_C_tlvInfo;
        pLiftPoint->field_27C_pTlv = pState->field_10_pTlv;
        pLiftPoint->field_270_floorYLevel = pState->field_14_floorYLevel;
        pLiftPoint->field_130_lift_point_stop_type = pState->field_18_lift_point_stop_type;

        if (pState->field_1A.Get(LiftPoint_State::eBit1_bMoving))
        {
            pLiftPoint->field_12C_bMoving |= 1;
        }
        else
        {
            pLiftPoint->field_12C_bMoving &= ~1;
        }

        pLiftPoint->field_280_flags.Set(LiftFlags::eBit1_bTopFloor, pState->field_1A.Get(LiftPoint_State::eBit2_bTopFloor));
        pLiftPoint->field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor, pState->field_1A.Get(LiftPoint_State::eBit3_bMiddleFloor));
        pLiftPoint->field_280_flags.Set(LiftFlags::eBit3_bBottomFloor, pState->field_1A.Get(LiftPoint_State::eBit4_bBottomFloor));
        pLiftPoint->field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel, pState->field_1A.Get(LiftPoint_State::eBit5_bMoveToFloorLevel));
        pLiftPoint->field_280_flags.Set(LiftFlags::eBit6, pState->field_1A.Get(LiftPoint_State::eBit6));
        pLiftPoint->field_280_flags.Set(LiftFlags::eBit7_KeepOnMiddleFloor, pState->field_1A.Get(LiftPoint_State::eBit7_KeepOnMiddleFloor));
    }

    if (pState->field_10_pTlv == pState->field_C_tlvInfo)
    {
        return sizeof(LiftPoint_State);
    }

    pTlv->mTlvSpecificMeaning = 1;
    if (pState->field_10_pTlv == Guid{})
    {
        return sizeof(LiftPoint_State);
    }

    relive::Path_TLV* pTlv2 = sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_10_pTlv);
    pTlv2->mTlvSpecificMeaning = 3;
    return sizeof(LiftPoint_State);
}

void LiftPoint::vKeepOnMiddleFloor()
{
    field_280_flags.Set(LiftFlags::eBit7_KeepOnMiddleFloor);
}

bool LiftPoint::vOnTopFloor()
{
    return field_280_flags.Get(LiftFlags::eBit1_bTopFloor) && !(field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
}

bool LiftPoint::vOnMiddleFloor()
{
    return field_280_flags.Get(LiftFlags::eBit2_bMiddleFloor) && !(field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
}

bool LiftPoint::vOnBottomFloor()
{
    return field_280_flags.Get(LiftFlags::eBit3_bBottomFloor) && !(field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
}

bool LiftPoint::vOnAnyFloor()
{
    return vOnBottomFloor() || vOnTopFloor() || vOnMiddleFloor();
}

bool LiftPoint::vOnAFloorLiftMoverCanUse()
{
    // Top or bottom floor can still be activated by the lift mover?
    return (vOnMiddleFloor() && !field_280_flags.Get(LiftFlags::eBit8_bIgnoreLiftMover)) || vOnBottomFloor() || vOnTopFloor();
}

bool LiftPoint::vMovingToFloorLevel()
{
    return field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel);
}

void LiftPoint::vMove_4626A0(FP xSpeed, FP ySpeed, s32 /*not_used*/)
{
    mVelX = xSpeed * mSpriteScale;
    mVelY = ySpeed * mSpriteScale;

    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        field_12C_bMoving |= 1u;
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
                mScale,
                &r,
                &g,
                &b);

            field_13C_lift_wheel.mRed = static_cast<u8>(r);
            field_13C_lift_wheel.mGreen = static_cast<u8>(g);
            field_13C_lift_wheel.mBlue = static_cast<u8>(b);

            if (gMap.mCurrentLevel != EReliveLevelIds::eNecrum && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                field_13C_lift_wheel.VRender(
                    FP_GetExponent(mXPos - pScreenManager->CamXPos() + (FP_FromInteger(3) * mSpriteScale)),
                    FP_GetExponent(mYPos - pScreenManager->CamYPos() + (FP_FromInteger(-5) * mSpriteScale)),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                field_13C_lift_wheel.Get_Frame_Rect(&frameRect);
                pScreenManager->InvalidateRectCurrentIdx(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h);
            }

            if (field_280_flags.Get(LiftFlags::eBit4_bHasPulley))
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
                        mScale,
                        &r,
                        &g,
                        &b);

                    field_1D4_pulley_anim.mRed = static_cast<u8>(r);
                    field_1D4_pulley_anim.mGreen = static_cast<u8>(g);
                    field_1D4_pulley_anim.mBlue = static_cast<u8>(b);

                    field_1D4_pulley_anim.VRender(
                        FP_GetExponent(FP_FromInteger(field_26C_pulley_xpos) - pScreenManager->CamXPos()),
                        FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) - pScreenManager->CamYPos()),
                        ppOt,
                        0,
                        0);

                    PSX_RECT frameRect = {};
                    field_1D4_pulley_anim.Get_Frame_Rect(&frameRect);
                    pScreenManager->InvalidateRectCurrentIdx(
                        frameRect.x,
                        frameRect.y,
                        frameRect.w,
                        frameRect.h);
                }
            }

            // The base animation is the actual lift/platform itself
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

            if (gMap.mCurrentLevel == EReliveLevelIds::eNecrum && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                field_13C_lift_wheel.VRender(
                    FP_GetExponent(mXPos - pScreenManager->CamXPos() + (FP_FromInteger(3) * mSpriteScale)),
                    FP_GetExponent(mYPos - pScreenManager->CamYPos() + (FP_FromInteger(-5) * mSpriteScale)),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                field_13C_lift_wheel.Get_Frame_Rect(&frameRect);
                pScreenManager->InvalidateRectCurrentIdx(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h);
            }
        }
    }
}

void LiftPoint::VUpdate()
{
    if (field_12C_bMoving & 1)
    {
        if (field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel))
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

            if (absDistToFloor >= FP_FromInteger(2) * mSpriteScale)
            {
                if (distToFloor >= FP_FromInteger(0))
                {
                    mVelY = FP_FromInteger(2) * mSpriteScale;
                }
                else
                {
                    mVelY = -(FP_FromInteger(2) * mSpriteScale);
                }
                field_12C_bMoving |= 1u;
            }
            else
            {
                mYPos = field_270_floorYLevel;
                mVelY = FP_FromInteger(0);
                field_12C_bMoving &= ~1u;
                field_280_flags.Clear(LiftFlags::eBit5_bMoveToFloorLevel);
                SfxPlayMono(relive::SoundEffects::LiftStop, 0);
                SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -2000);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            field_130_lift_point_stop_type = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
            const FP lineY = FP_FromInteger(field_124_pCollisionLine->mRect.y);
            relive::Path_TLV* pTlvIter = sPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                lineY,
                mXPos,
                (mSpriteScale * FP_FromInteger(30)) + lineY);

            if (pTlvIter)
            {
                while (pTlvIter->mTlvType != ReliveTypes::eLiftPoint)
                {
                    pTlvIter = sPathInfo->TlvGetAt(
                        pTlvIter,
                        mXPos,
                        lineY,
                        mXPos,
                        lineY + (mSpriteScale * FP_FromInteger(30)));

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

                field_280_flags.Clear(LiftFlags::eBit8_bIgnoreLiftMover);
                if (pLiftTlv->mIgnoreLiftMover == relive::reliveChoice::eYes)
                {
                    field_280_flags.Set(LiftFlags::eBit8_bIgnoreLiftMover);
                }

                if (pLiftTlv->mTlvSpecificMeaning != static_cast<u8>(pLiftTlv->mIsStartPoint))
                {
                    field_280_flags.Set(LiftFlags::eBit6);
                }
                else
                {
                    field_280_flags.Clear(LiftFlags::eBit6);
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
                field_280_flags.Clear(LiftFlags::eBit6);
                field_270_floorYLevel = FP_FromInteger(0);
                field_130_lift_point_stop_type = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
            }

            const FP distanceToFloor = field_270_floorYLevel - mYPos;
            const FP kMinus25Scaled = mSpriteScale * FP_FromInteger(-25);
            const FP k30Scaled = mSpriteScale * FP_FromInteger(30);

            switch (field_130_lift_point_stop_type)
            {
                case relive::Path_LiftPoint::LiftPointStopType::eTopFloor:
                    if (mVelY >= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;
                            field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
                            field_27C_pTlv = Guid{};
                        }
                        else
                        {
                            field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel);
                            field_12C_bMoving |= 1u;
                            mYPos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            field_27C_pTlv = sPathInfo->TLVInfo_From_TLVPtr(pLiftTlv);
                            pLiftTlv->mLiftPointId = field_278_lift_point_id;
                            field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                        }
                    }
                    else if (mVelY + lineY <= FP_FromInteger(pLiftTlv->mTopLeftY))
                    {
                        vStayOnFloor(field_280_flags.Get(LiftFlags::eBit1_bTopFloor), pLiftTlv);
                        field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eBottomFloor:
                    if (mVelY <= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;

                            field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
                            field_27C_pTlv = Guid{};
                        }
                        else
                        {
                            field_12C_bMoving |= 1u;
                            field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel);
                            mYPos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            field_27C_pTlv = sPathInfo->TLVInfo_From_TLVPtr(pLiftTlv);
                            pLiftTlv->mLiftPointId = field_278_lift_point_id;
                            field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
                        }
                    }
                    else if (lineY + mVelY >= FP_FromInteger(pLiftTlv->mTopLeftY))
                    {
                        vStayOnFloor(field_280_flags.Get(LiftFlags::eBit3_bBottomFloor), pLiftTlv);
                        field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
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
                        if (field_280_flags.Get(LiftFlags::eBit7_KeepOnMiddleFloor))
                        {
                            vStayOnFloor(field_280_flags.Get(LiftFlags::eBit2_bMiddleFloor), pLiftTlv);
                            field_280_flags.Clear(LiftFlags::eBit7_KeepOnMiddleFloor);
                        }

                        if (mVelY == FP_FromInteger(0))
                        {
                            field_12C_bMoving |= 1u;
                            field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel);
                            mYPos = field_270_floorYLevel - distanceToFloor;
                        }

                        pLiftTlv->mTlvSpecificMeaning = 3;
                        field_27C_pTlv = sPathInfo->TLVInfo_From_TLVPtr(pLiftTlv);
                        pLiftTlv->mLiftPointId = field_278_lift_point_id;
                        field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor);
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly:
                    if (pLiftTlv)
                    {
                        pLiftTlv->mTlvSpecificMeaning = 1;
                        field_27C_pTlv = Guid{};
                    }
                    field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
                    field_280_flags.Clear(LiftFlags::eBit2_bMiddleFloor);
                    field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
                    break;
                default:
                    break;
            }
        }

        mXPos += mVelX;
        mYPos += mVelY;

        if (field_124_pCollisionLine)
        {
            SyncCollisionLinePosition();
        }

        MoveObjectsOnLift(mVelX);
    }

    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(field_134_rope2_id, ReliveTypes::eRope));
    pRope2->field_106_bottom = FP_GetExponent((FP_FromInteger(field_124_pCollisionLine->mRect.y) + (FP_FromInteger(25) * mSpriteScale)));

    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(field_138_rope1_id, ReliveTypes::eRope));
    pRope1->field_106_bottom = FP_GetExponent((FP_FromInteger(field_124_pCollisionLine->mRect.y) + (FP_FromInteger(25) * mSpriteScale)));

    if (field_280_flags.Get(LiftPoint::eBit4_bHasPulley))
    {
        pRope2->field_102_top = FP_GetExponent((FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * mSpriteScale)));
        pRope1->field_102_top = FP_GetExponent((FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * mSpriteScale)));
    }

    const FP v39 = mYPos * FP_FromDouble(1.5);
    const FP v40 = FP_FromRaw(FP_GetExponent(v39 * mSpriteScale) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);
    pRope2->mYPos = FP_NoFractional(v40 + (FP_FromInteger(25) * mSpriteScale) + mYPos + FP_FromInteger(pRope2->field_F6_rope_length));
    pRope1->mYPos = FP_NoFractional((FP_FromInteger(25) * mSpriteScale) + mYPos + FP_FromInteger(pRope1->field_F6_rope_length) - v40);

    field_13C_lift_wheel.mFlags.Set(AnimFlags::eBit2_Animate);
    field_1D4_pulley_anim.mFlags.Set(AnimFlags::eBit2_Animate);

    if (mVelY == FP_FromInteger(0))
    {
        // Wheels are stopped if not moving
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eBit2_Animate);
        field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eBit2_Animate);
    }
    else if (mVelY > FP_FromInteger(0))
    {
        // Pulley/lift wheels spin opposite ways for up/down
        field_13C_lift_wheel.mFlags.Clear(AnimFlags::eBit19_LoopBackwards);
        field_1D4_pulley_anim.mFlags.Set(AnimFlags::eBit19_LoopBackwards);
    }
    else
    {
        field_13C_lift_wheel.mFlags.Set(AnimFlags::eBit19_LoopBackwards);
        field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eBit19_LoopBackwards);
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eNecrum || gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault || gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault_Ender || gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault || gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault_Ender)
    {
        if (field_13C_lift_wheel.mCurrentFrame == 1 && field_13C_lift_wheel.mFlags.Get(AnimFlags::eBit2_Animate))
        {
            SfxPlayMono(relive::SoundEffects::WheelSqueak, 0);
        }
    }
    else if (field_13C_lift_wheel.mCurrentFrame == 1 && field_13C_lift_wheel.mFlags.Get(AnimFlags::eBit2_Animate) && sGnFrame & 1)
    {
        SfxPlayMono(relive::SoundEffects::WheelSqueak, 0);
    }

    if ((mCurrentLevel != gMap.mCurrentLevel || mCurrentPath != gMap.mCurrentPath || EventGet(kEventDeathReset))
        && mPlatformBaseCount <= 0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void LiftPoint::MoveObjectsOnLift(FP xVelocity)
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
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
            pObj->mYPos = FP_FromInteger(field_124_pCollisionLine->mRect.y);

            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8))
            {
                pObj->mCollectionRect.x = pObj->mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                pObj->mCollectionRect.y = pObj->mYPos - ScaleToGridSize(mSpriteScale);
                pObj->mCollectionRect.w = pObj->mXPos + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                pObj->mCollectionRect.h = pObj->mYPos;
            }
        }
    }
}

void LiftPoint::sub_461000(relive::Path_TLV* pTlv)
{
    pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
    pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
    pTlv->mTlvSpecificMeaning |= 1;
}

void LiftPoint::vStayOnFloor(s16 floor, relive::Path_LiftPoint* pTlv)
{
    if (!floor)
    {
        mYPos = FP_FromInteger(pTlv->mTopLeftY + -mPlatformBaseYOffset);
        SfxPlayMono(relive::SoundEffects::LiftStop, 0);
        SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -2000);
    }

    field_12C_bMoving &= ~1;
    pTlv->mTlvSpecificMeaning = 3;
    field_27C_pTlv = sPathInfo->TLVInfo_From_TLVPtr(pTlv);
    pTlv->mLiftPointId = field_278_lift_point_id;
    mVelY = FP_FromInteger(0);

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}

s32 LiftPoint::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<LiftPoint_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eLiftPoint_78;
    pState->field_4_xpos = mXPos;
    pState->field_8_ypos = mYPos;
    pState->field_C_tlvInfo = mPlatformBaseTlvInfo;
    pState->field_10_pTlv = field_27C_pTlv;
    pState->field_14_floorYLevel = field_270_floorYLevel;
    pState->field_18_lift_point_stop_type = field_130_lift_point_stop_type;


    pState->field_1A.Set(LiftPoint_State::eBit1_bMoving, field_12C_bMoving & 1);
    pState->field_1A.Set(LiftPoint_State::eBit2_bTopFloor, field_280_flags.Get(LiftFlags::eBit1_bTopFloor));
    pState->field_1A.Set(LiftPoint_State::eBit3_bMiddleFloor, field_280_flags.Get(LiftFlags::eBit2_bMiddleFloor));
    pState->field_1A.Set(LiftPoint_State::eBit4_bBottomFloor, field_280_flags.Get(LiftFlags::eBit3_bBottomFloor));
    pState->field_1A.Set(LiftPoint_State::eBit5_bMoveToFloorLevel, field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
    pState->field_1A.Set(LiftPoint_State::eBit6, field_280_flags.Get(LiftFlags::eBit6));
    pState->field_1A.Set(LiftPoint_State::eBit7_KeepOnMiddleFloor, field_280_flags.Get(LiftFlags::eBit7_KeepOnMiddleFloor));

    return sizeof(LiftPoint_State);
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
                const FP left = FP_FromInteger(field_124_pCollisionLine->mRect.x) + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                if (left <= FP_FromInteger(pTlvIter->mTopLeftX))
                {
                    const FP right = FP_FromInteger(field_124_pCollisionLine->mRect.w) - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
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
    const FP k13_scaled = FP_FromInteger(13) * mSpriteScale;
    const FP kM10_scaled = FP_FromInteger(-10) * mSpriteScale;

    field_26C_pulley_xpos = FP_GetExponent(((kM10_scaled + k13_scaled) / FP_FromInteger(2)) + FP_NoFractional(mXPos));
    field_26E_pulley_ypos = pFound->mTopLeftY;

    const LiftPointData& data = sLiftPointData_545AC8[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    field_1D4_pulley_anim.Init(
        GetAnimRes(data.field_10_lift_top_wheel_anim_id),
        this);

    field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eBit2_Animate);
    field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_1D4_pulley_anim.mFlags.Clear(AnimFlags::eBit16_bBlending);

    field_280_flags.Set(LiftFlags::eBit4_bHasPulley);

    field_1D4_pulley_anim.mRenderLayer = mAnim.mRenderLayer;
    field_1D4_pulley_anim.field_14_scale = mSpriteScale;

    field_1D4_pulley_anim.mRed = static_cast<u8>(mRGB.r);
    field_1D4_pulley_anim.mGreen = static_cast<u8>(mRGB.g);
    field_1D4_pulley_anim.mBlue = static_cast<u8>(mRGB.b);

    field_1D4_pulley_anim.mRenderMode = TPageAbr::eBlend_0;

    // Set the top of the ropes to be the bottom of the pulley
    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(field_134_rope2_id, ReliveTypes::eRope));
    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(field_138_rope1_id, ReliveTypes::eRope));

    pRope1->field_102_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * mSpriteScale));
    pRope2->field_102_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * mSpriteScale));
}

void LiftPoint::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
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

    relive::Path_TLV* pTlv = sPathInfo->TLV_Get_At_4DB4B0(
        FP_GetExponent(mXPos),
        FP_GetExponent(mSpriteScale * FP_FromInteger(30)),
        FP_GetExponent(mXPos),
        FP_GetExponent(FP_FromInteger(field_124_pCollisionLine->mRect.y) + (mSpriteScale * FP_FromInteger(30))),
        ReliveTypes::eLiftPoint);

    if (pTlv)
    {
        pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
        pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
    }

    field_13C_lift_wheel.VCleanUp();

    if (field_280_flags.Get(LiftFlags::eBit4_bHasPulley))
    {
        field_1D4_pulley_anim.VCleanUp();
    }
}
