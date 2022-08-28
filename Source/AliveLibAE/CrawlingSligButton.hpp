#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "SwitchStates.hpp"

enum class SwitchOp : s16;

enum class CrawlingSligButtonSounds : s16
{
    None_0 = 0,
    SackHit_1 = 1,
    FallingItemPresence2_2 = 2,
    SecurityOrb_3 = 3,
    SackHit_4 = 4,
    Bullet1_5 = 5,
    AbeGenericMovement_6 = 6,
};

struct Path_CrawlingSligButton final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    SwitchOp mAction;
    CrawlingSligButtonSounds mOnSound;
    CrawlingSligButtonSounds mOffSound;
    s16 mSoundDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_CrawlingSligButton, 0x1C);

class CrawlingSligButton final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    CrawlingSligButton(relive::Path_CrawlingSligButton* pTlv, const TLVUniqueId& tlvId);
    ~CrawlingSligButton();

    virtual void VUpdate() override;
    void UseButton();

private:
    TLVUniqueId field_F4_tlvInfo;
    s16 field_F8_switch_id = 0;
    relive::reliveSwitchOp field_FA_action = relive::reliveSwitchOp::eSetTrue;
    relive::Path_CrawlingSligButton::ButtonSounds field_FC_on_sound = relive::Path_CrawlingSligButton::ButtonSounds::None;
    relive::Path_CrawlingSligButton::ButtonSounds field_FE_off_sound = relive::Path_CrawlingSligButton::ButtonSounds::None;
    s16 field_100_sound_direction = 0;
    s16 field_102_in_use = 0;
};
ALIVE_ASSERT_SIZEOF(CrawlingSligButton, 0x104);
