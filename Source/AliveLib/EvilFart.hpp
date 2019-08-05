#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

enum class LevelIds : __int16;

class EvilFart : public BaseAliveGameObject
{
public:
    EXPORT EvilFart* ctor_422E30();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;
    virtual void VPossessed_408F70() override;

private:
    EXPORT void InputControlFart_423BB0();
    EXPORT void vOnPossesed_423DA0();
    EXPORT __int16 VTakeDamage_423B70(BaseGameObject* pFrom);
    EXPORT void vUpdate_423100();
    EXPORT void dtor_423D80();
    EXPORT EvilFart* vdtor_4230D0(signed int flags);

private:
    __int16 field_118_bBlowUp;
    __int16 field_11A_bPossesed;
    __int16 field_11C_alive_timer;
    __int16 field_11E_path;
    LevelIds field_120_level;
    __int16 field_122_camera;
    __int16 field_124_state;
    __int16 field_126_pad;
    int field_128_timer;
    int field_12C_back_to_abe_timer;
    int field_130_sound_channels;
};
ALIVE_ASSERT_SIZEOF(EvilFart, 0x134);
