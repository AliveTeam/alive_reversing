#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"
#include "Input.hpp"


struct Path_MineCar : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_max_damage;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MineCar, 0x14);

enum class MineCarStates : __int16
{
    eState_0_ParkedWithoutAbe = 0,
    eState_1_ParkedWithAbe = 1,
    eState_2_Moving = 2,
    eState_3_Falling = 3,
};

enum class MineCarDirs : __int16
{
    eDown_0 = 0,
    eRight_1 = 1,
    eLeft_2 = 2,
    eUp_3 = 3,
};

struct MineCar_SaveState
{
    Types field_0_type;
    __int16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    FP field_14_sprite_scale;
    __int16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    __int16 field_1C_r;
    __int16 field_1E_g;
    __int16 field_20_b;
    __int16 field_22_xFlip;
    int field_24_frame_table;
    __int16 field_28_current_motion;
    __int16 field_2A_current_anim_frame;
    __int16 field_2C_frame_change_counter;
    char field_2E_render;
    char field_2F_drawable;
    char field_30; // TODO: Remove because it's the same as field_2E_render.
    char field_31_padding;
    __int16 field_32; // TODO: Remove because it's the same as field_22_xFlip.
    __int16 field_34; // TODO: Remove because it's the same as field_2A_current_anim_frame.
    __int16 field_36; // TODO: Remove because it's the same as field_2C_frame_change_counter.
    int field_38_frame_table_offset2;
    FP field_3C_health;
    __int16 field_40_current_motion;
    __int16 field_42_next_motion;
    __int16 field_44_last_line_ypos;
    __int16 field_46_collision_line_type;
    __int16 field_48_padding;
    __int16 field_4A_padding;
    int field_4C_tlvInfo;
    MineCarStates field_50_state;
    MineCarDirs field_52_turn_direction;
    __int16 field_54_unused; // TODO: These two save and restore two other variables, that are unused, field_1BE and field_1C0. -- Nemin (5/7/2020)
    __int16 field_56_unused;
    __int16 field_58_falling_counter;
    __int16 field_5A_bAbeInCar;
    int field_5C_frame_mod_16;
    __int16 field_60_spawned_path;
    __int16 field_62_spanwed_camera;
    __int16 field_64_throw_item_key1;
    __int16 field_66_continue_move_input;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MineCar_SaveState, 0x68);

class MineCar : public BaseAliveGameObject
{
public:
    EXPORT MineCar* ctor_46BC80(Path_MineCar* pTlv, int tlvInfo, int a4, int a5, int a6);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    EXPORT static int CC CreateFromSaveState_467740(const BYTE* pBuffer);

private:
    EXPORT void LoadAnimation_46BF80(Animation* pAnim);
    EXPORT void vStopAudio_46F9C0();
    EXPORT void vScreenChanged_46F800();
    EXPORT MineCar* vdtor_46BF50(signed int flags);
    EXPORT void dtor_46F2A0();
    EXPORT BOOL CheckRoofCollision_46F6B0(FP hitX, FP hitY);
    EXPORT BOOL CheckFloorCollision_46F730(FP hitX, FP hitY);
    EXPORT void vUpdate_46C010();
    EXPORT void vUpdate_REAL_46C010();
    EXPORT void vRender_46E760(PrimHeader** ppOt);
    EXPORT void Stop_46E570();
    EXPORT void Move_46E640(unsigned __int16 frameTabeOffset, FP velX, FP velY, InputCommands input, MineCarDirs dir, char bChangeDirection);
    EXPORT __int16 IsBlocked_46F4A0(__int16 a2, int a3);
    EXPORT __int16 FollowDirection_46EA00();
    EXPORT void RunThingsOver_46F380();
    EXPORT __int16 vTakeDamage_46F7D0(BaseGameObject* pFrom);

    EXPORT int vGetSaveState_467E10(MineCar_SaveState* pState);

    void State_0_ParkedWithoutAbe();
    void State_1_ParkedWithAbe();
    void State_2_Moving();
    void State_3_Falling();

    //State_1 helper funcs
    void HandleUpDown();

    using mineCarFPFunc = BOOL (MineCar::*)(FP, FP);
    bool HandleState1Move(const mineCarFPFunc func, const FP mineCarFPFuncArg1, const FP mineCarFPFuncArg2, const FP mineCarFPFuncArg3, unsigned short moveArgument1, MineCarDirs moveArgument2, const char changeDir, FP rayCast1, FP rayCast2, FP rayCast3, FP rayCast4, const int ModelMask1, const int ModelMask2, FP moveX, FP moveY, InputCommands key, bool isVertical, bool verticalFlipXCond);

private:
    int field_118_tlvInfo;
public:
    MineCarStates field_11C_state;
private:
    __int16 field_11E_scale;
    __int16 field_120_max_damage;
    __int16 field_122_padding;
    Animation field_124_anim;
public:
    MineCarDirs field_1BC_turn_direction;
private:
    __int16 field_1BE_unused;
    __int16 field_1C0_unused;
    __int16 field_1C2_falling_counter;
    __int16 field_1C4_velx_index;
    __int16 field_1C6_padding;
    int field_1C8_frame_mod_16;
    __int16 field_1CC_spawned_path;
    __int16 field_1CE_spawned_camera;
    int field_1D0_sound_channels_mask;
    __int16 field_1D4_previous_input; //TODO change to InputCommands type when we can drop sizeof assert
    __int16 field_1D6_continue_move_input; //TODO change to InputCommands type when we can drop sizeof assert
};
ALIVE_ASSERT_SIZEOF(MineCar, 0x1D8);
