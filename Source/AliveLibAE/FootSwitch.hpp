#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "SwitchStates.hpp"

class BaseAliveGameObject;

struct FootSwitch_Data final
{
    s32 field_0_frameTableOffset;
    s32 field_4_frameTableOffset;
    s16 field_8_maxH;
    s16 field_A_frameTableOffset;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FootSwitch_Data, 0xC);


class FootSwitch final : public BaseAnimatedWithPhysicsGameObject
{
public:
    FootSwitch(relive::Path_FootSwitch* pTlv, const Guid& tlvId);
    ~FootSwitch();
    
    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    BaseAliveGameObject* WhoIsStoodOnMe();

private:
    Guid field_F4_tlvInfo;
    enum class States : s16
    {
        eWaitForStepOnMe = 0,
        eWaitForGetOffMe = 1,
    };
    States mState = States::eWaitForStepOnMe;
    s16 mSwitchId = 0;
    relive::reliveSwitchOp mAction = relive::reliveSwitchOp::eSetTrue;
    relive::Path_FootSwitch::FootSwitchTriggerBy mTriggeredBy = relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe;
    Guid mStoodOnMeId;
    bool mCreateSparks = false;
    bool mFindStander = false;
};
ALIVE_ASSERT_SIZEOF(FootSwitch, 0x108);
