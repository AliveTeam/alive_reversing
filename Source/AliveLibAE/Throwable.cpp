#include "stdafx.h"
#include "Throwable.hpp"
#include "Function.hpp"
#include "Bone.hpp"
#include "Grenade.hpp"
#include "Rock.hpp"
#include "Meat.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "PlatformBase.hpp"
#include "Game.hpp"

ALIVE_VAR(1, 0x5c1bde, WORD, gInfiniteGrenades_5C1BDE, 0);

// Overlay to throwable type table ??
const Types word_55FAFC[252] =
{
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eRock_105,   Types::eRock_105,  Types::eRock_105,   Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eRock_105,   Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eRock_105,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eRock_105,   Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eRock_105,     Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eRock_105,   Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eMeat_84,      Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eMeat_84,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eMeat_84,      Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eRock_105,   Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eBone_11,    Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,    Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eBone_11,    Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eBone_11,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eGrenade_65, Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eBone_11,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eBone_11,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,    Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eBone_11,    Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eGrenade_65, Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eGrenade_65, Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eBone_11,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eGrenade_65, Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0 };

EXPORT BaseThrowable* CCSTD Make_Throwable_49AF30(FP xpos, FP ypos, short count)
{
    switch (word_55FAFC[gMap_5C3030.field_22])
    {
    case Types::eBone_11:
        return ae_new<Bone>()->ctor_4112C0(xpos, ypos, count);
    case Types::eType_24:
        return ae_new<Grenade>()->ctor_447F70(xpos, ypos, count, 0, 1, 0);
    case Types::eGrenade_65:
        return ae_new<Grenade>()->ctor_447F70(xpos, ypos, count, 0, 0, 0);
    case Types::eMeat_84:
        return ae_new<Meat>()->ctor_4694A0(xpos, ypos, count);
    case Types::eRock_105:
        return ae_new<Rock>()->ctor_49E150(xpos, ypos, count);
    default:
        return nullptr;
    }
}

void BaseThrowable::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_4114D0();
}

__int16 BaseThrowable::VGetCount_448080()
{
    return vGetCount_448080();
}

void BaseThrowable::VToDead_4114B0()
{
    vToDead_4114B0();
}

void BaseThrowable::BaseAddToPlatform(BaseThrowable::FnTypeMatcher cb)
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos - FP_FromInteger(20),
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(20),
        &pLine,
        &hitX,
        &hitY,
        field_D6_scale == 0 ? 0xF0 : 0x10F))
    {
        if (pLine->field_8_type == 32 || pLine->field_8_type == 36)
        {
            if (ObjList_5C1B78)
            {
                for (int idx = 0; idx < ObjList_5C1B78->Size(); idx++)
                {
                    BaseGameObject* pObj = ObjList_5C1B78->ItemAt(idx);
                    if (!pObj)
                    {
                        break;
                    }

                    if (cb(pObj->field_4_typeId))
                    {
                        auto pPlatform = static_cast<PlatformBase*>(pObj);

                        PSX_RECT bRect = {};
                        pPlatform->vGetBoundingRect_424FD0(&bRect, 1);

                        if (FP_GetExponent(field_B8_xpos) > bRect.x  && FP_GetExponent(field_B8_xpos) < bRect.w  && FP_GetExponent(field_BC_ypos) < bRect.h)
                        {
                            pPlatform->VAdd(this);
                            field_110_id = pPlatform->field_8_object_id;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void BaseThrowable::vToDead_4114B0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    field_11A_bDead = 1;
}

void BaseThrowable::vOnPickUpOrSlapped_4114D0()
{
    VToDead_4114B0();
}

__int16 BaseThrowable::vGetCount_448080()
{
    return field_118_count;
}
