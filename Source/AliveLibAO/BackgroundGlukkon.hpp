#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

class BackgroundGlukkon final : public BaseAliveGameObject
{
public:
    BackgroundGlukkon(relive::Path_BackgroundGlukkon* pTlv, const Guid& tlvId);
    ~BackgroundGlukkon();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

    Guid field_10C_tlvInfo;
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
