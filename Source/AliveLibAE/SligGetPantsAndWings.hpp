#pragma once

#include "FunctionFwd.hpp"
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
class SligGetPantsAndWings : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT SligGetPantsAndWings* ctor_465BF0(Path_TLV* pTlv, int tlvInfo);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
private:
    EXPORT void vUpdate_465DD0();
    EXPORT SligGetPantsAndWings* vdtor_465D10(s32 flags);
    EXPORT void vScreenChanged_465EE0();
    EXPORT void dtor_465D40();
private:
    State field_F4_state;
    //s16 field_F6_pad;
    int field_F8_tlvInfo;
    int field_FC_timer;
};
ALIVE_ASSERT_SIZEOF(SligGetPantsAndWings, 0x100);
