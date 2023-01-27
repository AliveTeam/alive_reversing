#include "stdafx.h"
#include "../relive_lib/Function.hpp"
#include "ScopedSeq.hpp"
#include "stdlib.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Path.hpp"

struct Sound_Ambiance final
{
    ScopedSeq* mScopedSeq;
};

struct Sound_Ambiance_Array final
{
    Sound_Ambiance mArray[8];
};

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
    Sound_Ambiance_Array* pAmbianceTbl = nullptr;
    switch (direction)
    {
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
            pAmbianceTbl = &sTopBottomAmbiance;
            break;

        case CameraPos::eCamLeft_3:
            pAmbianceTbl = &sLeftAmbiance;
            break;

        case CameraPos::eCamRight_4:
            pAmbianceTbl = &sRightAmbiance;
            break;

        default:
            return;
    }

    switch (pTlv->mTlvType)
    {
        case ReliveTypes::eSlig:
        {
            auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
            if (pSligTlv->mData.mStartState == relive::Path_Slig_Data::StartState::Patrol)
            {
                if (!pAmbianceTbl->mArray[1].mScopedSeq)
                {
                    pAmbianceTbl->mArray[1].mScopedSeq = relive_new ScopedSeq(1, direction);
                }
            }
            else if (pSligTlv->mData.mStartState == relive::Path_Slig_Data::StartState::Sleeping)
            {
                if (!pAmbianceTbl->mArray[0].mScopedSeq)
                {
                    pAmbianceTbl->mArray[0].mScopedSeq = relive_new ScopedSeq(0, direction);
                }
            }
            break;
        }

        case ReliveTypes::eSlog:
        {
            if (static_cast<relive::Path_Slog*>(pTlv)->mAsleep == relive::reliveChoice::eYes)
            {
                if (!pAmbianceTbl->mArray[3].mScopedSeq)
                {
                    pAmbianceTbl->mArray[3].mScopedSeq = relive_new ScopedSeq(3, direction);
                }
            }
            else
            {
                if (!pAmbianceTbl->mArray[2].mScopedSeq)
                {
                    pAmbianceTbl->mArray[2].mScopedSeq = relive_new ScopedSeq(2, direction);
                }
            }
            break;
        }

        case ReliveTypes::eParamite:
            if (!pAmbianceTbl->mArray[5].mScopedSeq)
            {
                pAmbianceTbl->mArray[5].mScopedSeq = relive_new ScopedSeq(5, direction);
            }
            break;

        case ReliveTypes::eScrab:
            if (!pAmbianceTbl->mArray[6].mScopedSeq)
            {
                pAmbianceTbl->mArray[6].mScopedSeq = relive_new ScopedSeq(6, direction);
            }
            break;

        case ReliveTypes::eFleech:
        {
            auto pFleechTlv = static_cast<relive::Path_Fleech*>(pTlv);
            if ((pFleechTlv->mAsleep == relive::reliveChoice::eYes || pFleechTlv->mHanging == relive::reliveChoice::eYes))
            {
                if (!pAmbianceTbl->mArray[4].mScopedSeq)
                {
                    pAmbianceTbl->mArray[4].mScopedSeq = relive_new ScopedSeq(4, direction);
                }
            }
            break;
        }

        default:
            return;
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

void Start_Sounds_For_Objects_In_Near_Cameras()
{
    SND_Reset_Ambiance();
    Path::Start_Sounds_For_Objects_In_Camera(CameraPos::eCamLeft_3, -1, 0);
    Path::Start_Sounds_For_Objects_In_Camera(CameraPos::eCamRight_4, 1, 0);
    Path::Start_Sounds_For_Objects_In_Camera(CameraPos::eCamTop_1, 0, -1);
    Path::Start_Sounds_For_Objects_In_Camera(CameraPos::eCamBottom_2, 0, 1);
}
