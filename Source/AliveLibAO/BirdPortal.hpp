#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../AliveLibAE/Path.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

enum class LevelIds : s16;

enum class PortalType : s16
{
    eAbe_0 = 0,
    eWorker_1 = 1,
    eShrykull_2 = 2,
    eMudTeleport_3 = 3,
};

enum class PortalSide : s16
{
    eRight_0 = 0,
    eLeft_1 = 1,
};

struct Path_BirdPortal final : public Path_TLV
{
    PortalSide field_18_side;
    LevelIds field_1A_dest_level;
    s16 field_1C_dest_path;
    s16 field_1E_dest_camera;
    Scale_short field_20_scale;
    s16 field_22_movie_id;
    PortalType field_24_portal_type;
    s16 field_26_num_muds_for_shrykul;
};
ALIVE_ASSERT_SIZEOF(Path_BirdPortal, 0x28);

struct Path_BirdPortalExit final : public Path_TLV
{
    PortalSide field_18_side;
    Scale_short field_1A_scale;
};
ALIVE_ASSERT_SIZEOF(Path_BirdPortalExit, 0x1C);

class BirdPortalTerminator final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BirdPortalTerminator(FP xpos, FP ypos, FP scale, PortalType portalType);

    virtual void VScreenChanged() override;

    
    void Fadeout();
};
ALIVE_ASSERT_SIZEOF(BirdPortalTerminator, 0xE4);

class OrbWhirlWind;
class ThrowableTotalIndicator;
class ScreenClipper;
class Dove;

class BirdPortal final : public BaseGameObject
{
public:
    BirdPortal(Path_BirdPortal* pTlv, s32 tlvInfo);
    ~BirdPortal();

    s16 IsScaredAway();

    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;

    virtual s16 VPortalClipper(s16 bUnknown);
    virtual void VKillPortalClipper();
    virtual void VMudSaved();
    virtual bool VStateIs6();
    virtual void VGiveShrykull(s16 bPlaySound);
    virtual bool VStateIs16();
    virtual void VExitPortal();
    virtual void VIncreaseTimerAndKillPortalClipper();
    virtual bool VStateIs20();
    virtual void VGetMapChange(EReliveLevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId);

private:
    void CreateDovesAndShrykullNumber();
    void CreateTerminators();

public:
    PortalType field_10_portal_type = PortalType::eAbe_0;
    PortalSide field_12_side = PortalSide::eRight_0;
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
        State_8 = 8,
        GetShrykull_9 = 9,
        CollapseTerminators_10 = 10,
        StopSound_11 = 11,
        CreateFlash1_12 = 12,
        CreateFlash2_13 = 13,
        CreateFlash3_14 = 14,
        KillPortal_15 = 15,
        State_16 = 16,
        PortalExit_SetPosition_17 = 17,
        PortalExit_CreateTerminators_18 = 18,
        PortalExit_ExpandTerminators_19 = 19,
        State_20 = 20,
        KillPortalClipper_21 = 21,
        FadeoutTerminators_22 = 22,
    };
    PortalStates field_14_state = PortalStates::CreatePortal_0;
    s16 field_16 = 0;
    FP field_18_xpos = FP_FromInteger(0);
    FP field_1C_ypos = FP_FromInteger(0);
    FP field_20_exit_x = FP_FromInteger(0);
    FP field_24_exit_y = FP_FromInteger(0);
    FP field_28_ypos = FP_FromInteger(0);
    s32 field_2C_tlvInfo = 0;
    s32 field_30_timer = 0;
    FP field_34_scale = FP_FromInteger(0);
    s16 field_38_movie_id = 0;
    s16 field_3A = 0;
    BirdPortalTerminator* field_3C_pTerminator1 = nullptr;
    BirdPortalTerminator* field_40_pTerminator2 = nullptr;
    ScreenClipper* field_44_pScreenClipper1 = nullptr;
    ScreenClipper* field_48_pScreenClipper2 = nullptr;
    DynamicArrayT<Dove>* field_4C_pDovesArray = {};
    EReliveLevelIds field_50_dest_level = EReliveLevelIds::eNone;
    s16 field_52_dest_path = 0;
    s16 field_54_dest_camera = 0;
    s16 field_56_num_muds_for_shrykull = 0;
    s16 field_58_received_doves = 0;
    s16 field_5A = 0;
    ThrowableTotalIndicator* field_5C_pThrowableTotalIndicator = nullptr;
    OrbWhirlWind* field_60_pOrbWhirlWind = nullptr;
    EReliveLevelIds field_64_level = EReliveLevelIds::eNone;
    s16 field_66_path = 0;
    s32 field_68_sfx_ret = 0;
};
ALIVE_ASSERT_SIZEOF(BirdPortal, 0x6C);

} // namespace AO
