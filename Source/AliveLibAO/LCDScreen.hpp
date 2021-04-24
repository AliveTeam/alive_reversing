#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "Primitives.hpp"

namespace AO {

struct Path_LCDScreen : public Path_TLV
{
    s16 field_18_message_1_id;
    s16 field_1A_message_rand_min;
    s16 field_1C_message_rand_max;
    s16 field_1E_message_2_id_not_used;
};
ALIVE_ASSERT_SIZEOF(Path_LCDScreen, 0x20);

class LCDScreen : public BaseGameObject
{
public:
    EXPORT LCDScreen* ctor_433F60(Path_LCDScreen* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_434100();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_434630(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_434620();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4341B0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_434400(PrimHeader** ppOt);

    Prim_PrimClipper field_10_prim_clippers[2][2];
    FontContext field_50_font_context;
    AliveFont field_60_font;
    PSX_RECT field_98_pal_rect;
    char* field_A0_message;
    const char* field_A4_message_cutoff_ptr;
    s32 field_A8;
    char field_AC_message_buffer[512];
    s32 field_2AC_message_1_id;
    s32 field_2B0_x_offset;
    s32 field_2B4_character_width;
    s32 field_2B8_tlv_item_info;
    Path_TLV field_2BC_tlv;
    s32 field_2D4;
    short field_2D8_message_rand_min;
    short pad_2DA;
    short field_2DC_message_rand_max;
    short pad_2DE;
};
ALIVE_ASSERT_SIZEOF(LCDScreen, 0x2E0);

}

