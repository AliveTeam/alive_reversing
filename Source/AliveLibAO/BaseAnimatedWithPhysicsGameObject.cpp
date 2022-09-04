#include "stdafx_ao.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "ResourceManager.hpp"
#include "AnimResources.hpp"
#include "Particle.hpp"
#include "Sfx.hpp"
#include "Math.hpp"
#include "../relive_lib/GameType.hpp"

namespace AO {

BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject()
    : IBaseAnimatedWithPhysicsGameObject(0)
{
    mVisualFlags.Set(VisualFlags::eApplyShadowZoneColour);

    mCurrentPath = gMap.mCurrentPath;
    mCurrentLevel = gMap.mCurrentLevel;
	
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    mXPos = FP_FromInteger(0);
    mYPos = FP_FromInteger(0);

	// TODO: This can be removed if everything uses the tint table or passes down the RGB
	if (GetGameType() == GameType::eAe)
	{
    	mRGB.SetRGB(127, 127, 127);
	}
	else
	{
		mRGB.SetRGB(105, 105, 105);
	}
	
    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    mBaseGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mBaseGameObjectFlags.Set(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5);

    mSpriteScale = FP_FromInteger(1);

    mScale = Scale::Fg;
	
    mXOffset = 0;
	
	// TODO: factor this out, not yet known why AO needs an offset of 5
	if (GetGameType() == GameType::eAe)
	{
    	mYOffset = 0;
	}
	else
	{
		mYOffset = 5;
	}
	
    mShadow = nullptr;
}

BaseAnimatedWithPhysicsGameObject::~BaseAnimatedWithPhysicsGameObject()
{
    if (!mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        if (mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
        {
            gObjListDrawables->Remove_Item(this);
            mAnim.VCleanUp();
        }

        delete mShadow;
    }
}

void BaseAnimatedWithPhysicsGameObject::VRender(PrimHeader** ppOt)
{
    if (mAnim.mFlags.Get(AnimFlags::eBit3_Render))
    {
        // Only render if in the active level, path and camera
        if (gMap.mCurrentPath == mCurrentPath
            && gMap.mCurrentLevel == mCurrentLevel
            && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            mAnim.field_14_scale = mSpriteScale;

            s16 r = mRGB.r;
            s16 g = mRGB.g;
            s16 b = mRGB.b;

            const PSX_RECT boundingRect = VGetBoundingRect();

            if (mVisualFlags.Get(VisualFlags::eApplyShadowZoneColour))
            {
                ShadowZone::ShadowZones_Calculate_Colour(
                    FP_GetExponent(mXPos),         // Left side
                    (boundingRect.y + boundingRect.h) / 2, // Middle of Height
                    mScale,
                    &r,
                    &g,
                    &b);
            }

            mAnim.mRed = static_cast<u8>(r);
            mAnim.mGreen = static_cast<u8>(g);
            mAnim.mBlue = static_cast<u8>(b);

            mAnim.VRender(
                // FP_GetExponent((FP_FromInteger(mXOffset) + mXPos - pScreenManager->CamXPos())),

                //pScreenManager->CamXPos()
                //  return mCamPos->x - FP_FromInteger(mCamXOff);

                FP_GetExponent((FP_FromInteger(pScreenManager->mCamXOff + mXOffset)) + mXPos - pScreenManager->mCamPos->x),
                FP_GetExponent((FP_FromInteger(pScreenManager->mCamYOff + mYOffset)) + mYPos - pScreenManager->mCamPos->y),
                ppOt,
                0,
                0);

            PSX_RECT frameRect = {};
            mAnim.Get_Frame_Rect(&frameRect);
            pScreenManager->InvalidateRectCurrentIdx(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h);

            if (mShadow)
            {
                mShadow->Calculate_Position(
                    mXPos,
                    mYPos,
                    &frameRect,
                    mSpriteScale,
                    mScale);
                mShadow->Render(ppOt);
            }
        }
    }
}

void BaseAnimatedWithPhysicsGameObject::Animation_Init(s32 frameTableOffset, u16 maxW, u16 maxH, u8** ppAnimData)
{
    if (mAnim.Init(
        frameTableOffset,
        maxW,
        maxH,
        this,
        ppAnimData))
    {
        if (mSpriteScale == FP_FromInteger(1))
        {
            mAnim.mRenderLayer = Layer::eLayer_27;
        }
        else
        {
            mAnim.mRenderLayer = Layer::eLayer_8;
            mScale = Scale::Bg;
        }

        const bool added = gObjListDrawables->Push_Back(this) ? true : false;
        if (added)
        {
            mAnim.mRenderMode = TPageAbr::eBlend_0;
            mAnim.mFlags.Clear(AnimFlags::eBit16_bBlending);
            mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
        }
        else
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
        }
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BaseAnimatedWithPhysicsGameObject::Animation_Init(AnimId animId, u8** ppAnimData)
{
    const AnimRecord& rec = PerGameAnimRec(animId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppAnimData);
}

CameraPos BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera()
{
    const PSX_RECT rect = VGetBoundingRect();
    return gMap.Rect_Location_Relative_To_Active_Camera(&rect, 0);
}

void BaseAnimatedWithPhysicsGameObject::DeathSmokeEffect(bool bPlaySound)
{
    // note: mudokons used % 4
    if (!(sGnFrame % 5))
    {
        New_Smoke_Particles(
            mXPos + (FP_FromInteger(Math_RandomRange(-24, 24)) * mSpriteScale),
            mYPos - FP_FromInteger(6),
            mSpriteScale / FP_FromInteger(2),
            2,
            0);

        if (bPlaySound == true)
        {
            SFX_Play_Pitch(relive::SoundEffects::Vaporize, 25, FP_GetExponent(FP_FromInteger(2200) * mSpriteScale));
        }
    }
}

void BaseAnimatedWithPhysicsGameObject::VOnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn)
{
    if (pObjList)
    {
        for (s32 i = 0; i < pObjList->Size(); i++)
        {
            BaseGameObject* pObjIter = pObjList->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5))
            {
                if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
                {
                    BaseAnimatedWithPhysicsGameObject* pObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObjIter);
                    const PSX_RECT bRect = pObj->VGetBoundingRect(startingPointIdx);
                    if (xy.x <= bRect.w && wh.x >= bRect.x && wh.y >= bRect.y && xy.y <= bRect.h)
                    {
                        if (!(this->*(pFn))(pObj))
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}

s16 BaseAnimatedWithPhysicsGameObject::VIsObjNearby(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj)
{
    FP distance = FP_Abs(pOtherObj->mXPos - mXPos);
	// TODO: Factor out
	if (GetGameType() == GameType::eAo)
	{
 		if (distance > FP_FromInteger(400))
    	{
        	distance += ScaleToGridSize(mSpriteScale) - FP_FromInteger(656);
    	}	
	}
    return distance <= radius;
}

s16 BaseAnimatedWithPhysicsGameObject::VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->mXPos < mXPos && pOther->mVelX > mVelX)
    {
        // Its before our xpos but its velocity is moving towards our xpos!
        return TRUE;
    }

    if (pOther->mXPos > mXPos && pOther->mVelX < mVelX)
    {
        // Its after our xpos but its velocity is moving towards our xpos!
        return TRUE;
    }

    // Not heading our way
    return FALSE;
}

// Muds use this to face "away" from Abe when stood on the same grid block. Also used to follow Abe in the correct direction etc.
s16 BaseAnimatedWithPhysicsGameObject::VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->mXPos == mXPos
        && pOther->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) != mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are in the same spot as us, so they can only be facing us if they are NOT facing the same way.
        // This seems strange but its what causes muds to keep changing direction if you turn while you are stood in the same grid as them.
        return TRUE;
    }
    else if (pOther->mXPos > mXPos && !mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the right of us and facing left
        return TRUE;
    }
    else if (pOther->mXPos < mXPos && mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the left of using and facing right
        return TRUE;
    }

    return FALSE;
}

// This is how Scrabs, Fleeches (and probably other stuff) know you are on the same "floor"
s16 BaseAnimatedWithPhysicsGameObject::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    // Get bounding rects
    const PSX_RECT ourRect = VGetBoundingRect();
    const PSX_RECT theirRect = pOther->VGetBoundingRect();

    // Get mid Y of each
    const s32 theirMidY = (theirRect.h + theirRect.y) / 2;
    const s32 ourMidY = (ourRect.h + ourRect.y) / 2;

    if (theirMidY <= ourRect.h && theirMidY >= ourRect.y)
    {
        return TRUE;
    }

    if (ourMidY <= theirRect.h && ourMidY >= theirRect.y)
    {
        return TRUE;
    }

    return FALSE;
}

void BaseAnimatedWithPhysicsGameObject::VStackOnObjectsOfType(ReliveTypes typeToFind)
{
    static const s16 offsets[6] = {
        0, 3, -3, 6, -6, 2};

    s32 array_idx = 0;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == typeToFind && pObj != this)
        {
            array_idx++;
            if (array_idx >= ALIVE_COUNTOF(offsets))
            {
                array_idx = 0;
            }
        }
    }

    mXOffset = offsets[array_idx];
}

void BaseAnimatedWithPhysicsGameObject::VOnPickUpOrSlapped()
{
    // Empty
}

void BaseAnimatedWithPhysicsGameObject::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    // Empty
}

PSX_RECT BaseAnimatedWithPhysicsGameObject::VGetBoundingRect(s32 pointIdx)
{
    const FrameInfoHeader* pAnimFrameHeader = mAnim.Get_FrameHeader(-1);

    PSX_RECT rect = {};
    // Normally this data is 3 points, one that is the frame offset and then 2 that make up the bounding rect.
    // So usually pointIdx is 1. However the way the data is structured it could be anything to treat any index
    // into the array of points as a unique bounding rectangle. Also it appears there can be more than 3 points
    // supported in the data too.
    rect.x = pAnimFrameHeader->field_8_data.points[pointIdx].x;
    rect.y = pAnimFrameHeader->field_8_data.points[pointIdx].y;
    rect.w = pAnimFrameHeader->field_8_data.points[pointIdx + 1].x;
    rect.h = pAnimFrameHeader->field_8_data.points[pointIdx + 1].y;

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        std::swap(rect.x, rect.w);
        rect.x = -rect.x;
        rect.w = -rect.w;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit6_FlipY))
    {
        std::swap(rect.y, rect.h);
        rect.y = -rect.y;
        rect.h = -rect.h;
    }

    rect.x = FP_GetExponent((FP_FromInteger(rect.x) * mSpriteScale));
    rect.y = FP_GetExponent((FP_FromInteger(rect.y) * mSpriteScale));
    rect.w = FP_GetExponent((FP_FromInteger(rect.w) * mSpriteScale));
    rect.h = FP_GetExponent((FP_FromInteger(rect.h) * mSpriteScale));

    const s16 xpos = FP_GetExponent(mXPos);
    const s16 ypos = FP_GetExponent(mYPos);

    rect.x += xpos;
    rect.y += ypos;
    rect.w += xpos;
    rect.h += ypos;

    return rect;
}

void BaseAnimatedWithPhysicsGameObject::SetTint(const TintEntry* pTintArray, EReliveLevelIds level_id)
{
    while (pTintArray->field_0_level != level_id)
    {
        if (pTintArray->field_0_level == level_id || pTintArray->field_0_level == EReliveLevelIds::eNone)
        {
            break;
        }
        pTintArray++;
    }

    mRGB.SetRGB(pTintArray->field_1_r, pTintArray->field_2_g, pTintArray->field_3_b);
}

// AO only
BaseAnimatedWithPhysicsGameObject::BetweenCamPos BaseAnimatedWithPhysicsGameObject::BetweenCameras_418500()
{
    // TODO: Try to understand how the hell these calcs are supposed to work
    const s32 xPosMinusHalfCameraSpace = FP_GetExponent(mXPos - FP_FromInteger(512 / 2));
    if (xPosMinusHalfCameraSpace / 512 % 2)
    {
        return BetweenCamPos::Left_1;
    }
    // TODO :This seems wrong, won't it always be false ??
    else if (FP_FromInteger(xPosMinusHalfCameraSpace % 512) > FP_FromInteger(640))
    {
        return BetweenCamPos::Right_2;
    }
    else
    {
        return BetweenCamPos::None_0;
    }
}

} // namespace AO
