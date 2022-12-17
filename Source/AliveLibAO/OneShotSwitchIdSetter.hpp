#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "Path.hpp"

namespace AO {

struct Path_OneShotSwitchIdSetter final : public Path_TLV
{
    s16 mIdsToClear[8];
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_OneShotSwitchIdSetter, 0x28);

class OneShotSwitchIdSetter final : public ::BaseGameObject
{
public:
    OneShotSwitchIdSetter(Path_OneShotSwitchIdSetter* pTlv, const Guid& tlvId);
    ~OneShotSwitchIdSetter();

    virtual void VScreenChanged() override;

    Guid mTlvId;
};

} // namespace AO
