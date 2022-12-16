#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Primitives_common.hpp"

namespace relive
{
    struct Path_ExplosionSet;
}

class ExplosionSet final : public BaseGameObject
{
public:
    ExplosionSet();
    ~ExplosionSet();

    
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    void Init(relive::Path_ExplosionSet* pTlv);
    void Start();

private:
    Prim_ScreenOffset mScreenOffset[2] = {};
    s16 mScreenShakeIdx = 0;
    s16 mScreenShakeIdxModifier = 0;
    s16 mStartDelay = 0;
    s16 mSpacingMultiplicator = 0;
    PSX_RECT mTlvRect = {};
    FP mSpriteScale = {};
    s16 mSwitchId = 0;
    s16 mAssetInterval = 0;
    s16 mGridSpacing = 0;
    s16 mIncreasingGridSpacing = 0;
    bool mSpawnAssets = false;
    bool mFlipX = false;
    bool mActive = false;
};

extern ExplosionSet* gExplosionSet;
extern bool gExplosionSetEnabled;
