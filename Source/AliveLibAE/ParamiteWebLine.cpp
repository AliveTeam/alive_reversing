#include "stdafx.h"
#include "ParamiteWebLine.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "Rope.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

ParamiteWebLine::ParamiteWebLine(relive::Path_ParamiteWebLine* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eWebLine);
    field_100_tlv_info = tlvId;

    const AnimRecord& rec = AnimRec(AnimId::ParamiteWeb);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::ParamiteWeb, ppRes);

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        mAnim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_Half_5;
        mAnim.field_14_scale = FP_FromDouble(0.7);
        mSpriteScale = FP_FromDouble(0.7);
        mScale = Scale::Bg;
        field_F6_piece_length = 7;
        field_1A2_pulse_position_speed = 1;
        mRGB.SetRGB(50, 50, 200);
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_24;
        mAnim.field_14_scale = FP_FromInteger(1);
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
        field_F6_piece_length = 15;
        field_1A2_pulse_position_speed = 2;
        mRGB.SetRGB(10, 10, 10);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos + FP_FromInteger(20),
            mYPos + FP_FromInteger(20),
            &pLine, &hitX, &hitY, CollisionMask(eTrackLine_8)))
    {
        mXPos = FP_FromInteger(pLine->mRect.x);

        const FP screenTop = pScreenManager->CamYPos();
        if (FP_FromInteger(pLine->mRect.y) >= screenTop)
        {
            field_F8_top = pLine->mRect.y;
        }
        else
        {
            field_F8_top = FP_GetExponent(screenTop);
        }

        const FP screenBottom = pScreenManager->CamYPos() + FP_FromInteger(240);
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    field_F4_anim_segment_count = (field_F6_piece_length + field_FA_bottom - field_F8_top) / field_F6_piece_length;
    field_FC_pRes = relive_new AnimationUnknown[field_F4_anim_segment_count];
    for (s32 i = 0; i < field_F4_anim_segment_count; i++)
    {
        field_FC_pRes[i].mFlags.Set(AnimFlags::eBit3_Render);
        field_FC_pRes[i].field_68_anim_ptr = &mAnim;
        field_FC_pRes[i].mRenderLayer = mAnim.mRenderLayer;
        field_FC_pRes[i].field_6C_scale = mSpriteScale;
        field_FC_pRes[i].mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_FC_pRes[i].mFlags.Clear(AnimFlags::eBit16_bBlending);
    }

    field_104_wobble_idx = 0;
    field_106_wobble_pos = field_F8_top;

    const AnimRecord& orbRec = AnimRec(AnimId::ChantOrb_Particle);
    u8** ppFlareRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, orbRec.mResourceId, 0, 0);
    if (field_108_anim_flare.Init(AnimId::ChantOrb_Particle, this, ppFlareRes))
    {
        field_108_anim_flare.mRed = 100;
        field_108_anim_flare.mGreen = 100;
        field_108_anim_flare.mBlue = 100;

        field_108_anim_flare.mRenderLayer = mAnim.mRenderLayer;
        field_108_anim_flare.field_14_scale = FP_FromDouble(0.3);
        field_108_anim_flare.mRenderMode = TPageAbr::eBlend_1;

        field_108_anim_flare.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_108_anim_flare.mFlags.Clear(AnimFlags::eBit16_bBlending);
        field_108_anim_flare.mFlags.Set(AnimFlags::eBit7_SwapXY);

        field_1A0_pulse_position = field_F8_top;
        field_1A4_delay_counter = Math_RandomRange(0, 10);
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
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

PSX_RECT ParamiteWebLine::VGetBoundingRect(s32 /*idx*/)
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

void ParamiteWebLine::VRender(PrimHeader** ppOt)
{
    if (!field_104_wobble_idx && !field_1A4_delay_counter)
    {
        field_108_anim_flare.VRender(
            FP_GetExponent(mXPos - pScreenManager->CamXPos()),
            FP_GetExponent(FP_FromInteger(field_1A0_pulse_position) - pScreenManager->CamYPos()),
            ppOt, 0, 0);
        PSX_RECT rect = {};
        field_108_anim_flare.Get_Frame_Rect(&rect);
        pScreenManager->InvalidateRectCurrentIdx(rect.x, rect.y, rect.w, rect.h);
    }

    mAnim.VRender(640, 240, ppOt, 0, 0);

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
                xVal = mXPos - (Math_Cosine_496D60(ang / bottomDiff) * FP_FromInteger(word_563A8C[field_104_wobble_idx]));
            }
            else
            {
                if (field_106_wobble_pos == field_F8_top)
                {
                    break;
                }
                const FP topDiff = FP_FromInteger(field_106_wobble_pos - field_F8_top);
                const FP ang = FP_FromInteger(field_106_wobble_pos - render_ypos) * FP_FromInteger(128);
                xVal = (Math_Cosine_496D60(ang / topDiff) * FP_FromInteger(word_563A8C[field_104_wobble_idx])) + mXPos;
            }
        }

        const s16 render_xpos = FP_GetExponent(xVal);
        ShadowZone::ShadowZones_Calculate_Colour(render_xpos, render_ypos, mScale, &r, &g, &b);
        field_FC_pRes[idx].mRed = static_cast<u8>(r);
        field_FC_pRes[idx].mGreen = static_cast<u8>(g);
        field_FC_pRes[idx].mBlue = static_cast<u8>(b);

        field_FC_pRes[idx].VRender(
            FP_GetExponent(FP_FromInteger(render_xpos) - pScreenManager->CamXPos()),
            FP_GetExponent(FP_FromInteger(render_ypos) - pScreenManager->CamYPos()),
            ppOt, 0, 0);

        PSX_RECT rect = {};
        field_FC_pRes[idx].GetRenderedSize(&rect);
        pScreenManager->InvalidateRectCurrentIdx(rect.x, rect.y, rect.w, rect.h);

        ClipPoly_Vertically_4A09E0(
            &field_FC_pRes[idx].field_10_polys[gPsxDisplay.mBufferIndex],
            FP_GetExponent(FP_FromInteger(field_F8_top) - pScreenManager->CamYPos()),
            FP_GetExponent(FP_FromInteger(field_FA_bottom) - pScreenManager->CamYPos()));

        render_ypos -= field_F6_piece_length;
        idx++;
    }
}

void ParamiteWebLine::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
