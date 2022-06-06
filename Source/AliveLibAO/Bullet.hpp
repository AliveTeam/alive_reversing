#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

class BaseAliveGameObject;

enum class BulletType : s16
{
    ePossessedSlig_0 = 0,
    eNormalBullet_1 = 1,
    eZBullet_2 = 2
};

class Bullet final : public BaseGameObject
{
public:
    Bullet(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, s32 unused, FP scale, s32 numberOfBullets);

    virtual void VUpdate() override;

    BaseAliveGameObject* ShootObject(PSX_RECT* pRect);
    static bool InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect);

private:
    void PlayBulletSounds(s16 volume);

public:
    BulletType field_10_type;
    s16 field_12_unused;
    PathLine* field_14_pLine;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20_x_distance;
    s32 field_24_unused;
    EReliveLevelIds field_28_level;
    s16 field_2A_path;
    FP field_2C_scale;
    BaseAliveGameObject* field_30_pParent;
    s16 field_34_number_of_bullets;
    s16 field_36;
};
ALIVE_ASSERT_SIZEOF(Bullet, 0x38);

} // namespace AO
