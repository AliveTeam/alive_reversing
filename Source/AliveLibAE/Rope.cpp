#include "stdafx.h"
#include "Rope.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "Animation.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"

const TintEntry kRopeTints_55FD68[18] = {
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

void ClipPoly_Vertically_4A09E0(Poly_FT4* pPoly, s32 minY, s32 maxY)
{
    const s32 d1 = minY - Y0(pPoly);
    const s16 polyHeight = Y3(pPoly) - Y0(pPoly);
    if (minY - Y0(pPoly) > 0 && d1 <= polyHeight)
    {
        SetXY0(pPoly, X0(pPoly), Y0(pPoly) + (s16) d1);
        SetXY1(pPoly, X1(pPoly), Y1(pPoly) + (s16) d1);

        SetUV0(pPoly, U0(pPoly), V0(pPoly) + (u8) d1);
        SetUV1(pPoly, U1(pPoly), V1(pPoly) + (u8) d1);
    }

    const s32 d2 = Y3(pPoly) - maxY;
    if (d2 > 0 && d2 <= polyHeight)
    {
        SetXY2(pPoly, X2(pPoly), Y2(pPoly) - (s16) d2);
        SetXY3(pPoly, X3(pPoly), Y3(pPoly) - (s16) d2);

        SetUV2(pPoly, U2(pPoly), V2(pPoly) - (u8) d2);
        SetUV3(pPoly, U3(pPoly), V3(pPoly) - (u8) d2);
    }
}

Rope::Rope(s32 left, s32 top, s32 bottom, FP scale)
    : ::BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eLiftRope);

    const AnimRecord& rec = AnimRec(AnimId::Lift_Rope);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    SetTint(kRopeTints_55FD68, gMap.mCurrentLevel);

    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = scale;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_F6_rope_length = 15;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_RopeWebDrill_24;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }
    else
    {
        field_F6_rope_length = 7;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_RopeWebDrill_Half_5;
        mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = FP_FromDouble(0.7);
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.7);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    };

    mBaseAnimatedWithPhysicsGameObject_Anim.mRed = static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_Red);
    mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_Green);
    mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = static_cast<u8>(mBaseAnimatedWithPhysicsGameObject_Blue);

    field_102_top = static_cast<s16>(top);
    field_106_bottom = static_cast<s16>(bottom);
    field_100_left = static_cast<s16>(left);
    field_104_right = static_cast<s16>(left);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(left);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(bottom);

    field_F4_rope_segment_count = (240 / field_F6_rope_length) + 1; // psx screen height

    field_F8_ppRopeRes = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::Resource_Rope, 0, field_F4_rope_segment_count * sizeof(AnimationUnknown));
    field_FC_pRopeRes = reinterpret_cast<AnimationUnknown*>(*field_F8_ppRopeRes);

    for (s32 i = 0; i < field_F4_rope_segment_count; i++)
    {
        AnimationUnknown* pSegment = &field_FC_pRopeRes[i];
        pSegment = new (pSegment) AnimationUnknown(); // We have memory but no constructor was called.. so use placement new to get a constructed instance
        pSegment->mAnimFlags.Set(AnimFlags::eBit3_Render);
        pSegment->field_68_anim_ptr = &mBaseAnimatedWithPhysicsGameObject_Anim;
        pSegment->mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
        pSegment->field_6C_scale = scale;
        pSegment->mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        pSegment->mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
    }
}

void Rope::VUpdate()
{
    // nullsub@4A11E0
}

Rope::~Rope()
{
    ResourceManager::FreeResource_49C330(field_F8_ppRopeRes);
}

void Rope::VRender(PrimHeader** ppOt)
{
    PSX_Point camPos = {};
    gMap.GetCurrentCamCoords(&camPos);
    // In the current level/map?
    if (mBaseAnimatedWithPhysicsGameObject_LvlNumber == gMap.mCurrentLevel && mBaseAnimatedWithPhysicsGameObject_PathNumber == gMap.mCurrentPath)
    {
        // In the current camera x range?
        if (mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger(camPos.field_0_x) && mBaseAnimatedWithPhysicsGameObject_XPos <= FP_FromInteger(camPos.field_0_x + 375))
        {
            const FP camXPos = pScreenManager->CamXPos();
            const FP camYPos = pScreenManager->CamYPos();

            s16 minY = FP_GetExponent(FP_FromInteger(field_102_top) - camYPos);
            s16 maxY = FP_GetExponent(FP_FromInteger(field_106_bottom) - camYPos);

            s16 ypos = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);
            if (ypos > field_106_bottom)
            {
                ypos = field_106_bottom + ((ypos - field_106_bottom) % field_F6_rope_length);
            }

            s16 screenX = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - camXPos);
            s16 screenY = ypos - FP_GetExponent(camYPos);

            if (screenY > 240)
            {
                screenY = (screenY % field_F6_rope_length) + 240;
                ypos = FP_GetExponent(camYPos + (FP_FromInteger(screenY)));
            }

            if (minY < 0)
            {
                minY = 0;
            }

            if (maxY > 240)
            {
                maxY = 240;
            }

            mBaseAnimatedWithPhysicsGameObject_Anim.VRender(640, 240, ppOt, 0, 0);

            if (screenY >= minY)
            {
                for (s32 idx = 0; idx < field_F4_rope_segment_count; idx++)
                {
                    // Apply shadow to the segments colour
                    s16 r = mBaseAnimatedWithPhysicsGameObject_Red;
                    s16 g = mBaseAnimatedWithPhysicsGameObject_Green;
                    s16 b = mBaseAnimatedWithPhysicsGameObject_Blue;

                    ShadowZone::ShadowZones_Calculate_Colour(
                        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                        ypos - (idx * field_F6_rope_length),
                        mBaseAnimatedWithPhysicsGameObject_Scale,
                        &r,
                        &g,
                        &b);

                    field_FC_pRopeRes[idx].mRed = static_cast<u8>(r);
                    field_FC_pRopeRes[idx].mGreen = static_cast<u8>(g);
                    field_FC_pRopeRes[idx].mBlue = static_cast<u8>(b);

                    // Render the segment
                    field_FC_pRopeRes[idx].VRender(
                        screenX,
                        screenY,
                        ppOt,
                        0,
                        0);

                    PSX_RECT rect = {};
                    field_FC_pRopeRes[idx].GetRenderedSize(&rect);
                    pScreenManager->InvalidateRectCurrentIdx(
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h);

                    ClipPoly_Vertically_4A09E0(
                        &field_FC_pRopeRes[idx].field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index],
                        minY,
                        maxY);

                    screenY -= field_F6_rope_length;

                    if (screenY < minY)
                    {
                        break;
                    }
                }
            }
        }
    }
}
