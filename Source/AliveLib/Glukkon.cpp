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
    &Glukkon::M_5_4434C0,
    &Glukkon::M_6_WalkToFall_4434E0,
    &Glukkon::M_7_Fall_443510,
    &Glukkon::M_8_443760,
    &Glukkon::M_9_Land_443790,
    &Glukkon::M_10_Die_443B50,
    &Glukkon::M_11_Speak1_4437D0,
    &Glukkon::M_12_Speak2_4438F0,
    &Glukkon::M_13_LongLaugh_443930,
    &Glukkon::M_14_BeginWalk_443950,
    &Glukkon::M_15_EndWalk_443970,
    &Glukkon::M_16_4439B0,
    &Glukkon::M_17_EndJump_4439D0,
    &Glukkon::M_18_BeginRunJump_443A00,
    &Glukkon::M_19_443A30,
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
    { &Glukkon::AI_0_440B40, 0x402D60 },
    { &Glukkon::AI_1_4412F0, 0x403049 },
    { &Glukkon::AI_2_441720, 0x403864 },
    { &Glukkon::AI_3_PlayerControlled_441A30, 0x401BF4 },
    { &Glukkon::AI_4_442010, 0x401CE9 },
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

void Glukkon::M_0_Idle_442D10()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_1_Walk_442D30()
{
    NOT_IMPLEMENTED();
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

void Glukkon::M_5_4434C0()
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

void Glukkon::M_8_443760()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_9_Land_443790()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_10_Die_443B50()
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
    NOT_IMPLEMENTED();
}

void Glukkon::M_15_EndWalk_443970()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_16_4439B0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_17_EndJump_4439D0()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_18_BeginRunJump_443A00()
{
    NOT_IMPLEMENTED();
}

void Glukkon::M_19_443A30()
{
    NOT_IMPLEMENTED();
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

__int16 Glukkon::AI_0_440B40()
{
    NOT_IMPLEMENTED(); 
    return 0;
}

__int16 Glukkon::AI_1_4412F0()
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

__int16 Glukkon::AI_4_442010()
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
        SetBrain(&Glukkon::AI_0_440B40);
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
        SetBrain(&Glukkon::AI_0_440B40);
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
    NOT_IMPLEMENTED();
}
