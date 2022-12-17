#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/AnimResources.hpp"
namespace AO {

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

struct Gib_Data final
{
    AnimId mHead;
    AnimId mArm;
    AnimId mBody;
};

enum GibType : s32
{
    Abe_0 = 0,
    Slig_1 = 1,
    Slog_2 = 2,
    Elum_3 = 3,
    Mud_4 = 4,
    Metal_5 = 5
};

class Gibs final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Gibs(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale);
    ~Gibs();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    const Gib_Data* mGibData = nullptr;
    FP field_E8_z = {};
    FP field_EC_dz = {};
    s32 mAliveTimer = 0;
    GibPart mGibParts[7] = {};
    s16 mPartsUsedCount = 0;
};

} // namespace AO
