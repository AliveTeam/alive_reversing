#pragma once

#include "Path.hpp"
#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

struct Path_ChantSuppressor : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_disabled_resources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ChantSuppressor, 0x14);

class ChantSuppressor : public BaseAliveGameObject
{
public:
    EXPORT ChantSuppressor* ctor_466350(Path_ChantSuppressor* pTlv, int tlvInfo);

private:
    EXPORT ChantSuppressor* vdtor_4664B0(char flags);

    EXPORT void dtor_4664E0();

    EXPORT void vScreenChanged_466D20();

    EXPORT signed __int16 vTakeDamage_466BB0(BaseGameObject* pFrom);

private:
    __int16 field_116;
    int field_118_tlvInfo;
    __int16 field_11C_state;
    //__int16 field_11E_pad;
    int field_120_timer;
    int field_124_sound_channels_mask;
};
ALIVE_ASSERT_SIZEOF(ChantSuppressor, 0x128);
