#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_SecurityOrb final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_disable_resources;
};
ALIVE_ASSERT_SIZEOF(Path_SecurityOrb, 0x1C);

enum class SecurityOrbStates : s16
{
    eIdle_0,
    eDoZapEffects_1,
    eDoFlashAndSound_2
};

class SecurityOrb final : public BaseAliveGameObject
{
public:
    SecurityOrb(Path_SecurityOrb* pTlv, s32 tlvInfo);
    ~SecurityOrb();
    
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

public:
    s32 field_10C_tlvInfo;
    SecurityOrbStates field_110_state;
    s16 field_112_pad;
    s32 field_114_timer;
    s32 field_118_sound_channels;
};
ALIVE_ASSERT_SIZEOF(SecurityOrb, 0x11C);

} // namespace AO
