#pragma once

#include "Path.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Path_SecurityClaw final : public Path_TLV
{
    Scale_int mScale;
    s16 mDisabledResources;
    s16 field_14_unknown;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityClaw, 0x18);

struct Path_SecurityOrb final : public Path_TLV
{
    Scale_short mScale;
    s16 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityOrb, 0x14);

class SecurityOrb final : public BaseAliveGameObject
{
public:
    SecurityOrb(relive::Path_SecurityOrb* pTlv, s32 tlvInfo);
    ~SecurityOrb();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject* /*pFrom*/) override
    {
        // Empty
    }
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

private:
    s32 mTlvInfo = 0;
    enum class States : s16
    {
        eIdle_0,
        eDoZapEffects_1,
        eDoFlashAndSound_2
    };
    States mState = States::eIdle_0;
    s32 mTimer = 0;
    s32 mSoundChannelsMask = 0;
};
ALIVE_ASSERT_SIZEOF(SecurityOrb, 0x128);
