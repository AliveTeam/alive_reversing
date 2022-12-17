#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

class AnimationUnknown;

class Rope final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Rope(s32 left, s32 top, s32 bottom, FP scale);
    ~Rope();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    s16 mRopeSegmentCount = 0;

public:
    s16 mRopeLength = 0;
    AnimationUnknown* mRopeAnim = nullptr;
    s16 mTop = 0;
    s16 mBottom = 0;
};

void ClipPoly_Vertically(Poly_FT4* pPoly, s32 minY, s32 maxY);
