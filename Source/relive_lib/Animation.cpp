#include "stdafx.h"
#include "Animation.hpp"
#include "../AliveLibAE/Compression.hpp"
#include "../relive_lib/Compression.hpp"
#include "PsxDisplay.hpp"
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

    if (!mFlags.Get(AnimFlags::eRender))
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
    if (mFlags.Get(AnimFlags::eIgnorePosOffset))
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
    Poly_Set_SemiTrans(&pPoly->mBase.header, mFlags.Get(AnimFlags::eSemiTrans));
    Poly_Set_Blending(&pPoly->mBase.header, mFlags.Get(AnimFlags::eBlending));

    SetRGB0(pPoly, mRgb);

    u8 u1 = 0 /*mVramRect.x & 63*/;

    const u8 v0 = 0; /*static_cast<u8>(mVramRect.y);*/
    // TODO: u/v overflow on big sprites
    const u8 u0 = static_cast<u8>(pFrameInfoHeader->mWidth) + u1 - 1;
    const u8 v1 = static_cast<u8>(pFrameInfoHeader->mHeight) + v0 - 1;

    if (mSpriteScale != FP_FromInteger(1))
    {
        // Apply scale to x/y pos
        scaled_height *= mSpriteScale;
        scaled_width *= mSpriteScale;

		// TODO: Factor out when data conversion completed
		if (GetGameType() == GameType::eAe)
		{
			// If applied to AO causes BG sprites to bounce by 1 pixel on Y
	        if (mSpriteScale == FP_FromDouble(0.5))
	        {
	            // Add 1 if half scale
	            scaled_height += FP_FromDouble(1.0);
	            scaled_width += FP_FromDouble(1.0);
	        }
		}

        // Apply scale to x/y offset
        xOffSet_scaled *= mSpriteScale;
        yOffset_scaled = (yOffset_scaled * mSpriteScale) - FP_FromInteger(1);
    }

    s16 polyXPos = 0;
    s16 polyYPos = 0;

    const bool kFlipY = mFlags.Get(AnimFlags::eFlipY);
    const bool kFlipX = mFlags.Get(AnimFlags::eFlipX);


    if (kFlipX)
    {
        polyXPos = xpos_pc - FP_GetExponent(xOffSet_scaled) - FP_GetExponent(scaled_width);
    }
    else
    {
        polyXPos = xpos_pc + FP_GetExponent(xOffSet_scaled);
    }

    if (kFlipY)
    {
        // TODO: Might be wrong because this was doing something with the sign bit abs() ??
        polyYPos = static_cast<s16>(ypos) - FP_GetExponent(yOffset_scaled) - FP_GetExponent(scaled_height);
    }
    else
    {
        polyYPos = static_cast<s16>(ypos) + FP_GetExponent(yOffset_scaled);
    }

    SetUV0(pPoly, kFlipX ? u0 : u1, kFlipY ? v1 : v0);
    SetUV1(pPoly, kFlipX ? u1 : u0, kFlipY ? v1 : v0);
    SetUV2(pPoly, kFlipX ? u0 : u1, kFlipY ? v0 : v1);
    SetUV3(pPoly, kFlipX ? u1 : u0, kFlipY ? v0 : v1);


    SetXY0(pPoly, polyXPos, polyYPos);
    SetXY1(pPoly, polyXPos + FP_GetExponent(scaled_width), polyYPos);
    SetXY2(pPoly, polyXPos, polyYPos + FP_GetExponent(scaled_height));
    SetXY3(pPoly, polyXPos + FP_GetExponent(scaled_width), polyYPos + FP_GetExponent(scaled_height));

    // TPage blend mode
    u16 tpageEmptyBlend = GetTPage(pPoly) & 0xFFCF;
    u16 blendModeBit = ((u16) GetRenderMode()) << 4;

    SetTPage(pPoly, tpageEmptyBlend | blendModeBit);

    pPoly->mFlipX = kFlipX;
    pPoly->mFlipY = kFlipY;
    pPoly->mAnim = this;

    SetPrimExtraPointerHack(pPoly, &mAnimRes);

    OrderingTable_Add(OtLayer(ppOt, GetRenderLayer()), &pPoly->mBase.header);
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
    if (mAnimRes.mJsonPtr->mFrames.size() == 1 && mFlags.Get(AnimFlags::eForwardLoopCompleted))
    {
        return false;
    }

    bool isLastFrame = false;
    if (mFlags.Get(AnimFlags::eLoopBackwards))
    {
        // Loop backwards
        const s32 prevFrameNum = --mCurrentFrame;
        SetFrameChangeCounter(mFrameDelay);

        if (prevFrameNum < static_cast<s32>(mAnimRes.mJsonPtr->mAttributes.mLoopStartFrame))
        {
            if (mFlags.Get(AnimFlags::eLoop))
            {
                // Loop to last frame
                mCurrentFrame = static_cast<s32>(mAnimRes.mJsonPtr->mFrames.size()) - 1;
            }
            else
            {
                // Stay on current frame
                SetFrameChangeCounter(0);
                mCurrentFrame = prevFrameNum + 1;
            }

            // For some reason eForwardLoopCompleted isn't set when going backwards
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
        SetFrameChangeCounter(mFrameDelay);

        // Animation reached end point
        if (nextFrameNum >= static_cast<s32>(mAnimRes.mJsonPtr->mFrames.size()))
        {
            if (mFlags.Get(AnimFlags::eLoop))
            {
                // Loop back to loop start frame
                mCurrentFrame = mAnimRes.mJsonPtr->mAttributes.mLoopStartFrame;
            }
            else
            {
                // Stay on current frame
                mCurrentFrame = nextFrameNum - 1;
                SetFrameChangeCounter(0);
            }

            mFlags.Set(AnimFlags::eForwardLoopCompleted);
        }

        // Is last frame ?
        if (mCurrentFrame == static_cast<s32>(mAnimRes.mJsonPtr->mFrames.size() - 1))
        {
            isLastFrame = true;
        }
    }

    if (isLastFrame)
    {
        mFlags.Set(AnimFlags::eIsLastFrame);
    }
    else
    {
        mFlags.Clear(AnimFlags::eIsLastFrame);
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
        pFnCallBack(mGameObj, i, pFrameHeaderCopy->mPoints[i]);
    }
}

s16 Animation::Set_Animation_Data(AnimResource& pAnimRes)
{
    auto oldPal = mAnimRes.mCurPal;

    mAnimRes = pAnimRes;

    // Keep the custom pal that was set
    if (oldPal)
    {
        mAnimRes.mCurPal = oldPal;
    }

    mFrameDelay = pAnimRes.mJsonPtr->mAttributes.mFrameRate;

    mFlags.Clear(AnimFlags::eForwardLoopCompleted);
    mFlags.Clear(AnimFlags::eIsLastFrame);
    mFlags.Clear(AnimFlags::eLoopBackwards);
    mFlags.Clear(AnimFlags::eLoop);

    if (pAnimRes.mJsonPtr->mAttributes.mLoop)
    {
        mFlags.Set(AnimFlags::eLoop);
    }

    SetFrameChangeCounter(1);
    mCurrentFrame = -1;

    VDecode();

    // Reset to start frame
    SetFrameChangeCounter(1);
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
    mFlags.Clear(AnimFlags::eFlipX);
    mFlags.Clear(AnimFlags::eFlipY);
    mFlags.Clear(AnimFlags::eSwapXY);
    mFlags.Set(AnimFlags::eAnimate);
    mFlags.Set(AnimFlags::eRender);

    mFlags.Set(AnimFlags::eLoop, mAnimRes.mJsonPtr->mAttributes.mLoop);

    mFlags.Clear(AnimFlags::eBit10_alternating_flag);

    mFlags.Clear(AnimFlags::eIs16Bit);
    mFlags.Clear(AnimFlags::eIs8Bit);


    mFlags.Clear(AnimFlags::eSemiTrans);
    mFlags.Set(AnimFlags::eBlending);

    mFrameDelay = mAnimRes.mJsonPtr->mAttributes.mFrameRate;
    SetFrameChangeCounter(1);
    mCurrentFrame = -1;
    SetRenderMode(TPageAbr::eBlend_0);
    mSpriteScale = FP_FromInteger(1);

    // NOTE: OG bug or odd compiler code gen? Why isn't it using the passed in list which appears to always be this anyway ??
    if (!AnimationBase::gAnimations->Push_Back(this))
    {
        LOG_ERROR("AnimationBase::gAnimations->Push_Back(this) returned 0 but shouldn't");
        return 0;
    }

    // Get first frame decompressed/into VRAM
    VDecode();

    SetFrameChangeCounter(1);
    SetCurrentFrame(-1);

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

    SetFrameChangeCounter(1);
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
    if (!mFlags.Get(AnimFlags::eIgnorePosOffset))
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
    mAnimRes.mCurPal = pal.mPal;
}

void Animation::ReloadPal()
{
    // Put the original pal back
    mAnimRes.mCurPal = mAnimRes.mTgaPtr->mPal;
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
