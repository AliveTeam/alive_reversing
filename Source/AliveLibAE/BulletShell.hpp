#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class PathLine;

class BulletShell final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    BulletShell(FP xpos, FP ypos, s16 direction, FP scale);
    ~BulletShell();

    virtual void VUpdate() override;

private:
    void vUpdate_4AD550();

private:
    PathLine* field_F4_pLine;
    s16 field_F8_padding;
    s16 field_FA_padding;
    s16 field_FC_hitCount;
    s16 field_FE_padding;
    FP field_100_speed;
};
ALIVE_ASSERT_SIZEOF(BulletShell, 0x104);
