#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "SwitchStates.hpp"

enum class SwitchOp : s16;

class CrawlingSligButton final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    CrawlingSligButton(relive::Path_CrawlingSligButton* pTlv, const Guid& tlvId);
    ~CrawlingSligButton();

    virtual void VUpdate() override;
    void UseButton();

private:
    Guid field_F4_tlvInfo;
    s16 field_F8_switch_id = 0;
    relive::reliveSwitchOp field_FA_action = relive::reliveSwitchOp::eSetTrue;
    relive::Path_CrawlingSligButton::ButtonSounds field_FC_on_sound = relive::Path_CrawlingSligButton::ButtonSounds::None;
    relive::Path_CrawlingSligButton::ButtonSounds field_FE_off_sound = relive::Path_CrawlingSligButton::ButtonSounds::None;
    s16 field_100_sound_direction = 0;
    s16 field_102_in_use = 0;
};
ALIVE_ASSERT_SIZEOF(CrawlingSligButton, 0x104);
