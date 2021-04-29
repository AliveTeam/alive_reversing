#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

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

class SecurityOrb : public BaseAliveGameObject
{
public:
    EXPORT SecurityOrb* ctor_436C80(Path_SecurityOrb* pTlv, s32 tlvInfo);
    EXPORT BaseGameObject* dtor_436D60();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

private:
    EXPORT SecurityOrb* Vdtor_4373B0(s32 flags);
    EXPORT void VScreenChanged_4373A0();
    EXPORT s16 VTakeDamage_437280(BaseGameObject* pFrom);
    EXPORT void VUpdate_436DF0();

public:
    s32 field_10C_tlvInfo;
    SecurityOrbStates field_110_state;
    s16 field_112_pad;
    s32 field_114_timer;
    s32 field_118_sound_channels;
};
ALIVE_ASSERT_SIZEOF(SecurityOrb, 0x11C);

} // namespace AO
