#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

class BaseAliveGameObject;

enum class BulletType : s16
{
    ePossessedSlig_0 = 0,
    eNormalBullet_1 = 1,
    eZBullet_2 = 2
};

enum class LevelIds : s16;

class Bullet : public BaseGameObject
{
public:
    EXPORT Bullet* ctor_409380(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, s32 a7, FP scale, s16 a9);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;

private:
    EXPORT void VUpdate_408E30();

public:
    EXPORT BaseAliveGameObject* ShootObject_409400(PSX_RECT* pRect);

    static bool InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect);

    BulletType field_10_type;
    s16 field_12;
    PathLine* field_14_pLine;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20;
    s32 field_24;
    LevelIds field_28_level;
    s16 field_2A_path;
    FP field_2C_scale;
    BaseAliveGameObject* field_30_pParent;
    s16 field_34;
    s16 field_36;
};
ALIVE_ASSERT_SIZEOF(Bullet, 0x38);

} // namespace AO
