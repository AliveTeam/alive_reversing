#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

struct Path_TimedMine : public Path_TLV
{
    __int16 field_18_id;
    __int16 field_1A_state;
    __int16 field_1C_scale;
    __int16 field_1E_ticks_before_explode;
    __int16 field_20_disable_resources;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_TimedMine, 0x24);

class TimedMine : public BaseAliveGameObject
{
public:
    EXPORT TimedMine* ctor_4083F0(Path_TimedMine* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_408690();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT TimedMine* Vdtor_408E10(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_408DD0();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT __int16 VTakeDamage_408B90(BaseGameObject* pFrom);

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    EXPORT void VOnThrowableHit_408B10(BaseGameObject* pFrom);

    __int16 field_10C_armed;
    unsigned __int16 field_10E_explode_timeout;
    int field_110_tlvInfo;
    int field_114_timer;
    Animation field_118_anim;
    int field_1B0;
    int field_1B4;
    __int16 field_1B8;
    char field_1BA;
    char field_1BB;
};
ALIVE_ASSERT_SIZEOF(TimedMine, 0x1BC);

END_NS_AO

