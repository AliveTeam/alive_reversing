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
    SetType(AETypes::eRockSpawner_48);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    field_118_tlvInfo = tlvInfo;

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);

    const s32 lvlIdx = static_cast<s32>(gMap.mCurrentLevel);

    const AnimRecord& rec = AnimRec(sFallingItemData_544DC0[lvlIdx][0]);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_11E_switch_id = pTlv->field_10_switch_id;

    if (pTlv->field_12_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.mRenderLayer = Layer::eLayer_FallingItemPortalClip_Half_12;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.mRenderLayer = Layer::eLayer_FallingItemPortalClip_31;
    }

    field_124_fall_interval = pTlv->field_14_fall_interval;
    field_120_max_falling_items = pTlv->field_16_max_falling_items;
    field_122_remaining_falling_items = pTlv->field_16_max_falling_items;
    field_134_bHitDrillOrMineCar = FALSE;
    field_12C_reset_switch_id_after_use = pTlv->field_18_reset_switch_id_after_use;
    field_12E_do_sound_in_state_falling = TRUE;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (mBaseAnimatedWithPhysicsGameObject_YPos > pScreenManager_5BB5F4->field_20_pCamPos->field_4_y)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
    }

    field_138_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_13C_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_130_yPosStart = mBaseAnimatedWithPhysicsGameObject_YPos;
    field_11C_state = State::eWaitForIdEnable_0;
    field_140_sound_channels = 0;

    if (!pPrimaryFallingItem_5BC208)
    {
        pPrimaryFallingItem_5BC208 = this;
        field_144_created_gnFrame = sGnFrame_5C1B84;
    }

    mShadow = ae_new<Shadow>();
}

 FallingItem::FallingItem(s32 xpos, s32 ypos, s32 scale, s32 id, s32 fallInterval, s32 numItems, s32 bResetIdAfterUse)
    : BaseAliveGameObject(0)
{
    SetType(AETypes::eRockSpawner_48);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    field_118_tlvInfo = -1;

    const s32 lvlIdx = static_cast<s32>(gMap.mCurrentLevel);

    const AnimRecord& rec = AnimRec(sFallingItemData_544DC0[lvlIdx][0]);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.mRenderLayer = Layer::eLayer_FallingItemPortalClip_31;

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
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
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
        field_144_created_gnFrame = sGnFrame_5C1B84;
    }

    mShadow = ae_new<Shadow>();
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
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    // The primary item controls the main sound effects, otherwise there would be a crazy amount of smashing sounds
    if (pPrimaryFallingItem_5BC208 == this)
    {
        if (!((sGnFrame_5C1B84 - field_144_created_gnFrame) % 87))
        {
            if (field_D6_scale == 1)
            {
                SFX_Play_Mono(SoundEffect::FallingItemPresence1_74, 45);
            }
            else
            {
                SFX_Play_Mono(SoundEffect::FallingItemPresence1_74, 20);
            }
        }

        if (!((sGnFrame_5C1B84 - field_144_created_gnFrame) % 25))
        {
            if (field_D6_scale == 1)
            {
                SFX_Play_Mono(SoundEffect::FallingItemPresence2_75, 45);
            }
            else
            {
                SFX_Play_Mono(SoundEffect::FallingItemPresence2_75, 20);
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
                field_C4_velx = FP_FromInteger(0);
                field_C8_vely = FP_FromInteger(0);

                const AnimRecord& animRec = AnimRec(sFallingItemData_544DC0[static_cast<s32>(gMap.mCurrentLevel)][1]);
                field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

                field_128_fall_interval_timer = sGnFrame_5C1B84 + field_124_fall_interval;
            }
            break;

        // TODO: Must only be set outside of the object
        case State::eGoWaitForDelay_1:
        {
            mBaseGameObjectFlags.Clear(BaseGameObject::eCanExplode_Bit7);
            field_11C_state = State::eWaitForFallDelay_2;
            field_C4_velx = FP_FromInteger(0);
            field_C8_vely = FP_FromInteger(0);

            const AnimRecord& animRec = AnimRec(sFallingItemData_544DC0[static_cast<s32>(gMap.mCurrentLevel)][1]);
            field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

            field_128_fall_interval_timer = sGnFrame_5C1B84 + field_124_fall_interval;
            break;
        }

        case State::eWaitForFallDelay_2:
            if (static_cast<s32>(sGnFrame_5C1B84) >= field_128_fall_interval_timer)
            {
                field_11C_state = State::eFalling_3;
                field_12E_do_sound_in_state_falling = TRUE;
                if (field_D6_scale == 1)
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
                if (mBaseAnimatedWithPhysicsGameObject_YPos >= sActiveHero_5C1B68->mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(240 / 2))
                {
                    field_12E_do_sound_in_state_falling = FALSE;
                    if (field_D6_scale == 1)
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

            if (field_C8_vely < FP_FromInteger(20))
            {
                field_C8_vely += field_CC_sprite_scale * FP_FromDouble(1.8);
            }

            PathLine* pathLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_5C1128->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    field_C8_vely + mBaseAnimatedWithPhysicsGameObject_YPos,
                    &pathLine,
                    &hitX,
                    &hitY,
                    field_D6_scale != 0 ? 1 : 16)
                == 1)
            {
                if (!field_134_bHitDrillOrMineCar)
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                }
            }
            else if (!field_134_bHitDrillOrMineCar)
            {
                mBaseAnimatedWithPhysicsGameObject_YPos += field_C8_vely;
                return;
            }

            field_134_bHitDrillOrMineCar = FALSE;
            field_11C_state = State::eSmashed_4;

            ae_new<ScreenShake>(0, field_CC_sprite_scale == FP_FromDouble(0.5));

            if (gMap.mCurrentLevel == LevelIds::eBonewerkz_8)
            {
                ae_new<ParticleBurst>(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                   mBaseAnimatedWithPhysicsGameObject_YPos,
                                                   20,
                                                   field_CC_sprite_scale,
                                                   BurstType::eSticks_1,
                                                   13);

                const AnimRecord& rec = AnimRec(AnimId::Explosion);
                u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, rec.mResourceId, 0, 0);
                auto pParticle = ae_new<Particle>(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                  mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(15) * field_CC_sprite_scale),
                                                  rec.mFrameTableOffset,
                                                  rec.mMaxW,
                                                  rec.mMaxH,
                                                  ppRes);
                if (pParticle)
                {
                    pParticle->field_20_animation.mRenderMode = TPageAbr::eBlend_1;
                    pParticle->field_CC_sprite_scale = field_CC_sprite_scale * FP_FromDouble(0.75);
                }
            }
            else
            {
                ae_new<ParticleBurst>(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                        mBaseAnimatedWithPhysicsGameObject_YPos,
                                                        25,
                                                        field_CC_sprite_scale,
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

            Event_Broadcast(kEventLoudNoise, this);
            SFX_Play_Mono(SoundEffect::FallingItemLand_62, 0, field_CC_sprite_scale);

            if (field_D6_scale == 1)
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

            if ((field_120_max_falling_items > 0 && field_122_remaining_falling_items <= 0) || !gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_138_xpos, field_13C_ypos, 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                const AnimRecord& animRec = AnimRec(sFallingItemData_544DC0[static_cast<s32>(gMap.mCurrentLevel)][0]);
                field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
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
            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6) || pObj->Type() == AETypes::eDrill_30)
            {
                BaseAnimatedWithPhysicsGameObject* pAliveObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                PSX_RECT fallingItemRect = {};
                VGetBoundingRect(&fallingItemRect, 1);

                PSX_RECT objRect = {};
                pAliveObj->VGetBoundingRect(&objRect, 1);

                if (pAliveObj->field_CC_sprite_scale == field_CC_sprite_scale)
                {
                    if (pAliveObj->Type() == AETypes::eDrill_30 || pAliveObj->Type() == AETypes::eMineCar_89)
                    {
                        objRect.x += pAliveObj->field_DA_xOffset;
                        objRect.y += pAliveObj->field_D8_yOffset;

                        objRect.w += pAliveObj->field_DA_xOffset;
                        objRect.h += pAliveObj->field_D8_yOffset;
                    }

                    if (PSX_Rects_overlap_no_adjustment(&fallingItemRect, &objRect))
                    {
                        if (pAliveObj->Type() == AETypes::eDrill_30)
                        {
                            // Drill is not a type that implements VTakeDamage
                            field_134_bHitDrillOrMineCar = TRUE;
                        }
                        else if (pAliveObj->Type() == AETypes::eMineCar_89)
                        {
                            // ?? Could still call VTakeDamage here but OG doesn't ??
                            field_134_bHitDrillOrMineCar = TRUE;
                        }
                        else
                        {
                            bool doDamage = true;
                            if (pAliveObj->Type() == AETypes::eParamite_96)
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
