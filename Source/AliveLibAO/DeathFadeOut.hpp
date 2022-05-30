#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

namespace AO {

class DeathFadeOut final : public EffectBase
{
public:
    DeathFadeOut(Layer layer, s32 direction, bool destroyOnDone, s32 speed, TPageAbr abr);

    void Init(Layer layer, s16 direction, s16 destroyOnDone, s32 speed);
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;


    s16 field_68_current_fade_rgb;
    s16 field_6A_speed;
    s16 field_6C_direction;
    s16 field_6E_bDone;
    s16 field_70_destroy_on_done;
    s16 field_72;
};
ALIVE_ASSERT_SIZEOF(DeathFadeOut, 0x74);

} // namespace AO
