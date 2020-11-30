#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

class BaseAliveGameObject;

enum class BulletType : __int16
{
    Type_0 = 0,
    Type_1 = 1,
    ZBullet_2 = 2
};

enum class LevelIds : __int16;

class Bullet : public BaseGameObject
{
public:
    EXPORT Bullet* ctor_409380(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, int a7, FP scale, __int16 a9);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;

private:
    EXPORT void VUpdate_408E30();

public:
    EXPORT BaseAliveGameObject* ShootObject_409400(PSX_RECT* pRect);

    static bool InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect);

    BulletType field_10_type;
    __int16 field_12;
    PathLine* field_14_pLine;
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

}

