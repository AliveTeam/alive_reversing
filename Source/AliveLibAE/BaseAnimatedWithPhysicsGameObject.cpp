#include "stdafx.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "BaseAliveGameObject.hpp"
#include "AnimResources.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"

BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject(s16 resourceArraySize)
    : IBaseAnimatedWithPhysicsGameObject(resourceArraySize)
{
    mApplyShadows &= ~2;
    mApplyShadows |= 1;

    mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mCurrentPath;
    mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mCurrentLevel;
	
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(0);

    mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(127, 127, 127);

    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    mBaseGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mBaseGameObjectFlags.Set(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5);

    mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);

    mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
	
    mBaseAnimatedWithPhysicsGameObject_XOffset = 0;
    mBaseAnimatedWithPhysicsGameObject_YOffset = 0;

    mShadow = nullptr;
}

BaseAnimatedWithPhysicsGameObject::~BaseAnimatedWithPhysicsGameObject()
{
    if (!mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        if (mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
        {
            gObjListDrawables->Remove_Item(this);
            mBaseAnimatedWithPhysicsGameObject_Anim.VCleanUp();
        }

        delete mShadow;
    }
}

void BaseAnimatedWithPhysicsGameObject::VRender(PrimHeader** ppOt)
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        // Only render if in the active level, path and camera
        if (gMap.mCurrentPath == mBaseAnimatedWithPhysicsGameObject_PathNumber
            && gMap.mCurrentLevel == mBaseAnimatedWithPhysicsGameObject_LvlNumber
            && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

            s16 r = mBaseAnimatedWithPhysicsGameObject_RGB.r;
            s16 g = mBaseAnimatedWithPhysicsGameObject_RGB.g;
            s16 b = mBaseAnimatedWithPhysicsGameObject_RGB.b;

            const PSX_RECT boundingRect = VGetBoundingRect();

            if (mApplyShadows & 1)
            {
                ShadowZone::ShadowZones_Calculate_Colour(
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),         // Left side
                    (boundingRect.y + boundingRect.h) / 2, // Middle of Height
                    mBaseAnimatedWithPhysicsGameObject_Scale,
                    &r,
                    &g,
                    &b);
            }

            mBaseAnimatedWithPhysicsGameObject_Anim.mRed = static_cast<u8>(r);
            mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = static_cast<u8>(g);
            mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = static_cast<u8>(b);

            mBaseAnimatedWithPhysicsGameObject_Anim.VRender(
                FP_GetExponent((FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_XOffset) + mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos())),
                FP_GetExponent((FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_YOffset) + mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos())),
                ppOt,
                0,
                0);

            PSX_RECT frameRect = {};
            mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Rect(&frameRect);
            pScreenManager->InvalidateRectCurrentIdx(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h);

            if (mShadow)
            {
                mShadow->Calculate_Position(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    &frameRect,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    mBaseAnimatedWithPhysicsGameObject_Scale);
                mShadow->Render(ppOt);
            }
        }
    }
}


void BaseAnimatedWithPhysicsGameObject::Animation_Init(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, s16 bAddToDrawableList, u8 bOwnsPalData)
{
    FrameTableOffsetExists(frameTableOffset, true, maxW, maxH);
    if (mBaseAnimatedWithPhysicsGameObject_Anim.Init(
            frameTableOffset,
            gObjList_animations_5C1A24,
            this,
            static_cast<s16>(maxW),
            static_cast<s16>(maxH),
            ppAnimData,
            bOwnsPalData,
            0,
            0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
            mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        }

        bool added = true;
        if (bAddToDrawableList)
        {
            added = gObjListDrawables->Push_Back(this) ? true : false;
        }

        if (added)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_0;
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
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

CameraPos BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera()
{
    const PSX_RECT rect = VGetBoundingRect();
    return gMap.Rect_Location_Relative_To_Active_Camera(&rect);
}

void BaseAnimatedWithPhysicsGameObject::DeathSmokeEffect(bool bPlaySound)
{
    // note: mudokons used % 4
    if (!(sGnFrame % 5))
    {
        New_Smoke_Particles(
            mBaseAnimatedWithPhysicsGameObject_XPos + (FP_FromInteger(Math_RandomRange(-24, 24)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(6),
            mBaseAnimatedWithPhysicsGameObject_SpriteScale / FP_FromInteger(2),
            2,
            128u, 128u, 128u);

        if (bPlaySound == true)
        {
            SFX_Play_Pitch(SoundEffect::Vaporize_79, 25, FP_GetExponent((FP_FromInteger(2200) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)));
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
                    if (xy.field_0_x <= bRect.w && xy.field_2_y <= bRect.h && wh.field_0_x >= bRect.x && wh.field_2_y >= bRect.y && mBaseAnimatedWithPhysicsGameObject_Scale == pObj->mBaseAnimatedWithPhysicsGameObject_Scale)
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
    FP distance = FP_Abs(pOtherObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos);
    return distance <= radius;
}

s16 BaseAnimatedWithPhysicsGameObject::VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->mBaseAnimatedWithPhysicsGameObject_XPos < mBaseAnimatedWithPhysicsGameObject_XPos && pOther->mBaseAnimatedWithPhysicsGameObject_VelX > mBaseAnimatedWithPhysicsGameObject_VelX)
    {
        // Its before our xpos but its velocity is moving towards our xpos!
        return TRUE;
    }

    if (pOther->mBaseAnimatedWithPhysicsGameObject_XPos > mBaseAnimatedWithPhysicsGameObject_XPos && pOther->mBaseAnimatedWithPhysicsGameObject_VelX < mBaseAnimatedWithPhysicsGameObject_VelX)
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
    if (pOther->mBaseAnimatedWithPhysicsGameObject_XPos == mBaseAnimatedWithPhysicsGameObject_XPos
        && pOther->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are in the same spot as us, so they can only be facing us if they are NOT facing the same way.
        // This seems strange but its what causes muds to keep changing direction if you turn while you are stood in the same grid as them.
        return TRUE;
    }
    else if (pOther->mBaseAnimatedWithPhysicsGameObject_XPos > mBaseAnimatedWithPhysicsGameObject_XPos && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the right of us and facing left
        return TRUE;
    }
    else if (pOther->mBaseAnimatedWithPhysicsGameObject_XPos < mBaseAnimatedWithPhysicsGameObject_XPos && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
    const s16 offsets[6] = {
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

    mBaseAnimatedWithPhysicsGameObject_XOffset = FP_GetExponent(FP_FromInteger(offsets[array_idx]) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
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
    const FrameInfoHeader* pAnimFrameHeader = mBaseAnimatedWithPhysicsGameObject_Anim.Get_FrameHeader(-1);

    PSX_RECT rect = {};
    // Normally this data is 3 points, one that is the frame offset and then 2 that make up the bounding rect.
    // So usually pointIdx is 1. However the way the data is structured it could be anything to treat any index
    // into the array of points as a unique bounding rectangle. Also it appears there can be more than 3 points
    // supported in the data too.
    rect.x = pAnimFrameHeader->field_8_data.points[pointIdx].x;
    rect.y = pAnimFrameHeader->field_8_data.points[pointIdx].y;
    rect.w = pAnimFrameHeader->field_8_data.points[pointIdx + 1].x;
    rect.h = pAnimFrameHeader->field_8_data.points[pointIdx + 1].y;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        std::swap(rect.x, rect.w);
        rect.x = -rect.x;
        rect.w = -rect.w;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit6_FlipY))
    {
        std::swap(rect.y, rect.h);
        rect.y = -rect.y;
        rect.h = -rect.h;
    }

    rect.x = FP_GetExponent((FP_FromInteger(rect.x) * mBaseAnimatedWithPhysicsGameObject_SpriteScale));
    rect.y = FP_GetExponent((FP_FromInteger(rect.y) * mBaseAnimatedWithPhysicsGameObject_SpriteScale));
    rect.w = FP_GetExponent((FP_FromInteger(rect.w) * mBaseAnimatedWithPhysicsGameObject_SpriteScale));
    rect.h = FP_GetExponent((FP_FromInteger(rect.h) * mBaseAnimatedWithPhysicsGameObject_SpriteScale));

    const s16 xpos = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
    const s16 ypos = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);

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

    mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(pTintArray->field_1_r, pTintArray->field_2_g, pTintArray->field_3_b);
}
