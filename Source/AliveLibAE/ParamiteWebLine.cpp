#include "stdafx.h"
#include "ParamiteWebLine.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "Rope.hpp"
#include "Path.hpp"
#include "../relive_lib/AnimationUnknown.hpp"
#include "../relive_lib/PsxDisplay.hpp"

void ParamiteWebLine::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle));
}

ParamiteWebLine::ParamiteWebLine(relive::Path_ParamiteWebLine* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eWebLine);
    field_100_tlv_info = tlvId;

    LoadAnimations();

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ParamiteWeb));
    Animation_Init(GetAnimRes(AnimId::ParamiteWeb));

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_Half_5);
        GetAnimation().SetSpriteScale(FP_FromDouble(0.7));
        SetSpriteScale(FP_FromDouble(0.7));
        SetScale(Scale::Bg);
        field_F6_piece_length = 7;
        field_1A2_pulse_position_speed = 1;
        mRGB.SetRGB(50, 50, 200);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_24);
        GetAnimation().SetSpriteScale(FP_FromInteger(1));
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
        field_F6_piece_length = 15;
        field_1A2_pulse_position_speed = 2;
        mRGB.SetRGB(10, 10, 10);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos + FP_FromInteger(20),
            mYPos + FP_FromInteger(20),
            &pLine, &hitX, &hitY, CollisionMask(eTrackLine_8)))
    {
        mXPos = FP_FromInteger(pLine->mRect.x);

        const FP screenTop = gScreenManager->CamYPos();
        if (FP_FromInteger(pLine->mRect.y) >= screenTop)
        {
            field_F8_top = pLine->mRect.y;
        }
        else
        {
            field_F8_top = FP_GetExponent(screenTop);
        }

        const FP screenBottom = gScreenManager->CamYPos() + FP_FromInteger(240);
        if (FP_FromInteger(pLine->mRect.h) <= screenBottom)
        {
            field_FA_bottom = pLine->mRect.h;
        }
        else
        {
            field_FA_bottom = FP_GetExponent(screenBottom);
        }
        mYPos = FP_FromInteger(field_FA_bottom);
    }
    else
    {
        SetDead(true);
    }

    field_F4_anim_segment_count = (field_F6_piece_length + field_FA_bottom - field_F8_top) / field_F6_piece_length;
    field_FC_pRes = relive_new AnimationUnknown[field_F4_anim_segment_count];

    if (field_FC_pRes)
    {
        for (s32 i = 0; i < field_F4_anim_segment_count; i++)
        {
            field_FC_pRes[i].SetRender(true);
            field_FC_pRes[i].mAnimPtr = &GetAnimation();
            field_FC_pRes[i].SetRenderLayer(GetAnimation().GetRenderLayer());
            field_FC_pRes[i].mSpriteScale = GetSpriteScale();
            field_FC_pRes[i].SetSemiTrans(false);
            field_FC_pRes[i].SetBlending(false);
        }
    }

    field_104_wobble_idx = 0;
    field_106_wobble_pos = field_F8_top;

    if (field_108_anim_flare.Init(GetAnimRes(AnimId::ChantOrb_Particle), this))
    {
        field_108_anim_flare.SetRGB(100, 100, 100);

        field_108_anim_flare.SetRenderLayer(GetAnimation().GetRenderLayer());
        field_108_anim_flare.SetSpriteScale(FP_FromDouble(0.3));
        field_108_anim_flare.SetBlendMode(relive::TBlendModes::eBlend_1);

        field_108_anim_flare.SetSemiTrans(false);
        field_108_anim_flare.SetBlending(false);
        field_108_anim_flare.SetSwapXY(true);

        field_1A0_pulse_position = field_F8_top;
        field_1A4_delay_counter = Math_RandomRange(0, 10);
    }
    else
    {
        SetListAddFailed(true);
    }
}

void ParamiteWebLine::Wobble(s16 ypos)
{
    s16 yPosToUse = ypos;
    if (ypos < field_F8_top)
    {
        yPosToUse = field_F8_top;
    }

    field_104_wobble_idx = 7;

    if (yPosToUse <= field_FA_bottom)
    {
        field_106_wobble_pos = yPosToUse;
    }
    else
    {
        field_106_wobble_pos = field_FA_bottom;
    }
}

ParamiteWebLine::~ParamiteWebLine()
{
    relive_delete[] field_FC_pRes;
    field_108_anim_flare.VCleanUp();
    Path::TLV_Reset(field_100_tlv_info, -1, 0, 0);
}

void ParamiteWebLine::VUpdate()
{
    if (field_104_wobble_idx > 0)
    {
        field_104_wobble_idx--;
        field_1A0_pulse_position = field_F8_top;
        return;
    }

    if (field_1A4_delay_counter > 0)
    {
        field_1A4_delay_counter--;
    }
    else
    {
        field_1A0_pulse_position += field_1A2_pulse_position_speed;
        if (field_1A0_pulse_position > field_FA_bottom)
        {
            field_1A0_pulse_position = field_F8_top;
            SfxPlayMono(relive::RandomSfx(relive::SoundEffects::WebDrop1, relive::SoundEffects::WebDrop2), Math_RandomRange(40, 80));
            field_106_wobble_pos = field_F8_top;
            return;
        }
    }
    field_106_wobble_pos = field_F8_top;
}

PSX_RECT ParamiteWebLine::VGetBoundingRect()
{
    const s16 xpos = FP_GetExponent(mXPos);

    PSX_RECT r;
    r.x = xpos - 2;
    r.y = field_F8_top;

    r.w = xpos + 2;
    r.h = field_FA_bottom;

    return r;
}

const s16 word_563A8C[10] = {0, 1, -3, 2, -4, 4, -6, 4, 0, 0};

void ParamiteWebLine::VRender(BasePrimitive** ppOt)
{
    if (!field_104_wobble_idx && !field_1A4_delay_counter)
    {
        field_108_anim_flare.VRender(
            FP_GetExponent(mXPos - gScreenManager->CamXPos()),
            FP_GetExponent(FP_FromInteger(field_1A0_pulse_position) - gScreenManager->CamYPos()),
            ppOt, 0, 0);
    }

    GetAnimation().VRender(640, 240, ppOt, 0, 0);

    s32 idx = 0;
    s16 render_ypos = field_FA_bottom;
    while (render_ypos > field_F8_top)
    {
        s16 r = mRGB.r;
        s16 g = mRGB.g;
        s16 b = mRGB.b;

        FP xVal = mXPos;
        if (field_104_wobble_idx != 0)
        {
            if (render_ypos >= field_106_wobble_pos)
            {
                if (field_106_wobble_pos == field_FA_bottom)
                {
                    break;
                }
                const FP bottomDiff = FP_FromInteger(field_FA_bottom - field_106_wobble_pos);
                const FP ang = FP_FromInteger(field_FA_bottom - render_ypos) * FP_FromInteger(128);
                xVal = mXPos - (Math_Cosine(ang / bottomDiff) * FP_FromInteger(word_563A8C[field_104_wobble_idx]));
            }
            else
            {
                if (field_106_wobble_pos == field_F8_top)
                {
                    break;
                }
                const FP topDiff = FP_FromInteger(field_106_wobble_pos - field_F8_top);
                const FP ang = FP_FromInteger(field_106_wobble_pos - render_ypos) * FP_FromInteger(128);
                xVal = (Math_Cosine(ang / topDiff) * FP_FromInteger(word_563A8C[field_104_wobble_idx])) + mXPos;
            }
        }

        const s16 render_xpos = FP_GetExponent(xVal);
        ShadowZone::ShadowZones_Calculate_Colour(render_xpos, render_ypos, GetScale(), &r, &g, &b);
        field_FC_pRes[idx].SetRGB(r, g, b);

        field_FC_pRes[idx].VRender(
            FP_GetExponent(FP_FromInteger(render_xpos) - gScreenManager->CamXPos()),
            FP_GetExponent(FP_FromInteger(render_ypos) - gScreenManager->CamYPos()),
            ppOt, 0, 0);

        ClipPoly_Vertically(
            &field_FC_pRes[idx].mPolys[gPsxDisplay.mBufferIndex],
            FP_GetExponent(FP_FromInteger(field_F8_top) - gScreenManager->CamYPos()),
            FP_GetExponent(FP_FromInteger(field_FA_bottom) - gScreenManager->CamYPos()));

        render_ypos -= field_F6_piece_length;
        idx++;
    }
}

void ParamiteWebLine::VScreenChanged()
{
    SetDead(true);
}
