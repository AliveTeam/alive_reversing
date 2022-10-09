#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "PlatformBase.hpp"

class BaseBomb final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override;

    BaseBomb(FP x, FP y, s32 unused, FP scale);
private:
    void DealDamageRect(const PSX_RECT* pRect);

    FP mBombSpriteScale = {};
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xF8);
