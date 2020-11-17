#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_ZBall : public Path_TLV
{
    enum class StartPos : __int16
    {
        eCenter_0 = 0,
        eOut_1 = 1,
        eIn_2 = 2
    };

    enum class Speed : __int16
    {
        eNormal_0 = 0,
        eFast_1 = 1,
        eSlow_2 = 2
    };

    StartPos field_18_start_pos;
    __int16 field_1A_scale;
    Speed field_1C_speed;
    __int16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ZBall, 0x20);

class ZBall : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ZBall* ctor_478590(Path_ZBall* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;

    EXPORT ZBall* Vdtor_478860(signed int flags);
    EXPORT BaseGameObject* dtor_478710();
    EXPORT void VUpdate_478720();

    int field_D4[4];

    int field_E4_tlvInfo;
    __int16 field_E8_bFrameAbove12;
    __int16 field_EA_sound_pitch;
};
ALIVE_ASSERT_SIZEOF(ZBall, 0xEC);

}
