#include "stdafx.h"
#include "Rock.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "Abe.hpp"

Rock* Rock::ctor_49E150(FP xpos, FP ypos, __int16 count)
{
    NOT_IMPLEMENTED();

    ctor_408240(0);
    SetVTable(this, 0x546AF8);
    field_4_typeId = Types::eRock_105;

    field_11A_bDead = 0;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 350, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 350);
    Animation_Init_424E10(488, 17, 9, ppRes, 1, 1);
    field_6_flags.Clear(BaseGameObject::eInteractive);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_118_count = count;
    field_11C_state = 0;

    BYTE** ppPal = ResourceManager::GetLoadedResource_49C2A0('tlaP', 350, 0, 0);
    if (ppPal)
    {
        field_20_animation.Load_Pal_40A530(ppPal, 0);
    }
    else
    {
        // Pal offset is... WTF ??
        //field_20_animation.Load_Pal_40A530(field_20_animation.field_20_ppBlock, *(_DWORD *)&(*field_20_animation.field_20_ppBlock)[*(_DWORD *)&(*field_20_animation.field_20_ppBlock)[*((_DWORD *)*field_20_animation.field_20_ppBlock + 124)]]);
    }

    field_11E = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

void Rock::VScreenChanged()
{
    vScreenChanged_49F030();
}

int Rock::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_49F9A0(pSaveBuffer);
}

EXPORT void Rock::vScreenChanged_49F030()
{
    NOT_IMPLEMENTED();
}

EXPORT int Rock::vGetSaveState_49F9A0(BYTE* /*pSaveBuffer*/)
{
    NOT_IMPLEMENTED();
    return 0;
}
