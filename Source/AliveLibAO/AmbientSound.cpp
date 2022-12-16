#include "stdafx_ao.h"
#include "AmbientSound.hpp"
#include "../relive_lib/Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ScopedSeq.hpp"
#include "MusicTrigger.hpp"

namespace AO {

static Sound_Ambiance_Array sTopBottomAmbiance = {};
static Sound_Ambiance_Array sRightAmbiance = {};
static Sound_Ambiance_Array sLeftAmbiance = {};

void SND_Reset_Ambiance()
{
    for (auto& amb : sTopBottomAmbiance.mArray)
    {
        if (amb.mScopedSeq)
        {
            delete amb.mScopedSeq;
            amb.mScopedSeq = nullptr;
        }
    }

    for (auto& amb : sRightAmbiance.mArray)
    {
        if (amb.mScopedSeq)
        {
            delete amb.mScopedSeq;
            amb.mScopedSeq = nullptr;
        }
    }

    for (auto& amb : sLeftAmbiance.mArray)
    {
        if (amb.mScopedSeq)
        {
            delete amb.mScopedSeq;
            amb.mScopedSeq = nullptr;
        }
    }
}

void Start_Sounds_for_TLV(CameraPos direction, relive::Path_TLV* pTlv)
{
    bool bDangerMusic = false;

    Sound_Ambiance* pAmbianceTbl = nullptr;
    switch (direction)
    {
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
            pAmbianceTbl = sTopBottomAmbiance.mArray;
            break;

        case CameraPos::eCamLeft_3:
            pAmbianceTbl = sRightAmbiance.mArray;
            break;

        case CameraPos::eCamRight_4:
            pAmbianceTbl = sRightAmbiance.mArray;
            break;

        default:
            return;
    }

    if (pAmbianceTbl)
    {
        switch (pTlv->mTlvType)
        {
            case ReliveTypes::eSlig:
                if (static_cast<relive::Path_Slig*>(pTlv)->mData.mStartState == relive::Path_Slig_Data::StartState::Patrol)
                {
                    if (!pAmbianceTbl[1].mScopedSeq)
                    {
                        pAmbianceTbl[1].mScopedSeq = relive_new ScopedSeq(1, direction);
                        bDangerMusic = true;
                    }
                }
                else if (static_cast<relive::Path_Slig*>(pTlv)->mData.mStartState == relive::Path_Slig_Data::StartState::Sleeping)
                {
                    if (!pAmbianceTbl->mScopedSeq)
                    {
                        pAmbianceTbl[0].mScopedSeq = relive_new ScopedSeq(0, direction);
                        bDangerMusic = true;
                    }
                }
                break;

            case ReliveTypes::eSlog:
                if (static_cast<relive::Path_Slog*>(pTlv)->mAsleep == relive::reliveChoice::eYes)
                {
                    if (!pAmbianceTbl[3].mScopedSeq)
                    {
                        pAmbianceTbl[3].mScopedSeq = relive_new ScopedSeq(3, direction);
                        bDangerMusic = true;
                    }
                }
                else
                {
                    if (!pAmbianceTbl[2].mScopedSeq)
                    {
                        pAmbianceTbl[2].mScopedSeq = relive_new ScopedSeq(2, direction);
                        bDangerMusic = true;
                    }
                }
                break;

            case ReliveTypes::eParamite:
                if (!pAmbianceTbl[4].mScopedSeq)
                {
                    pAmbianceTbl[4].mScopedSeq = relive_new ScopedSeq(4, direction);
                    bDangerMusic = true;
                }
                break;

            case ReliveTypes::eScrab:
                if (!pAmbianceTbl[5].mScopedSeq)
                {
                    pAmbianceTbl[5].mScopedSeq = relive_new ScopedSeq(5, direction);
                    bDangerMusic = true;
                }
                break;

            case ReliveTypes::eMeatSaw:
                if (!pAmbianceTbl[6].mScopedSeq)
                {
                    pAmbianceTbl[6].mScopedSeq = relive_new ScopedSeq(6, direction);
                }
                break;

            default:
                break;
        }

        if (bDangerMusic)
        {
            relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 40);
        }
    }
}

void Start_Slig_sounds(CameraPos direction, s8 kZero)
{
    Sound_Ambiance_Array* pTable = nullptr;
    switch (direction)
    {
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
            pTable = &sTopBottomAmbiance;
            break;
        case CameraPos::eCamLeft_3:
            pTable = &sLeftAmbiance;
            break;
        case CameraPos::eCamRight_4:
            pTable = &sRightAmbiance;
            break;
        default:
            return;
    }

    ScopedSeq** ppSeqPtr = &pTable->mArray[kZero].mScopedSeq;
    if (!*ppSeqPtr)
    {
        *ppSeqPtr = relive_new ScopedSeq(kZero, direction);
    }
}

void Stop_slig_sounds(CameraPos direction, s8 kZero)
{
    Sound_Ambiance_Array* pTable = nullptr;
    switch (direction)
    {
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
            pTable = &sTopBottomAmbiance;
            break;
        case CameraPos::eCamLeft_3:
            pTable = &sLeftAmbiance;
            break;
        case CameraPos::eCamRight_4:
            pTable = &sRightAmbiance;
            break;
        default:
            return;
    }

    if (pTable->mArray[kZero].mScopedSeq)
    {
        delete pTable->mArray[kZero].mScopedSeq;
        pTable->mArray[kZero].mScopedSeq = nullptr;
    }
}


void SND_Init_Ambiance()
{
    for (auto& amb : sTopBottomAmbiance.mArray)
    {
        amb.mScopedSeq = nullptr;
    }

    for (auto& amb : sRightAmbiance.mArray)
    {
        amb.mScopedSeq = nullptr;
    }

    for (auto& amb : sLeftAmbiance.mArray)
    {
        amb.mScopedSeq = nullptr;
    }
}

} // namespace AO
