#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "FlyingSligSpawner.hpp"

enum class LevelIds : __int16;

class FlyingSlig;

using TFlyingSligFn = void(FlyingSlig::*)(void);

#define FLYING_SLIG_MOTIONS_ENUM(ENTRY) \
    ENTRY(M_Idle_0_4385E0) \
    ENTRY(M_HorizontalMovement_1_4386A0) \
    ENTRY(M_IdleToTurn_2_4388B0) \
    ENTRY(M_DownMovement_3_438AA0) \
    ENTRY(M_DownMovementToTurn_4_438CC0) \
    ENTRY(M_UpMovement_5_438DD0) \
    ENTRY(M_UpMovementToTurn_6_439030) \
    ENTRY(M_LeverPull_7_439150) \
    ENTRY(M_GameSpeak_8_4391D0) \
    ENTRY(M_Possession_9_434290) \
    ENTRY(M_EndHorizontalMovement_10_4387D0) \
    ENTRY(M_BeginUpMovement_11_438E40) \
    ENTRY(M_HorizontalToDownMovement_12_438B10) \
    ENTRY(M_UpToHorizontalMovement_13_438F60) \
    ENTRY(M_DownToHorizontalMovement_14_438BF0) \
    ENTRY(M_QuickTurn_15_4387F0) \
    ENTRY(M_IdleToHorizontalMovement_16_438730) \
    ENTRY(M_BeginDownMovement_17_438B80) \
    ENTRY(M_EndDownMovement_18_438C90) \
    ENTRY(M_DownKnockback_19_4390D0) \
    ENTRY(M_UpKnockback_20_439110) \
    ENTRY(M_EndUpMovement_21_438EB0) \
    ENTRY(M_InstantUpXTurn_22_438EE0) \
    ENTRY(M_InstantDownXTurn_23_438F20) \
    ENTRY(M_HorizontalToUpMovement_24_438D60) \
    ENTRY(M_TurnToHorizontalMovement_25_4389E0)

#define MAKE_ENUM(VAR) VAR,
enum eFlyingSligMotions : int
{
    FLYING_SLIG_MOTIONS_ENUM(MAKE_ENUM)
};

enum class SligSpeak : char;

struct FlyingSlig_State
{
    Types field_0_type;
    __int16 field_2;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    __int16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    __int16 field_1C_oldr;
    __int16 field_1E_oldg;
    __int16 field_20_oldb;
    __int16 field_22_bAnimFlipX;
    __int16 field_24_current_state;
    __int16 field_26_current_frame;
    __int16 field_28_frame_change_counter;
    BYTE field_2A_bAnimRender;
    BYTE field_2B_bDrawable;
    FP field_2C_current_health;
    __int16 field_30_current_state;
    __int16 field_32_delayed_state;
    __int16 field_34_lastLineYPos;
    __int16 field_36_line_idx;
    BYTE field_38;
    BYTE field_39_pad;
    enum Flags_3A
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
        eBit8 = 0x80,
        eBit9 = 0x100,
        eBit10 = 0x200,
        eBit11 = 0x400,
    };
    BitField16<Flags_3A> field_3A;
    int field_3C_tlvInfo;
    int field_40_timer;
    int field_44_grenade_delay;
    int field_48;
    FP field_4C_xSpeed;
    FP field_50_ySpeed;
    SligSpeak field_54_next_speak;
    char field_55;
    __int16 field_56_voice_pitch_min;
    int field_58_obj_id;
    FP field_5C;
    FP field_60;
    FP field_64;
    FP field_68_line_length;
    FP field_6C;
    FP field_70;
    FP field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    FP field_88_nextXPos;
    FP field_8C_nextYPos;
    int field_90_fns1_idx;
    __int16 field_94;
    __int16 field_96;
    __int16 field_98;
    LevelIds field_9A_abe_level;
    __int16 field_9C_abe_path;
    __int16 field_9E_abe_camera;
    __int16 field_A0;
    __int16 field_A2;
    int field_A4;
    FP field_A8;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSlig_State, 0xAC);

class FlyingSlig : public BaseAliveGameObject
{
public:
    EXPORT FlyingSlig* ctor_4342B0(Path_FlyingSlig* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VRender(int** pOrderingTable) override;

    virtual void VScreenChanged() override;

    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    virtual void VPossessed_408F70() override;

    virtual void VSetMotion_4081C0(__int16 state) override;

    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    // TODO: Remaining virtuals
    
    EXPORT static int CC CreateFromSaveState_437E40(const BYTE* pBuffer);

private:
    EXPORT int vGetSaveState_43B1E0(FlyingSlig_State* pState);


    EXPORT void dtor_434990();

    EXPORT FlyingSlig* vdtor_434870(signed int flags);

    EXPORT void vScreenChanged_434C10();


    EXPORT void vUpdate_434AD0();

    EXPORT void vRender_435130(int** ot);

    EXPORT __int16 IsPossessed_436A90();
    EXPORT void sub_4348A0();
    EXPORT void Movement_4396E0();

    EXPORT __int16 vTakeDamage_434C90(BaseGameObject* pFrom);

public:

    EXPORT void AI_Inactive_0_4355B0();
    EXPORT void AI_Death_1_4364E0();
    EXPORT void AI_Moving_2_4356D0();
    EXPORT void AI_GetAlerted_3_435750();
    EXPORT void AI_ChasingEnemy_4_435BC0();
    EXPORT void AI_Idle_5_435820();
    EXPORT void AI_GameSpeakToMoving_6_435940();
    EXPORT void AI_PanicMoving_7_435990();
    EXPORT void AI_PanicIdle_8_435AC0();
    EXPORT void AI_SpottedEnemy_9_435E40();
    EXPORT void AI_LaunchingGrenade_10_435F10();
    EXPORT void AI_AbeDead_11_435FD0();
    EXPORT void AI_Possessed_12_436040();
    EXPORT void AI_Possession_13_4360F0();
    EXPORT void AI_DePossession_14_436180();
    EXPORT void AI_FlyingSligSpawn_15_4362C0();
    EXPORT void AI_FromNakedSlig_17_4355E0();
public:

    EXPORT void M_Idle_0_4385E0();
    EXPORT void M_HorizontalMovement_1_4386A0();
    EXPORT void M_IdleToTurn_2_4388B0();
    EXPORT void M_DownMovement_3_438AA0();
    EXPORT void M_DownMovementToTurn_4_438CC0();
    EXPORT void M_UpMovement_5_438DD0();
    EXPORT void M_UpMovementToTurn_6_439030();
    EXPORT void M_LeverPull_7_439150();
    EXPORT void M_GameSpeak_8_4391D0();
    EXPORT void M_Possession_9_434290();
    EXPORT void M_EndHorizontalMovement_10_4387D0();
    EXPORT void M_BeginUpMovement_11_438E40();
    EXPORT void M_HorizontalToDownMovement_12_438B10();
    EXPORT void M_UpToHorizontalMovement_13_438F60();
    EXPORT void M_DownToHorizontalMovement_14_438BF0();
    EXPORT void M_QuickTurn_15_4387F0();
    EXPORT void M_IdleToHorizontalMovement_16_438730();
    EXPORT void M_BeginDownMovement_17_438B80();
    EXPORT void M_EndDownMovement_18_438C90();
    EXPORT void M_DownKnockback_19_4390D0();
    EXPORT void M_UpKnockback_20_439110();
    EXPORT void M_EndUpMovement_21_438EB0();
    EXPORT void M_InstantUpXTurn_22_438EE0();
    EXPORT void M_InstantDownXTurn_23_438F20();
    EXPORT void M_HorizontalToUpMovement_24_438D60();
    EXPORT void M_TurnToHorizontalMovement_25_4389E0();

    EXPORT signed __int16 sub_43A510();

public:
    EXPORT void ToPlayerControlled_4360C0();

    EXPORT void ToMoving_435720();

    EXPORT void ToPanicIdle_435B50();

    EXPORT void ToChase_435E10();

    EXPORT __int16 CanChase_436850(BaseAliveGameObject* pObj);

    EXPORT void Say_436A50(SligSpeak speak, __int16 pitch);

    EXPORT __int16 sub_4374A0(__int16 a2);

    EXPORT static __int16 CCSTD IsAbeEnteringDoor_43B030(BaseAliveGameObject *pThis );

    EXPORT static BOOL CCSTD IsWallBetween_43A550(BaseAliveGameObject *pThis, BaseAliveGameObject *pObj);

    EXPORT void ThrowGrenade_43A1E0();

    EXPORT void BlowUp_436510();

    EXPORT __int16 sub_436730();

    EXPORT __int16 CanHearAbe_4369C0();

    EXPORT void ToSpottedEnemy_435E70();

    EXPORT void ToAbeDead_436010();

    EXPORT void ToAlerted_4357E0();

    EXPORT void ToPanicMoving_435A50();

    EXPORT __int16 IsTurning_436AE0();

    EXPORT BYTE** ResBlockForMotion_4350F0(int motion);

    EXPORT void ToChantShake_436270();

    EXPORT void ToPossesed_436130();

    EXPORT void vUpdateAnimRes_4350A0();

    EXPORT void PatrolDelay_435860();

    EXPORT __int16 CanThrowGrenade_43A490();

    EXPORT void ToLaunchingGrenade_435F50();

    EXPORT void HandlePlayerControls_439340();

    EXPORT __int16 sub_437C70(PathLine* pLine);

    EXPORT __int16 FindLeftOrRightBound_43B0A0(FP xOrY, FP wOrH);

    EXPORT void vPossessed_434FB0();

    EXPORT __int16 sub_436C60(PSX_RECT* pRect, __int16 op1);

    EXPORT BOOL sub_436B20();

    EXPORT void sub_4373B0();

    EXPORT void vSetMotion_43B1B0(__int16 newMotion);

    EXPORT __int16 CollisionUp_43A640(FP velY);

    EXPORT __int16 CollisionDown_43A9E0(FP velY);

    EXPORT __int16 CollisionLeftRight_43AC80(FP velX);

    EXPORT void sub_436450();

    EXPORT __int16 TryPullLever_439DB0();

    EXPORT void sub_437AC0(FP a2, FP_Point* pPoint);


    void SetBrain(TFlyingSligFn fn);
    bool BrainIs(TFlyingSligFn fn);

private:
    Path_FlyingSlig field_118_data;
    int field_148_tlvInfo;
    int field_14C_timer;
    int field_150_grenade_delay;
    int field_154;
    int field_158_obj_id;
    __int16 field_15C_voice_pitch_min;
    __int16 field_15E;
    __int16 field_160_voice_pitch_min;
    __int16 field_162;
    int field_164;
    __int16 field_168;
    __int16 field_16A;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    __int16 field_174;
    __int16 field_176;
    __int16 field_178;
    __int16 field_17A;
    BYTE field_17C_launch_id;
    SligSpeak field_17D_next_speak;

    enum Flags_17E
    {
        eBit1_Speaking_flag1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5_Throw = 0x10,
        eBit6 = 0x20,
        eBit7_DoAction = 0x40,
        eBit8 = 0x80,
        eBit9_Chanting = 0x100,
        eBit10_Speaking_flag2 = 0x200,
        eBit11_bNoPrevLine = 0x400,
        eBit12_bNoNextLine = 0x800,
        eBit13_Persistant = 0x1000,
    };
    BitField16<Flags_17E> field_17E_flags;
    __int16 field_180_bound2;
    __int16 field_182_bound1;
    FP field_184_xSpeed;
    FP field_188_ySpeed;
    FP field_18C;
    FP field_190;
    FP field_194;
    FP field_198_line_length;
    __int16 field_19C;
    __int16 field_19E;
    __int16 field_1A0;
    __int16 field_1A2;
    FP_Rect field_1A4_rect;
    __int16 field_1B4;
    __int16 field_1B6;
    __int16 field_1B8;
    __int16 field_1BA;
    FP field_1BC;
    FP field_1C0;
    FP field_1C4;
    FP field_1C8;
    FP field_1CC;
    __int16 field_1D0;
    __int16 field_1D2;
    __int16 field_1D4;
    __int16 field_1D6;
    int field_1D8;
    int field_1DC;
    int field_1E0;
    int field_1E4;
    __int16 field_1E8;
    __int16 field_1EA;
    PathLine* field_1EC_pNextLine;
    PathLine* field_1F0_pPrevLine;
    __int16 field_1F4_pPalAlloc[64];
    PSX_RECT field_274_pal_rect;
    __int16 field_27C_r;
    __int16 field_27E_g;
    __int16 field_280_b;
    __int16 field_282;
    FP field_284;
    const FP* field_288;
    __int16 field_28C;
    __int16 field_28E;
    int field_290;
public:
    FP field_294_nextXPos;
    FP field_298_nextYPos;
    TFlyingSligFn field_29C_brain_state;
    LevelIds field_2A0_abe_level;
    __int16 field_2A2_abe_path;
    __int16 field_2A4_abe_camera;
    __int16 field_2A6;
    FP field_2A8_max_x_speed;
    FP field_2AC_up_vel;
    FP field_2B0_down_vel;
    FP field_2B4_max_slow_down;
    FP field_2B8_max_speed_up;
};
ALIVE_ASSERT_SIZEOF(FlyingSlig, 0x2BC);
