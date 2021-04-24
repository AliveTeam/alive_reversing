#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

struct Path_Dove : public Path_TLV
{
    s16 field_18_dove_count;
    Choice_short field_1A_pixel_perfect;
    Scale_short field_1C_scale;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Dove, 0x20);

void CC Dove_static_ctor_40EE10();

class Dove : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Dove* ctor_40EE50(int frameTableOffset, int maxW, int maxH, int resourceID, int tlvInfo, FP scale);

    EXPORT Dove* ctor_40EFF0(int frameTableOffset, int maxW, int maxH, int resourceID, FP xpos, FP ypos, FP scale);

    EXPORT BaseGameObject* dtor_40F1B0();

    EXPORT void AsAlmostACircle_40F300(FP xpos, FP ypos, BYTE angle);

    EXPORT void AsACircle_40F280(FP xpos, FP ypos, BYTE angle);

    EXPORT void AsJoin_40F250(FP xpos, FP ypos);

    EXPORT void FlyAway_40F8F0(s16 a2);

    EXPORT static void All_FlyAway_40F390();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_40F960(PrimHeader** ppOt);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Dove* Vdtor_40F970(u32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_40F430();

    int field_D4[4];
    s16 field_E4_counter;
    s16 field_E6;
    int field_E8_tlvInfo;
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
    int field_F8_timer;
    char field_FC_angle;
    // 3 byte pad
    FP field_100_prevX;
    FP field_104_prevY;
};
ALIVE_ASSERT_SIZEOF(Dove, 0x108);

}

