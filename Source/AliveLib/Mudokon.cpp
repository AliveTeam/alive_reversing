#include "stdafx.h"
#include "Mudokon.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Abe.hpp"

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

const __int16 word_55CF08[8] =
{
    0,
    1,
    3,
    6,
    7,
    0,
    0,
};

Mudokon* Mudokon::ctor_474F30(Path_Mudokon* pTlv, int tlvInfo)
{
    //NOT_IMPLEMENTED();

    ctor_408240(18);

    field_154 = 0;
    field_140 = -1;
    field_156 = -1;

    SetVTable(this, 0x5462E4);

    field_11C = -1;
    field_12C = -1;
    field_158_obj_id = -1;
    field_4_typeId = Types::eMudokon2_81; // TODO: Set to 110 later, what is 81 ??
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
    field_16A_flags.Set(Flags::eBit4_blind, pTlv->field_22_bBlind & 1);
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

    if (field_16A_flags.Get(Flags::eBit4_blind))
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
    const short bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(pTlv->field_8_top_left.field_0_x),
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
    field_118 = field_C_objectId;

    field_128 = 0;
    field_160_delayed_speak = -1;
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

    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type != -1)
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
                PSX_RECT v26 = {};
                vGetBoundingRect_424FD0(&v26, 1);

                vOnCollisionWith_424EE0(
                    { static_cast<short>(v26.x + 0x50000), v26.y }, // TODO: WTF? Isn't fixed point ??
                    { static_cast<short>(v26.w + 0x50000), v26.h }, ObjList_5C1B78,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
            }
        }

        field_104_collision_line_type = 0;

        if (field_11C != -1)
        {
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_C_objectId == field_11C)
                {
                    field_11C = pObj->field_8_object_id;
                    //word_5C3012++;
                    field_16C |= 4u;
                    if (field_18E_fns1_idx == 6 && field_190 == 3)
                    {
                        // push event ??
                        //((void(__stdcall *)(signed int))v10->field_0_VTbl->VBaseAliveGameObject.field_18_vOnCollisionWith_424EE0)(1);
                        field_20_animation.field_C_render_layer = field_CC_sprite_scale != FP_FromInteger(1) ? 11 : 30;
                    }
                    break;
                }
            }
        }

        if (field_158_obj_id != -1)
        {
            for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_C_objectId == field_158_obj_id)
                {
                    field_158_obj_id = pObj->field_8_object_id;
                    static_cast<BaseAliveGameObject*>(pObj)->VUnPosses_408F90();
                    break;
                }
            }
        }
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        FP v15 = field_B8_xpos- sControlledCharacter_5C1B8C->field_B8_xpos;
        if (v15 < FP_FromInteger(0))
        {
            v15 = sControlledCharacter_5C1B8C->field_B8_xpos - field_B8_xpos;
        }
        if (v15 > FP_FromInteger(750))
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            return;
        }

        FP v16 = field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos;
        if (v16 < FP_FromInteger(0))
        {
            v16 = sControlledCharacter_5C1B8C->field_BC_ypos - field_BC_ypos;
        }
        if (v16 > FP_FromInteger(520))
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        }
        else
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }

            const __int16 oldState = field_106_current_state;
            //field_190 = sMudokon_fns1_55CDF0[field_18E_fns1_idx]();
            FP oldXPos = field_B8_xpos;
            FP oldYPos = field_BC_ypos;
            //sMudokon_fns2_55CE18[field_106_current_state]();
            FP curXPos = field_B8_xpos;

            if (oldXPos != curXPos || oldYPos != field_BC_ypos)
            {
                field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                    nullptr,
                    curXPos,
                    field_BC_ypos,
                    curXPos,
                    field_BC_ypos);
                //field_0_VTbl->VBaseAliveGameObject.field_50_VOn_TLV_Collision_4087F0(v22);
            }

            if (oldState != field_106_current_state || field_114_flags.Get(Flags_114::e114_Bit2))
            {
                field_114_flags.Clear(Flags_114::e114_Bit2);
                //field_0_VTbl->VLiftPoint.PlatformBase__vsub_4975B0();
            }
            else if (field_192)
            {
                field_106_current_state = field_F4;
                //field_0_VTbl->VLiftPoint.PlatformBase__vsub_4975B0();
                field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                field_192 = 0;
            }
        }
    }
}

void Mudokon::SetPal_4772D0(__int16 palType)
{
    switch (palType)
    {
    case 0:
        if (field_16A_flags.Get(Flags::eBit4_blind))
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
