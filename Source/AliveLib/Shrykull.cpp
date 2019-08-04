#include "stdafx.h"
#include "Shrykull.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "ObjectIds.hpp"
#include "Function.hpp"

Shrykull* Shrykull::ctor_4AEA20()
{
    ctor_408240(0);
    SetVTable(this, 0x5470F0);

    field_4_typeId = Types::eShrykull_121;

    field_6_flags.Set(BaseGameObject::eCanExplode);

    field_128_obj_being_zapped_id = -1;
    field_124_zap_line_id = -1;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kShrmorphResID);
    Animation_Init_424E10(82676, 123, 79u, ppRes, 1, 1u);

    field_B8_xpos = sActiveHero_5C1B68->field_B8_xpos;
    field_BC_ypos = sActiveHero_5C1B68->field_BC_ypos;
    field_CC_sprite_scale = sActiveHero_5C1B68->field_CC_sprite_scale;
    field_D6_scale = sActiveHero_5C1B68->field_D6_scale;

    field_118_state = 0;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, sActiveHero_5C1B68->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX));

    field_E0_pShadow = alive_new<Shadow>();
    field_E0_pShadow->ctor_4AC990();
    
    field_12E_bUnknown = 0;

    return this;
}

BaseGameObject* Shrykull::VDestructor(signed int flags)
{
    return vdtor_4AEB90(flags);
}

void Shrykull::VScreenChanged()
{
    vScreenChange_4AF650();
}

void Shrykull::dtor_4AEBC0()
{
    SetVTable(this, 0x5470F0);
    BaseGameObject* pZapLine = sObjectIds_5C1B70.Find_449CF0(field_124_zap_line_id);

    if (pZapLine)
    {
        pZapLine->field_6_flags.Set(BaseGameObject::eDead);
        field_124_zap_line_id = -1;
    }

    field_128_obj_being_zapped_id = -1;
    dtor_4080B0();
}

Shrykull* Shrykull::vdtor_4AEB90(signed int flags)
{
    dtor_4AEBC0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Shrykull::vScreenChange_4AF650()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}
