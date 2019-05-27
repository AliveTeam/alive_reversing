#pragma once

#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "BaseAliveGameObject.hpp"

struct Path_NakedSlig : public Path_TLV
{
    enum class Scale : __int16
    {
        Full_0 = 0,
        Half_1 = 1,
    };
    Scale field_10_scale;

    enum class Direction : __int16
    {
        Left_0 = 0,
        Right_1 = 1,
    };
    Direction field_12_direction;

    enum class State : __int16
    {
        Awake_0 = 0,
        Sleep_1 = 1,
        State_2 = 2, // ??
    };
    State field_14_state;

    enum class LockerDirection : __int16
    {
        Left_0 = 0,
        Right_1 = 1,
        Search_2 = 2,
    };
    LockerDirection field_16_locker_direction;

    __int16 field_18_panic_id;

    enum class ResetOnDeath : __int16
    {
        Yes_1 = 1,
        No_0 = 0,
    };
    ResetOnDeath field_1A_reset_on_death;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_NakedSlig, 0x1C);

class NakedSlig;

using TNakedSligAIFn = __int16 (NakedSlig::*)();
using TNakedSligMotionFn = void (NakedSlig::*)();

enum class LevelIds : __int16;

class NakedSlig : public BaseAliveGameObject
{
public:
    EXPORT NakedSlig* ctor_418C70(Path_NakedSlig* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

private:
    EXPORT void Set_AnimAndMotion_419890(__int16 currentMotion, __int16 bClearNextMotion);

    EXPORT void UpdateAnimBlock_419900();

    EXPORT BYTE** GetAnimBlock_419950(int currentMotion);

    EXPORT static __int16 CC NextRandom_4197D0();

    EXPORT void vUpdate_419100();



private:
    void SetBrain(TNakedSligAIFn fn);
    bool BrainIs(TNakedSligAIFn fn);

    EXPORT void dtor_418FE0();

    EXPORT NakedSlig* vdtor_418FB0(signed int flags);

public:
    EXPORT __int16 AI_0_419DE0();
    EXPORT __int16 AI_1_419F60();
    EXPORT __int16 AI_2_419FE0();
    EXPORT __int16 AI_3_41A5B0();
    EXPORT __int16 AI_4_41A880();
    EXPORT __int16 AI_5_41ADF0();


    EXPORT void M_0_41B260();
    EXPORT void M_1_41B890();
    EXPORT void M_2_41BF00();
    EXPORT void M_3_41B280();
    EXPORT void M_4_41B620();
    EXPORT void M_5_41B650();
    EXPORT void M_6_41B870();
    EXPORT void M_7_41C010();
    EXPORT void M_8_41BF70();
    EXPORT void M_9_41BD80();
    EXPORT void M_10_41B400();
    EXPORT void M_11_41B590();
    EXPORT void M_12_418C30();
    EXPORT void M_13_418C50();
    EXPORT void M_14_41C040();
    EXPORT void M_15_41B600();
    EXPORT void M_16_41B3C0();
    EXPORT void M_17_41B3A0();

private:
    __int16 field_116;
    int field_118_tlvInfo;
    __int16 field_11C_pal_buf[64];
    PSX_RECT field_19C_pal_rect;

    __int16 field_1A4_r;
    __int16 field_1A6_g;
    __int16 field_1A8_b;

    __int16 field_1AA;
    int field_1AC_timer;
    int field_1B0;
    int field_1B4;
    __int16 field_1B8;

    LevelIds field_1BA_prev_level;
    __int16 field_1BC_prev_path;
    __int16 field_1BE_prev_camera;

    char field_1C0;
    char field_1C1;
    __int16 field_1C2_pitch;
    __int16 field_1C4;
    __int16 field_1C6;
    int field_1C8;
    __int16 field_1CC;
    __int16 field_1CE;
    int field_1D0_slig_button_id;
    int field_1D4_obj_id;
    int field_1D8_obj_id;
    int field_1DC;
    Path_NakedSlig::LockerDirection field_1E0_locker_direction;
    __int16 field_1E2;
    Path_TLV* field_1E4_pPantsOrWingsTlv;
    Path_NakedSlig field_1E8_tlv;
    TNakedSligAIFn field_204_brain_state;
    __int16 field_208_brain_sub_state;
    __int16 field_20A;
};
ALIVE_ASSERT_SIZEOF(NakedSlig, 0x20C);
