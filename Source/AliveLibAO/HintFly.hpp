#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

struct StringTable;

namespace relive
{
    struct Path_HintFly;
}

namespace AO {

void SetHintFlyMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId);

struct HintFlyParticle;

class HintFly final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HintFly(relive::Path_HintFly* pTlv, const Guid& tlvId);
    ~HintFly();

    void InitParticle(HintFlyParticle* pParticle);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void FormWordAndAdvanceToNextWord();
    void UpdateParticles();

    s32 field_D4[4] = {};
    HintFlyParticle* field_E8_pRes = nullptr;
    Prim_SetTPage field_EC_tPages[2] = {};
    s32 field_10C_timer = 0;

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
    Guid field_124_tlvInfo;
};

} // namespace AO
