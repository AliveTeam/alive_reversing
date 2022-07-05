#include "stdafx_ao.h"
#include "Function.hpp"
#include "ParamiteWeb.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "ResourceManager.hpp"
#include "Rope.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "AnimationUnknown.hpp"

namespace AO {

void ParamiteWeb::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

ParamiteWeb::~ParamiteWeb()
{
    relive_delete[] field_EC_pRes;
}

ParamiteWeb::ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale)
{
    mBaseGameObjectTypeId = ReliveTypes::eRope;

    mBaseAnimatedWithPhysicsGameObject_YOffset = 0;

    if (scale == FP_FromInteger(1))
    {
        field_E6_segment_length = 15;
    }
    else
    {
        field_E6_segment_length = 7;
    }

    const AnimRecord rec = AO::AnimRec(AnimId::ParamiteWeb);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::ParamiteWeb, ppRes);

    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = scale;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

    if (scale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_24;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_Half_5;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }


    mBaseAnimatedWithPhysicsGameObject_Anim.mRed = 128;
    mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = 128;
    mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = 128;

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    field_EA_ttl_remainder = static_cast<s16>(top);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(top);
    field_E8_ttl = static_cast<s16>(bottom);

    field_E4_number_of_segments = 240 / field_E6_segment_length;

    field_EC_pRes = relive_new AnimationUnknown[field_E4_number_of_segments];
    if (field_EC_pRes)
    {
        for (s32 i = 0; i < field_E4_number_of_segments; i++)
        {
            AnimationUnknown* pSegment = &field_EC_pRes[i];
            pSegment->mAnimFlags.Set(AnimFlags::eBit3_Render);
            pSegment->field_68_anim_ptr = &mBaseAnimatedWithPhysicsGameObject_Anim;
            pSegment->mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
            pSegment->field_6C_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
            pSegment->mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
            pSegment->mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
        }
    }

    field_F0_bEnabled = 0;
}

void ParamiteWeb::VUpdate()
{
    if (field_F0_bEnabled == 1)
    {
        field_EA_ttl_remainder -= 8;
        if (field_EA_ttl_remainder <= field_E8_ttl)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

//TODO @ 100% - merge with Rope::vRender()
void ParamiteWeb::VRender(PrimHeader** ppOt)
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);
    if (mBaseAnimatedWithPhysicsGameObject_LvlNumber == gMap.mCurrentLevel && mBaseAnimatedWithPhysicsGameObject_PathNumber == gMap.mCurrentPath)
    {
        if (mBaseAnimatedWithPhysicsGameObject_XPos >= FP_FromInteger(camCoords.x) && mBaseAnimatedWithPhysicsGameObject_XPos <= FP_FromInteger(camCoords.x + 1024))
        {
            const FP cam_y = pScreenManager->mCamPos->y;
            const FP cam_x = pScreenManager->mCamPos->x;

            s16 minY = FP_GetExponent(FP_FromInteger(pScreenManager->mCamYOff + field_E8_ttl) - cam_y);
            s16 maxY = FP_GetExponent(FP_FromInteger(pScreenManager->mCamYOff + field_EA_ttl_remainder) - cam_y);

            s16 ypos_int = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);
            if (ypos_int > field_EA_ttl_remainder)
            {
                ypos_int = field_EA_ttl_remainder + (ypos_int - field_EA_ttl_remainder) % field_E6_segment_length;
            }

            const s16 x_start = PsxToPCX<s16>(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(pScreenManager->mCamXOff) - cam_x));

            s16 y_start = FP_GetExponent(FP_FromInteger(pScreenManager->mCamYOff + ypos_int) - cam_y);
            if (mBaseAnimatedWithPhysicsGameObject_YOffset + y_start > 240)
            {
                y_start = y_start % field_E6_segment_length + 240;
                ypos_int = FP_GetExponent(cam_y + FP_FromInteger(y_start - pScreenManager->mCamYOff));
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

            if (y_start >= minY)
            {
                for (s32 idx = 0; idx < field_E4_number_of_segments; ++idx)
                {
                    s16 r = 128;
                    s16 g = 128;
                    s16 b = 128;
                    ShadowZone::ShadowZones_Calculate_Colour(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos), ypos_int - (idx * field_E6_segment_length), mBaseAnimatedWithPhysicsGameObject_Scale, &r, &g, &b);
                    field_EC_pRes[idx].mRed = static_cast<u8>(r);
                    field_EC_pRes[idx].mGreen = static_cast<u8>(g);
                    field_EC_pRes[idx].mBlue = static_cast<u8>(b);
                    field_EC_pRes[idx].VRender(x_start, y_start + mBaseAnimatedWithPhysicsGameObject_YOffset, ppOt, 0, 0);
                    PSX_RECT rect = {};
                    field_EC_pRes[idx].GetRenderedSize(&rect);
                    pScreenManager->InvalidateRectCurrentIdx(rect.x, rect.y, rect.w, rect.h);
                    ClipPoly_Vertically_4584B0(&field_EC_pRes[idx].field_10_polys[gPsxDisplay.mBufferIndex], mBaseAnimatedWithPhysicsGameObject_YOffset + minY, mBaseAnimatedWithPhysicsGameObject_YOffset + maxY);
                    y_start -= field_E6_segment_length;
                    if (y_start < minY)
                    {
                        break;
                    }
                }
            }
        }
    }
}

} // namespace AO
