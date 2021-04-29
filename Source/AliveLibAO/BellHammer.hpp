#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

struct Path_BellHammer final : public Path_TLV
{
    s16 field_18_id;
    s16 field_1A_action;
    s16 field_1C_scale;
    s16 field_1E_direction;
};
ALIVE_ASSERT_SIZEOF(Path_BellHammer, 0x20);

enum class BellHammerStates : u16
{
    eWaitForActivation_0 = 0,
    eSmashingBell_1 = 1
};

class BellHammer : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BellHammer* ctor_405010(Path_BellHammer* pTlv, s32 tlvInfo);
    EXPORT BaseGameObject* dtor_405220();

    virtual void VScreenChanged() override;
    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT void VScreenChanged_4054B0();
    EXPORT BellHammer* Vdtor_4054C0(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_405320();

    static EXPORT void CC OnResLoaded_405210(BellHammer* pThis);

    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    s32 field_E0;
    BellHammerStates field_E4_state;
    s16 field_E6_switch_id;
    s32 field_E8_tlvInfo;
    s32 field_EC_pending_resource_count;
    s32 field_F0_bSpawnElum;
};
ALIVE_ASSERT_SIZEOF(BellHammer, 0xF4);


} // namespace AO
