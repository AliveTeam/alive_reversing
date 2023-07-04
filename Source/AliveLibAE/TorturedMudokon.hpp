#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_TorturedMudokon;
}

enum class TorturedMudokonState : s16
{
    eBeingTortured_0 = 0,
    eKilled_1 = 1,
    eReleased_2 = 2
};

class TorturedMudokon final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    TorturedMudokon(relive::Path_TorturedMudokon* pTlv, const Guid& tlvId);
    ~TorturedMudokon();
    
    void LoadAnimations();

    virtual void VRender(BasePrimitive** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    

private:
    void SetupTearsAnimation(Animation* pAnim);
    void SetupZapAnimation(Animation* pAnim);

private:
    Animation mTearsAnim = {};
    Animation mZapAnim = {};
    Guid mTlvInfo;
    s32 mFlashColourTimer = 0;
    s16 mFlashColourCounter = 0;
    s16 mKillSwitchId = 0;
    s16 mReleaseSwitchId = 0;
    TorturedMudokonState mState = TorturedMudokonState::eBeingTortured_0;
    s16 mPainSoundPitch = 0;
};
