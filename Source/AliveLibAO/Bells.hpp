#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"

START_NS_AO

enum class BellType
{
    eType_0 = 0,
    eType_1 = 1,
    eType_2 = 2,
};

class Bells : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Bells* ctor_40A650(BellType bellType, FP xpos, FP ypos, FP scale);

    EXPORT BaseGameObject* dtor_40A760();


    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Bells* Vdtor_40AB00(signed int flags);

    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;

    int field_E4;
    unsigned __int16 field_E8;
    unsigned __int16 field_EA_sound;
    int field_EC_timer;
    char field_F0_timer;
    char field_F1;
    char field_F2;
    char field_F3;
    int field_F4_timer;
};
ALIVE_ASSERT_SIZEOF(Bells, 0xF8);

END_NS_AO

