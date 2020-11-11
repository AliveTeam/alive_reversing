#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

void HintFly_ForceLink();

START_NS_AO

struct HintFlyParticle;

struct Path_HintFly : public Path_TLV
{
    __int16 field_18_mesage_id;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF(Path_HintFly, 0x1C);

class HintFly : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT HintFly* ctor_42A820(Path_HintFly* pTlv, int tlvInfo);

    void InitParticle(HintFlyParticle* pParticle);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT HintFly* Vdtor_42BCF0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_42BCE0();

    EXPORT BaseGameObject* dtor_42ADF0();

    EXPORT void FormWordAndAdvanceToNextWord_42AF90();

    EXPORT void UpdateParticles_42B1B0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_42B3D0();

    virtual void VRender(int** pOrderingTable) override;

    EXPORT void VRender_42BAD0(int** ppOt);

    int field_D4[4];
    BYTE** field_E4_ppRes;
    HintFlyParticle* field_E8_pRes;
    Prim_SetTPage field_EC_tPages[2];
    int field_10C_timer;
    __int16 field_110_bitMode;

    enum class State : __int16
    {
        eState_0 = 0,
        eState_1 = 1,
        eState_2 = 2,
        eState_3 = 3,
        eState_4 = 4,
        eState_5 = 5,
        eState_6 = 6,
    };
    State field_112_state;
    __int16 field_114_xScreen;
    __int16 field_116_yScreen;
    __int16 field_118_counter;
    __int16 field_11A_msg_len;
    __int16 field_11C_message_id;
    __int16 field_11E_msg_idx;
    __int16 field_120_idx;
    __int16 field_122_target_count;
    int field_124_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(HintFly, 0x128);

END_NS_AO

