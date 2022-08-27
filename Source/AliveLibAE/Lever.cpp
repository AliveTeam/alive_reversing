#include "stdafx.h"
#include "Lever.hpp"
#include "Function.hpp"
#include "../relive_lib/Collisions.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "PathData.hpp"
#include "SwitchStates.hpp"
#include "Grid.hpp"

const TintEntry kLeverTints_563228[18] = {
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

Lever::Lever(relive::Path_Lever* pTlv, u32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eLever);
    const AnimRecord& rec = AnimRec(AnimId::Lever_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Lever_Idle, ppRes);

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    field_F4_switch_id = pTlv->mSwitchId;
    field_102_action = pTlv->mAction;
    field_100_flags.Clear(Flags_100::eBit1_lever_anim_left_direction);

    if (pTlv->mPersistOffscreen == Choice_short::eYes_1)
    {
        field_100_flags.Set(Flags_100::eBit2_persist_offscreen);
    }
    else
    {
        field_100_flags.Clear(Flags_100::eBit2_persist_offscreen);
    }

    if (pTlv->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        mScale = Scale::Bg;
    }
    else if (pTlv->mScale == Scale_short::eFull_0)
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mScale = Scale::Fg;
    }

    SetTint(&kLeverTints_563228[0], gMap.mCurrentLevel);
    mXPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)));
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &pPathLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? kFgFloor : kBgFloor))
    {
        mYPos = hitY;
    }

    field_104_on_sound = pTlv->mOnSound;
    field_106_off_sound = pTlv->mOffSound;
    field_FC_tlvInfo = tlvInfo;
    field_108_sound_direction = pTlv->mSoundDirection;

    field_F8_state = LeverState::eWaiting_0;
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
}

Lever::~Lever()
{
    Path::TLV_Reset(field_FC_tlvInfo, -1, 0, 0);
}

void Lever::VScreenChanged()
{
    if (!field_100_flags.Get(Flags_100::eBit2_persist_offscreen) || gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Lever::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_F8_state == LeverState::ePulled_1)
    {
        if (mAnim.mCurrentFrame == 3)
        {
            SfxPlayMono(SoundEffect::LeverPull_63, 0);
        }

        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eMines
                || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz
                || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender
                || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot
                || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender
                || gMap.mCurrentLevel == EReliveLevelIds::eBarracks
                || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender
                || gMap.mCurrentLevel == EReliveLevelIds::eBrewery
                || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                SFX_Play_Pitch(SoundEffect::IndustrialTrigger_80, 30, 400);
            }

            field_F8_state = LeverState::eFinished_2;

            if (field_100_flags.Get(Flags_100::eBit1_lever_anim_left_direction))
            {
                mAnim.Set_Animation_Data(AnimId::Lever_Pull_Release_Left, nullptr);
            }
            else
            {
                mAnim.Set_Animation_Data(AnimId::Lever_Pull_Release_Right, nullptr);
            }

            const s32 switch_state = SwitchStates_Get(field_F4_switch_id);
            SwitchStates_Do_Operation(field_F4_switch_id, field_102_action);
            const s32 switch_state_after_op = SwitchStates_Get(field_F4_switch_id);
            if (switch_state != switch_state_after_op)
            {
                s32 volLeft = 0;
                s32 volRight = 0;

                if (field_108_sound_direction == LeverSoundDirection::eLeft_1)
                {
                    volLeft = 1;
                    volRight = 0;
                }
                else if (field_108_sound_direction == LeverSoundDirection::eRight_2)
                {
                    volLeft = 0;
                    volRight = 1;
                }
                else
                {
                    volLeft = 1;
                    volRight = 1;
                }

                if (switch_state_after_op)
                {
                    switch (field_104_on_sound)
                    {
                        case LeverSoundType::eWell_1:
                            SFX_Play_Stereo(SoundEffect::WellExit_20, 80 * volLeft + 25, 80 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eSwitchBellHammer_2:
                            SFX_Play_Stereo(SoundEffect::SwitchUnknownTrigger_11, 100 * volLeft + 25, 100 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eDoor_3:
                            SFX_Play_Stereo(SoundEffect::DoorEffect_57, 75 * volLeft + 15, 75 * volRight + 15);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eElectricWall_4:
                            SFX_Play_Stereo(SoundEffect::Zap1_49, 35 * volLeft + 25, 35 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eSecurityOrb_5:
                            SFX_Play_Stereo(SoundEffect::SecurityOrb_48, 35 * volLeft + 25, 35 * volRight + 25);
                            return;

                        case LeverSoundType::eLift_6:
                            SFX_Play_Stereo(SoundEffect::LiftStop_30, 35 * volLeft + 25, 35 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        default:
                            return;
                    }
                }
                else
                {
                    switch (field_106_off_sound)
                    {
                        case LeverSoundType::eWell_1:
                            SFX_Play_Stereo(SoundEffect::WellExit_20, 80 * volLeft + 25, 80 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eSwitchBellHammer_2:
                            SFX_Play_Stereo(SoundEffect::SwitchUnknownTrigger_11, 110 * volLeft + 25, 110 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eDoor_3:
                            SFX_Play_Stereo(SoundEffect::DoorEffect_57, 75 * volLeft + 15, 75 * volRight + 15);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eElectricWall_4:
                            SFX_Play_Stereo(SoundEffect::Zap1_49, 80 * volLeft + 25, 80 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case LeverSoundType::eSecurityOrb_5:
                            SFX_Play_Stereo(SoundEffect::SecurityOrb_48, 35 * volLeft + 75, 35 * volRight + 75);
                            break;

                        case LeverSoundType::eLift_6:
                            SFX_Play_Stereo(SoundEffect::LiftStop_30, 35 * volLeft + 25, 35 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        default:
                            return;
                    }
                }
            }
        }
    }
    else if (field_F8_state == LeverState::eFinished_2)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_F8_state = LeverState::eWaiting_0;
            mAnim.Set_Animation_Data(AnimId::Lever_Idle, nullptr);
        }
    }
}

s16 Lever::VPull(s16 bLeftDirection)
{
    if (field_F8_state != LeverState::eWaiting_0)
    {
        return 0;
    }

    field_F8_state = LeverState::ePulled_1;

    if (bLeftDirection)
    {
        mAnim.Set_Animation_Data(AnimId::Lever_Pull_Left, nullptr);
        field_100_flags.Set(Flags_100::eBit1_lever_anim_left_direction);
    }
    else
    {
        mAnim.Set_Animation_Data(AnimId::Lever_Pull_Right, nullptr);
        field_100_flags.Clear(Flags_100::eBit1_lever_anim_left_direction);
    }

    return 1;
}
