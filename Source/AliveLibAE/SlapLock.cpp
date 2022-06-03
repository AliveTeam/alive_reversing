#include "stdafx.h"
#include "SlapLock.hpp"
#include "SlapLockWhirlWind.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "AbilityRing.hpp"
#include "PossessionFlicker.hpp"
#include "Particle.hpp"
#include "ParticleBurst.hpp"

SlapLock::SlapLock(Path_SlapLock* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(AETypes::eLockedSoul_61);
    field_118_pTlv = pTlv;
    field_11C_tlvInfo = tlvInfo;
    field_C_objectId = tlvInfo;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }

    const AnimRecord& rec = AnimRec(AnimId::SlapLock_Initiate);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    if (field_118_pTlv->field_10_scale != Scale_short::eFull_0)
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }

    field_120_state = SlapLockStates::eShaking_0;
    field_124_timer1 = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + 25;
    field_134_id = -1;
    field_138_possesion_flicker_id = -1;
    const FP midX = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_128_midX = midX;
    mXPos = midX;

    const FP ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_12C_midY = ypos;
    mYPos = ypos;

    field_130_has_ghost = field_118_pTlv->field_18_has_ghost;

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eSlapLock_OrbWhirlWind_60 && static_cast<SlapLockWhirlWind*>(pObj)->SwitchId() == field_118_pTlv->field_14_target_tomb_id2)
        {
            field_130_has_ghost = Choice_short::eNo_0;
        }
    }

    if (SwitchStates_Get(pTlv->field_14_target_tomb_id2))
    {
        field_130_has_ghost = Choice_short::eNo_0;
    }

    if (pTlv->field_1_tlv_state == 0)
    {
        return;
    }

    mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    field_124_timer1 = sGnFrame_5C1B84 + 60;
    field_13C_timer2 = sGnFrame_5C1B84 + 30;

    if (field_118_pTlv->field_1A_give_invisibility_powerup == Choice_short::eYes_1)
    {
        field_120_state = SlapLockStates::eEmitInvisibilityPowerupRing_4;
    }
    else
    {
        field_120_state = SlapLockStates::eBroken_3;
    }
}

SlapLock::~SlapLock()
{
    Path::TLV_Reset(field_11C_tlvInfo, -1, 0, 0);
}

s32 SlapLock::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const SlapLock_State*>(pBuffer);

    auto pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_4_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kGhostTrpResID_1053, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("GHOSTTRP.BAN", nullptr);
    }

    auto pSlapLock = ae_new<SlapLock>(pTlv, pState->field_4_tlvInfo);
    if (pSlapLock)
    {
        pSlapLock->mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_2_render & 1);

        pSlapLock->field_11C_tlvInfo = pState->field_4_tlvInfo;

        pTlv->field_1_tlv_state = pState->field_8_tlv_state;

        pSlapLock->field_120_state = pState->field_A_state;
        pSlapLock->field_124_timer1 = pState->field_C_timer1;
        pSlapLock->mAliveGameObjectFlags.Set(Flags_114::e114_Bit1_bShot);
        pSlapLock->field_13C_timer2 = pState->field_14_timer2;
    }

    return sizeof(SlapLock_State);
}

void SlapLock::VScreenChanged()
{
    if (field_120_state == SlapLockStates::eFlickerHero_5 || field_120_state == SlapLockStates::eGiveInvisibilityFromFlicker_6)
    {
        GiveInvisibility();
    }
    mGameObjectFlags.Set(BaseGameObject::eDead);
}

void SlapLock::GiveInvisibility()
{
    field_118_pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_11C_tlvInfo));
    if (sActiveHero)
    {
        sActiveHero->field_176_invisibility_duration = field_118_pTlv->field_1C_invisibility_duration;
        sActiveHero->field_16C_bHaveShrykull = 0;
        sActiveHero->field_16E_bHaveInvisiblity = 1;
        sActiveHero->field_168_ring_pulse_timer = sGnFrame_5C1B84 + 200000;
    }
}

s32 SlapLock::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<SlapLock_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eLockedSoul_61;
    pState->field_2_render = mAnim.mAnimFlags.Get(AnimFlags::eBit3_Render) & 1;
    pState->field_4_tlvInfo = field_11C_tlvInfo;
    pState->field_8_tlv_state = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_11C_tlvInfo)->field_1_tlv_state;
    pState->field_A_state = field_120_state;
    pState->field_C_timer1 = field_124_timer1;
    pState->field_14_timer2 = field_13C_timer2;
    pState->field_10_obj_id = -1;

    if (field_134_id == -1)
    {
        return sizeof(SlapLock_State);
    }

    BaseGameObject* pObj = sObjectIds.Find_Impl(field_134_id);
    if (pObj)
    {
        pState->field_10_obj_id = pObj->field_C_objectId;
    }
    return sizeof(SlapLock_State);
}

void SlapLock::VUpdate()
{
    field_118_pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_11C_tlvInfo));

    if (Event_Get(kEventDeathReset))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (mAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
        {
            mAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);

            if (field_118_pTlv->field_1_tlv_state)
            {
                SwitchStates_Do_Operation(field_118_pTlv->field_14_target_tomb_id2, SwitchOp::eSetTrue_0);
            }

            if (field_134_id != -1)
            {
                for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->Type() == AETypes::eAbilityRing_104 && pObj->field_C_objectId == field_134_id)
                    {
                        field_134_id = pObj->field_8_object_id;
                        break;
                    }
                }
            }
        }

        BaseGameObject* pRingObj = sObjectIds.Find_Impl(field_134_id);
        BaseGameObject* pFlickerObj = sObjectIds.Find_Impl(field_138_possesion_flicker_id);

        switch (field_120_state)
        {
            case SlapLockStates::eShaking_0:
            {
                if (field_118_pTlv->field_1A_give_invisibility_powerup == Choice_short::eYes_1)
                {
                    if (!(sGnFrame_5C1B84 & 63))
                    {
                        AbilityRing::Factory_482F80(
                            mXPos,
                            mYPos - (FP_FromInteger(40) * mSpriteScale),
                            RingTypes::eInvisible_Pulse_Large_8,
                            mSpriteScale);
                    }
                }

                if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer1)
                {
                    return;
                }

                if (field_130_has_ghost == Choice_short::eNo_0)
                {
                    return;
                }

                const AnimRecord& animRec = AnimRec(AnimId::SlapLock_Shaking);
                mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

                field_120_state = SlapLockStates::eIdle_1;
                SFX_Play_Mono(SoundEffect::SpiritLockShake_105, 0);
                return;
            }
            case SlapLockStates::eIdle_1:
            {
                if (field_118_pTlv->field_1A_give_invisibility_powerup == Choice_short::eYes_1)
                {
                    if (!(sGnFrame_5C1B84 & 63))
                    {
                        AbilityRing::Factory_482F80(
                            mXPos,
                            mYPos - (FP_FromInteger(40) * mSpriteScale),
                            RingTypes::eInvisible_Pulse_Large_8,
                            mSpriteScale);
                    }
                }

                if (!(mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
                {
                    return;
                }

                const AnimRecord& animRec = AnimRec(AnimId::SlapLock_Initiate);
                mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

                field_120_state = SlapLockStates::eShaking_0;
                field_124_timer1 = Math_NextRandom() + sGnFrame_5C1B84 + 25;
                return;
            }
            case SlapLockStates::eSlapped_2:
            {
                if (!(mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
                {
                    return;
                }

                mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

                if (field_118_pTlv->field_1A_give_invisibility_powerup == Choice_short::eNo_0)
                {
                    field_13C_timer2 = sGnFrame_5C1B84 + 60;
                    field_120_state = SlapLockStates::eBroken_3;
                    break;
                }
                else
                {
                    field_120_state = SlapLockStates::eFlickerHero_5;
                    return;
                }
                break;
            }
            case SlapLockStates::eBroken_3:
            {
                if (static_cast<s32>(sGnFrame_5C1B84) <= field_13C_timer2)
                {
                    return;
                }

                New_TintShiny_Particle(
                    (mSpriteScale * (FP_FromInteger(Math_RandomRange(-2, 2)) + FP_FromInteger(1))) + mXPos,
                    (mSpriteScale * (FP_FromInteger(Math_RandomRange(-3, 3)) - FP_FromInteger(33))) + mYPos,
                    FP_FromDouble(0.3),
                    mAnim.mRenderLayer);

                field_13C_timer2 = Math_RandomRange(-30, 30) + sGnFrame_5C1B84 + 60;
                return;
            }
            case SlapLockStates::eEmitInvisibilityPowerupRing_4:
            {
                if (static_cast<s32>(sGnFrame_5C1B84) > field_124_timer1)
                {
                    if (!gMap.Is_Point_In_Current_Camera_4810D0(
                            sActiveHero->mLvlNumber,
                            sActiveHero->mPathNumber,
                            sActiveHero->mXPos,
                            sActiveHero->mYPos,
                            1)
                        || sActiveHero->field_168_ring_pulse_timer
                        || sActiveHero->mAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
                    {
                        AbilityRing::Factory_482F80(
                            mXPos,
                            mYPos - (FP_FromInteger(40) * mSpriteScale),
                            RingTypes::eInvisible_Pulse_Large_8,
                            mSpriteScale);
                        field_124_timer1 = Math_RandomRange(1, 10) + sGnFrame_5C1B84 + 55;
                    }
                    else
                    {
                        SetInvisibilityTarget();
                        field_120_state = SlapLockStates::eFlickerHero_5;
                    }
                }

                if (static_cast<s32>(sGnFrame_5C1B84) <= field_13C_timer2)
                {
                    return;
                }

                New_TintShiny_Particle(
                    (mSpriteScale * (FP_FromInteger(Math_RandomRange(-2, 2)) + FP_FromInteger(1))) + mXPos,
                    (mSpriteScale * (FP_FromInteger(Math_RandomRange(-3, 3)) - FP_FromInteger(33))) + mYPos,
                    FP_FromDouble(0.3),
                    mAnim.mRenderLayer);

                field_13C_timer2 = Math_RandomRange(-30, 30) + sGnFrame_5C1B84 + 60;
                return;
            }
            case SlapLockStates::eFlickerHero_5:
            {
                if (pRingObj)
                {
                    return;
                }

                if (sActiveHero->mAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
                {
                    field_120_state = SlapLockStates::eGiveInvisibility_7;
                }
                else
                {
                    auto pFlicker = ae_new<PossessionFlicker>(sActiveHero, 8, 128, 255, 128);
                    if (pFlicker)
                    {
                        field_138_possesion_flicker_id = pFlicker->field_8_object_id;
                    }
                    field_120_state = SlapLockStates::eGiveInvisibilityFromFlicker_6;
                }
                return;
            }
            case SlapLockStates::eGiveInvisibilityFromFlicker_6:
            {
                if (pFlickerObj)
                {
                    return;
                }
                GiveInvisibility();
                field_13C_timer2 = sGnFrame_5C1B84 + 60;
                field_120_state = SlapLockStates::eBroken_3;
                break;
            }
            case SlapLockStates::eGiveInvisibility_7:
            {
                GiveInvisibility();
                field_13C_timer2 = sGnFrame_5C1B84 + 60;
                field_120_state = SlapLockStates::eBroken_3;
                break;
            }
            default:
            {
                return;
            }
        }
    }
}

void SlapLock::SetInvisibilityTarget()
{
    AbilityRing::Factory_482F80(
        mXPos,
        mYPos - (FP_FromInteger(40) * mSpriteScale),
        RingTypes::eInvisible_Pulse_Emit_9,
        mSpriteScale);

    PSX_RECT bRect = {};
    sActiveHero->VGetBoundingRect(&bRect, 1);

    AbilityRing* pRing = AbilityRing::Factory_482F80(
        FP_FromInteger((bRect.x + bRect.w) / 2),
        FP_FromInteger((bRect.y + bRect.h) / 2),
        RingTypes::eInvisible_Pulse_Give_10,
        sActiveHero->mSpriteScale);

    pRing->field_C_objectId = field_C_objectId;
    field_134_id = pRing->field_8_object_id;

    pRing->VSetTarget(sActiveHero);
}

s16 SlapLock::VTakeDamage(BaseGameObject* pFrom)
{
    field_118_pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_11C_tlvInfo));

    if (pFrom->Type() != AETypes::eAbe_69)
    {
        // Only Abe can slap me up
        return 0;
    }

    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_62_Punch_454750)
    {
        // If Abe isn't slapping then he can't hurt me
        return 0;
    }

    if (field_120_state != SlapLockStates::eShaking_0 && field_120_state != SlapLockStates::eIdle_1)
    {
        return 0;
    }

    if (mGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    sActiveHero->ToKnockback_44E700(1, 0);

    if (field_130_has_ghost == Choice_short::eYes_1)
    {
        field_130_has_ghost = Choice_short::eNo_0;
        ae_new<SlapLockWhirlWind>(
            field_118_pTlv->field_12_target_tomb_id1,
            field_118_pTlv->field_14_target_tomb_id2,
            mXPos,
            mYPos - (FP_FromInteger(40) * mSpriteScale),
            mSpriteScale);
    }

    if (field_118_pTlv->field_1A_give_invisibility_powerup == Choice_short::eYes_1)
    {
        SetInvisibilityTarget();
    }

    field_120_state = SlapLockStates::eSlapped_2;
    SwitchStates_Do_Operation(field_118_pTlv->field_1E_toggle_switch_id, SwitchOp::eToggle_2);
    SFX_Play_Mono(SoundEffect::SpiritLockBreak_106, 0, mSpriteScale);
    Event_Broadcast(kEventLoudNoise, this);

    ae_new<ParticleBurst>(
        mXPos,
        mYPos - (FP_FromInteger(40) * mSpriteScale),
        15,
        mSpriteScale,
        BurstType::eGreenSparks_5,
        11);

    const AnimRecord& animRec = AnimRec(AnimId::SlapLock_Punched);
    mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

    field_118_pTlv->field_1_tlv_state = 1;
    return 1;
}
