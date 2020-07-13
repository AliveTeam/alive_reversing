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

    __int16 field_10C_state;
    __int16 field_10E;
    int field_110;
    int field_114;
    ZapLine* field_118;
    BaseAliveGameObject* field_11C;
    __int16 field_120;
    __int16 field_122_bUnknown;
};
ALIVE_ASSERT_SIZEOF(Shrykull, 0x124);

END_NS_AO

