#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

enum class State : s16
{
    eWaiting = 0,
    eActive = 1,
    eFinished = 2,
};

// TODO: HACK - pass in the scale directly at 100%.
// Either Path_Slig or Path_FlyingSlig with another TLV type value.
// Both of these start with a scale value which is the only thing this object reads.
class SligGetPantsAndWings final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    SligGetPantsAndWings(relive::Path_TLV* pTlv, const Guid& tlvId);
    ~SligGetPantsAndWings();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    State mState = State::eWaiting;
    Guid mTlvInfo;
    s32 mTransformTimer = 0;
};
ALIVE_ASSERT_SIZEOF(SligGetPantsAndWings, 0x100);
