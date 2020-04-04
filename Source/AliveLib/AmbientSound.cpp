#include "stdafx.h"
#include "Function.hpp"
#include "ScopedSeq.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "Slog.hpp"
#include "Slig.hpp"
#include "Fleech.hpp"
#include "stdlib.hpp"

struct Sound_Ambiance
{
    int field_0;
    int field_4;
    ScopedSeq* field_8_pScopedSeq;
};
ALIVE_ASSERT_SIZEOF(Sound_Ambiance, 0xC);

struct Sound_Ambiance_Array
{
    Sound_Ambiance mArray[8];
};

ALIVE_VAR(1, 0xBB3078, Sound_Ambiance_Array, sTopBottomAmbiance_BB3078, {});
ALIVE_VAR(1, 0xBB30D8, Sound_Ambiance_Array, sRightAmbiance_BB30D8, {});
ALIVE_VAR(1, 0xBB3138, Sound_Ambiance_Array, sLeftAmbiance_BB3138, {});


EXPORT void CC SND_Init_Ambiance_4CB480()
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


EXPORT void CC SND_Reset_Ambiance_4CB4B0()
{
    for (auto& amb : sTopBottomAmbiance_BB3078.mArray)
    {
        if (amb.field_8_pScopedSeq)
        {
            amb.field_8_pScopedSeq->VDestructor(1);
            amb.field_8_pScopedSeq = nullptr;
        }
    }

    for (auto& amb : sRightAmbiance_BB30D8.mArray)
    {
        if (amb.field_8_pScopedSeq)
        {
            amb.field_8_pScopedSeq->VDestructor(1);
            amb.field_8_pScopedSeq = nullptr;
        }
    }

    for (auto& amb : sLeftAmbiance_BB3138.mArray)
    {
        if (amb.field_8_pScopedSeq)
        {
            amb.field_8_pScopedSeq->VDestructor(1);
            amb.field_8_pScopedSeq = nullptr;
        }
    }
}


EXPORT void CC Start_Sounds_for_TLV_4CB530(CameraPos direction, Path_TLV* pTlv)
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

    switch (pTlv->field_4_type)
    {
    case Slig_15:
    {
        auto pSligTlv = static_cast<Path_Slig*>(pTlv);
        if (pSligTlv->field_12_start_state == Path_Slig::StartState::Paused_1)
        {
            if (!pAmbianceTbl->mArray[1].field_8_pScopedSeq)
            {
                pAmbianceTbl->mArray[1].field_8_pScopedSeq = alive_new<ScopedSeq>()->ctor_4CB210(1, direction);
            }
        }
        else if (pSligTlv->field_12_start_state == Path_Slig::StartState::Sleeping_2)
        {
            if (!pAmbianceTbl->mArray[0].field_8_pScopedSeq)
            {
                pAmbianceTbl->mArray[0].field_8_pScopedSeq = alive_new<ScopedSeq>()->ctor_4CB210(0, direction);
            }
        }
        break;
    }

    case Slog_16:
    {
        if (static_cast<Path_Slog*>(pTlv)->field_14_asleep)
        {
            if (!pAmbianceTbl->mArray[3].field_8_pScopedSeq)
            {
                pAmbianceTbl->mArray[3].field_8_pScopedSeq = alive_new<ScopedSeq>()->ctor_4CB210(3, direction);
            }
        }
        else
        {
            if (!pAmbianceTbl->mArray[2].field_8_pScopedSeq)
            {
                pAmbianceTbl->mArray[2].field_8_pScopedSeq = alive_new<ScopedSeq>()->ctor_4CB210(2, direction);
            }
        }
        break;
    }

    case Paramite_26:
        if (!pAmbianceTbl->mArray[5].field_8_pScopedSeq)
        {
            pAmbianceTbl->mArray[5].field_8_pScopedSeq = alive_new<ScopedSeq>()->ctor_4CB210(5, direction);
        }
        break;

    case Scrab_41:
        if (!pAmbianceTbl->mArray[6].field_8_pScopedSeq)
        {
            pAmbianceTbl->mArray[6].field_8_pScopedSeq = alive_new<ScopedSeq>()->ctor_4CB210(6, direction);
        }
        break;

    case Fleech_83:
    {
        auto pFleechTlv = static_cast<Path_Fleech*>(pTlv);
        if ((pFleechTlv->field_14_asleep || pFleechTlv->field_20_hanging))
        {
            if (!pAmbianceTbl->mArray[4].field_8_pScopedSeq)
            {
                pAmbianceTbl->mArray[4].field_8_pScopedSeq = alive_new<ScopedSeq>()->ctor_4CB210(4, direction);
            }
        }
        break;
    }

    default:
        return;
    }
}


EXPORT void CC Start_Slig_sounds_4CB980(CameraPos direction, char kZero)
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
        *ppSeqPtr = alive_new<ScopedSeq>()->ctor_4CB210(kZero, direction);
    }
}


EXPORT void CC Stop_slig_sounds_4CBA70(CameraPos direction, char kZero)
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
        pTable->mArray[kZero].field_8_pScopedSeq->VDestructor(1);
    }
}

EXPORT void CC Start_Sounds_For_Objects_In_Near_Cameras_4CBB60()
{
    SND_Reset_Ambiance_4CB4B0();
    Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(CameraPos::eCamLeft_3, -1, 0);
    Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(CameraPos::eCamRight_4, 1, 0);
    Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(CameraPos::eCamTop_1, 0, -1);
    Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(CameraPos::eCamBottom_2, 0, 1);
}
