#include "stdafx_ao.h"
#include "Function.hpp"
#include "FallingItem.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "ScreenShake.hpp"
#include "ParticleBurst.hpp"
#include "Midi.hpp"
#include "SwitchStates.hpp"

namespace AO {

ALIVE_VAR(1, 0x4FFA54, FallingItem*, pPrimaryFallingItem_4FFA54, nullptr);

const FallingItem_Data sFallingItemData_4BAB20[16] = {
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // menu
    {AnimId::FallingMeat_Falling, AnimId::FallingMeat_Waiting, 66, 42},       // rupture farms
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // lines
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // forest
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // forest temple
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // stock yards
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // stock yards return
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // removed
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // desert
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // desert temple
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // credits
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // removed
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // board room
    {AnimId::FallingMeat_Falling, AnimId::FallingMeat_Waiting, 66, 42},       // rupture farms return
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // forest chase
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}};// desert escape

FallingItem::FallingItem(relive::Path_FallingItem* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);

    mBaseGameObjectTypeId = ReliveTypes::eRockSpawner;

    field_10C_tlvInfo = tlvId;

    const s32 lvlIdx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
    const AnimRecord& rec = AO::AnimRec(sFallingItemData_4BAB20[lvlIdx].field_0_falling_animId);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(sFallingItemData_4BAB20[lvlIdx].field_0_falling_animId, ppRes);

    mAnim.mRenderLayer = Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31;
    if (gMap.mCurrentLevel == EReliveLevelIds::eLines)
    {
        mRGB.SetRGB(77, 120, 190);
    }

    field_112_switch_id = pTlv->mSwitchId;
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }


    mFallInterval = pTlv->mFallInterval;
    mMaxFallingItems = pTlv->mMaxFallingItems;
    mRemainingFallingItems = pTlv->mMaxFallingItems;

    mResetSwitchIdAfterUse = pTlv->mResetSwitchIdAfterUse;
    field_122_do_sound_in_state_falling = 1;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    field_128_xpos = FP_FromInteger((pTlv->mBottomRightX + pTlv->mTopLeftX) / 2);
    field_12C_ypos = FP_FromInteger(pTlv->mBottomRightY);

    field_124_yPosStart = mYPos;
    field_110_state = State::eWaitForIdEnable_0;
    field_130_sound_channels = 0;

    // Not sure why this rupture farms primary item hack is required
    if (!pPrimaryFallingItem_4FFA54 && (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn))
    {
        pPrimaryFallingItem_4FFA54 = this;
        field_134_created_gnFrame = sGnFrame;
    }

    mShadow = relive_new Shadow();
}

FallingItem::~FallingItem()
{
    if (pPrimaryFallingItem_4FFA54 == this)
    {
        pPrimaryFallingItem_4FFA54 = nullptr;
    }
    Path::TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
}

void FallingItem::DamageHitItems()
{
    for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj != this)
        {
            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
            {
                BaseAnimatedWithPhysicsGameObject* pAliveObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                const PSX_RECT fallingItemRect = VGetBoundingRect();
                const PSX_RECT objRect = pAliveObj->VGetBoundingRect();

                if (fallingItemRect.x <= objRect.w && fallingItemRect.w >= objRect.x && fallingItemRect.y <= objRect.h && fallingItemRect.h >= objRect.y)
                {
                    if (pAliveObj->mSpriteScale == mSpriteScale)
                    {
                        static_cast<BaseAliveGameObject*>(pAliveObj)->VTakeDamage(this);
                    }
                }
            }
        }
    }
}

void FallingItem::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (pPrimaryFallingItem_4FFA54 == this)
    {
        if (!((sGnFrame - field_134_created_gnFrame) % 87))
        {
            SfxPlayMono(relive::SoundEffects::MeatsawOffscreen, 45);
        }

        if (!((sGnFrame - field_134_created_gnFrame) % 25))
        {
            SfxPlayMono(relive::SoundEffects::MeatsawIdle, 45);
        }
    }

    switch (field_110_state)
    {
        case State::eWaitForIdEnable_0:
            if (!SwitchStates_Get(field_112_switch_id))
            {
                return;
            }
            [[fallthrough]];

        case State::eGoWaitForDelay_1:
        {
            mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
            field_110_state = State::eWaitForFallDelay_2;
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            mAnim.Set_Animation_Data(sFallingItemData_4BAB20[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].field_4_waiting_animId, nullptr);
            field_11C_delay_timer = sGnFrame + mFallInterval;
            break;
        }

        case State::eWaitForFallDelay_2:
            if (static_cast<s32>(sGnFrame) >= field_11C_delay_timer)
            {
                field_110_state = State::eFalling_3;
                field_122_do_sound_in_state_falling = TRUE;
                field_130_sound_channels = SFX_Play_Pitch(relive::SoundEffects::AirStream, 50, -2600);
            }
            break;

        case State::eFalling_3:
        {
            if (field_122_do_sound_in_state_falling)
            {
                if (mYPos >= sActiveHero->mYPos - FP_FromInteger(120))
                {
                    field_122_do_sound_in_state_falling = 0;
                    SFX_Play_Pitch(relive::SoundEffects::AirStream, 127, -1300);
                }
            }

            DamageHitItems();

            if (mVelY < FP_FromInteger(20))
            {
                mVelY += mSpriteScale * FP_FromDouble(1.8);
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            if (sCollisions->Raycast(
                    mXPos,
                    mYPos,
                    mXPos,
                    mVelY + mYPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    mSpriteScale != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
                == 1)
            {
                mYPos = hitY;
                field_110_state = State::eSmashed_4;

                relive_new ScreenShake(0);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    relive_new ParticleBurst(
                        mXPos,
                        mYPos,
                        25,
                        mSpriteScale,
                        BurstType::eMeat_4);
                }
                else
                {
                    relive_new ParticleBurst(
                        mXPos,
                        mYPos,
                        25,
                        mSpriteScale,
                        BurstType::eFallingRocks_0);
                }

                relive_new ParticleBurst(
                    mXPos,
                    mYPos,
                    25,
                    mSpriteScale,
                    BurstType::eSticks_1);
            }
            else
            {
                mYPos += mVelY;
            }
            break;
        }

        case State::eSmashed_4:
        {
            if (field_130_sound_channels)
            {
                SND_Stop_Channels_Mask(field_130_sound_channels);
                field_130_sound_channels = 0;
            }

            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    SFX_Play_Pitch(relive::SoundEffects::KillEffect, 127, -700);
                    SfxPlayMono(relive::SoundEffects::FallingItemHit, 110);
                }
                else
                {
                    SND_SEQ_Play_477760(SeqId::Unknown_25, 1, 65, 65);
                    SFX_Play_Pitch(relive::SoundEffects::KillEffect, 90, -700);
                    SfxPlayMono(relive::SoundEffects::FallingItemHit, 33);
                    SFX_Play_Pitch(relive::SoundEffects::MeatsawUp, 80, -400);
                }
            }
            else
            {
                SfxPlayMono(relive::SoundEffects::FallingItemLand, 0);
                SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 110, -1536);
            }

            if (field_112_switch_id)
            {
                if (mResetSwitchIdAfterUse == relive::reliveChoice::eYes)
                {
                    SwitchStates_Do_Operation(field_112_switch_id, relive::reliveSwitchOp::eSetFalse);
                }
            }

            mRemainingFallingItems--;

            if ((mMaxFallingItems && mRemainingFallingItems <= 0) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, field_128_xpos, field_12C_ypos, 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                mAnim.Set_Animation_Data(sFallingItemData_4BAB20[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].field_0_falling_animId, nullptr);
                mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                mYPos = field_124_yPosStart;
                field_110_state = State::eWaitForIdEnable_0;
            }
            break;
        }

        default:
            return;
    }
}

void FallingItem::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    // Empty
}

void FallingItem::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel
        || gMap.mCurrentPath != gMap.mNextPath
        || field_110_state != State::eFalling_3)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
