#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

START_NS_AO

class BaseAliveGameObject;

enum class LevelIds : __int16;

class Bullet : public BaseGameObject
{
public:
    EXPORT Bullet* ctor_409380(BaseAliveGameObject* pParent, __int16 type, FP xpos, FP ypos, FP xDist, int a7, FP scale, __int16 a9);

    virtual BaseGameObject* VDestructor(signed int flags) override;


    __int16 field_10_type;
    __int16 field_12;
    int field_14;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20;
    int field_24;
    LevelIds field_28_level;
    __int16 field_2A_path;
    FP field_2C_scale;
    BaseAliveGameObject* field_30_pParent;
    __int16 field_34;
    __int16 field_36;
};
ALIVE_ASSERT_SIZEOF(Bullet, 0x38);

END_NS_AO

