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
    
    void Fadeout();
};
ALIVE_ASSERT_SIZEOF(BirdPortalTerminator, 0xF4);

class OrbWhirlWind;
enum Event : s16;

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

    virtual s16 VPortalClipper(s16 bUnknown);
    virtual void VKillPortalClipper();
    virtual void VMudSaved();
    virtual Bool32 VActivePortal();
    virtual void VGiveShrykull(s16 bPlaySound);
    virtual Bool32 VAbeInsidePortal();
    virtual void VExitPortal();
    virtual Bool32 VPortalExit_AbeExitting();
    virtual void VIncreaseTimerAndKillPortalClipper();
    virtual void VGetMapChange(EReliveLevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId);

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    s16 IsScaredAway();
    void CreateDovesAndShrykullNumber();
    void KillTerminators();
    void CreateTerminators();
    Event GetEvent();

private:
    s32 field_20_tlvInfo = 0;

public:
    PortalType field_24_portal_type = PortalType::eAbe_0;
    PortalSide field_26_side = PortalSide::eRight_0;

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
    PortalStates field_28_state = PortalStates::CreatePortal_0;

public:
    FP field_2C_xpos = {};
    FP field_30_ypos = {};
    FP field_34_exit_x = {};
    FP field_38_exit_y = {};

public:
    FP field_3C_YPos = {};

private:
    s32 field_40_throwable_indicator_id = 0;
    s32 field_44_dove_ids[6] = {};
    s32 field_5C_timer = 0;
    FP field_60_scale = {};
    s16 field_64_movie_id = 0;
    s16 field_66_delete_portal_switch_id = 0;
    s16 field_68_doves_exist = 0;
    s32 field_6C_terminator_id = 0;
    s32 field_70_terminator_id = 0;
    s32 field_74_screen_clipper_id = 0;
    s32 field_78_screen_clipper_id = 0;
    EReliveLevelIds field_7C_dest_level = EReliveLevelIds::eNone;
    s16 field_7E_dest_path = 0;
    s16 field_80_dest_camera = 0;
    s16 field_82_num_muds_for_shrykull = 0;
    s16 field_84_received_doves = 0;
    OrbWhirlWind* field_88_pWhirlWind = nullptr;
    EReliveLevelIds field_8C_level = EReliveLevelIds::eNone;
    s16 field_8E_path = 0;
    s32 field_90_sfx_ret = 0;
};
ALIVE_ASSERT_SIZEOF(BirdPortal, 0x100);
