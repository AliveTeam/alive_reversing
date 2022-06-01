#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Input.hpp"


struct Path_MineCar final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_max_damage;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MineCar, 0x14);

enum class MineCarStates : s16
{
    eParkedWithoutAbe_0 = 0,
    eParkedWithAbe_1 = 1,
    eMoving_2 = 2,
    eFalling_3 = 3,
};

enum class MineCarDirs : s16
{
    eDown_0 = 0,
    eRight_1 = 1,
    eLeft_2 = 2,
    eUp_3 = 3,
};

struct MineCar_SaveState final
{
    AETypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    FP field_14_sprite_scale;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_xFlip;
    s32 field_24_frame_table;
    s16 field_28_current_motion;
    s16 field_2A_current_anim_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_render;
    s8 field_2F_drawable;
    s8 field_30_unused;
    s8 field_31_padding;
    s16 field_32_unused;
    s16 field_34_unused;
    s16 field_36_unused;
    s32 field_38_frame_table_offset2;
    FP field_3C_health;
    s16 field_40_current_motion;
    s16 field_42_next_motion;
    s16 field_44_last_line_ypos;
    s16 field_46_collision_line_type;
    s16 field_48_padding;
    s16 field_4A_padding;
    s32 field_4C_tlvInfo;
    MineCarStates field_50_state;
    MineCarDirs field_52_turn_direction;
    s16 field_54_unused;
    s16 field_56_unused;
    s16 field_58_falling_counter;
    s16 field_5A_bAbeInCar;
    s32 field_5C_frame_mod_16;
    s16 field_60_spawned_path;
    s16 field_62_spanwed_camera;
    s16 field_64_throw_item_key1;
    s16 field_66_continue_move_input;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MineCar_SaveState, 0x68);

class MineCar final : public BaseAliveGameObject
{
public:
    MineCar(Path_MineCar* pTlv, s32 tlvInfo, s32 a4, s32 a5, s32 a6);
    ~MineCar();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    void LoadAnimation_46BF80(Animation* pAnim);
    Bool32 CheckRoofCollision_46F6B0(FP hitX, FP hitY);
    Bool32 CheckFloorCollision_46F730(FP hitX, FP hitY);
    void Stop_46E570();
    void Move_46E640(u16 frameTabeOffset, FP velX, FP velY, InputCommands::Enum input, MineCarDirs dir, s8 bChangeDirection);
    s16 IsBlocked_46F4A0(MineCarDirs a2, s32 a3);
    s16 FollowDirection_46EA00();
    void RunThingsOver_46F380();

    void State_0_ParkedWithoutAbe();
    void State_1_ParkedWithAbe();
    void State_2_Moving();
    void State_3_Falling();

    //State_1 helper funcs
    void HandleUpDown();

    using mineCarFPFunc = Bool32 (MineCar::*)(FP, FP);
    bool HandleState1Move(const mineCarFPFunc func, const FP mineCarFPFuncArg1, const FP mineCarFPFuncArg2, const FP mineCarFPFuncArg3, u16 frameTableOffset, MineCarDirs mineCarDir, const s8 changeDir, FP rayCast1, FP rayCast2, FP rayCast3, FP rayCast4, const s32 ModelMask1, const s32 ModelMask2, FP moveX, FP moveY, InputCommands::Enum key, bool isVertical, bool verticalFlipXCond);

private:
    s32 field_118_tlvInfo;

public:
    MineCarStates field_11C_state;

private:
    Scale_short field_11E_scale;
    s16 field_120_max_damage;
    s16 field_122_padding;
    Animation field_124_anim;

public:
    MineCarDirs field_1BC_turn_direction;

private:
    s16 field_1BE_unused;
    s16 field_1C0_unused;
    s16 field_1C2_falling_counter;
    s16 field_1C4_velx_index;
    s16 field_1C6_padding;
    s32 field_1C8_frame_mod_16;
    s16 field_1CC_spawned_path;
    s16 field_1CE_spawned_camera;
    s32 field_1D0_sound_channels_mask;
    s16 field_1D4_previous_input;      //TODO change to InputCommands::Enum type when we can drop sizeof assert
    s16 field_1D6_continue_move_input; //TODO change to InputCommands::Enum type when we can drop sizeof assert
};
ALIVE_ASSERT_SIZEOF(MineCar, 0x1D8);
