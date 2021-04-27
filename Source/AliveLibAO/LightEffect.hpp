#pragma once

#include "FunctionFwd.hpp"
#include "DoorLight.hpp"

namespace AO {

class LightEffect : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT LightEffect* ctor_4064C0(Path_LightEffect* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_406770();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT LightEffect* Vdtor_406800(s32 flags);

    EXPORT void VRender_4067F0(PrimHeader** ppOt);

    virtual void VRender(PrimHeader** ppOt) override;

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4067E0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_406610();

    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    s32 field_E0;

    s32 field_E4_tlvInfo;
    s16 field_E8;
    s16 field_EA;
    s32 field_EC_rnd1;
    s32 field_F0_rnd2;
    s32 field_F4_rnd3;
    s32 field_F8_rnd4;
};
ALIVE_ASSERT_SIZEOF(LightEffect, 0xFC);

} // namespace AO
