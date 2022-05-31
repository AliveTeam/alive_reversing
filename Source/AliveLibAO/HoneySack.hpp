#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_HoneySack final : public Path_TLV
{
    s16 field_18_chase_ticks;
    Scale_short field_1A_scale;
};
ALIVE_ASSERT_SIZEOF(Path_HoneySack, 0x1C);

class BeeSwarm;

class HoneySack final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HoneySack(Path_HoneySack* pTlv, s32 tlvInfo);
    ~HoneySack();

    virtual void VScreenChanged() override;

    void VScreenChanged_42C390();

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    void VOnThrowableHit_42C370(BaseGameObject* pFrom);

    virtual void VUpdate() override;

    void VUpdate_42BFE0();

    s32 field_D4_padding[4];
    s32 field_E4_tlvInfo;
    enum class State : s16
    {
        eDripHoney_0 = 0,
        eSetFallAnimation_1 = 1,
        eFallOnGround_2 = 2,
        eUpdateHoneySackOnGround_3 = 3
    };
    State field_E8_state;
    s16 field_EA_bHit_ground;
    s32 field_EC_timer;
    BeeSwarm* field_F0_pBee;
    FP field_F4_drip_target_x;
    FP field_F8_drip_target_y;
    FP field_FC_ypos2;
    u16 field_100_chase_ticks;
    s16 field_102_padding;
};
ALIVE_ASSERT_SIZEOF(HoneySack, 0x104);


} // namespace AO
