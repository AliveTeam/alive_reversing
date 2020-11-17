#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

struct Path_HoneySack : public Path_TLV
{
    __int16 field_18_chase_ticks;
    __int16 field_1A_scale;
};
ALIVE_ASSERT_SIZEOF(Path_HoneySack, 0x1C);

class BeeSwarm;

class HoneySack : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT HoneySack* ctor_42BD10(Path_HoneySack* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    
    BaseGameObject* dtor_42BF20();

    EXPORT HoneySack* Vdtor_42C3B0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_42C390();

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    EXPORT void VOnThrowableHit_42C370(BaseGameObject* pFrom);

    virtual void VUpdate() override;

    EXPORT void VUpdate_42BFE0();

    int field_D4[4];
    int field_E4_tlvInfo;
    __int16 field_E8_state;
    __int16 field_EA;
    int field_EC_timer;
    BeeSwarm* field_F0_pBee;
    FP field_F4_drip_target_x;
    FP field_F8_drip_target_y;
    FP field_FC_ypos2;
    unsigned __int16 field_100_chase_ticks;
    __int16 field_102;
};
ALIVE_ASSERT_SIZEOF(HoneySack, 0x104);


}

