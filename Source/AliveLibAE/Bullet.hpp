#pragma once

#include "../relive_lib/BaseGameObject.hpp"
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
    BaseAliveGameObject* ShootObject(PSX_RECT* pRect);
    void PlayBulletSounds(s16 volume);

public:
    BulletType field_20_type = BulletType::eSligPossessedOrUnderGlukkonCommand_0;

private:
    s16 field_22_unused = 0;
    PathLine* field_24_pLine = nullptr;
    FP field_28_xpos = {};

public:
    FP field_2C_ypos = {};

public:
    FP field_30_x_distance = {};

private:
    s32 field_34_unused = 0;
    EReliveLevelIds field_38_level = EReliveLevelIds::eNone;
    s16 field_3A_path = 0;
    FP field_3C_scale = {};
    BaseAliveGameObject* field_40_pParent = nullptr;
    s16 field_44_number_of_bullets = 0;
};
ALIVE_ASSERT_SIZEOF(Bullet, 0x48);
