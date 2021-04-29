#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class LevelIds : s16;

enum class PortalType : s16
{
    eAbe_0 = 0,
    eWorker_1 = 1,
    eShrykull_2 = 2,
    // eMudTeleport_3 = 3, // TODO: not used ??
};

enum class PortalSide : s16
{
    eRight_0 = 0,
    eLeft_1 = 1,
};

struct Path_BirdPortal final : public Path_TLV
{
    PortalSide field_10_side;
    LevelIds field_12_dest_level;
    s16 field_14_dest_path;
    s16 field_16_dest_camera;
    Scale_short field_18_scale;
    s16 field_1A_movie_id;
    PortalType field_1C_portal_type;
    s16 field_1E_mudokon_amount_for_shrykull;
    s16 field_20_create_id;
    s16 field_22_delete_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BirdPortal, 0x24);

struct Path_BirdPortalExit final : public Path_TLV
{
    PortalSide field_10_side;
    Scale_short field_12_scale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BirdPortal, 0x24);

struct BirdPortal_State final
{
    AETypes field_0_type;
    u8 field_2_state;
    u8 field_3_mud_count;
    s32 field_4_tlvInfo;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(BirdPortal_State, 8);

class BirdPortalTerminator final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BaseAnimatedWithPhysicsGameObject* ctor_497960(FP xpos, FP ypos, FP scale, PortalType portalType);
    virtual void VScreenChanged() override;
    virtual BaseGameObject* VDestructor(s32 flags) override;
    EXPORT void Fadeout_497AC0();

private:
    EXPORT BaseAnimatedWithPhysicsGameObject* vdtor_497A70(s32 flags);
};
ALIVE_ASSERT_SIZEOF(BirdPortalTerminator, 0xF4);

class OrbWhirlWind;

class BirdPortal final : public BaseGameObject
{
public:
    EXPORT BaseGameObject* ctor_497E00(Path_BirdPortal* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    // New virtuals

    virtual s16 VPortalClipper_499430(s16 bUnknown);
    virtual void VKillPortalClipper_499610();
    virtual void VMudSaved_499A50();
    virtual BOOL VActivePortal_499830();
    virtual void VGiveShrukull_499680(s16 bPlaySound);
    virtual BOOL VAbeInsidePortal_499850();
    virtual void VExitPortal_499870();
    virtual BOOL VPortalExit_AbeExitting_499A00();
    virtual void VKillPortalClipper_499A20();
    virtual void VGetMapChange_499AE0(LevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId);

    EXPORT static s32 CC CreateFromSaveState_499C90(const u8* pBuffer);


private:
    EXPORT BirdPortal* vdtor_498050(s32 flags);
    EXPORT void vUpdate_498280();
    EXPORT void vScreenChanged_499B50();
    EXPORT void vStopAudio_499260();
    EXPORT s32 vGetSaveState_499F50(u8* pState);

    EXPORT s16 vPortalClipper_499430(s16 bUnknown);
    EXPORT void vKillPortalClipper_499610();
    EXPORT void vMudSaved_499A50();
    EXPORT BOOL vActivePortal_499830();
    EXPORT void vGiveShrukull_499680(s16 bPlaySound);
    EXPORT BOOL vAbeInsidePortal_499850();
    EXPORT void vExitPortal_499870();
    EXPORT BOOL vPortalExit_AbeExitting_499A00();
    EXPORT void vIncreaseTimerAndKillPortalClipper_499A20();
    EXPORT void vGetMapChange_499AE0(LevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId);
    EXPORT void dtor_4980A0();

    EXPORT s16 IsScaredAway_4992A0();

    EXPORT void CreateDovesAndShrykullNumber_497B50();

    EXPORT void KillTerminators_499220();

    EXPORT void CreateTerminators_497D10();

    EXPORT s16 GetEvent_499A70();

private:
    s32 field_20_tlvInfo;

public:
    PortalType field_24_portal_type;
    PortalSide field_26_side;

public:
    enum class PortalStates : s16
    {
        CreatePortal_0 = 0,
        IdlePortal_1 = 1,
        JoinBirdsInCenter_2 = 2,
        KillBirds_3 = 3,
        CreateTerminators_4 = 4,
        ExpandTerminators_5 = 5,
        ActivePortal_6 = 6,
        ShrykullGetDoves_7 = 7,
        Unused_8 = 8,
        GetShrykull_9 = 9,
        CollapseTerminators_10 = 10,
        StopSound_11 = 11,
        CreateFlash1_12 = 12,
        CreateFlash2_13 = 13,
        CreateFlash3_14 = 14,
        KillPortal_15 = 15,
        AbeInsidePortal_16 = 16,
        PortalExit_SetPosition_17 = 17,
        PortalExit_CreateTerminators_18 = 18,
        PortalExit_ExpandTerminators_19 = 19,
        PortalExit_AbeExitting_20 = 20,
        KillPortalClipper_21 = 21,
        FadeoutTerminators_22 = 22,
    };
    PortalStates field_28_state;

private:
    s16 field_2A_pad;

public:
    FP field_2C_xpos;
    FP field_30_ypos;
    FP field_34_exit_x;
    FP field_38_exit_y;

public:
    FP field_3C_YPos;

private:
    s32 field_40_throwable_indicator_id;
    s32 field_44_dove_ids[6];
    s32 field_5C_timer;
    FP field_60_scale;
    s16 field_64_movie_id;
    s16 field_66_delete_id;
    s16 field_68_doves_exist;
    s16 field_6A_pad;
    s32 field_6C_terminator_id;
    s32 field_70_terminator_id;
    s32 field_74_screen_clipper_id;
    s32 field_78_screen_clipper_id;
    LevelIds field_7C_dest_level;
    s16 field_7E_dest_path;
    s16 field_80_dest_camera;
    s16 field_82_num_muds_for_shrykull;
    s16 field_84;
    s16 field_86_pad;
    OrbWhirlWind* field_88_pWhirlWind;
    LevelIds field_8C_level;
    s16 field_8E_path;
    s32 field_90_sfx_ret;
    s32 field_94;
    s32 field_98;
    s32 field_9C;
    s32 field_A0;
    s32 field_A4;
    s32 field_A8;
    s32 field_AC;
    s32 field_B0;
    s32 field_B4;
    s32 field_B8;
    s32 field_BC;
    s32 field_C0;
    s32 field_C4;
    s32 field_C8;
    s32 field_CC;
    s32 field_D0;
    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    s32 field_E0;
    s32 field_E4;
    s32 field_E8;
    s32 field_EC;
    s32 field_F0;
    s32 field_F4;
    s32 field_F8;
    s32 field_FC;
};
ALIVE_ASSERT_SIZEOF(BirdPortal, 0x100);
