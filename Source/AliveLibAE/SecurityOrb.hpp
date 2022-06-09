#pragma once

#include "Path.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Path_SecurityClaw final : public Path_TLV
{
    Scale_int field_10_scale;
    s16 field_12_disabled_resources;
    s16 field_14_unknown;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityClaw, 0x18);

struct Path_SecurityOrb final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_disabled_resources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityOrb, 0x14);

class SecurityOrb final : public BaseAliveGameObject
{
public:
    SecurityOrb(Path_SecurityOrb* pTlv, s32 tlvInfo);
    ~SecurityOrb();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject* /*pFrom*/) override
    {
        // Empty
    }
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

private:
    s32 field_118_tlvInfo = 0;
    s16 field_11C_state = 0;
    s32 field_120_timer = 0;
    s32 field_124_sound_channels_mask = 0;
};
ALIVE_ASSERT_SIZEOF(SecurityOrb, 0x128);
