#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_MovingBomb : public Path_TLV
{
    unsigned __int16 field_10_speed;
    __int16 field_12_id;
    __int16 field_14_start_type;
    __int16 field_16_scale;
    __int16 field_18_max_rise;
    __int16 field_1A_disable_resources;
    unsigned __int16 field_1C_start_speed;
    __int16 field_1E_persist_offscreen;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovingBomb, 0x20);

struct Path_MovingBomb_Point : public Path_TLV
{
    __int16 field_10_min;
    __int16 field_12_max;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovingBomb_Point, 0x14);

class MovingBomb : public BaseAliveGameObject
{
public:
    EXPORT MovingBomb* ctor_46FD40(Path_MovingBomb* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void vnull_4081A0(BaseGameObject* pFrom) override;
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

private:
    EXPORT MovingBomb* vdtor_470040(signed int flags);
    EXPORT void dtor_4700C0();
    EXPORT void BlowUp_470070();
    EXPORT void vRender_4707D0(int** ot);
    EXPORT void vScreenChanged_470B90();
    EXPORT void FollowLine_470950();
    EXPORT __int16 vTakeDamage_470990(BaseGameObject* pFrom);
    EXPORT void vsub_470800(BaseGameObject* pObj);
    EXPORT signed __int16 HitObject_470830();
    EXPORT void vUpdate_4701E0();

private:
    __int16 field_118_state;
    __int16 field_11A;
    int field_11C_tlvInfo;
    int field_120_timer;
    FP field_124_speed;
    __int16 field_128_switch_id;
    __int16 field_12A_min;
    __int16 field_12C_max;
    __int16 field_12E;
    int field_130_sound_channels;
    __int16 field_134;
    __int16 field_136_persist_offscreen;
};
ALIVE_ASSERT_SIZEOF(MovingBomb, 0x138);
