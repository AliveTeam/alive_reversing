#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

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

    HintFlyParticle* mHintFlyParticle = nullptr;
    s32 mTimer = 0;

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
    State mState = State::eState_0;
    s16 mScreenX = 0;
    s16 mScreenY = 0;
    s16 mCounter = 0;
    s16 mMsgLength = 0;
    s16 mMessageId = 0;
    s16 mMsgIdx = 0;
    s16 mHintFlyIdx = 0;
    s16 mTargetCount = 0;
    Guid mTlvId;
};

} // namespace AO
