#include "stdafx_ao.h"
#include "Function.hpp"
#include "ParamiteWeb.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Rope.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "../relive_lib/AnimationUnknown.hpp"

namespace AO {

ParamiteWeb::ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eRope);

    mYOffset = 0;

    if (scale == FP_FromInteger(1))
    {
        field_E6_segment_length = 15;
    }
    else
    {
        field_E6_segment_length = 7;
    }

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ParamiteWeb));
    Animation_Init(GetAnimRes(AnimId::ParamiteWeb));

    if (scale == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_24);
        SetScale(Scale::Fg);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_Half_5);
        SetScale(Scale::Bg);
    }

    GetAnimation().SetSpriteScale(scale);
    SetSpriteScale(scale);

    GetAnimation().SetRGB(128, 128, 128);

    mXPos = xpos;
    field_EA_ttl_remainder = static_cast<s16>(top);
    mYPos = FP_FromInteger(top);
    field_E8_ttl = static_cast<s16>(bottom);

    field_E4_number_of_segments = 240 / field_E6_segment_length;

    field_EC_pRes = relive_new AnimationUnknown[field_E4_number_of_segments];
    if (field_EC_pRes)
    {
        for (s32 i = 0; i < field_E4_number_of_segments; i++)
        {
            AnimationUnknown* pSegment = &field_EC_pRes[i];
            pSegment->SetRender(true);
            pSegment->field_68_anim_ptr = &GetAnimation();
            pSegment->SetRenderLayer(GetAnimation().GetRenderLayer());
            pSegment->field_6C_scale = GetSpriteScale();
            pSegment->SetSemiTrans(false);
            pSegment->SetBlending(false);
        }
    }

    field_F0_bEnabled = 0;
}

ParamiteWeb::~ParamiteWeb()
{
    relive_delete[] field_EC_pRes;
}
void ParamiteWeb::VUpdate()
{
    if (field_F0_bEnabled == 1)
    {
        field_EA_ttl_remainder -= 8;
        if (field_EA_ttl_remainder <= field_E8_ttl)
        {
            SetDead(true);
        }
    }
}

void ParamiteWeb::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

//TODO @ 100% - merge with Rope::vRender()
void ParamiteWeb::VRender(PrimHeader** ppOt)
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);
    if (mCurrentLevel == gMap.mCurrentLevel && mCurrentPath == gMap.mCurrentPath)
    {
        if (mXPos >= FP_FromInteger(camCoords.x) && mXPos <= FP_FromInteger(camCoords.x + 1024))
        {
            const FP cam_y = pScreenManager->mCamPos->y;
            const FP cam_x = pScreenManager->mCamPos->x;

            s16 minY = FP_GetExponent(FP_FromInteger(pScreenManager->mCamYOff + field_E8_ttl) - cam_y);
            s16 maxY = FP_GetExponent(FP_FromInteger(pScreenManager->mCamYOff + field_EA_ttl_remainder) - cam_y);

            s16 ypos_int = FP_GetExponent(mYPos);
            if (ypos_int > field_EA_ttl_remainder)
            {
                ypos_int = field_EA_ttl_remainder + (ypos_int - field_EA_ttl_remainder) % field_E6_segment_length;
            }

            const s16 x_start = PsxToPCX<s16>(FP_GetExponent(mXPos + FP_FromInteger(pScreenManager->mCamXOff) - cam_x));

            s16 y_start = FP_GetExponent(FP_FromInteger(pScreenManager->mCamYOff + ypos_int) - cam_y);
            if (mYOffset + y_start > 240)
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

            GetAnimation().VRender(640, 240, ppOt, 0, 0);

            if (y_start >= minY)
            {
                for (s32 idx = 0; idx < field_E4_number_of_segments; ++idx)
                {
                    s16 r = 128;
                    s16 g = 128;
                    s16 b = 128;
                    ShadowZone::ShadowZones_Calculate_Colour(FP_GetExponent(mXPos), ypos_int - (idx * field_E6_segment_length), GetScale(), &r, &g, &b);
                    field_EC_pRes[idx].SetRGB(r, g, b);
                    field_EC_pRes[idx].VRender(x_start, y_start + mYOffset, ppOt, 0, 0);
                    ClipPoly_Vertically_4584B0(&field_EC_pRes[idx].field_10_polys[gPsxDisplay.mBufferIndex], mYOffset + minY, mYOffset + maxY);
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
