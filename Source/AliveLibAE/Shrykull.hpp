#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

class Shrykull final : public BaseAliveGameObject
{
public:
    Shrykull();
    ~Shrykull();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject*) override
    {
        // Empty
    }

private:
    static s16 CanElectrocute(BaseGameObject* pObj);
    static s16 CanKill(BaseAnimatedWithPhysicsGameObject* pObj);

private:
    enum class State : s16
    {
        eTransform_0 = 0,
        eZapTargets_1 = 1,
        eDetransform_2 = 2,
        eFinish_3 = 3,
        eKillTargets_4 = 4,
    };
    State field_118_state = State::eTransform_0;
    s32 field_11C_timer = 0;
    s32 field_120_timer = 0;
    Guid field_124_zap_line_id;
    Guid field_128_obj_being_zapped_id;
    s16 field_12C_bElectrocute = 0;
    s16 field_12E_bResetRingTimer = 0;
};
ALIVE_ASSERT_SIZEOF(Shrykull, 0x130);
