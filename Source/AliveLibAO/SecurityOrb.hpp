#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

namespace AO {

class SecurityOrb final : public BaseAliveGameObject
{
public:
    SecurityOrb(relive::Path_SecurityOrb* pTlv, const Guid& tlvId);
    ~SecurityOrb();
    
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

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
ALIVE_ASSERT_SIZEOF(SecurityOrb, 0x11C);

} // namespace AO
