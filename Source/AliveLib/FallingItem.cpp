#include "stdafx.h"
#include "FallingItem.hpp"
#include "Shadow.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "Midi.hpp"
#include "ParticleBurst.hpp"
#include "Particle.hpp"
#include "ScreenShake.hpp"
#include "Abe.hpp"

const FallingItem_Data sFallingItemData_544DC0[18] =
{
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 8076, 8100, 86, 47 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 8076, 8100, 86, 47 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

ALIVE_VAR(1, 0x5BC208, FallingItem*, pPrimaryFallingItem_5BC208, nullptr);

EXPORT FallingItem* FallingItem::ctor_4272C0(Path_FallingItem* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x544E98);

    field_4_typeId = Types::eRockSpawner_48;

    field_6_flags.Set(BaseGameObject::eCanExplode);
    field_118_tlvInfo = tlvInfo;

    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kF2rockResID);
    const int lvlIdx = static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030);
    Animation_Init_424E10(
        sFallingItemData_544DC0[lvlIdx].field_0,
        sFallingItemData_544DC0[lvlIdx].field_8,
        sFallingItemData_544DC0[lvlIdx].field_A,
        ppRes,
        1,
        1);
    
    field_11E_id = pTlv->field_10_id;

    if (pTlv->field_12_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 12;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 31;
    }

    field_124_delay_time = pTlv->field_14_delay_time;
    field_120_num_items = pTlv->field_16_number_of_items;
    field_122_num_items_remaining = pTlv->field_16_number_of_items;
    field_134_bHitGrinderOrMineCar = FALSE;
    field_12C_reset_id = pTlv->field_18_reset_id;
    field_12E_do_sound_in_state_3_falling = TRUE;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (field_BC_ypos > pScreenManager_5BB5F4->field_20_pCamPos->field_4_y)
    {
        field_BC_ypos = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
    }

    field_138_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_13C_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_130_yPosStart = field_BC_ypos;
    field_11C_state = State::eState_0_WaitForIdEnable;
    field_140_sound_channels = 0;

    if (!pPrimaryFallingItem_5BC208)
    {
        pPrimaryFallingItem_5BC208 = this;
        field_144_created_gnFrame = sGnFrame_5C1B84;
    }

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

FallingItem* FallingItem::ctor_427560(__int16 xpos, __int16 ypos, __int16 scale, __int16 id, __int16 delayTime, __int16 numItems, __int16 resetId)
{
    ctor_408240(0);
   
    SetVTable(this, 0x544E98);
    field_4_typeId = Types::eRockSpawner_48;

    field_6_flags.Set(BaseGameObject::eCanExplode);
    field_118_tlvInfo = -1;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kF2rockResID);
    const int lvlIdx = static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030);
    Animation_Init_424E10(
        sFallingItemData_544DC0[lvlIdx].field_0,
        sFallingItemData_544DC0[lvlIdx].field_8,
        sFallingItemData_544DC0[lvlIdx].field_A,
        ppRes,
        1,
        1);

    field_20_animation.field_C_render_layer = 31;

    if (id)
    {
        field_11E_id = id;
    }
    else
    {
        field_11E_id = 1;
    }

    if (scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_124_delay_time = delayTime;
    
    field_120_num_items = numItems;
    field_122_num_items_remaining = numItems;

    const FP xFixed = FP_FromInteger(xpos);
    const FP yFixed = FP_FromInteger(ypos);

    field_12C_reset_id = resetId;
    field_134_bHitGrinderOrMineCar = FALSE;
    field_12E_do_sound_in_state_3_falling = TRUE;
    field_B8_xpos = xFixed;
    field_BC_ypos = yFixed;
    field_138_xpos = xFixed;
    field_13C_ypos = yFixed;
    field_130_yPosStart = yFixed;
    field_11C_state = State::eState_0_WaitForIdEnable;
    field_140_sound_channels = 0;

    if (!pPrimaryFallingItem_5BC208)
    {
        pPrimaryFallingItem_5BC208 = this;
        field_144_created_gnFrame = sGnFrame_5C1B84;
    }

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* FallingItem::VDestructor(signed int flags)
{
    return vdtor_427530(flags);
}

void FallingItem::VUpdate()
{
    vUpdate_427780();
}

void FallingItem::VScreenChanged()
{
    vScreenChanged_428180();
}

void FallingItem::dtor_427EB0()
{
    SetVTable(this, 0x544E98);
    if (pPrimaryFallingItem_5BC208 == this)
    {
        pPrimaryFallingItem_5BC208 = nullptr;
    }
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

FallingItem* FallingItem::vdtor_427530(signed int flags)
{
    dtor_427EB0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void FallingItem::vScreenChanged_428180()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        field_11C_state != State::eState_3_Falling)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

EXPORT void FallingItem::vUpdate_427780()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    // The primary item controls the main sound effects, otherwise there would be a crazy amount of smashing sounds
    if (pPrimaryFallingItem_5BC208 == this)
    {
        if (!((sGnFrame_5C1B84 - field_144_created_gnFrame) % 87))
        {
            if (field_D6_scale == 1)
            {
                SFX_Play_46FA90(0x4Au, 45);
            }
            else
            {
                SFX_Play_46FA90(0x4Au, 20);
            }
        }

        if (!((sGnFrame_5C1B84 - field_144_created_gnFrame) % 25))
        {
            if (field_D6_scale == 1)
            {
                SFX_Play_46FA90(0x4Bu, 45);
            }
            else
            {
                SFX_Play_46FA90(0x4Bu, 20);
            }
        }
    }

    switch (field_11C_state)
    {
    case State::eState_0_WaitForIdEnable:
        if (field_11E_id && SwitchStates_Get_466020(field_11E_id))
        {
            field_6_flags.Clear(BaseGameObject::eCanExplode);
            field_11C_state = State::eState_2_WaitForFallDelay;
            field_C4_velx = FP_FromInteger(0);
            field_C8_vely = FP_FromInteger(0);
            field_20_animation.Set_Animation_Data_409C80(sFallingItemData_544DC0[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_4, nullptr);
            field_128_delay_timer = sGnFrame_5C1B84 + field_124_delay_time;
        }
        break;

    // TODO: Must only be set outside of the object
    case State::eState_1_GoWaitForDelay:
        field_6_flags.Clear(BaseGameObject::eCanExplode);
        field_11C_state = State::eState_2_WaitForFallDelay;
        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);
        field_20_animation.Set_Animation_Data_409C80(sFallingItemData_544DC0[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_4, nullptr);
        field_128_delay_timer = sGnFrame_5C1B84 + field_124_delay_time;
        break;

    case State::eState_2_WaitForFallDelay:
        if (static_cast<int>(sGnFrame_5C1B84) >= field_128_delay_timer)
        {
            field_11C_state = State::eState_3_Falling;
            field_12E_do_sound_in_state_3_falling = TRUE;
            if (field_D6_scale == 1)
            {
                field_140_sound_channels = SFX_Play_46FBA0(0x17u, 50, -2600);
            }
            else
            {
                field_140_sound_channels = SFX_Play_46FBA0(0x17u, 25, -2600);
            }
        }
        break;

    case State::eState_3_Falling:
    {
        if (field_12E_do_sound_in_state_3_falling)
        {
            if (field_BC_ypos >= sActiveHero_5C1B68->field_BC_ypos - FP_FromInteger(240 / 2))
            {
                field_12E_do_sound_in_state_3_falling = FALSE;
                if (field_D6_scale == 1)
                {
                    SFX_Play_46FBA0(0x17u, 127, -1300);
                }
                else
                {
                    SFX_Play_46FBA0(0x17u, 64, -1300);
                }
            }
        }

        DamageHitItems_427F40();

        if (field_C8_vely < FP_FromInteger(20))
        {
            field_C8_vely += field_CC_sprite_scale * FP_FromDouble(1.8);
        }

        PathLine* pathLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_C8_vely + field_BC_ypos,
            &pathLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16) == 1)
        {
            if (!field_134_bHitGrinderOrMineCar)
            {
                field_BC_ypos = hitY;
            }
        }
        else if (!field_134_bHitGrinderOrMineCar)
        {
            field_BC_ypos += field_C8_vely;
            return;
        }

        field_134_bHitGrinderOrMineCar = FALSE;
        field_11C_state = State::eState_4_Smashed;

        auto pShake = alive_new<ScreenShake>();
        if (pShake)
        {
            pShake->ctor_4ACF70(0, field_CC_sprite_scale == FP_FromDouble(0.5));
        }

        if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8)
        {
            auto pPart = alive_new<ParticleBurst>();
            if (pPart)
            {
                pPart->ctor_41CF50(
                    field_B8_xpos,
                    field_BC_ypos,
                    0x14u,
                    field_CC_sprite_scale,
                    BurstType::eSticks_1,
                    13);
            }

            auto pParticle = alive_new<Particle>();
            if (pParticle)
            {
                BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, 0, 0);
                pParticle->ctor_4CC4C0(
                    field_B8_xpos,
                    field_BC_ypos - (FP_FromInteger(15) * field_CC_sprite_scale),
                    51156,
                    202,
                    91,
                    ppRes);

                pParticle->field_20_animation.field_B_render_mode = 1;
                pParticle->field_CC_sprite_scale = field_CC_sprite_scale * FP_FromDouble(0.75);
            }
        }
        else
        {
            auto pPartBurst = alive_new<ParticleBurst>();
            if (pPartBurst)
            {
                pPartBurst->ctor_41CF50(
                    field_B8_xpos,
                    field_BC_ypos,
                    25,
                    field_CC_sprite_scale,
                    BurstType::eFallingRocks_0,
                    13);
            }
        }
    }
        break;

    case State::eState_4_Smashed:
        if (field_140_sound_channels)
        {
            SND_Stop_Channels_Mask_4CA810(field_140_sound_channels);
field_140_sound_channels = 0;
        }

        Event_Broadcast_422BC0(kEventLoudNoise, this);
        SFX_Play_46FA90(0x3Eu, 0, field_CC_sprite_scale);

        if (field_D6_scale == 1)
        {
            SFX_Play_46FBA0(0x2Fu, 110, -1536);
        }
        else
        {
            SFX_Play_46FBA0(0x2Fu, 55, -1536);
        }

        if (field_11E_id)
        {
            if (field_12C_reset_id)
            {
                SwitchStates_Do_Operation_465F00(field_11E_id, SwitchOp::eSetFalse_1);
            }
        }

        --field_122_num_items_remaining;

        if (field_120_num_items > 0 && field_122_num_items_remaining <= 0 || !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_138_xpos,
            field_13C_ypos,
            0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        else
        {
            field_20_animation.Set_Animation_Data_409C80(sFallingItemData_544DC0[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_0, nullptr);
            field_6_flags.Set(BaseGameObject::eCanExplode);
            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            field_BC_ypos = field_130_yPosStart;
            field_11C_state = State::eState_0_WaitForIdEnable;
        }
        break;

    default:
        return;
    }
}

void FallingItem::DamageHitItems_427F40()
{
    for (int idx = 0; idx < gBaseGameObject_list_BB47C4->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj != this)
        {
            if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject) || pObj->field_4_typeId == Types::eGrinder_30)
            {
                BaseAnimatedWithPhysicsGameObject* pAliveObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                PSX_RECT fallingItemRect = {};
                vGetBoundingRect_424FD0(&fallingItemRect, 1);

                PSX_RECT objRect = {};
                pAliveObj->vGetBoundingRect_424FD0(&objRect, 1);

                if (pAliveObj->field_CC_sprite_scale == field_CC_sprite_scale)
                {
                    if (pAliveObj->field_4_typeId == Types::eGrinder_30 || pAliveObj->field_4_typeId == Types::eMineCar_89)
                    {
                        objRect.x += pAliveObj->field_DA_xOffset;
                        objRect.y += pAliveObj->field_D8_yOffset;

                        objRect.w += pAliveObj->field_DA_xOffset;
                        objRect.h += pAliveObj->field_D8_yOffset;
                    }

                    if (PSX_Rects_overlap_no_adjustment(&fallingItemRect, &objRect))
                    {
                        if (pAliveObj->field_4_typeId == Types::eGrinder_30)
                        {
                            // Grinder is not a type that implements VTakeDamage
                            field_134_bHitGrinderOrMineCar = TRUE;
                        }
                        else if (pAliveObj->field_4_typeId == Types::eMineCar_89)
                        {
                            // ?? Could still call VTakeDamage here but OG doesn't ??
                            field_134_bHitGrinderOrMineCar = TRUE;
                        }
                        else
                        {
                            bool doDamage = true;
                            if (pAliveObj->field_4_typeId == Types::eParamite_96)
                            {
                                // Some strange edge case for paramites - prevents them getting smashed by
                                // falling items when stood on an edge by their huge heads peeking over a bit.
                                if (pAliveObj->field_B8_xpos < FP_FromInteger(fallingItemRect.x) ||
                                    pAliveObj->field_B8_xpos > FP_FromInteger(fallingItemRect.w))
                                {
                                    doDamage = false;
                                }
                            }

                            if (doDamage)
                            {
                               static_cast<BaseAliveGameObject*>(pAliveObj)->VTakeDamage_408730(this);
                            }
                        }
                    }
                }
            }
        }
    }
}
