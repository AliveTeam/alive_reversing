#include "stdafx.h"
#include "Glukkon.hpp"
#include "Function.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Shadow.hpp"
#include "ShadowZone.hpp"
#include "MusicController.hpp"
#include "SwitchStates.hpp"
#include "Spark.hpp"
#include "ParticleBurst.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "ObjectIds.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "Slurg.hpp"
#include "SlamDoor.hpp"
#include "DeathFadeOut.hpp"
#include "ScreenManager.hpp"
#include "Movie.hpp"
#include "DDCheat.hpp"
#include "PsxDisplay.hpp"
#include "Sound.hpp"
#include "MainMenu.hpp"
#include "GameSpeak.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Bullet.hpp"
#include "VRam.hpp"

#define MAKE_STRINGS(VAR) #VAR,
const char* const sGlukkonMotionNames[25] =
{
    GLUKKON_MOTIONS_ENUM(MAKE_STRINGS)
};

const TGlukkonMotionFn sGlukkon_motion_table_5544C0[25] = 
{
    &Glukkon::M_Idle_0_442D10,
    &Glukkon::M_Walk_1_442D30,
    &Glukkon::M_Turn_2_442F10,
    &Glukkon::M_KnockBack_3_442F40,
    &Glukkon::M_Jump_4_443030,
    &Glukkon::M_JumpToFall_5_4434C0,
    &Glukkon::M_WalkToFall_6_4434E0,
    &Glukkon::M_Fall_7_443510,
    &Glukkon::M_DeathFall_8_443760,
    &Glukkon::M_Land_9_443790,
    &Glukkon::M_ChantShake_10_443B50,
    &Glukkon::M_Speak1_11_4437D0,
    &Glukkon::M_Speak2_12_4438F0,
    &Glukkon::M_LongLaugh_13_443930,
    &Glukkon::M_BeginWalk_14_443950,
    &Glukkon::M_EndWalk_15_443970,
    &Glukkon::M_StandToJump_16_4439B0,
    &Glukkon::M_JumpToStand_17_4439D0,
    &Glukkon::M_WalkToJump_18_443A00,
    &Glukkon::M_JumpToWalk_19_443A30,
    &Glukkon::M_KnockBackStandBegin_20_442FC0,
    &Glukkon::M_GetShot_21_443A60,
    &Glukkon::M_KnockBackStandEnd_22_443010,
    &Glukkon::M_Speak3_23_443910,
    &Glukkon::M_EndSingleStep_24_443990
};

const TGlukkonAIFn sGlukkon_ai_table_5544A0[6] =
{
    &Glukkon::AI_0_Calm_WalkAround_440B40,
    &Glukkon::AI_1_Panic_4412F0,
    &Glukkon::AI_2_Slapped_441720,
    &Glukkon::AI_3_PlayerControlled_441A30,
    &Glukkon::AI_4_Death_442010,
    &Glukkon::AI_5_WaitToSpawn_442490
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

signed int CC Glukkon::CreateFromSaveState_442830(const BYTE* pData)
{
    const Glukkon_SaveState* pSaveState = reinterpret_cast< const Glukkon_SaveState* >( pData );
    auto pTlv = static_cast< Path_Glukkon* >( sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pSaveState->field_44_tlvInfo) );

    switch (gMap_5C3030.sCurrentLevelId_5C3030)
    {
        case LevelIds::eFeeCoDepot_5:
        case LevelIds::eFeeCoDepot_Ender_12:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kGlukAslikResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("ASLIK.BND", 0);
            }
            break;
        case LevelIds::eBarracks_6:
        case LevelIds::eBarracks_Ender_13:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kGlukDripikResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("DRIPIK.BND", 0);
            }
            break;
        case LevelIds::eBonewerkz_8:
        case LevelIds::eBonewerkz_Ender_14:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kGlukPhlegResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("PHLEG.BND", 0);
            }
            break;
        default:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kGlkbasicResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("GLUKKON.BND", 0);
            }
            break;
    }

    auto pGlukkon = alive_new<Glukkon>();
    if (pGlukkon)
    {
        pGlukkon->ctor_43F030(pTlv, pSaveState->field_44_tlvInfo);
    }
    pGlukkon->field_4_typeId = pSaveState->field_8E;
    pGlukkon->field_C_objectId = pSaveState->field_4_object_id;
    if (pSaveState->field_40_bIsActiveChar)
    {
        sControlledCharacter_5C1B8C = pGlukkon;
    }

    pGlukkon->field_FC_pPathTLV = nullptr;
    pGlukkon->field_100_pCollisionLine = nullptr;

    pGlukkon->field_B8_xpos = pSaveState->field_8_xpos;
    pGlukkon->field_BC_ypos = pSaveState->field_C_ypos;

    pGlukkon->field_C4_velx = pSaveState->field_10_xvel;
    pGlukkon->field_C8_vely = pSaveState->field_14_yvel;

    pGlukkon->field_1D8 = pSaveState->field_58;
    pGlukkon->field_C0_path_number = pSaveState->field_18_path;
    pGlukkon->field_C2_lvl_number = pSaveState->field_1A_level;
    pGlukkon->field_CC_sprite_scale = pSaveState->field_1C_sprite_scale;

    pGlukkon->field_D0_r = pSaveState->field_20_r;
    pGlukkon->field_D2_g = pSaveState->field_22_g;
    pGlukkon->field_D4_b = pSaveState->field_24_b;

    pGlukkon->field_1A0_red = pSaveState->field_20_r;
    pGlukkon->field_1A2_green = pSaveState->field_22_g;
    pGlukkon->field_1A4_blue = pSaveState->field_24_b;

    pGlukkon->field_106_current_motion = pSaveState->field_28_current_motion;

    GlukkonTypes glukType = pGlukkon->field_1A8_tlvData.field_22_glukkon_type;
    if (glukType > GlukkonTypes::Phleg_3)
    {
        glukType = GlukkonTypes::Normal_0;
    }

    pGlukkon->field_20_animation.Set_Animation_Data_409C80(dword_554524[static_cast< int >( glukType )][pSaveState->field_28_current_motion], 0);
    pGlukkon->field_20_animation.field_92_current_frame = pSaveState->field_2A_current_frame;
    pGlukkon->field_20_animation.field_E_frame_change_counter = pSaveState->field_2C_frame_change_counter;
    pGlukkon->field_6_flags.Set(BaseGameObject::Options::eDrawable, pSaveState->field_2F & 1);
    pGlukkon->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pSaveState->field_26_flipX & 1);
    pGlukkon->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pSaveState->field_2E & 1);

    auto pHeader = reinterpret_cast<AnimationHeader*>(&(pGlukkon->field_20_animation.field_20_ppBlock[pGlukkon->field_20_animation.field_18_frame_table_offset]));
    if (pGlukkon->field_20_animation.field_92_current_frame == pHeader->field_2_num_frames - 1)
    {
        pGlukkon->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pGlukkon->field_10C_health = pSaveState->field_30_health;
    pGlukkon->field_106_current_motion = pSaveState->field_34_current_motion;
    pGlukkon->field_108_next_motion = pSaveState->field_36_next_motion;
    pGlukkon->field_F8_LastLineYPos = FP_FromInteger(pSaveState->field_38);
    pGlukkon->field_114_flags.Set(Flags_114::e114_Bit9);
    pGlukkon->field_1D4_timer = pSaveState->field_54;
    pGlukkon->field_104_collision_line_type = pSaveState->field_3A_line_type;
    pGlukkon->field_214_tlv_info = pSaveState->field_44_tlvInfo;
    pGlukkon->SetBrain(sGlukkon_ai_table_5544A0[pSaveState->field_48_brain_state_idx]);
    pGlukkon->field_210 = pSaveState->field_50;
    pGlukkon->field_1E2_bUnknown = pSaveState->field_5E;
    pGlukkon->field_1E4_level = pSaveState->field_60_level;
    pGlukkon->field_1E6_path = pSaveState->field_62_path;
    pGlukkon->field_1E8_camera = pSaveState->field_64_camera;
    pGlukkon->field_1EA_speak = pSaveState->field_66_speak;
    pGlukkon->field_1E0 = pSaveState->field_68;
    pGlukkon->field_1DC = pSaveState->field_6C;
    pGlukkon->field_1F0 = pSaveState->field_70;
    pGlukkon->field_1F4 = pSaveState->field_74;
    pGlukkon->field_1F8 = pSaveState->field_78;
    pGlukkon->field_1FC = pSaveState->field_7C;
    pGlukkon->field_200 = pSaveState->field_80;
    pGlukkon->field_204 = pSaveState->field_84;
    pGlukkon->field_208_obj_id = pSaveState->field_88_obj_id;
    pGlukkon->field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed, pSaveState->field_8C );

    return sizeof(Glukkon_SaveState);
}

const static AIFunctionData<TGlukkonAIFn> sGlukkonAITable[6] =
{
    { &Glukkon::AI_0_Calm_WalkAround_440B40, 0x402D60, "AI_0_Calm_WalkAround" },
    { &Glukkon::AI_1_Panic_4412F0, 0x403049, "AI_1_Panic" },
    { &Glukkon::AI_2_Slapped_441720, 0x403864, "AI_2_Slapped" },
    { &Glukkon::AI_3_PlayerControlled_441A30, 0x401BF4, "AI_3_PlayerControlled" },
    { &Glukkon::AI_4_Death_442010, 0x401CE9, "AI_4_Death" },
    { &Glukkon::AI_5_WaitToSpawn_442490, 0x40357B, "AI_5_WaitToSpawn" }
};

void Glukkon::SetBrain(TGlukkonAIFn fn)
{
    ::SetBrain(fn, field_20C_brain_state_fn, sGlukkonAITable);
}

bool Glukkon::BrainIs(TGlukkonAIFn fn)
{
    return ::BrainIs(fn, field_20C_brain_state_fn, sGlukkonAITable);
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

void Glukkon::vRender_4406C0(int** ot)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap_5C3030.sCurrentPathId_5C3032 == field_C0_path_number &&
            gMap_5C3030.sCurrentLevelId_5C3030 == field_C2_lvl_number &&
            Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
        {
            field_20_animation.field_14_scale = field_CC_sprite_scale;
            
            PSX_RECT boundingRect = {};
            vGetBoundingRect_424FD0(&boundingRect, 1);
            __int16 rMod = field_D0_r;
            __int16 gMod = field_D2_g;
            __int16 bMod = field_D4_b;
            ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                FP_GetExponent(field_B8_xpos),
                (boundingRect.h + boundingRect.y) / 2,
                field_D6_scale,
                &rMod,
                &gMod,
                &bMod
            );
            if (!( field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted )))
            {
                if (rMod != field_1A0_red || gMod != field_1A2_green || bMod != field_1A4_blue)
                {
                    field_1A0_red = rMod;
                    field_1A2_green = gMod;
                    field_1A4_blue = bMod;

                    FrameInfoHeader *pFrameInfoHeader = field_20_animation.Get_FrameHeader_40B730(0);
                    int frameHeaderOffset = pFrameInfoHeader->field_0_frame_header_offset;
                    BYTE* pAnimData = *field_20_animation.field_20_ppBlock;
                    DWORD clut_offset = *reinterpret_cast<DWORD*>(&(pAnimData)[frameHeaderOffset]);
                    WORD* pAnimDataWithOffset = reinterpret_cast<WORD*>(&pAnimData[clut_offset + 4]);
                    WORD* pCurrentAnimData = pAnimDataWithOffset;
                    for (WORD& palValue : field_118_pPalAlloc)
                    {
                        __int32 pal_value = *pCurrentAnimData & 0x1F;
                        unsigned __int16 resultR = static_cast<__int16>(pal_value * field_1A0_red) >> 7;
                        if (resultR > 31)
                        {
                            resultR = 31;
                        }

                        pal_value = ( *pCurrentAnimData >> 5) & 0x1F;
                        unsigned __int16 resultG = static_cast< __int16 >(pal_value * field_1A2_green) >> 7;
                        if (resultG > 31)
                        {
                            resultG = 31;
                        }

                        pal_value = ( *pCurrentAnimData >> 10) & 0x1F;
                        unsigned __int16 resultB = static_cast< __int16 >(pal_value * field_1A4_blue) >> 7;
                        if (resultB > 31)
                        {
                            resultB = 31;
                        }

                        int resultMixed = (*pCurrentAnimData & 0x8000) | ((resultR & 31) + 32 * (resultG & 31) + 32 * 32 * (resultB & 31));
                        if (!resultMixed && *pCurrentAnimData)
                        {
                            resultMixed = 1;
                        }
                        palValue = static_cast<WORD>(resultMixed);
                        pCurrentAnimData += 1;
                    }

                    if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Aslik_1)
                    {
                        field_118_pPalAlloc[63] = pAnimDataWithOffset[63];
                    }
                    else if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Phleg_3)
                    {
                        field_118_pPalAlloc[63] = pAnimDataWithOffset[63];
                    }
                    else
                    {
                        field_118_pPalAlloc[62] = pAnimDataWithOffset[62];
                    }
                    Pal_Set_483510(
                        field_20_animation.field_8C_pal_vram_xy,
                        field_20_animation.field_90_pal_depth,
                        reinterpret_cast<const BYTE*>(&field_118_pPalAlloc),
                        &field_198
                    );
                }
                field_20_animation.field_8_r = 127;
                field_20_animation.field_9_g = 127;
                field_20_animation.field_A_b = 127;
            }

            field_20_animation.vRender_40B820(
                FP_GetExponent(FP_FromInteger(field_DA_xOffset) + field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                FP_GetExponent(FP_FromInteger(field_D8_yOffset) + field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                ot,
                0,
                0
            );

            PSX_RECT rectToInvalidate;
            field_20_animation.Get_Frame_Rect_409E10(&rectToInvalidate);
            pScreenManager_5BB5F4->InvalidateRect_40EC90(
                rectToInvalidate.x,
                rectToInvalidate.y,
                rectToInvalidate.w,
                rectToInvalidate.h,pScreenManager_5BB5F4->field_3A_idx
            );

            if (field_E0_pShadow)
            {
                field_E0_pShadow->Calculate_Position_4ACA50(field_B8_xpos, field_BC_ypos, &rectToInvalidate, field_CC_sprite_scale, field_D6_scale);
                field_E0_pShadow->Render_4ACE60(ot);
            }
        }
    }
}

void Glukkon::VScreenChanged()
{
    vScreenChanged_440110();
}

int Glukkon::vGetSaveState_444B90(BYTE* pSaveBuffer)
{
    Glukkon_SaveState* pSaveState = reinterpret_cast< Glukkon_SaveState* >( pSaveBuffer );

    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }
    pSaveState->field_0_id = Types::eGlukkon_67;
    pSaveState->field_4_object_id = field_C_objectId;
    pSaveState->field_8_xpos = field_B8_xpos;
    pSaveState->field_C_ypos = field_BC_ypos;
    pSaveState->field_10_xvel = field_C4_velx;
    pSaveState->field_14_yvel = field_C8_vely;
    pSaveState->field_18_path = field_C0_path_number;
    pSaveState->field_1A_level = field_C2_lvl_number;
    pSaveState->field_1C_sprite_scale = field_CC_sprite_scale;
    pSaveState->field_20_r = field_D0_r;
    pSaveState->field_22_g = field_D2_g;
    pSaveState->field_24_b = field_D4_b;
    pSaveState->field_28_current_motion = field_106_current_motion;
    pSaveState->field_2A_current_frame = field_20_animation.field_92_current_frame;
    pSaveState->field_2C_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pSaveState->field_26_flipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pSaveState->field_2E = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pSaveState->field_2F = field_6_flags.Get(BaseGameObject::Options::eDrawable);
    pSaveState->field_30_health = field_10C_health;
    pSaveState->field_34_current_motion = field_106_current_motion;
    pSaveState->field_36_next_motion = field_108_next_motion;
    pSaveState->field_38 = FP_GetExponent(field_F8_LastLineYPos);
    if (field_100_pCollisionLine)
    {
        pSaveState->field_3A_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pSaveState->field_3A_line_type = -1;
    }
    pSaveState->field_40_bIsActiveChar = this == static_cast<Glukkon*>(sControlledCharacter_5C1B8C);
    pSaveState->field_44_tlvInfo = field_214_tlv_info;

    pSaveState->field_48_brain_state_idx = 0;

    __int16 idx = 0;
    for (auto& fn : sGlukkon_ai_table_5544A0)
    {
        if (BrainIs(fn))
        {
            pSaveState->field_48_brain_state_idx = idx;
            break;
        }
        idx++;
    }

    pSaveState->field_50 = field_210;
    pSaveState->field_54 = field_1D4_timer;
    pSaveState->field_58 = field_1D8;
    pSaveState->field_5E = field_1E2_bUnknown;
    pSaveState->field_60_level = field_1E4_level;
    pSaveState->field_62_path = field_1E6_path;
    pSaveState->field_64_camera = field_1E8_camera;
    pSaveState->field_66_speak = field_1EA_speak;
    pSaveState->field_68 = field_1E0;
    pSaveState->field_6C = field_1DC;
    pSaveState->field_70 = field_1F0;
    pSaveState->field_74 = field_1F4;
    pSaveState->field_78 = field_1F8;
    pSaveState->field_7C = field_1FC;
    pSaveState->field_80 = field_200;
    pSaveState->field_84 = field_204;
    pSaveState->field_88_obj_id = field_208_obj_id;
    pSaveState->field_8C = field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed);
    pSaveState->field_8E = field_4_typeId;

    return sizeof(Glukkon_SaveState);
}

int Glukkon::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_444B90(pSaveBuffer);
}

void Glukkon::VPossessed_408F70()
{
    vPossessed_440160();
}

void Glukkon::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOn_TLV_Collision_4404A0(pTlv);
}

void Glukkon::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_444120();
}

__int16 Glukkon::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_43FA40(pFrom);
}

void Glukkon::M_Idle_0_442D10()
{
    HandleInput_443BB0();
}

void Glukkon::M_Walk_1_442D30()
{
    if (DoMovement_444190())
    {
        switch (field_20_animation.field_92_current_frame)
        {
        case 0:
        case 9:
            if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion == eGlukkonMotions::M_Jump_4_443030)
                {
                    SetAnim_43F9C0(eGlukkonMotions::M_WalkToJump_18_443A00);
                }
            }
            else if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::eRun)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_WalkToJump_18_443A00, TRUE);
            }

            MapFollowMe_408D10(TRUE);
            return;

        case 2:
        case 6:
        case 10:
        case 14:
            PlaySound_4447D0(0, this);
            field_212_currentWalkPitch++;
            return;

        case 8:
        case 17:
            if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion == eGlukkonMotions::M_Idle_0_442D10 ||
                    field_108_next_motion == eGlukkonMotions::M_Turn_2_442F10 ||
                    field_108_next_motion == eGlukkonMotions::M_Speak1_11_4437D0 ||
                    field_108_next_motion == eGlukkonMotions::M_Speak2_12_4438F0 ||
                    field_108_next_motion == eGlukkonMotions::M_Speak3_23_443910 ||
                    field_108_next_motion == eGlukkonMotions::M_LongLaugh_13_443930)
                {
                    if (field_20_animation.field_92_current_frame != 8)
                    {
                        SetAnim_43F9C0(eGlukkonMotions::M_EndWalk_15_443970);
                    }
                    else
                    {
                        SetAnim_43F9C0(eGlukkonMotions::M_EndSingleStep_24_443990);
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
                        SetAnim_43F9C0(eGlukkonMotions::M_EndSingleStep_24_443990, TRUE);
                    }
                    else
                    {
                        SetAnim_43F9C0(eGlukkonMotions::M_EndWalk_15_443970, TRUE);
                    }
                }
            }
            break;

        default:
            return;
        }
    }
}

void Glukkon::M_Turn_2_442F10()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand_443B70();
    }
}

void Glukkon::M_KnockBack_3_442F40()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        PlaySound_4447D0(2, this);
    }

    if (field_100_pCollisionLine)
    {
        SlowDown_444700(FP_FromDouble(0.35));
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                SetAnim_43F9C0(eGlukkonMotions::M_KnockBackStandBegin_20_442FC0, TRUE);
            }
        }
    }
    else
    {
        M_Fall_7_443510();
    }
}

const FP dword_5453DC[16] =
{
    FP_FromDouble(-10.01),
    FP_FromDouble(-10.58),
    FP_FromDouble(-7.2),
    FP_FromDouble(-4.66),
    FP_FromInteger(-3),
    FP_FromInteger(3),
    FP_FromDouble(4.40),
    FP_FromDouble(7.07),
    FP_FromDouble(10.94),
    FP_FromDouble(15.03),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)
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

void Glukkon::M_Jump_4_443030()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    
    if (field_20_animation.field_92_current_frame >= 10)
    {
        JumpHelper();
        return;
    }

    if (field_20_animation.field_92_current_frame == 0)
    {
        SFX_Play_46FBA0(28u, 50, -900);
        field_F8_LastLineYPos = field_BC_ypos;
        if (pPlatform)
        {
            pPlatform->VRemove(this);
            field_110_id = -1;
        }
        field_100_pCollisionLine = nullptr;
    }

    field_C8_vely = (field_CC_sprite_scale * dword_5453DC[field_20_animation.field_92_current_frame]);

    FP velXTableValue = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velXTableValue = -sJumpData_54539C[field_20_animation.field_92_current_frame];
    }
    else
    {
        velXTableValue = sJumpData_54539C[field_20_animation.field_92_current_frame];
    }

    field_C4_velx = (field_CC_sprite_scale * velXTableValue);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx) ||
        WallHit_408750(field_CC_sprite_scale * FP_FromInteger(2), field_C4_velx))
    {
        field_C8_vely = FP_FromInteger(0);
        field_1D8 = FP_FromDouble(0.35);
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        else
        {
            field_C4_velx = -(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(6));
        }
        SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
        MapFollowMe_408D10(TRUE);
        return;
    }

    if (sControlledCharacter_5C1B8C == this)
    {
        sub_408C40();
    }

    __int16 bCollision = FALSE;
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (field_100_pCollisionLine)
    {
        bCollision = FALSE;
    }
    else
    {
        bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromInteger(0));
    }

    if (field_20_animation.field_92_current_frame == 0 || !bCollision)
    {
        if (field_20_animation.field_92_current_frame == 9 && !field_100_pCollisionLine)
        {
            SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
        }
        JumpHelper();
        return;
    }

    switch (pLine->field_8_type)
    {
    case 0u:
    case 4u:
    case 32u:
    case 36u:
        PlaySound_4447D0(1, this);
        field_B8_xpos = hitX;
        field_100_pCollisionLine = pLine;
        field_BC_ypos = hitY;
        MapFollowMe_408D10(TRUE);
        GetOnPlatforms_444060();
        break;

    case 1u:
    case 2u:
        field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
        break;

    default:
        if (field_20_animation.field_92_current_frame == 9 && !field_100_pCollisionLine)
        {
            SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
        }
        JumpHelper();
        return;
    }

}

void Glukkon::JumpHelper()
{
    if (field_20_animation.field_92_current_frame != 15)
    {
        return;
    }

    if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
    {
        const auto input_pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

        if (field_C4_velx > FP_FromInteger(0) && (input_pressed & InputCommands::eLeft) ||
            field_C4_velx < FP_FromInteger(0) && (input_pressed & InputCommands::eRight))
        {
            // Direction changed
            SetAnim_43F9C0(eGlukkonMotions::M_JumpToStand_17_4439D0, TRUE);
        }
        else if (input_pressed & (InputCommands::eLeft | InputCommands::eRight))
        {
            if (!(input_pressed & InputCommands::eRun))
            {
                // Still moving but not running, so start walking
                SetAnim_43F9C0(eGlukkonMotions::M_JumpToWalk_19_443A30, TRUE);
            }
        }
        else if (!(input_pressed & InputCommands::eHop))
        {
            // Not changing direction or trying to move at all, back to standing
            SetAnim_43F9C0(eGlukkonMotions::M_JumpToStand_17_4439D0, TRUE);
        }
    }
    else
    {
        if (field_108_next_motion != 1)
        {
            if (field_108_next_motion == eGlukkonMotions::M_Idle_0_442D10 ||
                field_108_next_motion == eGlukkonMotions::M_Turn_2_442F10 ||
                field_108_next_motion == eGlukkonMotions::M_Speak1_11_4437D0 ||
                field_108_next_motion == eGlukkonMotions::M_Speak2_12_4438F0 ||
                field_108_next_motion == eGlukkonMotions::M_Speak3_23_443910 ||
                field_108_next_motion == eGlukkonMotions::M_LongLaugh_13_443930)
            {
                // Leaving jump to a motion that requires standing
                SetAnim_43F9C0(eGlukkonMotions::M_JumpToStand_17_4439D0);
            }
        }
        else
        {
            // No next motion so just stand
            SetAnim_43F9C0(eGlukkonMotions::M_JumpToWalk_19_443A30, TRUE);
        }
    }

    MapFollowMe_408D10(TRUE);
    return;
}

void Glukkon::M_JumpToFall_5_4434C0()
{
    M_WalkToFall_6_4434E0();
}

void Glukkon::M_WalkToFall_6_4434E0()
{
    M_Fall_7_443510();

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
    }
}

void Glukkon::M_Fall_7_443510()
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx = field_C4_velx - (field_CC_sprite_scale * field_1D8);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx = (field_CC_sprite_scale * field_1D8) + field_C4_velx;
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const __int16 bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
    
    if (BrainIs(&Glukkon::AI_3_PlayerControlled_441A30))
    {
        sub_408C40();
    }

    if (bCollision)
    {
        switch (pLine->field_8_type)
        {
        case 0:
        case 4:
        case 32:
        case 36:
            field_100_pCollisionLine = pLine;
            field_BC_ypos = hitY;
            field_B8_xpos = hitX;
            field_C8_vely = FP_FromInteger(0);
            
            GetOnPlatforms_444060();

            if (hitY - field_F8_LastLineYPos > (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(7)))
            {
                SetAnim_43F9C0(eGlukkonMotions::M_DeathFall_8_443760, TRUE);
                SetBrain(&Glukkon::AI_4_Death_442010);
                field_210 = 0;
            }
            else if (field_106_current_motion != eGlukkonMotions::M_KnockBack_3_442F40)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_Land_9_443790, TRUE);
            }
            break;

        case 1u:
        case 2u:
            field_B8_xpos = hitX;
            field_C4_velx = -field_C4_velx / FP_FromInteger(2);
            break;

        default:
            return;
        }
    }
}

void Glukkon::M_DeathFall_8_443760()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        SFX_Play_46FA90(64, 85);
    }
}

void Glukkon::M_Land_9_443790()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Glukkon::PlaySound_4447D0(1, this);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleInput_443BB0();
    }
}

void Glukkon::M_ChantShake_10_443B50()
{
    if (!field_100_pCollisionLine)
    {
        M_Fall_7_443510();
    }
}

void Glukkon::M_Speak1_11_4437D0()
{

    if (field_20_animation.field_92_current_frame == 2 && field_1EA_speak != GlukkonSpeak::None )
    {
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
        {
            if (field_1FC)
            {
                Event_Broadcast_422BC0(kEventUnknown17, this);
                field_1FC = 0;
            }
            
            if (BrainIs(&Glukkon::AI_3_PlayerControlled_441A30))
            {
                GameSpeakEvents evToBePushed;
                switch (field_1EA_speak)
                {
                    case GlukkonSpeak::Hey_0:
                        evToBePushed = GameSpeakEvents::Glukkon_Hey_36;
                        break;
                    case GlukkonSpeak::DoIt_1:
                        evToBePushed = GameSpeakEvents::Glukkon_DoIt_37;
                        break;
                    case GlukkonSpeak::StayHere_2:
                        evToBePushed = GameSpeakEvents::Glukkon_StayHere_38;
                        break;
                    case GlukkonSpeak::Commere_3:
                        evToBePushed = GameSpeakEvents::Glukkon_Commere_39;
                        break;
                    case GlukkonSpeak::AllOYa_4:
                        evToBePushed = GameSpeakEvents::Glukkon_AllOYa_40;
                        break;
                    case GlukkonSpeak::Heh_5:
                        evToBePushed = GameSpeakEvents::Glukkon_Heh_41;
                        break;
                    case GlukkonSpeak::Help_6:
                        evToBePushed = GameSpeakEvents::Glukkon_Help_42;
                        break;
                    case GlukkonSpeak::Laugh_7:
                        evToBePushed = GameSpeakEvents::Glukkon_Laugh_43;
                        break;
                    case GlukkonSpeak::KillEm_8:
                        evToBePushed = GameSpeakEvents::Glukkon_KillEm_44;
                        break;
                    case GlukkonSpeak::Unused_9:
                        evToBePushed = GameSpeakEvents::Glukkon_Unknown_45;
                        break;
                    case GlukkonSpeak::Unused_10:
                        evToBePushed = GameSpeakEvents::Glukkon_Unknown_46;
                        break;
                    case GlukkonSpeak::What_11:
                        evToBePushed = GameSpeakEvents::Glukkon_What_47;
                        break;
                    default:
                        evToBePushed = GameSpeakEvents::eUnknown_35; //GlukkonSpeak::None
                        break;
                }
                if (evToBePushed != GameSpeakEvents::eUnknown_35)
                {
                    pEventSystem_5BC11C->PushEvent_4218D0(evToBePushed);
                }
            }

            PlaySound_GameSpeak_444AF0(field_1EA_speak, 0, 0, 0);
            
            if (field_1EA_speak == GlukkonSpeak::Help_6)
            {
                SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_switch_id, SwitchOp::eSetTrue_0);
            }
        }
        field_1EA_speak = GlukkonSpeak::None;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleInput_443BB0();
    }
}

void Glukkon::M_Speak2_12_4438F0()
{
    M_Speak1_11_4437D0();
}

void Glukkon::M_LongLaugh_13_443930()
{
    M_Speak1_11_4437D0();
}

void Glukkon::M_BeginWalk_14_443950()
{
    M_JumpToWalk_19_443A30();
}

void Glukkon::M_EndWalk_15_443970()
{
    M_JumpToStand_17_4439D0();
}

void Glukkon::M_StandToJump_16_4439B0()
{
    M_WalkToJump_18_443A00();
}

void Glukkon::M_JumpToStand_17_4439D0()
{
    DoMovement_444190();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Glukkon::HandleInput_443BB0();
    }
}

void Glukkon::M_WalkToJump_18_443A00()
{
    DoMovement_444190();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim_43F9C0(eGlukkonMotions::M_Jump_4_443030);
    }
}

void Glukkon::M_JumpToWalk_19_443A30()
{
    DoMovement_444190();
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim_43F9C0(eGlukkonMotions::M_Walk_1_442D30);
    }
}

void Glukkon::M_KnockBackStandBegin_20_442FC0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        PlaySound_GameSpeak_444AF0(GlukkonSpeak::Heh_5, 0, 0, 0);
        SetAnim_43F9C0(eGlukkonMotions::M_KnockBackStandEnd_22_443010, TRUE);
    }
}

void Glukkon::M_GetShot_21_443A60()
{
    if (!field_100_pCollisionLine)
    {
        M_Fall_7_443510();
        if (field_106_current_motion != eGlukkonMotions::M_GetShot_21_443A60)
        {
            SetAnim_43F9C0(eGlukkonMotions::M_GetShot_21_443A60, TRUE);
        }
    }

    if (static_cast<int>(sGnFrame_5C1B84) >= field_204)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            field_210 = 2;
            const FP shotXVel = FP_FromInteger(20) * field_CC_sprite_scale;
            if (field_C4_velx >= FP_FromInteger(0))
            {
                field_C4_velx = field_C4_velx <= FP_FromInteger(0) ? FP_FromInteger(0) : shotXVel;
            }
            else
            {
                field_C4_velx = -shotXVel;
            }
        }
    }

    if (static_cast<int>(sGnFrame_5C1B84) > field_200)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_C4_velx > FP_FromInteger(0));
        SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
    }
}

void Glukkon::M_KnockBackStandEnd_22_443010()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim_43F9C0(eGlukkonMotions::M_Idle_0_442D10, TRUE);
    }
}

void Glukkon::M_Speak3_23_443910()
{
    M_Speak1_11_4437D0();
}

void Glukkon::M_EndSingleStep_24_443990()
{
    M_JumpToStand_17_4439D0();
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
            field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
            return 7;
        }
    }

    if (!field_100_pCollisionLine)
    {
        return 8;
    }

    if (sActiveHero_5C1B68->field_10C_health < FP_FromInteger(0))
    {
        Speak_444640(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::AI_4_Death_442010);
        return 6;
    }

    BaseAnimatedWithPhysicsGameObject* pEvent17 = nullptr;

    switch (field_210)
    {
    case 0:
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
        {
            return field_210;
        }

        if (ShouldPanic_440200(FALSE))
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
            Speak_444640(GlukkonSpeak::Hey_0);
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
                field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                return 2;
            }
            field_108_next_motion = eGlukkonMotions::M_BeginWalk_14_443950;
            return 1;
        }
        else
        {
            field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
            return 1;
        }
        break;

    case 1:
        if (ShouldPanic_440200(FALSE))
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

            Speak_444640(GlukkonSpeak::Hey_0);
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
                        field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
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
                    field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
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
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
        {
            return field_210;
        }
        return 0;

    case 3:
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
        {
            return field_210;
        }
        field_1D4_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(30, 120);
        return 4;

    case 4:
        if (ShouldPanic_440200(FALSE))
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
            Speak_444640(GlukkonSpeak::Hey_0);
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
            field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
            return 2;
        }
        break;

    case 5:
        if (ShouldPanic_440200(FALSE))
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
            Speak_444640(GlukkonSpeak::Hey_0);
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
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
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
        Speak_444640(GlukkonSpeak::Heh_5);
        return 6;

    default:
        return field_210;
    }
}

__int16 Glukkon::AI_1_Panic_4412F0()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(8, this, 0, 0);
    }

    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pLiftPoint && pLiftPoint->field_4_typeId == Types::eLiftPoint_78 && !pLiftPoint->vOnAnyFloor_461920() && field_210 != 6)
    {
        field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
        return 6;
    }

    if (!field_100_pCollisionLine)
    {
        return 7;
    }

    if (sActiveHero_5C1B68->field_10C_health < FP_FromInteger(0))
    {
        Glukkon::Speak_444640(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::AI_4_Death_442010);
        return 6;
    }

    switch (field_210)
    {
    case 0:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_1D4_timer || field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
        {
            return field_210;
        }
        field_1F8 = sGnFrame_5C1B84;
        Speak_444640(GlukkonSpeak::Help_6);
        return 4;

    case 1:
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
        {
            return field_210;
        }

        if (ShouldPanic_440200(TRUE))
        {
            field_1F8 = sGnFrame_5C1B84;
        }
        else if (static_cast<int>(sGnFrame_5C1B84) - field_1F8 > field_1A8_tlvData.field_1A_post_alarm_delay)
        {
            SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_switch_id, SwitchOp::eSetFalse_1);
            SetBrain(&Glukkon::AI_0_Calm_WalkAround_440B40);
            return 0;
        }

        if (field_1A8_tlvData.field_14_default_behaviour != 0)
        {
            if (Check_IsOnEndOfLine_408E90(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked_4442F0(field_C4_velx, 1))
            {
                field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                return 3;
            }
            field_108_next_motion = eGlukkonMotions::M_BeginWalk_14_443950;
        }
        else
        {
            field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
        }
        return 2;

    case 2:
        if (field_1A8_tlvData.field_14_default_behaviour == 1)
        {
            if (Check_IsOnEndOfLine_408E90(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked_4442F0(field_C4_velx, 1))
            {
                Glukkon::Speak_444640(GlukkonSpeak::Help_6);
                return 5;
            }
        }
        else
        {
            if (Math_NextRandom() < 5u && static_cast<int>(sGnFrame_5C1B84) > field_1F4)
            {
                field_1F4 = sGnFrame_5C1B84 + 120;
                Glukkon::Speak_444640(GlukkonSpeak::Help_6);
                return 5;
            }
        }
        if (Math_NextRandom() >= 0xAu || static_cast<int>(sGnFrame_5C1B84) <= field_1F0)
        {
            return field_210;
        }
        Glukkon::Speak_444640(GlukkonSpeak::Help_6);
        return 4;

    case 3:
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
        {
            return field_210;
        }
        return 1;

    case 4:
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
        {
            return field_210;
        }
        field_1F0 = sGnFrame_5C1B84 + 60;
        return 1;

    case 5:
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
        {
            return field_210;
        }
        field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
        return 3;

    case 6:
        if (pLiftPoint)
        {
            if (!pLiftPoint->vOnAnyFloor_461920())
            {
                return field_210;
            }
        }
        else
        {
            field_110_id = -1;
        }
        return 1;

    case 7:
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
        {
            return field_210;
        }
        return 1;

    default:
        return field_210;
    }
}

__int16 Glukkon::AI_2_Slapped_441720()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(8, this, 0, 0);
    }

    if (field_106_current_motion == eGlukkonMotions::M_Fall_7_443510)
    {
        return 3;
    }

    if (sActiveHero_5C1B68->field_10C_health < FP_FromInteger(0))
    {
        Glukkon::Speak_444640(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::AI_4_Death_442010);
        return 6;
    }

    switch (field_210)
    {
    case 0:
        if (field_106_current_motion)
        {
            return field_210;
        }

        if (ShouldPanic_440200(TRUE))
        {
            field_1F8 = sGnFrame_5C1B84;
        }
        else if ((signed int)(sGnFrame_5C1B84 - field_1F8) > field_1A8_tlvData.field_1A_post_alarm_delay)
        {
            SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_switch_id, SwitchOp::eSetFalse_1);
            SetBrain(&Glukkon::AI_0_Calm_WalkAround_440B40);

            if (FP_FromInteger(field_1A8_tlvData.field_8_top_left.field_0_x) >= field_B8_xpos)
            {
                if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                {
                    return 2;
                }
                if (FP_FromInteger(field_1A8_tlvData.field_8_top_left.field_0_x) >= field_B8_xpos)
                {
                    return 0;
                }
            }

            if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
            {
                return 0;
            }
            return 2;
        }

        if (field_100_pCollisionLine && Check_IsOnEndOfLine_408E90(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 4) || PathBlocked_4442F0(field_C4_velx, 0))
        {
            field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
            return 2;
        }
        else
        {
            field_108_next_motion = eGlukkonMotions::M_Jump_4_443030;
            return 1;
        }
        break;

    case 1:
        if (ShouldPanic_440200(TRUE))
        {
            field_1F8 = sGnFrame_5C1B84;
        }
        else if (static_cast<int>(sGnFrame_5C1B84 - field_1F8) > field_1A8_tlvData.field_1A_post_alarm_delay)
        {
            field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
            return 0;
        }

        if (!field_100_pCollisionLine || !Check_IsOnEndOfLine_408E90(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), 4) && !PathBlocked_4442F0(field_C4_velx, 0))
        {
            if (Math_NextRandom() >= 10u || static_cast<int>(sGnFrame_5C1B84) <= field_1F0)
            {
                return field_210;
            }
            Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Help_6, 0, 0, 0);
            field_1F0 = sGnFrame_5C1B84 + 40;
            return field_210;
        }

        field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
        return 2;

    case 2:
    case 3:
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10)
        {
            return field_210;
        }
        return 0;

    default:
        return field_210;
    }
}

__int16 Glukkon::AI_3_PlayerControlled_441A30()
{
    auto pDeathFadeOut = static_cast<DeathFadeOut*>(sObjectIds_5C1B70.Find_449CF0(field_208_obj_id));
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(9, this, 0, 0);
    }

    switch (field_210)
    {
    case 0:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_1D4_timer)
        {
            return field_210;
        }
        ToStand_443B70();
        return 1;

    case 1:
    {
        if (field_1A8_tlvData.field_22_glukkon_type != GlukkonTypes::Aslik_1 &&
            field_1A8_tlvData.field_22_glukkon_type != GlukkonTypes::Drpik_2 &&
            field_1A8_tlvData.field_22_glukkon_type != GlukkonTypes::Phleg_3 ||
            !SwitchStates_Get_466020(field_1A8_tlvData.field_26_play_movie_id))
        {
            if (Input_IsChanting_45F260() && field_106_current_motion != eGlukkonMotions::M_Jump_4_443030 && !field_1E2_bUnknown)
            {
                field_1D4_timer = sGnFrame_5C1B84 + 30;
                SFX_Play_46FA90(17u, 0);
                SetAnim_43F9C0(10, TRUE);
                return 2;
            }
            return field_210;
        }

        if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Aslik_1)
        {
            sVisitedFeecoEnder_5C1C06 = 1;
        }
        else if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Drpik_2)
        {
            sVisitedBarracks_5C1C04 = 1;
        }
        else
        {
            sVisitedBonewerks_5C1C02 = 1;
        }

        auto pDeathFadeOutMem = alive_new<DeathFadeOut>();
        if (pDeathFadeOutMem)
        {
            field_208_obj_id = pDeathFadeOutMem->ctor_427030(40, 1, 0, 8, 2)->field_8_object_id;
        }
    }
        return 3;

    case 2:
        if (Input_IsChanting_45F260()
            || (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Aslik_1 ||
                field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Drpik_2 ||
                field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Phleg_3)
            && SwitchStates_Get_466020(field_1A8_tlvData.field_26_play_movie_id))
        {
            if (!(static_cast<int>(sGnFrame_5C1B84) % 4))
            {
                const FP xRand = FP_FromInteger(Math_RandomRange_496AB0(-20, 20));
                const FP yRand = FP_FromInteger(Math_RandomRange_496AB0(20, 50));
                New_Chant_Particle_426BE0(
                    (field_CC_sprite_scale * xRand) + field_B8_xpos,
                    field_BC_ypos - (field_CC_sprite_scale * yRand),
                    field_CC_sprite_scale,
                    0);
            }

            if (static_cast<int>(sGnFrame_5C1B84) > field_1D4_timer || sActiveHero_5C1B68->field_10C_health <= FP_FromInteger(0))
            {
                field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
                SetBrain(&Glukkon::AI_4_Death_442010);
                field_210 = 2;
                MusicController::sub_47FD60(0, this, 0, 0);
            }
            return field_210;
        }
        ToStand_443B70();
        return 1;

    case 3:
        if (pDeathFadeOut && !pDeathFadeOut->field_7E_bDone)
        {
            return field_210;
        }

        for (int i=0; i<gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_4_typeId == Types::eSlig_125)
            {
                pObj->field_6_flags.Set(BaseGameObject::eDead);
            }
        }

        field_1C_update_delay = 2;
        return 4;

    case 4:
    {
        pScreenManager_5BB5F4->field_40_flags &= ~0x10000;
        sLevelId_dword_5CA408 = static_cast<DWORD>(gMap_5C3030.sCurrentLevelId_5C3030);

        const FmvInfo* pFmvRec = Path_Get_FMV_Record_460F70(gMap_5C3030.sCurrentLevelId_5C3030, field_1A8_tlvData.field_28_movie_to_play_id);

        DWORD pos = 0;
        Get_fmvs_sectors_494460(pFmvRec->field_0_pName, 0, 0, &pos, 0, 0);

        auto pMovie = alive_new<Movie>();
        if (pMovie)
        {
            pMovie->ctor_4DFDE0(pFmvRec->field_4_id, pos, pFmvRec->field_6_flags & 1, pFmvRec->field_8, pFmvRec->field_A_volume);
        }
    }
        return 5;

    case 5:
        if (sMovie_ref_count_BB4AE4)
        {
            return field_210;
        }
        gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
        pScreenManager_5BB5F4->DecompressToVRam_40EF60((WORD**)gMap_5C3030.field_2C_5C305C_camera_array[0]->field_C_pCamRes); // TODO: Cast hack
        if (pDeathFadeOut)
        {
            pDeathFadeOut->Init_427140(40, 0, 1, 8);
        }
        return 6;

    case 6:
        SND_Restart_4CB0E0();
        pScreenManager_5BB5F4->field_40_flags |= 0x10000;
        field_1D4_timer = sGnFrame_5C1B84 + 30;
        SFX_Play_46FA90(17u, 0);
        SetAnim_43F9C0(10, TRUE);
        return 2;

    default:
        return field_210;
    }
}

// TODO: GibType should also be an enum
static int AsGibType(GlukkonTypes glukkonType)
{
    switch (glukkonType)
    {
    case GlukkonTypes::Normal_0:
        return 0 + 6;

    case GlukkonTypes::Aslik_1:
        return 1 + 6;

    case GlukkonTypes::Drpik_2:
        return 2 + 6;

    case GlukkonTypes::Phleg_3:
        return 3 + 6;

    case GlukkonTypes::Normal_4:
        return 4 + 6;

    case GlukkonTypes::Normal_5:
        return 5 + 6;

    default:
        return 0 + 6;
    }
}

__int16 Glukkon::AI_4_Death_442010()
{
    if (gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_47FD60(0, this, 0, 0);
    }

    switch (field_210)
    {
    case 0:
        if (field_106_current_motion != eGlukkonMotions::M_DeathFall_8_443760 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_210;
        }
        else
        {
            field_10C_health = FP_FromInteger(0);
            field_1D4_timer = sGnFrame_5C1B84 + 90;
            return 1;
        }
        break;

    case 1:
        if (static_cast<int>(sGnFrame_5C1B84) > field_1D4_timer)
        {
            ToDead_43F640();
            return field_210;
        }
        else
        {
            field_CC_sprite_scale -= FP_FromDouble(0.008);

            field_D0_r -= 2;
            field_D2_g -= 2;
            field_D4_b -= 2;

            if (!(static_cast<int>(sGnFrame_5C1B84) % 5))
            {
                New_Particles_426C70(
                    (FP_FromInteger(Math_RandomRange_496AB0(-24, 24)) * field_CC_sprite_scale) + field_B8_xpos,
                    field_BC_ypos - FP_FromInteger(6),
                    (field_CC_sprite_scale / FP_FromInteger(2)), 
                    2, 0x80u, 0x80u, 0x80u);

                SFX_Play_46FBA0(79u, 25, FP_GetExponent(FP_FromInteger(2200) * field_CC_sprite_scale));
            }
            return field_210;
        }
        break;

    case 2:
    {
        auto pGibs = alive_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_40FB40(
                AsGibType(field_1A8_tlvData.field_22_glukkon_type),
                field_B8_xpos,
                field_BC_ypos,
                field_C4_velx,
                field_C8_vely,
                field_CC_sprite_scale,
                0);
        }

        auto pBlood = alive_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_40F0B0(
                field_B8_xpos,
                field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                20);
        }

        New_Particles_426C70(
            field_B8_xpos,
            field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale),
            field_CC_sprite_scale,
            3,
            128u,
            128u,
            128u);

        SFX_Play_46FA90(0x40u, 128, field_CC_sprite_scale);
        SFX_Play_46FA90(0x2Fu, 90, field_CC_sprite_scale);

        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

        SetAnim_43F9C0(eGlukkonMotions::M_ChantShake_10_443B50, TRUE);

        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        field_10C_health = FP_FromInteger(0);
        field_1D4_timer = sGnFrame_5C1B84 + 40;
    }
        return 3;

    case 3:
        if (static_cast<int>(sGnFrame_5C1B84) > field_1D4_timer)
        {
            ToDead_43F640();
        }
        return field_210;

    case 4:
    case 5:
        if (!field_100_pCollisionLine || 
            field_106_current_motion != eGlukkonMotions::M_KnockBack_3_442F40 || 
            !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
        {
            return field_210;
        }
        else
        {
            field_1D4_timer = sGnFrame_5C1B84 + 90;
            return 1;
        }
        break;

    default:
        return field_210;
    }

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
        if (field_106_current_motion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
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
            Speak_444640(GlukkonSpeak::Heh_5);
            return 2;
        }
        else
        {
            SFX_Play_46FA90(112u, 0);
            Speak_444640(GlukkonSpeak::Heh_5);
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
    field_1EA_speak = GlukkonSpeak::None;
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
        
        Update_Slurg_WatchPoints_440600();
        
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
    SetAnim_43F9C0(eGlukkonMotions::M_ChantShake_10_443B50, TRUE);
    SetBrain(&Glukkon::AI_3_PlayerControlled_441A30);
    field_210 = 0;
    field_1D4_timer = sGnFrame_5C1B84 + 35;
    field_1E4_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_1E6_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_1E8_camera = gMap_5C3030.sCurrentCamId_5C3034;
}

void Glukkon::Update_Slurg_WatchPoints_440600()
{
    if (field_106_current_motion == eGlukkonMotions::M_Walk_1_442D30 || 
        field_106_current_motion == eGlukkonMotions::M_Jump_4_443030 && 
        field_20_animation.field_92_current_frame > 8)
    {
        if (sGnFrame_5C1B84 & 1)
        {
            const char count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
            if (count < 5)
            {
                Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
                pPoints->field_0_points[count].field_0_xPos = FP_GetExponent(field_B8_xpos);
                pPoints->field_0_points[count].field_2_yPos = field_100_pCollisionLine->field_0_rect.y  - 5;
                sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
            }
        }
    }
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

void Glukkon::Speak_444640(GlukkonSpeak speak)
{
    switch (speak)
    {
    case GlukkonSpeak::Hey_0:
    case GlukkonSpeak::DoIt_1:
    case GlukkonSpeak::Heh_5:
    case GlukkonSpeak::Help_6:
        field_108_next_motion = eGlukkonMotions::M_Speak1_11_4437D0;
        field_1EA_speak = speak;
        break;

    case GlukkonSpeak::StayHere_2:
        field_108_next_motion = eGlukkonMotions::M_Speak3_23_443910;
        field_1EA_speak = speak;
        break;

    case GlukkonSpeak::Commere_3:
    case GlukkonSpeak::AllOYa_4:
    case GlukkonSpeak::KillEm_8:
    case GlukkonSpeak::Unused_9:
    case GlukkonSpeak::Unused_10:
        field_108_next_motion = eGlukkonMotions::M_Speak2_12_4438F0;
        field_1EA_speak = speak;
        break;

    case GlukkonSpeak::Laugh_7:
        field_108_next_motion = eGlukkonMotions::M_LongLaugh_13_443930;
        field_1EA_speak = speak;
        break;

    default:
        field_1EA_speak = speak;
        break;
    }
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
            field_1EA_speak = GlukkonSpeak::None;

            if (inputHeld & InputCommands::eGameSpeak1)
            {
                field_1EA_speak = GlukkonSpeak::Hey_0;
            }
            else if (inputHeld & InputCommands::eGameSpeak2)
            {
                field_1EA_speak = GlukkonSpeak::Commere_3;
            }
            else if (inputHeld & InputCommands::eGameSpeak3)
            {
                field_1EA_speak = GlukkonSpeak::StayHere_2;
            }
            else if (inputHeld & InputCommands::eGameSpeak4)
            {
                field_1EA_speak = GlukkonSpeak::DoIt_1;
            }
            else if (inputHeld & InputCommands::eGameSpeak5)
            {
                field_1EA_speak = GlukkonSpeak::KillEm_8;
            }
            else if (inputHeld & InputCommands::eGameSpeak7)
            {
                field_1EA_speak = GlukkonSpeak::Help_6;
            }
            else if (inputHeld & InputCommands::eGameSpeak8)
            {
                field_1EA_speak = GlukkonSpeak::Laugh_7;
            }
            else if (inputHeld & InputCommands::eGameSpeak6)
            {
                field_1EA_speak = GlukkonSpeak::AllOYa_4;
            }

            if (field_1EA_speak != GlukkonSpeak::None)
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
                    field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                }
                else
                {
                    field_108_next_motion = eGlukkonMotions::M_BeginWalk_14_443950;
                }
            }
            else if (inputPressed & InputCommands::eLeft)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_108_next_motion = eGlukkonMotions::M_BeginWalk_14_443950;
                }
                else
                {
                    field_108_next_motion = eGlukkonMotions::M_Turn_2_442F10;
                }
            }

            if (inputPressed & InputCommands::eRun)
            {
                if (field_108_next_motion == eGlukkonMotions::M_Walk_1_442D30)
                {
                    field_108_next_motion = eGlukkonMotions::M_Jump_4_443030;
                }
            }

            if (inputPressed & InputCommands::eHop)
            {
                field_108_next_motion = eGlukkonMotions::M_Jump_4_443030;
            }
        }
    }

    switch (field_108_next_motion)
    {
    case -1:
        if (field_106_current_motion == eGlukkonMotions::M_Idle_0_442D10)
        {
            return;
        }
        field_108_next_motion = eGlukkonMotions::M_Idle_0_442D10;
        SetAnim_43F9C0(field_108_next_motion, TRUE);
        break;

    case eGlukkonMotions::M_Idle_0_442D10:
    case eGlukkonMotions::M_Turn_2_442F10:
    case eGlukkonMotions::M_Speak1_11_4437D0:
    case eGlukkonMotions::M_Speak2_12_4438F0:
    case eGlukkonMotions::M_LongLaugh_13_443930:
    case eGlukkonMotions::M_Speak3_23_443910:
        SetAnim_43F9C0(field_108_next_motion, TRUE);
        break;

    case eGlukkonMotions::M_Walk_1_442D30:
    case eGlukkonMotions::M_BeginWalk_14_443950:
        if (field_106_current_motion != eGlukkonMotions::M_Walk_1_442D30)
        {
            field_212_currentWalkPitch = 0;
        }
        // Fall through

    case eGlukkonMotions::M_Jump_4_443030:
    case eGlukkonMotions::M_StandToJump_16_4439B0:
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
        if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), xOff))
        {
            SetAnim_43F9C0(field_108_next_motion, TRUE);
        }
    }
        break;

    default:
        return;
    }

}

__int16 Glukkon::ShouldPanic_440200(__int16 panicEvenIfNotFacingMe)
{
    if (IsLineOfSightBetween_4403B0(this, sControlledCharacter_5C1B8C)
        && !(sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
        && !BaseAliveGameObject::IsInInvisibleZone_425710(sControlledCharacter_5C1B8C)
        && !Event_Get_422C00(kEventResetting)
        && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0)
        && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            sControlledCharacter_5C1B8C->field_C2_lvl_number,
            sControlledCharacter_5C1B8C->field_C0_path_number,
            sControlledCharacter_5C1B8C->field_B8_xpos,
            sControlledCharacter_5C1B8C->field_BC_ypos,
            0)
        && (panicEvenIfNotFacingMe || vIsFacingMe_4254A0(sControlledCharacter_5C1B8C)))
    {
        return 1;
    }

    // Panic if Abe is chanting
    if (Event_Is_Event_In_Range_422C30(
        kEventAbeOhm,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale))
    {
        return 1;
    }

    // Panic if an alarm is on
    if (Event_Get_422C00(kEventAlarm))
    {
        return 1;
    }

    // Panic if player is speaking (I guess it can be rather alarming?)
    auto pSpeakEvent = Event_Is_Event_In_Range_422C30(
        kEventSpeaking,
        field_B8_xpos,
        field_BC_ypos,
        field_D6_scale);
    return pSpeakEvent && pSpeakEvent == sControlledCharacter_5C1B8C;
}

__int16 Glukkon::PathBlocked_4442F0(FP /*a2*/, __int16 checkBounds)
{
    FP gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (field_106_current_motion == eGlukkonMotions::M_Jump_4_443030)
    {
        gridSize = (gridSize * FP_FromInteger(4));
    }

    Path_EnemyStopper::StopDirection direction = Path_EnemyStopper::StopDirection::Left_0;
    short boundType = TlvTypes::ScrabLeftBound_43;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        boundType = TlvTypes::ScrabLeftBound_43;
        direction = Path_EnemyStopper::StopDirection::Left_0;
        gridSize = -gridSize;
    }
    else
    {
        boundType = TlvTypes::ScrabRightBound_44;
        direction = Path_EnemyStopper::StopDirection::Right_1;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), gridSize * FP_FromInteger(1)))
    {
        return 1;
    }

    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
       FP_GetExponent(field_B8_xpos),
       FP_GetExponent(field_BC_ypos), // TODO Abs() ??
       FP_GetExponent(field_B8_xpos + gridSize),
       FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
       TlvTypes::SlamDoor_85);

    auto pSlamDoorTlv = static_cast<Path_SlamDoor*>(field_FC_pPathTLV);

    if (pSlamDoorTlv &&
        (pSlamDoorTlv->field_10_starts_shut == 1 && !SwitchStates_Get_466020(pSlamDoorTlv->field_14_id)||
         pSlamDoorTlv->field_10_starts_shut == 0 &&  SwitchStates_Get_466020(pSlamDoorTlv->field_14_id)))
    {
        return 1;
    }

    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos + gridSize),
        FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
        TlvTypes::EnemyStopper_47);

    auto pEnemyStopper = static_cast<Path_EnemyStopper*>(field_FC_pPathTLV);
    if (pEnemyStopper
        && (pEnemyStopper->field_10_stop_direction == direction || pEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Both_2)
        && SwitchStates_Get_466020(pEnemyStopper->field_12_id))
    {
        return 1;
    }

    if (!checkBounds)
    {
        return 0;
    }

    if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos), // TODO: Abs() ??
        FP_GetExponent(field_B8_xpos + gridSize),
        FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
        boundType))
    {
        return 1;
    }

    return 0;

}

void Glukkon::SpeakRandomish_4405D0()
{
    if (sGnFrame_5C1B84 & 1)
    {
        if ((sGnFrame_5C1B84 & 1) == 1)
        {
            Speak_444640(GlukkonSpeak::Heh_5);
        }
    }
    else
    {
        Speak_444640(GlukkonSpeak::Laugh_7);
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
    nullptr,                    // M_Idle_0_442D10
    sWalkData_545354,           // M_Walk_1_442D30
    nullptr,                    // M_Turn_2_442F10
    nullptr,                    // M_KnockBack_3_442F40
    sJumpData_54539C,           // M_Jump_4_443030
    sJumpToFallData_54542C,     // M_JumpToFall_5_4434C0
    sWalkToFallData_54541C,     // M_WalkToFall_6_4434E0
    nullptr,                    // M_Fall_7_443510
    nullptr,                    // M_DeathFall_8_443760
    nullptr,                    // M_Land_9_443790
    nullptr,                    // M_ChantShake_10_443B50
    nullptr,                    // M_Speak1_11_4437D0
    nullptr,                    // M_Speak2_12_4438F0
    nullptr,                    // M_LongLaugh_13_443930
    sBeginWalkData_545454,      // M_BeginWalk_14_443950
    sEndWalkData_54546C,        // M_EndWalk_15_443970
    sStandToJumpData_545484,    // M_StandToJump_16_4439B0
    sJumpToStandData_54549C,    // M_JumpToStand_17_4439D0
    sWalkToJumpData_5454B4,     // M_WalkToJump_18_443A00
    sJumpToWalkData_5454CC,     // M_JumpToWalk_19_443A30
    nullptr,                    // M_KnockBackStandBegin_20_442FC0
    nullptr,                    // M_GetShot_21_443A60
    nullptr,                    // M_KnockBackStandEnd_22_443010
    nullptr,                    // M_Speak3_23_443910
    nullptr                     // M_EndSingleStep_24_443990
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

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(50), field_C4_velx))
    {
        field_1D8 = FP_FromInteger(0);
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
        SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
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
                    field_1D8 = FP_FromDouble(0.35);
                }
            }
            else if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
            {
                GetOnPlatforms_444060();
            }
        }
        else
        {
            field_F8_LastLineYPos = field_BC_ypos;
            
            VOnTrapDoorOpen();

            if (field_106_current_motion == eGlukkonMotions::M_Walk_1_442D30 ||
                field_106_current_motion == eGlukkonMotions::M_BeginWalk_14_443950 ||
                field_106_current_motion == eGlukkonMotions::M_EndWalk_15_443970 ||
                field_106_current_motion == eGlukkonMotions::M_WalkToJump_18_443A00)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_WalkToFall_6_4434E0, TRUE);
            }
            else if (field_106_current_motion == eGlukkonMotions::M_Jump_4_443030 ||
                     field_106_current_motion == eGlukkonMotions::M_StandToJump_16_4439B0 ||
                     field_106_current_motion == eGlukkonMotions::M_JumpToStand_17_4439D0 ||
                     field_106_current_motion == eGlukkonMotions::M_JumpToWalk_19_443A30)
            {
                SetAnim_43F9C0(eGlukkonMotions::M_JumpToFall_5_4434C0, TRUE);
            }
            else
            {
                SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
            }

            field_B8_xpos = prevXPos + field_C4_velx;
            field_1D8 = FP_FromInteger(1);

            if (field_106_current_motion == eGlukkonMotions::M_KnockBack_3_442F40)
            {
                field_1D8 = FP_FromDouble(0.67);
            }
        }
    }
    else
    {
        field_F8_LastLineYPos = field_BC_ypos;
        SetAnim_43F9C0(eGlukkonMotions::M_Fall_7_443510, TRUE);
    }
}

void Glukkon::GetOnPlatforms_444060()
{
    vOnCollisionWith_424EE0(
        { FP_GetExponent(field_B8_xpos - FP_FromInteger(5)), FP_GetExponent(field_BC_ypos - FP_FromInteger(5)) },
        { FP_GetExponent(field_B8_xpos + FP_FromInteger(5)), FP_GetExponent(field_BC_ypos + FP_FromInteger(5)) },
        ObjList_5C1B78,
        1,
        (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
}

SfxDefinition stepSfx_554840[3] =
{
    { 0u, 8u, 36u, 25u, 1524, 1905 },
    { 0u, 3u, 59u, 60u, 0, 254 },
    { 0u, 3u, 72u, 120u, 0, 254 }
};

void CC Glukkon::PlaySound_4447D0(int sndIdx, Glukkon* pGlukkon)
{
    int volumeLeft, volumeRight;
    int defaultSndIdxVol = stepSfx_554840[sndIdx].field_3_default_volume;

    __int16 pitch;
    if ( sndIdx || !pGlukkon)
    {
        pitch = Math_RandomRange_496AB0(stepSfx_554840[sndIdx].field_4_pitch_min, stepSfx_554840[sndIdx].field_6_pitch_max);
    }
    else
    {
        signed __int16 pitchCap = pGlukkon->field_212_currentWalkPitch;
        if (pitchCap > 12)
        {
            pitchCap = pitchCap % 4 + 12;
        }
        pitch = 127 * pitchCap;
    }

    if (pGlukkon->field_CC_sprite_scale == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else // Glukkon in background layer? TODO figure out if this does actually happen
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap_5C3030.GetDirection_4811A0(
        pGlukkon->field_C2_lvl_number,
        pGlukkon->field_C0_path_number,
        pGlukkon->field_B8_xpos,
        pGlukkon->field_BC_ypos
    );
    PSX_RECT pRect;
    gMap_5C3030.Get_Camera_World_Rect_481410(direction, &pRect);
    switch ( direction )
    {
        case CameraPos::eCamCurrent_0:
        {
            volumeLeft = volumeRight;
            break;
        }
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
            break;
        case CameraPos::eCamLeft_3:
        {
            FP percentHowFar = (FP_FromInteger(pRect.w) - pGlukkon->field_B8_xpos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight =  volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
        }
            break;
        case CameraPos::eCamRight_4:
        {
            FP percentHowFar = (pGlukkon->field_B8_xpos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
        }
            break;
        default:
            return;
    }

    if (pGlukkon->field_CC_sprite_scale == FP_FromDouble(0.5)) //TODO figure out if this does actually happen
    {
        volumeLeft = FP_GetExponent(FP_FromInteger(volumeLeft * 2) / FP_FromInteger(3));
        volumeRight = FP_GetExponent(FP_FromInteger(volumeRight * 2) / FP_FromInteger(3));
    }

    SFX_SfxDefinition_Play_4CA700(&stepSfx_554840[sndIdx], (__int16) volumeLeft, ( __int16 ) volumeRight, pitch, pitch);
}

void Glukkon::ToDead_43F640()
{
    MusicController::sub_47FD60(0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        // When its a player controlled gluk go back to the screen the player is in
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        MusicController::sub_47FD60(0, this, 0, 0);

        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_1E4_level,
                field_1E6_path,
                field_1E8_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }

    SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_switch_id, SwitchOp::eSetFalse_1);

    if ((field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Aslik_1 || 
         field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Drpik_2 ||
         field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::Phleg_3 ) && 
        !SwitchStates_Get_466020(field_1A8_tlvData.field_26_play_movie_id))
    {
        // If an exec is dead trigger ze gas
        SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_24_start_gas_id, SwitchOp::eSetTrue_0);
    }

    if (field_1A8_tlvData.field_1C_spawn_id == 0)
    {
        // Don't spawn again, dead
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (field_1A8_tlvData.field_1C_spawn_id > 1)
        {
            // If above 1 then its an actual switch id
            SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_1C_spawn_id, SwitchOp::eSetFalse_1);
        }

        // Spawn again if id is 1 or above
        Init_43F260();
    }
}

void Glukkon::vOn_TLV_Collision_4404A0(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);

                PlaySound_GameSpeak_444AF0(GlukkonSpeak::Unused_9, 0, 0, 0);
                ToDead_43F640();

                // Muds love it when people DIE
                Event_Broadcast_422BC0(kEventMudokonComfort, this);
            }
        }

        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            pTlv,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
    }
}

SfxDefinition gameSpeak_554858[15] =
{
    { 0u, 8u, 66u, 127u, 0, 0 },
    { 0u, 8u, 64u, 127u, 0, 0 },
    { 0u, 8u, 67u, 127u, 0, 0 },
    { 0u, 8u, 61u, 127u, 0, 0 },
    { 0u, 8u, 63u, 127u, 0, 0 },
    { 0u, 8u, 70u, 127u, 0, 0 },
    { 0u, 8u, 65u, 127u, 0, 0 },
    { 0u, 8u, 69u, 127u, 0, 0 },
    { 0u, 8u, 71u, 127u, 0, 0 },
    { 0u, 8u, 60u, 127u, 0, 0 },
    { 0u, 8u, 60u, 127u, 0, 0 },
    { 0u, 8u, 62u, 127u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 }
};

void CC Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak sndIdx, __int16 volume, __int16 pitch, Glukkon* pGlukkon)
{
    auto sndIdxShort = ( __int16 ) sndIdx;
    __int16 calcedVolume = volume;
    if (!calcedVolume)
    {
        calcedVolume = gameSpeak_554858[sndIdxShort].field_3_default_volume;
    }
    if (pGlukkon)
    {
        if (pGlukkon->field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            calcedVolume = FP_GetExponent(FP_FromInteger(calcedVolume * 2) / FP_FromInteger(3));
        }
    }
    SFX_SfxDefinition_Play_4CA420(&gameSpeak_554858[sndIdxShort], calcedVolume, pitch, pitch);
}

BOOL CCSTD Glukkon::IsLineOfSightBetween_4403B0(Glukkon* pGlukkon, BaseAliveGameObject* pOther)
{
    FP hitX = {}, hitY = {};
    PathLine* pathLine = nullptr;

    PSX_RECT bRect = {};
    pOther->vGetBoundingRect_424FD0(&bRect, 1);
    const FP X2 = FP_FromInteger(( bRect.x + bRect.w ) / 2);
    const FP Y2 = FP_FromInteger(( bRect.y + bRect.y ) / 2);

    return sCollisions_DArray_5C1128->Raycast_417A60(
        pGlukkon->field_B8_xpos,
        pGlukkon->field_BC_ypos,
        X2,
        Y2,
        &pathLine,
        &hitX,
        &hitY,
        ( pGlukkon->field_D6_scale != 0 ? 1 : 0x10 ) | ( pGlukkon->field_D6_scale != 0 ? 6 : 0x60 ) | ( pGlukkon->field_D6_scale != 0 ? 8 : 0x80 )
    ) != 1;
}

void Glukkon::ToStand_443B70()
{
    field_1D8 = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    SetAnim_43F9C0(eGlukkonMotions::M_Idle_0_442D10);
    MapFollowMe_408D10(TRUE);
}

void Glukkon::SlowDown_444700(FP speed)
{
    if (FP_GetExponent(field_C4_velx))
    {
        FollowLine_443EB0();
        if (field_C4_velx <= FP_FromInteger(0))
        {
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = (field_CC_sprite_scale * speed) + field_C4_velx;;
                if (field_C4_velx > FP_FromInteger(0))
                {
                    field_C4_velx = FP_FromInteger(0);
                }
            }
        }
        else
        {
            field_C4_velx = field_C4_velx - (field_CC_sprite_scale * speed);;
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = FP_FromInteger(0);
            }
        }
    }
}

void Glukkon::vScreenChanged_440110()
{
    BaseGameObject::VScreenChanged();
    SwitchStates_Do_Operation_465F00(field_1A8_tlvData.field_18_switch_id, SwitchOp::eSetFalse_1);
    if (BrainIs(&Glukkon::AI_5_WaitToSpawn_442490) && !field_210)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Glukkon::vOnTrapDoorOpen_444120()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        field_F8_LastLineYPos = field_BC_ypos;
        pPlatform->VRemove(this);
        field_110_id = -1;
        SetAnim_43F9C0(eGlukkonMotions::M_WalkToFall_6_4434E0, TRUE);
    }
}

__int16 Glukkon::vTakeDamage_43FA40(BaseGameObject* pFrom)
{
    if (BrainIs(&Glukkon::AI_4_Death_442010))
    {
        return 1;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_15:
    {
        auto pBullet = static_cast<Bullet*>(pFrom);
        switch (pBullet->field_20_type)
        {
        case BulletType::Type_0:
        case BulletType::Type_2:
        {
            auto pBlood1 = alive_new<Blood>();
            if (pBlood1)
            {
                const FP yRand = (FP_FromInteger(Math_NextRandom() % 16)) - FP_FromInteger(8);
                const FP xRand = FP_FromInteger(Math_NextRandom() & 0xF); // TODO: Might be wrong as was trying to make this abs() but result is unsigned anyway ??

                const FP xPos = (field_CC_sprite_scale * (pBullet->field_30 <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6)));
                pBlood1->ctor_40F0B0(
                    xPos + field_B8_xpos,
                    field_BC_ypos - (FP_FromInteger(25) * field_CC_sprite_scale),
                    ((pBullet->field_30 <= FP_FromInteger(0) ? -FP_FromInteger(1) : FP_FromInteger(1)) * xRand + FP_FromInteger(16)),
                    yRand,
                    field_CC_sprite_scale,
                    12);
            }

            auto pBlood2 = alive_new<Blood>();
            if (pBlood2)
            {
                const FP xPos = (field_CC_sprite_scale * (pBullet->field_30 <= FP_FromInteger(0) ? -FP_FromInteger(12) : FP_FromInteger(12)));
                pBlood2->ctor_40F0B0(
                    xPos + field_B8_xpos,
                    field_BC_ypos - (FP_FromInteger(25) * field_CC_sprite_scale),
                    pBullet->field_30 <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    8);
            }
        }
            break;

        case BulletType::Type_1:
        case BulletType::Type_3:
        {
            auto pBlood2 = alive_new<Blood>();
            if (pBlood2)
            {
                pBlood2->ctor_40F0B0(
                    field_B8_xpos,
                    field_BC_ypos - (FP_FromInteger(25) * field_CC_sprite_scale),
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_CC_sprite_scale,
                    25);
            }
        }
        break;

        default:
            break;
        }

        field_200 = sGnFrame_5C1B84 + 5;

        if (field_106_current_motion == eGlukkonMotions::M_GetShot_21_443A60)
        {
            field_10C_health = FP_FromInteger(0);
            Event_Broadcast_422BC0(kEventMudokonComfort, this);
            return 1;
        }

        if (field_106_current_motion == eGlukkonMotions::M_KnockBack_3_442F40)
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                SetBrain(&Glukkon::AI_4_Death_442010);
                field_210 = 4;
            }
            field_10C_health = FP_FromInteger(0);
            Event_Broadcast_422BC0(kEventMudokonComfort, this);
            return 1;
        }

        field_204 = sGnFrame_5C1B84 + 20;
        SetAnim_43F9C0(eGlukkonMotions::M_GetShot_21_443A60, TRUE);

        SetBrain(&Glukkon::AI_4_Death_442010);
        field_210 = 4;

        if (pBullet->field_30 >= FP_FromInteger(0))
        {
            field_C4_velx = FP_FromDouble(0.001);
        }
        else
        {
            field_C4_velx = -FP_FromDouble(0.001);
        }
        field_10C_health = FP_FromInteger(0);
        Event_Broadcast_422BC0(kEventMudokonComfort, this);
    }
        break;

    case Types::eGrinder_30:
    case Types::eBaseBomb_46:
    case Types::eType_86:
    case Types::eExplosion_109:
        SetBrain(&Glukkon::AI_4_Death_442010);
        field_210 = 2;
        Event_Broadcast_422BC0(kEventMudokonComfort, this);
        break;

    case Types::eElectricWall_39:
        Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Unused_9, 0, field_1E0, this);
        return 1;

    case Types::eRockSpawner_48:
    case Types::eMineCar_89:
    case Types::eType_107:
        SetAnim_43F9C0(eGlukkonMotions::M_DeathFall_8_443760, TRUE);
        SetBrain(&Glukkon::AI_4_Death_442010);
        field_210 = 0;
        Event_Broadcast_422BC0(kEventMudokonComfort, this);
        break;

    case Types::eAbe_69:
        if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_62_Punch_454750)
        {
            if (Math_NextRandom() <= 32u)
            {
                Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Unused_9, 0, 0, 0);
            }
            else
            {
                Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Hey_0, 0, 0, 0);
            }
            field_1F8 = sGnFrame_5C1B84;
            SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
            SetBrain(&Glukkon::AI_2_Slapped_441720);
            field_210 = 0;
        }
        break;

    case Types::eSlog_126:
        if (field_106_current_motion != eGlukkonMotions::M_KnockBack_3_442F40)
        {
            field_10C_health = FP_FromInteger(0);
            SetBrain(&Glukkon::AI_4_Death_442010);
            field_210 = 5;
            Abe_SFX_2_457A40(13, 0, 32767, this);
            Event_Broadcast_422BC0(kEventMudokonComfort, this);
            if (!vIsFacingMe_4254A0(static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)))
            {
                field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
            }
            field_C4_velx = FP_FromInteger(0);
            SetAnim_43F9C0(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
        }
        break;

    case Types::eElectrocute_150:
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_10C_health = FP_FromInteger(0);
        SetBrain(&Glukkon::AI_4_Death_442010);
        field_210 = 3;
        field_1D4_timer = sGnFrame_5C1B84 + 1;
        Event_Broadcast_422BC0(kEventMudokonComfort, this);
        break;

    default:
        return 1;
    }

    return 1;
}
