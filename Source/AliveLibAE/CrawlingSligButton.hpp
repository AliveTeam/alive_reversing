#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
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
    Guid mTlvId;
    s16 mSwitchId = 0;
    relive::reliveSwitchOp mAction = relive::reliveSwitchOp::eSetTrue;
    relive::Path_CrawlingSligButton::ButtonSounds mOnSound = relive::Path_CrawlingSligButton::ButtonSounds::None;
    relive::Path_CrawlingSligButton::ButtonSounds mOffSound = relive::Path_CrawlingSligButton::ButtonSounds::None;
    s16 mSoundDirection = 0;
    bool mInUse = false;
};
