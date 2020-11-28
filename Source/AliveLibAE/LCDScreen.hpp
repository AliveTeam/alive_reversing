#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "Path.hpp"
#include "Factory.hpp"

extern const char * sLCDMessageTable_555768[101];

struct Path_LCDScreen : public Path_TLV
{
    __int16 field_10_message_1_id;
    __int16 field_12_message_rand_min;
    __int16 field_14_message_rand_max;
    __int16 field_16_message_2_id;
    int field_18_swap_message_sets_switch_id;
};

class LCDScreen : public BaseGameObject
{
public:
    EXPORT LCDScreen* ctor_460680(Path_LCDScreen *params, TlvItemInfoUnion itemInfo);
    EXPORT void Update_460A00();
    EXPORT void Render_460CB0(int **ot);
    EXPORT void vsub_460F10();
    EXPORT void dtor_460920();
    EXPORT BaseGameObject* vdtor_4608F0(signed int flags);

    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;

public:
    Prim_PrimClipper field_20_prim_clippers[2][2];
    Alive::Font field_60_font;
    PSX_RECT field_98_pal_rect;
    const char *field_A0_message;
    const char * field_A4_message_cutoff_ptr;
    char field_A8_message_buffer[512];
    WORD field_2A8_play_sound_toggle;
    signed __int16 field_2AA_message_1_id;
    __int16 field_2AC_x_offset;
    WORD field_2AE_character_width;
    unsigned __int16 field_2B0_message_2_id;
    WORD field_2B2_swap_message_sets_switch_id;
    WORD field_2B4;
    WORD field_2B6_message_rand_min;
    WORD field_2B8_message_rand_max;
    __int16 field_2BA_padding;
    TlvItemInfoUnion field_2BC_tlv_item_info;
    Path_TLV field_2C0_tlv;
};
ALIVE_ASSERT_SIZEOF(LCDScreen, 0x2D0);
