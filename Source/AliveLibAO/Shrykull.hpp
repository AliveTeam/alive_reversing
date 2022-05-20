#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

class ZapLine;

class Shrykull final : public BaseAliveGameObject
{
public:
    EXPORT Shrykull* ctor_463880();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_464280();

    

    EXPORT Shrykull* Vdtor_464300(s32 flags);

    EXPORT BaseGameObject* dtor_463990();

    virtual void VOnThrowableHit(BaseGameObject*) override;

    virtual void VUpdate() override;

    bool CanKill(BaseAnimatedWithPhysicsGameObject* pObj);

    bool CanElectrocute(BaseGameObject* pObj) const;

    EXPORT void VUpdate_463AE0();

    enum class State : s16
    {
        eTransform_0 = 0,
        eZapTargets_1 = 1,
        eDetransform_2 = 2,
        eFinish_3 = 3,
        eKillTargets_4 = 4,
    };
    State field_10C_state;
    s16 field_10E_pad;
    s32 field_110_timer;
    s32 field_114_timer;
    ZapLine* field_118_zap_line;
    BaseAliveGameObject* field_11C_obj_being_zapped;
    s16 field_120_bElectrocute;
    s16 field_122_bResetRingTimer;
};
ALIVE_ASSERT_SIZEOF(Shrykull, 0x124);

} // namespace AO
