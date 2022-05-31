#pragma once

#include "BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"

class PathLine;

enum class LevelIds : s16;

class BaseAliveGameObject;

enum class BulletType : s16
{
    eSligPossessedOrUnderGlukkonCommand_0 = 0,
    ePossessedSligZBullet_1 = 1,
    eNormalBullet_2 = 2,
    eZBullet_3 = 3
};

class Bullet final : public BaseGameObject
{
public:
    Bullet(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, s32 unused, FP scale, s32 numberOfBullets);
    

    virtual void VUpdate() override;


    static bool InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect);

private:
    BaseAliveGameObject* ShootObject_414630(PSX_RECT* pRect);

    void vUpdate_413560();

    void PlayBulletSounds(s16 volume);

public:
    BulletType field_20_type;

private:
    s16 field_22_unused;
    PathLine* field_24_pLine;
    FP field_28_xpos;

public:
    FP field_2C_ypos;

public:
    FP field_30_x_distance;

private:
    s32 field_34_unused;
    LevelIds field_38_level;
    s16 field_3A_path;
    FP field_3C_scale;
    BaseAliveGameObject* field_40_pParent;
    s16 field_44_number_of_bullets;
    s16 field_46_padding;
};
ALIVE_ASSERT_SIZEOF(Bullet, 0x48);
