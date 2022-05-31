#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_BoomMachine final : public Path_TLV
{
    Scale_short field_10_scale;
    enum class NozzleSide : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    NozzleSide field_12_nozzle_side;
    s16 field_14_disabled_resources;
    s16 field_16_number_of_grenades;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoomMachine, 0x18);

enum class BoomMachineStates : s16
{
    eInactive_0,
    eAlreadyUsed_1,
    eDropGrenadeAnimation_2,
    eDropGrenade_3
};

class BoomMachine final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    BoomMachine(Path_BoomMachine* pTlv, s32 tlvInfo);
    ~BoomMachine();

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual Bool32 VIsButtonOn_445DF0();

    virtual void VHandleButton_445F00();

private:

    Bool32 vIsButtonOn_445DF0();

    void vHandleButton_445F00();

private:
    s32 field_F4_tlvInfo;
    s32 field_F8_nozzle_id;
    s32 field_FC_bIsButtonOn;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0x100);
