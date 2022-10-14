#include "stdafx.h"
#include "FallingItem.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "ParticleBurst.hpp"
#include "../relive_lib/Particle.hpp"
#include "ScreenShake.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "Path.hpp"

const AnimId sFallingItemData_544DC0[15][2] = {
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::FallingCrate_Falling, AnimId::FallingCrate_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::AE_FallingRock_Falling, AnimId::AE_FallingRock_Waiting},
    {AnimId::FallingCrate_Falling, AnimId::FallingCrate_Waiting}};

FallingItem* pPrimaryFallingItem_5BC208 = nullptr;

void FallingItem::LoadAnimations()
{
    const AnimId FallingItemAnimIds[] =
    {
        AnimId::AE_FallingRock_Falling,
        AnimId::AE_FallingRock_Waiting,
        AnimId::FallingCrate_Falling,
        AnimId::FallingCrate_Waiting,
    };

    for (auto& animId : FallingItemAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AirExplosion));
}

FallingItem::FallingItem(relive::Path_FallingItem* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eRockSpawner);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    field_118_tlvInfo = tlvId;

    const s32 lvlIdx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    LoadAnimations();
    Animation_Init(GetAnimRes(sFallingItemData_544DC0[lvlIdx][0]));

    field_11E_switch_id = pTlv->mSwitchId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        GetAnimation().SetRenderLayer(Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12);
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
        GetAnimation().SetRenderLayer(Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31);
    }

    field_124_fall_interval = pTlv->mFallInterval;
    field_120_max_falling_items = pTlv->mMaxFallingItems;
    field_122_remaining_falling_items = pTlv->mMaxFallingItems;
    field_134_bHitDrillOrMineCar = FALSE;
    field_12C_reset_switch_id_after_use = pTlv->mResetSwitchIdAfterUse;
    field_12E_do_sound_in_state_falling = TRUE;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (mYPos > pScreenManager->CamYPos())
    {
        mYPos = pScreenManager->CamYPos();
    }

    field_138_xpos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    field_13C_ypos = FP_FromInteger(pTlv->mBottomRightY);
    field_130_yPosStart = mYPos;
    field_11C_state = State::eWaitForIdEnable_0;
    field_140_sound_channels = 0;

    if (!pPrimaryFallingItem_5BC208)
    {
        pPrimaryFallingItem_5BC208 = this;
        field_144_created_gnFrame = sGnFrame;
    }

    mShadow = relive_new Shadow();
}

 FallingItem::FallingItem(s32 xpos, s32 ypos, s32 scale, s32 id, s32 fallInterval, s32 numItems, s32 bResetIdAfterUse)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eRockSpawner);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    field_118_tlvInfo = Guid{};

    const s32 lvlIdx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));
    LoadAnimations();
    Animation_Init(GetAnimRes(sFallingItemData_544DC0[lvlIdx][0]));

    GetAnimation().SetRenderLayer(Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31);

    if (id)
    {
        field_11E_switch_id = static_cast<s16>(id);
    }
    else
    {
        field_11E_switch_id = 1;
    }

    if (scale)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    field_124_fall_interval = static_cast<s16>(fallInterval);

    field_120_max_falling_items = static_cast<s16>(numItems);
    field_122_remaining_falling_items = static_cast<s16>(numItems);

    const FP xFixed = FP_FromInteger(xpos);
    const FP yFixed = FP_FromInteger(ypos);

    field_12C_reset_switch_id_after_use = static_cast<relive::reliveChoice>(bResetIdAfterUse);
    field_134_bHitDrillOrMineCar = FALSE;
    field_12E_do_sound_in_state_falling = TRUE;
    mXPos = xFixed;
    mYPos = yFixed;
    field_138_xpos = xFixed;
    field_13C_ypos = yFixed;
    field_130_yPosStart = yFixed;
    field_11C_state = State::eWaitForIdEnable_0;
    field_140_sound_channels = 0;

    if (!pPrimaryFallingItem_5BC208)
    {
        pPrimaryFallingItem_5BC208 = this;
        field_144_created_gnFrame = sGnFrame;
    }

    mShadow = relive_new Shadow();
}

FallingItem::~FallingItem()
{
    if (pPrimaryFallingItem_5BC208 == this)
    {
        pPrimaryFallingItem_5BC208 = nullptr;
    }
    Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
}

void FallingItem::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || field_11C_state != State::eFalling_3)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void FallingItem::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    // The primary item controls the main sound effects, otherwise there would be a crazy amount of smashing sounds
    if (pPrimaryFallingItem_5BC208 == this)
    {
        if (!((sGnFrame - field_144_created_gnFrame) % 87))
        {
            if (mScale == Scale::Fg)
            {
                SfxPlayMono(relive::SoundEffects::FallingItemPresence1, 45);
            }
            else
            {
                SfxPlayMono(relive::SoundEffects::FallingItemPresence1, 20);
            }
        }

        if (!((sGnFrame - field_144_created_gnFrame) % 25))
        {
            if (mScale == Scale::Fg)
            {
                SfxPlayMono(relive::SoundEffects::FallingItemPresence2, 45);
            }
            else
            {
                SfxPlayMono(relive::SoundEffects::FallingItemPresence2, 20);
            }
        }
    }

    switch (field_11C_state)
    {
        case State::eWaitForIdEnable_0:
            if (field_11E_switch_id && SwitchStates_Get(field_11E_switch_id))
            {
                mBaseGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);
                field_11C_state = State::eWaitForFallDelay_2;
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);

                GetAnimation().Set_Animation_Data(GetAnimRes(sFallingItemData_544DC0[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]));

                field_128_fall_interval_timer = sGnFrame + field_124_fall_interval;
            }
            break;

        // TODO: Must only be set outside of the object
        case State::eGoWaitForDelay_1:
        {
            mBaseGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);
            field_11C_state = State::eWaitForFallDelay_2;
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);

            GetAnimation().Set_Animation_Data(GetAnimRes(sFallingItemData_544DC0[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]));

            field_128_fall_interval_timer = sGnFrame + field_124_fall_interval;
            break;
        }

        case State::eWaitForFallDelay_2:
            if (static_cast<s32>(sGnFrame) >= field_128_fall_interval_timer)
            {
                field_11C_state = State::eFalling_3;
                field_12E_do_sound_in_state_falling = TRUE;
                if (mScale == Scale::Fg)
                {
                    field_140_sound_channels = SFX_Play_Pitch(relive::SoundEffects::AirStream, 50, -2600);
                }
                else
                {
                    field_140_sound_channels = SFX_Play_Pitch(relive::SoundEffects::AirStream, 25, -2600);
                }
            }
            break;

        case State::eFalling_3:
        {
            if (field_12E_do_sound_in_state_falling)
            {
                if (mYPos >= sActiveHero->mYPos - FP_FromInteger(240 / 2))
                {
                    field_12E_do_sound_in_state_falling = FALSE;
                    if (mScale == Scale::Fg)
                    {
                        SFX_Play_Pitch(relive::SoundEffects::AirStream, 127, -1300);
                    }
                    else
                    {
                        SFX_Play_Pitch(relive::SoundEffects::AirStream, 64, -1300);
                    }
                }
            }

            DamageHitItems();

            if (mVelY < FP_FromInteger(20))
            {
                mVelY += mSpriteScale * FP_FromDouble(1.8);
            }

            PathLine* pathLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mXPos,
                    mYPos,
                    mXPos,
                    mVelY + mYPos,
                    &pathLine,
                    &hitX,
                    &hitY,
                    mScale == Scale::Fg ? kFgFloor : kBgFloor)
                == 1)
            {
                if (!field_134_bHitDrillOrMineCar)
                {
                    mYPos = hitY;
                }
            }
            else if (!field_134_bHitDrillOrMineCar)
            {
                mYPos += mVelY;
                return;
            }

            field_134_bHitDrillOrMineCar = FALSE;
            field_11C_state = State::eSmashed_4;

            relive_new ScreenShake(0, mSpriteScale == FP_FromDouble(0.5));

            if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz)
            {
                relive_new ParticleBurst(mXPos,
                                                   mYPos,
                                                   20,
                                                   mSpriteScale,
                                                   BurstType::eSticks_1,
                                                   13);

                auto pParticle = relive_new Particle(mXPos,
                                                  mYPos - (FP_FromInteger(15) * mSpriteScale),
                                                  GetAnimRes(AnimId::AirExplosion));
                if (pParticle)
                {
                    pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
                    pParticle->mSpriteScale = mSpriteScale * FP_FromDouble(0.75);
                }
            }
            else
            {
                relive_new ParticleBurst(mXPos,
                                                        mYPos,
                                                        25,
                                                        mSpriteScale,
                                                        BurstType::eFallingRocks_0,
                                                        13);
            }
        }
        break;

        case State::eSmashed_4:
            if (field_140_sound_channels)
            {
                SND_Stop_Channels_Mask(field_140_sound_channels);
                field_140_sound_channels = 0;
            }

            EventBroadcast(kEventLoudNoise, this);
            SfxPlayMono(relive::SoundEffects::FallingItemLand, 0, mSpriteScale);

            if (mScale == Scale::Fg)
            {
                SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 110, -1536);
            }
            else
            {
                SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 55, -1536);
            }

            if (field_11E_switch_id)
            {
                if (field_12C_reset_switch_id_after_use == relive::reliveChoice::eYes)
                {
                    SwitchStates_Do_Operation(field_11E_switch_id, relive::reliveSwitchOp::eSetFalse);
                }
            }

            --field_122_remaining_falling_items;

            if ((field_120_max_falling_items > 0 && field_122_remaining_falling_items <= 0) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, field_138_xpos, field_13C_ypos, 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(sFallingItemData_544DC0[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0]));
                mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                mYPos = field_130_yPosStart;
                field_11C_state = State::eWaitForIdEnable_0;
            }
            break;

        default:
            return;
    }
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
            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6) || pObj->Type() == ReliveTypes::eDrill)
            {
                BaseAnimatedWithPhysicsGameObject* pAliveObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                PSX_RECT fallingItemRect = VGetBoundingRect();
                PSX_RECT objRect = pAliveObj->VGetBoundingRect();

                if (pAliveObj->mSpriteScale == mSpriteScale)
                {
                    if (pAliveObj->Type() == ReliveTypes::eDrill || pAliveObj->Type() == ReliveTypes::eMineCar)
                    {
                        objRect.x += pAliveObj->mXOffset;
                        objRect.y += pAliveObj->mYOffset;

                        objRect.w += pAliveObj->mXOffset;
                        objRect.h += pAliveObj->mYOffset;
                    }

                    if (PSX_Rects_overlap_no_adjustment(&fallingItemRect, &objRect))
                    {
                        if (pAliveObj->Type() == ReliveTypes::eDrill)
                        {
                            // Drill is not a type that implements VTakeDamage
                            field_134_bHitDrillOrMineCar = TRUE;
                        }
                        else if (pAliveObj->Type() == ReliveTypes::eMineCar)
                        {
                            // ?? Could still call VTakeDamage here but OG doesn't ??
                            field_134_bHitDrillOrMineCar = TRUE;
                        }
                        else
                        {
                            bool doDamage = true;
                            if (pAliveObj->Type() == ReliveTypes::eParamite)
                            {
                                // Some strange edge case for paramites - prevents them getting smashed by
                                // falling items when stood on an edge by their huge heads peeking over a bit.
                                if (pAliveObj->mXPos < FP_FromInteger(fallingItemRect.x) || pAliveObj->mXPos > FP_FromInteger(fallingItemRect.w))
                                {
                                    doDamage = false;
                                }
                            }

                            if (doDamage)
                            {
                                static_cast<BaseAliveGameObject*>(pAliveObj)->VTakeDamage(this);
                            }
                        }
                    }
                }
            }
        }
    }
}
