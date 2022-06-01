#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

enum class State : s16
{
    eWaiting_0 = 0,
    eActive_1 = 1,
    eFinished_2 = 2,
};

// TODO: HACK - pass in the scale directly at 100%.
// Either Path_Slig or Path_FlyingSlig with another TLV type value.
// Both of these start with a scale value which is the only thing this object reads.
class SligGetPantsAndWings final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    SligGetPantsAndWings(Path_TLV* pTlv, s32 tlvInfo);
    ~SligGetPantsAndWings();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    State field_F4_state;
    //s16 field_F6_pad;
    s32 field_F8_tlvInfo;
    s32 field_FC_timer;
};
ALIVE_ASSERT_SIZEOF(SligGetPantsAndWings, 0x100);
