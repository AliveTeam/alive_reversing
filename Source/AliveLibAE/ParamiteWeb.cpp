#include "stdafx.h"
#include "ParamiteWeb.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Rope.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

ParamiteWeb::ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eRope);

    if (scale == FP_FromInteger(1))
    {
        field_F6_segment_length = 15;
    }
    else
    {
        field_F6_segment_length = 7;
    }

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ParamiteWeb));
    Animation_Init(GetAnimRes(AnimId::ParamiteWeb));

    if (scale == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_24);
        SetScale(Scale::Fg);
        GetAnimation().SetSpriteScale(FP_FromInteger(1));
        SetSpriteScale(FP_FromInteger(1));
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_Half_5);
        GetAnimation().SetSpriteScale(FP_FromDouble(0.7));
        SetSpriteScale(FP_FromDouble(0.7));
        SetScale(Scale::Bg);
        xpos += FP_FromInteger(2);
    }

    GetAnimation().SetRGB(128, 128, 128);

    mXPos = xpos;
    field_FA_ttl_remainder = static_cast<s16>(top);
    mYPos = FP_FromInteger(top);
    field_F8_ttl = static_cast<s16>(bottom);

    field_F4_number_of_segments = 240 / field_F6_segment_length;

    field_100_pRes = relive_new AnimationUnknown[field_F4_number_of_segments];
    if (field_100_pRes)
    {
        for (s32 i = 0; i < field_F4_number_of_segments; i++)
        {
            AnimationUnknown* pSegment = &field_100_pRes[i];
            pSegment->mFlags.Set(AnimFlags::eRender);
            pSegment->field_68_anim_ptr = &GetAnimation();
            pSegment->SetRenderLayer(GetAnimation().GetRenderLayer());
            pSegment->field_6C_scale = GetSpriteScale();
            pSegment->mFlags.Clear(AnimFlags::eSemiTrans);
            pSegment->mFlags.Clear(AnimFlags::eBlending);
        }
    }

    field_104_bEnabled = 0;
}

ParamiteWeb::~ParamiteWeb()
{
    relive_delete[] field_100_pRes;
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
    if (gMap.LevelChanged() || gMap.PathChanged())
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

            GetAnimation().VRender(640, 240, ppOt, 0, 0);

            if (y_start >= minY)
            {
                for (s32 idx = 0; idx < field_F4_number_of_segments; ++idx)
                {
                    s16 r = 128;
                    s16 g = 128;
                    s16 b = 128;
                    ShadowZone::ShadowZones_Calculate_Colour(FP_GetExponent(mXPos), ypos_int - (idx * field_F6_segment_length), GetScale(), &r, &g, &b);
                    field_100_pRes[idx].SetRGB(r, g, b);
                    field_100_pRes[idx].VRender(x_start, y_start, ppOt, 0, 0);
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
