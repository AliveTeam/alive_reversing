#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "SwitchStates.hpp"

enum class FootSwitchTriggerBy : s16
{
    eAbe_0 = 0,
    eAnyone_1 = 1,
};

struct Path_FootSwitch final : public Path_TLV
{
    s16 mSwitchId;
    Scale_short mScale;
    SwitchOp mAction;
    FootSwitchTriggerBy mTriggeredBy;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FootSwitch, 0x18);

struct FootSwitch_Data final
{
    s32 field_0_frameTableOffset;
    s32 field_4_frameTableOffset;
    s16 field_8_maxH;
    s16 field_A_frameTableOffset;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FootSwitch_Data, 0xC);


class FootSwitch final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    FootSwitch(Path_FootSwitch* pTlv, s32 tlvInfo);
    ~FootSwitch();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    BaseAliveGameObject* WhoIsStoodOnMe();

private:
    s32 field_F4_tlvInfo = 0;
    enum class States : s16
    {
        eWaitForStepOnMe = 0,
        eWaitForGetOffMe = 1,
    };
    States mState = States::eWaitForStepOnMe;
    s16 mSwitchId = 0;
    SwitchOp mAction = SwitchOp::eSetTrue_0;
    FootSwitchTriggerBy mTriggeredBy = FootSwitchTriggerBy::eAbe_0;
    s32 mStoodOnMeId = 0;
    bool mCreateSparks = false;
    bool mFindStander = false;
};
ALIVE_ASSERT_SIZEOF(FootSwitch, 0x108);
