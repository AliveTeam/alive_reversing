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
    Bullet(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, FP scale, s32 numberOfBullets);
    
    virtual void VUpdate() override;
    static bool InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect);

private:
    BaseAliveGameObject* ShootObject(PSX_RECT* pRect);
    void PlayBulletSounds(s16 volume);

public:
    BulletType mBulletType = BulletType::eSligPossessedOrUnderGlukkonCommand_0;

private:
    PathLine* mLine = nullptr;
    FP mXPos = {};

public:
    FP mYPos = {};
    FP mXDistance = {};

private:
    EReliveLevelIds mBulletLevel = EReliveLevelIds::eNone;
    s16 mBulletPath = 0;
    FP mSpriteScale = {};
    BaseAliveGameObject* mBulletParent = nullptr;
    s16 mNumberOfBullets = 0;
};
ALIVE_ASSERT_SIZEOF(Bullet, 0x48);
