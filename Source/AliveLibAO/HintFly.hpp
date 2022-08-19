#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

void HintFly_ForceLink();

struct StringTable;

namespace AO {

void SetHintFlyMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId);

struct HintFlyParticle;

struct Path_HintFly final : public Path_TLV
{
    s16 mMessageId;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_HintFly, 0x1C);

class HintFly final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HintFly(Path_HintFly* pTlv, s32 tlvInfo);
    ~HintFly();

    void InitParticle(HintFlyParticle* pParticle);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void FormWordAndAdvanceToNextWord();
    void UpdateParticles();

    s32 field_D4[4] = {};
    u8** field_E4_ppRes = nullptr;
    HintFlyParticle* field_E8_pRes = nullptr;
    Prim_SetTPage field_EC_tPages[2] = {};
    s32 field_10C_timer = 0;
    TPageMode field_110_bitMode = TPageMode::e4Bit_0;

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
    State field_112_state = State::eState_0;
    s16 field_114_xScreen = 0;
    s16 field_116_yScreen = 0;
    s16 field_118_counter = 0;
    s16 field_11A_msg_len = 0;
    s16 field_11C_message_id = 0;
    s16 field_11E_msg_idx = 0;
    s16 field_120_idx = 0;
    s16 field_122_target_count = 0;
    s32 field_124_tlvInfo = 0;
};
ALIVE_ASSERT_SIZEOF(HintFly, 0x128);

} // namespace AO
