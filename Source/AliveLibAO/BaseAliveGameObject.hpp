#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

void BaseAliveGameObject_ForceLink();


struct Path_TLV;
class LiftPoint;

enum class CameraPos : __int16;

class BaseAliveGameObject : public BaseAnimatedWithPhysicsGameObject
{
public:
    // TODO: Part of base?
    virtual void VOnPickUpOrSlapped();

    // TODO: Part of base?
    virtual void VOnThrowableHit(BaseGameObject* pFrom);

    virtual void VUnPosses();
    virtual void VPossessed();
    virtual void VSetMotion(__int16 state);
    virtual void VSetXSpawn(__int16 camWorldX, int screenXPos);
    virtual void VSetYSpawn(int camWorldY, __int16 bLeft);
    virtual void VOnPathTransition(__int16 camWorldX, int camWorldY, CameraPos direction);
    virtual __int16 VTakeDamage(BaseGameObject* pFrom);
    virtual void VOn_TLV_Collision(Path_TLV* pTlv);
    virtual void VCheckCollisionLineStillValid(int distance);
    virtual void VOnPickUpOrSlapped2();

    EXPORT void VCheckCollisionLineStillValid_401A90(int distance);

    int field_D4_pad[4];
    __int16 field_E4;
    __int16 field_E6_last_state;
    int field_E8_last_frame;
    __int16 field_EC_oldY;
    __int16 field_EE_type;
    Path_TLV* field_F0_pTlv;
    int field_F4_pLine;
    LiftPoint* field_F8_pLiftPoint;
    __int16 field_FC_state;
    __int16 field_FE_next_state;
    int field_100_health;
    __int16 field_104_pending_resource_count;
    __int16 field_106_shot;
    __int16 field_108_bMotionChanged;
    __int16 field_10A_flags;
};
ALIVE_ASSERT_SIZEOF(BaseAliveGameObject, 0x10C);



END_NS_AO
