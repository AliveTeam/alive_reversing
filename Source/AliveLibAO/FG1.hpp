#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

class FG1 : public BaseGameObject
{
public:
    EXPORT FG1* ctor_4539C0(unsigned __int8** ppRes);

    EXPORT FG1* Vdtor_453E90(signed int flags);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_453DE0();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_453D50(int** ppOt);

    __int16 field_10;
    __int16 field_12;
    __int16 field_14;
    __int16 field_16;
    __int16 field_18;
    __int16 field_1A;
    int field_1C;
    int field_20;
};
ALIVE_ASSERT_SIZEOF(FG1, 0x24);

END_NS_AO
