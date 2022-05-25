#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Font.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class PauseMenu final : public BaseAnimatedWithPhysicsGameObject
{
public:
    PauseMenu();
    ~PauseMenu();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChange_44EA90();

    virtual void VUpdate() override;

    EXPORT void VUpdate_44DFB0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_44E6F0(PrimHeader** ppOt);

    struct PauseEntry final
    {
        s16 field_0_x;
        s16 field_2_y;
        const char_type* field_4_strBuf;
        u8 field_8_r;
        u8 field_9_g;
        u8 field_A_b;
        s8 field_B;
    };
    void DrawEntries(PrimHeader** ppOt, PauseEntry* entries, s16 compilerhack, s32 polyOffset);

    s32 field_D4[4];
    AliveFont field_E4_font;
    s16 field_11C;
    s16 field_11E_selected_glow;
    s16 field_120_selected_glow_counter;
    s16 field_122;
    s16 field_124;
    s16 field_126_page;
    s16 field_128_controller_id;
    s16 field_12A;
    s16 field_12C;
    s16 field_12E;
    s16 field_130;
    s16 field_132_always_0;
    s16 field_134;
    s16 field_136;
    Prim_SetTPage field_138_tPage[2];
    Poly_F4 field_158[2];
};
ALIVE_ASSERT_SIZEOF(PauseMenu, 0x190);

ALIVE_VAR_EXTERN(PauseMenu*, pPauseMenu_5080E0);
ALIVE_VAR_EXTERN(s8, byte_A88B90);

} // namespace AO
