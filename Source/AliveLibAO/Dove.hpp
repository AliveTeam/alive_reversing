#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_Dove final : public Path_TLV
{
    s16 field_18_dove_count;
    Choice_short field_1A_pixel_perfect;
    Scale_short field_1C_scale;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Dove, 0x20);

class Dove final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, s32 tlvInfo, FP scale);
    Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, FP xpos, FP ypos, FP scale);
    ~Dove();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    static void All_FlyAway();

    void AsAlmostACircle(FP xpos, FP ypos, u8 angle);
    void AsACircle(FP xpos, FP ypos, u8 angle);
    void AsJoin(FP xpos, FP ypos);
    void FlyAway(s16 a2);

    s16 field_E4_counter;
    s32 field_E8_tlvInfo;
    s16 field_EC_keepInGlobalArray;
    enum class State : s16
    {
        eOnGround_0 = 0,
        eFlyAway_1 = 1,
        eJoin_2 = 2,
        eCircle_3 = 3,
        eAlmostACircle_4 = 4,
    };
    State field_EE_state;
    FP field_F0_xJoin;
    FP field_F4_yJoin;
    s32 field_F8_timer;
    s8 field_FC_angle;
    FP field_100_prevX;
    FP field_104_prevY;
};
ALIVE_ASSERT_SIZEOF(Dove, 0x108);

} // namespace AO
