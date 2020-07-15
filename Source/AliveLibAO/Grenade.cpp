#include "stdafx_ao.h"
#include "Function.hpp"
#include "Grenade.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "LiftPoint.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5076EC, short, gInfiniteGrenades_5076EC, 0);

Grenade* Grenade::ctor_41EBD0(FP xpos, FP ypos, __int16 numGrenades)
{
    ctor_401090();
    field_10E_bDead = 0;
    SetVTable(this, 0x4BB0A0);
    field_4_typeId = Types::eGrenade_40;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6005, 1, 0);
    Animation_Init_417FD0(
        1132,
        17,
        11,
        ppRes,
        1);

    field_6_flags.Clear(Options::eInteractive_Bit8);
    field_10_anim.field_B_render_mode = 0;

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_A8_xpos = xpos;
    field_120 = xpos;

    field_AC_ypos = ypos;
    field_124 = ypos;

    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_10C_count = numGrenades;

    if (numGrenades > 0)
    {
        field_110_state = 0;
    }
    else
    {
        field_110_state = 3;
        field_112_explode_timer = 90;
    }

    field_118 = 0;
    field_11C = 0;
    return this;
}

BaseGameObject* Grenade::dtor_41ECD0()
{
    SetVTable(this, 0x4BB0A0);

    if (field_11C)
    {
        field_11C->field_C_refCount--;
    }

    if (!gInfiniteGrenades_5076EC && !field_10E_bDead)
    {
        if (field_F8_pLiftPoint)
        {
            field_F8_pLiftPoint->VRemove(this);
            field_F8_pLiftPoint->field_C_refCount--;
            field_F8_pLiftPoint = nullptr;
        }

        if (gpThrowableArray_50E26C)
        {
            __int16 count = 0;
            if (field_10C_count >= 1u)
            {
                count = field_10C_count;
            }
            else
            {
                count = 1;
            }
            gpThrowableArray_50E26C->Remove_4540D0(count);
        }
    }
    return dtor_401000();
}

END_NS_AO

