#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

enum class LevelIds : __int16;

enum class FartStates : __int16
{
    e0_IDLE = 0,
    e1_FLYING = 1,
    e2_DECHANTING = 2,
};

struct EvilFart_State
{
    Types field_0_type;
    __int16 field_2_r;
    __int16 field_4_g;
    __int16 field_6_b;
    __int16 field_8_path_number;
    LevelIds field_A_lvl_number;
    FP field_C_xpos;
    FP field_10_ypos;
    FP field_14_velx;
    FP field_18_vely;
    FP field_1C_sprite_scale;
    __int16 field_20_anim_cur_frame;
    __int16 field_22_frame_change_counter;
    char field_24_bAnimRender;
    char field_25_bDrawable;
    LevelIds field_26_level;
    __int16 field_28_path;
    __int16 field_2A_camera;

    enum Flags_2C
    {
        eBit1_bControlled = 0x1,
        eBit2_bBlowUp = 0x2,
    };
    BitField16<Flags_2C> field_2C;

    __int16 field_2E_alive_timer;
    FartStates field_30_state;
    __int16 field_32;
    int field_34_timer;
    int field_38_timer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(EvilFart_State, 60);

class EvilFart : public BaseAliveGameObject
{
public:
    EXPORT EvilFart* ctor_422E30();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;
    virtual void VPossessed_408F70() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    EXPORT static int CC CreateFromSaveState_4281C0(const BYTE* pBuffer);

private:
    EXPORT int vGetSaveState_4283F0(EvilFart_State* pState);

    EXPORT void InputControlFart_423BB0();
    EXPORT void vOnPossesed_423DA0();

    void ResetFartColour();

    EXPORT __int16 VTakeDamage_423B70(BaseGameObject* pFrom);
    EXPORT void vUpdate_423100();
    EXPORT void vUpdate_REAL_423100();

    void CalculateFartColour();
    void BlowUp();

    EXPORT void dtor_423D80();
    EXPORT EvilFart* vdtor_4230D0(signed int flags);

private:
    __int16 field_118_bBlowUp;
    __int16 field_11A_bPossesed;
    __int16 field_11C_alive_timer;
    __int16 field_11E_path;
    LevelIds field_120_level;
    __int16 field_122_camera;
    FartStates field_124_state;
    __int16 field_126_pad;
    int field_128_timer;
    int field_12C_back_to_abe_timer;
    int field_130_sound_channels;
};
ALIVE_ASSERT_SIZEOF(EvilFart, 0x134);
