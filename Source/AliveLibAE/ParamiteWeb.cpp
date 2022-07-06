#include "stdafx.h"
#include "ParamiteWeb.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Rope.hpp"
#include "ShadowZone.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"

ParamiteWeb::ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eLiftRope);

    if (scale == FP_FromInteger(1))
    {
        field_F6_segment_length = 15;
    }
    else
    {
        field_F6_segment_length = 7;
    }

    const AnimRecord& rec = AnimRec(AnimId::ParamiteWeb);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::ParamiteWeb, ppRes);

    if (scale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_24;
        mScale = Scale::Fg;
        mAnim.field_14_scale = FP_FromInteger(1);
        mSpriteScale = FP_FromInteger(1);
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_Half_5;
        mAnim.field_14_scale = FP_FromDouble(0.7);
        mSpriteScale = FP_FromDouble(0.7);
        mScale = Scale::Bg;
        xpos += FP_FromInteger(2);
    }

    mAnim.mRed = 128;
    mAnim.mGreen = 128;
    mAnim.mBlue = 128;

    mXPos = xpos;
    field_FA_ttl_remainder = static_cast<s16>(top);
    mYPos = FP_FromInteger(top);
    field_F8_ttl = static_cast<s16>(bottom);

    field_F4_number_of_segments = 240 / field_F6_segment_length;

    field_FC_ppRes = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::Resource_Web, 0, sizeof(AnimationUnknown) * (field_F4_number_of_segments));
    field_100_pRes = reinterpret_cast<AnimationUnknown*>(*field_FC_ppRes);

    for (s32 i = 0; i < field_F4_number_of_segments; i++)
    {
        AnimationUnknown* pSegment = &field_100_pRes[i];
        pSegment = new (pSegment) AnimationUnknown(); // We have memory but no constructor was called.. so use placement new to get a constructed instance
        pSegment->mFlags.Set(AnimFlags::eBit3_Render);
        pSegment->field_68_anim_ptr = &mAnim;
        pSegment->mRenderLayer = mAnim.mRenderLayer;
        pSegment->field_6C_scale = mSpriteScale;
        pSegment->mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        pSegment->mFlags.Clear(AnimFlags::eBit16_bBlending);
    }

    field_104_bEnabled = 0;
}

ParamiteWeb::~ParamiteWeb()
{
    ResourceManager::FreeResource_49C330(field_FC_ppRes);
}

void ParamiteWeb::VUpdate()
{
    if (field_104_bEnabled == 1)
    {
        field_FA_ttl_remainder -= 8;
        if (field_FA_ttl_remainder <= field_F8_ttl)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void ParamiteWeb::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ParamiteWeb::VRender(PrimHeader** ppOt)
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);
    if (mCurrentLevel == gMap.mCurrentLevel && mCurrentPath == gMap.mCurrentPath)
    {
        if (mXPos >= FP_FromInteger(camCoords.x) && mXPos <= FP_FromInteger(camCoords.x + 1024))
        {
            const FP cam_y = pScreenManager->CamYPos();
            const FP cam_x = pScreenManager->CamXPos();

            s16 minY = FP_GetExponent(FP_FromInteger(field_F8_ttl) - cam_y);
            s16 maxY = FP_GetExponent(FP_FromInteger(field_FA_ttl_remainder) - cam_y);

            s16 ypos_int = FP_GetExponent(mYPos);
            if (ypos_int > field_FA_ttl_remainder)
            {
                ypos_int = field_FA_ttl_remainder + (ypos_int - field_FA_ttl_remainder) % field_F6_segment_length;
            }

            const s16 x_start = FP_GetExponent(mXPos - cam_x);

            s16 y_start = FP_GetExponent((FP_FromInteger(ypos_int)) - cam_y);
            if (y_start > 240)
            {
                y_start = y_start % field_F6_segment_length + 240;
                ypos_int = FP_GetExponent(cam_y + FP_FromInteger(y_start));
            }

            if (minY < 0)
            {
                minY = 0;
            }

            if (maxY > 240)
            {
                maxY = 240;
            }

            mAnim.VRender(640, 240, ppOt, 0, 0);

            if (y_start >= minY)
            {
                for (s32 idx = 0; idx < field_F4_number_of_segments; ++idx)
                {
                    s16 r = 128;
                    s16 g = 128;
                    s16 b = 128;
                    ShadowZone::ShadowZones_Calculate_Colour(FP_GetExponent(mXPos), ypos_int - (idx * field_F6_segment_length), mScale, &r, &g, &b);
                    field_100_pRes[idx].mRed = static_cast<u8>(r);
                    field_100_pRes[idx].mGreen = static_cast<u8>(g);
                    field_100_pRes[idx].mBlue = static_cast<u8>(b);
                    field_100_pRes[idx].VRender(x_start, y_start, ppOt, 0, 0);
                    PSX_RECT rect = {};
                    field_100_pRes[idx].GetRenderedSize(&rect);
                    pScreenManager->InvalidateRectCurrentIdx(rect.x, rect.y, rect.w, rect.h);
                    ClipPoly_Vertically_4A09E0(&field_100_pRes[idx].field_10_polys[gPsxDisplay.mBufferIndex], minY, maxY);
                    y_start -= field_F6_segment_length;
                    if (y_start < minY)
                    {
                        break;
                    }
                }
            }
        }
    }
}
