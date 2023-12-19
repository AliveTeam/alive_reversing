#include "stdafx.h"
#include "AmbientSound.hpp"
#include "Function.hpp"
#include "ScopedSeq.hpp"
#include "MapWrapper.hpp"
#include "data_conversion/relive_tlvs.hpp"
#include "../AliveLibAE/Path.hpp"
#include "GameType.hpp"
#include "../AliveLibAO/MusicTrigger.hpp"

static Sound_Ambiance_Array sTopBottomAmbiance = {};
static Sound_Ambiance_Array sRightAmbiance = {};
static Sound_Ambiance_Array sLeftAmbiance = {};

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
        {
            if (GetGameType() == GameType::eAo)
            {
                // TODO: this is supposed to be sLeftAmbiance but it will desync the recording
                pAmbianceTbl = sRightAmbiance.mArray;
            }
            else
            {
                pAmbianceTbl = sLeftAmbiance.mArray;
            }
            break;
        }

        case CameraPos::eCamRight_4:
            pAmbianceTbl = sRightAmbiance.mArray;
            break;

        default:
            return;
    }

    switch (pTlv->mTlvType)
    {
        case ReliveTypes::eSlig:
        {
            const auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
            if (pSligTlv->mData.mStartState == relive::Path_Slig_Data::StartState::Patrol)
            {
                if (!pAmbianceTbl[AmbienceId::eSligPatrol].mScopedSeq)
                {
                    pAmbianceTbl[AmbienceId::eSligPatrol].mScopedSeq = relive_new ScopedSeq(AmbienceId::eSligPatrol, direction);
                    bDangerMusic = true;
                }
            }
            else if (pSligTlv->mData.mStartState == relive::Path_Slig_Data::StartState::Sleeping)
            {
                if (!pAmbianceTbl[AmbienceId::eSligSleeping].mScopedSeq)
                {
                    pAmbianceTbl[AmbienceId::eSligSleeping].mScopedSeq = relive_new ScopedSeq(AmbienceId::eSligSleeping, direction);
                    bDangerMusic = true;
                }
            }
            break;
        }

        case ReliveTypes::eSlog:
        {
            const auto pSlogTlv = static_cast<relive::Path_Slog*>(pTlv);
            if (pSlogTlv->mAsleep == relive::reliveChoice::eYes)
            {
                if (!pAmbianceTbl[AmbienceId::eSlogSleeping].mScopedSeq)
                {   
                    pAmbianceTbl[AmbienceId::eSlogSleeping].mScopedSeq = relive_new ScopedSeq(AmbienceId::eSlogSleeping, direction);
                    bDangerMusic = true;
                }
            }
            else
            {
                if (!pAmbianceTbl[AmbienceId::eSlogAwake].mScopedSeq)
                {
                    pAmbianceTbl[AmbienceId::eSlogAwake].mScopedSeq = relive_new ScopedSeq(AmbienceId::eSlogAwake, direction);
                    bDangerMusic = true;
                }
            }
            break;
        }

        case ReliveTypes::eParamite:
            if (!pAmbianceTbl[AmbienceId::eParamite].mScopedSeq)
            {
                pAmbianceTbl[AmbienceId::eParamite].mScopedSeq = relive_new ScopedSeq(AmbienceId::eParamite, direction);
                bDangerMusic = true;
            }
            break;

        case ReliveTypes::eScrab:
            if (!pAmbianceTbl[AmbienceId::eNone].mScopedSeq)
            {
                pAmbianceTbl[AmbienceId::eNone].mScopedSeq = relive_new ScopedSeq(AmbienceId::eNone, direction);
                bDangerMusic = true;
            }
            break;

        case ReliveTypes::eMeatSaw:
            if (!pAmbianceTbl[AmbienceId::eMeatSaw].mScopedSeq)
            {
                pAmbianceTbl[AmbienceId::eMeatSaw].mScopedSeq = relive_new ScopedSeq(AmbienceId::eMeatSaw, direction);
            }
            break;

        case ReliveTypes::eFleech:
        {
            auto pFleechTlv = static_cast<relive::Path_Fleech*>(pTlv);
            if ((pFleechTlv->mAsleep == relive::reliveChoice::eYes || pFleechTlv->mHanging == relive::reliveChoice::eYes))
            {
                if (!pAmbianceTbl[AmbienceId::eFleechAsleep].mScopedSeq)
                {
                    pAmbianceTbl[AmbienceId::eFleechAsleep].mScopedSeq = relive_new ScopedSeq(AmbienceId::eFleechAsleep, direction);
                }
            }
            break;
        }

        default:
            break;
    }

    if (bDangerMusic && GetGameType() == GameType::eAo)
    {
        relive_new AO::MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 40);
    }
}


void Start_Slig_sounds(CameraPos direction)
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

    ScopedSeq** ppSeqPtr = &pTable->mArray[AmbienceId::eSligSleeping].mScopedSeq;
    if (!*ppSeqPtr)
    {
        *ppSeqPtr = relive_new ScopedSeq(AmbienceId::eSligSleeping, direction);
    }
}


void Stop_slig_sounds(CameraPos direction)
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

    if (pTable->mArray[AmbienceId::eSligSleeping].mScopedSeq)
    {
        delete pTable->mArray[AmbienceId::eSligSleeping].mScopedSeq;
        pTable->mArray[AmbienceId::eSligSleeping].mScopedSeq = nullptr;
    }
}

// AE only
void Start_Sounds_For_Objects_In_Near_Cameras()
{
    SND_Reset_Ambiance();
    Path::Start_Sounds_For_Objects_In_Camera(CameraPos::eCamLeft_3, -1, 0);
    Path::Start_Sounds_For_Objects_In_Camera(CameraPos::eCamRight_4, 1, 0);
    Path::Start_Sounds_For_Objects_In_Camera(CameraPos::eCamTop_1, 0, -1);
    Path::Start_Sounds_For_Objects_In_Camera(CameraPos::eCamBottom_2, 0, 1);
}
