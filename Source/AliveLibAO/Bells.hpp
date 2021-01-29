#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

enum class BellSize : __int16
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

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Bells* Vdtor_40AB00(signed int flags);

    EXPORT BOOL CanSmash_40AA70();

    EXPORT void Ring_40AA80();

    int field_D4_padding;
    int field_D8_padding;
    int field_DC_padding;
    int field_E0_padding;
    int field_E4_padding;

    unsigned __int16 field_E8_bSmashing;
    enum class Sounds : unsigned __int16
    {
        eLowPitch_0 = 0,
        eMediumPitch_1 = 1,
        eHighPitch_2 = 2
    };
    Sounds field_EA_sound;
    int field_EC_timer;
    char field_F0_timer;
    char field_F1_padding;
    char field_F2_padding;
    char field_F3_padding;
    int field_F4_timer;
};
ALIVE_ASSERT_SIZEOF(Bells, 0xF8);

}

