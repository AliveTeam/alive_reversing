#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "Primitives.hpp"

START_NS_AO

struct Path_LCDScreen : public Path_TLV
{
    __int16 field_18_message_1_id;
    __int16 field_1A_message_rand_min;
    __int16 field_1C_message_rand_max;
    __int16 field_1E_message_2_id_not_used;
    int field_20_switch_id_not_used;
};
ALIVE_ASSERT_SIZEOF(Path_LCDScreen, 0x24);

class LCDScreen : public BaseGameObject
{
public:
    EXPORT LCDScreen* ctor_433F60(Path_LCDScreen* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_434100();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_434630(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_434620();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4341B0();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_434400(int** ppOt);

    Prim_PrimClipper field_10_prim_clippers[4];
    FontContext field_50_font_context;
    AliveFont field_60_font;
    PSX_RECT field_98_pal_rect;
    char* field_A0_message;
    int field_A4_message_cutoff_ptr;
    int field_A8;
    char field_AC_message_buffer[512];
    int field_2AC_message_1_id;
    int field_2B0_x_offset;
    int field_2B4_character_width;
    int field_2B8_tlv_item_info;
    Path_TLV field_2BC_tlv;
    int field_2D4;
    __int16 field_2D8_message_rand_min;
    __int16 field_2DA;
    int field_2DC_message_rand_max;
};
ALIVE_ASSERT_SIZEOF(LCDScreen, 0x2E0);


ALIVE_VAR_EXTERN(int, dword_508A60);

END_NS_AO

