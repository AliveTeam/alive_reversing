#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum ElectricWallStartState : __int16
{
    eOff_0 = 0,
    eOn_1 = 1,
};

struct Path_ElectricWall : public Path_TLV
{
    Scale_short field_10_scale;
    __int16 field_12_id;
    ElectricWallStartState field_14_start_state;
    __int16 field_16_pad;
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

    int field_F4_tlvInfo;
    __int16 field_F8_switch_id;
    ElectricWallStartState field_FA_start_state;
    int field_FC_sound_timer;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0x100);
