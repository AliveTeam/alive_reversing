#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"
#include "BitField.hpp"
#include "Psx.hpp"

START_NS_AO

void BaseAliveGameObject_ForceLink();


EXPORT FP CC ScaleToGridSize_41FA30(FP scale);

struct Path_TLV;
class PlatformBase;
class PathLine;

enum class CameraPos : __int16;

enum Flags_10A
{
    e10A_Bit1_Can_Be_Possessed = 0x1,
    e10A_Bit2 = 0x2,
    e10A_Bit3 = 0x4,
    e10A_Bit4_SetOffExplosives = 0x8,
    e10A_Bit5 = 0x10,
    e10A_Bit6 = 0x20,
    e10A_Bit7 = 0x40,
    e10A_Bit8 = 0x80,
    e10A_Bit9 = 0x100,
    e10A_Bit10 = 0x200,
    e10A_Bit11 = 0x400,
    e10A_Bit12 = 0x800,
    e10A_Bit13 = 0x1000,
    e10A_Bit14 = 0x2000,
    e10A_Bit15 = 0x4000,
    e10A_Bit16 = 0x8000,
};

class BaseAliveGameObject : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BaseAliveGameObject* ctor_401090();

    EXPORT BaseGameObject* dtor_401000();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_402540(signed int flags);
    // TODO: Non empty so standalone builds
    
    // TODO: HACK call the impls here
    virtual void VUnPosses() {}
    virtual void VPossessed() {}
    virtual void VSetMotion(__int16 /*state*/) {}
    virtual void VSetXSpawn(__int16 /*camWorldX*/, int /*screenXPos*/) {}
    virtual void VSetYSpawn(int /*camWorldY*/, __int16 /*bLeft*/) {}
    virtual void VOnPathTransition(__int16 /*camWorldX*/, int /*camWorldY*/, CameraPos /*direction*/) {}
    virtual __int16 VTakeDamage(BaseGameObject* /*pFrom*/) { return 0; }
    virtual void VOn_TLV_Collision(Path_TLV* /*pTlv*/) {}
    virtual void VCheckCollisionLineStillValid(int /*distance*/) {}
    virtual void VOnTrapDoorOpen() {}

    

    EXPORT void VCheckCollisionLineStillValid_401A90(int distance);

    EXPORT __int16 MapFollowMe_401D30(__int16 snapToGrid);

    EXPORT void SetActiveCameraDelayedFromDir_401C90();

    EXPORT signed __int16 OnTrapDoorIntersection_401C10(PlatformBase* pPlatform);

    EXPORT __int16 WallHit_401930(FP offY, FP offX);

    EXPORT __int16 InAirCollision_4019C0(PathLine** ppLine, FP* hitX, FP* hitY, FP vely);

    EXPORT static void CC OnResourceLoaded_4019A0(BaseAliveGameObject* ppRes);

    EXPORT void VSetXSpawn_401150(__int16 camWorldX, int screenXPos);

    EXPORT void VSetYSpawn_401380(int camWorldY, __int16 bLeft);

    EXPORT __int16 IsBeeSwarmChasingMe_4022B0();

    EXPORT void VSetMotion_402520(__int16 state);

    EXPORT void sub_4020D0();

    EXPORT static BaseGameObject* CC FindObjectOfType_418280(Types typeToFind, FP xpos, FP ypos);

    FP_RECT field_D4_collection_rect;
    __int16 field_E4;
    __int16 field_E6_last_anim_frame;
    FP field_E8_LastLineYPos;
    __int16 field_EC_oldY;
    __int16 field_EE_type;
    Path_TLV* field_F0_pTlv;
    PathLine* field_F4_pLine;
    PlatformBase* field_F8_pLiftPoint;
    __int16 field_FC_current_motion;
    __int16 field_FE_next_state;
    FP field_100_health;
    __int16 field_104_pending_resource_count;
    __int16 field_106_shot;
    __int16 field_108_bMotionChanged;
    BitField16<Flags_10A> field_10A_flags;
};
ALIVE_ASSERT_SIZEOF(BaseAliveGameObject, 0x10C);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_4FC8A0);

END_NS_AO
