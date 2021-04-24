#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "DynamicArray.hpp"

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

struct Path_BirdPortal : public Path_TLV
{
    PortalSide field_18_side;
    LevelIds field_1A_dest_level;
    s16 field_1C_dest_path;
    s16 field_1E_dest_camera;
    s16 field_20_scale;
    s16 field_22_movie_id;
    PortalType field_24_portal_type;
    s16 field_26_num_muds_for_shrykul;
};
ALIVE_ASSERT_SIZEOF(Path_BirdPortal, 0x28);

struct Path_BirdPortalExit : public Path_TLV
{
    PortalSide field_18_side;
    s16 field_1A_scale;
};
ALIVE_ASSERT_SIZEOF(Path_BirdPortalExit, 0x1C);

class BirdPortalTerminator : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BirdPortalTerminator* ctor_451F70(FP xpos, FP ypos, FP scale, PortalType portalType);

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4_padding[4];
};
ALIVE_ASSERT_SIZEOF(BirdPortalTerminator, 0xE4);

class OrbWhirlWind;
class ThrowableTotalIndicator;
class ScreenClipper;
class Dove;

class BirdPortal : public BaseGameObject
{
public:
    EXPORT BirdPortal* ctor_4520A0(Path_BirdPortal* pTlv, int tlvInfo);
    EXPORT BaseGameObject* dtor_452230();
    EXPORT s16 IsScaredAway_4532E0();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;

    virtual s16 VPortalClipper(s16 bUnknown);
    virtual void VKillPortalClipper();
    virtual void VMudSaved();
    virtual BOOL VStateIs6();
    virtual void VGiveShrukull(s16 bPlaySound);
    virtual BOOL VStateIs16();
    virtual void VExitPortal();
    virtual void VIncreaseTimerAndKillPortalClipper();
    virtual BOOL VStateIs20();
    virtual void VGetMapChange(LevelIds* level, WORD* path, WORD* camera, CameraSwapEffects* screenChangeEffect, WORD* movieId);

private:
    EXPORT s16 VPortalClipper_4533E0(s16 bUnknown);
    EXPORT void VMudSaved_453830();
    EXPORT void VGetMapChange_453840(LevelIds* level, WORD* path, WORD* camera, CameraSwapEffects* screenChangeEffect, WORD* movieId);
    EXPORT void VIncreaseTimerAndKillPortalClipper_453810();
    EXPORT BOOL VStateIs20_453800();
    EXPORT void VExitPortal_453720();
    EXPORT BOOL VStateIs16_453710();
    EXPORT BOOL VStateIs6_453700();
    EXPORT void VKillPortalClipper_453570();
    EXPORT void VStopAudio_4532C0();
    EXPORT void VScreenChanged_4538E0();
    EXPORT void VGiveShrukull_4535A0(s16 bPlaySound);
    EXPORT void VUpdate_4523D0();
    EXPORT BirdPortal* Vdtor_453990(signed int flags);

public:
    PortalType field_10_portal_type;
    PortalSide field_12_side;
    enum class States : s16
    {
        State_0 = 0,
        State_1 = 1,
        State_2 = 2,
        State_3 = 3,
        State_4 = 4,
        State_5 = 5,
        State_6 = 6,
        State_7 = 7,
        State_8 = 8,
        State_9 = 9,
        State_10 = 10,
        State_11 = 11,
        State_12 = 12,
        State_13 = 13,
        State_14 = 14,
        State_15 = 15,
        State_16 = 16,
        State_17 = 17,
        State_18 = 18,
        State_19 = 19,
        State_20 = 20,
        State_21 = 21,
        State_22 = 22,
    };
    States field_14_state;
    s16 field_16;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20_exit_x;
    FP field_24_exit_y;
    FP field_28_ypos;
    int field_2C_tlvInfo;
    int field_30_timer;
    FP field_34_scale;
    s16 field_38_movie_id;
    s16 field_3A;
    BirdPortalTerminator* field_3C_pTerminator1;
    BirdPortalTerminator* field_40_pTerminator2;
    ScreenClipper* field_44_pScreenClipper;
    ScreenClipper* field_48_pScreenClipper;
    DynamicArrayT<Dove>* field_4C_pDovesArray;
    LevelIds field_50_dest_level;
    s16 field_52_dest_path;
    s16 field_54_dest_camera;
    s16 field_56_num_muds_for_shrykull;
    s16 field_58;
    s16 field_5A;
    ThrowableTotalIndicator* field_5C_pThrowableTotalIndicator;
    OrbWhirlWind* field_60_pOrbWhirlWind;
    LevelIds field_64_level;
    s16 field_66_path;
    int field_68_sfx_ret;
};
ALIVE_ASSERT_SIZEOF(BirdPortal, 0x6C);

}
