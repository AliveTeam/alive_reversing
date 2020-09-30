#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

class ZapLine;

class Shrykull : public BaseAliveGameObject
{
public:
    EXPORT Shrykull* ctor_463880();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_464280();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Shrykull* Vdtor_464300(signed int flags);

    EXPORT BaseGameObject* dtor_463990();

    virtual void VOnThrowableHit(BaseGameObject* ) override;

    virtual void VUpdate() override;

    bool CanKill(BaseAnimatedWithPhysicsGameObject* pObj);

    bool CanElectrocute(BaseGameObject* pObj) const;

    EXPORT void VUpdate_463AE0();

    enum class State : __int16
    {
        eTransform_0 = 0,
        eZapTargets_1 = 1,
        eDetransform_2 = 2,
        eFinish_3 = 3,
        eKillTargets_4 = 4,
    };
    State field_10C_state;
    __int16 field_10E_pad;
    int field_110_timer;
    int field_114_timer;
    ZapLine* field_118_zap_line;
    BaseAliveGameObject* field_11C_obj_being_zapped;
    __int16 field_120_bElectrocute;
    __int16 field_122_bResetRingTimer;
};
ALIVE_ASSERT_SIZEOF(Shrykull, 0x124);

END_NS_AO

