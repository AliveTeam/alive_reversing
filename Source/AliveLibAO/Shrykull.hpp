#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

class ZapLine;

class Shrykull final : public BaseAliveGameObject
{
public:
    Shrykull();
    ~Shrykull();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject*) override;
    virtual void VUpdate() override;

    bool CanKill(BaseAnimatedWithPhysicsGameObject* pObj);
    bool CanElectrocute(BaseGameObject* pObj) const;

    enum class State : s16
    {
        eTransform_0 = 0,
        eZapTargets_1 = 1,
        eDetransform_2 = 2,
        eFinish_3 = 3,
        eKillTargets_4 = 4,
    };
    State field_10C_state = State::eTransform_0;
    s32 field_110_timer = 0;
    s32 field_114_timer = 0;
    ZapLine* field_118_zap_line = nullptr;
    BaseAliveGameObject* field_11C_obj_being_zapped = nullptr;
    s16 field_120_bElectrocute = 0;
    s16 field_122_bResetRingTimer = 0;
};

} // namespace AO
