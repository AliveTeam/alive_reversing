#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/FixedPoint_common.hpp"
#include "../relive_lib/MapWrapper.hpp"

enum class EReliveLevelIds : s16;

class IBaseAliveGameObject;
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
    Bullet(IBaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, FP scale, s32 numberOfBullets);

    virtual void VUpdate() override;

    IBaseAliveGameObject* ShootObject(PSX_RECT* pRect);
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
    IBaseAliveGameObject* mBulletParent = nullptr;
    s16 mNumberOfBullets = 0;
};

} // namespace AO
