#pragma once

#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "FixedPoint_common.hpp"
#include "Psx_common.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

class Shadow;
class BaseAliveGameObject;

enum class LevelIds : __int16;

class BaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
public:
    EXPORT BaseAnimatedWithPhysicsGameObject* ctor_417C10();

    EXPORT void Animation_Init_417FD0(int frameTableOffset, int maxW, int maxH, BYTE** ppAnimData, __int16 a6);

    virtual void VOnCollisionWith(PSX_Point /*xy*/, PSX_Point /*wh*/, DynamicArray* /*pObjList*/, int /*startingPointIdx*/, void* /*pFn*/) {}
    virtual PSX_RECT* VGetBoundingRect(PSX_RECT* /*pRect*/, int /*pointIdx*/) { return nullptr;  }
    virtual __int16 VIsObjNearby(int /*radius*/, BaseAliveGameObject* /*pOtherObj*/) { return 0; }
    
    virtual __int16 VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* /*pOther*/) { return 0; }

    virtual __int16 VIsFacingMe(BaseAliveGameObject* /*pOther*/) { return 0; }

    virtual __int16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* /*pOther*/) { return 0; }

    virtual void VStackOnObjectsOfType(unsigned __int16 /*typeToFind*/) {}


    Animation field_10_anim;
    FP field_A8_xpos;
    FP field_AC_ypos;
    __int16 field_B0_path;
    LevelIds field_B2_level;
    FP field_B4_velx;
    FP field_B8_vely;
    FP field_BC_scale;
    __int16 field_C0_r;
    __int16 field_C2_g;
    __int16 field_C4_b;
    __int16 field_C6;
    __int16 field_C8;
    __int16 field_CA;
    char field_CC;
    char field_CD;
    char field_CE;
    char field_CF;
    Shadow* field_D0_pShadow;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xD4);

END_NS_AO
