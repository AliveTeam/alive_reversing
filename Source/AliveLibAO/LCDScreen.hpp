#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Map.hpp"
#include "Font.hpp"
#include "../relive_lib/Primitives.hpp"

struct StringTable;

namespace relive
{
    struct Path_LCDScreen;
}

namespace AO {

void SetLcdMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId);

class LCDScreen final : public ::BaseGameObject
{
public:
    LCDScreen(relive::Path_LCDScreen* pTlv, const Guid& tlvId);
    ~LCDScreen();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;


    Prim_PrimClipper field_10_prim_clippers[2][2] = {};
    FontContext field_50_font_context;
    AliveFont field_60_font;
    PSX_RECT field_98_pal_rect = {};
    char_type* field_A0_message = nullptr;
    const char_type* field_A4_message_cutoff_ptr = nullptr;
    s32 field_A8 = 0;
    char_type field_AC_message_buffer[512] = {};
    s32 field_2AC_message_1_id = 0;
    s32 field_2B0_x_offset = 0;
    s32 field_2B4_character_width = 0;
    Guid field_2B8_tlv_item_info;
    relive::Path_TLV* field_2BC_tlv = {};
    s32 field_2D4 = 0;
    s16 field_2D8_message_rand_min = 0;
    s16 pad_2DA = 0;
    s16 field_2DC_message_rand_max = 0;
    s16 pad_2DE = 0;
};
ALIVE_ASSERT_SIZEOF(LCDScreen, 0x2E0);

} // namespace AO
