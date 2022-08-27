#include "stdafx.h"
#include "Mine.hpp"
#include "UXB.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/VRam.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "BaseBomb.hpp"
#include "Grid.hpp"

ALIVE_VAR(0, 0x5C3008, Mine*, sMineSFXOwner_5C3008, nullptr);

Mine::Mine(relive::Path_Mine* pPath, TlvItemInfoUnion tlv)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eMine);

    const AnimRecord& rec = AnimRec(AnimId::Mine);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Mine, ppRes);

    field_118_detonating = 0;
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);

    if (pPath->mScale != relive::reliveScale::eFull)
    {
        if (pPath->mScale == relive::reliveScale::eHalf)
        {
            mSpriteScale = FP_FromDouble(0.5);
            mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_Half_16;
            mScale = Scale::Bg;
        }
    }
    else
    {
        mSpriteScale = FP_FromDouble(1);
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_35;
        mScale = Scale::Fg;
    }

    const s32 v7 = pPath->mTopLeftX + pPath->mBottomRightX;
    mXPos = FP_FromInteger(v7 / 2);
    const FP v8 = FP_FromInteger(pPath->mTopLeftY);
    mYPos = v8;

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
            mScale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }
    field_11C_tlv = tlv;
    field_120_gnframe = sGnFrame;
    const AnimRecord& mineFlashrec = AnimRec(AnimId::Mine_Flash);
    field_124_animation.Init(AnimId::Mine_Flash, this, Add_Resource(ResourceManager::Resource_Animation, mineFlashrec.mResourceId));

    field_124_animation.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    field_124_animation.mFlags.Set(AnimFlags::eBit16_bBlending);

    field_124_animation.mRenderLayer = mAnim.mRenderLayer;
    field_124_animation.field_14_scale = mSpriteScale;
    field_124_animation.mRed = 128;
    field_124_animation.mGreen = 128;
    field_124_animation.mBlue = 128;

    field_11A_disabled_resources = pPath->mDisabledResources;

    field_1BC_flags.Clear(Mine_Flags_1BC::eBit1_PersistOffscreen);
    if (pPath->mPersistOffscreen == relive::reliveChoice::eYes)
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

    const FP gridSnap = ScaleToGridSize(mSpriteScale);
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    mCollectionRect.x = mXPos - (gridSnap / FP_FromDouble(2.0));
    mCollectionRect.y = mYPos - gridSnap;
    mCollectionRect.w = (gridSnap / FP_FromDouble(2.0)) + mXPos;
    mCollectionRect.h = mYPos;
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
    const s16 onScreen = gMap.Is_Point_In_Current_Camera(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos,
        0);

    if (field_118_detonating)
    {
        if (field_118_detonating == 1 && sGnFrame >= field_120_gnframe)
        {
            relive_new BaseBomb(mXPos, mYPos, 0, mSpriteScale);
            mBaseGameObjectFlags.Set(Options::eDead);
        }
    }
    else
    {
        if (mAnim.mCurrentFrame == 1
            && (!sMineSFXOwner_5C3008 || sMineSFXOwner_5C3008 == this))
        {
            if (onScreen)
            {
                SfxPlayMono(SoundEffect::RedTick_3, 35);
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
        BaseGameObject* pEventObj = EventGet(kEventDeathReset);
        if (pEventObj || mCurrentLevel != gMap.mCurrentLevel || mCurrentPath != gMap.mCurrentPath)
        {
            mBaseGameObjectFlags.Set(Options::eDead);
        }
    }
}

void Mine::VRender(PrimHeader** ppOt)
{
    if (mAnim.mFlags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            this->field_124_animation.VRender(FP_GetExponent(mXPos - pScreenManager->CamXPos()),
                                                     FP_GetExponent(FP_FromInteger(mYOffset) + mYPos - pScreenManager->CamYPos()),
                                                     ppOt,
                                                     0,
                                                     0);

            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
    }
}

void Mine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel
        || gMap.mCurrentPath != gMap.mNextPath
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
    relive_new BaseBomb(mXPos, mYPos, 0, mSpriteScale);
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
            relive_new BaseBomb(mXPos, mYPos, 0, mSpriteScale);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            field_118_detonating = 1;
            field_120_gnframe = sGnFrame;
            return 1;
    }
}

bool Mine::IsColliding()
{
    const PSX_RECT mineBound = VGetBoundingRect();

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        // e114_Bit6 May be "can set off explosives?"
        if (pObj->mBaseAliveGameObjectFlags.Get(e114_Bit6_SetOffExplosives) && pObj->mAnim.mFlags.Get(AnimFlags::eBit3_Render))
        {
            const PSX_RECT objBound = pObj->VGetBoundingRect();

            s32 objX = FP_GetExponent(pObj->mXPos);
            s32 objY = FP_GetExponent(pObj->mYPos);

            if (objX > mineBound.x && objX < mineBound.w && objY < mineBound.h + 12 && mineBound.x <= objBound.w && mineBound.w >= objBound.x && mineBound.h >= objBound.y && mineBound.y <= objBound.h && pObj->mSpriteScale == mSpriteScale)
            {
                return 1;
            }
        }
    }

    return 0;
}
