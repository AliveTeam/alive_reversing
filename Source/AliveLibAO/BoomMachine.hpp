#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class GrenadeMachineNozzle;

struct Path_BoomMachine final : public Path_TLV
{
    Scale_short field_18_scale;
    enum class NozzleSide : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    NozzleSide field_1A_nozzle_side;
    s16 field_1C_disabled_resources;
    s16 field_1E_number_of_grenades;
};
ALIVE_ASSERT_SIZEOF(Path_BoomMachine, 0x20);

enum class BoomMachineStates : s16
{
    eInactive_0,
    eAlreadyUsed_1,
    eDropGrenadeAnimation_2,
    eDropGrenade_3
};

class BoomMachine final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BoomMachine* ctor_41E420(Path_BoomMachine* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_41E670();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BoomMachine* Vdtor_41E850(s32 flags);

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual Bool32 VIsButtonOn();

    virtual void VHandleButton();

private:
    EXPORT void VScreenChanged_41E7F0();

    EXPORT void VUpdate_41E750();

    EXPORT Bool32 VIsButtonOn_41E840();

    EXPORT void VHandleButton_41E6F0();

public:
    s32 field_D4_padding[4];
    s32 field_E4_tlvInfo;
    s16 field_E8_bIsButtonOn;
    s16 field_EA_padding;
    GrenadeMachineNozzle* field_EC_pNozzle;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0xF0);

} // namespace AO
