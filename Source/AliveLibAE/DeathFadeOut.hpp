#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

class DeathFadeOut final : public EffectBase
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT DeathFadeOut* ctor_427030(Layer layer, s16 direction, s16 destroyOnDone, s16 speed, TPageAbr abr);
    EXPORT void Init_427140(Layer layer, s16 direction, s16 destroyOnDone, s16 speed);

private:
    EXPORT void dtor_427120();
    EXPORT DeathFadeOut* vdtor_4270F0(s32 flags);
    EXPORT void Update_4271C0();
    EXPORT void vRender_427210(PrimHeader** ppOt);

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
