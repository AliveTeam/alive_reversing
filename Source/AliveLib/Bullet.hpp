#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"

struct PathLine;

enum class LevelIds : __int16;

class BaseAliveGameObject;

enum class BulletType : __int16
{
    Type_0 = 0,
    Type_1 = 1,
    Type_2 = 2,
    ZBullet_3 = 3,
};

class Bullet : public BaseGameObject
{
public:
    EXPORT Bullet* ctor_414540(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, int a7, FP scale, __int16 a9);
    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;


    static bool InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect);
private:
    EXPORT Bullet* vdtor_4145E0(signed int flags);

    EXPORT BaseAliveGameObject* ShootObject_414630(PSX_RECT* pRect);

    EXPORT void vUpdate_413560();

public:
    BulletType field_20_type;
private:
    __int16 field_22;
    PathLine* field_24_pLine;
    FP field_28_xpos;
public:
    FP field_2C_ypos;
public:
    FP field_30;
private:
    int field_34;
    LevelIds field_38_level;
    __int16 field_3A_path;
    FP field_3C_scale;
    BaseAliveGameObject* field_40_pParent;
    __int16 field_44;
    __int16 field_46;
};
ALIVE_ASSERT_SIZEOF(Bullet, 0x48);
