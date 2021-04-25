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

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT PauseMenu* Vdtor_44EAA0(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChange_44EA90();

    virtual void VUpdate() override;

    EXPORT void VUpdate_44DFB0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_44E6F0(PrimHeader** ppOt);

    struct PauseEntry
    {
        s16 field_0_x;
        s16 field_2_y;
        const s8 *field_4_strBuf;
        u8 field_8_r;
        u8 field_9_g;
        u8 field_A_b;
        s8 field_B;
    };
    void DrawEntries(PrimHeader** ppOt, PauseEntry* entries, s16 compilerhack, s32 polyOffset);

    s32 field_D4[4];
    AliveFont field_E4_font;
    s16 field_11C;
    s16 field_11E;
    s16 field_120;
    s16 field_122;
    s16 field_124;
    s16 field_126_page;
    s16 field_128_controller_id;
    s16 field_12A;
    s16 field_12C;
    s16 field_12E;
    s16 field_130;
    s16 field_132;
    s16 field_134;
    s16 field_136;
    Prim_SetTPage field_138_tPage[2];
    Poly_F4 field_158[2];
};
ALIVE_ASSERT_SIZEOF(PauseMenu, 0x190);

ALIVE_VAR_EXTERN(PauseMenu*, pPauseMenu_5080E0);
ALIVE_VAR_EXTERN(s8, byte_A88B90);

}
