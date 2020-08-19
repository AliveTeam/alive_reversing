#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Animation.hpp"
#include "PlatformBase.hpp"

START_NS_AO

class Map;

class LiftPoint : public PlatformBase
{
public:
    EXPORT LiftPoint* ctor_434710(unsigned __int8* pTlv, Map* pPath, int tlvInfo);

    __int16 field_12C_bMoving;
    __int16 field_12E;
    __int16 field_130;
    __int16 field_132;
    int field_134_pRope2;
    int field_138_pRope1;
    Animation field_13C_lift_wheel;
    Animation field_1D4_pulley_anim;
    int field_26C_pulley_xpos;
    int field_270_pulley_ypos;
    int field_274;
    char field_278;
    char field_279;
    char field_27A;
    char field_27B;
};

ALIVE_ASSERT_SIZEOF(LiftPoint, 0x27C);

END_NS_AO

