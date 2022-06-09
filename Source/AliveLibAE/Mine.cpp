#include "stdafx.h"
#include "Mine.hpp"
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
#include "stdlib.hpp"
#include "BaseBomb.hpp"
#include "Grid.hpp"

ALIVE_VAR(0, 0x5C3008, Mine*, sMineSFXOwner_5C3008, nullptr);

Mine::Mine(Path_Mine* pPath, TlvItemInfoUnion tlv)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eMine);

    const AnimRecord& rec = AnimRec(AnimId::Mine);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_118_detonating = 0;
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);

    if (pPath->field_14_scale != Scale_short::eFull_0)
    {
        if (pPath->field_14_scale == Scale_short::eHalf_1)
        {
            mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombMineCar_Half_16;
            mBaseAnimatedWithPhysicsGameObject_Scale = 0;
        }
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombMineCar_35;
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
    }

    const s32 v7 = pPath->field_8_top_left.field_0_x + pPath->field_C_bottom_right.field_0_x;
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(v7 / 2);
    const FP v8 = FP_FromInteger(pPath->field_8_top_left.field_2_y);
    mBaseAnimatedWithPhysicsGameObject_YPos = v8;

    FP hitY;
    FP hitX;

    if (sCollisions->Raycast(
            FP_FromInteger(v7 / 2),
            v8,
            FP_FromInteger(v7 / 2),
            v8 + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? 1 : 16)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }
    field_11C_tlv = tlv;
    field_120_gnframe = sGnFrame;
    const AnimRecord& mineFlashrec = AnimRec(AnimId::Mine_Flash);
    field_124_animation.Init(mineFlashrec.mFrameTableOffset, gObjList_animations_5C1A24, this, mineFlashrec.mMaxW, mineFlashrec.mMaxH, Add_Resource(ResourceManager::Resource_Animation, mineFlashrec.mResourceId), 1u, 0, 0);

    field_124_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    field_124_animation.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

    field_124_animation.mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
    field_124_animation.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_124_animation.mRed = 128;
    field_124_animation.mGreen = 128;
    field_124_animation.mBlue = 128;

    field_11A_disabled_resources = pPath->field_16_disabled_resources;

    field_1BC_flags.Clear(Mine_Flags_1BC::eBit1_PersistOffscreen);
    if (pPath->field_18_persist_offscreen == Choice_short::eYes_1)
    {
        field_1BC_flags.Set(Mine_Flags_1BC::eBit1_PersistOffscreen);
    }

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbebombResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID);

    if (!(field_11A_disabled_resources & 1))
    {
        Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);
    }
    if (!(field_11A_disabled_resources & 2))
    {
        Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID);
    }

    const FP gridSnap = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mApplyShadows |= 2u;

    mCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (gridSnap / FP_FromDouble(2.0));
    mCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - gridSnap;
    mCollectionRect.w = (gridSnap / FP_FromDouble(2.0)) + mBaseAnimatedWithPhysicsGameObject_XPos;
    mCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;
}


Mine::~Mine()
{
    if (field_118_detonating == 1)
    {
        Path::TLV_Reset(field_11C_tlv.all, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_11C_tlv.all, -1, 0, 0);
    }

    field_124_animation.VCleanUp();
    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);

    if (sMineSFXOwner_5C3008 == this)
    {
        sMineSFXOwner_5C3008 = nullptr;
    }
}

void Mine::VUpdate()
{
    const s16 onScreen = gMap.Is_Point_In_Current_Camera_4810D0(
        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
        mBaseAnimatedWithPhysicsGameObject_PathNumber,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        0);

    if (field_118_detonating)
    {
        if (field_118_detonating == 1 && sGnFrame >= field_120_gnframe)
        {
            relive_new BaseBomb(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            mBaseGameObjectFlags.Set(Options::eDead);
        }
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 1
            && (!sMineSFXOwner_5C3008 || sMineSFXOwner_5C3008 == this))
        {
            if (onScreen)
            {
                SFX_Play_Mono(SoundEffect::RedTick_3, 35);
                sMineSFXOwner_5C3008 = this;
            }
            else
            {
                sMineSFXOwner_5C3008 = nullptr;
            }
        }
        if (Mine::IsColliding())
        {
            field_118_detonating = 1;
            field_120_gnframe = sGnFrame;
        }
    }
    if (field_118_detonating != 1)
    {
        BaseGameObject* pEventObj = Event_Get(kEventDeathReset);
        if (pEventObj || mBaseAnimatedWithPhysicsGameObject_LvlNumber != gMap.mCurrentLevel || mBaseAnimatedWithPhysicsGameObject_PathNumber != gMap.mCurrentPath)
        {
            mBaseGameObjectFlags.Set(Options::eDead);
        }
    }
}

void Mine::VRender(PrimHeader** ppOt)
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap.Is_Point_In_Current_Camera_4810D0(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            this->field_124_animation.VRender(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->field_20_pCamPos->field_0_x),
                                                     FP_GetExponent(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_YOffset) + mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->field_20_pCamPos->field_4_y),
                                                     ppOt,
                                                     0,
                                                     0);

            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
    }
}

void Mine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel
        || gMap.mCurrentPath != gMap.mPath
        || !field_1BC_flags.Get(Mine_Flags_1BC::eBit1_PersistOffscreen))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

void Mine::VOnPickUpOrSlapped()
{
    if (field_118_detonating != 1)
    {
        field_118_detonating = 1;
        field_120_gnframe = sGnFrame + 5;
    }
}

void Mine::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new BaseBomb(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    field_118_detonating = 1;
}

s16 Mine::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        default:
            return 0;

        case ReliveTypes::eGreeter:
        case ReliveTypes::eAbe:
        case ReliveTypes::eMineCar:
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eExplosion:
        case ReliveTypes::eMudokon:
        case ReliveTypes::eShrykull:
            relive_new BaseBomb(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            field_118_detonating = 1;
            field_120_gnframe = sGnFrame;
            return 1;
    }
}

bool Mine::IsColliding()
{
    PSX_RECT mineBound;
    VGetBoundingRect(&mineBound, 1);

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        // e114_Bit6 May be "can set off explosives?"
        if (pObj->mBaseAliveGameObjectFlags.Get(e114_Bit6_SetOffExplosives) && pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
        {
            PSX_RECT objBound;
            pObj->VGetBoundingRect(&objBound, 1);

            s32 objX = FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos);
            s32 objY = FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos);

            if (objX > mineBound.x && objX < mineBound.w && objY < mineBound.h + 12 && mineBound.x <= objBound.w && mineBound.w >= objBound.x && mineBound.h >= objBound.y && mineBound.y <= objBound.h && pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
            {
                return 1;
            }
        }
    }

    return 0;
}
