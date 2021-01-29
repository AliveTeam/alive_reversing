#pragma once

#include "BaseGameObject.hpp"
#include "Font.hpp"

enum class MessageType : int
{
    eLongTitle_0 = 0,
    eSkipMovie_1 = 1,
    eSkipDemo_2 = 2,
    eShortTitle_3 = 3
};

class Text : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    EXPORT Text* ctor_46ADA0(const char* pMessage, int renderCount, int bShadow);
    EXPORT BaseGameObject* vdtor_46AED0(signed int flags);
    EXPORT void dtor_46AF00();
    EXPORT void SetYPos_46AFB0(int /*not_used*/, __int16 ypos);
    EXPORT void Render_46AFD0(PrimHeader** ppOt);
private:
    Alive::Font field_20_font;
    __int16 field_58_k0_unused;
    __int16 field_5A_k4_unused;
    __int16 field_5C_xpos;
    __int16 field_5E_ypos;
    __int16 field_60_bShadow;
    int field_64_render_count;
    char field_68_txt_buffer[60];
};
ALIVE_ASSERT_SIZEOF(Text, 0xA4);

EXPORT char CC Display_Full_Screen_Message_Blocking_465820(int /*not_used*/, MessageType messageType);
