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
#include "Map.hpp"
#include "Meat.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "PlatformBase.hpp"

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

#define MAKE_FN(VAR) &Paramite::VAR,

const TParamiteMotionFn sParamite_motion_table_55D5B0[44] = 
{
    PARAMITE_MOTIONS_ENUM(MAKE_FN)
};

const TParamiteAIFn sParamite_ai_table_55D710[10] =
{
    &Paramite::AI_Patrol_0_4835B0,
    &Paramite::AI_Death_1_484CD0,
    &Paramite::AI_ChasingAbe_2_4859D0,
    &Paramite::AI_SurpriseWeb_3_4851B0,
    &Paramite::AI_UNKNOWN_4_48F8F0,
    &Paramite::AI_SpottedMeat_5_486880,
    &Paramite::AI_Possessed_6_484BC0,
    &Paramite::AI_DeathDrop_7_484FF0,
    &Paramite::AI_ControlledByGameSpeak_8_48DFC0,
    &Paramite::AI_ParamiteSpawn_9_48ED80
};


const static AIFunctionData<TParamiteAIFn> sParamiteAITable[10] =
{
    { &Paramite::AI_Patrol_0_4835B0, 0x402A7C, "AI_Patrol_0" },
    { &Paramite::AI_Death_1_484CD0, 0x404223, "AI_Death_1" },
    { &Paramite::AI_ChasingAbe_2_4859D0, 0x401799, "AI_ChasingAbe_2" },
    { &Paramite::AI_SurpriseWeb_3_4851B0, 0x401645, "AI_SurpriseWeb_3" },
    { &Paramite::AI_UNKNOWN_4_48F8F0, 0x48F8F0, "AI_UNKNOWN_4" }, // Dead or unused ??
    { &Paramite::AI_SpottedMeat_5_486880, 0x4012E4, "AI_SpottedMeat_5" },
    { &Paramite::AI_Possessed_6_484BC0, 0x40187F, "AI_Possessed_6" },
    { &Paramite::AI_DeathDrop_7_484FF0, 0x4021A3, "AI_DeathDrop_7" },
    { &Paramite::AI_ControlledByGameSpeak_8_48DFC0, 0x4010B4, "AI_ControlledByGameSpeak_8" },
    { &Paramite::AI_ParamiteSpawn_9_48ED80, 0x401EE7, "AI_ParamiteSpawn_9" },
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
        SetBrain(&Paramite::AI_SurpriseWeb_3_4851B0);
        break;

    case 2:
        SetBrain(&Paramite::AI_ParamiteSpawn_9_48ED80);
        field_BC_ypos -= FP_FromInteger(20);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    case 3:
        SetBrain(&Paramite::AI_ParamiteSpawn_9_48ED80);
        field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    case 4:
        SetBrain(&Paramite::AI_ParamiteSpawn_9_48ED80);
        field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    default:
        SetBrain(&Paramite::AI_Patrol_0_4835B0);
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

__int16 Paramite::AI_Patrol_0_4835B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_Death_1_484CD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_ChasingAbe_2_4859D0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_SurpriseWeb_3_4851B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_UNKNOWN_4_48F8F0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_SpottedMeat_5_486880()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_Possessed_6_484BC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_DeathDrop_7_484FF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_ControlledByGameSpeak_8_48DFC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_ParamiteSpawn_9_48ED80()
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
                SetBrain(&Paramite::AI_Patrol_0_4835B0);
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
            if (!BrainIs(&Paramite::AI_ParamiteSpawn_9_48ED80) || field_178_flags.Get(Flags_178::eBit6))
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
    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eParamite_96 &&
            pObj != this &&
            gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pObj->field_C2_lvl_number, pObj->field_C0_path_number, pObj->field_B8_xpos, pObj->field_BC_ypos, 0))
        {
            return 1;
        }
    }
    return 0;
}

const int dword_55D660[44] =
{
    96696,
    96676,
    96548,
    96612,
    96792,
    96728,
    97164,
    97172,
    96828,
    96868,
    96848,
    96888,
    96904,
    96932,
    96944,
    96972,
    97192,
    97096,
    97120,
    97136,
    17032,
    17072,
    17088,
    17088,
    17088,
    17192,
    17088,
    17120,
    17152,
    17088,
    17120,
    19068,
    52312,
    52312,
    52344,
    52476,
    52428,
    52380,
    52548,
    52588,
    15628,
    8108,
    9636,
    10948
};

void Paramite::vUpdateAnim_487170()
{
    field_20_animation.Set_Animation_Data_409C80(dword_55D660[field_106_current_motion], ResBlockForMotion_488130(field_106_current_motion));
}

Meat* Paramite::FindMeat_488930()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eMeat_84)
        {
            auto pMeat = static_cast<Meat*>(pObj);
            if (pMeat->VCanEatMe_4696A0())
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pMeat->field_C2_lvl_number, pMeat->field_C0_path_number, pMeat->field_B8_xpos, pMeat->field_BC_ypos, 0) &&
                    !WallHit_408750(field_BC_ypos, pMeat->field_B8_xpos - field_B8_xpos))
                {
                    if (!pMeat->field_130_pLine)
                    {
                        return pMeat;
                    }

                    if (FP_Abs(pMeat->field_BC_ypos - field_BC_ypos) <= FP_FromInteger(20))
                    {
                        return pMeat;
                    }
                }
            }
        }
    }
    return nullptr;
}

__int16 Paramite::IsNear_488B10(Paramite* pOther)
{
    return FP_Abs(pOther->field_BC_ypos - field_BC_ypos) < field_CC_sprite_scale * FP_FromInteger(100);
}

__int16 Paramite::vOnSameYLevel_488A40(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther)
    {
        PSX_RECT bRect = {};
        pOther->vGetBoundingRect_424FD0(&bRect, 1);

        if ((FP_Abs(field_BC_ypos - FP_FromInteger(bRect.h)) < field_CC_sprite_scale * FP_FromInteger(40)) ||
            (pOther->field_4_typeId == Types::eParamite_96 && static_cast<Paramite*>(pOther)->field_106_current_motion == eParamiteMotions::M_JumpUpMidair_13_48BAF0))
        {
            return 1;
        }
    }
    return 0;
}

void Paramite::vUnPosses_488BE0()
{
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
    field_130_timer = sGnFrame_5C1B84 + 180;
    SetBrain(&Paramite::AI_Patrol_0_4835B0);
    field_12C_brain_ret = 0;
}

void Paramite::vPossessed_488B60()
{
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_178_flags.Set(Flags_178::eBit5);
    SetBrain(&Paramite::AI_Possessed_6_484BC0);
    field_108_next_motion = eParamiteMotions::M_Idle_0_489FB0;
    field_12C_brain_ret = 0;
    field_130_timer = sGnFrame_5C1B84 + 30;
    field_14E_return_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_150_return_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_152_return_camera = gMap_5C3030.sCurrentCamId_5C3034;
}

__int16 Paramite::vTakeDamage_488250(BaseGameObject* pFrom)
{
    if (field_10C_health <= FP_FromInteger(0))
    {
        return 1;
    }

    field_118 = -1;

    switch (pFrom->field_4_typeId)
    {
    case Types::eGrinder_30:
    case Types::eBaseBomb_46:
    case Types::eExplosion_109:
    {
        Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
        auto pGibs = alive_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_40FB40(2, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
        }
        field_10C_health = FP_FromInteger(0);
        field_6_flags.Set(BaseGameObject::eDead);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        if (sControlledCharacter_5C1B8C != this)
        {
            return 1;
        }
        SND_SEQ_Play_4CAB10(29u, 1, 127, 127);
    }
        return 1;

    case Types::eFleech_50:
    {
        field_10C_health -= FP_FromDouble(0.15);
        if (field_10C_health < FP_FromInteger(0))
        {
            field_10C_health = FP_FromInteger(0);
        }
        if (sControlledCharacter_5C1B8C != this)
        {
            if (field_120_obj_id == -1 || field_120_obj_id == sActiveHero_5C1B68->field_8_object_id)
            {
                if (BrainIs(&Paramite::AI_Patrol_0_4835B0) || BrainIs(&Paramite::AI_ControlledByGameSpeak_8_48DFC0))
                {
                    SetBrain(&Paramite::AI_ChasingAbe_2_4859D0);
                    field_12C_brain_ret = 0;
                    field_148_timer = sGnFrame_5C1B84 + field_144_attack_duration;
                    field_120_obj_id = pFrom->field_8_object_id;
                }
            }
        }

        if (field_10C_health > FP_FromInteger(0))
        {
            return 1;
        }

        Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
        SetBrain(&Paramite::AI_Death_1_484CD0);
        field_130_timer = sGnFrame_5C1B84 + 90;
        field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
        vUpdateAnim_487170();

        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 50);
        }

        if (sControlledCharacter_5C1B8C == this)
        {
            SND_SEQ_Play_4CAB10(29u, 1, 127, 127);
        }
    }
        return 0;

    case Types::eType_104:
        return 0;

    default:
    {
        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
        field_10C_health = FP_FromInteger(0);
        SetBrain(&Paramite::AI_Death_1_484CD0);
        field_130_timer = sGnFrame_5C1B84 + 90;
        field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
        vUpdateAnim_487170();

        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 50);
        }

        if (sControlledCharacter_5C1B8C != this)
        {
            return 1;
        }

        SND_SEQ_Play_4CAB10(29u, 1, 127, 127);
        return 1;
    }
    }
}

BYTE** Paramite::ResBlockForMotion_488130(__int16 motion)
{
    if (motion < eParamiteMotions::M_GameSpeakBegin_20_48C010)
    {
        return field_10_resources_array.ItemAt(0);
    }
    if (motion < eParamiteMotions::M_RunningAttack_31_48C9E0)
    {
        return field_10_resources_array.ItemAt(5);
    }
    if (motion < eParamiteMotions::M_Empty_32_48D740)
    {
        return field_10_resources_array.ItemAt(14);
    }
    if (motion < eParamiteMotions::M_Eating_40_48A0F0)
    {
        return field_10_resources_array.ItemAt(4);
    }
    if (motion < eParamiteMotions::M_Death_41_48D8E0)
    {
        return field_10_resources_array.ItemAt(1);
    }
    if (motion < eParamiteMotions::M_UNKNOWN_42_48D900)
    {
        return field_10_resources_array.ItemAt(15);
    }
    if (motion >= eParamiteMotions::M_Attack_43_48DB70)
    {
        return field_10_resources_array.ItemAt(motion >= 44 ? 0 : 9);
    }
    return field_10_resources_array.ItemAt(10);
}

void Paramite::vOn_TLV_Collision_488640(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
                field_12C_brain_ret = 0;
                SetBrain(&Paramite::AI_DeathDrop_7_484FF0);
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                field_106_current_motion = eParamiteMotions::M_Death_41_48D8E0;
                Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
            }
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, field_B8_xpos, field_BC_ypos, field_B8_xpos, field_BC_ypos);
    }
}

__int16 Paramite::AnotherParamiteNear_4886E0()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eParamite_96 && pObj != this)
        {
            auto pOther = static_cast<Paramite*>(pObj);
            if (pOther->field_CC_sprite_scale == field_CC_sprite_scale && 
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pOther->field_C2_lvl_number, pOther->field_C0_path_number, pOther->field_B8_xpos, pOther->field_BC_ypos, 0)  &&
                gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) &&
                IsNear_488B10(pOther))
            {
                if (pOther->BrainIs(&Paramite::AI_Patrol_0_4835B0) || pOther->BrainIs(&Paramite::AI_ChasingAbe_2_4859D0))
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

PathLine* Paramite::Collision_4888A0(FP yOff, FP xOff)
{
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos + yOff,
        field_B8_xpos + xOff,
        field_BC_ypos + yOff, &pLine, &hitX, &hitY, 0x100))
    {
        return 0;
    }

    field_B8_xpos = hitX;
    field_BC_ypos = hitY;
    return pLine;
}

void Paramite::vRender_488220(int** ot)
{
    if (field_1C_update_delay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void Paramite::vOnTrapDoorOpen_489F60()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        field_106_current_motion = eParamiteMotions::M_Falling_11_48B200;
    }
}

void Paramite::ToHop_489C20()
{
    VOnTrapDoorOpen();
    field_106_current_motion = eParamiteMotions::M_Hop_5_48B5B0;
    field_108_next_motion = -1;
    field_F8_LastLineYPos = field_BC_ypos;
    field_C8_vely = FP_FromDouble(-6.3) * field_CC_sprite_scale;
    field_BC_ypos += field_C8_vely;
    field_100_pCollisionLine = nullptr;
}
