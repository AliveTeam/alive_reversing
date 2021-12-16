#include "stdafx_ao.h"
#include "BaseGameObject.hpp"
#include "DynamicArray.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Map.hpp"

void AliveLibAO_ForceLink()
{ }

namespace AO {

ALIVE_VAR(1, 0x9F2DF0, DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_9F2DF0, nullptr);

BaseGameObject* BaseGameObject::ctor_487E10(s16 arraySize)
{
    SetIFields();
    SetVTable(this, 0x4BD488); // vTable_BaseGameObject_4BD488

    field_8_update_delay = 0;
    field_4_typeId = Types::eNone_0;
    field_C_refCount = 0;

    InitFlags();

    if (!arraySize)
    {
        return this;
    }

    if (!gBaseGameObject_list_9F2DF0->Push_Back(this))
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }

    return this;
}

BaseGameObject* BaseGameObject::dtor_487DF0()
{
    SetVTable(this, 0x4BD488); // vTable_BaseGameObject_4BD488
    Event_Cancel_For_Obj_417350(this);
    return this;
}

void BaseGameObject::VScreenChanged_487E70()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path
        || gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void BaseGameObject::SetIFields()
{
    field_flags = &field_6_flags;
    field_update_delay = &field_8_update_delay;
};

} // namespace AO
