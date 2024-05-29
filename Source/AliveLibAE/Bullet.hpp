#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/Psx.hpp"

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
    const FP& xpos() const { return mXPos; }
    const FP& ypos() const { return mYPos; }
    const FP& XDistance() const { return mXDistance; }

private:
    BaseAliveGameObject* ShootObject(PSX_RECT* pRect);
    void PlayBulletSounds(s16 volume);

public:
    BulletType mBulletType = BulletType::eSligPossessedOrUnderGlukkonCommand_0;

private:
    PathLine* mLine = nullptr;
    FP mXPos = {};
    FP mYPos = {};
    BaseAliveGameObject* mBulletParent = nullptr;
    FP mSpriteScale = {};
    s32 mNumberOfBullets = 0;
    FP mXDistance = {};
    EReliveLevelIds mBulletLevel = EReliveLevelIds::eNone;
    s16 mBulletPath = 0;
};
