#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

struct Path_BackgroundGlukkon final : public Path_TLV
{
    s16 field_18_scale_percent;
    u16 field_1A_pal_id;
    s16 field_1C_target_id;
    s16 field_1E_voice_adjust;
};
ALIVE_ASSERT_SIZEOF(Path_BackgroundGlukkon, 0x20);

class BackgroundGlukkon final : public BaseAliveGameObject
{
public:
    BackgroundGlukkon(Path_BackgroundGlukkon* pTlv, s32 tlvInfo);
    ~BackgroundGlukkon();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

    s32 field_10C_tlvInfo;
    enum class State : s16
    {
        eToSetSpeakPauseTimer_0 = 0,
        eSetSpeakPauseTimer_1 = 1,
        eRandomizedLaugh_2 = 2,
        eAfterLaugh_SetSpeakPauseTimer_3 = 3,
        eKilledByShrykull_7 = 7
    };
    State field_110_state;
    s16 field_112_pad;
    s32 field_114_speak_pause_timer;
    s32 field_118_never_read;
    s32 field_11C_voice_adjust;
    s16 field_120_target_id;
    s16 field_122_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundGlukkon, 0x124);

} // namespace AO
