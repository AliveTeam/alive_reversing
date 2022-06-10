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
    : IBaseAnimatedWithPhysicsGameObject(0)
{
    mApplyShadows |= 1u;
    mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mCurrentPath;
    mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mCurrentLevel;

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_XOffset = 0;
    mShadow = nullptr;
    mBaseAnimatedWithPhysicsGameObject_Blue = 105;
    mBaseAnimatedWithPhysicsGameObject_Green = 105;
    mBaseAnimatedWithPhysicsGameObject_Red = 105;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
    mBaseAnimatedWithPhysicsGameObject_Scale = 1;
    mBaseAnimatedWithPhysicsGameObject_YOffset = 5;

    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);

    mBaseGameObjectFlags.Set(Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);
}

void BaseAnimatedWithPhysicsGameObject::Animation_Init_417FD0(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, s16 bAddToDrawableList)
{
    FrameTableOffsetExists(frameTableOffset, false, maxW, maxH);
    const auto init = mBaseAnimatedWithPhysicsGameObject_Anim.Init(
        frameTableOffset,
        gObjList_animations_505564,
        this,
        static_cast<u16>(maxW),
        static_cast<u16>(maxH),
        ppAnimData,
        1, 0, 0);

    if (init)
    {
        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
            mBaseAnimatedWithPhysicsGameObject_Scale = 0;
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
            mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
            gBaseGameObjects->Remove_Item(this);
        }
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
        mBaseGameObjectFlags.Set(Options::eDead);
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
    if (!(sGnFrame % 5))
    {
        New_Smoke_Particles_419A80(
            mBaseAnimatedWithPhysicsGameObject_XPos + (FP_FromInteger(Math_RandomRange_450F20(-24, 24)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(6),
            mBaseAnimatedWithPhysicsGameObject_SpriteScale / FP_FromInteger(2),
            2,
            0);

        if (bPlaySound == true)
        {
            SFX_Play_Pitch(SoundEffect::Vaporize_96, 25, FP_GetExponent(FP_FromInteger(2200) * mBaseAnimatedWithPhysicsGameObject_SpriteScale));
        }
    }
}

void BaseAnimatedWithPhysicsGameObject::VRender_417DA0(PrimHeader** ppOt)
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        // Only render if in the active level, path and camera
        if (gMap.mCurrentPath == mBaseAnimatedWithPhysicsGameObject_PathNumber && gMap.mCurrentLevel == mBaseAnimatedWithPhysicsGameObject_LvlNumber && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

            s16 r = mBaseAnimatedWithPhysicsGameObject_Red;
            s16 g = mBaseAnimatedWithPhysicsGameObject_Green;
            s16 b = mBaseAnimatedWithPhysicsGameObject_Blue;

            PSX_RECT boundingRect = {};
            VGetBoundingRect(&boundingRect, 1);

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
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + (FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos + mBaseAnimatedWithPhysicsGameObject_XOffset)) - pScreenManager_4FF7C8->mCamPos->field_0_x),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + mBaseAnimatedWithPhysicsGameObject_YOffset)) - pScreenManager_4FF7C8->mCamPos->field_4_y),
                ppOt,
                0,
                0);

            PSX_RECT frameRect = {};
            mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Rect(&frameRect);
            pScreenManager_4FF7C8->InvalidateRect(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_4FF7C8->mIdx);

            if (mShadow)
            {
                mShadow->Calculate_Position(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    &frameRect,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);
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

void BaseAnimatedWithPhysicsGameObject::VStackOnObjectsOfType(ReliveTypes typeToFind)
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

s16 BaseAnimatedWithPhysicsGameObject::VIsObj_GettingNear_On_X_418390(BaseAnimatedWithPhysicsGameObject* pOther)
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

s16 BaseAnimatedWithPhysicsGameObject::VIsObjNearby_418330(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj)
{
    FP x_abs = FP_Abs(pOtherObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos);
    if (x_abs > FP_FromInteger(400))
    {
        x_abs = x_abs + ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) - FP_FromInteger(656);
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

            if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5))
            {
                if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
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

    *pRect = rect;
    return pRect;
}

void BaseAnimatedWithPhysicsGameObject::SetTint_418750(const TintEntry* pTintArray, EReliveLevelIds level_id)
{
    while (pTintArray->field_0_level != level_id)
    {
        if (pTintArray->field_0_level == level_id || pTintArray->field_0_level == EReliveLevelIds::eNone)
        {
            break;
        }
        pTintArray++;
    }

    mBaseAnimatedWithPhysicsGameObject_Red = pTintArray->field_1_r;
    mBaseAnimatedWithPhysicsGameObject_Green = pTintArray->field_2_g;
    mBaseAnimatedWithPhysicsGameObject_Blue = pTintArray->field_3_b;
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

s16 BaseAnimatedWithPhysicsGameObject::SetBaseAnimPaletteTint_4187C0(const TintEntry* pTintArray, EReliveLevelIds lvl, s32 palId)
{
    const TintEntry* pIter = pTintArray;
    while (pIter->field_0_level != lvl)
    {
        if (pIter->field_0_level == EReliveLevelIds::eNone) // End of entries
        {
            return 0;
        }
        pIter++;
    }

    mBaseAnimatedWithPhysicsGameObject_Red = pIter->field_1_r;
    mBaseAnimatedWithPhysicsGameObject_Green = pIter->field_2_g;
    mBaseAnimatedWithPhysicsGameObject_Blue = pIter->field_3_b;

    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, palId, 1, 0);
    if (!ppRes)
    {
        return 0;
    }
    mBaseAnimatedWithPhysicsGameObject_Anim.LoadPal(ppRes, 0);
    ResourceManager::FreeResource_455550(ppRes);
    return 1;
}

void BaseAnimatedWithPhysicsGameObject::VStackOnObjectsOfType_418930(ReliveTypes typeToFind)
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

        if (pObjIter->mBaseGameObjectTypeId == typeToFind && pObjIter != this)
        {
            array_idx++;
            if (array_idx >= ALIVE_COUNTOF(offsets))
            {
                array_idx = 0;
            }
        }
    }

    mBaseAnimatedWithPhysicsGameObject_XOffset = offsets[array_idx];
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
    const s32 xPosMinusHalfCameraSpace = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(512 / 2));
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
