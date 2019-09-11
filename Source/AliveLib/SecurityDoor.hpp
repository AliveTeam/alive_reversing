#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_SecurityDoor : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_id;
    __int16 field_14_code_1;
    __int16 field_16_code2;
    __int16 field_18_xpos;
    __int16 field_1A_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityDoor, 0x1C);

class SecurityDoor : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT SecurityDoor* ctor_4ABFC0(Path_SecurityDoor* pTlv, int tlvInfo);

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    EXPORT void dtor_4AC260();

    EXPORT void vScreenChanged_4AC970();

    EXPORT SecurityDoor* vdtor_4AC230(signed int flags);

    EXPORT __int16 IsPlayerNear_4AC300();

private:
    int field_F4_tlvInfo;
    __int16 field_F8_state;
    __int16 field_FA_id;
    int field_FC_code_converted;
    __int16 field_100_code_len;
    __int16 field_102;
    int field_104_event_idx;
    __int16 field_108_stru;
    __int16 field_10A;
    __int16 field_10C;
    __int16 field_10E;
    __int16 field_110;
    __int16 field_112;
    __int16 field_114;
    __int16 field_116;
    __int16 field_118_max_idx;
    __int16 field_11A_event_idx;
    PSX_Point field_11C_top_left;
    PSX_Point field_120_bottom_right;
    int field_124_timer;
    __int16 field_128_max_idx;
    __int16 field_12A;
};
ALIVE_ASSERT_SIZEOF(SecurityDoor, 0x12C);
