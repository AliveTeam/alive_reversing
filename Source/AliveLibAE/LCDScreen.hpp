#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "Path.hpp"
#include "Factory.hpp"

struct Path_LCDScreen final : public Path_TLV
{
    s16 field_10_message_1_id;
    s16 field_12_message_rand_min_id;
    s16 field_14_message_rand_max_id;
    s16 field_16_message_2_id;
    s32 field_18_toggle_message_switch_id;
};

struct StringTable;
void SetLcdMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId);

class LCDScreen final : public BaseGameObject
{
public:
    EXPORT LCDScreen* ctor_460680(Path_LCDScreen* params, TlvItemInfoUnion itemInfo);
    EXPORT void Update_460A00();
    EXPORT void Render_460CB0(PrimHeader** ppOt);
    EXPORT void vSetDead_460F10();
    EXPORT void dtor_460920();
    EXPORT BaseGameObject* vdtor_4608F0(s32 flags);

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    
    virtual void VScreenChanged() override;

public:
    Prim_PrimClipper field_20_prim_clippers[2][2];
    Alive::Font field_60_font;
    PSX_RECT field_98_pal_rect;
    const char_type* field_A0_message;
    const char_type* field_A4_message_cutoff_ptr;
    char_type field_A8_message_buffer[512];
    u16 field_2A8_play_sound_toggle;
    s16 field_2AA_message_1_id;
    s16 field_2AC_x_offset;
    u16 field_2AE_character_width;
    u16 field_2B0_message_2_id;
    u16 field_2B2_toggle_message_switch_id;
    u16 field_2B4_show_random_message;
    u16 field_2B6_message_rand_min_id;
    u16 field_2B8_message_rand_max_id;
    s16 field_2BA_padding;
    TlvItemInfoUnion field_2BC_tlv_item_info;
    Path_TLV field_2C0_tlv;
};
ALIVE_ASSERT_SIZEOF(LCDScreen, 0x2D0);
