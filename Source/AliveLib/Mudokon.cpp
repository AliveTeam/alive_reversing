#include "stdafx.h"
#include "Mudokon.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"

const TintEntry kMudTints_55C744[18] =
{
    { 1, 87u, 103u, 67u },
    { 2, 87u, 103u, 67u },
    { 3, 87u, 103u, 67u },
    { 4, 87u, 103u, 67u },
    { 5, 87u, 103u, 67u },
    { 6, 87u, 103u, 67u },
    { 7, 87u, 103u, 67u },
    { 8, 87u, 103u, 67u },
    { 9, 87u, 103u, 67u },
    { 10, 87u, 103u, 67u },
    { 11, 87u, 103u, 67u },
    { 12, 87u, 103u, 67u },
    { 13, 87u, 103u, 67u },
    { 14, 87u, 103u, 67u },
    { -1, 87u, 103u, 67u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};

const __int16 word_55CF08[6] =
{
    0,
    1,
    3,
    6,
    7,
};

Mudokon* Mudokon::ctor_474F30(Path_Mudokon* pTlv, int tlvInfo)
{
    ctor_408240(18);

    field_154 = 0;
    field_140 = -1;
    field_156 = -1;

    SetVTable(this, 0x5462E4);

    field_11C = -1;
    field_12C = -1;
    field_158_obj_id = -1;
    field_4_typeId = Types::eType_81; // TODO: Set to 110 later, what is 81 ??
    field_C_objectId = tlvInfo;
    field_194 = 0;
    field_18E_fns1_idx = 0;
    field_190 = 0;
    field_108_delayed_state = -1;
    field_192 = 0;
    field_13C_voice_pitch = 0;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 55, TRUE, FALSE));
    field_10_resources_array.SetAt(1, nullptr);
    Animation_Init_424E10(58888, 135, 80, field_10_resources_array.ItemAt(0), 1, 1);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_DC_bApplyShadows |= 2u;

    field_114_flags.Clear(Flags_114::e114_Bit9);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_18C = 0;
    SetTint_425600(kMudTints_55C744, field_C2_lvl_number);

    field_17E = 17;

    field_16A_flags.Clear(Flags::eBit2_save_state);
    field_16A_flags.Clear(Flags::eBit3);
    field_16A_flags.Set(Flags::eBit4, pTlv->field_22_bBlind & 1);
    field_16A_flags.Clear(Flags::eBit5);
    field_16A_flags.Clear(Flags::eBit6);
    field_16A_flags.Clear(Flags::eBit7);
    field_16A_flags.Clear(Flags::eBit8);
    field_16A_flags.Clear(Flags::eBit9);
    // 10 and 11 ??
    field_16A_flags.Set(Flags::eBit12);
    field_16A_flags.Clear(Flags::eBit13);
    field_16A_flags.Clear(Flags::eBit14);

    field_180_emo_tbl = word_55CF08[static_cast<int>(pTlv->field_20_emotion) & ALIVE_COUNTOF(word_55CF08)]; // TODO: Couple more entries in here, why?
    field_188 = 0;
    field_182 = -1;
    field_120_angry_trigger = pTlv->field_24_angry_trigger;
    field_16C &= ~6u;
    field_198 = 0;

    switch (pTlv->field_12_state)
    {
    case Mud_State::eChisle_0:
        field_18E_fns1_idx = 1;
        field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 511, TRUE, FALSE));
        break;

    case Mud_State::eScrub_1:
        field_18E_fns1_idx = 2;
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 510, TRUE, FALSE));
        break;

    case Mud_State::eAngryWorker_2:
        field_18E_fns1_idx = 8;
        field_180_emo_tbl = 1;
        field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 510, TRUE, FALSE));
        break;

    case Mud_State::eDamageRingGiver_3:
    case Mud_State::eHealthRingGiver_4:
        if (pTlv->field_12_state == Mud_State::eDamageRingGiver_3)
        {
            field_168 = 2;
        }
        else
        {
            field_168 = 11;
        }
        field_164_ring_timeout = pTlv->field_2A_ring_timeout;
        field_16A_flags.Set(Flags::eBit16_instant_power_up, pTlv->field_2C_bInstant_power_up & 1);
        field_16C &= ~1;
        field_18E_fns1_idx = 0;
        field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 48, TRUE, FALSE));
        break;

    default:
        break;
    }

    if (field_180_emo_tbl == 6)
    {
        field_18E_fns1_idx = 4;
    }

    field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 514, TRUE, FALSE));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 517, TRUE, FALSE));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 10, TRUE, FALSE));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 27, TRUE, FALSE));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 26, TRUE, FALSE));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 43, TRUE, FALSE));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 512, TRUE, FALSE));
    field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 530, TRUE, FALSE));
    field_10_resources_array.SetAt(14, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 531, TRUE, FALSE));
    field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 532, TRUE, FALSE));
    field_10_resources_array.SetAt(16, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 533, TRUE, FALSE));
    field_10_resources_array.SetAt(17, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, 534, TRUE, FALSE));

    if (field_16A_flags.Get(Flags::eBit4))
    {
        SetPal_4772D0(0);
    }

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_14_direction == Mud_Direction::eLeft_0);

    field_4_typeId = Types::eMudokon_110;

    field_13C_voice_pitch = pTlv->field_16_voice_pitch;
    field_17A_rescue_id = pTlv->field_18_rescue_id;

    field_16A_flags.Set(Flags::eBit2_save_state, pTlv->field_1E_save_state & 1);
    field_16A_flags.Set(Flags::eBit10_stop_trigger, pTlv->field_26_stop_trigger & 1);
    field_16A_flags.Set(Flags::eBit11_get_depressed, pTlv->field_28_bGets_depressed & 1);
    field_16A_flags.Set(Flags::eBit15_ring_timeout, pTlv->field_2A_ring_timeout & 1);

    field_17C = 0;

    vStackOnObjectsOfType_425840(Types::eMudokon_110);

    if (field_180_emo_tbl == 7)
    {
        field_18E_fns1_idx = 9;
        field_114_flags.Set(Flags_114::e114_Bit2);
        field_106_current_state = 15;
    }
    else
    {
        field_106_current_state = 0;
    }

    if (pTlv->field_10_scale == TLV_Scale::Half_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 9;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == TLV_Scale::Full_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 28;
        field_D6_scale = 1;
    }

    FP hitX = {};
    FP hitY = {};
    const FP tlvXPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    const short bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        tlvXPos,
        FP_FromInteger(pTlv->field_8_top_left.field_2_y),
        FP_FromInteger(pTlv->field_C_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_C_bottom_right.field_2_y),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16);

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    if (bCollision)
    {
        field_BC_ypos = hitY;
        if (field_100_pCollisionLine->field_8_type == 32 || field_100_pCollisionLine->field_8_type == 36)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            vOnCollisionWith_424EE0(
                { bRect.x, static_cast<short>(bRect.y + 5) },
                { bRect.w, static_cast<short>(bRect.h + 5) },
                ObjList_5C1B78,
                1,
                (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
    }

    field_20_animation.field_1C_fn_ptr_array = kAbe_Anim_Frame_Fns_55EF98;
    field_F8 = field_BC_ypos;

    if (field_18E_fns1_idx == 2 || field_18E_fns1_idx == 1)
    {
        MapFollowMe_408D10(TRUE);
    }

    field_16A_flags.Set(Flags::eBit1);
    field_190 = 0;
    field_118 = tlvXPos;
    field_128 = 0;
    field_160 = -1;
    field_162 = field_DA_xOffset;
    
    field_E0_176_ptr = alive_new<Shadow>();
    if (field_E0_176_ptr)
    {
        field_E0_176_ptr->ctor_4AC990();
    }

    vUpdate_4757A0();

    return this;
}

void Mudokon::vUpdate_4757A0()
{
    NOT_IMPLEMENTED();
}

void Mudokon::SetPal_4772D0(__int16 palType)
{
    switch (palType)
    {
    case 0:
        if (field_16A_flags.Get(Flags::eBit4))
        {
            field_D0_r = 63;
            field_D2_g = 63;
            field_D4_b = 63;
            field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(16), 0);
        }
        else
        {
            field_D0_r = 87;
            field_D2_g = 103;
            field_D4_b = 67;

            FrameInfoHeader* pFrameInfoHeader = field_20_animation.Get_FrameHeader_40B730(-1);
            FrameHeader* pHeader = reinterpret_cast<FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);
            field_20_animation.Load_Pal_40A530(field_20_animation.field_20_ppBlock, pHeader->field_0_clut_offset);
        }
        break;

    case 1:
    case 2:
        field_D0_r = 63;
        field_D2_g = 63;
        field_D4_b = 63;
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(13), 0);
        break;

    case 3:
    case 4:
        field_D0_r = 63;
        field_D2_g = 63;
        field_D4_b = 63;
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(14), 0);
        break;

    case 5:
    case 6:
        field_D0_r = 74;
        field_D2_g = 74;
        field_D4_b = 74;
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(15), 0);
        break;

    case 7:
        field_D0_r = 63;
        field_D2_g = 63;
        field_D4_b = 63;
        field_20_animation.Load_Pal_40A530(field_10_resources_array.ItemAt(17), 0);
        break;

    default:
        return;
    }
}
