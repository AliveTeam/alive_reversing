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
    CrawlingSligButton(Path_CrawlingSligButton* pTlv, s32 tlvInfo);
    ~CrawlingSligButton();

    virtual void VUpdate() override;
    void UseButton();

private:
    s32 field_F4_tlvInfo = 0;
    s16 field_F8_switch_id = 0;
    SwitchOp field_FA_action = SwitchOp::eSetTrue_0;
    CrawlingSligButtonSounds field_FC_on_sound = CrawlingSligButtonSounds::None_0;
    CrawlingSligButtonSounds field_FE_off_sound = CrawlingSligButtonSounds::None_0;
    s16 field_100_sound_direction = 0;
    s16 field_102_in_use = 0;
};
ALIVE_ASSERT_SIZEOF(CrawlingSligButton, 0x104);
