#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

enum class SwitchOp : s16;

class CrawlingSligButton final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    CrawlingSligButton(relive::Path_CrawlingSligButton* pTlv, const Guid& tlvId);
    ~CrawlingSligButton();

    void LoadAnimations();

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
