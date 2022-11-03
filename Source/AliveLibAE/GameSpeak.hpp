#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Game.hpp"

enum class GameSpeakEvents : s16
{
    eSameAsLast_m2 = -2,
    eNone_m1 = -1,
    eUnknown_0 = 0,
    eUnknown_1 = 1,
    eUnknown_2 = 2,
    eFart_3 = 3,
    eUnknown_4 = 4,
    Slig_BS_5 = 5,
    Slig_LookOut_6 = 6,
    Slig_BS2_7 = 7,
    Slig_Laugh_8 = 8,
    eHello_9 = 9,
    eFollowMe_10 = 10,
    eAnger_11 = 11,
    eWait_12 = 12,
    eUnknown_13 = 13,
    eUnknown_14 = 14,
    eUnknown_15 = 15,
    eUnknown_16 = 16,
    eUnknown_17 = 17,
    eUnknown_18 = 18,
    eUnknown_19 = 19,
    eUnknown_20 = 20,
    eWork_21 = 21,
    eStopIt_22 = 22,
    eAllYa_23 = 23,
    eSorry_24 = 24,
    eUnknown_25 = 25,
    eUnknown_26 = 26,
    Slig_Hi_27 = 27,
    Slig_HereBoy_28 = 28,
    Slig_GetEm_29 = 29,
    eUnknown_30 = 30,
    Slig_Freeze_31 = 31,
    eUnknown_32 = 32,
    eUnknown_33 = 33,
    eUnknown_34 = 34,
    eUnknown_35 = 35, //Glukkon_None_35?
    Glukkon_Hey_36 = 36,
    Glukkon_DoIt_37 = 37,
    Glukkon_StayHere_38 = 38,
    Glukkon_Commere_39 = 39,
    Glukkon_AllOYa_40 = 40,
    Glukkon_Heh_41 = 41,
    Glukkon_Help_42 = 42,
    Glukkon_Laugh_43 = 43,
    Glukkon_KillEm_44 = 44,
    Glukkon_Unknown_45 = 45,
    Glukkon_Unknown_46 = 46,
    Glukkon_What_47 = 47,

    Paramite_Howdy_48 = 48,
    Paramite_Stay_49 = 49,
    Paramite_CMon_or_Attack_50 = 50,
    Paramite_DoIt_51 = 51,
    Paramite_AllYa_52 = 52,

    Scrab_Howl_53 = 53,
    Scrab_Shriek_54 = 54,
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
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    // A new virtual that is never overridden as there are no other known super classes
    virtual void PushEvent(GameSpeakEvents event);

    GameSpeakMatch MatchBuffer(u8* pBuffer, s16 max_idx, s16 src_idx);
    static s32 FillBuffer(s32 code, u8* pBufffer);

private:
    void PushEvent_Impl(GameSpeakEvents event);

public:
    GameSpeakEvents field_20_last_event = GameSpeakEvents::eUnknown_0;
    u32 field_24_last_event_frame = 0;
    s32 field_28_last_event_index = 0;
    s8 field_2C_event_buffer[32] = {};
};

extern GameSpeak* gEventSystem;

s16 Code_Length(u32 code);

s32 Code_Convert(u16 code1, u16 code2);

GameSpeakEvents Code_LookUp(u32 code, u16 idx, u16 code_len);
