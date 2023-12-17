#pragma once

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
    eAbe,
    eSlig,
    eSlog,
    eMud,
    eBlindMud,
    eMetal,
    eGlukkon,
    eAslik,
    eDripik,
    ePhleg,
    eFleech,
    eElum,
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
