#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

class RockSack final : public BaseAliveGameObject
{
public:
    RockSack(relive::Path_RockSack* pTlv, const Guid& tlvId);
    ~RockSack();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    Guid field_118_tlvInfo;
    s16 field_11C_has_been_hit = 0;
    s16 field_11E_rock_amount = 0;
    s16 field_120_can_play_wobble_sound = 0;
    s16 field_122_force_wobble_sound = 0; // Makes sure RockSacks play the sound when you first encounter them. Unused afterwards. -- Nemin (6/7/2020)
    FP field_124_x_vel = {};
    FP field_128_y_vel = {};
};
ALIVE_ASSERT_SIZEOF(RockSack, 0x12C);
