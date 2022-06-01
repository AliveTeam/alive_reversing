#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

enum class LevelIds : s16;

enum class FartStates : s16
{
    eIdle_0 = 0,
    eFlying_1 = 1,
    eDechanting_2 = 2
};

struct EvilFart_State final
{
    AETypes field_0_type;
    s16 field_2_r;
    s16 field_4_g;
    s16 field_6_b;
    s16 field_8_path_number;
    LevelIds field_A_lvl_number;
    FP field_C_xpos;
    FP field_10_ypos;
    FP field_14_velx;
    FP field_18_vely;
    FP field_1C_sprite_scale;
    s16 field_20_anim_cur_frame;
    s16 field_22_frame_change_counter;
    s8 field_24_bAnimRender;
    s8 field_25_bDrawable;
    LevelIds field_26_level;
    s16 field_28_path;
    s16 field_2A_camera;

    enum Flags_2C
    {
        eBit1_bControlled = 0x1,
        eBit2_bBlowUp = 0x2,
    };
    BitField16<Flags_2C> field_2C;

    s16 field_2E_alive_timer;
    FartStates field_30_state;
    s16 field_32_padding;
    s32 field_34_timer;
    s32 field_38_timer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(EvilFart_State, 60);

class EvilFart final : public BaseAliveGameObject
{
public:
    EvilFart();

    virtual void VUpdate() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VPossessed() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    void InputControlFart_423BB0();
    void ResetFartColour();
    void CalculateFartColour();
    void BlowUp();

private:
    s16 field_118_bBlowUp;
    s16 field_11A_bPossesed;
    s16 field_11C_alive_timer;
    s16 field_11E_path;
    LevelIds field_120_level;
    s16 field_122_camera;
    FartStates field_124_state;
    s16 field_126_pad;
    s32 field_128_timer;
    s32 field_12C_back_to_abe_timer;
    s32 field_130_sound_channels;
};
ALIVE_ASSERT_SIZEOF(EvilFart, 0x134);
