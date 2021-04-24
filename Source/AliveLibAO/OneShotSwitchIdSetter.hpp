#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

namespace AO {

struct Path_OneShotSwitchIdSetter : public Path_TLV
{
    s16 field_18_ids_to_clear[8];
};
ALIVE_ASSERT_SIZEOF(Path_OneShotSwitchIdSetter, 0x28);

class OneShotSwitchIdSetter : public BaseGameObject
{
public:
    EXPORT BaseGameObject* ctor_432E10(Path_OneShotSwitchIdSetter* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_432EA0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_432F10(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_432F00();

    int field_10_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(OneShotSwitchIdSetter, 0x14);

}

