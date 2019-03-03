#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_ElectricWall : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_id;
    __int16 field_14_start_state;
    // pad
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ElectricWall, 0x18);

class ElectricWall : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ElectricWall* ctor_421DA0(Path_ElectricWall* pTlv, int tlvInfo);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT ElectricWall* vdtor_421F70(signed int flags);
    EXPORT void dtor_421FA0();
    EXPORT void vScreenChanged_422530();
    EXPORT void vUpdate_422030();

private:
    int field_E4_not_used[4];

    int field_F4_tlvInfo;
    __int16 field_F8_switch_id;
    __int16 field_FA_start_state;
    int field_FC_sound_timer;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0x100);
