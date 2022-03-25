#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

enum ElectricWallStartState : s16
{
    eOff_0 = 0,
    eOn_1 = 1,
};

struct Path_ElectricWall final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_switch_id;
    ElectricWallStartState field_1C_start_state;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ElectricWall, 0x20);


class ElectricWall final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ElectricWall* ctor_40FCF0(Path_ElectricWall* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_40FE80();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT ElectricWall* Vdtor_410280(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_410220();

    virtual void VUpdate() override;

    EXPORT void VUpdate_40FEF0();

    s32 field_D4_padding[4];
    s32 field_E4_tlv;
    s16 field_E8_switch_id;
    ElectricWallStartState field_EA_start_state;
    s32 field_EC_sound_timer;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0xF0);

} // namespace AO
