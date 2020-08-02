#include "stdafx_ao.h"
#include "Function.hpp"
#include "OneShotSwitchIdSetter.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"

START_NS_AO

void OneShotSwitchIdSetter::VScreenChanged()
{
    VScreenChanged_432F00();
}

EXPORT BaseGameObject* OneShotSwitchIdSetter::Vdtor_432F10(signed int flags)
{
    dtor_432EA0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* OneShotSwitchIdSetter::VDestructor(signed int flags)
{
    return Vdtor_432F10(flags);
}

BaseGameObject* OneShotSwitchIdSetter::dtor_432EA0()
{
    SetVTable(this, 0x4BB3D8);
    gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

BaseGameObject* OneShotSwitchIdSetter::ctor_432E10(Path_OneShotSwitchIdSetter* pTlv, int tlvInfo)
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

END_NS_AO

