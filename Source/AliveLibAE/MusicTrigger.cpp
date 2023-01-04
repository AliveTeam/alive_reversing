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
    mTlvTopLeft.x = pTlv->mTopLeftX;
    mTlvTopLeft.y = pTlv->mTopLeftY;
    mTlvBottomRight.x = pTlv->mBottomRightX;
    mTlvBottomRight.y = pTlv->mBottomRightY;
    mTlvId = tlvId;
}

MusicTrigger::MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType musicType, relive::Path_MusicTrigger::TriggeredBy triggeredBy, s32 musicDelay)
    : BaseGameObject(true, 0)
{
    Init(musicType, triggeredBy, static_cast<s16>(musicDelay));
    mTlvTopLeft = {};
    mTlvBottomRight = {};
    mTlvId = Guid{};
}

void MusicTrigger::Init(relive::Path_MusicTrigger::MusicTriggerMusicType musicType, relive::Path_MusicTrigger::TriggeredBy triggeredBy, s16 musicDelay)
{
    mTriggeredByTouching = false;
    mTriggeredByTimer = false;
    mSetMusicToNoneOnDtor = false;

    SetType(ReliveTypes::eMusicTrigger);
    mCounter = 0;

    switch (musicType)
    {
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience:
            mMusicType = MusicController::MusicTypes::eDrumAmbience_3;
            mCounter = 400;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort:
            mMusicType = MusicController::MusicTypes::eDeathDrumShort_10;
            mCounter = 30;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong:
            mMusicType = MusicController::MusicTypes::eSecretAreaLong_13;
            mCounter = 30;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase:
            mMusicType = MusicController::MusicTypes::eSoftChase_8;
            mSetMusicToNoneOnDtor = true;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase:
            mMusicType = MusicController::MusicTypes::eIntenseChase_7;
            mSetMusicToNoneOnDtor = true;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eChime:
            mMusicType = MusicController::MusicTypes::eChime_2;
            mCounter = musicDelay;
            break;
        case relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort:
            mMusicType = MusicController::MusicTypes::eSecretAreaShort_12;
            mCounter = 30;
            break;
        default:
            break;
    }

    if (triggeredBy == relive::Path_MusicTrigger::TriggeredBy::eTimer)
    {
        SetUpdateDelay(musicDelay); // OG bug? mBaseGameObjectUpdateDelay should've been mCounter?
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
        const FP xpos = sControlledCharacter->mXPos;
        const FP ypos = sControlledCharacter->mYPos;

        if (xpos >= FP_FromInteger(mTlvTopLeft.x) && xpos <= FP_FromInteger(mTlvBottomRight.x) && (ypos >= FP_FromInteger(mTlvTopLeft.y) && ypos <= FP_FromInteger(mTlvBottomRight.y)))
        {
            mTriggeredByTouching = false;
            MusicController::static_PlayMusic(mMusicType, this, mSetMusicToNoneOnDtor, 1);
            mTriggeredByTimer = true;
            if (mCounter >= 0)
            {
                mCounter += sGnFrame;
            }
        }
    }
    else if (mTriggeredByTimer)
    {
        if (mCounter < 0 || static_cast<s32>(sGnFrame) < mCounter)
        {
            MusicController::static_PlayMusic(mMusicType, this, mSetMusicToNoneOnDtor, 0);
        }
        else
        {
            SetDead(true);
        }
    }
    else
    {
        MusicController::static_PlayMusic(mMusicType, this, mSetMusicToNoneOnDtor, 1);
        mTriggeredByTimer = true;
        mCounter += sGnFrame;
    }
}
