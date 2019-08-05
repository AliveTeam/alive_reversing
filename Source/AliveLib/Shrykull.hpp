#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

class Shrykull : public BaseAliveGameObject
{
public:
    EXPORT Shrykull* ctor_4AEA20();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

private:
    EXPORT void dtor_4AEBC0();

    EXPORT Shrykull* vdtor_4AEB90(signed int flags);

    EXPORT void vScreenChange_4AF650();

    EXPORT static __int16 CCSTD Electrocute_4AED10(BaseGameObject* pObj);

    EXPORT static __int16 CCSTD CanKill_4AEC50(BaseAnimatedWithPhysicsGameObject* pObj);

    EXPORT void vUpdate_4AEDE0();


private:
    enum class State : __int16
    {
        eState0 = 0,
        eState1 = 1,
        eState2 = 2,
        eState3 = 3,
        eState4 = 4,
    };
    State field_118_state;
    __int16 field_11A_pad;
    int field_11C_timer;
    int field_120_timer;
    int field_124_zap_line_id;
    int field_128_obj_being_zapped_id;
    __int16 field_12C_bElectrocute;
    __int16 field_12E_bUnknown;
};
ALIVE_ASSERT_SIZEOF(Shrykull, 0x130);
