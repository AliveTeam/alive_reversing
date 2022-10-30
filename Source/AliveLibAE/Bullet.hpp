#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"

class PathLine;

enum class LevelIds : s16;

class IBaseAliveGameObject;

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
    Bullet(IBaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, FP scale, s32 numberOfBullets);
    
    virtual void VUpdate() override;
    static bool InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect);

private:
    IBaseAliveGameObject* ShootObject(PSX_RECT* pRect);
    void PlayBulletSounds(s16 volume);

public:
    BulletType mBulletType = BulletType::eSligPossessedOrUnderGlukkonCommand_0;
    FP mYPos = {};
    FP mXDistance = {};

private:
    PathLine* mLine = nullptr;
    FP mXPos = {};
    EReliveLevelIds mBulletLevel = EReliveLevelIds::eNone;
    s16 mBulletPath = 0;
    FP mSpriteScale = {};
    IBaseAliveGameObject* mBulletParent = nullptr;
    s16 mNumberOfBullets = 0;
};
