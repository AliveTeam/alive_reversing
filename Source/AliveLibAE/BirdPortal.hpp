#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

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
    s16 field_20_create_portal_switch_id;
    s16 field_22_delete_portal_switch_id;
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
    BirdPortalTerminator(FP xpos, FP ypos, FP scale, PortalType portalType);
    virtual void VScreenChanged() override;
    
    void Fadeout_497AC0();

private:
    BaseAnimatedWithPhysicsGameObject* vdtor_497A70(s32 flags);
};
ALIVE_ASSERT_SIZEOF(BirdPortalTerminator, 0xF4);

class OrbWhirlWind;

class BirdPortal final : public BaseGameObject
{
public:
    BirdPortal(Path_BirdPortal* pTlv, s32 tlvInfo);
    ~BirdPortal();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    // New virtuals

    virtual s16 VPortalClipper_499430(s16 bUnknown);
    virtual void VKillPortalClipper_499610();
    virtual void VMudSaved_499A50();
    virtual Bool32 VActivePortal_499830();
    virtual void VGiveShrukull_499680(s16 bPlaySound);
    virtual Bool32 VAbeInsidePortal_499850();
    virtual void VExitPortal_499870();
    virtual Bool32 VPortalExit_AbeExitting_499A00();
    virtual void VKillPortalClipper_499A20();
    virtual void VGetMapChange_499AE0(LevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId);

    static s32 CreateFromSaveState(const u8* pBuffer);


private:
    void vStopAudio_499260();
    s32 vGetSaveState_499F50(u8* pState);

    s16 vPortalClipper_499430(s16 bUnknown);
    void vKillPortalClipper_499610();
    void vMudSaved_499A50();
    Bool32 vActivePortal_499830();
    void vGiveShryukull_499680(s16 bPlaySound);
    Bool32 vAbeInsidePortal_499850();
    void vExitPortal_499870();
    Bool32 vPortalExit_AbeExitting_499A00();
    void vIncreaseTimerAndKillPortalClipper_499A20();
    void vGetMapChange_499AE0(LevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId);

    s16 IsScaredAway_4992A0();

    void CreateDovesAndShrykullNumber_497B50();

    void KillTerminators_499220();

    void CreateTerminators_497D10();

    s16 GetEvent_499A70();

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
        JoinDovesInCenter_2 = 2,
        KillDoves_3 = 3,
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
    s16 field_66_delete_portal_switch_id;
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
    s16 field_84_received_doves;
    s16 field_86_pad;
    OrbWhirlWind* field_88_pWhirlWind;
    LevelIds field_8C_level;
    s16 field_8E_path;
    s32 field_90_sfx_ret;
    s32 field_94_padding;
    s32 field_98_padding;
    s32 field_9C_padding;
    s32 field_A0_padding;
    s32 field_A4_padding;
    s32 field_A8_padding;
    s32 field_AC_padding;
    s32 field_B0_padding;
    s32 field_B4_padding;
    s32 field_B8_padding;
    s32 field_BC_padding;
    s32 field_C0_padding;
    s32 field_C4_padding;
    s32 field_C8_padding;
    s32 field_CC_padding;
    s32 field_D0_padding;
    s32 field_D4_padding;
    s32 field_D8_padding;
    s32 field_DC_padding;
    s32 field_E0_padding;
    s32 field_E4_padding;
    s32 field_E8_padding;
    s32 field_EC_padding;
    s32 field_F0_padding;
    s32 field_F4_padding;
    s32 field_F8_padding;
    s32 field_FC_padding;
};
ALIVE_ASSERT_SIZEOF(BirdPortal, 0x100);
