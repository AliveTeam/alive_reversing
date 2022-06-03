#include "stdafx_ao.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "ResourceManager.hpp"
#include "AnimResources.hpp"
#include "Particle.hpp"
#include "Sfx.hpp"
#include "Math.hpp"

namespace AO {

BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject()
    : BaseGameObject(TRUE)
{
    mApplyShadows |= 1u;
    mPathNumber = gMap.mCurrentPath;
    mLvlNumber = gMap.mCurrentLevel;

    mVelX = FP_FromInteger(0);
    mXPos = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mYPos = FP_FromInteger(0);
    mXOffset = 0;
    mShadow = nullptr;
    mBlue = 105;
    mGreen = 105;
    mRed = 105;
    mSpriteScale = FP_FromInteger(1);
    mScale = 1;
    mYOffset = 5;

    mGameObjectFlags.Clear(Options::eCanExplode_Bit7);
    mGameObjectFlags.Clear(Options::eInteractive_Bit8);

    mGameObjectFlags.Set(Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    mGameObjectFlags.Set(Options::eDrawable_Bit4);
}

void BaseAnimatedWithPhysicsGameObject::Animation_Init_417FD0(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, s16 bAddToDrawableList)
{
    FrameTableOffsetExists(frameTableOffset, false, maxW, maxH);
    const auto init = mAnim.Init(
        frameTableOffset,
        gObjList_animations_505564,
        this,
        static_cast<u16>(maxW),
        static_cast<u16>(maxH),
        ppAnimData,
        1, 0, 0);

    if (init)
    {
        if (mSpriteScale == FP_FromInteger(1))
        {
            mAnim.mRenderLayer = Layer::eLayer_27;
        }
        else
        {
            mAnim.mRenderLayer = Layer::eLayer_8;
            mScale = 0;
        }

        bool added = true;
        if (bAddToDrawableList)
        {
            added = gObjList_drawables_504618->Push_Back(this) ? true : false;
        }

        if (added)
        {
            mAnim.mRenderMode = TPageAbr::eBlend_0;
            mAnim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
            mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
        }
        else
        {
            mGameObjectFlags.Set(Options::eListAddFailed_Bit1);
            gBaseGameObjects->Remove_Item(this);
        }
    }
    else
    {
        mGameObjectFlags.Set(Options::eListAddFailed_Bit1);
        mGameObjectFlags.Set(Options::eDead);
    }
}


CameraPos BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera()
{
    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);
    return gMap.Rect_Location_Relative_To_Active_Camera(&rect, 0);
}

void BaseAnimatedWithPhysicsGameObject::DeathSmokeEffect(bool bPlaySound)
{
    // note: mudokons used % 4
    if (!(gnFrameCount_507670 % 5))
    {
        New_Smoke_Particles_419A80(
            mXPos + (FP_FromInteger(Math_RandomRange_450F20(-24, 24)) * mSpriteScale),
            mYPos - FP_FromInteger(6),
            mSpriteScale / FP_FromInteger(2),
            2,
            0);

        if (bPlaySound == true)
        {
            SFX_Play_Pitch(SoundEffect::Vaporize_96, 25, FP_GetExponent(FP_FromInteger(2200) * mSpriteScale));
        }
    }
}

void BaseAnimatedWithPhysicsGameObject::VRender_417DA0(PrimHeader** ppOt)
{
    if (mAnim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        // Only render if in the active level, path and camera
        if (gMap.mCurrentPath == mPathNumber && gMap.mCurrentLevel == mLvlNumber && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            mAnim.field_14_scale = mSpriteScale;

            s16 r = mRed;
            s16 g = mGreen;
            s16 b = mBlue;

            PSX_RECT boundingRect = {};
            VGetBoundingRect(&boundingRect, 1);

            if (mApplyShadows & 1)
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
                FP_GetExponent(mXPos + (FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos + mXOffset)) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
                FP_GetExponent(mYPos + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + mYOffset)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
                ppOt,
                0,
                0);

            PSX_RECT frameRect = {};
            mAnim.Get_Frame_Rect(&frameRect);
            pScreenManager_4FF7C8->InvalidateRect(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_4FF7C8->field_2E_idx);

            if (mShadow)
            {
                mShadow->Calculate_Position(
                    mXPos,
                    mYPos,
                    &frameRect,
                    mSpriteScale);
                mShadow->Render(ppOt);
            }
        }
    }
}

void BaseAnimatedWithPhysicsGameObject::VRender(PrimHeader** ppOt)
{
    VRender_417DA0(ppOt);
}

void BaseAnimatedWithPhysicsGameObject::VOnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn)
{
    VOnCollisionWith_418080(xy, wh, pObjList, startingPointIdx, pFn);
}


PSX_RECT* BaseAnimatedWithPhysicsGameObject::VGetBoundingRect(PSX_RECT* pRect, s32 pointIdx)
{
    return VGetBoundingRect_418120(pRect, pointIdx);
}

s16 BaseAnimatedWithPhysicsGameObject::VIsObjNearby(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj)
{
    return VIsObjNearby_418330(radius, pOtherObj);
}

s16 BaseAnimatedWithPhysicsGameObject::VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VIsObj_GettingNear_On_X_418390(pOther);
}

s16 BaseAnimatedWithPhysicsGameObject::VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VIsFacingMe_4183F0(pOther);
}

s16 BaseAnimatedWithPhysicsGameObject::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VOnSameYLevel_418450(pOther);
}

void BaseAnimatedWithPhysicsGameObject::VStackOnObjectsOfType(Types typeToFind)
{
    VStackOnObjectsOfType_418930(typeToFind);
}

void BaseAnimatedWithPhysicsGameObject::VOnPickUpOrSlapped()
{
    // Empty
}

void BaseAnimatedWithPhysicsGameObject::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    // Empty
}

s16 BaseAnimatedWithPhysicsGameObject::VOnSameYLevel_418450(BaseAnimatedWithPhysicsGameObject* pOther)
{
    // Get bounding rects
    PSX_RECT ourRect = {};
    VGetBoundingRect(&ourRect, 1);

    PSX_RECT theirRect = {};
    pOther->VGetBoundingRect(&theirRect, 1);

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

s16 BaseAnimatedWithPhysicsGameObject::VIsFacingMe_4183F0(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->mXPos == mXPos
        && pOther->mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are in the same spot as us, so they can only be facing us if they are NOT facing the same way.
        // This seems strange but its what causes muds to keep changing direction if you turn while you are stood in the same grid as them.
        return TRUE;
    }
    else if (pOther->mXPos > mXPos && !mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the right of us and facing left
        return TRUE;
    }
    else if (pOther->mXPos < mXPos && mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the left of using and facing right
        return TRUE;
    }

    return FALSE;
}

s16 BaseAnimatedWithPhysicsGameObject::VIsObj_GettingNear_On_X_418390(BaseAnimatedWithPhysicsGameObject* pOther)
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

s16 BaseAnimatedWithPhysicsGameObject::VIsObjNearby_418330(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj)
{
    FP x_abs = FP_Abs(pOtherObj->mXPos - mXPos);
    if (x_abs > FP_FromInteger(400))
    {
        x_abs = x_abs + ScaleToGridSize(mSpriteScale) - FP_FromInteger(656);
    }
    return x_abs <= radius;
}

void BaseAnimatedWithPhysicsGameObject::VOnCollisionWith_418080(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn)
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

            if (pObjIter->mGameObjectFlags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5))
            {
                if (pObjIter->mGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
                {
                    auto pObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObjIter);

                    PSX_RECT bRect = {};
                    pObj->VGetBoundingRect(&bRect, startingPointIdx);
                    if (xy.field_0_x <= bRect.w && wh.field_0_x >= bRect.x && wh.field_2_y >= bRect.y && xy.field_2_y <= bRect.h)
                    {
                        if (!(this->*pFn)(pObj))
                        {
                            break;
                        }
                    }
                }
            }
        };
    }
}

PSX_RECT* BaseAnimatedWithPhysicsGameObject::VGetBoundingRect_418120(PSX_RECT* pRect, s32 pointIdx)
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

    if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        std::swap(rect.x, rect.w);
        rect.x = -rect.x;
        rect.w = -rect.w;
    }

    if (mAnim.mAnimFlags.Get(AnimFlags::eBit6_FlipY))
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

    *pRect = rect;
    return pRect;
}

void BaseAnimatedWithPhysicsGameObject::SetTint_418750(const TintEntry* pTintArray, LevelIds level_id)
{
    while (pTintArray->field_0_level != static_cast<s32>(level_id))
    {
        if (pTintArray->field_0_level == static_cast<s32>(level_id) || pTintArray->field_0_level == static_cast<s32>(LevelIds::eNone))
        {
            break;
        }
        pTintArray++;
    }

    mRed = pTintArray->field_1_r;
    mGreen = pTintArray->field_2_g;
    mBlue = pTintArray->field_3_b;
}

BaseAnimatedWithPhysicsGameObject::~BaseAnimatedWithPhysicsGameObject()
{
    if (!mGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        if (mGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
        {
            gObjList_drawables_504618->Remove_Item(this);
            mAnim.VCleanUp();
        }

        delete mShadow;
    }
}

s16 BaseAnimatedWithPhysicsGameObject::SetBaseAnimPaletteTint_4187C0(const TintEntry* pTintArray, LevelIds lvl, s32 palId)
{
    const TintEntry* pIter = pTintArray;
    while (pIter->field_0_level != static_cast<u8>(lvl))
    {
        if (pIter->field_0_level == -1) // End of entries
        {
            return 0;
        }
        pIter++;
    }

    mRed = pIter->field_1_r;
    mGreen = pIter->field_2_g;
    mBlue = pIter->field_3_b;

    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, palId, 1, 0);
    if (!ppRes)
    {
        return 0;
    }
    mAnim.LoadPal(ppRes, 0);
    ResourceManager::FreeResource_455550(ppRes);
    return 1;
}

void BaseAnimatedWithPhysicsGameObject::VStackOnObjectsOfType_418930(Types typeToFind)
{
    const s16 offsets[6] = {
        0, 3, -3, 6, -6, 2};

    s32 array_idx = 0;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mTypeId == typeToFind && pObjIter != this)
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


CameraPos BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera_417CC0()
{
    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);
    return gMap.Rect_Location_Relative_To_Active_Camera(&rect, 0);
}


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
