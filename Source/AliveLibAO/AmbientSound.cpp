#include "stdafx_ao.h"
#include "AmbientSound.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ScopedSeq.hpp"
#include "MusicTrigger.hpp"
#include "PathData.hpp"
#include "Slig.hpp"
#include "Slog.hpp"

namespace AO {

ALIVE_VAR(1, 0x9F11D0, Sound_Ambiance_Array, sTopBottomAmbiance_9F11D0, {});
ALIVE_VAR(1, 0x9F1228, Sound_Ambiance_Array, sRightAmbiance_9F1228, {});
ALIVE_VAR(1, 0x9F1280, Sound_Ambiance_Array, sLeftAmbiance_9F1280, {});

EXPORT void CC SND_Reset_Ambiance_4765E0()
{
    for (auto& amb : sTopBottomAmbiance_9F11D0.mArray)
    {
        if (amb.field_8_pScopedSeq)
        {
            amb.field_8_pScopedSeq->VDestructor(1);
            amb.field_8_pScopedSeq = nullptr;
        }
    }

    for (auto& amb : sRightAmbiance_9F1228.mArray)
    {
        if (amb.field_8_pScopedSeq)
        {
            amb.field_8_pScopedSeq->VDestructor(1);
            amb.field_8_pScopedSeq = nullptr;
        }
    }

    for (auto& amb : sLeftAmbiance_9F1280.mArray)
    {
        if (amb.field_8_pScopedSeq)
        {
            amb.field_8_pScopedSeq->VDestructor(1);
            amb.field_8_pScopedSeq = nullptr;
        }
    }
}

EXPORT void CC Start_Sounds_for_TLV_476640(CameraPos direction, Path_TLV* pTlv)
{
    bool bDangerMusic = false;

    Sound_Ambiance* pAmbianceTbl = nullptr;
    switch (direction)
    {
    case CameraPos::eCamTop_1:
    case CameraPos::eCamBottom_2:
        pAmbianceTbl = sTopBottomAmbiance_9F11D0.mArray;
        break;

    case CameraPos::eCamLeft_3:
        pAmbianceTbl = sRightAmbiance_9F1228.mArray;
       break;
 
    case CameraPos::eCamRight_4:
        pAmbianceTbl = sRightAmbiance_9F1228.mArray;
        break;

    default:
        return;
    }

    if (pAmbianceTbl)
    {
        switch (pTlv->field_4_type.mType)
        {
        case TlvTypes::Slig_24:
            if (static_cast<Path_Slig*>(pTlv)->field_1A_start_state == Path_Slig::StartState::Paused_1)
            {
                if (!pAmbianceTbl[1].field_8_pScopedSeq)
                {
                    pAmbianceTbl[1].field_8_pScopedSeq = ao_new<ScopedSeq>();
                    if (pAmbianceTbl[1].field_8_pScopedSeq)
                    {
                        pAmbianceTbl[1].field_8_pScopedSeq->ctor_476400(1, direction);
                    }
                    bDangerMusic = true;
                }
            }
            else if (static_cast<Path_Slig*>(pTlv)->field_1A_start_state == Path_Slig::StartState::Sleeping_2)
            {
                if (!pAmbianceTbl->field_8_pScopedSeq)
                {
                    pAmbianceTbl[0].field_8_pScopedSeq = ao_new<ScopedSeq>();
                    if (pAmbianceTbl[0].field_8_pScopedSeq)
                    {
                        pAmbianceTbl[0].field_8_pScopedSeq->ctor_476400(0, direction);
                    }
                    bDangerMusic = true;
                }
            }
            break;

        case TlvTypes::Slog_25:
            if (static_cast<Path_Slog*>(pTlv)->field_1C_wakeup_anger)
            {
                if (!pAmbianceTbl[3].field_8_pScopedSeq)
                {
                    pAmbianceTbl[3].field_8_pScopedSeq = ao_new<ScopedSeq>();
                    if (pAmbianceTbl[3].field_8_pScopedSeq)
                    {
                        pAmbianceTbl[3].field_8_pScopedSeq->ctor_476400(3, direction);
                    }
                    bDangerMusic = true;
                }
            }
            else
            {
                if (!pAmbianceTbl[2].field_8_pScopedSeq)
                {
                    pAmbianceTbl[2].field_8_pScopedSeq = ao_new<ScopedSeq>();
                    if (pAmbianceTbl[2].field_8_pScopedSeq)
                    {
                        pAmbianceTbl[2].field_8_pScopedSeq->ctor_476400(2, direction);
                    }
                    bDangerMusic = true;
                }
            }
            break;

        case TlvTypes::Paramite_48:
            if (!pAmbianceTbl[4].field_8_pScopedSeq)
            {
                pAmbianceTbl[4].field_8_pScopedSeq = ao_new<ScopedSeq>();
                if (pAmbianceTbl[4].field_8_pScopedSeq)
                {
                    pAmbianceTbl[4].field_8_pScopedSeq->ctor_476400(4, direction);
                }
                bDangerMusic = true;
            }
            break;

        case TlvTypes::Scrab_72:
            if (!pAmbianceTbl[5].field_8_pScopedSeq)
            {
                pAmbianceTbl[5].field_8_pScopedSeq = ao_new<ScopedSeq>();
                if (pAmbianceTbl[5].field_8_pScopedSeq)
                {
                    pAmbianceTbl[5].field_8_pScopedSeq->ctor_476400(5, direction);
                }
                bDangerMusic = true;
            }
            break;

        case TlvTypes::MeatSaw_88:
            if (!pAmbianceTbl[6].field_8_pScopedSeq)
            {
                pAmbianceTbl[6].field_8_pScopedSeq = ao_new<ScopedSeq>();
                if (pAmbianceTbl[6].field_8_pScopedSeq)
                {
                    pAmbianceTbl[6].field_8_pScopedSeq->ctor_476400(6, direction);
                }
            }
            break;

        default:
            break;
        }

        if (bDangerMusic)
        {
            auto pMusicTrigger = ao_new<MusicTrigger>();
            if (pMusicTrigger)
            {
                pMusicTrigger->ctor_443A60(0, 1, 0, 40);
            }
        }
    }
}

EXPORT void CC Start_Slig_sounds_476960(CameraPos direction, s8 kZero)
{
    Sound_Ambiance_Array* pTable = nullptr;
    switch (direction)
    {
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
            pTable = &sTopBottomAmbiance_9F11D0;
            break;
        case CameraPos::eCamLeft_3:
            pTable = &sLeftAmbiance_9F1280;
            break;
        case CameraPos::eCamRight_4:
            pTable = &sRightAmbiance_9F1228;
            break;
        default:
            return;
    }

    ScopedSeq** ppSeqPtr = &pTable->mArray[kZero].field_8_pScopedSeq;
    if (!*ppSeqPtr)
    {
        *ppSeqPtr = ao_new<ScopedSeq>()->ctor_476400(kZero, direction);
    }
}

EXPORT void CC Stop_slig_sounds_476A20(CameraPos direction, s8 kZero)
{
    Sound_Ambiance_Array* pTable = nullptr;
    switch (direction)
    {
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
            pTable = &sTopBottomAmbiance_9F11D0;
            break;
        case CameraPos::eCamLeft_3:
            pTable = &sLeftAmbiance_9F1280;
            break;
        case CameraPos::eCamRight_4:
            pTable = &sRightAmbiance_9F1228;
            break;
        default:
            return;
    }

    if (pTable->mArray[kZero].field_8_pScopedSeq)
    {
        pTable->mArray[kZero].field_8_pScopedSeq->VDestructor(1);
    }
}


EXPORT void CC SND_Init_Ambiance_4765C0()
{
    for (auto& amb : sTopBottomAmbiance_9F11D0.mArray)
    {
        amb.field_8_pScopedSeq = nullptr;
    }

    for (auto& amb : sRightAmbiance_9F1228.mArray)
    {
        amb.field_8_pScopedSeq = nullptr;
    }

    for (auto& amb : sLeftAmbiance_9F1280.mArray)
    {
        amb.field_8_pScopedSeq = nullptr;
    }
}

}
