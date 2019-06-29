#include "stdafx.h"
#include "Paramite.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "ObjectIds.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"
#include "Throwable.hpp"

class Class_547F58 : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Class_547F58* ctor_4E1840(FP /*xpos*/, __int16 /*bottom*/, unsigned __int16 /*top*/, FP /*scale*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

private:
    __int16 field_F4;
    __int16 field_F6;
    __int16 field_F8_ttl;
    __int16 field_FA_ttl_remainder;
    int field_FC;
    int field_100;
    __int16 field_104_bEnabled;
    __int16 field_106;
};
ALIVE_ASSERT_SIZEOF(Class_547F58, 0x108);

TintEntry stru_55D73C[24] =
{
    { 3u, 105u, 105u, 105u },
    { 11u, 105u, 105u, 105u },
    { -1, 105u, 105u, 105u }
};

const TParamiteMotionFn sParamite_motion_table_55D5B0[44] = 
{
    &Paramite::M_Idle_0_489FB0,
    &Paramite::M_WalkBegin_1_48A7B0,
    &Paramite::M_Walking_2_48A2D0,
    &Paramite::M_Running_3_48AA00,
    &Paramite::M_Turn_4_48B180,
    &Paramite::M_Hop_5_48B5B0,
    &Paramite::M_UNKNOWN_6_48A930,
    &Paramite::M_WalkRunTransition_7_48B0C0,
    &Paramite::M_WalkEnd_8_48A870,
    &Paramite::M_RunBegin_9_48AF10,
    &Paramite::M_RunEnd_10_48B000,
    &Paramite::M_Falling_11_48B200,
    &Paramite::M_JumpUpBegin_12_48BE40,
    &Paramite::M_JumpUpMidair_13_48BAF0,
    &Paramite::M_JumpUpLand_14_48BF00,
    &Paramite::M_RopePull_15_48D930,
    &Paramite::M_CloseAttack_16_48DDA0,
    &Paramite::M_Landing_17_48B590,
    &Paramite::M_UNKNOWN_18_48DF60,
    &Paramite::M_Knockback_19_48BF50,
    &Paramite::M_GameSpeakBegin_20_48C010,
    &Paramite::M_PreHiss_21_48C180,
    &Paramite::M_Hiss1_22_48C3E0,
    &Paramite::M_Hiss2_23_48C600,
    &Paramite::M_Empty_24_48C680,
    &Paramite::M_AllOYaGameSpeakBegin_25_48C6A0,
    &Paramite::M_Hiss3_26_48C6F0,
    &Paramite::M_PostHiss_27_48C780,
    &Paramite::M_GameSpeakEnd_28_48C8B0,
    &Paramite::M_GetDepossessedBegin_29_48D9D0,
    &Paramite::M_GetDepossessedEnd_30_48DB50,
    &Paramite::M_RunningAttack_31_48C9E0,
    &Paramite::M_Empty_32_48D740,
    &Paramite::M_SurpriseWeb_33_48D760,
    &Paramite::M_WebLeaveDown_34_48D870,
    &Paramite::M_WebIdle_35_48D400,
    &Paramite::M_WebGoingUp_36_48D000,
    &Paramite::M_WebGoingDown_37_48CC60,
    &Paramite::M_WebGrab_38_48D6C0,
    &Paramite::M_WebLeaveUp_39_48D8C0,
    &Paramite::M_Eating_40_48A0F0,
    &Paramite::M_Death_41_48D8E0,
    &Paramite::M_UNKNOWN_42_48D900,
    &Paramite::M_Attack_43_48DB70
};

const TParamiteAIFn sParamite_ai_table_55D710[10] =
{
    &Paramite::AI_0_4835B0,
    &Paramite::AI_1_484CD0,
    &Paramite::AI_2_4859D0,
    &Paramite::AI_3_4851B0,
    &Paramite::AI_4_48F8F0,
    &Paramite::AI_5_486880,
    &Paramite::AI_6_484BC0,
    &Paramite::AI_7_484FF0,
    &Paramite::AI_8_48DFC0,
    &Paramite::AI_9_48ED80
};


const static AIFunctionData<TParamiteAIFn> sParamiteAITable[10] =
{
    { &Paramite::AI_0_4835B0, 0x402A7C, "AI_0" },
    { &Paramite::AI_1_484CD0, 0x404223, "AI_1" },
    { &Paramite::AI_2_4859D0, 0x401799, "AI_2" },
    { &Paramite::AI_3_4851B0, 0x401645, "AI_3" },
    { &Paramite::AI_4_48F8F0, 0x48F8F0, "AI_4" }, // No stub ??
    { &Paramite::AI_5_486880, 0x4012E4, "AI_5" },
    { &Paramite::AI_6_484BC0, 0x40187F, "AI_6" },
    { &Paramite::AI_7_484FF0, 0x4021A3, "AI_7" },
    { &Paramite::AI_8_48DFC0, 0x4010B4, "AI_8" },
    { &Paramite::AI_9_48ED80, 0x401EE7, "AI_9" },
};

void Paramite::SetBrain(TParamiteAIFn fn)
{
    return ::SetBrain(fn, field_128_fn_brainState, sParamiteAITable);
}

bool Paramite::BrainIs(TParamiteAIFn fn)
{
    return ::BrainIs(fn, field_128_fn_brainState, sParamiteAITable);
}


Paramite* Paramite::ctor_4879B0(Path_Paramite* pTlv, int tlvInfo)
{
    ctor_408240(16);
    field_160 = -1;
    field_174 = 0;
    field_176 = -1;

    SetVTable(this, 0x54640C);
    field_4_typeId = Types::eParamite_96;

    field_C_objectId = tlvInfo;

    field_10_resources_array.SetAt(0,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 600, 1, 0));
    field_10_resources_array.SetAt(5,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 605, 1, 0));
    field_10_resources_array.SetAt(14, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 614, 1, 0));
    field_10_resources_array.SetAt(4,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 604, 1, 0));
    field_10_resources_array.SetAt(1,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 601, 1, 0));
    field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 615, 1, 0));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 610, 1, 0));
    field_10_resources_array.SetAt(9,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 609, 1, 0));
    
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 2034);
    Animation_Init_424E10(
        96696,
        137,
        65u,
        field_10_resources_array.ItemAt(0),
        1,
        1);

    SetTint_425600(&stru_55D73C[0], gMap_5C3030.sCurrentLevelId_5C3030);

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_110_id = -1;
    field_158 = -1;
    field_15A = -1;
    field_11C_obj_id = -1;
    field_118 = -1;
    field_120_obj_id = -1;
    field_124 = -1;
    field_130_timer = 0;
    field_12C_brain_ret = 0;
    field_108_next_motion = 0;
    field_106_current_motion = 0;
    field_154 = 0;
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }

    switch (pTlv->field_12_entrace_type)
    {
    case 1:
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        SetBrain(&Paramite::AI_3_4851B0);
        break;

    case 2:
        SetBrain(&Paramite::AI_9_48ED80);
        field_BC_ypos -= FP_FromInteger(20);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    case 3:
        SetBrain(&Paramite::AI_9_48ED80);
        field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    case 4:
        SetBrain(&Paramite::AI_9_48ED80);
        field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    default:
        SetBrain(&Paramite::AI_0_4835B0);
        break;
    }

    field_136_attack_delay = pTlv->field_14_attack_delay;
    field_12E_drop_delay = pTlv->field_16_drop_delay;
    field_134_meat_eating_time = pTlv->field_18_meat_eating_time;
    field_144_attack_duration = pTlv->field_1A_attack_duration;
    field_14C_id = pTlv->field_1E_id;

    field_178_flags.Set(Flags_178::eBit1, pTlv->field_20_hiss_before_attack & 1);
    field_178_flags.Clear(Flags_178::eBit2);
    field_178_flags.Set(Flags_178::eBit4, pTlv->field_22_delete_when_far_away & 1);
    field_178_flags.Clear(Flags_178::eBit5);
    field_178_flags.Set(Flags_178::eBit8, pTlv->field_24_deadly_scratch & 1);
    field_178_flags.Clear(Flags_178::eBit6);
    field_178_flags.Clear(Flags_178::eBit7);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_F8_LastLineYPos = hitY;
        field_BC_ypos = hitY;
        field_106_current_motion = 0;
        sub_489EA0();
    }

    field_F8_LastLineYPos = field_BC_ypos;
    field_140_tlvInfo = tlvInfo;
    MapFollowMe_408D10(TRUE);

    vStackOnObjectsOfType_425840(Types::eParamite_96);

    field_DC_bApplyShadows |= 2u;
    field_15C = field_DA_xOffset;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

BaseGameObject* Paramite::VDestructor(signed int flags)
{
    return vdtor_487F90(flags);
}

void Paramite::VUpdate()
{
    vUpdate_4871B0();
}

__int16 Paramite::AI_0_4835B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_1_484CD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_2_4859D0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_3_4851B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_4_48F8F0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_5_486880()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_6_484BC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_7_484FF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_8_48DFC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_9_48ED80()
{
    NOT_IMPLEMENTED(); 
    return 0;
}

void Paramite::M_Idle_0_489FB0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_WalkBegin_1_48A7B0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Walking_2_48A2D0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Running_3_48AA00()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Turn_4_48B180()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Hop_5_48B5B0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_UNKNOWN_6_48A930()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_WalkRunTransition_7_48B0C0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_WalkEnd_8_48A870()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_RunBegin_9_48AF10()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_RunEnd_10_48B000()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Falling_11_48B200()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_JumpUpBegin_12_48BE40()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_JumpUpMidair_13_48BAF0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_JumpUpLand_14_48BF00()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_RopePull_15_48D930()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_CloseAttack_16_48DDA0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Landing_17_48B590()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_UNKNOWN_18_48DF60()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Knockback_19_48BF50()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_GameSpeakBegin_20_48C010()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_PreHiss_21_48C180()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Hiss1_22_48C3E0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Hiss2_23_48C600()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Empty_24_48C680()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_AllOYaGameSpeakBegin_25_48C6A0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Hiss3_26_48C6F0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_PostHiss_27_48C780()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_GameSpeakEnd_28_48C8B0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_GetDepossessedBegin_29_48D9D0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_GetDepossessedEnd_30_48DB50()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_RunningAttack_31_48C9E0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Empty_32_48D740()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_SurpriseWeb_33_48D760()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_WebLeaveDown_34_48D870()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_WebIdle_35_48D400()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_WebGoingUp_36_48D000()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_WebGoingDown_37_48CC60()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_WebGrab_38_48D6C0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_WebLeaveUp_39_48D8C0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Eating_40_48A0F0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Death_41_48D8E0()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_UNKNOWN_42_48D900()
{
    NOT_IMPLEMENTED();
}

void Paramite::M_Attack_43_48DB70()
{
    NOT_IMPLEMENTED();
}


void Paramite::dtor_487FC0()
{
    SetVTable(this, 0x54640C);

    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_11C_obj_id);
    if (pObj)
    {
        pObj->field_6_flags.Set(BaseGameObject::eDead);
        field_11C_obj_id = -1;
    }

    field_118 = -1;
    VOnTrapDoorOpen();

    if (field_10C_health > FP_FromInteger(0) || field_178_flags.Get(Flags_178::eBit6))
    {
        Path::TLV_Reset_4DB8E0(field_140_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_140_tlvInfo, -1, 0, 1);
    }

    SND_SEQ_Stop_4CAE60(25u);
    MusicController::sub_47FD60(0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_14E_return_level,
                field_150_return_path,
                field_152_return_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }
    dtor_4080B0();
}

Paramite* Paramite::vdtor_487F90(signed int flags)
{
    dtor_487FC0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Paramite::sub_489EA0()
{
    NOT_IMPLEMENTED();
}

void Paramite::vUpdate_4871B0()
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
        }

        field_104_collision_line_type = 0;
        field_118 = BaseGameObject::Find_Flags_4DC170(field_118);
        field_120_obj_id = BaseGameObject::Find_Flags_4DC170(field_120_obj_id);
        field_124 = BaseGameObject::Find_Flags_4DC170(field_124);

        if (field_11C_obj_id != -1)
        {
            auto pUnknown = alive_new<Class_547F58>();
            if (pUnknown)
            {
                pUnknown->ctor_4E1840(
                    field_B8_xpos,
                    FP_GetExponent(field_BC_ypos) - 20,
                    FP_GetExponent(field_BC_ypos) - 10,
                    field_CC_sprite_scale);
            }
            field_11C_obj_id = pUnknown->field_8_object_id;
        }
    }

    auto pMeat = static_cast<BaseThrowable*>(sObjectIds_5C1B70.Find_449CF0(field_118));
    if (field_10C_health > FP_FromInteger(0)
        && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
    {
        SND_SEQ_PlaySeq_4CA960(25u, 1, 0);
    }
    else if (!Find_Paramite_488810())
    {
        SND_SEQ_Stop_4CAE60(25u);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    const FP xDelta = FP_Abs(field_B8_xpos- sControlledCharacter_5C1B8C->field_B8_xpos);
    const FP yDelta = FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos);

    if (xDelta > FP_FromInteger(750) || yDelta > FP_FromInteger(390))
    {
        if (field_178_flags.Get(Flags_178::eBit4) && field_178_flags.Get(Flags_178::eBit6))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        else if (pMeat)
        {
            if (pMeat->VIsFalling_49E330() || pMeat->field_6_flags.Get(BaseGameObject::eDead))
            {
                field_118 = -1;
                field_108_next_motion = 0;
                SetBrain(&Paramite::AI_0_4835B0);
                field_12C_brain_ret = 0;
            }
        }
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        if (field_10C_health > FP_FromInteger(0))
        {
            if (!BrainIs(&Paramite::AI_9_48ED80) || field_178_flags.Get(Flags_178::eBit6))
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
        }

        if (!Input_IsChanting_45F260())
        {
            field_178_flags.Clear(Flags_178::eBit5);
        }


        if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
        {
            // Handle DDCheat mode

            VOnTrapDoorOpen();
            field_106_current_motion = 11;
            field_F8_LastLineYPos = field_BC_ypos;
            field_100_pCollisionLine = nullptr;

            static int sArray1_5C929C[8] = {};
            static int sArray2_5C92BC[8] = {};

            static char byte_5C9298 = 0;

            if (!(byte_5C9298 & 1))
            {
                byte_5C9298 |= 1u;
                sArray1_5C929C[0] = 4;
                sArray1_5C929C[1] = 4;
                sArray1_5C929C[2] = 0;
                sArray1_5C929C[3] = -4;
                sArray1_5C929C[4] = -4;
                sArray1_5C929C[5] = -4;
                sArray1_5C929C[6] = 0;
                sArray1_5C929C[7] = 4;
            }
            else if (!(byte_5C9298 & 2))
            {
                byte_5C9298 = byte_5C9298 | 2;
                sArray2_5C92BC[0] = 0;
                sArray2_5C92BC[1] = -4;
                sArray2_5C92BC[2] = -4;
                sArray2_5C92BC[3] = -4;
                sArray2_5C92BC[4] = 0;
                sArray2_5C92BC[5] = 4;
                sArray2_5C92BC[6] = 4;
                sArray2_5C92BC[7] = 4;
            }

            // TODO: InputCommand constants
            if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & 0xF)
            {
                field_C4_velx = FP_FromInteger(sArray1_5C929C[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);
                field_C8_vely = FP_FromInteger(sArray2_5C92BC[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);

                if (sInputKey_Run_5550E8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
                {
                    field_C4_velx += FP_FromInteger(sArray1_5C929C[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);
                    field_C4_velx += FP_FromInteger(sArray1_5C929C[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);
                    field_C8_vely += FP_FromInteger(sArray2_5C92BC[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5]);
                }

                field_B8_xpos += field_C4_velx;
                field_BC_ypos += field_C8_vely;

                // Keep in map bounds
                PSX_Point mapBounds = {};
                gMap_5C3030.Get_map_size_480640(&mapBounds);
                if (field_B8_xpos < FP_FromInteger(0))
                {
                    field_B8_xpos = FP_FromInteger(0);
                }

                if (field_B8_xpos >= FP_FromInteger(mapBounds.field_0_x))
                {
                    field_B8_xpos = FP_FromInteger(mapBounds.field_0_x) - FP_FromInteger(1);
                }

                if (field_BC_ypos < FP_FromInteger(0))
                {
                    field_BC_ypos = FP_FromInteger(0);
                }

                if (field_BC_ypos >= FP_FromInteger(mapBounds.field_2_y))
                {
                    field_BC_ypos = FP_FromInteger(mapBounds.field_2_y) - FP_FromInteger(1);
                    sub_408C40();
                    return;
                }
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
                field_C8_vely = FP_FromInteger(0);
            }
            sub_408C40();
        }
        else
        {
            const auto oldMotion = field_106_current_motion;
            const auto oldBrain = field_128_fn_brainState;
            field_12C_brain_ret = (this->*field_128_fn_brainState)();

            if (sDDCheat_ShowAI_Info_5C1BD8)
            {
                DDCheat::DebugStr_4F5560(
                    "Paramite %d %d %d %d\n",
                    field_12C_brain_ret,
                    field_130_timer,
                    field_106_current_motion,
                    field_108_next_motion);
            }

            const FP oldXPos = field_B8_xpos;
            const FP oldYPos = field_BC_ypos;
            (this->*sParamite_motion_table_55D5B0[field_106_current_motion])();

            // TODO: This is extra debug logging to figure out the motion names
            if (oldBrain != field_128_fn_brainState)
            {
                LOG_INFO("Paramite: Old brain = " << GetOriginalFn(oldBrain, sParamiteAITable).fnName << " new brain = " << GetOriginalFn(field_128_fn_brainState, sParamiteAITable).fnName);
                //LOG_INFO("Paramite: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
            }

            if (oldXPos != field_B8_xpos || oldYPos != field_BC_ypos)
            {
                field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                    nullptr,
                    field_B8_xpos,
                    field_BC_ypos,
                    field_B8_xpos,
                    field_BC_ypos);
                VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
            }

            if (field_114_flags.Get(Flags_114::e114_Bit1_bShot))
            {
                sub_489BB0();
                field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
                field_106_current_motion = field_108_next_motion;
                field_108_next_motion = -1;
            }

            if (oldMotion == field_106_current_motion)
            {
                if (field_178_flags.Get(Flags_178::eBit2))
                {
                    field_106_current_motion = field_F4;
                    vUpdateAnim_487170();
                    field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                    field_178_flags.Clear(Flags_178::eBit2);
                }
            }
            else
            {
                vUpdateAnim_487170();
            }
        }
    }
}

void Paramite::sub_489BB0()
{
    NOT_IMPLEMENTED();
}

__int16 Paramite::Find_Paramite_488810()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Paramite::vUpdateAnim_487170()
{
    NOT_IMPLEMENTED();
}

