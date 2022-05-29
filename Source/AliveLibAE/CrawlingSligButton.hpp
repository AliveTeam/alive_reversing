#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

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
    Scale_short field_10_scale;
    s16 field_12_switch_id;
    SwitchOp field_14_action;
    CrawlingSligButtonSounds field_16_on_sound;
    CrawlingSligButtonSounds field_18_off_sound;
    s16 field_1A_sound_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_CrawlingSligButton, 0x1C);

class CrawlingSligButton final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    CrawlingSligButton(Path_CrawlingSligButton* pTlv, s32 tlvInfo);
    ~CrawlingSligButton();

    virtual void VUpdate() override;
    EXPORT void UseButton_414C60();

private:
    EXPORT void vUpdate_414B20();

private:
    s32 field_F4_tlvInfo;
    s16 field_F8_switch_id;
    SwitchOp field_FA_action;
    CrawlingSligButtonSounds field_FC_on_sound;
    CrawlingSligButtonSounds field_FE_off_sound;
    s16 field_100_sound_direction;
    s16 field_102_in_use;
};
ALIVE_ASSERT_SIZEOF(CrawlingSligButton, 0x104);
