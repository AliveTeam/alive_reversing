#pragma once

#include "Path.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

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
    EXPORT SecurityOrb* ctor_466350(Path_SecurityOrb* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override
    {
        return vdtor_4664B0(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_4665A0();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_466D20();
    }

    virtual void VOnThrowableHit(BaseGameObject* /*pFrom*/) override
    {
        // Empty
    }

    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override
    {
        return vTakeDamage_466BB0(pFrom);
    }

private:
    EXPORT SecurityOrb* vdtor_4664B0(s32 flags);

    EXPORT void dtor_4664E0();

    EXPORT void vScreenChanged_466D20();

    EXPORT s16 vTakeDamage_466BB0(BaseGameObject* pFrom);

    EXPORT void vUpdate_4665A0();

private:
    s32 field_118_tlvInfo;
    s16 field_11C_state;
    //s16 field_11E_pad;
    s32 field_120_timer;
    s32 field_124_sound_channels_mask;
};
ALIVE_ASSERT_SIZEOF(SecurityOrb, 0x128);
