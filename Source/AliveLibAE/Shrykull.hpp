#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

class Shrykull final : public BaseAliveGameObject
{
public:
    Shrykull();
    ~Shrykull();

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
    State field_118_state;
    s32 field_11C_timer;
    s32 field_120_timer;
    s32 field_124_zap_line_id;
    s32 field_128_obj_being_zapped_id;
    s16 field_12C_bElectrocute;
    s16 field_12E_bResetRingTimer;
};
ALIVE_ASSERT_SIZEOF(Shrykull, 0x130);
