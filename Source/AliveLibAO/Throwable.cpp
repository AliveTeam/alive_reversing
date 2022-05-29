#include "stdafx_ao.h"
#include "Function.hpp"
#include "Throwable.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Grenade.hpp"
#include "Rock.hpp"
#include "Meat.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "PlatformBase.hpp"

namespace AO {

// Overlay to throwable type table ??
const Types word_4CF158[54] = {
    Types::eNone_0, Types::eNone_0, Types::eNone_0, Types::eGrenade_40, Types::eNone_0, Types::eNone_0, Types::eRock_70, Types::eNone_0, Types::eRock_70, Types::eRock_70,
    Types::eNone_0, Types::eRock_70, Types::eNone_0, Types::eRock_70, Types::eNone_0, Types::eRock_70, Types::eNone_0, Types::eNone_0, Types::eRock_70, Types::eNone_0,
    Types::eMeat_54, Types::eNone_0, Types::eMeat_54, Types::eNone_0, Types::eRock_70, Types::eNone_0, Types::eRock_70, Types::eRock_70, Types::eRock_70, Types::eNone_0,
    Types::eRock_70, Types::eNone_0, Types::eRock_70, Types::eNone_0, Types::eRock_70, Types::eNone_0, Types::eRock_70, Types::eNone_0, Types::eNone_0, Types::eGrenade_40,
    Types::eNone_0, Types::eGrenade_40, Types::eGrenade_40, Types::eGrenade_40, Types::eGrenade_40, Types::eGrenade_40, Types::eGrenade_40, Types::eGrenade_40, Types::eNone_0, Types::eGrenade_40,
    Types::eNone_0, Types::eNone_0, Types::eNone_0, Types::eNone_0};

EXPORT BaseThrowable* CCSTD Make_Throwable_454560(FP xpos, FP ypos, s16 count)
{
    switch (word_4CF158[gMap.mOverlayId])
    {
        case Types::eGrenade_40:
        {
            auto pGrenade = ao_new<Grenade>(xpos, ypos, count);
            if (pGrenade)
            {
                return pGrenade;
            }
            break;
        }

        case Types::eMeat_54:
        {
            auto pMeat = ao_new<Meat>(xpos, ypos, count);
            if (pMeat)
            {
                return pMeat;
            }
            break;
        }

        case Types::eRock_70:
        {
            auto pRock = ao_new<Rock>(xpos, ypos, count);
            if (pRock)
            {
                return pRock;
            }
            break;
        }

        default:
            break;
    }
    return nullptr;
}


void BaseThrowable::VToDead_41F990()
{
    mFlags.Set(BaseGameObject::eDead);
    field_10E_bDead = TRUE;
}


void BaseThrowable::VToDead()
{
    VToDead_41F990();
}


s16 BaseThrowable::VGetCount_41F980()
{
    return field_10C_count;
}


s16 BaseThrowable::VGetCount()
{
    return VGetCount_41F980();
}

void BaseThrowable::VOnPickUpOrSlapped()
{
    VToDead();
}

void BaseThrowable::BaseAddToPlatform()
{
    const FP scale = field_BC_sprite_scale - FP_FromDouble(0.5);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
            field_A8_xpos,
            field_AC_ypos - FP_FromInteger(20),
            field_A8_xpos,
            field_AC_ypos + FP_FromInteger(20),
            &pLine,
            &hitX,
            &hitY,
            scale != FP_FromInteger(0) ? 7 : 0x70))
    {
        if (pLine->field_8_type == eLineTypes::eUnknown_32 ||
            pLine->field_8_type == eLineTypes::eUnknown_36)
        {
            for (s32 i = 0; i < ObjListPlatforms_50766C->Size(); i++)
            {
                BaseGameObject* pObjIter = ObjListPlatforms_50766C->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->field_4_typeId == Types::eLiftPoint_51 || pObjIter->field_4_typeId == Types::eTrapDoor_98)
                {
                    auto pPlatformBase = static_cast<PlatformBase*>(pObjIter);

                    PSX_RECT objRect = {};
                    pPlatformBase->VGetBoundingRect(&objRect, 1);

                    if (FP_GetExponent(field_A8_xpos) > objRect.x && FP_GetExponent(field_A8_xpos) < objRect.w && FP_GetExponent(field_AC_ypos) < objRect.h)
                    {
                        if (field_F8_pLiftPoint)
                        {
                            if (field_F8_pLiftPoint == pPlatformBase)
                            {
                                return;
                            }
                            field_F8_pLiftPoint->VRemove(this);
                            field_F8_pLiftPoint->field_C_refCount--;
                            field_F8_pLiftPoint = nullptr;
                        }

                        field_F8_pLiftPoint = pPlatformBase;
                        field_F8_pLiftPoint->VAdd(this);
                        field_F8_pLiftPoint->field_C_refCount++;
                        return;
                    }
                }
            }
        }
    }
}

} // namespace AO
