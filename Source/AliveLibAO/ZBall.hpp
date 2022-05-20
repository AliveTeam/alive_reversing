#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_ZBall final : public Path_TLV
{
    enum class StartPos : s16
    {
        eCenter_0 = 0,
        eOut_1 = 1,
        eIn_2 = 2
    };

    enum class Speed : s16
    {
        eNormal_0 = 0,
        eFast_1 = 1,
        eSlow_2 = 2
    };

    StartPos field_18_start_pos;
    Scale_short field_1A_scale;
    Speed field_1C_speed;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ZBall, 0x20);

class ZBall final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ZBall* ctor_478590(Path_ZBall* pTlv, s32 tlvInfo);

    
    virtual void VUpdate() override;

    EXPORT ZBall* Vdtor_478860(s32 flags);
    EXPORT BaseGameObject* dtor_478710();
    EXPORT void VUpdate_478720();

    s32 field_D4_padding[4];

    s32 field_E4_tlvInfo;
    s16 field_E8_bFrameAbove12;
    s16 field_EA_sound_pitch;
};
ALIVE_ASSERT_SIZEOF(ZBall, 0xEC);

} // namespace AO
