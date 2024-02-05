#pragma once

#include "BaseAliveGameObject.hpp"
#include "Input.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/SaveStateBase.hpp"

class CollisionMask;
enum eLineTypes : s16;

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

struct MineCarSaveState final : public SaveStateBase
{
    MineCarSaveState()
        : SaveStateBase(ReliveTypes::eMineCar, sizeof(*this))
    { }
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    FP field_14_sprite_scale;
    s16 field_18_path_number;
    EReliveLevelIds field_1A_lvl_number;
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_xFlip;
    s32 field_24_frame_table;
    s16 field_2A_current_anim_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_render;
    s8 field_2F_drawable;
    s32 field_38_frame_table_offset2;
    FP field_3C_health;
    s16 field_44_last_line_ypos;
    eLineTypes field_46_collision_line_type;
    Guid field_4C_tlvInfo;
    MineCarStates field_50_state;
    MineCarDirs field_52_turn_direction;
    s16 field_58_falling_counter;
    s16 field_5A_bAbeInCar;
    s32 field_5C_frame_mod_16;
    s16 field_60_spawned_path;
    s16 field_62_spawned_camera;
    s16 field_64_throw_item_key1;
    s16 field_66_continue_move_input;
};

class MineCar final : public BaseAliveGameObject
{
public:
    MineCar(relive::Path_MineCar* pTlv, const Guid& tlvId, s32 a4, s32 a5, s32 a6);
    ~MineCar();
    
    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;

    static void CreateFromSaveState(SerializedObjectData& pBuffer);

private:
    void LoadAnimation(Animation* pAnim);
    bool CheckRoofCollision(FP hitX, FP hitY);
    bool CheckFloorCollision(FP hitX, FP hitY);
    void Stop();
    void Move(AnimId animId, FP velX, FP velY, InputCommands input, MineCarDirs dir, s8 bChangeDirection);
    s16 IsBlocked(MineCarDirs a2, s32 a3);
    s16 FollowDirection();
    void RunThingsOver();

    void State_0_ParkedWithoutAbe();
    void State_1_ParkedWithAbe();
    void State_2_Moving();
    void State_3_Falling();

    //State_1 helper funcs
    void HandleUpDown();

    using mineCarFPFunc = bool (MineCar::*)(FP, FP);
    bool HandleState1Move(const mineCarFPFunc func, const FP mineCarFPFuncArg1, const FP mineCarFPFuncArg2, const FP mineCarFPFuncArg3, AnimId animId, MineCarDirs mineCarDir, const s8 changeDir, FP rayCast1, FP rayCast2, FP rayCast3, FP rayCast4, const CollisionMask ModelMask1, const CollisionMask ModelMask2, FP moveX, FP moveY, InputCommands key, bool isVertical, bool verticalFlipXCond);

private:
    Guid field_118_tlvInfo;

public:
    MineCarStates field_11C_state = MineCarStates::eParkedWithoutAbe_0;

private:
    relive::reliveScale field_11E_scale = relive::reliveScale::eFull;
    Animation mTreadAnim = {};

public:
    MineCarDirs field_1BC_turn_direction = MineCarDirs::eDown_0;

private:
    s16 field_1C2_falling_counter = 0;
    s16 field_1C4_velx_index = 0;
    s32 field_1C8_frame_mod_16 = 0;
    s16 field_1CC_spawned_path = 0;
    s16 field_1CE_spawned_camera = 0;
    s32 field_1D0_sound_channels_mask = 0;
    s16 field_1D4_previous_input = 0;      //TODO change to InputCommands type when we can drop sizeof assert
    s16 field_1D6_continue_move_input = 0; //TODO change to InputCommands type when we can drop sizeof assert
};
