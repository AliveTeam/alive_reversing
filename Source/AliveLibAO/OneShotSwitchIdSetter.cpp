#include "stdafx_ao.h"
#include "Function.hpp"
#include "OneShotSwitchIdSetter.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"

namespace AO {

void OneShotSwitchIdSetter::VScreenChanged()
{
    VScreenChanged_432F00();
}

BaseGameObject* OneShotSwitchIdSetter::Vdtor_432F10(s32 flags)
{
    dtor_432EA0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* OneShotSwitchIdSetter::VDestructor(s32 flags)
{
    return Vdtor_432F10(flags);
}

BaseGameObject* OneShotSwitchIdSetter::dtor_432EA0()
{
    SetVTable(this, 0x4BB3D8);
    gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

BaseGameObject* OneShotSwitchIdSetter::ctor_432E10(Path_OneShotSwitchIdSetter* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB3D8);

    field_10_tlvInfo = tlvInfo;

    if (!SwitchStates_Get(236))
    {
        for (auto id : pTlv->field_18_ids_to_clear)
        {
            SwitchStates_Do_Operation_436A10(id, SwitchOp::eSetTrue_0);
        }
        SwitchStates_Do_Operation_436A10(236, SwitchOp::eSetTrue_0);
    }

    return this;
}

void OneShotSwitchIdSetter::VScreenChanged_432F00()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

} // namespace AO
