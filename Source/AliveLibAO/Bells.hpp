#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

enum class BellSize : s16
{
    eBig_0 = 0,
    eMedium_1 = 1,
    eSmall_2 = 2,
};

class Bells : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Bells* ctor_40A650(BellSize bellSize, FP xpos, FP ypos, FP scale);

    EXPORT BaseGameObject* dtor_40A760();

    virtual void VUpdate() override;

    EXPORT void VUpdate_40A770();

    void PlaySounds();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Bells* Vdtor_40AB00(s32 flags);

    EXPORT BOOL CanSmash_40AA70();

    EXPORT void Ring_40AA80();

    s32 field_D4_padding;
    s32 field_D8_padding;
    s32 field_DC_padding;
    s32 field_E0_padding;
    s32 field_E4_padding;

    u16 field_E8_bSmashing;
    enum class Sounds : u16
    {
        eLowPitch_0 = 0,
        eMediumPitch_1 = 1,
        eHighPitch_2 = 2
    };
    Sounds field_EA_sound;
    s32 field_EC_timer;
    s8 field_F0_timer;
    s8 field_F1_padding;
    s8 field_F2_padding;
    s8 field_F3_padding;
    s32 field_F4_timer;
};
ALIVE_ASSERT_SIZEOF(Bells, 0xF8);

} // namespace AO
