#pragma once

#include "Font.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class PauseMenu final : public BaseAnimatedWithPhysicsGameObject
{
public:
    PauseMenu();
    ~PauseMenu();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

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
    void DrawEntries(OrderingTable& ot, PauseEntry* entries, s16 compilerhack, s32 polyOffset);

    FontContext mFontContext;
    PalResource mPal;
    AliveFont field_E4_font;
    s16 field_11C = 0;
    s16 field_11E_selected_glow = 0;
    s16 field_120_selected_glow_counter = 0;
    s16 field_124 = 0;
    s16 field_126_page = 0;
    s16 field_128_controller_id = 0;
    s16 field_12C = 0;
    s16 field_134 = 0;
    Poly_G4 field_158[2] = {};
};

extern PauseMenu* gPauseMenu;

} // namespace AO
