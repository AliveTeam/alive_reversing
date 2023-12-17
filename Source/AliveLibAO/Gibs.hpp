#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/AnimResources.hpp"
namespace AO {

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

    void LoadAnimations(const Gib_Data& data);

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

    const Gib_Data* mGibData = nullptr;
    FP mZ = {};
    FP mDz = {};
    s32 mAliveTimer = 0;
    GibPart mGibParts[7] = {};
    s16 mPartsUsedCount = 0;
};

} // namespace AO
