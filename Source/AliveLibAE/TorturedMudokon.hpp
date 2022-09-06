#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

enum class TorturedMudokonState : s16
{
    eBeingTortured_0 = 0,
    eKilled_1 = 1,
    eReleased_2 = 2
};

struct Path_TorturedMudokon final : public Path_TLV
{
    s16 mKillSwitchId;
    s16 mReleaseSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TorturedMudokon, 0x14);

class TorturedMudokon final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    TorturedMudokon(relive::Path_TorturedMudokon* pTlv, const Guid& tlvId);
    ~TorturedMudokon();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    

private:
    void SetupTearsAnimation(Animation* pAnim);
    void SetupZapAnimation(Animation* pAnim);

private:
    Animation mTearsAnim = {};
    Animation mZapAnim = {};
    u8** mTorturedMudRes = nullptr;
    Guid mTlvInfo;
    s32 mFlashColourTimer = 0;
    s16 mFlashColourCounter = 0;
    s16 mKillSwitchId = 0;
    s16 mReleaseSwitchId = 0;
    TorturedMudokonState mState = TorturedMudokonState::eBeingTortured_0;
    s16 mPainSoundPitch = 0;
};
ALIVE_ASSERT_SIZEOF(TorturedMudokon, 0x244);
