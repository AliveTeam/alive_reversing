#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

struct Path_SecurityOrb : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_disable_resources;
};
ALIVE_ASSERT_SIZEOF(Path_SecurityOrb, 0x1C);

class SecurityOrb : public BaseAliveGameObject
{
public:
    EXPORT SecurityOrb* ctor_436C80(Path_SecurityOrb* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_436D60();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT SecurityOrb* Vdtor_4373B0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4373A0();

    EXPORT signed __int16 VTakeDamage_437280(BaseGameObject* pFrom);

    virtual void VUpdate() override;

    EXPORT void VUpdate_436DF0();

    int field_10C_tlvInfo;
    __int16 field_110_state;
    __int16 field_112_pad;
    int field_114_timer;
    int field_118_sound_channels;
};
ALIVE_ASSERT_SIZEOF(SecurityOrb, 0x11C);

}

