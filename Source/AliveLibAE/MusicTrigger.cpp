#include "stdafx.h"
#include "MusicTrigger.hpp"
#include "MusicController.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "Game.hpp"

MusicTrigger::MusicTrigger(relive::Path_MusicTrigger* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    Init(pTlv->mMusicType, pTlv->mTriggeredBy, pTlv->mMusicDelay);
    field_2C_tl.x = pTlv->mTopLeftX;
    field_2C_tl.y = pTlv->mTopLeftY;
    field_30_br.x = pTlv->mBottomRightX;
    field_30_br.y = pTlv->mBottomRightY;
    mTlvId = tlvId;
}

MusicTrigger::MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType musicType, relive::Path_MusicTrigger::TriggeredBy triggeredBy, s32 /*not_used*/, s32 musicDelay)
    : BaseGameObject(true, 0)
{
    Init(musicType, triggeredBy, static_cast<s16>(musicDelay));
    field_2C_tl = {};
    field_30_br = {};
    mTlvId = Guid{};
}

void MusicTrigger::Init(relive::Path_MusicTrigger::MusicTriggerMusicType musicType, relive::Path_MusicTrigger::TriggeredBy triggeredBy, s16 musicDelay)
{
    mTriggeredByTouching = false;
    mTriggeredByTimer = false;
    mSetMusicToNoneOnDtor = false;

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
            mSetMusicToNoneOnDtor = true;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase:
            field_26_music_type = MusicController::MusicTypes::eIntenseChase_7;
            mSetMusicToNoneOnDtor = true;
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
        mTriggeredByTouching = true;
    }
}

MusicTrigger::~MusicTrigger()
{
    if (mSetMusicToNoneOnDtor)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }
}

void MusicTrigger::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel)
    {
        SetDead(true);
    }
}

void MusicTrigger::VUpdate()
{
    if (EventGet(kEventHeroDying))
    {
        SetDead(true);
    }

    if (mTriggeredByTouching)
    {
        FP xpos = sControlledCharacter->mXPos;
        FP ypos = sControlledCharacter->mYPos;

        if (xpos >= FP_FromInteger(field_2C_tl.x) && xpos <= FP_FromInteger(field_30_br.x) && (ypos >= FP_FromInteger(field_2C_tl.y) && ypos <= FP_FromInteger(field_30_br.y)))
        {
            mTriggeredByTouching = false;
            MusicController::static_PlayMusic(field_26_music_type, this, mSetMusicToNoneOnDtor, 1);
            mTriggeredByTimer = true;
            if (field_28_counter >= 0)
            {
                field_28_counter += sGnFrame;
            }
        }
    }
    else if (mTriggeredByTimer)
    {
        if (field_28_counter < 0 || static_cast<s32>(sGnFrame) < field_28_counter)
        {
            MusicController::static_PlayMusic(field_26_music_type, this, mSetMusicToNoneOnDtor, 0);
        }
        else
        {
            SetDead(true);
        }
    }
    else
    {
        MusicController::static_PlayMusic(field_26_music_type, this, mSetMusicToNoneOnDtor, 1);
        mTriggeredByTimer = true;
        field_28_counter += sGnFrame;
    }
}
