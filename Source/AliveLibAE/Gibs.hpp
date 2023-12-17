#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

struct GibPart final
{
    FP x;
    FP y;
    FP z;
    FP dx;
    FP dy;
    FP dz;
    Animation mAnimation;
};

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

    void LoadAnimations(AnimId head, AnimId arm, AnimId body);

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

private:
    FP mZ = {};
    FP mDz = {};
    s32 mAliveTimer = 0;
    GibPart mGibParts[7] = {};
    s16 mPartsUsedCount = 0;
    bool mMakeSmaller = false;
};
