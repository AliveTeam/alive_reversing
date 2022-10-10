#pragma once

#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

class RockSack final : public BaseAliveGameObject
{
public:
    RockSack(relive::Path_RockSack* pTlv, const Guid& tlvId);
    ~RockSack();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    Guid field_10C_tlvInfo;
    s16 field_110_has_been_hit = 0;
    s16 field_112_rock_amount = 0;
    s16 field_114_can_play_wobble_sound = 0;
    s16 field_116_force_wobble_sound = 0;
    FP field_118_x_vel = {};
    FP field_11C_y_vel = {};
};
}
