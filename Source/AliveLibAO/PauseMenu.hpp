#pragma once

#include "FunctionFwd.hpp"
#include "Font.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class PauseMenu : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT PauseMenu* ctor_44DEA0();

    EXPORT BaseGameObject* dtor_44DF40();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT PauseMenu* Vdtor_44EAA0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChange_44EA90();

    virtual void VUpdate() override;

    EXPORT void VUpdate_44DFB0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_44E6F0(PrimHeader** ppOt);

    struct PauseEntry
    {
        __int16 field_0_x;
        __int16 field_2_y;
        const char *field_4_strBuf;
        BYTE field_8_r;
        BYTE field_9_g;
        BYTE field_A_b;
        char field_B;
    };
    void DrawEntries(PrimHeader** ppOt, PauseEntry* entries, short compilerhack, int polyOffset);

    int field_D4[4];
    AliveFont field_E4_font;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126_page;
    __int16 field_128_controller_id;
    __int16 field_12A;
    __int16 field_12C;
    __int16 field_12E;
    __int16 field_130;
    __int16 field_132;
    __int16 field_134;
    __int16 field_136;
    Prim_SetTPage field_138[2];
    Poly_F4 field_158[2];
};
ALIVE_ASSERT_SIZEOF(PauseMenu, 0x190);

ALIVE_VAR_EXTERN(PauseMenu*, pPauseMenu_5080E0);
ALIVE_VAR_EXTERN(char, byte_A88B90);

}
