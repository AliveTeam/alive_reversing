#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

class Shrykull : public BaseAliveGameObject
{
public:
    EXPORT Shrykull* ctor_4AEA20();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VScreenChanged() override;

private:
    EXPORT void dtor_4AEBC0();

    EXPORT Shrykull* vdtor_4AEB90(signed int flags);

    EXPORT void vScreenChange_4AF650();


private:
    __int16 field_118_state;
    __int16 field_11A_pad;
    int field_11C_timer;
    int field_120_timer;
    int field_124_zap_line_id;
    int field_128_obj_being_zapped_id;
    __int16 field_12C_bElectrocute;
    __int16 field_12E_bUnknown;
};
ALIVE_ASSERT_SIZEOF(Shrykull, 0x130);
