#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

namespace relive
{
    class Path_SecurityOrb;
}

class SecurityOrb final : public BaseAliveGameObject
{
public:
    SecurityOrb(relive::Path_SecurityOrb* pTlv, const Guid& tlvId);
    ~SecurityOrb();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject* /*pFrom*/) override
    {
        // Empty
    }
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

private:
    Guid mTlvInfo;
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
