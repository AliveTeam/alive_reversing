#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

namespace AO {

class FlintLockFire final : public BaseAnimatedWithPhysicsGameObject
{
public:
    FlintLockFire(relive::Path_FlintLockFire* pTlv, const Guid& tlvId);
    ~FlintLockFire();

    void LoadAnimations();

    virtual void VStopAudio() override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    enum class States : s16
    {
        eDisabled_0 = 0,
        eActivating_1 = 1,
        eActivated_2 = 2
    };
    States mState = States::eDisabled_0;
    s16 mSwitchId = 0;
    Guid mTlvId;
    s32 mFireSound = 0;
    Animation mGourdAnim;
    Animation mFire1Anim;
    Animation mFire2Anim;
};

} // namespace AO
