#include "stdafx.h"
#include "Function.hpp"
#include "ScopedSeq.hpp"
#include "Map.hpp"
#include "Slog.hpp"
#include "Slig.hpp"
#include "Fleech.hpp"
#include "stdlib.hpp"

struct Sound_Ambiance final
{
    s32 field_0;
    s32 field_4;
    ScopedSeq* field_8_pScopedSeq;
};
ALIVE_ASSERT_SIZEOF(Sound_Ambiance, 0xC);

struct Sound_Ambiance_Array final
{
    Sound_Ambiance mArray[8];
};

ALIVE_VAR(1, 0xBB3078, Sound_Ambiance_Array, sTopBottomAmbiance_BB3078, {});
ALIVE_VAR(1, 0xBB30D8, Sound_Ambiance_Array, sRightAmbiance_BB30D8, {});
ALIVE_VAR(1, 0xBB3138, Sound_Ambiance_Array, sLeftAmbiance_BB3138, {});


void SND_Init_Ambiance()
{
    for (auto& amb : sTopBottomAmbiance_BB3078.mArray)
    {
        amb.field_8_pScopedSeq = nullptr;
    }

    for (auto& amb : sRightAmbiance_BB30D8.mArray)
    {
        amb.field_8_pScopedSeq = nullptr;
    }

    for (auto& amb : sLeftAmbiance_BB3138.mArray)
    {
        amb.field_8_pScopedSeq = nullptr;
    }
}


void SND_Reset_Ambiance()
{
    for (auto& amb : sTopBottomAmbiance_BB3078.mArray)
    {
        if (amb.field_8_pScopedSeq)
        {
            delete amb.field_8_pScopedSeq;
            amb.field_8_pScopedSeq = nullptr;
        }
    }

    for (auto& amb : sRightAmbiance_BB30D8.mArray)
    {
        if (amb.field_8_pScopedSeq)
        {
            delete amb.field_8_pScopedSeq;
            amb.field_8_pScopedSeq = nullptr;
        }
    }

    for (auto& amb : sLeftAmbiance_BB3138.mArray)
    {
        if (amb.field_8_pScopedSeq)
        {
            delete amb.field_8_pScopedSeq;
            amb.field_8_pScopedSeq = nullptr;
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
            pAmbianceTbl = &sTopBottomAmbiance_BB3078;
            break;

        case CameraPos::eCamLeft_3:
            pAmbianceTbl = &sLeftAmbiance_BB3138;
            break;

        case CameraPos::eCamRight_4:
            pAmbianceTbl = &sRightAmbiance_BB30D8;
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
                if (!pAmbianceTbl->mArray[1].field_8_pScopedSeq)
                {
                    pAmbianceTbl->mArray[1].field_8_pScopedSeq = relive_new ScopedSeq(1, direction);
                }
            }
            else if (pSligTlv->mData.mStartState == relive::Path_Slig_Data::StartState::Sleeping)
            {
                if (!pAmbianceTbl->mArray[0].field_8_pScopedSeq)
                {
                    pAmbianceTbl->mArray[0].field_8_pScopedSeq = relive_new ScopedSeq(0, direction);
                }
            }
            break;
        }

        case ReliveTypes::eSlog:
        {
            if (static_cast<relive::Path_Slog*>(pTlv)->mAsleep == relive::reliveChoice::eYes)
            {
                if (!pAmbianceTbl->mArray[3].field_8_pScopedSeq)
                {
                    pAmbianceTbl->mArray[3].field_8_pScopedSeq = relive_new ScopedSeq(3, direction);
                }
            }
            else
            {
                if (!pAmbianceTbl->mArray[2].field_8_pScopedSeq)
                {
                    pAmbianceTbl->mArray[2].field_8_pScopedSeq = relive_new ScopedSeq(2, direction);
                }
            }
            break;
        }

        case ReliveTypes::eParamite:
            if (!pAmbianceTbl->mArray[5].field_8_pScopedSeq)
            {
                pAmbianceTbl->mArray[5].field_8_pScopedSeq = relive_new ScopedSeq(5, direction);
            }
            break;

        case ReliveTypes::eScrab:
            if (!pAmbianceTbl->mArray[6].field_8_pScopedSeq)
            {
                pAmbianceTbl->mArray[6].field_8_pScopedSeq = relive_new ScopedSeq(6, direction);
            }
            break;

        case ReliveTypes::eFleech:
        {
            auto pFleechTlv = static_cast<relive::Path_Fleech*>(pTlv);
            if ((pFleechTlv->mAsleep == relive::reliveChoice::eYes || pFleechTlv->mHanging == relive::reliveChoice::eYes))
            {
                if (!pAmbianceTbl->mArray[4].field_8_pScopedSeq)
                {
                    pAmbianceTbl->mArray[4].field_8_pScopedSeq = relive_new ScopedSeq(4, direction);
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
            pTable = &sTopBottomAmbiance_BB3078;
            break;
        case CameraPos::eCamLeft_3:
            pTable = &sLeftAmbiance_BB3138;
            break;
        case CameraPos::eCamRight_4:
            pTable = &sRightAmbiance_BB30D8;
            break;
        default:
            return;
    }

    ScopedSeq** ppSeqPtr = &pTable->mArray[kZero].field_8_pScopedSeq;
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
            pTable = &sTopBottomAmbiance_BB3078;
            break;
        case CameraPos::eCamLeft_3:
            pTable = &sLeftAmbiance_BB3138;
            break;
        case CameraPos::eCamRight_4:
            pTable = &sRightAmbiance_BB30D8;
            break;
        default:
            return;
    }

    if (pTable->mArray[kZero].field_8_pScopedSeq)
    {
        delete pTable->mArray[kZero].field_8_pScopedSeq;
        pTable->mArray[kZero].field_8_pScopedSeq = nullptr;
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
