#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"

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
    Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, s32 tlvInfo, FP scale);
    Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, FP xpos, FP ypos, FP scale);
    ~Dove();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }


    void AsAlmostACircle(FP xpos, FP ypos, u8 angle);
    void AsACircle(FP xpos, FP ypos, u8 angle);
    void AsJoin(FP xpos, FP ypos);
    void FlyAway(bool spookedInstantly);

    static void All_FlyAway(bool spookedInstantly);

private:
    s16 field_F4_counter = 0;
    s32 field_F8_tlvInfo = 0;
    s16 field_FC_keepInGlobalArray = 0;
    enum class State : s16
    {
        eOnGround_0 = 0,
        eFlyAway_1 = 1,
        eJoin_2 = 2,
        eCircle_3 = 3,
        eAlmostACircle_4 = 4,
    };
    State field_FE_state = State::eOnGround_0;
    FP field_100_xJoin = {};
    FP field_104_yJoin = {};
    s32 field_108_timer = 0;
    s8 field_10C_angle = 0;
    FP field_110_prevX = {};
    FP field_114_prevY = {};
};
ALIVE_ASSERT_SIZEOF(Dove, 0x118);
