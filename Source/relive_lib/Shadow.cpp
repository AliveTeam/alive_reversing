#include "stdafx.h"
#include "Shadow.hpp"
#include "ScreenManager.hpp"
#include "ResourceManagerWrapper.hpp"
#include "GameType.hpp"
#include "Collisions.hpp"
#include "../AliveLibCommon/AnimResources.hpp"
#include <algorithm>

Shadow::Shadow()
{
    AnimResource res = ResourceManagerWrapper::LoadAnimation(AnimId::ObjectShadow);
    mAnim.Init(res, nullptr);

    mShadowAtBottom = false;
    mEnabled = true;

    mAnim.SetRenderMode(TPageAbr::eBlend_2);

    mAnim.mFlags.Clear(AnimFlags::eRender);
    mAnim.mFlags.Clear(AnimFlags::eBlending);

    mAnim.mFlags.Set(AnimFlags::eAnimate);
    mAnim.mFlags.Set(AnimFlags::eLoop);
    mAnim.mFlags.Set(AnimFlags::eSemiTrans);
    mAnim.mFlags.Set(AnimFlags::eIsLastFrame);
    mAnim.mFlags.Set(AnimFlags::eIgnorePosOffset);
    mAnim.mFlags.Set(AnimFlags::eBit21);
}

Shadow::~Shadow()
{
    mAnim.VCleanUp();
}

void Shadow::Calculate_Position(FP xpos, FP ypos, PSX_RECT* frameRect, FP spriteScale, Scale scale)
{
    if (mEnabled)
    {
        // TODO: Is this the same as PsxToPCX ??
        const s16 objX = (23 * frameRect->x) / 40;
        const s16 objW = (23 * frameRect->w) / 40;

        FP objY = {};
        if (mShadowAtBottom)
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

            mAnim.mFlags.Set(AnimFlags::eRender);

            mXPos = xpos;

            // TODO :Refactor out, AO uses an offset of 3 for unknown reasons
            mYPos = hitY + FP_FromInteger(GetGameType() == GameType::eAe ? 0: 3);

            mSpriteScale = (FP_FromInteger(1) - (((hitY - objY) * FP_FromDouble(0.75)) / FP_FromInteger(240))) * spriteScale;

            const CollisionMask lineType = scale == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls;
            // lineType = spriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor;

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
                        lineType)) // NOTE: AO didn't check ceilings
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
                        lineType))
                {
                    lineWScreen = std::max(pLine->mRect.x, pLine->mRect.w) - FP_GetExponent(pScreenManager->CamXPos());
                }
            }


            mX1 = std::max(objX, lineXScreen);
            mX2 = std::min(objW, lineWScreen);

            s16 height;
            if (FP_GetExponent(mSpriteScale * FP_FromInteger(6)) <= 6)
            {
                height = FP_GetExponent(mSpriteScale * FP_FromInteger(6));
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
            mAnim.mFlags.Clear(AnimFlags::eRender);
        }

        if (scale == Scale::Fg)
        {
            mAnim.SetRenderLayer(Layer::eLayer_Shadow_26);
        }
        else
        {
            mAnim.SetRenderLayer(Layer::eLayer_Shadow_Half_7);
        }
    }
}

void Shadow::Render(PrimHeader** ppOt)
{
    if (mEnabled)
    {
        mAnim.SetSpriteScale(FP_FromInteger(1));
        if (mSpriteScale == FP_FromDouble(0.5))
        {
            mAnim.SetRGB(63, 63, 63);
        }
        else
        {
            mAnim.SetRGB(127, 127, 127);
        }

        mAnim.VRender(
            // Note: OG converted to FP and back here but its pointless
            mX1,
            mY1,
            ppOt,
            (mX2 - mX1) + 1,
            (mY2 - mY1) + 1);

    }
}
