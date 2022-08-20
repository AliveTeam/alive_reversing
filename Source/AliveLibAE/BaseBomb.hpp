#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "PlatformBase.hpp"
#include "Path.hpp"

class BaseBomb final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override;

    BaseBomb(FP x, FP y, s32 unused, FP scale);

    void DealDamageRect(const PSX_RECT* pRect);

private:
    FP mBombSpriteScale = {};
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xF8);
