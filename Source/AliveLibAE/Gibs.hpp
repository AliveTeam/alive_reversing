#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

struct GibPart final
{
    FP x;
    FP y;
    FP field_8_z;
    FP field_C_dx;
    FP field_10_dy;
    FP field_14_dz;
    Animation mAnimation;
};
ALIVE_ASSERT_SIZEOF(GibPart, 0xB0);

enum class GibType : s32
{
    Abe_0 = 0,
    Slig_1 = 1,
    Slog_2 = 2,
    Mud_3 = 3,
    BlindMud_4 = 4,
    Metal_5 = 5,
    Glukkon_6 = 6,
    Aslik_7 = 7,
    Dripik_8 = 8,
    Phleg_9 = 9,
    Fleech_10 = 10,
};

class Gibs final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Gibs(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale, bool bMakeSmaller);
    ~Gibs();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(BasePrimitive** ppOt) override;

private:
    FP field_F8_z = {};
    FP field_FC_dz = {};
    s32 mAliveTimer = 0;
    GibPart mGibParts[7] = {};
    s16 mPartsUsedCount = 0;
    bool field_5D6_bMakeSmaller = false;
};
