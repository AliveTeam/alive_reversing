#include "stdafx_ao.h"
#include "Function.hpp"
#include "Throwable.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Grenade.hpp"
#include "Rock.hpp"
#include "Meat.hpp"

START_NS_AO

// Overlay to throwable type table ??
const Types word_4CF158[54] =
{
    Types::eNone_0,     Types::eNone_0,      Types::eNone_0,      Types::eGrenade_40,  Types::eNone_0,      Types::eNone_0,      Types::eRock_70,     Types::eNone_0,     Types::eRock_70,  Types::eRock_70,
    Types::eNone_0,     Types::eRock_70,     Types::eNone_0,      Types::eRock_70,     Types::eNone_0,      Types::eRock_70,     Types::eNone_0,      Types::eNone_0,     Types::eRock_70,  Types::eNone_0,
    Types::eMeat_54,    Types::eNone_0,      Types::eMeat_54,     Types::eNone_0,      Types::eRock_70,     Types::eNone_0,      Types::eRock_70,     Types::eRock_70,    Types::eRock_70,  Types::eNone_0,
    Types::eRock_70,    Types::eNone_0,      Types::eRock_70,     Types::eNone_0,      Types::eRock_70,     Types::eNone_0,      Types::eRock_70,     Types::eNone_0,     Types::eNone_0,   Types::eGrenade_40,
    Types::eNone_0,     Types::eGrenade_40,  Types::eGrenade_40,  Types::eGrenade_40,  Types::eGrenade_40,  Types::eGrenade_40,  Types::eGrenade_40,  Types::eGrenade_40, Types::eNone_0,   Types::eGrenade_40,
    Types::eNone_0,     Types::eNone_0,      Types::eNone_0,      Types::eNone_0
};

EXPORT BaseThrowable* CCSTD Make_Throwable_454560(FP xpos, FP ypos, __int16 count)
{
    switch (word_4CF158[gMap_507BA8.field_28_cd_or_overlay_num])
    {
    case Types::eGrenade_40:
    {
        auto pGrenade = ao_new<Grenade>();
        if (pGrenade)
        {
            return pGrenade->ctor_41EBD0(xpos, ypos, count);
        }
        break;
    }

    case Types::eMeat_54:
    {
        auto pMeat = ao_new<Meat>();
        if (pMeat)
        {
            return pMeat->ctor_438550(xpos, ypos, count);
        }
        break;
    }

    case Types::eRock_70:
    {
        auto pRock = ao_new<Rock>();
        if (pRock)
        {
            return pRock->ctor_456960(xpos, ypos, count);
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
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    field_10E_bDead = TRUE;
}


void BaseThrowable::VToDead()
{
    VToDead_41F990();
}


__int16 BaseThrowable::VGetCount_41F980()
{
    return field_10C_count;
}


__int16 BaseThrowable::VGetCount()
{
    return VGetCount_41F980();
}

void BaseThrowable::VOnPickUpOrSlapped()
{
    VToDead();
}

END_NS_AO

