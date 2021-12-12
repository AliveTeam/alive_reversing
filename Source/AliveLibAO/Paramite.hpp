#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

void Paramite_ForceLink();

namespace AO {

// TODO: check me, enum taken from AE
enum class ParamiteSpeak : u8
{
    CMon_or_Attack_0 = 0,
    Stay_1 = 1,
    DoIt_2 = 2,
    LoudStep_3 = 3,
    SlightStep_4 = 4,
    Howdy_5 = 5,
    ClimbingWeb_6 = 6,
    DetectedMeat_7 = 7,
    None_8 = 8,
    AllYa_9 = 9,
    None_10 = 10,
    None_11 = 11,
};

#define PARAMITE_MOTIONS_ENUM(ENTRY)         \
    ENTRY(Motion_0_Idle_44B900)              \
    ENTRY(Motion_1_WalkBegin_44BCA0)         \
    ENTRY(Motion_2_Walking_44B9E0)           \
    ENTRY(Motion_3_Running_44C070)           \
    ENTRY(Motion_4_Unknown_44B6C0)           \
    ENTRY(Motion_5_Turn_44C8E0)              \
    ENTRY(Motion_6_Hop_44CB20)               \
    ENTRY(Motion_7_Unknown_44BF10)           \
    ENTRY(Motion_8_WalkRunTransition_44C790) \
    ENTRY(Motion_9_WalkEnd_44BDE0)           \
    ENTRY(Motion_10_RunBegin_44C4C0)         \
    ENTRY(Motion_11_RunEnd_44C620)           \
    ENTRY(Motion_12_Falling_44C960)          \
    ENTRY(Motion_13_GameSpeakBegin_44D050)   \
    ENTRY(Motion_14_PreHiss_44D170)          \
    ENTRY(Motion_15_Hiss_44D300)             \
    ENTRY(Motion_16_PostHiss_44D440)         \
    ENTRY(Motion_17_GameSpeakEnd_44D4F0)     \
    ENTRY(Motion_18_RunningAttack_44D5D0)    \
    ENTRY(Motion_19_Empty_44D990)            \
    ENTRY(Motion_20_SurpriseWeb_44D9A0)      \
    ENTRY(Motion_21_WebLeaveDown_44DB00)         \
    ENTRY(Motion_22_Unknown_44D8F0)          \
    ENTRY(Motion_23_Eating_44B970)           \
    ENTRY(Motion_24_Struggle_44DB70)         \
    ENTRY(Motion_25_Death_44DB90)

#define MAKE_ENUM(VAR) VAR,
enum eParamiteMotions : s32
{
    PARAMITE_MOTIONS_ENUM(MAKE_ENUM)
};

class ParamiteWeb;
class Meat;

struct Path_Paramite final : public Path_TLV
{
    Scale_short field_18_scale;
    Choice_short field_1A_bEnter_from_web;
    s16 field_1C_alone_chase_delay;
    s16 field_1E_surprise_web_delay_timer;
    s16 field_20_meat_eating_time;
    s16 field_22_group_chase_delay;
    s16 field_24_disabled_resources; // Not actually used
    s16 field_26_id;
    Choice_short field_28_hiss_before_attack;
    Choice_short field_2A_delete_when_far_away;
};
ALIVE_ASSERT_SIZEOF(Path_Paramite, 0x2C);

class Paramite final : public BaseAliveGameObject
{
public:
    EXPORT Paramite* ctor_44A7A0(Path_Paramite* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_44AB00();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Paramite* Vdtor_44B300(s32 flags);

    EXPORT u8** ResBlockForMotion_44AC10(s16 motion);

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_44ACA0(PrimHeader** ppOt);

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_44ACC0(BaseGameObject* pFrom);

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    EXPORT void VOn_Tlv_Collision_44AF30(Path_TLV* pTlv);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_44B2C0();

    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;

    EXPORT s16 VOnSameYLevel_44B240(BaseAnimatedWithPhysicsGameObject* pOther);

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_44B8C0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_44A490();

    EXPORT void ToIdle_44B580();

    EXPORT s16 ToNextMotion_44B320();

    EXPORT void VUpdateAnimData_44A460();

    EXPORT s16 AnotherParamiteNear_44AF80();

    EXPORT void ToKnockBack_44B5B0();

    EXPORT Meat* FindMeat_44B160();

    EXPORT void MoveOnLine_44B740();

    EXPORT void Sound_44DBB0(ParamiteSpeak idx);

    EXPORT void ToHop_44B660();

    void SetMusic();

    // Brains
    EXPORT s16 Brain_0_Patrol_447A10();
    EXPORT s16 Brain_1_SurpriseWeb_448D00();
    EXPORT s16 Brain_2_Struggling_44DD70();
    EXPORT s16 Brain_3_Death_448BF0();
    EXPORT s16 Brain_4_ChasingAbe_449170();
    EXPORT s16 Brain_5_SpottedMeat_449CD0();


    using TParamiteBrain = decltype(&Paramite::Brain_0_Patrol_447A10);

    void SetBrain(TParamiteBrain fn);

    bool BrainIs(TParamiteBrain fn);

    enum class DirToCheck
    {
        Left,
        Right,
    };

    s16 HandleEnemyStopper(s16 numGridBlocks, Path_EnemyStopper::StopDirection dir);

    // Motions
    EXPORT void Motion_0_Idle_44B900();
    EXPORT void Motion_1_WalkBegin_44BCA0();
    EXPORT void Motion_2_Walking_44B9E0();
    EXPORT void Motion_3_Running_44C070();
    EXPORT void Motion_4_Unknown_44B6C0();
    EXPORT void Motion_5_Turn_44C8E0();
    EXPORT void Motion_6_Hop_44CB20();
    EXPORT void Motion_7_Unknown_44BF10();
    EXPORT void Motion_8_WalkRunTransition_44C790();
    EXPORT void Motion_9_WalkEnd_44BDE0();
    EXPORT void Motion_10_RunBegin_44C4C0();
    EXPORT void Motion_11_RunEnd_44C620();
    EXPORT void Motion_12_Falling_44C960();
    EXPORT void Motion_13_GameSpeakBegin_44D050();
    EXPORT void Motion_14_PreHiss_44D170();
    EXPORT void Motion_15_Hiss_44D300();
    EXPORT void Motion_16_PostHiss_44D440();
    EXPORT void Motion_17_GameSpeakEnd_44D4F0();
    EXPORT void Motion_18_RunningAttack_44D5D0();
    EXPORT void Motion_19_Empty_44D990();
    EXPORT void Motion_20_SurpriseWeb_44D9A0();
    EXPORT void Motion_21_WebLeaveDown_44DB00();
    EXPORT void Motion_22_Unknown_44D8F0();
    EXPORT void Motion_23_Eating_44B970();
    EXPORT void Motion_24_Struggle_44DB70();
    EXPORT void Motion_25_Death_44DB90();


    TParamiteBrain field_10C_fn;
    s16 field_110_brain_sub_state;
    s16 field_112_surprise_web_delay_timer;
    s32 field_114_timer;
    s32 field_118_not_used;
    s16 field_11C_meat_eating_time;
    s16 field_11E_alone_chase_delay;
    s32 field_120_wait_timer;
    FP field_124_XSpeed;
    s16 field_128_never_read;
    s16 field_12A_res_idx;
    s32 field_12C_tlvInfo;
    s32 field_130_not_used;
    s32 field_134_group_chase_delay;
    s32 field_138_attack_timer;
    s16 field_13C_id;
    Choice_short field_13E_hiss_before_attack;
    s16 field_140_use_prev_motion;
    s16 field_142_bSnapped;
    Choice_short field_144_delete_when_far_away;
    s16 field_146_not_used;
    Meat* field_148_pMeat;
    ParamiteWeb* field_14C_pWeb;
    u8** field_150_resources[16];
};
ALIVE_ASSERT_SIZEOF(Paramite, 0x190);

} // namespace AO
