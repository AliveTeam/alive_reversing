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
#include "ResourceManager.hpp"
#include "Map.hpp"

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

void Lever::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Release_Left));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Release_Right));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Left));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Right));
}

Lever::Lever(relive::Path_Lever* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eLever);
    const AnimRecord& rec = AnimRec(AnimId::Lever_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(GetAnimRes(AnimId::Lever_Idle));

    LoadAnimations();

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    field_F4_switch_id = pTlv->mSwitchId;
    field_102_action = pTlv->mAction;
    field_100_flags.Clear(Flags_100::eBit1_lever_anim_left_direction);

    if (pTlv->mPersistOffscreen == relive::reliveChoice::eYes)
    {
        field_100_flags.Set(Flags_100::eBit2_persist_offscreen);
    }
    else
    {
        field_100_flags.Clear(Flags_100::eBit2_persist_offscreen);
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        mScale = Scale::Bg;
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mScale = Scale::Fg;
    }

    SetTint(&kLeverTints_563228[0], gMap.mCurrentLevel);
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)));
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

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
    field_FC_tlvInfo = tlvId;
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
            SfxPlayMono(relive::SoundEffects::LeverPull, 0);
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
                SFX_Play_Pitch(relive::SoundEffects::IndustrialTrigger, 30, 400);
            }

            field_F8_state = LeverState::eFinished_2;

            if (field_100_flags.Get(Flags_100::eBit1_lever_anim_left_direction))
            {
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::Lever_Pull_Release_Left));
            }
            else
            {
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::Lever_Pull_Release_Right));
            }

            const s32 switch_state = SwitchStates_Get(field_F4_switch_id);
            SwitchStates_Do_Operation(field_F4_switch_id, field_102_action);
            const s32 switch_state_after_op = SwitchStates_Get(field_F4_switch_id);
            if (switch_state != switch_state_after_op)
            {
                s32 volLeft = 0;
                s32 volRight = 0;

                if (field_108_sound_direction == relive::Path_Lever::LeverSoundDirection::eLeft)
                {
                    volLeft = 1;
                    volRight = 0;
                }
                else if (field_108_sound_direction == relive::Path_Lever::LeverSoundDirection::eRight)
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
                        case relive::Path_Lever::LeverSoundType::eWell:
                            SFX_Play_Stereo(relive::SoundEffects::WellExit, 80 * volLeft + 25, 80 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eSwitchBellHammer:
                            SFX_Play_Stereo(relive::SoundEffects::SwitchUnknownTrigger, 100 * volLeft + 25, 100 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eDoor:
                            SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * volLeft + 15, 75 * volRight + 15);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eElectricWall:
                            SFX_Play_Stereo(relive::SoundEffects::Zap1, 35 * volLeft + 25, 35 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eSecurityOrb:
                            SFX_Play_Stereo(relive::SoundEffects::SecurityOrb, 35 * volLeft + 25, 35 * volRight + 25);
                            return;

                        case relive::Path_Lever::LeverSoundType::eLift:
                            SFX_Play_Stereo(relive::SoundEffects::LiftStop, 35 * volLeft + 25, 35 * volRight + 25);
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
                        case relive::Path_Lever::LeverSoundType::eWell:
                            SFX_Play_Stereo(relive::SoundEffects::WellExit, 80 * volLeft + 25, 80 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eSwitchBellHammer:
                            SFX_Play_Stereo(relive::SoundEffects::SwitchUnknownTrigger, 110 * volLeft + 25, 110 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eDoor:
                            SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * volLeft + 15, 75 * volRight + 15);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eElectricWall:
                            SFX_Play_Stereo(relive::SoundEffects::Zap1, 80 * volLeft + 25, 80 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eSecurityOrb:
                            SFX_Play_Stereo(relive::SoundEffects::SecurityOrb, 35 * volLeft + 75, 35 * volRight + 75);
                            break;

                        case relive::Path_Lever::LeverSoundType::eLift:
                            SFX_Play_Stereo(relive::SoundEffects::LiftStop, 35 * volLeft + 25, 35 * volRight + 25);
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
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::Lever_Idle));
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
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::Lever_Pull_Left));
        field_100_flags.Set(Flags_100::eBit1_lever_anim_left_direction);
    }
    else
    {
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::Lever_Pull_Right));
        field_100_flags.Clear(Flags_100::eBit1_lever_anim_left_direction);
    }

    return 1;
}
