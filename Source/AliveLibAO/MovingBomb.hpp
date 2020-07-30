#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

class Path_MovingBomb : public Path_TLV
{
public:


    __int16 field_18_speed;
    __int16 field_1A_id;
    __int16 field_1C_start_type;
    __int16 field_1E_scale;
    __int16 field_20_max_rise;
    __int16 field_22_disabled_resources;
    __int16 field_24_start_speed;
    __int16 field_26_persist_offscreen;
};
ALIVE_ASSERT_SIZEOF(Path_MovingBomb, 0x28);

class MovingBomb : public BaseAliveGameObject
{
public:
    EXPORT MovingBomb* ctor_43AFE0(Path_MovingBomb* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_43B2C0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_43BCC0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_43BC90();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT __int16 VTakeDamage_43BB60(BaseGameObject* pFrom);

    virtual void VRender(int** pOrderingTable) override;

    EXPORT void VRender_43B910(int** ppOt);

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    EXPORT void VOnThrowableHit_43B930(BaseGameObject* pFrom);

    __int16 field_10C_state;
    __int16 field_10E_pad;
    int field_110_tlvInfo;
    int field_114_timer;
    FP field_118_speed;
    unsigned __int16 field_11C_id;
    __int16 field_11E_max;
    __int16 field_120_min;
    __int16 field_122_pad;
    int field_124_sound_channels;
    __int16 field_128_disabled_resources;
    __int16 field_12A_persist_offscreen;
};
ALIVE_ASSERT_SIZEOF(MovingBomb, 0x12C);

END_NS_AO
