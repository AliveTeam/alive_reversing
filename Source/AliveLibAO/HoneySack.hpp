#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_HoneySack final : public Path_TLV
{
    s16 mChaseTime;
    Scale_short mScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_HoneySack, 0x1C);

class BeeSwarm;

class HoneySack final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HoneySack(Path_HoneySack* pTlv, s32 tlvInfo);
    ~HoneySack();

    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

    s32 field_E4_tlvInfo = 0;
    enum class State : s16
    {
        eDripHoney_0 = 0,
        eSetFallAnimation_1 = 1,
        eFallOnGround_2 = 2,
        eUpdateHoneySackOnGround_3 = 3
    };
    State field_E8_state = State::eDripHoney_0;
    s16 field_EA_bHit_ground = 0;
    s32 field_EC_timer = 0;
    BeeSwarm* field_F0_pBee = nullptr;
    FP field_F4_drip_target_x = {};
    FP field_F8_drip_target_y = {};
    FP field_FC_ypos2 = {};
    u16 mChaseTime = 0;
};
ALIVE_ASSERT_SIZEOF(HoneySack, 0x104);


} // namespace AO
