#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Path.hpp"

EXPORT void CC Static_init_dynamic_array_41F3A0();

struct Path_Dove : public Path_TLV
{
    __int16 field_10_dove_count;
    __int16 field_12_pixel_perfect;
    __int16 field_14_scale;
    __int16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Dove, 0x18);

class Dove : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Dove* ctor_41F430(int frameTableOffset, int maxW, unsigned __int16 maxH, int resourceID, int tlvInfo, FP scale);

    EXPORT Dove* ctor_41F660(int frameTableOffset, int maxW, __int16 maxH, int resourceID, FP xpos, FP ypos, FP scale);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    EXPORT void dtor_41F870();

    EXPORT void AsAlmostACircle_41FA20(FP xpos, FP ypos, BYTE angle);
    EXPORT void AsACircle_41F980(FP xpos, FP ypos, BYTE angle);
    EXPORT void AsJoin_41F940(FP xpos, FP ypos);
    EXPORT void FlyAway_420020(__int16 a2);

    static void CC All_FlyAway_41FA60(__int16 a1);

private:
    EXPORT void vRender_4200B0(int** ot);
    EXPORT Dove* vdtor_41F630(signed int flags);
    EXPORT void vUpdate_41FAE0();

private:
    __int16 field_F4_counter;
    __int16 field_F6;
    int field_F8_tlvInfo;
    __int16 field_FC_keepInGlobalArray;
    enum class State : __int16
    {
        State_0_OnGround = 0,
        State_1_FlyAway = 1,
        State_2_Join = 2,
        State_3_Circle = 3,
        State_4_AlmostACircle = 4,
    };
    State field_FE_state;
    FP field_100_xJoin;
    FP field_104_yJoin;
    int field_108_timer;
    char field_10C_angle;
    // 3 byte pad
    FP field_110_prevX;
    FP field_114_prevY;
};
ALIVE_ASSERT_SIZEOF(Dove, 0x118);
