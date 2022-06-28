#include "stdafx.h"
#include "LiftPoint.hpp"
#include "Function.hpp"
#include "Rope.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "ObjectIds.hpp"
#include "Abe.hpp"
#include "ShadowZone.hpp"
#include "ScreenManager.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "Grid.hpp"

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

LiftPoint::LiftPoint(Path_LiftPoint* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTlvInfo = tlvInfo;
    SetType(ReliveTypes::eLiftPoint);

    pTlv->field_1_tlv_state = 3;

    field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr(pTlv);

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kLiftResID);
    if (pTlv->field_18_scale != Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    const LiftPointData& rPlatformData = sLiftPointData_545AC8[static_cast<u32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    const AnimRecord& platformRec = AnimRec(rPlatformData.field_0_platform_anim_id);
    AddDynamicCollision(
        platformRec.mFrameTableOffset,
        platformRec.mMaxW,
        static_cast<u16>(platformRec.mMaxH),
        ppRes,
        pTlv,
        &gMap,
        tlvInfo);

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        field_124_pCollisionLine->field_8_type = eLineTypes::eBackgroundDynamicCollision_36;
    }

    SetTint(sLiftTints_55BF50, gMap.mCurrentLevel);

    const FP oldX = mBaseAnimatedWithPhysicsGameObject_XPos;
    MapFollowMe(TRUE);
    const s16 xSnapDelta = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - oldX);
    mPlatformBaseXOffset -= xSnapDelta;
    mPlatformBaseWidthOffset -= xSnapDelta;

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);


    u8** ppLiftWheels = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kLiftWheelsResID);
    const LiftPointData& rLiftWheelData = sLiftPointData_545AC8[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    const AnimRecord& bottomWheelRec = AnimRec(rLiftWheelData.field_C_lift_bottom_wheel_anim_id);
    if (field_13C_lift_wheel.Init(
            bottomWheelRec.mFrameTableOffset,
            gAnimations,
            this,
            static_cast<u16>(bottomWheelRec.mMaxW),
            static_cast<u16>(bottomWheelRec.mMaxH),
            ppLiftWheels))
    {
        if (pTlv->field_18_scale != Scale_short::eFull_0)
        {
            field_13C_lift_wheel.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
            field_13C_lift_wheel.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        }
        else
        {
            field_13C_lift_wheel.mRenderLayer = Layer::eLayer_BeforeShadow_25;
            field_13C_lift_wheel.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        }

        u8** ppAbeLiftRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, TRUE, FALSE);

        field_13C_lift_wheel.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        field_13C_lift_wheel.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_13C_lift_wheel.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);

        field_12C_bMoving &= ~1u;

        field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
        field_280_flags.Clear(LiftFlags::eBit2_bMiddleFloor);
        field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
        field_280_flags.Clear(LiftFlags::eBit5_bMoveToFloorLevel);

        field_274_ppRes = ppAbeLiftRes;

        field_13C_lift_wheel.mGreen = static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_RGB.g);
        field_13C_lift_wheel.mRed = static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_RGB.b);
        field_13C_lift_wheel.mBlue = static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_RGB.r);
        field_13C_lift_wheel.mRenderMode = TPageAbr::eBlend_0;

        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

        const FP k25 = FP_FromInteger(25);
        const FP k13 = FP_FromInteger(13);
        const FP km10 = FP_FromInteger(-10);


        auto pRope1 = relive_new Rope(FP_GetExponent((k13 * mBaseAnimatedWithPhysicsGameObject_SpriteScale + mBaseAnimatedWithPhysicsGameObject_XPos)),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_YPos),
                                   mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        if (pRope1)
        {
            field_138_rope1_id = pRope1->field_8_object_id;
        }

        auto pRope2 = relive_new Rope(FP_GetExponent((km10 * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_YPos),
                                   mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        if (pRope2)
        {
            field_134_rope2_id = pRope2->field_8_object_id;
        }

        pRope2->field_106_bottom = FP_GetExponent((k25 * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + FP_FromInteger(field_124_pCollisionLine->field_0_rect.y));
        pRope1->field_106_bottom = FP_GetExponent((k25 * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + FP_FromInteger(field_124_pCollisionLine->field_0_rect.y));

        const FP v28 = mBaseAnimatedWithPhysicsGameObject_YPos * FP_FromDouble(1.5);
        const FP v29 = FP_FromRaw(FP_GetExponent(v28 * mBaseAnimatedWithPhysicsGameObject_SpriteScale) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);

        pRope2->mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(FP_GetExponent(v29 + (k25 * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(pRope2->field_F6_rope_length)));
        pRope1->mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(FP_GetExponent((k25 * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(pRope1->field_F6_rope_length) - v29));

        field_280_flags.Clear(LiftFlags::eBit4_bHasPulley);
        CreatePulleyIfExists();

        field_278_lift_point_id = static_cast<s8>(pTlv->field_10_lift_point_id);
        field_130_lift_point_stop_type = pTlv->field_16_lift_point_stop_type;

        switch (field_130_lift_point_stop_type)
        {
            case LiftPointStopType::eTopFloor_0:
                field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                break;

            case LiftPointStopType::eBottomFloor_1:
                field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
                break;

            case LiftPointStopType::eMiddleFloor_2:
                field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor);
                break;

            default:
            case LiftPointStopType::eMiddleLockFloor_3: // Not used ??
            case LiftPointStopType::eStartPointOnly_4:
                field_130_lift_point_stop_type = LiftPointStopType::eStartPointOnly_4;
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

static void LoadLiftResourceBans(const char_type* pRopeBanName, const char_type* pLiftBanName)
{
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kRopesResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170(pRopeBanName, nullptr);
    }
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kLiftResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170(pLiftBanName, nullptr);
    }
}

s32 LiftPoint::CreateFromSaveState(const u8* pData)
{
    const LiftPoint_State* pState = reinterpret_cast<const LiftPoint_State*>(pData);

    Path_LiftPoint* pTlv = static_cast<Path_LiftPoint*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_C_tlvInfo));

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABELIFT.BAN", nullptr);
    }

    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eMines:
            LoadLiftResourceBans("ROPES.BAN", "MILIFT.BND");
            break;

        case EReliveLevelIds::eNecrum:
            LoadLiftResourceBans("NECROPE.BAN", "NELIFT.BND");
            break;

        case EReliveLevelIds::eMudomoVault:
        case EReliveLevelIds::eMudomoVault_Ender:
            LoadLiftResourceBans("NECROPE.BAN", "PVLIFT.BND");
            break;

        case EReliveLevelIds::eMudancheeVault:
        case EReliveLevelIds::eMudancheeVault_Ender:
            LoadLiftResourceBans("NECROPE.BAN", "SVLIFT.BND");
            break;

        case EReliveLevelIds::eFeeCoDepot:
        case EReliveLevelIds::eFeeCoDepot_Ender:
            LoadLiftResourceBans("ROPES.BAN", "FDLIFT.BND");
            break;

        case EReliveLevelIds::eBarracks:
        case EReliveLevelIds::eBarracks_Ender:
            LoadLiftResourceBans("ROPES.BAN", "BALIFT.BND");
            break;

        case EReliveLevelIds::eBrewery:
        case EReliveLevelIds::eBrewery_Ender:
            LoadLiftResourceBans("ROPES.BAN", "BRLIFT.BND");
            break;

        default:
            LoadLiftResourceBans("ROPES.BAN", "BWLIFT.BND");
            break;
    }

    auto pLiftPoint = relive_new LiftPoint(pTlv, pState->field_C_tlvInfo);
    pLiftPoint->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_4_xpos;
    pLiftPoint->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_8_ypos;
    pLiftPoint->SyncCollisionLinePosition();

    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->field_134_rope2_id, ReliveTypes::eLiftRope));
    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->field_138_rope1_id, ReliveTypes::eLiftRope));

    pRope2->field_106_bottom = FP_GetExponent(FP_FromInteger(pLiftPoint->field_124_pCollisionLine->field_0_rect.y) + (FP_FromInteger(25) * pLiftPoint->mBaseAnimatedWithPhysicsGameObject_SpriteScale));
    pRope1->field_106_bottom = FP_GetExponent(FP_FromInteger(pLiftPoint->field_124_pCollisionLine->field_0_rect.y) + (FP_FromInteger(25) * pLiftPoint->mBaseAnimatedWithPhysicsGameObject_SpriteScale));

    if (pLiftPoint->field_280_flags.Get(LiftPoint::eBit4_bHasPulley))
    {
        pRope2->field_102_top = FP_GetExponent(FP_FromInteger(pLiftPoint->field_26E_pulley_ypos) + FP_FromInteger(-19) * pLiftPoint->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        pRope1->field_102_top = FP_GetExponent(FP_FromInteger(pLiftPoint->field_26E_pulley_ypos) + FP_FromInteger(-19) * pLiftPoint->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
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


    if (pState->field_10_pTlv == pState->field_C_tlvInfo)
    {
        return sizeof(LiftPoint_State);
    }

    pTlv->field_1_tlv_state = 1;
    if (pState->field_10_pTlv == -1)
    {
        return sizeof(LiftPoint_State);
    }

    Path_TLV* pTlv2 = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_10_pTlv);
    pTlv2->field_1_tlv_state = 3;
    return sizeof(LiftPoint_State);
}

void LiftPoint::vKeepOnMiddleFloor()
{
    field_280_flags.Set(LiftFlags::eBit7_KeepOnMiddleFloor);
}

Bool32 LiftPoint::vOnTopFloor()
{
    return field_280_flags.Get(LiftFlags::eBit1_bTopFloor) && !(field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
}

Bool32 LiftPoint::vOnMiddleFloor()
{
    return field_280_flags.Get(LiftFlags::eBit2_bMiddleFloor) && !(field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
}

Bool32 LiftPoint::vOnBottomFloor()
{
    return field_280_flags.Get(LiftFlags::eBit3_bBottomFloor) && !(field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
}

Bool32 LiftPoint::vOnAnyFloor()
{
    return vOnBottomFloor() || vOnTopFloor() || vOnMiddleFloor();
}

Bool32 LiftPoint::vOnAFloorLiftMoverCanUse()
{
    // Top or bottom floor can still be activated by the lift mover?
    return (vOnMiddleFloor() && !field_280_flags.Get(LiftFlags::eBit8_bIgnoreLiftMover)) || vOnBottomFloor() || vOnTopFloor();
}

Bool32 LiftPoint::vMovingToFloorLevel()
{
    return field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel);
}

void LiftPoint::vMove_4626A0(FP xSpeed, FP ySpeed, s32 /*not_used*/)
{
    mBaseAnimatedWithPhysicsGameObject_VelX = xSpeed * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    mBaseAnimatedWithPhysicsGameObject_VelY = ySpeed * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        field_12C_bMoving |= 1u;
    }
}

void LiftPoint::VRender(PrimHeader** ppOt)
{
    // Renders the pulley, lift platform and lift platform wheel

    // In the current level/map?
    if (mBaseAnimatedWithPhysicsGameObject_LvlNumber == gMap.mCurrentLevel && mBaseAnimatedWithPhysicsGameObject_PathNumber == gMap.mCurrentPath)
    {
        // Within the current camera X bounds?
        PSX_Point camPos = {};
        gMap.GetCurrentCamCoords(&camPos);

        if (mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger(camPos.field_0_x) && mBaseAnimatedWithPhysicsGameObject_XPos <= FP_FromInteger(camPos.field_0_x + 640))
        {
            s16 r = mBaseAnimatedWithPhysicsGameObject_RGB.r;
            s16 g = mBaseAnimatedWithPhysicsGameObject_RGB.g;
            s16 b = mBaseAnimatedWithPhysicsGameObject_RGB.b;

            const PSX_RECT bRect = VGetBoundingRect();
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                (bRect.h + bRect.y) / 2,
                mBaseAnimatedWithPhysicsGameObject_Scale,
                &r,
                &g,
                &b);

            field_13C_lift_wheel.mRed = static_cast<u8>(r);
            field_13C_lift_wheel.mGreen = static_cast<u8>(g);
            field_13C_lift_wheel.mBlue = static_cast<u8>(b);

            if (gMap.mCurrentLevel != EReliveLevelIds::eNecrum && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                field_13C_lift_wheel.VRender(
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos() + (FP_FromInteger(3) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos() + (FP_FromInteger(-5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
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
                        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                        mBaseAnimatedWithPhysicsGameObject_PathNumber,
                        FP_FromInteger(field_26C_pulley_xpos),
                        FP_FromInteger(field_26E_pulley_ypos),
                        0))
                {
                    r = mBaseAnimatedWithPhysicsGameObject_RGB.r;
                    g = mBaseAnimatedWithPhysicsGameObject_RGB.g;
                    b = mBaseAnimatedWithPhysicsGameObject_RGB.b;

                    ShadowZone::ShadowZones_Calculate_Colour(
                        field_26C_pulley_xpos,
                        field_26E_pulley_ypos,
                        mBaseAnimatedWithPhysicsGameObject_Scale,
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
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos() + (FP_FromInteger(3) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos() + (FP_FromInteger(-5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
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
            const bool bOnFloor = field_270_floorYLevel == mBaseAnimatedWithPhysicsGameObject_YPos;
            const FP distToFloor = field_270_floorYLevel - mBaseAnimatedWithPhysicsGameObject_YPos;

            FP absDistToFloor = {};
            if (distToFloor < FP_FromInteger(0) || bOnFloor)
            {
                absDistToFloor = -distToFloor;
            }
            else
            {
                absDistToFloor = distToFloor;
            }

            if (absDistToFloor >= FP_FromInteger(2) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)
            {
                if (distToFloor >= FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(2) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelY = -(FP_FromInteger(2) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                field_12C_bMoving |= 1u;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = field_270_floorYLevel;
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                field_12C_bMoving &= ~1u;
                field_280_flags.Clear(LiftFlags::eBit5_bMoveToFloorLevel);
                SFX_Play_Mono(SoundEffect::LiftStop_30, 0);
                SFX_Play_Pitch(SoundEffect::LiftStop_30, 80, -2000);
                Event_Broadcast(kEventNoise, this);
                Event_Broadcast(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            field_130_lift_point_stop_type = LiftPointStopType::eStartPointOnly_4;
            const FP lineY = FP_FromInteger(field_124_pCollisionLine->field_0_rect.y);
            Path_TLV* pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                lineY,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(30)) + lineY);

            if (pTlvIter)
            {
                while (pTlvIter->field_4_type.mType != TlvTypes::LiftPoint_7)
                {
                    pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                        pTlvIter,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        lineY,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        lineY + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(30)));

                    if (!pTlvIter)
                    {
                        break;
                    }
                }
            }

            // TODO: Bugged if lift point not found - last TLV will get casted which could be anything
            auto pLiftTlv = static_cast<Path_LiftPoint*>(pTlvIter);
            if (pLiftTlv)
            {
                field_130_lift_point_stop_type = pLiftTlv->field_16_lift_point_stop_type;

                field_280_flags.Clear(LiftFlags::eBit8_bIgnoreLiftMover);
                if (pLiftTlv->field_1A_bIgnore_lift_mover == Choice_short::eYes_1)
                {
                    field_280_flags.Set(LiftFlags::eBit8_bIgnoreLiftMover);
                }

                if (pLiftTlv->field_1_tlv_state != static_cast<u8>(pLiftTlv->field_12_bStart_point))
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
                field_270_floorYLevel = FP_FromInteger(pLiftTlv->field_8_top_left.field_2_y + -mPlatformBaseYOffset);
            }
            else
            {
                field_280_flags.Clear(LiftFlags::eBit6);
                field_270_floorYLevel = FP_FromInteger(0);
                field_130_lift_point_stop_type = LiftPointStopType::eStartPointOnly_4;
            }

            const FP distanceToFloor = field_270_floorYLevel - mBaseAnimatedWithPhysicsGameObject_YPos;
            const FP kMinus25Scaled = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-25);
            const FP k30Scaled = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(30);

            switch (field_130_lift_point_stop_type)
            {
                case LiftPointStopType::eTopFloor_0:
                    if (mBaseAnimatedWithPhysicsGameObject_VelY >= FP_FromInteger(0))
                    {
                        if (mBaseAnimatedWithPhysicsGameObject_VelY != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->field_1_tlv_state = 1;
                            field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
                            field_27C_pTlv = -1;
                        }
                        else
                        {
                            field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel);
                            field_12C_bMoving |= 1u;
                            mBaseAnimatedWithPhysicsGameObject_YPos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->field_1_tlv_state = 3;

                            field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr(pLiftTlv);
                            pLiftTlv->field_10_lift_point_id = field_278_lift_point_id;
                            field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                        }
                    }
                    else if (mBaseAnimatedWithPhysicsGameObject_VelY + lineY <= FP_FromInteger(pLiftTlv->field_8_top_left.field_2_y))
                    {
                        vStayOnFloor(field_280_flags.Get(LiftFlags::eBit1_bTopFloor), pLiftTlv);
                        field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                    }
                    break;

                case LiftPointStopType::eBottomFloor_1:
                    if (mBaseAnimatedWithPhysicsGameObject_VelY <= FP_FromInteger(0))
                    {
                        if (mBaseAnimatedWithPhysicsGameObject_VelY != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->field_1_tlv_state = 1;

                            field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
                            field_27C_pTlv = -1;
                        }
                        else
                        {
                            field_12C_bMoving |= 1u;
                            field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel);
                            mBaseAnimatedWithPhysicsGameObject_YPos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->field_1_tlv_state = 3;

                            field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr(pLiftTlv);
                            pLiftTlv->field_10_lift_point_id = field_278_lift_point_id;
                            field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
                        }
                    }
                    else if (lineY + mBaseAnimatedWithPhysicsGameObject_VelY >= FP_FromInteger(pLiftTlv->field_8_top_left.field_2_y))
                    {
                        vStayOnFloor(field_280_flags.Get(LiftFlags::eBit3_bBottomFloor), pLiftTlv);
                        field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
                    }
                    break;

                case LiftPointStopType::eMiddleFloor_2:
                    if (distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                    {
                        pLiftTlv->field_1_tlv_state = 1;
                        field_27C_pTlv = -1;
                    }
                    else
                    {
                        if (field_280_flags.Get(LiftFlags::eBit7_KeepOnMiddleFloor))
                        {
                            vStayOnFloor(field_280_flags.Get(LiftFlags::eBit2_bMiddleFloor), pLiftTlv);
                            field_280_flags.Clear(LiftFlags::eBit7_KeepOnMiddleFloor);
                        }

                        if (mBaseAnimatedWithPhysicsGameObject_VelY == FP_FromInteger(0))
                        {
                            field_12C_bMoving |= 1u;
                            field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel);
                            mBaseAnimatedWithPhysicsGameObject_YPos = field_270_floorYLevel - distanceToFloor;
                        }

                        pLiftTlv->field_1_tlv_state = 3;
                        field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr(pLiftTlv);
                        pLiftTlv->field_10_lift_point_id = field_278_lift_point_id;
                        field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor);
                    }
                    break;

                case LiftPointStopType::eStartPointOnly_4:
                    if (pLiftTlv)
                    {
                        pLiftTlv->field_1_tlv_state = 1;
                        field_27C_pTlv = -1;
                    }
                    field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
                    field_280_flags.Clear(LiftFlags::eBit2_bMiddleFloor);
                    field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
                    break;
                default:
                    break;
            }
        }

        mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

        if (field_124_pCollisionLine)
        {
            SyncCollisionLinePosition();
        }

        MoveObjectsOnLift(mBaseAnimatedWithPhysicsGameObject_VelX);
    }

    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(field_134_rope2_id, ReliveTypes::eLiftRope));
    pRope2->field_106_bottom = FP_GetExponent((FP_FromInteger(field_124_pCollisionLine->field_0_rect.y) + (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)));

    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(field_138_rope1_id, ReliveTypes::eLiftRope));
    pRope1->field_106_bottom = FP_GetExponent((FP_FromInteger(field_124_pCollisionLine->field_0_rect.y) + (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)));

    if (field_280_flags.Get(LiftPoint::eBit4_bHasPulley))
    {
        pRope2->field_102_top = FP_GetExponent((FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)));
        pRope1->field_102_top = FP_GetExponent((FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)));
    }

    const FP v39 = mBaseAnimatedWithPhysicsGameObject_YPos * FP_FromDouble(1.5);
    const FP v40 = FP_FromRaw(FP_GetExponent(v39 * mBaseAnimatedWithPhysicsGameObject_SpriteScale) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);
    pRope2->mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(v40 + (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(pRope2->field_F6_rope_length));
    pRope1->mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional((FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(pRope1->field_F6_rope_length) - v40);

    field_13C_lift_wheel.mAnimFlags.Set(AnimFlags::eBit2_Animate);
    field_1D4_pulley_anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);

    if (mBaseAnimatedWithPhysicsGameObject_VelY == FP_FromInteger(0))
    {
        // Wheels are stopped if not moving
        field_13C_lift_wheel.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        field_1D4_pulley_anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0))
    {
        // Pulley/lift wheels spin opposite ways for up/down
        field_13C_lift_wheel.mAnimFlags.Clear(AnimFlags::eBit19_LoopBackwards);
        field_1D4_pulley_anim.mAnimFlags.Set(AnimFlags::eBit19_LoopBackwards);
    }
    else
    {
        field_13C_lift_wheel.mAnimFlags.Set(AnimFlags::eBit19_LoopBackwards);
        field_1D4_pulley_anim.mAnimFlags.Clear(AnimFlags::eBit19_LoopBackwards);
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eNecrum || gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault || gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault_Ender || gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault || gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault_Ender)
    {
        if (field_13C_lift_wheel.field_92_current_frame == 1 && field_13C_lift_wheel.mAnimFlags.Get(AnimFlags::eBit2_Animate))
        {
            SFX_Play_Mono(SoundEffect::WheelSqueak_31, 0);
        }
    }
    else if (field_13C_lift_wheel.field_92_current_frame == 1 && field_13C_lift_wheel.mAnimFlags.Get(AnimFlags::eBit2_Animate) && sGnFrame & 1)
    {
        SFX_Play_Mono(SoundEffect::WheelSqueak_31, 0);
    }

    if ((mBaseAnimatedWithPhysicsGameObject_LvlNumber != gMap.mCurrentLevel || mBaseAnimatedWithPhysicsGameObject_PathNumber != gMap.mCurrentPath || Event_Get(kEventDeathReset))
        && mPlatformBaseCount <= 0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void LiftPoint::MoveObjectsOnLift(FP xVelocity)
{
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        BaseGameObject* pObjectsLiftPoint = sObjectIds.Find_Impl(pObj->BaseAliveGameObject_PlatformId);
        if (pObjectsLiftPoint == this)
        {
            if (pObj == sControlledCharacter_5C1B8C)
            {
                // Keep the player in the screen
                const FP oldVelY = pObj->mBaseAnimatedWithPhysicsGameObject_VelY;
                pObj->mBaseAnimatedWithPhysicsGameObject_VelY = mBaseAnimatedWithPhysicsGameObject_VelY;
                pObj->SetActiveCameraDelayedFromDir();

                // And keep the old velY
                pObj->mBaseAnimatedWithPhysicsGameObject_VelY = oldVelY;
            }

            // Do platforms ever move horizontally? This is always 0 it seems
            pObj->mBaseAnimatedWithPhysicsGameObject_XPos += xVelocity;

            // Keep ypos on the platform
            pObj->mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(field_124_pCollisionLine->field_0_rect.y);

            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8))
            {
                pObj->mCollectionRect.x = pObj->mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
                pObj->mCollectionRect.y = pObj->mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                pObj->mCollectionRect.w = pObj->mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
                pObj->mCollectionRect.h = pObj->mBaseAnimatedWithPhysicsGameObject_YPos;
            }
        }
    }
}

void LiftPoint::sub_461000(Path_TLV* pTlv)
{
    pTlv->field_0_flags.Clear(eBit1_Created);
    pTlv->field_0_flags.Clear(eBit2_Destroyed);
    pTlv->field_1_tlv_state |= 1;
}

void LiftPoint::vStayOnFloor(s16 floor, Path_LiftPoint* pTlv)
{
    if (!floor)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y + -mPlatformBaseYOffset);
        SFX_Play_Mono(SoundEffect::LiftStop_30, 0);
        SFX_Play_Pitch(SoundEffect::LiftStop_30, 80, -2000);
    }

    field_12C_bMoving &= ~1;
    pTlv->field_1_tlv_state = 3;
    field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr(pTlv);
    pTlv->field_10_lift_point_id = field_278_lift_point_id;
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    Event_Broadcast(kEventNoise, this);
    Event_Broadcast(kEventSuspiciousNoise, this);
}

s32 LiftPoint::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<LiftPoint_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eLiftPoint_78;
    pState->field_4_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_8_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
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
    Path_TLV* pFound = nullptr;

    const PathData* pPathData = sPath_dword_BB47C0->field_C_pPathData;
    s16 yCamIdx = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) / pPathData->field_C_grid_height;
    // If we are in the top row of cameras then there can't be a pulley in the screen above because there are no more screens above!
    while (yCamIdx >= 0)
    {
        const s16 xCamIdx = (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) / pPathData->field_A_grid_width) - gMap.field_D0_cam_x_idx;
        // Keep looking up 1 camera for any camera that has TLVs in it.
        Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera(xCamIdx, yCamIdx - gMap.field_D2_cam_y_idx);
        while (pTlvIter)
        {
            if (pTlvIter->field_4_type == TlvTypes::Pulley_21)
            {
                const FP left = FP_FromInteger(field_124_pCollisionLine->field_0_rect.x) + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
                if (left <= FP_FromInteger(pTlvIter->field_8_top_left.field_0_x))
                {
                    const FP right = FP_FromInteger(field_124_pCollisionLine->field_0_rect.w) - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
                    if (FP_FromInteger(pTlvIter->field_8_top_left.field_0_x) <= right)
                    {
                        pFound = pTlvIter;
                        break;
                    }
                }
            }
            pTlvIter = Path::TLV_Next_Of_Type(pTlvIter, TlvTypes::Pulley_21);
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
    const FP k13_scaled = FP_FromInteger(13) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    const FP kM10_scaled = FP_FromInteger(-10) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    field_26C_pulley_xpos = FP_GetExponent(((kM10_scaled + k13_scaled) / FP_FromInteger(2)) + FP_NoFractional(mBaseAnimatedWithPhysicsGameObject_XPos));
    field_26E_pulley_ypos = pFound->field_8_top_left.field_2_y;

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kLiftWheelsResID);
    const LiftPointData& data = sLiftPointData_545AC8[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    const AnimRecord& topWheelRec = AnimRec(data.field_10_lift_top_wheel_anim_id);
    field_1D4_pulley_anim.Init(
        topWheelRec.mFrameTableOffset,
        gAnimations,
        this,
        static_cast<s16>(topWheelRec.mMaxW),
        static_cast<s16>(topWheelRec.mMaxH),
        ppRes);

    field_1D4_pulley_anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
    field_1D4_pulley_anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_1D4_pulley_anim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);

    field_280_flags.Set(LiftFlags::eBit4_bHasPulley);

    field_1D4_pulley_anim.mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
    field_1D4_pulley_anim.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    field_1D4_pulley_anim.mRed = static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_RGB.r);
    field_1D4_pulley_anim.mGreen = static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_RGB.g);
    field_1D4_pulley_anim.mBlue = static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_RGB.b);

    field_1D4_pulley_anim.mRenderMode = TPageAbr::eBlend_0;

    // Set the top of the ropes to be the bottom of the pulley
    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(field_134_rope2_id, ReliveTypes::eLiftRope));
    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(field_138_rope1_id, ReliveTypes::eLiftRope));

    pRope1->field_102_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * mBaseAnimatedWithPhysicsGameObject_SpriteScale));
    pRope2->field_102_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * mBaseAnimatedWithPhysicsGameObject_SpriteScale));
}

void LiftPoint::VScreenChanged()
{
    if (sActiveHero)
    {
        FP xd = FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos);
        if (xd <= FP_FromInteger(375))
        {
            if (field_274_ppRes == nullptr)
            {
                field_274_ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, TRUE, FALSE);
            }
        }
        else
        {
            ResourceManager::FreeResource_49C330(field_274_ppRes);
            field_274_ppRes = nullptr;
        }
    }

    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

LiftPoint::~LiftPoint()
{
    BaseGameObject* pRope2 = sObjectIds.Find(field_134_rope2_id, ReliveTypes::eLiftRope);
    BaseGameObject* pRope1 = sObjectIds.Find(field_138_rope1_id, ReliveTypes::eLiftRope);
    if (pRope2)
    {
        pRope2->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_134_rope2_id = -1;
    }

    if (pRope1)
    {
        pRope1->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_138_rope1_id = -1;
    }

    Path::TLV_Reset(mPlatformBaseTlvInfo, -1, 0, 0);

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(30)),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(FP_FromInteger(field_124_pCollisionLine->field_0_rect.y) + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(30))),
        TlvTypes::LiftPoint_7);

    if (pTlv)
    {
        pTlv->field_0_flags.Clear(eBit1_Created);
        pTlv->field_0_flags.Clear(eBit2_Destroyed);
    }

    field_13C_lift_wheel.VCleanUp();

    if (field_280_flags.Get(LiftFlags::eBit4_bHasPulley))
    {
        field_1D4_pulley_anim.VCleanUp();
    }

    ResourceManager::FreeResource_49C330(field_274_ppRes);
}
