#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class MusicTrigger : public BaseGameObject
{
public:
    EXPORT MusicTrigger* ctor_47FF10(__int16 type, __int16 a3, int /*a4*/, __int16 delay);
    EXPORT void Init_47FFB0(__int16 type, __int16 a3, __int16 delay);
    EXPORT BaseGameObject* vdtor_47FEE0(signed int flags);
    EXPORT void dtor_4800C0();
    EXPORT void vScreenChange_4802A0();
    EXPORT void vUpdate_480140();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    int field_20;
    __int16 field_24_flags;
    __int16 field_26;
    int field_28_counter;
    __int16 field_2C;
    __int16 field_2E;
    __int16 field_30;
    __int16 field_32;
};
ALIVE_ASSERT_SIZEOF(MusicTrigger, 0x34);
