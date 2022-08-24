#include "stdafx_ao.h"
#include "Function.hpp"
#include "OneShotSwitchIdSetter.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"

namespace AO {

OneShotSwitchIdSetter::~OneShotSwitchIdSetter()
{
    Path::TLV_Reset(field_10_tlvInfo, -1, 0, 0);
}

OneShotSwitchIdSetter::OneShotSwitchIdSetter(Path_OneShotSwitchIdSetter* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_10_tlvInfo = tlvInfo;

    if (!SwitchStates_Get(236))
    {
        for (auto id : pTlv->field_18_ids_to_clear)
        {
            SwitchStates_Do_Operation(id, relive::reliveSwitchOp::eSetTrue);
        }
        SwitchStates_Do_Operation(236, relive::reliveSwitchOp::eSetTrue);
    }
}

void OneShotSwitchIdSetter::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

} // namespace AO
