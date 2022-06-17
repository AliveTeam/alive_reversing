#include "stdafx.h"
#include "UXB.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "BaseBomb.hpp"
#include "Grid.hpp"
#include <math.h>

const TintEntry sTintMap_UXB_563A3C[19] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 137u, 137u, 137u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};

void UXB_ForceLink()
{
}

void UXB::InitBlinkAnim_4DEED0(Animation* pAnimation)
{
    const AnimRecord& rec = AnimRec(AnimId::Bomb_RedGreenTick);
    if (pAnimation->Init_40A030(rec.mFrameTableOffset, gObjList_animations_5C1A24, this, rec.mMaxW, rec.mMaxH, Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId), 1u, 0, 0))
    {
        pAnimation->field_C_render_layer = field_20_animation.field_C_render_layer;
        pAnimation->field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        pAnimation->field_4_flags.Set(AnimFlags::eBit16_bBlending);
        pAnimation->field_14_scale = field_CC_sprite_scale;
        pAnimation->field_8_r = 128;
        pAnimation->field_9_g = 128;
        pAnimation->field_A_b = 128;
        pAnimation->field_B_render_mode = TPageAbr::eBlend_1;
    }
    else
    {
        mFlags.Set(Options::eListAddFailed_Bit1);
    }
}

void UXB::PlaySFX_4DE930(SoundEffect sfxIdx)
{
    if (gMap.Is_Point_In_Current_Camera_4810D0(
            this->field_C2_lvl_number,
            this->field_C0_path_number,
            this->field_B8_xpos,
            this->field_BC_ypos,
            0))
    {
        SFX_Play_46FA90(sfxIdx, 35);
    }
}

s32 UXB::IsColliding_4DF630()
{
    PSX_RECT uxbBound = {};
    vGetBoundingRect_424FD0(&uxbBound, 1);

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        if (pObj->field_114_flags.Get(e114_Bit6_SetOffExplosives) && pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            PSX_RECT objBound = {};
            pObj->vGetBoundingRect_424FD0(&objBound, 1);

            const s32 objX = FP_GetExponent(pObj->field_B8_xpos);
            const s32 objY = FP_GetExponent(pObj->field_BC_ypos);

            if (objX > uxbBound.x && objX < uxbBound.w && objY < uxbBound.h + 5 && uxbBound.x <= objBound.w && uxbBound.w >= objBound.x && uxbBound.h >= objBound.y && uxbBound.y <= objBound.h && pObj->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                return 1;
            }
        }
    }

    return 0;
}

void UXB::VUpdate()
{
    Update_4DF030();
}

void UXB::VRender(PrimHeader** ppOt)
{
    Render_4DF3D0(ppOt);
}

void UXB::VScreenChanged()
{
    ScreenChanged_4DF9C0();
}

s16 UXB::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_4DF850(pFrom);
}

UXB::UXB(Path_UXB* tlv_params, TlvItemInfoUnion itemInfo)
    : BaseAliveGameObject(0)
{
    SetType(AETypes::eUXB_143);

    const AnimRecord& activeRec = AnimRec(AnimId::UXB_Active);
    auto pResource = BaseGameObject::Add_Resource(ResourceManager::Resource_Animation, activeRec.mResourceId);
    Animation_Init(activeRec.mFrameTableOffset, activeRec.mMaxW, activeRec.mMaxH, pResource, 1, 1u);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = TPageAbr::eBlend_0;

    SetTint_425600(sTintMap_UXB_563A3C, gMap.mCurrentLevel);

    mFlags.Set(BaseGameObject::Options::eInteractive_Bit8);
    field_1C8_flags.Clear(UXB_Flags_1C8::eUnused_Bit0);
    field_118_state = UXBState::eDelay_0;

    field_1C0_pattern_length = tlv_params->field_10_pattern_length;
    if (tlv_params->field_10_pattern_length < 1 || tlv_params->field_10_pattern_length > 4)
    {
        field_1C0_pattern_length = 1;
    }


    field_1C4_pattern = tlv_params->field_12_pattern;
    if (!tlv_params->field_12_pattern) // If no pattern set, go to a default one.
    {
        field_1C4_pattern = 11111;
    }

    field_1C2_pattern_index = 0;
    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
    field_1C6_red_blink_count = (field_1C4_pattern / static_cast<s32>(pow(10, field_1C0_pattern_length - 1))) % 10;

    if (tlv_params->field_14_scale != Scale_short::eFull_0)
    {
        if (tlv_params->field_14_scale == Scale_short::eHalf_1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = Layer::eLayer_BombMineCar_Half_16;
            field_D6_scale = 0;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1.0);
        field_20_animation.field_C_render_layer = Layer::eLayer_BombMineCar_35;
        field_D6_scale = 1;
    }

    InitBlinkAnim_4DEED0(&field_128_animation);
    if (tlv_params->field_1_tlv_state) // Stores the activated/deactivated state for UXB.
    {
        if (tlv_params->field_16_start_state == Path_UXB::StartState::eOn_0)
        {
            field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kGrenflshResID, 0, 0), 0);
            field_1C8_flags.Clear(UXB_Flags_1C8::eIsRed_Bit1);
            const AnimRecord& flashRec = AnimRec(AnimId::Bomb_RedGreenTick);
            field_128_animation.Set_Animation_Data_409C80(flashRec.mFrameTableOffset, 0);
            PlaySFX_4DE930(SoundEffect::GreenTick_2);

            const AnimRecord& animRec = AnimRec(AnimId::UXB_Disabled);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            field_118_state = UXBState::eDeactivated_3;
            field_11A_starting_state = UXBState::eDelay_0;
        }
        else
        {
            field_11A_starting_state = UXBState::eDeactivated_3;
        }
    }
    else
    {
        if (tlv_params->field_16_start_state == Path_UXB::StartState::eOn_0)
        {
            field_11A_starting_state = UXBState::eDelay_0;
        }
        else
        {
            field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kGrenflshResID, 0, 0), 0);
            field_1C8_flags.Clear(UXB_Flags_1C8::eIsRed_Bit1);
            const AnimRecord& flashRec = AnimRec(AnimId::Bomb_RedGreenTick);
            field_128_animation.Set_Animation_Data_409C80(flashRec.mFrameTableOffset, 0);

            const AnimRecord& animRec = AnimRec(AnimId::UXB_Disabled);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            field_11A_starting_state = UXBState::eDeactivated_3;
            field_118_state = UXBState::eDeactivated_3;
        }
    }

    FP hitX = {};
    FP hitY = {};

    field_B8_xpos = FP_FromInteger((tlv_params->field_8_top_left.field_0_x + tlv_params->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(tlv_params->field_8_top_left.field_2_y);

    // Raycasts on ctor to place perfectly on the floor.
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            FP_FromInteger(tlv_params->field_8_top_left.field_2_y),
            field_B8_xpos,
            FP_FromInteger(tlv_params->field_8_top_left.field_2_y + 24),
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16)
        == 1)
    {
        field_BC_ypos = hitY;
    }

    field_120_tlv = itemInfo;
    field_124_next_state_frame = sGnFrame_5C1B84;
    field_11C_disabled_resources = static_cast<u16>(tlv_params->field_18_disabled_resources);

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbebombResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID);
    Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kGrenflshResID);

    if (!(field_11C_disabled_resources & 1))
    {
        Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);
    }
    if (!(field_11C_disabled_resources & 2))
    {
        Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID);
    }

    const FP gridSnap = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    mFlags.Set(Options::eInteractive_Bit8);
    field_DC_bApplyShadows |= 2u;

    field_E4_collection_rect.x = field_B8_xpos - (gridSnap / FP_FromDouble(2.0));
    field_E4_collection_rect.y = field_BC_ypos - gridSnap;
    field_E4_collection_rect.w = (gridSnap / FP_FromDouble(2.0)) + field_B8_xpos;
    field_E4_collection_rect.h = field_BC_ypos;
}


EXPORT void UXB::vOnPickUpOrSlapped_4DF540()
{
    if (field_118_state != UXBState::eExploding_2)
    {
        if (field_118_state != UXBState::eDeactivated_3 || field_124_next_state_frame > sGnFrame_5C1B84)
        {
            if (field_1C6_red_blink_count)
            {
                field_118_state = UXBState::eExploding_2;
                field_124_next_state_frame = sGnFrame_5C1B84 + 2;
            }
            else
            {
                const AnimRecord& flashRec = AnimRec(AnimId::Bomb_RedGreenTick);
                field_128_animation.Set_Animation_Data_409C80(flashRec.mFrameTableOffset, 0);
                PlaySFX_4DE930(SoundEffect::GreenTick_2);

                const AnimRecord& animRec = AnimRec(AnimId::UXB_Toggle);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                field_118_state = UXBState::eDeactivated_3;

                field_124_next_state_frame = sGnFrame_5C1B84 + 10;
            }
        }
        else
        {
            field_118_state = UXBState::eDelay_0;
            SetUpdateDelay(6);
            const AnimRecord& animRec = AnimRec(AnimId::UXB_Active);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            PlaySFX_4DE930(SoundEffect::RedTick_3);
        }
    }
}

void UXB::vOnThrowableHit_4DF7B0(BaseGameObject* /*pFrom*/)
{
    ae_new<BaseBomb>(field_B8_xpos,
                                  field_BC_ypos,
                                  0,
                                  field_CC_sprite_scale);
    field_118_state = UXBState::eExploding_2;
    mFlags.Set(BaseGameObject::eDead);
    field_124_next_state_frame = sGnFrame_5C1B84;
}

s16 UXB::vTakeDamage_4DF850(BaseGameObject* pFrom)
{
    if (mFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case AETypes::eAbe_69:
        case AETypes::eMudokon_110:
            if (field_118_state == UXBState::eDeactivated_3)
            {
                return 0;
            }
            break;

        case AETypes::eMineCar_89:
        case AETypes::eAbilityRing_104:
        case AETypes::eExplosion_109:
        case AETypes::eShrykull_121:
            break;

        default:
            return 0;
    }

    mFlags.Set(BaseGameObject::eDead);

    ae_new<BaseBomb>(field_B8_xpos,
                                 field_BC_ypos,
                                 0,
                                 field_CC_sprite_scale);
    field_118_state = UXBState::eExploding_2;
    field_124_next_state_frame = sGnFrame_5C1B84;

    return 1;
}

UXB::~UXB()
{
    if (field_118_state != UXBState::eExploding_2 || sGnFrame_5C1B84 < field_124_next_state_frame)
    {
        Path::TLV_Reset_4DB8E0(field_120_tlv.all, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_120_tlv.all, -1, 0, 1);
    }

    field_128_animation.vCleanUp_40C630();

    mFlags.Clear(Options::eInteractive_Bit8);
}

void UXB::Update_4DF030()
{
    switch (field_118_state)
    {
        case UXBState::eDelay_0:
            if (IsColliding_4DF630())
            {
                field_118_state = UXBState::eExploding_2;
                field_124_next_state_frame = sGnFrame_5C1B84 + 2;
            }
            else if (field_124_next_state_frame <= sGnFrame_5C1B84)
            {
                const AnimRecord& flashRec = AnimRec(AnimId::Bomb_Flash);
                field_118_state = UXBState::eActive_1;
                field_128_animation.Set_Animation_Data_409C80(flashRec.mFrameTableOffset, 0);
                field_124_next_state_frame = sGnFrame_5C1B84 + 2;
            }
            break;

        case UXBState::eDeactivated_3:
            // Do nothing
            break;

        case UXBState::eActive_1:
            if (IsColliding_4DF630())
            {
                field_118_state = UXBState::eExploding_2;
                field_124_next_state_frame = sGnFrame_5C1B84 + 2;
            }
            else if (field_124_next_state_frame <= sGnFrame_5C1B84)
            {
                if (field_1C6_red_blink_count)
                {
                    field_1C6_red_blink_count--;
                    if (field_1C6_red_blink_count == 0)
                    {
                        field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kGrenflshResID, 0, 0), 0);
                        field_1C8_flags.Clear(UXB_Flags_1C8::eIsRed_Bit1);
                    }
                }
                else
                {
                    const FrameInfoHeader* pFrameInfo = field_128_animation.Get_FrameHeader_40B730(-1);
                    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_128_animation.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);
                    field_128_animation.Load_Pal_40A530(field_128_animation.field_20_ppBlock, pFrameHeader->field_0_clut_offset);

                    field_1C8_flags.Set(UXB_Flags_1C8::eIsRed_Bit1);

                    field_1C2_pattern_index++;

                    if (field_1C2_pattern_index >= field_1C0_pattern_length)
                    {
                        field_1C2_pattern_index = 0;
                    }

                    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
                    field_1C6_red_blink_count = (field_1C4_pattern / static_cast<s32>(pow(10, field_1C0_pattern_length - field_1C2_pattern_index - 1))) % 10;
                }

                const AnimRecord& rec = AnimRec(AnimId::Bomb_RedGreenTick);
                field_128_animation.Set_Animation_Data_409C80(rec.mFrameTableOffset, 0);

                if (field_1C8_flags.Get(UXB_Flags_1C8::eIsRed_Bit1))
                {
                    PlaySFX_4DE930(SoundEffect::RedTick_3);
                }
                else
                {
                    PlaySFX_4DE930(SoundEffect::GreenTick_2);
                }

                field_118_state = UXBState::eDelay_0;
                field_124_next_state_frame = sGnFrame_5C1B84 + 10; // UXB change color delay.
            }
            break;

        case UXBState::eExploding_2:
            if (sGnFrame_5C1B84 >= field_124_next_state_frame)
            {
                ae_new<BaseBomb>(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
                mFlags.Set(Options::eDead);
            }
            break;
    }

    if (field_118_state != UXBState::eExploding_2)
    {
        if (Event_Get_422C00(kEventDeathReset))
        {
            if (field_11A_starting_state != UXBState::eDeactivated_3 || field_118_state == UXBState::eDeactivated_3)
            {
                if (field_11A_starting_state != UXBState::eDelay_0 || field_118_state != UXBState::eDeactivated_3)
                {
                    Path::TLV_Reset_4DB8E0(field_120_tlv.all, 0, 1, 0);
                }
                else
                {
                    Path::TLV_Reset_4DB8E0(field_120_tlv.all, 1, 1, 0);
                }
            }
            else
            {
                Path::TLV_Reset_4DB8E0(field_120_tlv.all, 1, 1, 0);
            }
            mFlags.Set(Options::eDead);
        }
    }
}

void UXB::Render_4DF3D0(PrimHeader** ppOt)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos,
                0))
        {
            field_128_animation.vRender_40B820(
                FP_GetExponent((field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x)),
                FP_GetExponent((field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y - FP_NoFractional(field_CC_sprite_scale * FP_FromInteger(17)))),
                ppOt,
                0,
                0);

            PSX_RECT frameRect;
            field_128_animation.Get_Frame_Rect_409E10(&frameRect);

            pScreenManager_5BB5F4->InvalidateRect_40EC90(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_5BB5F4->field_3A_idx);

            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
    }
}

void UXB::ScreenChanged_4DF9C0()
{
    BaseGameObject::VScreenChanged();

    const FP x_distance = FP_Abs(sControlledCharacter_5C1B8C->field_B8_xpos - field_B8_xpos);
    const FP y_distance = FP_Abs(sControlledCharacter_5C1B8C->field_BC_ypos - field_BC_ypos);

    if (y_distance > FP_FromInteger(520) || x_distance > FP_FromInteger(750))
    {
        if (field_11A_starting_state != UXBState::eDeactivated_3 || field_118_state == UXBState::eDeactivated_3)
        {
            if (field_11A_starting_state != UXBState::eDelay_0 || field_118_state != UXBState::eDeactivated_3)
            {
                Path::TLV_Reset_4DB8E0(field_120_tlv.all, 0, 1, 0);
                mFlags.Set(Options::eDead);
            }
            else
            {
                Path::TLV_Reset_4DB8E0(field_120_tlv.all, 1, 1, 0);
                mFlags.Set(Options::eDead);
            }
        }
        else
        {
            Path::TLV_Reset_4DB8E0(field_120_tlv.all, 1, 1, 0);
            mFlags.Set(Options::eDead);
        }
    }
}

s32 UXB::GetSaveState_4DFD40(u8* __pSaveBuffer)
{
    SaveState_UXB* pSaveState = reinterpret_cast<SaveState_UXB*>(__pSaveBuffer);

    pSaveState->field_0_id = AETypes::eUXB_143;
    pSaveState->field_4_tlv = field_120_tlv;
    pSaveState->field_8_next_state_frame = field_124_next_state_frame;
    pSaveState->field_C_state = field_118_state;
    pSaveState->field_E_starting_state = field_11A_starting_state;
    pSaveState->field_10_disabled_resources = field_11C_disabled_resources;
    pSaveState->field_12_pattern_index = field_1C2_pattern_index;
    pSaveState->field_14_red_blink_count = field_1C6_red_blink_count;
    pSaveState->field_16_is_red = field_1C8_flags.Get(UXB_Flags_1C8::eIsRed_Bit1);

    return sizeof(SaveState_UXB);
}

EXPORT s32 CC UXB::CreateFromSaveState_4DFAE0(const u8* __pSaveState)
{
    const SaveState_UXB* pSaveState = reinterpret_cast<const SaveState_UXB*>(__pSaveState);

    Path_UXB* uxbPath = reinterpret_cast<Path_UXB*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pSaveState->field_4_tlv.all));

    if (!(uxbPath->field_18_disabled_resources & 1) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", 0);
    }
    if (!(uxbPath->field_18_disabled_resources & 2) && !ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("DOGBLOW.BAN", 0);
    }
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kUXBResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("UXB.BND", 0);
    }
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170("EXPLODE.BND", 0);
    }

    UXB* pUXB = ae_new<UXB>(uxbPath, pSaveState->field_4_tlv);

    if (pSaveState->field_C_state == UXBState::eDeactivated_3)
    {
        pUXB->field_128_animation.Load_Pal_40A530(ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kGrenflshResID, 0, 0), 0);
        const AnimRecord& tickRec = AnimRec(AnimId::Bomb_RedGreenTick);
        pUXB->field_128_animation.Set_Animation_Data_409C80(tickRec.mFrameTableOffset, 0);
        const AnimRecord& animRec = AnimRec(AnimId::UXB_Disabled);
        pUXB->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
    }

    pUXB->field_124_next_state_frame = pSaveState->field_8_next_state_frame;
    pUXB->field_118_state = pSaveState->field_C_state;
    pUXB->field_11A_starting_state = pSaveState->field_E_starting_state;
    pUXB->field_11C_disabled_resources = pSaveState->field_10_disabled_resources;
    pUXB->field_1C2_pattern_index = pSaveState->field_12_pattern_index;
    pUXB->field_1C6_red_blink_count = pSaveState->field_14_red_blink_count;

    pUXB->field_1C8_flags.Clear(UXB_Flags_1C8::eIsRed_Bit1);

    if (pSaveState->field_16_is_red)
    {
        pUXB->field_1C8_flags.Set(UXB_Flags_1C8::eIsRed_Bit1);
    }

    return sizeof(SaveState_UXB);
}

s32 UXB::VGetSaveState(u8* __pSaveBuffer)
{
    return GetSaveState_4DFD40(__pSaveBuffer);
}

void UXB::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_4DF540();
}

void UXB::VOnThrowableHit(BaseGameObject* pFrom)
{
    vOnThrowableHit_4DF7B0(pFrom);
}
