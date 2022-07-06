#include "stdafx.h"
#include "Animation.hpp"
#include "../AliveLibAE/Compression.hpp"
#include "../AliveLibAO/Compression.hpp"
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

static IRenderer::BitDepth AnimFlagsToBitDepth(const BitField32<AnimFlags>& flags)
{
    if (flags.Get(AnimFlags::eBit14_Is16Bit))
    {
        return IRenderer::BitDepth::e16Bit;
    }
    else if (flags.Get(AnimFlags::eBit13_Is8Bit))
    {
        return IRenderer::BitDepth::e8Bit;
    }
    return IRenderer::BitDepth::e4Bit;
}

void Animation::UploadTexture(const FrameHeader* pFrameHeader, const PSX_RECT& vram_rect, s16 width_bpp_adjusted)
{
    IRenderer& renderer = *IRenderer::GetRenderer();
    
    // TODO: Remove when rendering is common
    const bool ignoreCacheBit = GetGameType() == GameType::eAo;

    switch (pFrameHeader->field_7_compression_type)
    {
        case CompressionType::eType_0_NoCompression:
            // No compression, load the data directly into frame buffer
            mFlags.Set(AnimFlags::eBit25_bDecompressDone);
            renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2)); // TODO: Refactor structure to get pixel data
            break;

        case CompressionType::eType_1_NotUsed:
            // This isn't in any of the animation data files on disk, therefore can't ever be used.
            ALIVE_FATAL("Compression type 1 never expected to be used.");
            break;

        case CompressionType::eType_2_ThreeToFourBytes:
            mFlags.Set(AnimFlags::eBit25_bDecompressDone);
            if (EnsureDecompressionBuffer())
            {
                // TODO: Refactor structure to get pixel data.
                CompressionType2_Decompress_40AA50(
                    reinterpret_cast<const u8*>(&pFrameHeader[1]),
                    *mDbuf,
                    width_bpp_adjusted * pFrameHeader->field_5_height * 2);

                renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, *mDbuf);
            }
            break;

        case CompressionType::eType_3_RLE_Blocks:
            if (ignoreCacheBit || mFlags.Get(AnimFlags::eBit25_bDecompressDone))
            {
                if (EnsureDecompressionBuffer())
                {
                    // TODO: Refactor structure to get pixel data.
					
					// Type 3 algorithm changes per game, yikies
					if (GetGameType() == GameType::eAe)
    				{
                    	CompressionType_3Ae_Decompress_40A6A0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *mDbuf);
					}
					else
					{
						 AO::Decompress_Type_3(
		                    (u8*) &pFrameHeader[1],
		                    *mDbuf,
		                    *(u32*) &pFrameHeader->field_8_width2,
		                    2 * pFrameHeader->field_5_height * width_bpp_adjusted);
					}
                    renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, *mDbuf);
                }
            }
            break;

        case CompressionType::eType_4_RLE:
        case CompressionType::eType_5_RLE:
            if (EnsureDecompressionBuffer())
            {
                // TODO: Refactor structure to get pixel data.
                CompressionType_4Or5_Decompress_4ABAB0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *mDbuf);
                renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, *mDbuf);
            }
            break;

        case CompressionType::eType_6_RLE:
            if (mFlags.Get(AnimFlags::eBit25_bDecompressDone))
            {
                if (EnsureDecompressionBuffer())
                {
                    // TODO: Refactor structure to get pixel data.
                    CompressionType6Ae_Decompress_40A8A0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *mDbuf);
                    renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, *mDbuf);
                }
            }
            break;

        case CompressionType::eType_7_NotUsed:
        case CompressionType::eType_8_NotUsed:
            ALIVE_FATAL("Decompression 7 and 8 never expected to be used");
            break;
			
		default:
            LOG_ERROR("Unknown compression type " << static_cast<s32>(pFrameHeader->field_7_compression_type));
            ALIVE_FATAL("Unknown compression type");
            break;
    }
}

bool Animation::EnsureDecompressionBuffer()
{
    if (!mDbuf)
    {
        mDbuf = ResourceManagerWrapper::Alloc_New_Resource(ResourceManagerWrapper::Resource_DecompressionBuffer, 0, mDbufSize);
    }
    return mDbuf != nullptr;
}

void Animation::DecompressFrame()
{
    if (mFlags.Get(AnimFlags::eBit11_bToggle_Bit10))
    {
        mFlags.Toggle(AnimFlags::eBit10_alternating_flag);
    }

    const FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader(-1); // -1 = use current frame
    if (pFrameInfoHeader->field_6_count > 0)
    {
        Invoke_CallBacks();
    }

    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

    // No VRAM allocation
    if (mVramRect.w <= 0)
    {
        return;
    }

    s16 width_bpp_adjusted = 0;
    if (mFlags.Get(AnimFlags::eBit13_Is8Bit))
    {
        // 8 bit, divided by half
        width_bpp_adjusted = ((pFrameHeader->field_4_width + 3) / 2) & ~1;
    }
    else if (mFlags.Get(AnimFlags::eBit14_Is16Bit))
    {
        // 16 bit, only multiple of 2 rounding
        width_bpp_adjusted = (pFrameHeader->field_4_width + 1) & ~1;
    }
    else
    {
        // 4 bit divide by quarter
        width_bpp_adjusted = ((pFrameHeader->field_4_width + 7) / 4) & ~1;
    }

    PSX_RECT vram_rect = {
        mVramRect.x,
        mVramRect.y,
        width_bpp_adjusted,
        pFrameHeader->field_5_height,
    };

    // Clamp width
    if (vram_rect.w > mVramRect.w)
    {
        vram_rect.w = mVramRect.w;
    }

    // Clamp height
    if (pFrameHeader->field_5_height > mVramRect.h)
    {
        vram_rect.h = mVramRect.h;
    }

    UploadTexture(pFrameHeader, vram_rect, width_bpp_adjusted);
}

void Animation::VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s32 height)
{
    // TODO: Factor out  - required to due the way psx rendering currently works for AE
    if (GetGameType() == GameType::eAe)
    {
        if ((mVramRect.x || mVramRect.y) && !(mFlags.Get(AnimFlags::eBit25_bDecompressDone)))
        {
            Vram_free({mVramRect.x, mVramRect.y}, {mVramRect.w, mVramRect.h});
            mVramRect.x = 0;
            mVramRect.y = 0;
        }
    }

    const s16 xpos_pc = static_cast<s16>(PsxToPCX(xpos));
    const s16 width_pc = static_cast<s16>(PsxToPCX(width));

    if (!mFlags.Get(AnimFlags::eBit3_Render))
    {
        return;
    }

    if (!field_20_ppBlock)
    {
        return;
    }


    const FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader(-1);
    const FrameHeader* pFrameHeader = (const FrameHeader*) &(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset];

    FP scaled_width = {};
    FP scaled_height = {};
    if (width_pc)
    {
        scaled_width = FP_FromInteger(width_pc);
        scaled_height = FP_FromInteger(height);
    }
    else
    {
        scaled_width = FP_FromInteger(pFrameHeader->field_4_width);
        scaled_height = FP_FromInteger(pFrameHeader->field_5_height);
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
		// TODO: Factor out when data conversion completed
    	if (GetGameType() == GameType::eAe)
		{
        	xOffSet_scaled = FP_FromInteger(pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x);
		}
		else
		{
			// TODO: Fix this when data conversion is done
			xOffSet_scaled = PsxToPCX(FP_FromInteger(pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x));
		}	
        yOffset_scaled = FP_FromInteger(pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y);
    }

    TPageMode textureMode = {};
    if (mFlags.Get(AnimFlags::eBit13_Is8Bit))
    {
        textureMode = TPageMode::e8Bit_1;
    }
    else if (mFlags.Get(AnimFlags::eBit14_Is16Bit))
    {
        textureMode = TPageMode::e16Bit_2;
    }
    else
    {
        textureMode = TPageMode::e4Bit_0;
    }

    Poly_FT4* pPoly = &mOtData[gPsxDisplay.mBufferIndex];
    PolyFT4_Init(pPoly);
    Poly_Set_SemiTrans(&pPoly->mBase.header, mFlags.Get(AnimFlags::eBit15_bSemiTrans));
    Poly_Set_Blending(&pPoly->mBase.header, mFlags.Get(AnimFlags::eBit16_bBlending));

    SetRGB0(pPoly, mRed, mGreen, mBlue);
    SetTPage(pPoly, static_cast<u16>(PSX_getTPage(textureMode, mRenderMode, mVramRect.x, mVramRect.y)));
    SetClut(pPoly, static_cast<u16>(PSX_getClut(mPalVramXY.x, mPalVramXY.y)));

    u8 u1 = mVramRect.x & 63;
    if (textureMode == TPageMode::e8Bit_1)
    {
        // 8 bit
        u1 *= 2;
    }
    else if (textureMode == TPageMode::e4Bit_0)
    {
        // 4 bit
        u1 *= 4;
    }
    else
    {
        // 16 bit
    }

    const u8 v0 = static_cast<u8>(mVramRect.y);
    const u8 u0 = pFrameHeader->field_4_width + u1 - 1;
    const u8 v1 = pFrameHeader->field_5_height + v0 - 1;

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

	// TODO: Factor out when rendering is rewritten
    if (GetGameType() == GameType::eAe && (pFrameHeader->field_7_compression_type == CompressionType::eType_3_RLE_Blocks || pFrameHeader->field_7_compression_type == CompressionType::eType_6_RLE))
    {
	
        SetPrimExtraPointerHack(pPoly, &pFrameHeader->field_8_width2);
    }
    else
    {
        SetPrimExtraPointerHack(pPoly, nullptr);
    }

    OrderingTable_Add(OtLayer(ppOt, mRenderLayer), &pPoly->mBase.header);
}

void Animation::VCleanUp()
{
    AnimationBase::gAnimations->Remove_Item(this);

    Animation_Pal_Free();

    ResourceManagerWrapper::FreeResource(mDbuf);
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
    if (!field_20_ppBlock || !*field_20_ppBlock)
    {
        return false;
    }

    AnimationHeader* pAnimationHeader = reinterpret_cast<AnimationHeader*>(&(*field_20_ppBlock)[mFrameTableOffset]);
    if (pAnimationHeader->field_2_num_frames == 1 && mFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        return false;
    }

    bool isLastFrame = false;
    if (mFlags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        // Loop backwards
        const s16 prevFrameNum = --mCurrentFrame;
        mFrameChangeCounter = mFrameDelay;

        if (prevFrameNum < pAnimationHeader->field_4_loop_start_frame)
        {
            if (mFlags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop to last frame
                mCurrentFrame = pAnimationHeader->field_2_num_frames - 1;
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
        const s16 nextFrameNum = ++mCurrentFrame;
        mFrameChangeCounter = mFrameDelay;

        // Animation reached end point
        if (nextFrameNum >= pAnimationHeader->field_2_num_frames)
        {
            if (mFlags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop back to loop start frame
                mCurrentFrame = pAnimationHeader->field_4_loop_start_frame;
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
        if (mCurrentFrame == pAnimationHeader->field_2_num_frames - 1)
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
    if (!field_20_ppBlock || !mFnPtrArray)
    {
        return;
    }

    FrameInfoHeader* pFrameHeaderCopy = Get_FrameHeader(-1);
    // This data can be an array of u32's + other data up to field_6_count
    // which appears AFTER the usual data.
    // TODO: Should be typed to s16* ??
    const u32* pCallBackData = reinterpret_cast<const u32*>(&pFrameHeaderCopy->field_8_data.points[3]);
    for (s32 i = 0; i < pFrameHeaderCopy->field_6_count; i++)
    {
        const auto pFnCallBack = mFnPtrArray[*pCallBackData];
        if (!pFnCallBack)
        {
            break;
        }
        pCallBackData++; // Skip the array index
        // Pass the data pointer into the call back which will then read and skip any extra data
        pCallBackData += pFnCallBack(mGameObj, (s16*) pCallBackData);
    }
}

s16 Animation::Set_Animation_Data(AnimId animId, u8** pAnimRes)
{
    const AnimRecord& rec = PerGameAnimRec(animId);
    return Set_Animation_Data(rec.mFrameTableOffset, pAnimRes);
}

s16 Animation::Set_Animation_Data(s32 frameTableOffset, u8** pAnimRes)
{
    FrameTableOffsetExists(frameTableOffset, GetGameType() == GameType::eAe ? true : false);
    if (pAnimRes)
    {
        field_20_ppBlock = pAnimRes;
    }

    if (!field_20_ppBlock)
    {
        return 0;
    }

    mFrameTableOffset = frameTableOffset;

    AnimationHeader* pAnimationHeader = reinterpret_cast<AnimationHeader*>(&(*field_20_ppBlock)[mFrameTableOffset]);
    mFrameDelay = pAnimationHeader->field_0_fps;

    mFlags.Clear(AnimFlags::eBit12_ForwardLoopCompleted);
    mFlags.Clear(AnimFlags::eBit18_IsLastFrame);
    mFlags.Clear(AnimFlags::eBit19_LoopBackwards);
    mFlags.Clear(AnimFlags::eBit8_Loop);

    if (pAnimationHeader->field_6_flags & AnimationHeader::eLoopFlag)
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

void Animation::Animation_Pal_Free()
{
	// TODO: Remove when all objects use the BaseGameObject resource list in AO
	if (GetGameType() == GameType::eAo)
	{
	    if (field_20_ppBlock)
	    {
            ResourceManagerWrapper::FreeResource(field_20_ppBlock);
	        field_20_ppBlock = nullptr;
	    }
	}

    if (mVramRect.w > 0)
    {
        if (mVramRect.x || mVramRect.y)
        {
            Vram_free({mVramRect.x, mVramRect.y}, {mVramRect.w, mVramRect.h});
        }
    }

    if (mPalDepth > 0)
    {
        IRenderer::GetRenderer()->PalFree(IRenderer::PalRecord{mPalVramXY.x, mPalVramXY.y, mPalDepth});
    }
}

void Animation::SetFrame(s16 newFrame)
{
    if (field_20_ppBlock)
    {
        if (newFrame == -1)
        {
            newFrame = 0;
        }

        AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + mFrameTableOffset); // TODO: Make getting offset to animation header cleaner

        if (newFrame > pHead->field_2_num_frames)
        {
            newFrame = pHead->field_2_num_frames;
        }

        mFrameChangeCounter = 1;
        mCurrentFrame = newFrame - 1;
    }
}

FrameInfoHeader* Animation::Get_FrameHeader(s16 frame)
{
    if (!field_20_ppBlock)
    {
        return nullptr;
    }

    if (frame < -1 || frame == -1)
    {
        frame = mCurrentFrame != -1 ? mCurrentFrame : 0;
    }

    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + mFrameTableOffset); // TODO: Make getting offset to animation header cleaner
    u32 frameOffset = pHead->mFrameOffsets[frame];

    FrameInfoHeader* pFrame = reinterpret_cast<FrameInfoHeader*>(*field_20_ppBlock + frameOffset);
    return pFrame;
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

u16 Animation::Get_Frame_Count()
{
    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + mFrameTableOffset); // TODO: Make getting offset to animation header cleaner
    return pHead->field_2_num_frames;
}

s16 Animation::Init(AnimId animId, BaseGameObject* pGameObj, u8** ppAnimData)
{
    const AnimRecord& anim = PerGameAnimRec(animId);
    return Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, pGameObj, ppAnimData);
}

s16 Animation::Init(s32 frameTableOffset, u16 maxW, u16 maxH, BaseGameObject* pGameObj, u8** ppAnimData)
{
    FrameTableOffsetExists(frameTableOffset, GetGameType() == GameType::eAe ? true : false, maxW, maxH);
    mFlags.Raw().all = 0; // TODO extra - init to 0's first - this may be wrong if any bits are explicitly set before this is called

    mFrameTableOffset = frameTableOffset;
    field_20_ppBlock = ppAnimData;
    mFnPtrArray = nullptr;
    mDbuf = nullptr;

    if (!ppAnimData)
    {
        return 0;
    }

    mGameObj = pGameObj;
    AnimationHeader* pHeader = reinterpret_cast<AnimationHeader*>(&(*ppAnimData)[frameTableOffset]);

    mFlags.Clear(AnimFlags::eBit1);
    mFlags.Clear(AnimFlags::eBit5_FlipX);
    mFlags.Clear(AnimFlags::eBit6_FlipY);
    mFlags.Clear(AnimFlags::eBit7_SwapXY);
    mFlags.Set(AnimFlags::eBit2_Animate);
    mFlags.Set(AnimFlags::eBit3_Render);

    mFlags.Set(AnimFlags::eBit8_Loop, pHeader->field_6_flags & AnimationHeader::eLoopFlag);

    mFlags.Clear(AnimFlags::eBit10_alternating_flag);

    mFlags.Clear(AnimFlags::eBit11_bToggle_Bit10);

    mFlags.Clear(AnimFlags::eBit14_Is16Bit);
    mFlags.Clear(AnimFlags::eBit13_Is8Bit);

    // Clear vram/pal inits to not allocated
    mVramRect.w = 0;
    mPalDepth = 0;

    mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    mFlags.Set(AnimFlags::eBit16_bBlending);

    mFrameDelay = pHeader->field_0_fps;
    mFrameChangeCounter = 1;
    mCurrentFrame = -1;
    mRenderMode = TPageAbr::eBlend_0;
    mBlue = 0;
    mGreen = 0;
    mRed = 0;
    field_14_scale = FP_FromInteger(1);

    FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader(0);
    u8* pAnimData = *field_20_ppBlock;

    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

    u8* pClut = &pAnimData[pFrameHeader->field_0_clut_offset];

    s8 b256Pal = 0;
    s32 vram_width = 0;
    s16 pal_depth = 0;

    switch (pFrameHeader->field_6_colour_depth)
    {
    case 4:
    {
        vram_width = (maxW % 2) + (maxW / 2);
        pal_depth = 16;
        b256Pal = 0; // is 16 pal
    }
    break;

    case 8:
    {
        vram_width = maxW;
		// TODO: Should this be u16* ??
        if (*(u32*)pClut == 64) // CLUT entry count/len
        {
            pal_depth = 64;
        }
        else
        {
            pal_depth = 256;
            b256Pal = 1; // is 256 pal
        }
        mFlags.Set(AnimFlags::eBit13_Is8Bit);

    }
    break;

    case 16:
    {
        vram_width = maxW * 2;
        mFlags.Set(AnimFlags::eBit14_Is16Bit);
    }
    break;

    default:
        return 0;
    }

    if (!Vram_alloc(maxW, maxH, pFrameHeader->field_6_colour_depth, &mVramRect))
    {
        // Seems like this can at least happen with many bomb particles
        // this will be fixed in the future
        LOG_ERROR("Vram alloc failed");
        return 0;
    }

    // TODO: This flag name is wrong and likely not needed once the rendering
	// or psx rendering simulation is removed
    mFlags.Set(AnimFlags::eBit25_bDecompressDone, b256Pal);


    if (pal_depth > 0)
    {
        IRenderer::PalRecord palRec{ 0, 0, pal_depth };
        if (!IRenderer::GetRenderer()->PalAlloc(palRec))
        {
            ALIVE_FATAL("PalAlloc failed");
        }

        mPalVramXY.x = palRec.x;
        mPalVramXY.y = palRec.y;
        mPalDepth = palRec.depth;

        IRenderer::GetRenderer()->PalSetData(palRec, pClut + 4); // +4 Skip len, load pal
    }

    mDbufSize = maxH * (vram_width + 3);
    mDbufSize += 8; // Add 8 for some reason
    mDbuf = nullptr;

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

void Animation::LoadPal(u8** pAnimData, s32 palOffset)
{
    if (pAnimData)
    {
        // +4 = skip CLUT len
        const u8* pPalDataOffset = &(*pAnimData)[palOffset];
        if (mPalDepth != 16 && mPalDepth != 64 && mPalDepth != 256)
        {
            LOG_ERROR("Bad pal depth " << mPalDepth);
            ALIVE_FATAL("Bad pal depth");
        }
        IRenderer::GetRenderer()->PalSetData(IRenderer::PalRecord{mPalVramXY.x, mPalVramXY.y, mPalDepth}, pPalDataOffset + 4); // +4 skip len, load pal
    }
}

void Animation::Get_Frame_Offset(s16* pBoundingX, s16* pBoundingY)
{
    FrameInfoHeader* pFrameHeader = Get_FrameHeader(-1);
    *pBoundingX = pFrameHeader->field_8_data.offsetAndRect.mOffset.x;
    *pBoundingY = pFrameHeader->field_8_data.offsetAndRect.mOffset.y;
}


void Animation::Get_Frame_Width_Height(s16* pWidth, s16* pHeight)
{
    FrameInfoHeader* pFrameHeader = Get_FrameHeader(-1);
    if (mFlags.Get(AnimFlags::eBit22_DeadMode))
    {
        ALIVE_FATAL("Mode should never be used");
    }
    else
    {
        auto pHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameHeader->field_0_frame_header_offset]);
        *pWidth = pHeader->field_4_width;
        *pHeight = pHeader->field_5_height;
    }
}
