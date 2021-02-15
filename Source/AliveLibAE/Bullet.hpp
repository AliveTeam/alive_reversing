#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"

class PathLine;

enum class LevelIds : __int16;

class BaseAliveGameObject;

enum class BulletType : __int16
{
    eSligPossessedOrUnderGlukkonCommand_0 = 0,
    ePossessedSligZBullet_1 = 1,
    eNormalBullet_2 = 2,
    ZBullet_3 = 3
};

class Bullet : public BaseGameObject
{
public:
    EXPORT Bullet* ctor_414540(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, int unused, FP scale, __int16 numberOfBullets);
    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;


    static bool InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect);
private:
    EXPORT Bullet* vdtor_4145E0(signed int flags);

    EXPORT BaseAliveGameObject* ShootObject_414630(PSX_RECT* pRect);

    EXPORT void vUpdate_413560();

    void PlayBulletSounds(short volume);
public:
    BulletType field_20_type;
private:
    __int16 field_22_unused;
    PathLine* field_24_pLine;
    FP field_28_xpos;
public:
    FP field_2C_ypos;
public:
    FP field_30_x_distance;
private:
    int field_34_unused;
    LevelIds field_38_level;
    __int16 field_3A_path;
    FP field_3C_scale;
    BaseAliveGameObject* field_40_pParent;
    __int16 field_44_number_of_bullets;
    __int16 field_46_padding;
};
ALIVE_ASSERT_SIZEOF(Bullet, 0x48);
