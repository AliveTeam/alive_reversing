#include "stdafx_ao.h"
#include "Function.hpp"
#include "MusicTrigger.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "MusicController.hpp"
#include "Game.hpp"

namespace AO {

MusicTrigger::MusicTrigger(MusicTriggerMusicType type, TriggeredBy triggeredBy, s32 switchId, s32 delay)
    : BaseGameObject(TRUE, 0)
{
    Init(type, triggeredBy, static_cast<s16>(switchId), static_cast<s16>(delay));
    field_10_tlvInfo = -1;
}

MusicTrigger::MusicTrigger(Path_MusicTrigger* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    Init(pTlv->field_18_music_type, pTlv->field_1A_triggered_by, pTlv->field_1C_switch_id, pTlv->field_1E_music_delay);
    field_10_tlvInfo = tlvInfo;
}

void MusicTrigger::Init(MusicTriggerMusicType type, TriggeredBy triggeredBy, u16 switchId, s16 delay)
{
    field_14_flags &= ~7u;
    mBaseGameObjectTypeId = ReliveTypes::eNone;

    switch (type)
    {
        case MusicTriggerMusicType::eDrumAmbience_0:
            field_1C_music_type = MusicController::MusicTypes::eDrumAmbience_3;
            field_18_counter = 400;
            break;

        case MusicTriggerMusicType::eDeathDrumShort_1:
            field_1C_music_type = MusicController::MusicTypes::eDeathDrumShort_13;
            field_18_counter = 30;
            break;

        case MusicTriggerMusicType::eSecretAreaLong_2:
            field_1C_music_type = MusicController::MusicTypes::eSecretAreaLong_16;
            field_18_counter = 30;
            break;

        case MusicTriggerMusicType::eSoftChase_3:
            field_1C_music_type = MusicController::MusicTypes::eSlogChase_5;
            field_14_flags |= 4;
            break;

        case MusicTriggerMusicType::eIntenseChase_4:
            field_1C_music_type = MusicController::MusicTypes::eIntenseChase_8;
            field_14_flags |= 4;
            break;

        case MusicTriggerMusicType::eChime_5:
            field_1C_music_type = MusicController::MusicTypes::eChime_2;
            field_18_counter = delay;
            break;

        case MusicTriggerMusicType::eSecretAreaShort_6:
            field_1C_music_type = MusicController::MusicTypes::eSecretAreaShort_15;
            field_18_counter = 30;
            break;

        default:
            break;
    }

    switch (triggeredBy)
    {
        case TriggeredBy::eTimer_0:
            mBaseGameObjectUpdateDelay = 0;
            break;

        case TriggeredBy::eTouching_1:
            mBaseGameObjectUpdateDelay = delay;
            break;

        case TriggeredBy::eSwitchID_2: // removed in AE
            field_14_flags |= 1u;
            field_1E_switch_id = switchId;
            mBaseGameObjectUpdateDelay = 0;
            field_18_counter = delay;
            if (switchId > 1)
            {
                if (SwitchStates_Get(switchId))
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }
            break;

        case TriggeredBy::eUnknown_3: // removed in AE
            field_14_flags |= 1u;
            field_1E_switch_id = switchId;
            mBaseGameObjectUpdateDelay = 0;
            field_18_counter = -1;
            break;

        default:
            return;
    }
}

MusicTrigger::~MusicTrigger()
{
    if (field_14_flags & 4)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
    }
    MusicController::ClearObject(this);
}

void MusicTrigger::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void MusicTrigger::VUpdate()
{
    if (EventGet(kEventHeroDying))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        if (field_10_tlvInfo >= 0)
        {
            gMap.TLV_Reset(field_10_tlvInfo, -1, 0, 0);
        }
    }

    if (field_14_flags & 1)
    {
        if (SwitchStates_Get(field_1E_switch_id))
        {
            field_14_flags &= ~1u;

            MusicController::static_PlayMusic(
                field_1C_music_type,
                this,
                ((u8) field_14_flags >> 2) & 1,
                1);
            field_14_flags |= 2u;

            if (field_18_counter >= 0)
            {
                field_18_counter += sGnFrame;
            }
        }
    }
    else
    {
        if (!(field_14_flags & 2))
        {
            MusicController::static_PlayMusic(field_1C_music_type, this, (field_14_flags >> 2) & 1, 1);
            field_14_flags |= 2u;
            field_18_counter += sGnFrame;
            return;
        }

        if (field_18_counter < 0)
        {
            if (!SwitchStates_Get(field_1E_switch_id))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            if (field_18_counter < 0)
            {
                MusicController::static_PlayMusic(field_1C_music_type, this, (field_14_flags >> 2) & 1, 0);
                return;
            }
        }

        if (static_cast<s32>(sGnFrame) < field_18_counter)
        {
            MusicController::static_PlayMusic(field_1C_music_type, this, (field_14_flags >> 2) & 1, 0);
        }
        else
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

} // namespace AO
