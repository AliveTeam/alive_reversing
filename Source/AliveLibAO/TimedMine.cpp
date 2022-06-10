#include "stdafx_ao.h"
#include "Function.hpp"
#include "TimedMine.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "LiftPoint.hpp"
#include "BaseBomb.hpp"
#include "ScreenManager.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"

namespace AO {

const TintEntry kTimedMineTints_4C3140[3] = {
    {EReliveLevelIds::eStockYards, 60u, 60u, 60u},
    {EReliveLevelIds::eStockYardsReturn, 60u, 60u, 60u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u},
};

TimedMine::TimedMine(Path_TimedMine* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eTimedMine;

    const AnimRecord& rec = AO::AnimRec(AnimId::TimedMine_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    field_1B8_flags.Clear(TimedMine_Flags_1B8::eStickToLiftPoint_0);
    field_10C_armed = 0;

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombRollingBall_Half_16;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombRollingBall_35;
    }

    const AnimRecord& tickRec = AO::AnimRec(AnimId::Bomb_RedGreenTick);
    if (field_118_anim.Init(
            tickRec.mFrameTableOffset,
            gObjList_animations_505564,
            this,
            tickRec.mMaxW,
            tickRec.mMaxH,
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kBombflshAOResID, 1, 0),
            1,
            0,
            0))
    {
        field_118_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
        field_118_anim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);
        field_118_anim.mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
        field_118_anim.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        field_118_anim.mRed = 128;
        field_118_anim.mGreen = 128;
        field_118_anim.mBlue = 128;
        field_118_anim.mRenderMode = TPageAbr::eBlend_1;
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }

    field_10E_ticks_before_explosion = pTlv->field_1E_ticks_before_explosion;
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 24);

    field_114_timer = sGnFrame;
    field_110_tlvInfo = tlvInfo;

    SetBaseAnimPaletteTint_4187C0(kTimedMineTints_4C3140, gMap.mCurrentLevel, 1005);
    mCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
    mCollectionRect.w = mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
    mCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;
    mCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mLiftPoint = nullptr;
}

TimedMine::~TimedMine()
{
    if (field_10C_armed != 1 || static_cast<s32>(sGnFrame) < field_114_timer)
    {
        gMap.TLV_Reset(field_110_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_110_tlvInfo, -1, 0, 1);
    }

    field_118_anim.VCleanUp();

    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
    }

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);
}

void TimedMine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (field_10C_armed != 1)
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

s16 TimedMine::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->mBaseGameObjectTypeId)
    {
        case ReliveTypes::eAbe:
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eExplosion:
        case ReliveTypes::eShrykull:
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            relive_new BaseBomb(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            field_10C_armed = 1;
            field_114_timer = sGnFrame;
            return 1;
        }
        default:
            return 0;
    }
}

void TimedMine::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera_4449C0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        field_118_anim.VRender(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(pScreenManager->mCamXOff) - pScreenManager->mCamPos->field_0_x),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(pScreenManager->mCamYOff - FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(14)))
                           - pScreenManager->mCamPos->field_4_y),
            ppOt,
            0,
            0);

        PSX_RECT pRect = {};
        field_118_anim.Get_Frame_Rect(&pRect);
        pScreenManager->InvalidateRect(
            pRect.x,
            pRect.y,
            pRect.w,
            pRect.h,
            pScreenManager->mIdx);
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void TimedMine::StickToLiftPoint()
{
    FP hitY = {};
    FP hitX = {};
    PathLine* pLine = nullptr;
    field_1B8_flags.Set(TimedMine_Flags_1B8::eStickToLiftPoint_0);
    if (sCollisions->RayCast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
            &pLine,
            &hitX,
            &hitY,
            (mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5)) ? 7 : 0x70))
    {
        if (pLine->field_8_type == eLineTypes ::eUnknown_32 ||
            pLine->field_8_type == eLineTypes::eUnknown_36)
        {
            if (ObjListPlatforms_50766C)
            {
                for (s32 i = 0; i < ObjListPlatforms_50766C->Size(); i++)
                {
                    BaseGameObject* pObj = ObjListPlatforms_50766C->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
                    {
                        auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                        const PSX_RECT pObjRect = pLiftPoint->VGetBoundingRect();

                        if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) > pObjRect.x && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) < pObjRect.w && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) < pObjRect.h)
                        {
                            mLiftPoint = pLiftPoint;
                            pLiftPoint->VAdd(this);
                            mLiftPoint->mBaseGameObjectRefCount++;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void TimedMine::VUpdate()
{
    auto pPlatform = static_cast<LiftPoint*>(mLiftPoint);
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!field_1B8_flags.Get(TimedMine_Flags_1B8::eStickToLiftPoint_0))
    {
        StickToLiftPoint();
    }

    if (pPlatform && pPlatform->OnAnyFloor())
    {
        mCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2);
        mCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        mCollectionRect.w = mBaseAnimatedWithPhysicsGameObject_XPos + ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2);
        mCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;
    }
    if (field_10C_armed == 1)
    {
        if (static_cast<s32>(sGnFrame) > (field_1B4_detonation_timer + field_1B0_gnFrame_2))
        {
            field_1B0_gnFrame_2 = sGnFrame;
            auto direction = gMap.GetDirection(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);
            SFX_Play_Camera(SoundEffect::GreenTick_3, 50, direction);

            //~7 limits the number to multiples of 8
            if (((field_114_timer - sGnFrame) & ~7) >= 18 * 8)
            {
                field_1B4_detonation_timer = 18;
            }
            else
            {
                field_1B4_detonation_timer = (field_114_timer - sGnFrame) / 8;
            }
        }
        if (static_cast<s32>(sGnFrame) >= field_114_timer)
        {
            relive_new BaseBomb(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void TimedMine::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new BaseBomb(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        0,
        mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    field_10C_armed = 1;
    field_114_timer = sGnFrame;
}

void TimedMine::VOnPickUpOrSlapped()
{
    if (field_10C_armed != 1)
    {
        field_10C_armed = 1;
        if ((s32)(field_10E_ticks_before_explosion & 0xFFFC) >= 72)
        {
            field_1B4_detonation_timer = 18;
        }
        else
        {
            field_1B4_detonation_timer = field_10E_ticks_before_explosion >> 2;
        }
        field_1B0_gnFrame_2 = sGnFrame;
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(804, 0);
        field_114_timer = sGnFrame + field_10E_ticks_before_explosion;
        field_118_anim.Set_Animation_Data(384, 0);
        SFX_Play_Mono(SoundEffect::GreenTick_3, 0, 0);
    }
}

} // namespace AO
