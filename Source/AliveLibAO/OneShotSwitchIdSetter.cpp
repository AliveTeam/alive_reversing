#include "stdafx_ao.h"
#include "Function.hpp"
#include "OneShotSwitchIdSetter.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"

namespace AO {

OneShotSwitchIdSetter::~OneShotSwitchIdSetter()
{
    gMap.TLV_Reset(field_10_tlvInfo, -1, 0, 0);
}

OneShotSwitchIdSetter::OneShotSwitchIdSetter(Path_OneShotSwitchIdSetter* pTlv, s32 tlvInfo)
    : BaseGameObject(1)
{
    field_10_tlvInfo = tlvInfo;

    if (!SwitchStates_Get(236))
    {
        for (auto id : pTlv->field_18_ids_to_clear)
        {
            SwitchStates_Do_Operation(id, SwitchOp::eSetTrue_0);
        }
        SwitchStates_Do_Operation(236, SwitchOp::eSetTrue_0);
    }
}

void OneShotSwitchIdSetter::VScreenChanged()
{
    mFlags.Set(BaseGameObject::eDead);
}

} // namespace AO
