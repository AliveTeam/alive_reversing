#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

enum ElectricWallStartState : s16
{
    eOff_0 = 0,
    eOn_1 = 1,
};

struct Path_ElectricWall final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_switch_id;
    ElectricWallStartState field_14_start_state;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ElectricWall, 0x18);

class ElectricWall final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ElectricWall* ctor_421DA0(Path_ElectricWall* pTlv, s32 tlvInfo);
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT ElectricWall* vdtor_421F70(s32 flags);
    EXPORT void dtor_421FA0();
    EXPORT void vScreenChanged_422530();
    EXPORT void vUpdate_422030();

private:
    s32 field_F4_tlvInfo;
    s16 field_F8_switch_id;
    ElectricWallStartState field_FA_start_state;
    s32 field_FC_sound_timer;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0x100);
