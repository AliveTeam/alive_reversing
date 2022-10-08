#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"

namespace AO {

struct Path_OneShotSwitchIdSetter final : public Path_TLV
{
    s16 field_18_ids_to_clear[8];
};
ALIVE_ASSERT_SIZEOF(Path_OneShotSwitchIdSetter, 0x28);

class OneShotSwitchIdSetter final : public ::BaseGameObject
{
public:
    OneShotSwitchIdSetter(Path_OneShotSwitchIdSetter* pTlv, const Guid& tlvId);
    ~OneShotSwitchIdSetter();

    virtual void VScreenChanged() override;

    Guid field_10_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(OneShotSwitchIdSetter, 0x14);

} // namespace AO
