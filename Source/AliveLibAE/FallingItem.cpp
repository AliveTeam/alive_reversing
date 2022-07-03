#include "stdafx.h"
#include "FallingItem.hpp"
#include "Shadow.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "ParticleBurst.hpp"
#include "Particle.hpp"
#include "ScreenShake.hpp"
#include "Abe.hpp"

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

ALIVE_VAR(1, 0x5BC208, FallingItem*, pPrimaryFallingItem_5BC208, nullptr);

FallingItem::FallingItem(Path_FallingItem* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eRockSpawner);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    field_118_tlvInfo = tlvInfo;

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);

    const s32 lvlIdx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    const AnimRecord& rec = AnimRec(sFallingItemData_544DC0[lvlIdx][0]);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_11E_switch_id = pTlv->field_10_switch_id;

    if (pTlv->field_12_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_FallingItemPortalClip_Half_12;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_FallingItemPortalClip_31;
    }

    field_124_fall_interval = pTlv->field_14_fall_interval;
    field_120_max_falling_items = pTlv->field_16_max_falling_items;
    field_122_remaining_falling_items = pTlv->field_16_max_falling_items;
    field_134_bHitDrillOrMineCar = FALSE;
    field_12C_reset_switch_id_after_use = pTlv->field_18_reset_switch_id_after_use;
    field_12E_do_sound_in_state_falling = TRUE;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    if (mBaseAnimatedWithPhysicsGameObject_YPos > pScreenManager->CamYPos())
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = pScreenManager->CamYPos();
    }

    field_138_xpos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    field_13C_ypos = FP_FromInteger(pTlv->mBottomRight.y);
    field_130_yPosStart = mBaseAnimatedWithPhysicsGameObject_YPos;
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
    field_118_tlvInfo = -1;

    const s32 lvlIdx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    const AnimRecord& rec = AnimRec(sFallingItemData_544DC0[lvlIdx][0]);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_FallingItemPortalClip_31;

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
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    field_124_fall_interval = static_cast<s16>(fallInterval);

    field_120_max_falling_items = static_cast<s16>(numItems);
    field_122_remaining_falling_items = static_cast<s16>(numItems);

    const FP xFixed = FP_FromInteger(xpos);
    const FP yFixed = FP_FromInteger(ypos);

    field_12C_reset_switch_id_after_use = static_cast<Choice_short>(bResetIdAfterUse);
    field_134_bHitDrillOrMineCar = FALSE;
    field_12E_do_sound_in_state_falling = TRUE;
    mBaseAnimatedWithPhysicsGameObject_XPos = xFixed;
    mBaseAnimatedWithPhysicsGameObject_YPos = yFixed;
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
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || field_11C_state != State::eFalling_3)
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
            if (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg)
            {
                SfxPlayMono(SoundEffect::FallingItemPresence1_74, 45);
            }
            else
            {
                SfxPlayMono(SoundEffect::FallingItemPresence1_74, 20);
            }
        }

        if (!((sGnFrame - field_144_created_gnFrame) % 25))
        {
            if (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg)
            {
                SfxPlayMono(SoundEffect::FallingItemPresence2_75, 45);
            }
            else
            {
                SfxPlayMono(SoundEffect::FallingItemPresence2_75, 20);
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
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sFallingItemData_544DC0[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1], nullptr);

                field_128_fall_interval_timer = sGnFrame + field_124_fall_interval;
            }
            break;

        // TODO: Must only be set outside of the object
        case State::eGoWaitForDelay_1:
        {
            mBaseGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);
            field_11C_state = State::eWaitForFallDelay_2;
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sFallingItemData_544DC0[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1], nullptr);

            field_128_fall_interval_timer = sGnFrame + field_124_fall_interval;
            break;
        }

        case State::eWaitForFallDelay_2:
            if (static_cast<s32>(sGnFrame) >= field_128_fall_interval_timer)
            {
                field_11C_state = State::eFalling_3;
                field_12E_do_sound_in_state_falling = TRUE;
                if (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg)
                {
                    field_140_sound_channels = SFX_Play_Pitch(SoundEffect::AirStream_23, 50, -2600);
                }
                else
                {
                    field_140_sound_channels = SFX_Play_Pitch(SoundEffect::AirStream_23, 25, -2600);
                }
            }
            break;

        case State::eFalling_3:
        {
            if (field_12E_do_sound_in_state_falling)
            {
                if (mBaseAnimatedWithPhysicsGameObject_YPos >= sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(240 / 2))
                {
                    field_12E_do_sound_in_state_falling = FALSE;
                    if (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg)
                    {
                        SFX_Play_Pitch(SoundEffect::AirStream_23, 127, -1300);
                    }
                    else
                    {
                        SFX_Play_Pitch(SoundEffect::AirStream_23, 64, -1300);
                    }
                }
            }

            DamageHitItems();

            if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(20))
            {
                mBaseAnimatedWithPhysicsGameObject_VelY += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.8);
            }

            PathLine* pathLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_VelY + mBaseAnimatedWithPhysicsGameObject_YPos,
                    &pathLine,
                    &hitX,
                    &hitY,
                    mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
                == 1)
            {
                if (!field_134_bHitDrillOrMineCar)
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                }
            }
            else if (!field_134_bHitDrillOrMineCar)
            {
                mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
                return;
            }

            field_134_bHitDrillOrMineCar = FALSE;
            field_11C_state = State::eSmashed_4;

            relive_new ScreenShake(0, mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5));

            if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz)
            {
                relive_new ParticleBurst(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                   mBaseAnimatedWithPhysicsGameObject_YPos,
                                                   20,
                                                   mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                                                   BurstType::eSticks_1,
                                                   13);

                const AnimRecord& rec = AnimRec(AnimId::Explosion);
                u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 0, 0);
                auto pParticle = relive_new Particle(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                  mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(15) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                                                  rec.mFrameTableOffset,
                                                  rec.mMaxW,
                                                  rec.mMaxH,
                                                  ppRes);
                if (pParticle)
                {
                    pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
                    pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.75);
                }
            }
            else
            {
                relive_new ParticleBurst(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                        mBaseAnimatedWithPhysicsGameObject_YPos,
                                                        25,
                                                        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
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
            SfxPlayMono(SoundEffect::FallingItemLand_62, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            if (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg)
            {
                SFX_Play_Pitch(SoundEffect::FallingItemHit_47, 110, -1536);
            }
            else
            {
                SFX_Play_Pitch(SoundEffect::FallingItemHit_47, 55, -1536);
            }

            if (field_11E_switch_id)
            {
                if (field_12C_reset_switch_id_after_use == Choice_short::eYes_1)
                {
                    SwitchStates_Do_Operation(field_11E_switch_id, SwitchOp::eSetFalse_1);
                }
            }

            --field_122_remaining_falling_items;

            if ((field_120_max_falling_items > 0 && field_122_remaining_falling_items <= 0) || !gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, field_138_xpos, field_13C_ypos, 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sFallingItemData_544DC0[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0], nullptr);
                mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_YPos = field_130_yPosStart;
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

                if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                {
                    if (pAliveObj->Type() == ReliveTypes::eDrill || pAliveObj->Type() == ReliveTypes::eMineCar)
                    {
                        objRect.x += pAliveObj->mBaseAnimatedWithPhysicsGameObject_XOffset;
                        objRect.y += pAliveObj->mBaseAnimatedWithPhysicsGameObject_YOffset;

                        objRect.w += pAliveObj->mBaseAnimatedWithPhysicsGameObject_XOffset;
                        objRect.h += pAliveObj->mBaseAnimatedWithPhysicsGameObject_YOffset;
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
                                if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(fallingItemRect.x) || pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(fallingItemRect.w))
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
