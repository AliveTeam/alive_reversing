#pragma once

#include "BaseGameObject.hpp"
#include "Font.hpp"

enum class MessageType : s32
{
    eLongTitle_0 = 0,
    eSkipMovie_1 = 1,
    eSkipDemo_2 = 2,
    eShortTitle_3 = 3
};

class Text final : public BaseGameObject
{
public:
    Text(const char_type* pMessage, s32 renderCount, s32 bShadow);
    ~Text();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    void SetYPos(s32 /*not_used*/, s16 ypos);

private:
    Alive::Font field_20_font;
    s16 field_58_k0_unused;
    s16 field_5A_k4_unused;
    s16 field_5C_xpos;
    s16 field_5E_ypos;
    s16 field_60_bShadow;
    s32 field_64_render_count;
    char_type field_68_txt_buffer[60];
};
ALIVE_ASSERT_SIZEOF(Text, 0xA4);

s8 Display_Full_Screen_Message_Blocking(s32 /*not_used*/, MessageType messageType);
