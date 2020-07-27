#pragma once

#include "FunctionFwd.hpp"
#include "Font.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO;

class PauseMenu : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT PauseMenu* ctor_48FB80();

    EXPORT BaseGameObject* dtor_44DF40();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT PauseMenu* Vdtor_44EAA0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChange_44EA90();

    int field_D4[4];
    AliveFont field_E4_font;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126_page;
    __int16 field_128;
    __int16 field_12A;
    __int16 field_12C;
    __int16 field_12E;
    __int16 field_130;
    __int16 field_132;
    __int16 field_134;
    __int16 field_136;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int field_188;
    int field_18C;
};
ALIVE_ASSERT_SIZEOF(PauseMenu, 0x190);

ALIVE_VAR_EXTERN(PauseMenu*, pPauseMenu_5080E0);

END_NS_AO;
