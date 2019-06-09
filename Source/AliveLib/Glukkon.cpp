#include "stdafx.h"
#include "Glukkon.hpp"
#include "Function.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Shadow.hpp"
#include "MusicController.hpp"
#include "SwitchStates.hpp"
#include "Spark.hpp"
#include "ParticleBurst.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "ObjectIds.hpp"
#include "LiftPoint.hpp"

#define MAKE_STRINGS(VAR) #VAR,
const char* const sGlukkonMotionNames[25] =
{
    GLUKKON_MOTIONS_ENUM(MAKE_STRINGS)
};

const TGlukkonMotionFn sGlukkon_motion_table_5544C0[25] = 
{
    &Glukkon::M_0_Idle_442D10,
    &Glukkon::M_1_Walk_442D30,
    &Glukkon::M_2_Turn_442F10,
    &Glukkon::M_3_KnockBack_442F40,
    &Glukkon::M_4_Jump_443030,
    &Glukkon::M_5_JumpToFall_4434C0,
    &Glukkon::M_6_WalkToFall_4434E0,
    &Glukkon::M_7_Fall_443510,
    &Glukkon::M_8_DeathFall_443760,
    &Glukkon::M_9_Land_443790,
    &Glukkon::M_10_ChantShake_443B50,
    &Glukkon::M_11_Speak1_4437D0,
    &Glukkon::M_12_Speak2_4438F0,
    &Glukkon::M_13_LongLaugh_443930,
    &Glukkon::M_14_BeginWalk_443950,
    &Glukkon::M_15_EndWalk_443970,
    &Glukkon::M_16_StandToJump_4439B0,
    &Glukkon::M_17_JumpToStand_4439D0,
    &Glukkon::M_18_WalkToJump_443A00,
    &Glukkon::M_19_JumpToWalk_443A30,
    &Glukkon::M_20_KnockBackStandBegin_442FC0,
    &Glukkon::M_21_GetShot_443A60,
    &Glukkon::M_22_KnockBackStandEnd_443010,
    &Glukkon::M_23_Speak3_443910,
    &Glukkon::M_24_EndSingleStep_443990
};

const int dword_554524[4][25] =
{
    {
        169608, 169644, 169724, 169772, 169828,
        169900, 169920, 169952, 169968, 170000,
        170152, 170216, 170268, 170056, 170312,
        170332, 170352, 170372, 170392, 170412,
        170432, 170152, 170472, 170504, 170560
    },                          
    {                           
        171356, 171392, 171472, 171520, 171576,
        171648, 171668, 171700, 171716, 171748,
        171900, 171964, 172016, 171804, 172060,
        172080, 172100, 172120, 172140, 172160,
        172180, 171900, 172220, 172252, 172308
    },                          
    {                           
        193064, 193100, 193180, 193228, 193284,
        193356, 193376, 193408, 193424, 193456,
        193608, 193672, 193724, 193512, 193768,
        193788, 193808, 193828, 193848, 193868,
        193888, 193608, 193928, 193960, 194016
    },                          
    {                           
        199500, 199536, 199616, 199664, 199720,
        199792, 199812, 199844, 199860, 199892,
        200044, 200108, 200160, 199948, 200204,
        200224, 200244, 200264, 200284, 200304,
        200324, 200044, 200364, 200396, 200452
    }
};

TintEntry stru_5546B4[18] =
{
    { 1u, 137u, 137u, 137u },
    { 2u, 137u, 137u, 137u },
    { 3u, 137u, 137u, 137u },
    { 4u, 137u, 137u, 137u },
    { 5u, 137u, 137u, 137u },
    { 6u, 137u, 137u, 137u },
    { 7u, 137u, 137u, 137u },
    { 8u, 137u, 137u, 137u },
    { 9u, 137u, 137u, 137u },
    { 10u, 137u, 137u, 137u },
    { 11u, 137u, 137u, 137u },
    { 12u, 137u, 137u, 137u },
    { 13u, 137u, 137u, 137u },
    { 14u, 137u, 137u, 137u },
    { -1, 137u, 137u, 137u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};


struct FnPair
{
    TGlukkonAIFn mOurFn;
    DWORD mOriginal;
};

static FnPair sAiFns[6] =
{
    { &Glukkon::AI_0_Calm_WalkAround_440B40, 0x402D60 },
    { &Glukkon::AI_1_Panic_4412F0, 0x403049 },
    { &Glukkon::AI_2_441720, 0x403864 },
    { &Glukkon::AI_3_PlayerControlled_441A30, 0x401BF4 },
    { &Glukkon::AI_4_Death_442010, 0x401CE9 },
    { &Glukkon::AI_5_WaitToSpawn_442490, 0x40357B }
};

#if _WIN32 || !_WIN64
static DWORD GetOriginalFn(TGlukkonAIFn fn)
{
    // If not running as standalone set the address to be
    // the address of the real function rather than the reimpl as the real
    // game code compares the function pointer addresses (see IsBrain(x)).
    for (const auto& addrPair : sAiFns)
    {
        if (addrPair.mOurFn == fn)
        {
            const DWORD actualAddressToUse = addrPair.mOriginal;
            // Hack to overwrite the member function pointer bytes with arbitrary data
            return actualAddressToUse;
        }
    }
    ALIVE_FATAL("No matching address!");
}
#endif

void Glukkon::SetBrain(TGlukkonAIFn fn)
{
#if _WIN32 || !_WIN64
    if (IsAlive())
    {
        const DWORD actualAddressToUse = GetOriginalFn(fn);
        // Hack to overwrite the member function pointer bytes with arbitrary data
        memcpy(&field_20C_brain_state_fn, &actualAddressToUse, sizeof(DWORD));
        return;
    }
#endif
    field_20C_brain_state_fn = fn;
}

bool Glukkon::BrainIs(TGlukkonAIFn fn)
{
#if _WIN32 || !_WIN64
    if (IsAlive())
    {
        const DWORD actualAddressToUse = GetOriginalFn(fn);
        TGlukkonAIFn hack = nullptr;
        memcpy(&hack, &actualAddressToUse, sizeof(DWORD));
        return hack == field_20C_brain_state_fn;
    }
#endif
    return field_20C_brain_state_fn == fn;
}


Glukkon* Glukkon::ctor_43F030(Path_Glukkon* pTlv, int tlvInfo)
{
    ctor_408240(0);
    field_1EC = -1;
    SetVTable(this, 0x5452E0);

    field_1A8_tlvData = *pTlv;

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_214_tlv_info = tlvInfo;

    // TODO: Resource IDs
    switch (field_1A8_tlvData.field_22_glukkon_type)
    {
    case GlukkonTypes::Normal_0:
    case GlukkonTypes::Normal_4:
    case GlukkonTypes::Normal_5:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 801);
        Animation_Init_424E10(169608, 163, 79, Add_Resource_4DC130(ResourceManager::Resource_Animation, 800), 1, 1);
        break;

    case GlukkonTypes::Aslik_1:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 803);
        Animation_Init_424E10(171356, 157, 76, Add_Resource_4DC130(ResourceManager::Resource_Animation, 802), 1, 1);
        break;

    case GlukkonTypes::Drpik_2:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 805);
        Animation_Init_424E10(193064,162,78, Add_Resource_4DC130(ResourceManager::Resource_Animation, 804), 1, 1);
        break;

    case GlukkonTypes::Phleg_3:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 807);
        Animation_Init_424E10(199500, 145, 72, Add_Resource_4DC130(ResourceManager::Resource_Animation, 806), 1, 1);
        break;
    default:
        break;
    }
    
    Init_43F260();

    return this;
}

BaseGameObject* Glukkon::VDestructor(signed int flags)
{
    return vdtor_43F230(flags);
}

void Glukkon::VUpdate()
{
    vUpdate_43F770();
}

void Glukkon::VPossessed_408F70()
{
    vPossessed_440160();
}

void Glukkon::M_0_Idle_442D10()
{
    HandleInput_443BB0();
}

void Glukkon::M_1_Walk_442D30()
{
    if (DoMovement_444190())
    {
        switch (field_20_animation.field_92_current_frame)
        {
        case 0:
        case 9:
            if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion == eGlukkonMotions::M_4_Jump_443030)
                {
                    SetAnim_43F9C0(eGlukkonMotions::M_18_WalkToJump_443A00);
                }
            }
            else if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::eRun)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_18_WalkToJump_443A00, TRUE);
            }

            MapFollowMe_408D10(TRUE);
            return;

        case 2:
        case 6:
        case 10:
        case 14:
            PlaySound_4447D0(0, this);
            field_212++;
            return;

        case 8:
        case 17:
            if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion == eGlukkonMotions::M_0_Idle_442D10 ||
                    field_108_next_motion == eGlukkonMotions::M_2_Turn_442F10 ||
                    field_108_next_motion == eGlukkonMotions::M_11_Speak1_4437D0 ||
                    field_108_next_motion == eGlukkonMotions::M_12_Speak2_4438F0 ||
                    field_108_next_motion == eGlukkonMotions::M_23_Speak3_443910 ||
                    field_108_next_motion == eGlukkonMotions::M_13_LongLaugh_443930)
                {
                    if (field_20_animation.field_92_current_frame != 8)
                    {
                        SetAnim_43F9C0(eGlukkonMotions::M_15_EndWalk_443970);
                    }
                    else
                    {
                        SetAnim_43F9C0(eGlukkonMotions::M_24_EndSingleStep_443990);
                    }
                }
            }
            else
            {
                if (field_C4_velx > FP_FromInteger(0) && (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::eLeft) ||
                    field_C4_velx < FP_FromInteger(0) && (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::eRight) ||
                    !(sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (InputCommands::eLeft | InputCommands::eRight)))
                {
                    if (field_20_animation.field_92_current_frame == 8)
                    {
                        SetAnim_43F9C0(eGlukkonMotions::M_24_EndSingleStep_443990, TRUE);
                    }
                    else
                    {
                        SetAnim_43F9C0(eGlukkonMotions::M_15_EndWalk_443970, TRUE);
                    }
                }
            }
            break;

        default:
            return;
        }
    }
}

void Glukkon::M_2_Turn_442F10()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_3_KnockBack_442F40()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_4_Jump_443030()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_5_JumpToFall_4434C0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_6_WalkToFall_4434E0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_7_Fall_443510()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_8_DeathFall_443760()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_9_Land_443790()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_10_ChantShake_443B50()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_11_Speak1_4437D0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_12_Speak2_4438F0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_13_LongLaugh_443930()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_14_BeginWalk_443950()
{
    M_19_JumpToWalk_443A30();
}

void Glukkon::M_15_EndWalk_443970()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_16_StandToJump_4439B0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_17_JumpToStand_4439D0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_18_WalkToJump_443A00()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_19_JumpToWalk_443A30()
{
    DoMovement_444190();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim_43F9C0(eGlukkonMotions::M_1_Walk_442D30);
    }
}

void Glukkon::M_20_KnockBackStandBegin_442FC0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_21_GetShot_443A60()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_22_KnockBackStandEnd_443010()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_23_Speak3_443910()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_24_EndSingleStep_443990()
{
    NOT_IMPLEMENTED();
}

__int16 Glukkon::AI_0_Calm_WalkAround_440B40()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(4, this, 0, 0);
    }

    auto pObj = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    LiftPoint* pLiftPoint = nullptr;
    if (pObj && pObj->field_4_typeId == Types::eLiftPoint_78)
    {
        pLiftPoint = static_cast<LiftPoint*>(pObj);
        if (!pLiftPoint->vOnAnyFloor_461920() && field_210 != 7)
        {
            field_108_next_motion = eGlukkonMotions::M_0_Idle_442D10;
            return 7;
        }
    }

    if (!field_100_pCollisionLine)
    {
        return 8;
    }

    if (sActiveHero_5C1B68->field_10C_health < FP_FromInteger(0))
    {
        Speak_444640(7);
        SetBrain(&Glukkon::AI_4_Death_442010);
        return 6;
    }

    BaseAnimatedWithPhysicsGameObject* pEvent17 = nullptr;

    switch (field_210)
    {
    case 0:
        if (field_106_current_motion != eGlukkonMotions::M_0_Idle_442D10)
        {
            return field_210;
        }

        if (sub_440200(0))
        {
            if (Event_Is_Event_In_Range_422C30(
                kEventAbeOhm,
                field_B8_xpos,
                field_BC_ypos,
                field_D6_scale))
            {
                field_1D4_timer = sGnFrame_5C1B84 + 10;
            }
            else
            {
                field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_16_pre_alarmed_delay;
            }
            Speak_444640(0);
            SetBrain(&Glukkon::AI_1_Panic_4412F0);
            return 0;
        }

        pEvent17 = Event_Is_Event_In_Range_422C30(
            kEventUnknown17,
            field_B8_xpos,
            field_BC_ypos,
            field_D6_scale);
        if (pEvent17 && pEvent17 != this)
        {
            field_1D4_timer = sGnFrame_5C1B84 + 20;
            return 9;
        }

        if (field_1A8_tlvData.field_14_default_behaviour)
        {
            if (Check_IsOnEndOfLine_408E90(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked_4442F0(field_C4_velx, 1))
            {
                field_108_next_motion = eGlukkonMotions::M_2_Turn_442F10;
                return 2;
            }
            field_108_next_motion = eGlukkonMotions::M_14_BeginWalk_443950;
            return 1;
        }
        else
        {
            field_108_next_motion = eGlukkonMotions::M_0_Idle_442D10;
            return 1;
        }
        break;

    case 1:
        if (sub_440200(0))
        {
            if (Event_Is_Event_In_Range_422C30(
                kEventAbeOhm,
                field_B8_xpos,
                field_BC_ypos,
                field_D6_scale))
            {
                field_1D4_timer = sGnFrame_5C1B84 + 10;
            }
            else
            {
                field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_16_pre_alarmed_delay;
            }

            Speak_444640(0);
            SetBrain(&Glukkon::AI_1_Panic_4412F0);
            return 0;
        }
        else
        {
            auto pEvent17_1 = Event_Is_Event_In_Range_422C30(
                kEventUnknown17,
                field_B8_xpos,
                field_BC_ypos,
                field_D6_scale);
            if (pEvent17_1 && pEvent17_1 != this)
            {
                field_1D4_timer = sGnFrame_5C1B84 + 20;
                return 9;
            }

            if (field_1A8_tlvData.field_14_default_behaviour == 1)
            {
                if (Check_IsOnEndOfLine_408E90(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked_4442F0(field_C4_velx, 1))
                {
                    if (static_cast<int>(sGnFrame_5C1B84) <= field_1F0)
                    {
                        field_108_next_motion = eGlukkonMotions::M_0_Idle_442D10;
                        field_1D4_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 120);
                        return 4;
                    }
                    else
                    {
                        field_1F0 = sGnFrame_5C1B84 + 120;
                        SpeakRandomish_4405D0();
                        return 3;
                    }
                }
            }
            else
            {
                if (Math_NextRandom() < 5 && static_cast<int>(sGnFrame_5C1B84) > field_1F4)
                {
                    field_1F4 = sGnFrame_5C1B84 + 120;
                    field_108_next_motion = eGlukkonMotions::M_2_Turn_442F10;
                    return 2;
                }
            }

            if (Math_NextRandom() >= 5 || static_cast<int>(sGnFrame_5C1B84) <= field_1F0)
            {
                return field_210;
            }

            field_1F0 = sGnFrame_5C1B84 + 120;
            SpeakRandomish_4405D0();
            return 6;
        }
        break;

    case 2:
    case 8:
        if (field_106_current_motion != eGlukkonMotions::M_0_Idle_442D10)
        {
            return field_210;
        }
        return 0;

    case 3:
        if (field_106_current_motion != eGlukkonMotions::M_0_Idle_442D10 || field_1EA_speak != -1)
        {
            return field_210;
        }
        field_1D4_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 120);
        return 4;

    case 4:
        if (sub_440200(0))
        {
            if (Event_Is_Event_In_Range_422C30(
                kEventAbeOhm,
                field_B8_xpos,
                field_BC_ypos,
                field_D6_scale))
            {
                field_1D4_timer = sGnFrame_5C1B84 + 10;
            }
            else
            {
                field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_16_pre_alarmed_delay;
            }
            Speak_444640(0);
            SetBrain(&Glukkon::AI_1_Panic_4412F0);
            return 0;
        }

        pEvent17 = Event_Is_Event_In_Range_422C30(
            kEventUnknown17,
            field_B8_xpos,
            field_BC_ypos,
            field_D6_scale);
        if (pEvent17 && pEvent17 != this)
        {
            field_1D4_timer = sGnFrame_5C1B84 + 20;
            return 9;
        }
        else
        {
            if (static_cast<int>(sGnFrame_5C1B84) <= field_1D4_timer)
            {
                return field_210;
            }
            field_108_next_motion = eGlukkonMotions::M_2_Turn_442F10;
            return 2;
        }
        break;

    case 5:
        if (sub_440200(0))
        {
            if (Event_Is_Event_In_Range_422C30(
                kEventAbeOhm,
                field_B8_xpos,
                field_BC_ypos,
                field_D6_scale))
            {
                field_1D4_timer = sGnFrame_5C1B84 + 10;
            }
            else
            {
                field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_16_pre_alarmed_delay;
            }
            Speak_444640(0);
            SetBrain(&Glukkon::AI_1_Panic_4412F0);
            return 0;
        }
        else
        {
            auto pEvent17_3 = Event_Is_Event_In_Range_422C30(
                kEventUnknown17,
                field_B8_xpos,
                field_BC_ypos,
                field_D6_scale);
            if (pEvent17_3 && pEvent17_3 != this)
            {
                field_1D4_timer = sGnFrame_5C1B84 + 20;
                return 9;
            }
            else
            {
                if (static_cast<int>(sGnFrame_5C1B84) <= field_1D4_timer)
                {
                    return field_210;
                }
                return 0;
            }
        }
        break;

    case 6:
        if (field_106_current_motion != eGlukkonMotions::M_0_Idle_442D10)
        {
            return field_210;
        }
        field_1D4_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 120);
        return 5;

    case 7:
        if (pLiftPoint)
        {
            if (!pLiftPoint->vOnAnyFloor_461920())
            {
                return field_210;
            }
            return 0;
        }
        else
        {
            field_110_id = -1;
            return 0;
        }
        break;

    case 9:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_1D4_timer)
        {
            return field_210;
        }
        field_1FC = 1;
        Speak_444640(5);
        return 6;

    default:
        return field_210;
    }
}

__int16 Glukkon::AI_1_Panic_4412F0()
{
    NOT_IMPLEMENTED(); 
    return 0;
}

__int16 Glukkon::AI_2_441720()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Glukkon::AI_3_PlayerControlled_441A30()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Glukkon::AI_4_Death_442010()
{
    NOT_IMPLEMENTED();
    return 0;
}

const PSX_Point v00554768[8] =
{
    { -25,  -5 },
    { -30,  -22 },
    { -25,  -39 },
    { -10,  -51 },
    { 6,    -39 },
    { 9,    -23 },
    { 5,    -6 },
    { 0,     0 }
};


__int16 Glukkon::AI_5_WaitToSpawn_442490()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(0, this, 0, 0);
    }

    if (field_210 == 0)
    {
        if (!SwitchStates_Get_466020(field_1A8_tlvData.field_1C_spawn_id))
        {
            return field_210;
        }
        field_1D4_timer = sGnFrame_5C1B84 + field_1A8_tlvData.field_20_spawn_delay;
        return 1;
    }
    else if (field_210 == 2)
    {
        if (field_106_current_motion != eGlukkonMotions::M_0_Idle_442D10 || field_1EA_speak != -1)
        {
            return field_210;
        }
        SetBrain(&Glukkon::AI_0_Calm_WalkAround_440B40);
        field_210 = 0;
        return field_210;
    }
    else if (field_210 == 1)
    {
        if (static_cast<int>(sGnFrame_5C1B84) <= field_1D4_timer)
        {
            return field_210;
        }

        field_6_flags.Set(BaseGameObject::eDrawable);
        field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);

        field_4_typeId = Types::eGlukkon_67;

        if (field_1A8_tlvData.field_1E_spawn_direction == 3)
        {
            SFX_Play_46FBA0(49u, 60, -300);

            for (const auto& p : v00554768)
            {
                const short sparkX = FP_GetExponent(FP_FromInteger(p.field_0_x) + field_B8_xpos + FP_FromInteger(13));
                const short sparkY = FP_GetExponent(field_BC_ypos + FP_FromInteger(p.field_2_y) - FP_FromInteger(11));
                auto pSpark = alive_new<Spark>();
                if (pSpark)
                {
                    pSpark->ctor_4CBBB0(FP_FromInteger(sparkX), FP_FromInteger(sparkY), FP_FromInteger(1), 9, -31, 159, 1);
                }
            }

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            New_Particle_426F40(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2) + (field_CC_sprite_scale * FP_FromInteger(60)),
                field_CC_sprite_scale);

            auto pParticleBurst = alive_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    field_B8_xpos,
                    field_BC_ypos - FP_FromInteger(18),
                    6,
                    FP_FromInteger(1),
                    BurstType::eBigRedSparks_3,
                    9);
            }
            Speak_444640(5);
            return 2;
        }
        else
        {
            SFX_Play_46FA90(112u, 0);
            Speak_444640(5);
            return 2;
        }
    }
    return field_210;
}

void Glukkon::Init_43F260()
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_6_flags.Set(BaseGameObject::eDrawable);

    SetTint_425600(&stru_5546B4[0], gMap_5C3030.sCurrentLevelId_5C3030);
    field_B8_xpos = FP_FromInteger((field_1A8_tlvData.field_8_top_left.field_0_x  + field_1A8_tlvData.field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(field_1A8_tlvData.field_8_top_left.field_2_y);
    
    if (field_1A8_tlvData.field_12_start_direction == 1)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    if (field_1A8_tlvData.field_1C_spawn_id)
    {
        if (field_1A8_tlvData.field_1E_spawn_direction == 1)
        {
            field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }
        else if (field_1A8_tlvData.field_1E_spawn_direction == 2)
        {
            field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        field_6_flags.Clear(BaseGameObject::eDrawable);
        SetBrain(&Glukkon::AI_5_WaitToSpawn_442490);
        field_210 = 0;
        field_4_typeId = Types::eNone_0;
    }
    else
    {
        field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
        field_4_typeId = Types::eGlukkon_67;
        SetBrain(&Glukkon::AI_0_Calm_WalkAround_440B40);
        field_210 = 0;
    }

    if (field_1A8_tlvData.field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 8;
    }
    else if (field_1A8_tlvData.field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 27;
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(79),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }

    SetAnim_43F9C0(0, TRUE);

    field_208_obj_id = -1;
    field_110_id = -1;
    field_1D4_timer = 0;
    field_10C_health = FP_FromInteger(1);
    field_1F0 = 0;
    field_1F4 = 0;
    field_1F8 = 0;
    field_1EA_speak = -1;
    field_1E0 = 0;
    field_1FC = 0;
    field_200 = 0;
    field_204 = 0;
    field_1DC = field_BC_ypos;

    if (!field_E0_pShadow)
    {
        field_E0_pShadow = alive_new<Shadow>();
        if (field_E0_pShadow)
        {
            field_E0_pShadow->ctor_4AC990();
        }
    }
}

Glukkon* Glukkon::vdtor_43F230(signed int flags)
{
    dtor_43F570();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Glukkon::dtor_43F570()
{
    SetVTable(this, 0x5452E0);

    if (field_10C_health <= FP_FromInteger(0))
    {
        Path::TLV_Reset_4DB8E0(field_214_tlv_info, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_214_tlv_info, -1, 0, 0);
    }

    field_6_flags.Set(BaseGameObject::eDrawable); // Seems wrong to do this here ??

    if (this == sControlledCharacter_5C1B8C)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
    }

    dtor_4080B0();
}

void Glukkon::vUpdate_43F770()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);

            if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);

                PSX_Point xy = { bRect.x, bRect.y };
                PSX_Point wh = { bRect.w, bRect.h };
                xy.field_2_y += 5;
                wh.field_2_y += 5;

                vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }
        field_208_obj_id = BaseGameObject::Find_Flags_4DC170(field_208_obj_id);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (!Input_IsChanting_45F260())
        {
            field_1E2_bUnknown = 0;
        }

        const auto oldMotion = field_106_current_motion;

        field_210 = (this->*field_20C_brain_state_fn)();

        const FP oldXPos = field_B8_xpos;
        const FP oldYPos = field_BC_ypos;

        (this->*sGlukkon_motion_table_5544C0[field_106_current_motion])();

        // TODO: This is extra debug logging to figure out the motion names
        if (oldMotion != field_106_current_motion)
        {
            LOG_INFO("Glukkon: Old motion = " << sGlukkonMotionNames[oldMotion] << " new motion = " << sGlukkonMotionNames[field_106_current_motion]);
        }

        if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
        {
            Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            VOn_TLV_Collision_4087F0(pTlv);
        }
        
        sub_440600();
        
        if (sControlledCharacter_5C1B8C == this && field_110_id != -1)
        {
            field_C8_vely = field_BC_ypos - field_1DC;
            sub_408C40();
        }
        field_1DC = field_BC_ypos;
    }
}

void Glukkon::vPossessed_440160()
{
    SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_switch_id, SwitchOp::eSetFalse_1);
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_1E2_bUnknown = 1;
    SetAnim_43F9C0(eGlukkonMotions::M_10_Shake_443B50, TRUE);
    SetBrain(&Glukkon::AI_3_PlayerControlled_441A30);
    field_210 = 0;
    field_1D4_timer = sGnFrame_5C1B84 + 35;
    field_1E4_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_1E6_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_1E8_camera = gMap_5C3030.sCurrentCamId_5C3034;
}

void Glukkon::sub_440600()
{
    NOT_IMPLEMENTED();
}

void Glukkon::SetAnim_43F9C0(__int16 currentMotion, __int16 bClearNextMotion)
{
    int typeIndex = static_cast<int>(field_1A8_tlvData.field_22_glukkon_type);
    if (typeIndex > 3)
    {
        typeIndex = 0;
    }
    field_20_animation.Set_Animation_Data_409C80(dword_554524[typeIndex][currentMotion], nullptr);

    field_106_current_motion = currentMotion;
    if (bClearNextMotion)
    {
        field_108_next_motion = -1;
    }
}

void Glukkon::Speak_444640(unsigned __int8 /*speak*/)
{
    NOT_IMPLEMENTED();
}

void Glukkon::HandleInput_443BB0()
{
    MapFollowMe_408D10(TRUE);

    if (BrainIs(&Glukkon::AI_3_PlayerControlled_441A30) && field_210 == 1 && !(field_114_flags.Get(Flags_114::e114_Bit10)))
    {
        const auto inputHeld = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
        const auto matchButtons = 
            InputCommands::eGameSpeak1 |
            InputCommands::eGameSpeak2 |
            InputCommands::eGameSpeak3 |
            InputCommands::eGameSpeak4 |
            InputCommands::eGameSpeak5 |
            InputCommands::eGameSpeak6 |
            InputCommands::eGameSpeak7 |
            InputCommands::eGameSpeak8 |
            InputCommands::eChant;

        if (inputHeld & matchButtons)
        {
            field_1EA_speak = -1;

            if (inputHeld & InputCommands::eGameSpeak1)
            {
                field_1EA_speak = 0;
            }
            else if (inputHeld & InputCommands::eGameSpeak2)
            {
                field_1EA_speak = 3;
            }
            else if (inputHeld & InputCommands::eGameSpeak3)
            {
                field_1EA_speak = 2;
            }
            else if (inputHeld & InputCommands::eGameSpeak4)
            {
                field_1EA_speak = 1;
            }
            else if (inputHeld & InputCommands::eGameSpeak5)
            {
                field_1EA_speak = 8;
            }
            else if (inputHeld & InputCommands::eGameSpeak7)
            {
                field_1EA_speak = 6;
            }
            else if (inputHeld & InputCommands::eGameSpeak8)
            {
                field_1EA_speak = 7;
            }

            else if (inputHeld & InputCommands::eGameSpeak6)
            {
                field_1EA_speak = 4;
            }

            if (field_1EA_speak != -1)
            {
                Speak_444640(field_1EA_speak);
            }
        }
        else
        {
            const auto inputPressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            if (inputPressed & InputCommands::eRight)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_108_next_motion = eGlukkonMotions::M_2_Turn_442F10;
                }
                else
                {
                    field_108_next_motion = eGlukkonMotions::M_14_BeginWalk_443950;
                }
            }
            else if (inputPressed & InputCommands::eLeft)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_108_next_motion = eGlukkonMotions::M_14_BeginWalk_443950;
                }
                else
                {
                    field_108_next_motion = eGlukkonMotions::M_2_Turn_442F10;
                }
            }

            if (inputPressed & InputCommands::eRun)
            {
                if (field_108_next_motion == eGlukkonMotions::M_1_Walk_442D30)
                {
                    field_108_next_motion = eGlukkonMotions::M_4_Jump_443030;
                }
            }

            if (inputPressed & InputCommands::eHop)
            {
                field_108_next_motion = eGlukkonMotions::M_4_Jump_443030;
            }
        }
    }

    switch (field_108_next_motion)
    {
    case -1:
        if (field_106_current_motion == eGlukkonMotions::M_0_Idle_442D10)
        {
            return;
        }
        field_108_next_motion = eGlukkonMotions::M_0_Idle_442D10;
        SetAnim_43F9C0(field_108_next_motion, TRUE);
        break;

    case eGlukkonMotions::M_0_Idle_442D10:
    case eGlukkonMotions::M_2_Turn_442F10:
    case eGlukkonMotions::M_11_Speak1_4437D0:
    case eGlukkonMotions::M_12_Speak2_4438F0:
    case eGlukkonMotions::M_13_LongLaugh_443930:
    case eGlukkonMotions::M_23_Speak3_443910:
        SetAnim_43F9C0(field_108_next_motion, TRUE);
        break;

    case eGlukkonMotions::M_1_Walk_442D30:
    case eGlukkonMotions::M_14_BeginWalk_443950:
        if (field_106_current_motion != eGlukkonMotions::M_1_Walk_442D30)
        {
            field_212 = 0;
        }
        // Fall through

    case eGlukkonMotions::M_4_Jump_443030:
    case eGlukkonMotions::M_16_StandToJump_4439B0:
    {
        FP xOff = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            xOff = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            xOff = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        if (!Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), xOff))
        {
            SetAnim_43F9C0(field_108_next_motion, TRUE);
        }
    }
        break;

    default:
        return;
    }

}

__int16 Glukkon::sub_440200(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Glukkon::PathBlocked_4442F0(FP /*a2*/, __int16 /*checkBounds*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void Glukkon::SpeakRandomish_4405D0()
{
    if (sGnFrame_5C1B84 & 1)
    {
        if ((sGnFrame_5C1B84 & 1) == 1)
        {
            Speak_444640(5);
        }
    }
    else
    {
        Speak_444640(7);
    }
}

const FP sWalkData_545354[18] =
{
    FP_FromInteger(0),
    FP_FromDouble(2.11),
    FP_FromDouble(3.92),
    FP_FromDouble(2.11),
    FP_FromDouble(0.82),
    FP_FromDouble(4.99),
    FP_FromDouble(6.02),
    FP_FromDouble(2.92),
    FP_FromDouble(2.11),
    FP_FromDouble(3.92),
    FP_FromDouble(2.11),
    FP_FromDouble(0.82),
    FP_FromDouble(4.99),
    FP_FromDouble(6.02),
    FP_FromDouble(2.92),
    FP_FromDouble(2.11),
    FP_FromDouble(3.92),
    FP_FromDouble(2.11)
};

const FP sJumpData_54539C[16] =
{
    FP_FromDouble(10.33),
    FP_FromDouble(21.65),
    FP_FromDouble(18.93),
    FP_FromDouble(12.51),
    FP_FromDouble(10.81),
    FP_FromDouble(7.71),
    FP_FromDouble(5.19),
    FP_FromDouble(4.02),
    FP_FromDouble(2.60),
    FP_FromDouble(0.92),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)
};

// These tables just contain all zeros
// TODO/NOTE: These are all pointless - the logic in 0x444190 will use 0 if there is no table
const FP sJumpToFallData_54542C[10] = {};
const FP sWalkToFallData_54541C[4] = {};
const FP sBeginWalkData_545454[6] = { };
const FP sEndWalkData_54546C[6] = { };
const FP sStandToJumpData_545484[6] = { };
const FP sJumpToStandData_54549C[6] = { };
const FP sWalkToJumpData_5454B4[6] = { };
const FP sJumpToWalkData_5454CC[26] = { };

const FP* motion_velx_table_5547C4[25] =
{
    nullptr,                    // M_0_Idle_442D10
    sWalkData_545354,           // M_1_Walk_442D30
    nullptr,                    // M_2_Turn_442F10
    nullptr,                    // M_3_KnockBack_442F40
    sJumpData_54539C,           // M_4_Jump_443030
    sJumpToFallData_54542C,     // M_5_JumpToFall_4434C0
    sWalkToFallData_54541C,     // M_6_WalkToFall_4434E0
    nullptr,                    // M_7_Fall_443510
    nullptr,                    // M_8_DeathFall_443760
    nullptr,                    // M_9_Land_443790
    nullptr,                    // M_10_ChantShake_443B50
    nullptr,                    // M_11_Speak1_4437D0
    nullptr,                    // M_12_Speak2_4438F0
    nullptr,                    // M_13_LongLaugh_443930
    sBeginWalkData_545454,      // M_14_BeginWalk_443950
    sEndWalkData_54546C,        // M_15_EndWalk_443970
    sStandToJumpData_545484,    // M_16_StandToJump_4439B0
    sJumpToStandData_54549C,    // M_17_JumpToStand_4439D0
    sWalkToJumpData_5454B4,     // M_18_WalkToJump_443A00
    sJumpToWalkData_5454CC,     // M_19_JumpToWalk_443A30
    nullptr,                    // M_20_KnockBackStandBegin_442FC0
    nullptr,                    // M_21_GetShot_443A60
    nullptr,                    // M_22_KnockBackStandEnd_443010
    nullptr,                    // M_23_Speak3_443910
    nullptr                     // M_24_EndSingleStep_443990
};


__int16 Glukkon::DoMovement_444190()
{
    const FP* pTable = motion_velx_table_5547C4[field_106_current_motion];
    if (pTable)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = -pTable[field_20_animation.field_92_current_frame];
        }
        else
        {
            field_C4_velx = pTable[field_20_animation.field_92_current_frame];
        }
    }
    else
    {
        field_C4_velx = FP_FromInteger(0);
    }

    field_C4_velx = field_C4_velx * field_CC_sprite_scale;

    if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        field_1D8 = 0;
        field_C8_vely = FP_FromInteger(0);
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        else
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        MapFollowMe_408D10(TRUE);
        SetAnim_43F9C0(eGlukkonMotions::M_3_KnockBack_442F40, TRUE);
        return FALSE;
    }
    else
    {
        FollowLine_443EB0();
        return field_100_pCollisionLine != nullptr;
    }
}

void Glukkon::FollowLine_443EB0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    const FP prevXPos = field_B8_xpos;

    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        if (field_100_pCollisionLine)
        {
            if (pPlatform)
            {
                if (field_100_pCollisionLine->field_8_type != 32 && field_100_pCollisionLine->field_8_type != 36)
                {
                    pPlatform->VRemove(this);
                    field_110_id = -1;
                    field_1D8 = 22937;
                }
            }
            else if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                sub_444060();
            }
        }
        else
        {
            field_F8_LastLineYPos = field_BC_ypos;
            
            VOnTrapDoorOpen();

            if (field_106_current_motion == eGlukkonMotions::M_1_Walk_442D30 ||
                field_106_current_motion == eGlukkonMotions::M_14_BeginWalk_443950 ||
                field_106_current_motion == eGlukkonMotions::M_15_EndWalk_443970 ||
                field_106_current_motion == eGlukkonMotions::M_18_WalkToJump_443A00)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_6_WalkToFall_4434E0, TRUE);
            }
            else if (field_106_current_motion == eGlukkonMotions::M_4_Jump_443030 ||
                     field_106_current_motion == eGlukkonMotions::M_16_StandToJump_4439B0 ||
                     field_106_current_motion == eGlukkonMotions::M_17_JumpToStand_4439D0 ||
                     field_106_current_motion == eGlukkonMotions::M_19_JumpToWalk_443A30)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_5_JumpToFall_4434C0, TRUE);
            }
            else
            {
                SetAnim_43F9C0(eGlukkonMotions::M_7_Fall_443510, TRUE);
            }

            field_B8_xpos = prevXPos + field_C4_velx;
            field_1D8 = 0x10000;

            if (field_106_current_motion == eGlukkonMotions::M_3_KnockBack_442F40)
            {
                field_1D8 = 0xAB85;
            }
        }
    }
    else
    {
        field_F8_LastLineYPos = field_BC_ypos;
        SetAnim_43F9C0(eGlukkonMotions::M_7_Fall_443510, TRUE);
    }
}

void Glukkon::sub_444060()
{
    NOT_IMPLEMENTED();
}

void CC Glukkon::PlaySound_4447D0(int /*sndIdx*/, Glukkon* /*pGlukkon*/)
{
    NOT_IMPLEMENTED();
}
