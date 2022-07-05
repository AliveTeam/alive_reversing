#include "stdafx.h"
#include "Shadow.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "../relive_lib/GameType.hpp"

Shadow::Shadow()
{
    const AnimRecord& shadowRec = PerGameAnimRec(AnimId::ObjectShadow);
    mAnimRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, shadowRec.mResourceId, TRUE, FALSE);
    mAnim.Init(AnimId::ObjectShadow, nullptr, mAnimRes);

    mFlags.Clear(Flags::eShadowAtBottom);
    mFlags.Set(Flags::eEnabled);

    mAnim.mRenderMode = TPageAbr::eBlend_2;

    mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    mAnim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);

    mAnim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
    mAnim.mAnimFlags.Set(AnimFlags::eBit8_Loop);
    mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mAnim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
    mAnim.mAnimFlags.Set(AnimFlags::eBit20_use_xy_offset);
    mAnim.mAnimFlags.Set(AnimFlags::eBit21);
}

Shadow::~Shadow()
{
    mAnim.VCleanUp();
    ResourceManager::FreeResource_49C330(mAnimRes);
}

void Shadow::Calculate_Position(FP xpos, FP ypos, PSX_RECT* frameRect, FP spriteScale, Scale scale)
{
    if (mFlags.Get(Flags::eEnabled))
    {
        // TODO: Is this the same as PsxToPCX ??
        const s16 objX = (23 * frameRect->x) / 40;
        const s16 objW = (23 * frameRect->w) / 40;

        FP objY = {};
        if (mFlags.Get(Flags::eShadowAtBottom))
        {
            // Get the bottom of the object
            objY = FP_FromInteger(frameRect->h) + pScreenManager->CamYPos();
        }
        else
        {
            objY = ypos;
        }

        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (sCollisions->Raycast(
                xpos,
                objY,
                xpos,
                objY + FP_FromInteger(240),
                &pLine,
                &hitX,
                &hitY,
                scale == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls)) // NOTE: AO didn't check ceilings
        {
            const s16 camXPos = FP_GetExponent(pScreenManager->CamXPos());
            s16 lineXScreen = pLine->mRect.x - camXPos;
            s16 lineWScreen = pLine->mRect.w - camXPos;

            if (lineWScreen < lineXScreen)
            {
                lineXScreen = pLine->mRect.w - camXPos;
                lineWScreen = pLine->mRect.x - camXPos;
            }

            mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render);

            mXPos = xpos;

            // TODO :Refactor out, AO uses an offset of 3 for unknown reasons
            mYPos = hitY + FP_FromInteger(GetGameType() == GameType::eAe ? 0: 3);

            mScale = (FP_FromInteger(1) - (((hitY - objY) * FP_FromDouble(0.75)) / FP_FromInteger(240))) * spriteScale;

            // Object is before the line we hit
            if (objX < lineXScreen)
            {
                if (sCollisions->Raycast(
                        FP_NoFractional(pScreenManager->CamXPos()) + (FP_FromInteger(lineXScreen - 1)) - FP_FromInteger(4),
                        hitY - FP_FromInteger(2),
                        FP_NoFractional(pScreenManager->CamXPos()) + (FP_FromInteger(lineXScreen - 1)) - FP_FromInteger(4),
                        hitY + FP_FromInteger(2),
                        &pLine,
                        &hitX,
                        &hitY,
                        scale == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls)) // NOTE: AO didn't check ceilings
                {
                    lineXScreen = std::min(pLine->mRect.x, pLine->mRect.w) - FP_GetExponent(pScreenManager->CamXPos());
                }
            }

            // Object is after the line we hit
            if (objW > lineWScreen)
            {
                if (sCollisions->Raycast(
                        FP_NoFractional(pScreenManager->CamXPos()) + (FP_FromInteger(lineWScreen + 1)) + FP_FromInteger(4),
                        hitY - FP_FromInteger(2),
                        FP_NoFractional(pScreenManager->CamXPos()) + (FP_FromInteger(lineWScreen + 1)) + FP_FromInteger(4),
                        hitY + FP_FromInteger(2),
                        &pLine,
                        &hitX,
                        &hitY,
                        scale == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
                {
                    lineWScreen = std::max(pLine->mRect.x, pLine->mRect.w) - FP_GetExponent(pScreenManager->CamXPos());
                }
            }


            mX1 = std::max(objX, lineXScreen);
            mX2 = std::min(objW, lineWScreen);

            s16 height;
            if (FP_GetExponent(mScale * FP_FromInteger(6)) <= 6)
            {
                height = FP_GetExponent(mScale * FP_FromInteger(6));
            }
            else
            {
                height = 6;
            }

            const s16 finalYPos = FP_GetExponent(mYPos - pScreenManager->CamYPos()) - height / 2;
            mY1 = finalYPos;
            mY2 = finalYPos + height;
        }
        else
        {
            // Didn't hit anything so don't draw a shadow
            mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        }

        if (scale == Scale::Fg)
        {
            mAnim.mRenderLayer = Layer::eLayer_Shadow_26;
        }
        else
        {
            mAnim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        }
    }
}

void Shadow::Render(PrimHeader** ppOt)
{
    if (mFlags.Get(Flags::eEnabled))
    {
        mAnim.field_14_scale = FP_FromInteger(1);
        if (mScale == FP_FromDouble(0.5))
        {
            mAnim.mRed = 63;
            mAnim.mGreen = 63;
            mAnim.mBlue = 63;
        }
        else
        {
            mAnim.mRed = 127;
            mAnim.mGreen = 127;
            mAnim.mBlue = 127;
        }

        mAnim.VRender(
            // Note: OG converted to FP and back here but its pointless
            mX1,
            mY1,
            ppOt,
            (mX2 - mX1) + 1,
            (mY2 - mY1) + 1);

        PSX_RECT frameRect = {};
        mAnim.Get_Frame_Rect(&frameRect);
        pScreenManager->InvalidateRectCurrentIdx(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h);
    }
}
