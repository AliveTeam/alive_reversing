#include "stdafx.h"
#include "TimedMine.hpp"
#include "UXB.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "ObjectIds.hpp"
#include "BaseBomb.hpp"
#include "LiftPoint.hpp"
#include "Grid.hpp"

static TintEntry sTimedMineTint_550EB8[1] = {{EReliveLevelIds::eNone, 127u, 127u, 127u}};


TimedMine::TimedMine(Path_TimedMine* pPath, TlvItemInfoUnion tlv)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eTimedMine_or_MovingBomb);

    const AnimRecord& rec = AnimRec(AnimId::TimedMine_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);

    field_1C4_flags.Clear(TimedMine_Flags_1C4::eStickToLiftPoint_0);

    field_118_armed = 0;

    if (pPath->field_14_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombMineCar_Half_16;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(1.0);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombMineCar_35;
    }

    InitBlinkAnimation(&field_124_animation);

    field_11A_ticks_before_explosion = pPath->field_16_ticks_before_explosion;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pPath->field_8_top_left.field_0_x + pPath->field_C_bottom_right.field_0_x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pPath->field_8_top_left.field_2_y);

    FP hitY = {};
    FP hitX = {};

    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    field_11C_tlv = tlv.all;
    field_120_gnframe = sGnFrame;
    SetBaseAnimPaletteTint(sTimedMineTint_550EB8, gMap.mCurrentLevel, kBombResID);

    const FP gridSnap = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    mCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (gridSnap / FP_FromDouble(2.0));
    mCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - gridSnap;
    mCollectionRect.w = (gridSnap / FP_FromDouble(2.0)) + mBaseAnimatedWithPhysicsGameObject_XPos;
    mCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;

    BaseAliveGameObject_PlatformId = -1;
}

void TimedMine::VUpdate()
{
    auto pPlatform = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (!field_1C4_flags.Get(TimedMine_Flags_1C4::eStickToLiftPoint_0))
    {
        StickToLiftPoint();
    }

    if (pPlatform && pPlatform->vOnAnyFloor())
    {
        mCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        mCollectionRect.w = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2)) + mBaseAnimatedWithPhysicsGameObject_XPos;
        mCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;
        mCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    if (field_118_armed == 1)
    {
        if (sGnFrame > field_1BC_gnFrame_2 + field_1C0_detonation_timer)
        {
            field_1BC_gnFrame_2 = sGnFrame;
            const CameraPos soundDir = gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
            SFX_Play_Camera(SoundEffect::GreenTick_2, 50, soundDir);

            // TODO: Modulus ?
            if (((field_120_gnframe - sGnFrame) & 0xFFFFFFF8) >= 144)
            {
                field_1C0_detonation_timer = 18;
            }
            else
            {
                field_1C0_detonation_timer = (field_120_gnframe - sGnFrame) >> 3;
            }
        }

        if (sGnFrame >= field_120_gnframe)
        {
            relive_new BaseBomb(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            mBaseGameObjectFlags.Set(Options::eDead);
        }
    }
}

void TimedMine::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        field_124_animation.VRender(
            FP_GetExponent((mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos())),
            FP_GetExponent((mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos() - FP_NoFractional(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(14)))),
            ppOt,
            0,
            0);

        PSX_RECT frameRect = {};
        field_124_animation.Get_Frame_Rect(&frameRect);

        pScreenManager->InvalidateRectCurrentIdx(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h);

        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void TimedMine::InitBlinkAnimation(Animation* pAnimation)
{
    const AnimRecord& tickRec = AnimRec(AnimId::Bomb_RedGreenTick);
    if (pAnimation->Init(tickRec.mFrameTableOffset, gAnimations, this, tickRec.mMaxW, tickRec.mMaxH, Add_Resource(ResourceManager::Resource_Animation, tickRec.mResourceId)))
    {
        pAnimation->mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
        pAnimation->mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
        pAnimation->mAnimFlags.Set(AnimFlags::eBit16_bBlending);
        pAnimation->field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        pAnimation->mRed = 128;
        pAnimation->mGreen = 128;
        pAnimation->mBlue = 128;
        pAnimation->mRenderMode = TPageAbr::eBlend_1;
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

void TimedMine::StickToLiftPoint()
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
            mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
            &pLine, &hitX, &hitY,
            (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {
        if (pLine->field_8_type == eLineTypes::eDynamicCollision_32 ||
            pLine->field_8_type == eLineTypes::eBackgroundDynamicCollision_36)
        {
            if (ObjList_5C1B78)
            {
                for (s32 i = 0; i < ObjList_5C1B78->Size(); i++)
                {
                    BaseGameObject* pObj = ObjList_5C1B78->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->Type() == ReliveTypes::eLiftPoint)
                    {
                        auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                        PSX_RECT bRect = pLiftPoint->VGetBoundingRect();
                        if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) > bRect.x && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) < bRect.w && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) < bRect.h)
                        {
                            pLiftPoint->VAdd(this);
                            BaseAliveGameObject_PlatformId = pObj->field_8_object_id;
                            return;
                        }
                    }
                }
            }
        }
    }
}

TimedMine::~TimedMine()
{
    auto pPlatform = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (field_118_armed != 1 || sGnFrame < field_120_gnframe)
    {
        Path::TLV_Reset(field_11C_tlv, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_11C_tlv, -1, 0, 1);
    }

    field_124_animation.VCleanUp();

    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = -1;
    }

    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
}

void TimedMine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    if (field_118_armed != 1)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 TimedMine::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eAbe:
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eExplosion:
        case ReliveTypes::eShrykull:
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            relive_new BaseBomb(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            field_118_armed = 1;
            field_120_gnframe = sGnFrame;
            return 1;
        }

        default:
            return 0;
    }
}

void TimedMine::VOnThrowableHit(BaseGameObject* /*pHitBy*/)
{
    relive_new BaseBomb(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    
    field_118_armed = 1;
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    field_120_gnframe = sGnFrame;
}

void TimedMine::VOnPickUpOrSlapped()
{
    if (field_118_armed != 1)
    {
        field_118_armed = 1;
        if ((s32)(field_11A_ticks_before_explosion & 0xFFFC) >= 72)
        {
            field_1C0_detonation_timer = 18;
        }
        else
        {
            field_1C0_detonation_timer = field_11A_ticks_before_explosion >> 2;
        }
        field_1BC_gnFrame_2 = sGnFrame;
        const AnimRecord& animRec = AnimRec(AnimId::TimedMine_Activated);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        const AnimRecord& flashRec = AnimRec(AnimId::Bomb_Flash);
        field_120_gnframe = sGnFrame + field_11A_ticks_before_explosion;
        field_124_animation.Set_Animation_Data(flashRec.mFrameTableOffset, 0);
        SFX_Play_Mono(SoundEffect::GreenTick_2, 0);
    }
}
