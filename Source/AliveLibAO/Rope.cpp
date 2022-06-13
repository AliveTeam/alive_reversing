#include "stdafx_ao.h"
#include "Function.hpp"
#include "Rope.hpp"
#include "stdlib.hpp"
#include "PathData.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

namespace AO {

void ClipPoly_Vertically_4584B0(Poly_FT4* pPoly, s32 minY, s32 maxY)
{
    const s32 d1 = minY - Y0(pPoly);
    const s16 polyHeight = Y3(pPoly) - Y0(pPoly);
    if (d1 > 0 && d1 <= polyHeight)
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

Rope::~Rope()
{
    relive_delete[] field_E8_pRopeRes;
}

Rope::Rope(s32 left, s32 top, s32 bottom, FP scale)
{
    mBaseGameObjectTypeId = ReliveTypes::eRope;

    mBaseAnimatedWithPhysicsGameObject_YOffset = 0;

    if (scale == FP_FromInteger(1))
    {
        field_E6_rope_length = 15;
    }
    else
    {
        field_E6_rope_length = 7;
    }

    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eRuptureFarms:
        case EReliveLevelIds::eDesert:
        case EReliveLevelIds::eDesertTemple:
        case EReliveLevelIds::eBoardRoom:
        case EReliveLevelIds::eRuptureFarmsReturn:
        case EReliveLevelIds::eDesertEscape:
        {
            const AnimRecord ropeRec = AO::AnimRec(AnimId::Rope_R1);
            u8** ppRopeRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, ropeRec.mResourceId, 1, 0);
            Animation_Init_417FD0(ropeRec.mFrameTableOffset, ropeRec.mMaxW, ropeRec.mMaxH, ppRopeRes, 1);
            break;
        }

        default:
        {
            const AnimRecord ropeRec = AO::AnimRec(AnimId::Rope_Lines);
            u8** ppRopeRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ropeRec.mResourceId, 1, 0);
            Animation_Init_417FD0(ropeRec.mFrameTableOffset, ropeRec.mMaxW, ropeRec.mMaxH, ppRopeRes, 1);
            break;
        }
    }
    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = scale;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
    if (scale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_RopeWebMeatSaw_24;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_RopeWebMeatSaw_Half_5;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }

    mBaseAnimatedWithPhysicsGameObject_Anim.mRed = 128;
    mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = 128;
    mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = 128;

    field_F2_bottom = static_cast<s16>(bottom);
    field_E4_rope_segment_count = 240 / field_E6_rope_length + 1;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(left);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(bottom);
    field_EC_left = static_cast<s16>(left);
    field_F0_right = static_cast<s16>(left);
    field_EE_top = static_cast<s16>(top);

    field_E8_pRopeRes = relive_new AnimationUnknown[field_E4_rope_segment_count];
    if (field_E8_pRopeRes)
    {
        for (s32 i = 0; i < field_E4_rope_segment_count; i++)
        {
            AnimationUnknown* pSegment = &field_E8_pRopeRes[i];
            pSegment->mAnimFlags.Set(AnimFlags::eBit3_Render);
            pSegment->field_68_anim_ptr = &mBaseAnimatedWithPhysicsGameObject_Anim;
            pSegment->mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
            pSegment->field_6C_scale = scale;
            pSegment->mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
            pSegment->mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
        }
    }
}

void Rope::VUpdate()
{
    // Empty
}

void Rope::VRender(PrimHeader** ppOt)
{
    PSX_Point camPos = {};
    gMap.GetCurrentCamCoords(&camPos);
    if (mBaseAnimatedWithPhysicsGameObject_LvlNumber == gMap.mCurrentLevel)
    {
        if (mBaseAnimatedWithPhysicsGameObject_PathNumber == gMap.mCurrentPath)
        {
            if (mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger(camPos.field_0_x) && mBaseAnimatedWithPhysicsGameObject_XPos <= FP_FromInteger(camPos.field_0_x + 1024))
            {
                const FP camYPos = pScreenManager->mCamPos->field_4_y;

                s32 minY = FP_GetExponent((FP_FromInteger(pScreenManager->mCamYOff + field_EE_top))
                                          - camYPos);
                s32 maxY = FP_GetExponent((FP_FromInteger(pScreenManager->mCamYOff + field_F2_bottom))
                                          - camYPos);

                s16 ypos = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);
                if (ypos > field_F2_bottom)
                {
                    ypos = field_F2_bottom + ((ypos - field_F2_bottom) % field_E6_rope_length);
                }

                s16 screenX = PsxToPCX(
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(pScreenManager->mCamXOff) - pScreenManager->mCamPos->field_0_x),
                    11);
                s16 screenY = FP_GetExponent(
                    (FP_FromInteger(pScreenManager->mCamYOff + ypos))
                    - camYPos);

                if (mBaseAnimatedWithPhysicsGameObject_YOffset + screenY > 240)
                {
                    screenY = screenY % field_E6_rope_length + 240;
                    ypos = FP_GetExponent(pScreenManager->mCamPos->field_4_y
                                          + FP_FromInteger(screenY - pScreenManager->mCamYOff));
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
                    for (s32 idx = 0; idx < field_E4_rope_segment_count; idx++)
                    {
                        s16 r = 128;
                        s16 g = 128;
                        s16 b = 128;
                        ShadowZone::ShadowZones_Calculate_Colour(
                            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                            ypos - (idx * field_E6_rope_length),
                            mBaseAnimatedWithPhysicsGameObject_Scale,
                            &r,
                            &g,
                            &b);

                        field_E8_pRopeRes[idx].mRed = static_cast<u8>(r);
                        field_E8_pRopeRes[idx].mGreen = static_cast<u8>(g);
                        field_E8_pRopeRes[idx].mBlue = static_cast<u8>(b);

                        field_E8_pRopeRes[idx].VRender2(
                            screenX,
                            mBaseAnimatedWithPhysicsGameObject_YOffset + screenY,
                            ppOt);

                        PSX_RECT rect = {};
                        field_E8_pRopeRes[idx].GetRenderedSize(&rect);
                        pScreenManager->InvalidateRect(
                            rect.x,
                            rect.y,
                            rect.w,
                            rect.h,
                            pScreenManager->mIdx);

                        ClipPoly_Vertically_4584B0(
                            &field_E8_pRopeRes[idx].field_10_polys[gPsxDisplay_504C78.field_A_buffer_index],
                            minY + mBaseAnimatedWithPhysicsGameObject_YOffset,
                            maxY + mBaseAnimatedWithPhysicsGameObject_YOffset);

                        screenY -= field_E6_rope_length;
                        if (screenY < minY)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}

} // namespace AO
