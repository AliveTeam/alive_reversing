#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

void HintFly_ForceLink();

struct StringTable;

namespace AO {

void SetHintFlyMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId);

struct HintFlyParticle;

struct Path_HintFly final : public Path_TLV
{
    s16 field_18_message_id;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF(Path_HintFly, 0x1C);

class HintFly final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT HintFly* ctor_42A820(Path_HintFly* pTlv, s32 tlvInfo);

    void InitParticle(HintFlyParticle* pParticle);

    

    EXPORT HintFly* Vdtor_42BCF0(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_42BCE0();

    EXPORT BaseGameObject* dtor_42ADF0();

    EXPORT void FormWordAndAdvanceToNextWord_42AF90();

    EXPORT void UpdateParticles_42B1B0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_42B3D0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_42BAD0(PrimHeader** ppOt);

    s32 field_D4[4];
    u8** field_E4_ppRes;
    HintFlyParticle* field_E8_pRes;
    Prim_SetTPage field_EC_tPages[2];
    s32 field_10C_timer;
    TPageMode field_110_bitMode;
    // pad

    enum class State : s16
    {
        eState_0 = 0,
        eIdleWaitForChanting_1 = 1,
        eState_2 = 2,
        eState_3 = 3,
        eState_4 = 4,
        eState_5 = 5,
        eState_6 = 6,
    };
    State field_112_state;
    s16 field_114_xScreen;
    s16 field_116_yScreen;
    s16 field_118_counter;
    s16 field_11A_msg_len;
    s16 field_11C_message_id;
    s16 field_11E_msg_idx;
    s16 field_120_idx;
    s16 field_122_target_count;
    s32 field_124_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(HintFly, 0x128);

} // namespace AO
