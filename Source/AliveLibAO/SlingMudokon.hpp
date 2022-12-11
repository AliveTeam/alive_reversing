#pragma once

#include "BaseAliveGameObject.hpp"
#include "GameSpeak.hpp"

namespace relive
{
    struct Path_SlingMudokon;
}

namespace AO {

enum class eSlingMudMotions : s32
{
    Motion_0_Idle,
    Motion_1_Angry,
    Motion_2_Speak,
    Motion_3_ShootStart,
    Motion_4_ShootEnd,
    Motion_5_AngryToIdle
};

enum SlingMudBrainStates : u16
{
    Brain_0_GiveCode = 0,
    Brain_1_Spawn = 1,
    Brain_2_AskForPassword = 2
};

class SlingMudokon final : public BaseAliveGameObject
{
public:
    SlingMudokon(relive::Path_SlingMudokon* pTlv, const Guid& tlvId);
    ~SlingMudokon();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    virtual void VCallBrain();
    virtual void VCallMotion();

    void VUpdateAnimData();

    eSlingMudMotions GetNextMotion() const
    {
        return static_cast<eSlingMudMotions>(mNextMotion);
    }

    GameSpeakEvents getLastIdx();

    // Motions
    void Motion_0_Idle();
    void Motion_1_Angry();
    void Motion_2_Speak();
    void Motion_3_ShootStart();
    void Motion_4_ShootEnd();
    void Motion_5_AngryToIdle();

    // Brains
    s16 Brain_0_GiveCode();
    s16 Brain_1_Spawn();
    s16 Brain_2_AskForPassword();

    Guid mTlvId;
    s32 mCodeConverted = 0;
    s16 mCodeLength = 0;
    bool mDontSetDestroyed = false;
    bool mAbeGettingCloser = false;
    s32 field_120_last_event_idx = 0;
    u8 field_124_code_buffer[16] = {};
    s16 field_134_buffer_start = 0;
    s16 field_136_buffer_idx = 0;
    u16 field_138_brain_state = 0;
    s16 field_13A_brain_sub_state = 0;
    s32 field_140_timer = 0;
    s32 field_144_timer2 = 0;
    s16 field_154_previous_brain_state = 0;
    s16 field_156_always_4 = 0;
    s16 field_158_code_pos = 0;
    s16 field_15A_bCodeMatches = 0;
};

} // namespace AO
