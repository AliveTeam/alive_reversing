#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "EffectBase.hpp"
#include "../relive_lib/Layer.hpp"

class DeathFadeOut final : public EffectBase
{
public:
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    DeathFadeOut(Layer layer, s32 direction, bool destroyOnDone, s32 speed, TPageAbr abr);
    void Init(Layer layer, s32 direction, bool destroyOnDone, s32 speed);

private:
    s16 field_78_current_fade_rgb = 0;
    s16 field_7A_speed = 0;
    s16 field_7C_direction = 0;

public:
    s16 field_7E_bDone = 0;

private:
    s16 field_80_destroy_on_done = 0;
    s16 field_82 = 0;
};
ALIVE_ASSERT_SIZEOF(DeathFadeOut, 0x84);
