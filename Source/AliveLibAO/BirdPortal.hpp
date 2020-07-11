#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "DynamicArray.hpp"

START_NS_AO

enum class LevelIds : __int16;

struct Path_BirdPortal : public Path_TLV
{
    __int16 field_18_side;
    LevelIds field_1A_dest_level;
    __int16 field_1C_dest_path;
    __int16 field_1E_dest_camera;
    __int16 field_20_scale;
    __int16 field_22_movie_id;
    __int16 field_24_portal_type;
    __int16 field_26_num_muds_for_shrykul;
};
ALIVE_ASSERT_SIZEOF(Path_BirdPortal, 0x28);

struct Path_BirdPortalExit : public Path_TLV
{
    __int16 field_18_side;
    __int16 field_1A_scale;
};
ALIVE_ASSERT_SIZEOF(Path_BirdPortalExit, 0x1C);

class BirdPortalTerminator : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BirdPortalTerminator* ctor_451F70(FP xpos, FP ypos, FP scale, int portalType);

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
};
ALIVE_ASSERT_SIZEOF(BirdPortalTerminator, 0xE4);

class OrbWhirlWind;
class ThrowableTotalIndicator;
class ScreenClipper;

class BirdPortal : public BaseGameObject
{
public:
    EXPORT BirdPortal* ctor_4520A0(Path_BirdPortal* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_452230();


    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BirdPortal* Vdtor_453990(signed int flags);

    __int16 field_10_portal_type;
    __int16 field_12_side;
    __int16 field_14_state;
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
    BirdPortalTerminator* field_40_pTerminator;
    ScreenClipper* field_44_pScreenClipper;
    ScreenClipper* field_48_pScreenClipper;
    DynamicArrayT<BaseAliveGameObject>* field_4C_pDovesArray;
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

END_NS_AO
