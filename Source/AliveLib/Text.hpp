#pragma once

#include "BaseGameObject.hpp"
#include "Font.hpp"

class Text : public BaseGameObject
{
public:
    virtual void VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    EXPORT Text* ctor_46ADA0(const char* pMessage, int renderCount, int bShadow);
    EXPORT void vdtor_46AED0(signed int flags);
    EXPORT void dtor_46AF00();
    EXPORT void SetYPos_46AFB0(int /*not_used*/, __int16 ypos);
    EXPORT void Render_46AFD0(int** pOt);
private:
    Font field_20_font;
    __int16 field_58_k0;
    __int16 field_5A_k4;
    __int16 field_5C_xpos;
    __int16 field_5E_ypos;
    __int16 field_60_bShadow;
    //__int16 field_62_pad;
    int field_64_render_count;
    char field_68_txt_buffer[60];
};
ALIVE_ASSERT_SIZEOF(Text, 0xA4);

EXPORT char CC Display_Full_Screen_Message_Blocking_465820(int /*not_used*/, int messageType);
