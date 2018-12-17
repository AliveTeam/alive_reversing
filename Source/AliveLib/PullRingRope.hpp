#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

class PullRingRope : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual __int16 VPull_49BBD0(BaseGameObject* a2);
    virtual BOOL Vsub_49BC90();
    virtual void Vsub_49B610();
private:
    EXPORT PullRingRope* vdtor_49B630(signed int flags);
    EXPORT void vUpdate_49B720();
    EXPORT void vScreenChanged_49BCB0();
    EXPORT __int16 vPull_49BBD0(BaseGameObject* a2);
    EXPORT BOOL vsub_49BC90();
    EXPORT void vsub_49B610();
private:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    __int16 field_100;
    __int16 field_102;
    __int16 field_104;
    __int16 field_106;
    __int16 field_108;
    __int16 field_10A;
    int field_10C;
    int field_110;
};
ALIVE_ASSERT_SIZEOF(PullRingRope, 0x114);
