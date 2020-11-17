#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "DynamicArray.hpp"

namespace AO {

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
    PortalSide field_18_side;
    LevelIds field_1A_dest_level;
    __int16 field_1C_dest_path;
    __int16 field_1E_dest_camera;
    __int16 field_20_scale;
    __int16 field_22_movie_id;
    PortalType field_24_portal_type;
    __int16 field_26_num_muds_for_shrykul;
};
ALIVE_ASSERT_SIZEOF(Path_BirdPortal, 0x28);

struct Path_BirdPortalExit : public Path_TLV
{
    PortalSide field_18_side;
    __int16 field_1A_scale;
};
ALIVE_ASSERT_SIZEOF(Path_BirdPortalExit, 0x1C);

class BirdPortalTerminator : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BirdPortalTerminator* ctor_451F70(FP xpos, FP ypos, FP scale, PortalType portalType);

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
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

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BirdPortal* Vdtor_453990(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4523D0();

    EXPORT __int16 IsScaredAway_4532E0();

    EXPORT void VGiveShrukull_4535A0(__int16 bPlaySound);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4538E0();

    virtual void VStopAudio() override;

    EXPORT void VStopAudio_4532C0();

    EXPORT void Vsub_453570();

    EXPORT BOOL VStateIs6_453700();

    EXPORT BOOL VStateIs16_453710();

    EXPORT void VExitPortal_453720();

    EXPORT BOOL VStateIs20_453800();

    EXPORT void Vsub_453810();

    EXPORT void VMudSaved_453830();

    EXPORT void VGetMapChange_453840(LevelIds* level, WORD* path, WORD* camera, CameraSwapEffects* screenChangeEffect, WORD* movieId);

    EXPORT __int16 Vsub_4533E0(__int16 bUnknown);

    PortalType field_10_portal_type;
    PortalSide field_12_side;
    enum class States : __int16
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
    __int16 field_16;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20_exit_x;
    FP field_24_exit_y;
    FP field_28_ypos;
    int field_2C_tlvInfo;
    int field_30_timer;
    FP field_34_scale;
    __int16 field_38_movie_id;
    __int16 field_3A;
    BirdPortalTerminator* field_3C_pTerminator1;
    BirdPortalTerminator* field_40_pTerminator2;
    ScreenClipper* field_44_pScreenClipper;
    ScreenClipper* field_48_pScreenClipper;
    DynamicArrayT<Dove>* field_4C_pDovesArray;
    LevelIds field_50_dest_level;
    __int16 field_52_dest_path;
    __int16 field_54_dest_camera;
    __int16 field_56_num_muds_for_shrykul;
    __int16 field_58;
    __int16 field_5A;
    ThrowableTotalIndicator* field_5C;
    OrbWhirlWind* field_60_pOrbWhirlWind;
    LevelIds field_64_level;
    __int16 field_66_path;
    int field_68_sfx_ret;
};
ALIVE_ASSERT_SIZEOF(BirdPortal, 0x6C);

}
