#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"
#include "BitField.hpp"

START_NS_AO

void BaseAliveGameObject_ForceLink();


EXPORT FP CC Grid_Scale_41FA30(FP scale);

struct Path_TLV;
class LiftPoint;
class PathLine;

enum class CameraPos : __int16;

enum Flags_10A
{
    e10A_Bit1 = 0x1,
    e10A_Bit2 = 0x2,
    e10A_Bit3 = 0x4,
    e10A_Bit4 = 0x8,
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

    EXPORT BaseAliveGameObject* dtor_401000();

    // TODO: Non empty so standalone builds

    // TODO: Part of base?
    virtual void VOnPickUpOrSlapped() {}

    // TODO: Part of base?
    virtual void VOnThrowableHit(BaseGameObject* /*pFrom*/) {}

    virtual void VUnPosses() {}
    virtual void VPossessed() {}
    virtual void VSetMotion(__int16 /*state*/) {}
    virtual void VSetXSpawn(__int16 /*camWorldX*/, int /*screenXPos*/) {}
    virtual void VSetYSpawn(int /*camWorldY*/, __int16 /*bLeft*/) {}
    virtual void VOnPathTransition(__int16 /*camWorldX*/, int /*camWorldY*/, CameraPos /*direction*/) {}
    virtual __int16 VTakeDamage(BaseGameObject* /*pFrom*/) { return 0; }
    virtual void VOn_TLV_Collision(Path_TLV* /*pTlv*/) {}
    virtual void VCheckCollisionLineStillValid(int /*distance*/) {}
    virtual void VOnPickUpOrSlapped2() {}

    EXPORT void VCheckCollisionLineStillValid_401A90(int distance);

    EXPORT __int16 MapFollowMe_401D30(__int16 snapToGrid);

    int field_D4_pad[4];
    __int16 field_E4;
    __int16 field_E6_last_state;
    int field_E8_last_frame;
    __int16 field_EC_oldY;
    __int16 field_EE_type;
    Path_TLV* field_F0_pTlv;
    PathLine* field_F4_pLine;
    LiftPoint* field_F8_pLiftPoint;
    __int16 field_FC_state; //todo enum
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
