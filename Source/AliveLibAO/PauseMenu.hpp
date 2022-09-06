#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Font.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class PauseMenu final : public BaseAnimatedWithPhysicsGameObject
{
public:
    PauseMenu();
    ~PauseMenu();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    struct PauseEntry final
    {
        s16 x;
        s16 y;
        const char_type* field_4_strBuf;
        u8 mRed;
        u8 mGreen;
        u8 mBlue;
        s8 field_B;
    };
    void DrawEntries(PrimHeader** ppOt, PauseEntry* entries, s16 compilerhack, s32 polyOffset);

    s32 field_D4[4] = {};
    AliveFont field_E4_font;
    s16 field_11C = 0;
    s16 field_11E_selected_glow = 0;
    s16 field_120_selected_glow_counter = 0;
    s16 field_122 = 0;
    s16 field_124 = 0;
    s16 field_126_page = 0;
    s16 field_128_controller_id = 0;
    s16 field_12A = 0;
    s16 field_12C = 0;
    s16 field_12E = 0;
    s16 field_130 = 0;
    s16 field_132_always_0 = 0;
    s16 field_134 = 0;
    s16 field_136 = 0;
    Prim_SetTPage field_138_tPage[2] = {};
    Poly_F4 field_158[2] = {};
};
ALIVE_ASSERT_SIZEOF(PauseMenu, 0x190);

ALIVE_VAR_EXTERN(PauseMenu*, pPauseMenu_5080E0);
ALIVE_VAR_EXTERN(s8, byte_A88B90);

} // namespace AO
