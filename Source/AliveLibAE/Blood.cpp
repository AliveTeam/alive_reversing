#include "stdafx.h"
#include "Blood.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "../relive_lib/PsxDisplay.hpp"

Blood::Blood(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, s32 count)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mSpriteScale = scale;

    const AnimRecord& rec = AnimRec(AnimId::BloodDrop);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::BloodDrop, ppRes);

    mAnim.mFlags.Set(AnimFlags::eBit25_bDecompressDone);
    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    mAnim.mRed = 127;
    mAnim.mGreen = 127;
    mAnim.mBlue = 127;

    if (mSpriteScale == FP_FromInteger(1))
    {
        field_12C_render_layer = Layer::eLayer_Foreground_36;
    }
    else
    {
        field_12C_render_layer = Layer::eLayer_Foreground_Half_17;
    }

    field_126_total_count = static_cast<s16>(count);
    field_122_to_render_count = static_cast<s16>(count);

    field_F4_ppResBuf = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::Resource_Blood, 0, count * sizeof(BloodParticle));
    if (field_F4_ppResBuf)
    {
        field_F8_pResBuf = reinterpret_cast<BloodParticle*>(*field_F4_ppResBuf);
        field_128_timer = 0;

        mXPos = xpos - FP_FromInteger(12);
        mYPos = ypos - FP_FromInteger(12);

        field_11E_xpos = FP_GetExponent(xpos - FP_FromInteger(12) - pScreenManager->CamXPos());
        field_120_ypos = FP_GetExponent(ypos - FP_FromInteger(12) - pScreenManager->CamYPos());

        if (mAnim.mFlags.Get(AnimFlags::eBit13_Is8Bit))
        {
            field_11C_texture_mode = TPageMode::e8Bit_1;
        }
        else if (mAnim.mFlags.Get(AnimFlags::eBit14_Is16Bit))
        {
            field_11C_texture_mode = TPageMode::e16Bit_2;
        }
        else
        {
            // 4 bit
            field_11C_texture_mode = TPageMode::e4Bit_0;
        }

        u8 u0 = mAnim.mVramRect.x & 63;
        if (field_11C_texture_mode == TPageMode::e8Bit_1)
        {
            u0 = 2 * u0;
        }
        else if (field_11C_texture_mode == TPageMode::e4Bit_0)
        {
            u0 = 4 * u0;
        }

        u8 v0 = mAnim.mVramRect.y & 0xFF;

        FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*mAnim.field_20_ppBlock)[mAnim.Get_FrameHeader(-1)->field_0_frame_header_offset]);

        const s16 frameW = pFrameHeader->field_4_width;
        const s16 frameH = pFrameHeader->field_5_height;

        mAnim.mFlags.Set(AnimFlags::eBit16_bBlending);

        for (s32 i = 0; i < field_126_total_count; i++)
        {
            for (s32 j = 0; j < 2; j++)
            {
                BloodParticle* pParticle = &field_F8_pResBuf[i];
                Prim_Sprt* pSprt = &pParticle->field_10_prims[j];
                Sprt_Init(pSprt);
                Poly_Set_SemiTrans(&pSprt->mBase.header, 1);

                if (mAnim.mFlags.Get(AnimFlags::eBit16_bBlending))
                {
                    Poly_Set_Blending(&pSprt->mBase.header, 1);
                }
                else
                {
                    Poly_Set_Blending(&pSprt->mBase.header, 0);
                    SetRGB0(pSprt, mAnim.mRed, mAnim.mGreen, mAnim.mBlue);
                }

                SetClut(pSprt,
                        static_cast<s16>(
                            PSX_getClut(
                                mAnim.mPalVramXY.x,
                                mAnim.mPalVramXY.y)));

                SetUV0(pSprt, u0, v0);
                pSprt->field_14_w = frameW - 1;
                pSprt->field_16_h = frameH - 1;
            }
        }

        // Has its own random seed based on the frame counter.. no idea why
        field_124_rand_seed = static_cast<u8>(sGnFrame);
        for (s32 i = 0; i < field_122_to_render_count; i++)
        {
            field_F8_pResBuf[i].x = FP_FromInteger(field_11E_xpos);
            field_F8_pResBuf[i].y = FP_FromInteger(field_120_ypos);

            const FP randX = FP_FromInteger(sRandomBytes_546744[field_124_rand_seed++]) / FP_FromInteger(16);
            const FP adjustedX = FP_FromDouble(1.3) * (randX - FP_FromInteger(8));
            field_F8_pResBuf[i].field_8_offx = mSpriteScale * (xOff + adjustedX);

            const FP randY = FP_FromInteger(sRandomBytes_546744[field_124_rand_seed++]) / FP_FromInteger(16);
            const FP adjustedY = FP_FromDouble(1.3) * (randY - FP_FromInteger(8));
            field_F8_pResBuf[i].field_C_offy = mSpriteScale * (yOff + adjustedY);
        }
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

Blood::~Blood()
{
    if (field_F4_ppResBuf)
    {
        ResourceManager::FreeResource_49C330(field_F4_ppResBuf);
    }
}

void Blood::VUpdate()
{
    if (field_128_timer > 0)
    {
        if (field_128_timer > 5)
        {
            field_122_to_render_count -= 10;
        }

        if (field_122_to_render_count <= 0)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            field_122_to_render_count = 0;
            return;
        }

        for (s32 i = 0; i < field_122_to_render_count; i++)
        {
            field_F8_pResBuf[i].field_C_offy += FP_FromDouble(1.8);

            field_F8_pResBuf[i].field_8_offx = field_F8_pResBuf[i].field_8_offx * FP_FromDouble(0.9);
            field_F8_pResBuf[i].field_C_offy = field_F8_pResBuf[i].field_C_offy * FP_FromDouble(0.9);

            field_F8_pResBuf[i].x += field_F8_pResBuf[i].field_8_offx;
            field_F8_pResBuf[i].y += field_F8_pResBuf[i].field_C_offy;
        }
    }

    field_128_timer++;
}

void Blood::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        PSX_Point xy = {32767, 32767};
        PSX_Point wh = {-32767, -32767};

        for (s32 i = 0; i < field_122_to_render_count; i++)
        {
            BloodParticle* pParticle = &field_F8_pResBuf[i];
            Prim_Sprt* pSprt = &pParticle->field_10_prims[gPsxDisplay.mBufferIndex];

            u8 u0 = mAnim.mVramRect.x & 63;
            if (field_11C_texture_mode == TPageMode::e8Bit_1)
            {
                u0 *= 2;
            }
            else if (field_11C_texture_mode == TPageMode::e4Bit_0)
            {
                u0 *= 4;
            }

            SetUV0(pSprt, u0, static_cast<u8>(mAnim.mVramRect.y));

            FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(
                &(*mAnim.field_20_ppBlock)[mAnim.Get_FrameHeader(-1)->field_0_frame_header_offset]);

            pSprt->field_14_w = pFrameHeader->field_4_width - 1;
            pSprt->field_16_h = pFrameHeader->field_5_height - 1;

            const s16 x0 = PsxToPCX(FP_GetExponent(pParticle->x));
            const s16 y0 = FP_GetExponent(pParticle->y);

            SetXY0(pSprt, x0, y0);

            if (!mAnim.mFlags.Get(AnimFlags::eBit16_bBlending))
            {
                SetRGB0(pSprt, mAnim.mRed, mAnim.mGreen, mAnim.mBlue);
            }

            OrderingTable_Add(OtLayer(ppOt, field_12C_render_layer), &pSprt->mBase.header);

            xy.x = std::min(x0, xy.x);
            xy.y = std::min(y0, xy.y);

            wh.x = std::max(x0, wh.x);
            wh.y = std::max(y0, wh.y);
        }

        const s32 tpage = PSX_getTPage(
            field_11C_texture_mode,
            TPageAbr::eBlend_0,
            mAnim.mVramRect.x,
            mAnim.mVramRect.y);

        Prim_SetTPage* pTPage = &field_FC_tPages[gPsxDisplay.mBufferIndex];
        Init_SetTPage(pTPage, 0, 0, static_cast<s16>(tpage));
        OrderingTable_Add(OtLayer(ppOt, field_12C_render_layer), &pTPage->mBase);

        pScreenManager->InvalidateRectCurrentIdx(
            (xy.x - 12),
            (xy.y - 12),
            (wh.x + 12),
            (wh.y + 12));
    }
}

void Blood::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
