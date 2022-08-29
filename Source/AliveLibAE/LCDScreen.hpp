#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Font.hpp"
#include "Path.hpp"
#include "Factory.hpp"

struct Path_LCDScreen final : public Path_TLV
{
    s16 mMessageId1;
    s16 mMessageRandMinId;
    s16 mMessageRandMaxId;
    s16 mMessageId2;
    s32 mToggleMessageSwitchId;
};

struct StringTable;
void SetLcdMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId);

class LCDScreen final : public BaseGameObject
{
public:
    LCDScreen(relive::Path_LCDScreen* params, const Guid& tlvId);
    ~LCDScreen();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

public:
    Prim_PrimClipper field_20_prim_clippers[2][2] = {};
    Alive::Font field_60_font = {};
    PSX_RECT field_98_pal_rect = {};
    const char_type* field_A0_message = nullptr;
    const char_type* field_A4_message_cutoff_ptr = nullptr;
    char_type field_A8_message_buffer[512] = {};
    u16 field_2A8_play_sound_toggle = 0;
    s16 field_2AA_message_1_id = 0;
    s16 field_2AC_x_offset = 0;
    u16 field_2AE_character_width = 0;
    u16 field_2B0_message_2_id = 0;
    u16 field_2B2_toggle_message_switch_id = 0;
    u16 field_2B4_show_random_message = 0;
    u16 field_2B6_message_rand_min_id = 0;
    u16 field_2B8_message_rand_max_id = 0;
    Guid field_2BC_tlv_item_info;
    relive::Path_TLV field_2C0_tlv = {};
};
ALIVE_ASSERT_SIZEOF(LCDScreen, 0x2D0);
