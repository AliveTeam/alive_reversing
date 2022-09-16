#include "stdafx.h"
#include "Animation.hpp"
#include "../AliveLibAE/Compression.hpp"
#include "../relive_lib/Compression.hpp"
#include "VRam.hpp"
#include "PsxDisplay.hpp"
#include "../AliveLibCommon/Sys_common.hpp"
#include "../AliveLibAE/Renderer/IRenderer.hpp"
#include "../relive_lib/GameType.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"
#include "../AliveLibCommon/AnimResources.hpp"

const AnimRecord PerGameAnimRec(AnimId id)
{
    if (GetGameType() == GameType::eAe)
    {
        return AnimRec(id);
    }
    else
    {
        return AO::AnimRec(id);
    }
}

void Animation::DecompressFrame()
{
    const PerFrameInfo* pFrameInfoHeader = Get_FrameHeader(-1); // -1 = use current frame
    if (pFrameInfoHeader->mPointCount > 0)
    {
        Invoke_CallBacks();
    }

    // TODO
    //UploadTexture(pFrameHeader, vram_rect, width_bpp_adjusted);
}

void Animation::VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s32 height)
{
    const s16 xpos_pc = static_cast<s16>(PsxToPCX(xpos));
    const s16 width_pc = static_cast<s16>(PsxToPCX(width));

    if (!mFlags.Get(AnimFlags::eBit3_Render))
    {
        return;
    }

    const PerFrameInfo* pFrameInfoHeader = Get_FrameHeader(-1);

    FP scaled_width = {};
    FP scaled_height = {};
    if (width_pc)
    {
        scaled_width = FP_FromInteger(width_pc);
        scaled_height = FP_FromInteger(height);
    }
    else
    {
        scaled_width = FP_FromInteger(pFrameInfoHeader->mWidth);
        scaled_height = FP_FromInteger(pFrameInfoHeader->mHeight);
    }

    FP xOffSet_scaled = {};
    FP yOffset_scaled = {};
    if (mFlags.Get(AnimFlags::eBit20_use_xy_offset))
    {
        xOffSet_scaled = FP_FromInteger(0);
        yOffset_scaled = FP_FromInteger(0);
    }
    else
    {
       	xOffSet_scaled = FP_FromInteger(pFrameInfoHeader->mXOffset);
        yOffset_scaled = FP_FromInteger(pFrameInfoHeader->mYOffset);
    }

    Poly_FT4* pPoly = &mOtData[gPsxDisplay.mBufferIndex];
    PolyFT4_Init(pPoly);
    Poly_Set_SemiTrans(&pPoly->mBase.header, mFlags.Get(AnimFlags::eBit15_bSemiTrans));
    Poly_Set_Blending(&pPoly->mBase.header, mFlags.Get(AnimFlags::eBit16_bBlending));

    SetRGB0(pPoly, mRed, mGreen, mBlue);
    //SetTPage(pPoly, static_cast<u16>(PSX_getTPage(textureMode, mRenderMode, mVramRect.x, mVramRect.y)));
    //SetClut(pPoly, static_cast<u16>(PSX_getClut(mPalVramXY.x, mPalVramXY.y)));
    

    u8 u1 = 0 /*mVramRect.x & 63*/;

    const u8 v0 = 0; /*static_cast<u8>(mVramRect.y);*/
    // TODO: u/v overflow on big sprites
    const u8 u0 = static_cast<u8>(pFrameInfoHeader->mWidth) + u1 - 1;
    const u8 v1 = static_cast<u8>(pFrameInfoHeader->mHeight) + v0 - 1;

    if (field_14_scale != FP_FromInteger(1))
    {
        // Apply scale to x/y pos
        scaled_height *= field_14_scale;
        scaled_width *= field_14_scale;

		// TODO: Factor out when data conversion completed
		if (GetGameType() == GameType::eAe)
		{
			// If applied to AO causes BG sprites to bounce by 1 pixel on Y
	        if (field_14_scale == FP_FromDouble(0.5))
	        {
	            // Add 1 if half scale
	            scaled_height += FP_FromDouble(1.0);
	            scaled_width += FP_FromDouble(1.0);
	        }
		}

        // Apply scale to x/y offset
        xOffSet_scaled *= field_14_scale;
        yOffset_scaled = (yOffset_scaled * field_14_scale) - FP_FromInteger(1);
    }

    s16 polyXPos = 0;
    s16 polyYPos = 0;

    const bool kFlipY = mFlags.Get(AnimFlags::eBit6_FlipY);
    const bool kFlipX = mFlags.Get(AnimFlags::eBit5_FlipX);


    if (kFlipX)
    {
        polyXPos = xpos_pc - FP_GetExponent(xOffSet_scaled + FP_FromDouble(0.499)) - FP_GetExponent(scaled_width + FP_FromDouble(0.499));
    }
    else
    {
        polyXPos = xpos_pc + FP_GetExponent(xOffSet_scaled + FP_FromDouble(0.499));
    }

    if (kFlipY)
    {
        // TODO: Might be wrong because this was doing something with the sign bit abs() ??
        polyYPos = static_cast<s16>(ypos) - FP_GetExponent(yOffset_scaled + FP_FromDouble(0.499)) - FP_GetExponent(scaled_height + FP_FromDouble(0.499));
    }
    else
    {
        polyYPos = static_cast<s16>(ypos) + FP_GetExponent(yOffset_scaled + FP_FromDouble(0.499));
    }

    SetUV0(pPoly, kFlipX ? u0 : u1, kFlipY ? v1 : v0);
    SetUV1(pPoly, kFlipX ? u1 : u0, kFlipY ? v1 : v0);
    SetUV2(pPoly, kFlipX ? u0 : u1, kFlipY ? v0 : v1);
    SetUV3(pPoly, kFlipX ? u1 : u0, kFlipY ? v0 : v1);


    SetXY0(pPoly, polyXPos, polyYPos);
    SetXY1(pPoly, polyXPos + FP_GetExponent(scaled_width - FP_FromDouble(0.501)), polyYPos);
    SetXY2(pPoly, polyXPos, polyYPos + FP_GetExponent(scaled_height - FP_FromDouble(0.501)));
    SetXY3(pPoly, polyXPos + FP_GetExponent(scaled_width - FP_FromDouble(0.501)), polyYPos + FP_GetExponent(scaled_height - FP_FromDouble(0.501)));

    pPoly->mAnim = this;

    SetPrimExtraPointerHack(pPoly, &mAnimRes);

    OrderingTable_Add(OtLayer(ppOt, mRenderLayer), &pPoly->mBase.header);
}

void Animation::VCleanUp()
{
    AnimationBase::gAnimations->Remove_Item(this);
}

void Animation::VDecode()
{
    if (DecodeCommon())
    {
        DecompressFrame();
    }
}

bool Animation::DecodeCommon()
{
    if (mAnimRes.mJsonPtr->mFrames.size() == 1 && mFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        return false;
    }

    bool isLastFrame = false;
    if (mFlags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        // Loop backwards
        const s32 prevFrameNum = --mCurrentFrame;
        mFrameChangeCounter = mFrameDelay;

        if (prevFrameNum < static_cast<s32>(mAnimRes.mJsonPtr->mAttributes.mLoopStartFrame))
        {
            if (mFlags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop to last frame
                mCurrentFrame = mAnimRes.mJsonPtr->mFrames.size() - 1;
            }
            else
            {
                // Stay on current frame
                mFrameChangeCounter = 0;
                mCurrentFrame = prevFrameNum + 1;
            }

            // For some reason eBit12_ForwardLoopCompleted isn't set when going backwards
        }

        // Is first (last since running backwards) frame?
        if (mCurrentFrame == 0)
        {
            isLastFrame = true;
        }
    }
    else
    {
        // Loop forwards
        const s32 nextFrameNum = ++mCurrentFrame;
        mFrameChangeCounter = mFrameDelay;

        // Animation reached end point
        if (nextFrameNum >= static_cast<s32>(mAnimRes.mJsonPtr->mFrames.size()))
        {
            if (mFlags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop back to loop start frame
                mCurrentFrame = mAnimRes.mJsonPtr->mAttributes.mLoopStartFrame;
            }
            else
            {
                // Stay on current frame
                mCurrentFrame = nextFrameNum - 1;
                mFrameChangeCounter = 0;
            }

            mFlags.Set(AnimFlags::eBit12_ForwardLoopCompleted);
        }

        // Is last frame ?
        if (mCurrentFrame == static_cast<s32>(mAnimRes.mJsonPtr->mFrames.size() - 1))
        {
            isLastFrame = true;
        }
    }

    if (isLastFrame)
    {
        mFlags.Set(AnimFlags::eBit18_IsLastFrame);
    }
    else
    {
        mFlags.Clear(AnimFlags::eBit18_IsLastFrame);
    }

    return true;
}

void Animation::Invoke_CallBacks()
{
    if (!mFnPtrArray)
    {
        return;
    }

    const PerFrameInfo* pFrameHeaderCopy = Get_FrameHeader(-1);
    for (u32 i = 0; i < pFrameHeaderCopy->mPointCount; i++)
    {
        const auto pFnCallBack = mFnPtrArray[pFrameHeaderCopy->mPoints[i].mIndex];
        if (!pFnCallBack)
        {
            break;
        }
        // NOTE: the call back can alter "i"
        pFnCallBack(mGameObj, i, pFrameHeaderCopy->mPoints[i].mPoint);
    }
}

s16 Animation::Set_Animation_Data(AnimResource& pAnimRes)
{
    mAnimRes = pAnimRes;

    mFrameDelay = pAnimRes.mJsonPtr->mAttributes.mFrameRate;

    mFlags.Clear(AnimFlags::eBit12_ForwardLoopCompleted);
    mFlags.Clear(AnimFlags::eBit18_IsLastFrame);
    mFlags.Clear(AnimFlags::eBit19_LoopBackwards);
    mFlags.Clear(AnimFlags::eBit8_Loop);

    if (pAnimRes.mJsonPtr->mAttributes.mLoop)
    {
        mFlags.Set(AnimFlags::eBit8_Loop);
    }

    mFrameChangeCounter = 1;
    mCurrentFrame = -1;

    VDecode();

    // Reset to start frame
    mFrameChangeCounter = 1;
    mCurrentFrame = -1;

    return 1;
}

s16 Animation::Init(const AnimResource& ppAnimData, BaseGameObject* pGameObj)
{
    mFlags.Raw().all = 0; // TODO extra - init to 0's first - this may be wrong if any bits are explicitly set before this is called

    mAnimRes = ppAnimData;
    mFnPtrArray = nullptr;

    mGameObj = pGameObj;

    mFlags.Clear(AnimFlags::eBit1);
    mFlags.Clear(AnimFlags::eBit5_FlipX);
    mFlags.Clear(AnimFlags::eBit6_FlipY);
    mFlags.Clear(AnimFlags::eBit7_SwapXY);
    mFlags.Set(AnimFlags::eBit2_Animate);
    mFlags.Set(AnimFlags::eBit3_Render);

    mFlags.Set(AnimFlags::eBit8_Loop, mAnimRes.mJsonPtr->mAttributes.mLoop);

    mFlags.Clear(AnimFlags::eBit10_alternating_flag);

    mFlags.Clear(AnimFlags::eBit14_Is16Bit);
    mFlags.Clear(AnimFlags::eBit13_Is8Bit);


    mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    mFlags.Set(AnimFlags::eBit16_bBlending);

    mFrameDelay = mAnimRes.mJsonPtr->mAttributes.mFrameRate;
    mFrameChangeCounter = 1;
    mCurrentFrame = -1;
    mRenderMode = TPageAbr::eBlend_0;
    mBlue = 0;
    mGreen = 0;
    mRed = 0;
    field_14_scale = FP_FromInteger(1);

    // NOTE: OG bug or odd compiler code gen? Why isn't it using the passed in list which appears to always be this anyway ??
    if (!AnimationBase::gAnimations->Push_Back(this))
    {
        LOG_ERROR("AnimationBase::gAnimations->Push_Back(this) returned 0 but shouldn't");
        return 0;
    }

    // Get first frame decompressed/into VRAM
    VDecode();

    mFrameChangeCounter = 1;
    mCurrentFrame = -1;

    return 1;
}

void Animation::SetFrame(s32 newFrame)
{
    if (newFrame == -1)
    {
        newFrame = 0;
    }

    if (newFrame > static_cast<s32>(mAnimRes.mJsonPtr->mFrames.size()))
    {
        newFrame = static_cast<s32>(mAnimRes.mJsonPtr->mFrames.size());
    }

    mFrameChangeCounter = 1;
    mCurrentFrame = newFrame - 1;
}

const PerFrameInfo* Animation::Get_FrameHeader(s32 frame)
{
    if (frame < -1 || frame == -1)
    {
        frame = mCurrentFrame != -1 ? mCurrentFrame : 0;
    }

    return &mAnimRes.mJsonPtr->mFrames[frame];
}

void Animation::Get_Frame_Rect(PSX_RECT* pRect)
{
    Poly_FT4* pPoly = &mOtData[gPsxDisplay.mBufferIndex];
    if (!mFlags.Get(AnimFlags::eBit20_use_xy_offset))
    {
        Poly_FT4_Get_Rect(pRect, pPoly);
        return;
    }

    const auto min_x0_x1 = std::min(X0(pPoly), X1(pPoly));
    const auto min_x2_x3 = std::min(X2(pPoly), X3(pPoly));
    pRect->x = std::min(min_x0_x1, min_x2_x3);

    const auto max_x0_x1 = std::max(X0(pPoly), X1(pPoly));
    const auto max_x2_x3 = std::max(X2(pPoly), X3(pPoly));
    pRect->w = std::max(max_x0_x1, max_x2_x3);

    const auto min_y0_y1 = std::min(Y0(pPoly), Y1(pPoly));
    const auto min_y2_y3 = std::min(Y2(pPoly), Y3(pPoly));
    pRect->y = std::min(min_y0_y1, min_y2_y3);

    const auto max_y0_y1 = std::max(Y0(pPoly), Y1(pPoly));
    const auto max_y2_y3 = std::max(Y2(pPoly), Y3(pPoly));
    pRect->h = std::max(max_y0_y1, max_y2_y3);
}

u32 Animation::Get_Frame_Count()
{
    return static_cast<u32>(mAnimRes.mJsonPtr->mFrames.size());
}

void Animation::LoadPal(const PalResource& pal)
{
    // Override the pal with another one

    // TODO: Need an active pal ptr instead of overwriting ??
    mAnimRes.mTgaPtr->mPal = *pal.mPal;

}

void Animation::ReloadPal()
{
    // TODO: Put the original pal back
}

void Animation::Get_Frame_Offset(s16* pBoundingX, s16* pBoundingY)
{
    const PerFrameInfo* pFrameHeader = Get_FrameHeader(-1);
    *pBoundingX = static_cast<s16>(pFrameHeader->mXOffset);
    *pBoundingY = static_cast<s16>(pFrameHeader->mYOffset);
}


void Animation::Get_Frame_Width_Height(s16* pWidth, s16* pHeight)
{
    const PerFrameInfo* pFrameHeader = Get_FrameHeader(-1);
    *pWidth = static_cast<s16>(pFrameHeader->mWidth);
    *pHeight = static_cast<s16>(pFrameHeader->mHeight);
}
