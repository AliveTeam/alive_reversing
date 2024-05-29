#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/MapWrapper.hpp"

enum class EReliveLevelIds : s16;

class BaseAliveGameObject;
class PathLine;
struct PSX_RECT;

namespace AO {


enum class BulletType : s16
{
    ePossessedSlig_0 = 0,
    eNormalBullet_1 = 1,
    eZBullet_2 = 2
};

class Bullet final : public ::BaseGameObject
{
public:
    Bullet(::BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, FP scale, s32 numberOfBullets);

    virtual void VUpdate() override;

    ::BaseAliveGameObject* ShootObject(PSX_RECT* pRect);
    static bool InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect);

private:
    void PlayBulletSounds(s16 volume);

public:
    BulletType mBulletType = BulletType::ePossessedSlig_0;
    PathLine* mLine = nullptr;
    FP mXPos = {};
    FP mYPos = {};
    FP mXDistance = {};
    EReliveLevelIds mBulletLevel = EReliveLevelIds::eNone;
    s16 mBulletPath = 0;
    FP mSpriteScale = {};
    ::BaseAliveGameObject* mBulletParent = nullptr;
    s16 mNumberOfBullets = 0;
};

} // namespace AO
