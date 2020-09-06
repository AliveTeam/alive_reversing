#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "FixedPoint.hpp"

START_NS_AO

struct Path_Dove : public Path_TLV
{
    __int16 field_18_dove_count;
    __int16 field_1A_pixel_perfect;
    __int16 field_1C_scale;
    __int16 field_1E_pad;
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

    EXPORT void FlyAway_40F8F0(__int16 a2);

    EXPORT static void All_FlyAway_40F390();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_40F960(int** ppOt);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Dove* Vdtor_40F970(unsigned int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_40F430();

    int field_D4[4];
    __int16 field_E4_counter;
    __int16 field_E6;
    int field_E8_tlvInfo;
    __int16 field_EC_keepInGlobalArray;
    enum class State : __int16
    {
        State_0_OnGround = 0,
        State_1_FlyAway = 1,
        State_2_Join = 2,
        State_3_Circle = 3,
        State_4_AlmostACircle = 4,
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

END_NS_AO

