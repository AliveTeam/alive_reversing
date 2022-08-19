#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

struct Path_BackgroundGlukkon final : public Path_TLV
{
    s16 mScalePercent;
    u16 mPalId;
    s16 mTargetId; // unused
    s16 mVoiceAdjust; // unused
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BackgroundGlukkon, 0x20);

class BackgroundGlukkon final : public BaseAliveGameObject
{
public:
    BackgroundGlukkon(Path_BackgroundGlukkon* pTlv, s32 tlvInfo);
    ~BackgroundGlukkon();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

    s32 field_10C_tlvInfo = 0;
    enum class State : s16
    {
        eToSetSpeakPauseTimer_0 = 0,
        eSetSpeakPauseTimer_1 = 1,
        eRandomizedLaugh_2 = 2,
        eAfterLaugh_SetSpeakPauseTimer_3 = 3,
        eKilledByShrykull_7 = 7
    };
    State field_110_state = State::eToSetSpeakPauseTimer_0;
    s32 field_114_speak_pause_timer = 0;
    s32 field_118_never_read = 0;
    s32 field_11C_voice_adjust = 0;
    s16 field_120_target_id = 0;
};
ALIVE_ASSERT_SIZEOF(BackgroundGlukkon, 0x124);

} // namespace AO
