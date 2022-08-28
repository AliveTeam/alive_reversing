#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_BoomMachine final : public Path_TLV
{
    Scale_short mScale;
    enum class NozzleSide : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    NozzleSide mNozzleSide;
    s16 mDisabledResources;
    s16 mGrenadeAmount;
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
    BoomMachine(relive::Path_BoomMachine* pTlv, const TLVUniqueId& tlvId);
    ~BoomMachine();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual bool VIsButtonOn();
    virtual void VHandleButton();

private:
    TLVUniqueId field_F4_tlvInfo;
    s32 field_F8_nozzle_id = 0;
    s32 field_FC_bIsButtonOn = 0;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0x100);
