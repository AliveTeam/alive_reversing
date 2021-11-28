#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

class Shrykull final : public BaseAliveGameObject
{
public:
    EXPORT Shrykull* ctor_4AEA20();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual void VOnThrowableHit(BaseGameObject*) override
    { }

private:
    EXPORT void dtor_4AEBC0();

    EXPORT Shrykull* vdtor_4AEB90(s32 flags);

    EXPORT void vScreenChange_4AF650();

    EXPORT static s16 CCSTD CanElectrocute_4AED10(BaseGameObject* pObj);

    EXPORT static s16 CCSTD CanKill_4AEC50(BaseAnimatedWithPhysicsGameObject* pObj);

    EXPORT void vUpdate_4AEDE0();


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
    s16 field_11A_padding;
    s32 field_11C_timer;
    s32 field_120_timer;
    s32 field_124_zap_line_id;
    s32 field_128_obj_being_zapped_id;
    s16 field_12C_bElectrocute;
    s16 field_12E_bResetRingTimer;
};
ALIVE_ASSERT_SIZEOF(Shrykull, 0x130);
