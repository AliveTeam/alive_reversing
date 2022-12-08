#include "stdafx_ao.h"
#include "OneShotSwitchIdSetter.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

OneShotSwitchIdSetter::~OneShotSwitchIdSetter()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

OneShotSwitchIdSetter::OneShotSwitchIdSetter(Path_OneShotSwitchIdSetter* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvId = tlvId;

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
    SetDead(true);
}

} // namespace AO
