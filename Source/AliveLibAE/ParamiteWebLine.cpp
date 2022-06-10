#include "stdafx.h"
#include "ParamiteWebLine.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "PsxDisplay.hpp"
#include "ShadowZone.hpp"
#include "Rope.hpp"

ParamiteWebLine::ParamiteWebLine(Path_ParamiteWebLine* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eWebLine);
    field_100_tlv_info = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::ParamiteWeb);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1u);

    if (pTlv->field_10_scale != Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_RopeWebDrill_Half_5;
        mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = FP_FromDouble(0.7);
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.7);
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
        field_F6_piece_length = 7;
        field_1A2_pulse_position_speed = 1;
        mBaseAnimatedWithPhysicsGameObject_Red = 50;
        mBaseAnimatedWithPhysicsGameObject_Green = 50;
        mBaseAnimatedWithPhysicsGameObject_Blue = 200;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_RopeWebDrill_24;
        mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
        field_F6_piece_length = 15;
        field_1A2_pulse_position_speed = 2;
        mBaseAnimatedWithPhysicsGameObject_Red = 10;
        mBaseAnimatedWithPhysicsGameObject_Green = 10;
        mBaseAnimatedWithPhysicsGameObject_Blue = 10;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(20),
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
            &pLine, &hitX, &hitY, 0x100))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pLine->field_0_rect.x);

        const FP screenTop = pScreenManager->mCamPos->field_4_y;
        if (FP_FromInteger(pLine->field_0_rect.y) >= screenTop)
        {
            field_F8_top = pLine->field_0_rect.y;
        }
        else
        {
            field_F8_top = FP_GetExponent(screenTop);
        }

        const FP screenBottom = pScreenManager->mCamPos->field_4_y + FP_FromInteger(240);
        if (FP_FromInteger(pLine->field_0_rect.h) <= screenBottom)
        {
            field_FA_bottom = pLine->field_0_rect.h;
        }
        else
        {
            field_FA_bottom = FP_GetExponent(screenBottom);
        }
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(field_FA_bottom);
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    field_F4_anim_segment_count = (field_F6_piece_length + field_FA_bottom - field_F8_top) / field_F6_piece_length;
    field_FC_pRes = relive_new AnimationUnknown[field_F4_anim_segment_count];
    for (s32 i = 0; i < field_F4_anim_segment_count; i++)
    {
        field_FC_pRes[i].mAnimFlags.Set(AnimFlags::eBit3_Render);
        field_FC_pRes[i].field_68_anim_ptr = &mBaseAnimatedWithPhysicsGameObject_Anim;
        field_FC_pRes[i].mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
        field_FC_pRes[i].field_6C_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        field_FC_pRes[i].mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_FC_pRes[i].mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
    }

    field_104_wobble_idx = 0;
    field_106_wobble_pos = field_F8_top;

    const AnimRecord& orbRec = AnimRec(AnimId::ChantOrb_Particle);
    u8** ppFlareRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, orbRec.mResourceId, 0, 0);
    if (field_108_anim_flare.Init(orbRec.mFrameTableOffset, gObjList_animations_5C1A24, this, orbRec.mMaxW, orbRec.mMaxH, ppFlareRes, 1u, 0, 0))
    {
        field_108_anim_flare.mRed = 100;
        field_108_anim_flare.mGreen = 100;
        field_108_anim_flare.mBlue = 100;

        field_108_anim_flare.mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
        field_108_anim_flare.field_14_scale = FP_FromDouble(0.3);
        field_108_anim_flare.mRenderMode = TPageAbr::eBlend_1;

        field_108_anim_flare.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_108_anim_flare.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
        field_108_anim_flare.mAnimFlags.Set(AnimFlags::eBit7_SwapXY);

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
            SFX_Play_Mono(static_cast<SoundEffect>(Math_RandomRange(SoundEffect::WebDrop1_103, SoundEffect::WebDrop2_104)), Math_RandomRange(40, 80));
            field_106_wobble_pos = field_F8_top;
            return;
        }
    }
    field_106_wobble_pos = field_F8_top;
}

PSX_RECT ParamiteWebLine::VGetBoundingRect(s32 /*idx*/)
{
    const s16 xpos = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);

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
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->mCamPos->field_0_x),
            FP_GetExponent(FP_FromInteger(field_1A0_pulse_position) - pScreenManager->mCamPos->field_4_y),
            ppOt, 0, 0);
        PSX_RECT rect = {};
        field_108_anim_flare.Get_Frame_Rect(&rect);
        pScreenManager->InvalidateRect(rect.x, rect.y, rect.w, rect.h, pScreenManager->mIdx);
    }

    mBaseAnimatedWithPhysicsGameObject_Anim.VRender(640, 240, ppOt, 0, 0);

    s32 idx = 0;
    s16 render_ypos = field_FA_bottom;
    while (render_ypos > field_F8_top)
    {
        s16 r = mBaseAnimatedWithPhysicsGameObject_Red;
        s16 g = mBaseAnimatedWithPhysicsGameObject_Green;
        s16 b = mBaseAnimatedWithPhysicsGameObject_Blue;

        FP xVal = mBaseAnimatedWithPhysicsGameObject_XPos;
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
                xVal = mBaseAnimatedWithPhysicsGameObject_XPos - (Math_Cosine_496D60(ang / bottomDiff) * FP_FromInteger(word_563A8C[field_104_wobble_idx]));
            }
            else
            {
                if (field_106_wobble_pos == field_F8_top)
                {
                    break;
                }
                const FP topDiff = FP_FromInteger(field_106_wobble_pos - field_F8_top);
                const FP ang = FP_FromInteger(field_106_wobble_pos - render_ypos) * FP_FromInteger(128);
                xVal = (Math_Cosine_496D60(ang / topDiff) * FP_FromInteger(word_563A8C[field_104_wobble_idx])) + mBaseAnimatedWithPhysicsGameObject_XPos;
            }
        }

        const s16 render_xpos = FP_GetExponent(xVal);
        ShadowZone::ShadowZones_Calculate_Colour(render_xpos, render_ypos, mBaseAnimatedWithPhysicsGameObject_Scale, &r, &g, &b);
        field_FC_pRes[idx].mRed = static_cast<u8>(r);
        field_FC_pRes[idx].mGreen = static_cast<u8>(g);
        field_FC_pRes[idx].mBlue = static_cast<u8>(b);

        field_FC_pRes[idx].VRender(
            FP_GetExponent(FP_FromInteger(render_xpos) - pScreenManager->mCamPos->field_0_x),
            FP_GetExponent(FP_FromInteger(render_ypos) - pScreenManager->mCamPos->field_4_y),
            ppOt, 0, 0);

        PSX_RECT rect = {};
        field_FC_pRes[idx].GetRenderedSize(&rect);
        pScreenManager->InvalidateRect(rect.x, rect.y, rect.w, rect.h, pScreenManager->mIdx);

        ClipPoly_Vertically_4A09E0(
            &field_FC_pRes[idx].field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index],
            FP_GetExponent(FP_FromInteger(field_F8_top) - pScreenManager->mCamPos->field_4_y),
            FP_GetExponent(FP_FromInteger(field_FA_bottom) - pScreenManager->mCamPos->field_4_y));

        render_ypos -= field_F6_piece_length;
        idx++;
    }
}

void ParamiteWebLine::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
