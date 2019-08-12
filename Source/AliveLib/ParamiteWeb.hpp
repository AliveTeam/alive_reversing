#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class ParamiteWeb : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParamiteWeb* ctor_4E1840(FP xpos, __int16 bottom, __int16 top, FP scale);

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    EXPORT ParamiteWeb* vdtor_4E1AF0(signed int flags);

    EXPORT void dtor_4E1B20();

    EXPORT void vUpdate_4E1F40();


    EXPORT void vScreenChanged_4E1F80();


public:
    __int16 field_F4;
    __int16 field_F6;
    __int16 field_F8_ttl;
    __int16 field_FA_ttl_remainder;
    BYTE** field_FC;
    AnimationUnknown* field_100;
    __int16 field_104_bEnabled;
    __int16 field_106;
};
ALIVE_ASSERT_SIZEOF(ParamiteWeb, 0x108);
