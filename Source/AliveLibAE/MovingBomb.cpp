#include "stdafx.h"
#include "MovingBomb.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Sound/Midi.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "BaseAliveGameObject.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"

const TintEntry kMovingBombTints_55C734[4] = {
    {LevelIds_s8::eBarracks_6, 97u, 97u, 97u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};

ALIVE_VAR(1, 0x5C300C, MovingBomb*, gMovingBomb_5C300C, nullptr);

MovingBomb::MovingBomb(Path_MovingBomb* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    mGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    SetType(AETypes::eTimedMine_or_MovingBomb_10);

    const AnimRecord& rec = AnimRec(AnimId::MovingBomb);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mAnim.mRenderMode = TPageAbr::eBlend_0;
    field_118_state = States::eTriggeredBySwitch_1;

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = 0;
        mAnim.mRenderLayer = Layer::eLayer_BombMineCar_Half_16;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = 1;
        mAnim.mRenderLayer = Layer::eLayer_BombMineCar_35;
    }

    mXPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_124_speed = FP_FromRaw(pTlv->field_10_speed << 8);
    mVelX = FP_FromRaw(pTlv->field_1C_start_speed << 8);
    field_128_start_moving_switch_id = pTlv->field_12_start_moving_switch_id;
    field_120_timer = sGnFrame_5C1B84;
    field_11C_tlvInfo = tlvInfo;
    field_12C_max = 0;
    field_12A_min = 0;
    field_136_persist_offscreen = pTlv->field_1E_persist_offscreen;
    field_130_sound_channels = 0;

    if (pTlv->field_14_bTriggered_by_alarm == Choice_short::eYes_1)
    {
        field_118_state = States::eTriggeredByAlarm_0;
        mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    }

    SetTint(&kMovingBombTints_55C734[0], gMap.mCurrentLevel);

    field_134_disabled_resources = pTlv->field_1A_disabled_resources;
    if (!(field_134_disabled_resources & 1))
    {
        Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);
    }

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
    Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kAbeblowResID);
    Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kSlogBlowResID);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            mXPos,
            mYPos,
            mXPos + FP_FromInteger(24),
            mYPos + FP_FromInteger(24),
            &mCollisionLine,
            &hitX,
            &hitY,
            0x100))
    {
        mYPos = hitY;
        mXPos = hitX;
    }

    mShadow = ae_new<Shadow>();
}

MovingBomb::~MovingBomb()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(mObjectId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        mObjectId = -1;
    }

    if (field_118_state >= States::eBlowingUp_6)
    {
        Path::TLV_Reset(field_11C_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_11C_tlvInfo, -1, 0, 0);
    }

    if (gMovingBomb_5C300C == this)
    {
        if (field_130_sound_channels)
        {
            SND_Stop_Channels_Mask(field_130_sound_channels);
            field_130_sound_channels = 0;
        }
        gMovingBomb_5C300C = 0;
    }
}

void MovingBomb::BlowUp()
{
    mGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);
    field_118_state = States::eBlowingUp_6;
    mVelY = FP_FromInteger(0);
    field_120_timer = sGnFrame_5C1B84 + 1;
    SFX_Play_Mono(SoundEffect::GreenTick_2, 100, mSpriteScale);
}

void MovingBomb::VRender(PrimHeader** ot)
{
    if (mAnim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void MovingBomb::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    if (field_136_persist_offscreen == Choice_short::eNo_0)
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP xDelta = FP_Abs(sControlledCharacter->mXPos - mXPos);
    if (xDelta > FP_FromInteger(750))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP yDelta = FP_Abs(sControlledCharacter->mYPos - mYPos);
    if (yDelta > FP_FromInteger(520))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }
}

void MovingBomb::FollowLine()
{
    if (mCollisionLine)
    {
        mCollisionLine = mCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
    }
}

s16 MovingBomb::VTakeDamage(BaseGameObject* pFrom)
{
    if (mGameObjectFlags.Get(BaseGameObject::eDead) || mHealth <= FP_FromInteger(0))
    {
        return 1;
    }

    switch (pFrom->Type())
    {
        case AETypes::eAbilityRing_104:
        case AETypes::eExplosion_109:
        case AETypes::eShrykull_121:
        {
            mHealth = FP_FromInteger(0);
            ae_new<Explosion>(mXPos, mYPos, mSpriteScale, 0);

            ae_new<Gibs>(GibType::Metal_5, mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), mSpriteScale, 0);

            field_118_state = States::eKillMovingBomb_7;

            mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            field_120_timer = sGnFrame_5C1B84 + 4;
        }
            return 0;

        case AETypes::eElectrocute_150:
            field_118_state = States::eKillMovingBomb_7;
            return 0;

        default:
            return 1;
    }
}

void MovingBomb::VOnThrowableHit(BaseGameObject* /*pObj*/)
{
    if (!mAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        BlowUp();
    }
}

s16 MovingBomb::HitObject()
{
    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this)
        {
            if (pObj->mAliveGameObjectFlags.Get(Flags_114::e114_Bit6_SetOffExplosives))
            {
                if (pObj->mHealth > FP_FromInteger(0))
                {
                    PSX_RECT objRect = {};
                    pObj->VGetBoundingRect(&objRect, 1);
                    if (bRect.x <= objRect.w && bRect.w >= objRect.x && bRect.h >= objRect.y && bRect.y <= objRect.h && pObj->mSpriteScale == mSpriteScale && pObj->mPathNumber == mPathNumber)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void MovingBomb::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_118_state < States::eBlowingUp_6)
    {
        if (HitObject())
        {
            if (!mAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
            {
                BlowUp();
            }
        }
    }

    if (gMovingBomb_5C300C == nullptr || gMovingBomb_5C300C == this)
    {
        if (mAnim.field_92_current_frame != 0 && mAnim.field_92_current_frame != 7)
        {
            gMovingBomb_5C300C = this;
        }
        else
        {
            if (field_130_sound_channels)
            {
                SND_Stop_Channels_Mask(field_130_sound_channels);
            }

            if (VIsObjNearby(FP_FromInteger(700), sActiveHero))
            {
                const FP yDelta = FP_Abs(sActiveHero->mYPos - mYPos);
                if (yDelta <= FP_FromInteger(700))
                {
                    if (field_118_state == States::eWaitABit_4)
                    {
                        field_130_sound_channels = SFX_Play_Mono(SoundEffect::SecurityOrb_48, 55, mSpriteScale);
                    }
                    else
                    {
                        field_130_sound_channels = SFX_Play_Mono(SoundEffect::SecurityOrb_48, 80, mSpriteScale);
                    }
                    gMovingBomb_5C300C = this;
                }
                else
                {
                    if (field_118_state == States::eWaitABit_4)
                    {
                        field_130_sound_channels = 0;
                        gMovingBomb_5C300C = this;
                    }
                    else
                    {
                        field_130_sound_channels = SFX_Play_Mono(SoundEffect::SecurityOrb_48, 12, mSpriteScale);
                        gMovingBomb_5C300C = this;
                    }
                }
            }
        }
    }

    switch (field_118_state)
    {
        case States::eTriggeredByAlarm_0:
            if (Event_Get(kEventAlarm))
            {
                mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eTriggeredBySwitch_1:
            if (SwitchStates_Get(field_128_start_moving_switch_id))
            {
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eMoving_2:
            if (mVelX < field_124_speed)
            {
                mVelX += (mSpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            mPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                TlvTypes::MovingBombStopper_53);

            if (mPathTLV)
            {
                auto pStopper = static_cast<Path_MovingBombStopper*>(mPathTLV);
                field_12A_min = pStopper->field_10_min;
                field_12C_max = pStopper->field_12_max;
                field_118_state = States::eStopMoving_3;
            }
            break;

        case States::eStopMoving_3:
            mVelX -= (mSpriteScale * FP_FromDouble(0.5));
            if (mVelX < FP_FromInteger(0))
            {
                field_118_state = States::eWaitABit_4;
                field_120_timer = sGnFrame_5C1B84 + Math_RandomRange(field_12A_min, field_12C_max);
            }

            FollowLine();
            break;

        case States::eWaitABit_4:
            if (field_120_timer <= static_cast<s32>(sGnFrame_5C1B84))
            {
                field_118_state = States::eToMoving_5;
            }
            break;

        case States::eToMoving_5:
            if (mVelX < field_124_speed)
            {
                mVelX += (mSpriteScale * FP_FromDouble(0.5));
            }

            FollowLine();

            mPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                TlvTypes::MovingBombStopper_53);

            if (!mPathTLV)
            {
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eBlowingUp_6:
            if (field_120_timer <= static_cast<s32>(sGnFrame_5C1B84))
            {
                SFX_Play_Mono(SoundEffect::GreenTick_2, 100, mSpriteScale);

                mHealth = FP_FromInteger(0);

                ae_new<Explosion>(
                    mXPos,
                    mYPos,
                    mSpriteScale,
                    0);

                ae_new<Gibs>(
                    GibType::Metal_5,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(5),
                    mSpriteScale,
                    0);

                field_118_state = States::eKillMovingBomb_7;
                mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                field_120_timer = sGnFrame_5C1B84 + 4;
            }
            break;

        case States::eKillMovingBomb_7:
            if (field_120_timer <= static_cast<s32>(sGnFrame_5C1B84))
            {
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        default:
            return;
    }
}
