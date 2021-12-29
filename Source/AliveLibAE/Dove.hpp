#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Path.hpp"

EXPORT void CC Dove_static_ctor_41F3A0();

struct Path_Dove final : public Path_TLV
{
    s16 field_10_dove_count;
    Choice_short field_12_pixel_perfect;
    Scale_short field_14_scale;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Dove, 0x18);

class Dove final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Dove* ctor_41F430(s32 frameTableOffset, s32 maxW, u16 maxH, s32 resourceID, s32 tlvInfo, FP scale);

    EXPORT Dove* ctor_41F660(s32 frameTableOffset, s32 maxW, s16 maxH, s32 resourceID, FP xpos, FP ypos, FP scale);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    EXPORT void dtor_41F870();

    EXPORT void AsAlmostACircle_41FA20(FP xpos, FP ypos, u8 angle);
    EXPORT void AsACircle_41F980(FP xpos, FP ypos, u8 angle);
    EXPORT void AsJoin_41F940(FP xpos, FP ypos);
    EXPORT void FlyAway_420020(Bool32 spookedInstantly);

    static void CC All_FlyAway_41FA60(Bool32 spookedInstantly);

private:
    EXPORT void vRender_4200B0(PrimHeader** ppOt);
    EXPORT Dove* vdtor_41F630(s32 flags);
    EXPORT void vUpdate_41FAE0();

private:
    s16 field_F4_counter;
    s16 field_F6_padding;
    s32 field_F8_tlvInfo;
    s16 field_FC_keepInGlobalArray;
    enum class State : s16
    {
        eOnGround_0 = 0,
        eFlyAway_1 = 1,
        eJoin_2 = 2,
        eCircle_3 = 3,
        eAlmostACircle_4 = 4,
    };
    State field_FE_state;
    FP field_100_xJoin;
    FP field_104_yJoin;
    s32 field_108_timer;
    s8 field_10C_angle;
    // 3 byte pad
    FP field_110_prevX;
    FP field_114_prevY;
};
ALIVE_ASSERT_SIZEOF(Dove, 0x118);
