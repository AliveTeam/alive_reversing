#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class LevelIds : __int16;

enum class PortalType : __int16
{
    eAbe_0 = 0,
    eWorker_1 = 1,
    eShrykull_2 = 2,
    eMudTeleport_3 = 3,
};

enum class PortalSide : __int16
{
    eRight_0 = 0,
    eLeft_1 = 1,
};

struct Path_BirdPortal : public Path_TLV
{
    PortalSide field_10_side;
    LevelIds field_12_dest_level;
    __int16 field_14_dest_path;
    __int16 field_16_dest_camera;
    __int16 field_18_scale;
    __int16 field_1A_moive_id;
    PortalType field_1C_portal_type;
    __int16 field_1E_num_muds_for_shrykul;
    __int16 field_20_create_id;
    __int16 field_22_delete_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BirdPortal, 0x24);

struct Path_BirdPortal_Exit : public Path_TLV
{
    PortalSide field_10_side;
    __int16 field_12_scale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BirdPortal, 0x24);

struct BirdPortal_State
{
    Types field_0_type;
    BYTE field_2_state;
    BYTE field_3_mud_count;
    int field_4_tlvInfo;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(BirdPortal_State, 8);

class BirdPortalTerminator : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BaseAnimatedWithPhysicsGameObject* ctor_497960(FP xpos, FP ypos, FP scale, PortalType portalType);
    virtual void VScreenChanged() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;
    EXPORT void Fadeout_497AC0();
private:
    EXPORT BaseAnimatedWithPhysicsGameObject* vdtor_497A70(signed int flags);
};
ALIVE_ASSERT_SIZEOF(BirdPortalTerminator, 0xF4);

class OrbWhirlWind;

class BirdPortal : public BaseGameObject
{
public:
    EXPORT BaseGameObject* ctor_497E00(Path_BirdPortal* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual signed int VGetSaveState(BYTE* pSaveBuffer) override;

    // New virtuals

    virtual signed __int16 VPortalClipper_499430(__int16 bUnknown);
    virtual void VKillPortalClipper_499610();
    virtual void VMudSaved_499A50();
    virtual BOOL VActivePortal_499830();
    virtual void VGiveShrukull_499680(__int16 bPlaySound);
    virtual BOOL VAbeInsidePortal_499850();
    virtual void VExitPortal_499870();
    virtual BOOL VPortalExit_AbeExitting_499A00();
    virtual void VKillPortalClipper_499A20();
    virtual void VGetMapChange_499AE0(LevelIds* level, WORD* path, WORD* camera, CameraSwapEffects* screenChangeEffect, WORD* movieId);

    EXPORT static int CC CreateFromSaveState_499C90(const BYTE* pBuffer);


private:
    EXPORT BirdPortal* vdtor_498050(signed int flags);
    EXPORT void vUpdate_498280();
    EXPORT void vScreenChanged_499B50();
    EXPORT void vStopAudio_499260();
    EXPORT signed int vGetSaveState_499F50(BYTE* pState);

    EXPORT signed __int16 vPortalClipper_499430(__int16 bUnknown);
    EXPORT void vKillPortalClipper_499610();
    EXPORT void vMudSaved_499A50();
    EXPORT BOOL vActivePortal_499830();
    EXPORT void vGiveShrukull_499680(__int16 bPlaySound);
    EXPORT BOOL vAbeInsidePortal_499850();
    EXPORT void vExitPortal_499870();
    EXPORT BOOL vPortalExit_AbeExitting_499A00();
    EXPORT void vKillPortalClipper_499A20();
    EXPORT void vGetMapChange_499AE0(LevelIds* level, WORD* path, WORD* camera, CameraSwapEffects* screenChangeEffect, WORD* movieId);
    EXPORT void dtor_4980A0();

    EXPORT signed __int16 IsScaredAway_4992A0();

    EXPORT void CreateDovesAndShrykullNumber_497B50();

    EXPORT void KillTerminators_499220();

    EXPORT void CreateTerminators_497D10();

    EXPORT signed __int16 GetEvent_499A70();

private:
    int field_20_tlvInfo;
public:
    PortalType field_24_portal_type;
    PortalSide field_26_side;
public:
    enum class PortalStates : __int16
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
    __int16 field_2A_pad;
public:
    FP field_2C_xpos;
    FP field_30_ypos;
    FP field_34_exit_x;
    FP field_38_exit_y;
public:
    FP field_3C_YPos;
private:
    int field_40_throwable_indicator_id;
    int field_44_dove_ids[6];
    int field_5C_timer;
    FP field_60_scale;
    __int16 field_64_movie_id;
    __int16 field_66_delete_id;
    __int16 field_68_doves_exist;
    __int16 field_6A_pad;
    int field_6C_terminator_id;
    int field_70_terminator_id;
    int field_74_screen_clipper_id;
    int field_78_screen_clipper_id;
    LevelIds field_7C_dest_level;
    __int16 field_7E_dest_path;
    __int16 field_80_dest_camera;
    __int16 field_82_num_muds_for_shrykull;
    __int16 field_84;
    __int16 field_86_pad;
    OrbWhirlWind* field_88_pWhirlWind;
    LevelIds field_8C_level;
    __int16 field_8E_path;
    int field_90_sfx_ret;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
};
ALIVE_ASSERT_SIZEOF(BirdPortal, 0x100);
