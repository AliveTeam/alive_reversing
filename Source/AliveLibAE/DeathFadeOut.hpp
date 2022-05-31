#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

class DeathFadeOut final : public EffectBase
{
public:
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    DeathFadeOut(Layer layer, s32 direction, bool destroyOnDone, s32 speed, TPageAbr abr);
    void Init_427140(Layer layer, s32 direction, bool destroyOnDone, s32 speed);

private:
    s16 field_78_current_fade_rgb;
    s16 field_7A_speed;
    s16 field_7C_direction;

public:
    s16 field_7E_bDone;

private:
    s16 field_80_destroy_on_done;
    s16 field_82;
};
ALIVE_ASSERT_SIZEOF(DeathFadeOut, 0x84);
