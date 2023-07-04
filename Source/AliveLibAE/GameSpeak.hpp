#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

enum class GameSpeakEvents : s16
{
    eSameAsLast_m2 = -2,
    eNone_m1 = -1,
    eUnknown_0 = 0, // used
    eUnknown_1 = 1, // used
    eAbe_Fart_3 = 3,
    Slig_BS_5 = 5,
    Slig_LookOut_6 = 6,
    Slig_BS2_7 = 7,
    Slig_Laugh_8 = 8,
    eAbe_Hello_9 = 9,
    eAbe_FollowMe_10 = 10,
    eAbe_Anger_11 = 11,
    eAbe_Wait_12 = 12,
    eAbe_Work_21 = 21,
    eAbe_StopIt_22 = 22,
    eAbe_AllYa_23 = 23,
    eAbe_Sorry_24 = 24,
    eSlig_Hi_27 = 27,
    eSlig_HereBoy_28 = 28,
    eSlig_GetEm_29 = 29,
    eSlig_Freeze_31 = 31,
    eGlukkon_None_35 = 35,
    eGlukkon_Hey_36 = 36,
    eGlukkon_DoIt_37 = 37,
    eGlukkon_StayHere_38 = 38,
    eGlukkon_Commere_39 = 39,
    eGlukkon_AllOYa_40 = 40,
    eGlukkon_Heh_41 = 41,
    eGlukkon_Help_42 = 42,
    eGlukkon_Laugh_43 = 43,
    eGlukkon_KillEm_44 = 44,
    eGlukkon_Unknown_45 = 45, // no sound
    eGlukkon_What_47 = 47,

    eParamite_Howdy_48 = 48,
    eParamite_Stay_49 = 49,
    eParamite_CMon_or_Attack_50 = 50,
    eParamite_DoIt_51 = 51,
    eParamite_AllYa_52 = 52,

    eScrab_Howl_53 = 53,
    eScrab_Shriek_54 = 54,
};

enum class GameSpeakMatch : s16
{
    eNoMatch_0 = 0,
    eFullMatch_1 = 1,
    ePartMatch_2 = 2,
};

class GameSpeak final : public BaseGameObject
{
public:
    GameSpeak();
    ~GameSpeak();

    virtual void VUpdate() override;
    virtual void VRender(BasePrimitive** ppOt) override;
    virtual void VScreenChanged() override;

    // A new virtual that is never overridden as there are no other known super classes
    virtual void PushEvent(GameSpeakEvents event);

    GameSpeakMatch MatchBuffer(u8* pBuffer, s16 max_idx, s16 src_idx);
    static s32 FillBuffer(s32 code, u8* pBufffer);

private:
    void PushEvent_Impl(GameSpeakEvents event);

public:
    GameSpeakEvents mLastEvent = GameSpeakEvents::eUnknown_0;
    u32 mLastEventFrame = 0;
    s32 mLastEventIndex = 0;
    s8 mEventBuffer[32] = {};
};

extern GameSpeak* gEventSystem;

s16 Code_Length(u32 code);

s32 Code_Convert(u16 code1, u16 code2);

GameSpeakEvents Code_LookUp(u32 code, u16 idx, u16 code_len);
