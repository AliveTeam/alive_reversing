#include "stdafx.h"
#include "MusicTrigger.hpp"
#include "Function.hpp"
#include "MusicController.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Game.hpp"

MusicTrigger::MusicTrigger(relive::Path_MusicTrigger* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    Init(pTlv->mMusicType, pTlv->mTriggeredBy, pTlv->mMusicDelay);
    field_2C_tl.x = pTlv->mTopLeftX;
    field_2C_tl.y = pTlv->mTopLeftY;
    field_30_br.x = pTlv->mBottomRightX;
    field_30_br.y = pTlv->mBottomRightY;
    field_20_tlvInfo = tlvId;
}

MusicTrigger::MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType musicType, relive::Path_MusicTrigger::TriggeredBy triggeredBy, s32 /*not_used*/, s32 musicDelay)
    : BaseGameObject(TRUE, 0)
{
    Init(musicType, triggeredBy, static_cast<s16>(musicDelay));
    field_2C_tl = {};
    field_30_br = {};
    field_20_tlvInfo = Guid{};
}

void MusicTrigger::Init(relive::Path_MusicTrigger::MusicTriggerMusicType musicType, relive::Path_MusicTrigger::TriggeredBy triggeredBy, s16 musicDelay)
{
    field_24_flags.Clear(Flags_24::e24_Bit1_TriggeredByTouching);
    field_24_flags.Clear(Flags_24::e24_Bit2_TriggeredByTimer);
    field_24_flags.Clear(Flags_24::e24_Bit3_SetMusicToNoneOnDtor);

    SetType(ReliveTypes::eMusicTrigger);
    field_28_counter = 0;

    switch (musicType)
    {
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience:
            field_26_music_type = MusicController::MusicTypes::eDrumAmbience_3;
            field_28_counter = 400;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort:
            field_26_music_type = MusicController::MusicTypes::eDeathDrumShort_10;
            field_28_counter = 30;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong:
            field_26_music_type = MusicController::MusicTypes::eSecretAreaLong_13;
            field_28_counter = 30;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase:
            field_26_music_type = MusicController::MusicTypes::eSoftChase_8;
            field_24_flags.Set(Flags_24::e24_Bit3_SetMusicToNoneOnDtor);
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase:
            field_26_music_type = MusicController::MusicTypes::eIntenseChase_7;
            field_24_flags.Set(Flags_24::e24_Bit3_SetMusicToNoneOnDtor);
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eChime:
            field_26_music_type = MusicController::MusicTypes::eChime_2;
            field_28_counter = musicDelay;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort:
            field_26_music_type = MusicController::MusicTypes::eSecretAreaShort_12;
            field_28_counter = 30;
            break;
        default:
            break;
    }

    if (triggeredBy == relive::Path_MusicTrigger::TriggeredBy::eTimer)
    {
        SetUpdateDelay(musicDelay); // OG bug? mBaseGameObjectUpdateDelay should've been field_28_counter?
    }
    else if (triggeredBy == relive::Path_MusicTrigger::TriggeredBy::eTouching)
    {
        field_24_flags.Set(Flags_24::e24_Bit1_TriggeredByTouching);
    }
}

MusicTrigger::~MusicTrigger()
{
    if (field_24_flags.Get(Flags_24::e24_Bit3_SetMusicToNoneOnDtor))
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }
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
    }

    if (field_24_flags.Get(Flags_24::e24_Bit1_TriggeredByTouching))
    {
        FP xpos = sControlledCharacter->mXPos;
        FP ypos = sControlledCharacter->mYPos;

        if (xpos >= FP_FromInteger(field_2C_tl.x) && xpos <= FP_FromInteger(field_30_br.x) && (ypos >= FP_FromInteger(field_2C_tl.y) && ypos <= FP_FromInteger(field_30_br.y)))
        {
            field_24_flags.Clear(Flags_24::e24_Bit1_TriggeredByTouching);
            MusicController::static_PlayMusic(field_26_music_type, this, field_24_flags.Get(Flags_24::e24_Bit3_SetMusicToNoneOnDtor), 1);
            field_24_flags.Set(Flags_24::e24_Bit2_TriggeredByTimer);
            if (field_28_counter >= 0)
            {
                field_28_counter += sGnFrame;
            }
        }
    }
    else if (field_24_flags.Get(Flags_24::e24_Bit2_TriggeredByTimer))
    {
        if (field_28_counter < 0 || static_cast<s32>(sGnFrame) < field_28_counter)
        {
            MusicController::static_PlayMusic(field_26_music_type, this, field_24_flags.Get(Flags_24::e24_Bit3_SetMusicToNoneOnDtor), 0);
        }
        else
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        MusicController::static_PlayMusic(field_26_music_type, this, field_24_flags.Get(Flags_24::e24_Bit3_SetMusicToNoneOnDtor), 1);
        field_24_flags.Set(Flags_24::e24_Bit2_TriggeredByTimer);
        field_28_counter += sGnFrame;
    }
}
