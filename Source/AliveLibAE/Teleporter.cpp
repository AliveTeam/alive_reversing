#include "stdafx.h"
#include "Teleporter.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ObjectIds.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Spark.hpp"
#include "ParticleBurst.hpp"
#include "Electrocute.hpp"

Teleporter* Teleporter::ctor_4DC1E0(Path_Teleporter* pTlv, DWORD tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x547AE0);

    field_4C_pTlv = pTlv; // TODO: Don't think this is used, and it can become a dangling ptr?
    field_34_mTlvData = pTlv->field_10_data;
    field_20_tlvInfo = tlvInfo;

    field_24_global_y1 = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y));
    field_28_global_y2 = FP_GetExponent((FP_FromInteger(pTlv->field_C_bottom_right.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y));
    field_26_global_x1 = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));
    field_2A_global_x2 = FP_GetExponent((FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));

    field_2C_switch_state = SwitchStates_Get_466020(field_34_mTlvData.field_1A_trigger_id);

    field_54_effect_created = 0;

    field_32_bDestroySelf = 0;
    field_30_state = States::eState_WaitForSwitchOn_0;
    field_50_objId = -1;

    return this;
}

BaseGameObject* Teleporter::VDestructor(signed int flags)
{
    return vdtor_4DC350(flags);
}

void Teleporter::VUpdate()
{
    vUpdate_4DC400();
}

void Teleporter::VScreenChanged()
{
    vScreenChanged_4DCE80();
}

Teleporter* Teleporter::vdtor_4DC350(signed int flags)
{
    dtor_4DC380();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Teleporter::dtor_4DC380()
{
    SetVTable(this, 0x547AE0);
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

void Teleporter::vScreenChanged_4DCE80()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    field_32_bDestroySelf = 1;
}

Electrocute* CC Teleporter::Create_ElectrocuteEffect_4DCEB0()
{
    auto pObj = ae_new<Electrocute>();
    pObj->ctor_4E5E80(sControlledCharacter_5C1B8C, TRUE, FALSE);
    return pObj;
}

const PSX_Point kSparkOffs_563988[8] =
{
    { -15, 46 },
    { -20, 29 },
    { -15, 12 },
    { 0, 2 },
    { 16, 12 },
    { 19, 28 },
    { 15, 45 },
    { 0, 0 }
};

void Teleporter::SpawnRingSparks(Path_Teleporter_Data* pTlvData)
{
    PSX_Point abeSpawnPos = {};
    gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&abeSpawnPos);

    const short xOrg = pTlvData->field_22_eletric_x - abeSpawnPos.field_0_x;
    const short yOrg = pTlvData->field_24_electric_y - abeSpawnPos.field_2_y;

    for (auto& sparkOffs : kSparkOffs_563988)
    {
        int sparkX = 0;
        int sparkY = 0;
        if (pTlvData->field_1C_scale)
        {
            sparkX = xOrg + (sparkOffs.field_0_x / 2);
            sparkY = yOrg + (sparkOffs.field_2_y / 2);
        }
        else
        {
            sparkX = xOrg + (sparkOffs.field_0_x);
            sparkY = yOrg + (sparkOffs.field_2_y);
        }

        auto pSpark = ae_new<Spark>();
        if (pSpark)
        {
            pSpark->ctor_4CBBB0(FP_FromInteger(sparkX), FP_FromInteger(sparkY), FP_FromInteger(1), 9, -31, 159, 1);
        }
    }
}

void Teleporter::vUpdate_4DC400()
{
    Electrocute* pObj = static_cast<Electrocute*>(sObjectIds_5C1B70.Find(field_50_objId, Types::eElectrocute_150));
    switch (field_30_state)
    {
    case States::eState_WaitForSwitchOn_0:
    {
        if (field_32_bDestroySelf)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }

        if (SwitchStates_Get_466020(field_34_mTlvData.field_1A_trigger_id) == field_2C_switch_state)
        {
            return;
        }

        field_2C_switch_state = SwitchStates_Get_466020(field_34_mTlvData.field_1A_trigger_id);

        if (!sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos),
            FP_GetExponent(sControlledCharacter_5C1B8C->field_BC_ypos),
            FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos),
            FP_GetExponent(sControlledCharacter_5C1B8C->field_BC_ypos),
            TlvTypes::Teleporter_88))
        {
            return;
        }

        if (sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit10))
        {
            return;
        }

        field_30_state = States::eState_Into_Teleporter_1;
        field_50_objId = Teleporter::Create_ElectrocuteEffect_4DCEB0()->field_8_object_id;

        SFX_Play_46FBA0(SoundEffect::Zap1_49, 60, -400);
        sControlledCharacter_5C1B8C->field_114_flags.Set(Flags_114::e114_Bit10);
        
        SpawnRingSparks(&field_34_mTlvData);
    }
    break;

    case States::eState_Into_Teleporter_1:
    {
        if (pObj)
        {
            if (pObj->VSub_4E6630() || field_54_effect_created)
            {
                if (!(pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)))
                {
                    return;
                }
            }

            // Only create the effects once (disable this if you like a crazy amount of sparks and things)
            if (!field_54_effect_created)
            {
                // Spawn the falling "red" sparks from Abe's feet that appear after you enter the teleporter
                if (field_34_mTlvData.field_1C_scale)
                {
                    // Steam/smoke effect at Abe's body
                    New_Smoke_Particles_426C70(
                        sControlledCharacter_5C1B8C->field_B8_xpos,
                        sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(9), // 18/2
                        sControlledCharacter_5C1B8C->field_CC_sprite_scale,
                        3,
                        128u,
                        128u,
                        128u);

                    auto pParticleBurst = ae_new<ParticleBurst>();
                    if (pParticleBurst)
                    {
                        pParticleBurst->ctor_41CF50(
                            sControlledCharacter_5C1B8C->field_B8_xpos,
                            sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(9), // 18/2
                            9u,
                            FP_FromDouble(0.5),
                            BurstType::eBigRedSparks_3,
                            9);
                    }
                }
                else
                {
                    // Steam/smoke effect at Abe's body
                    New_Smoke_Particles_426C70(
                        sControlledCharacter_5C1B8C->field_B8_xpos,
                        sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(18),
                        sControlledCharacter_5C1B8C->field_CC_sprite_scale,
                        3,
                        128u,
                        128u,
                        128u);

                    auto pParticleBurst = ae_new<ParticleBurst>();
                    if (pParticleBurst)
                    {
                        pParticleBurst->ctor_41CF50(
                            sControlledCharacter_5C1B8C->field_B8_xpos,
                            sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(18),
                            9u,
                            FP_FromInteger(1),
                            BurstType::eBigRedSparks_3,
                            9);
                    }
                }
                field_54_effect_created = 1;
            }

            if (!(pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)))
            {
                return;
            }
        }

        sControlledCharacter_5C1B8C->field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

        gMap_5C3030.field_20 = 1;

        const CameraSwapEffects effect = kPathChangeEffectToInternalScreenChangeEffect_55D55C[field_34_mTlvData.field_1E_wipe];
        short bForceChange = 0;
        if (effect == CameraSwapEffects::eEffect5_1_FMV || effect == CameraSwapEffects::eEffect11)
        {
            bForceChange = 1;
        }

        gMap_5C3030.SetActiveCam_480D30(
            field_34_mTlvData.field_18_level,
            field_34_mTlvData.field_16_path,
            field_34_mTlvData.field_14_camera,
            effect,
            field_34_mTlvData.field_20_movie_number,
            bForceChange);

        sControlledCharacter_5C1B8C->field_1C_update_delay = 3;
        sActiveHero_5C1B68->field_1A0_door_id = field_34_mTlvData.field_12_target_id;
        field_30_state = States::eState_Teleporting_2;
    }
    break;

    case States::eState_Teleporting_2:
    {
        gMap_5C3030.field_20 = 0;

        Path_Teleporter* pTeleporterTlv = static_cast<Path_Teleporter*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::Teleporter_88, 0));
        Path_Teleporter_Data tlvData = pTeleporterTlv->field_10_data;
        if (tlvData.field_10_id != field_34_mTlvData.field_12_target_id)
        {
            while (pTeleporterTlv)
            {
                pTeleporterTlv = static_cast<Path_Teleporter*>(sPath_dword_BB47C0->TLV_Next_Of_Type_4DB720(pTeleporterTlv, TlvTypes::Teleporter_88));
                tlvData = pTeleporterTlv->field_10_data;

                if (tlvData.field_10_id == field_34_mTlvData.field_12_target_id)
                {
                    break;
                }
            }
        }
        
        SFX_Play_46FBA0(SoundEffect::Zap1_49, 60, -300, tlvData.field_1C_scale != 0 ? FP_FromDouble(0.5) : FP_FromInteger(1));
        SpawnRingSparks(&tlvData);

        if (tlvData.field_1C_scale)
        {
            if (sControlledCharacter_5C1B8C->field_D6_scale == 1)
            {
                sControlledCharacter_5C1B8C->field_C4_velx *= FP_FromDouble(0.5);
                sControlledCharacter_5C1B8C->field_C8_vely *= FP_FromDouble(0.5);
            }
            sControlledCharacter_5C1B8C->field_CC_sprite_scale = FP_FromDouble(0.5);
            sControlledCharacter_5C1B8C->field_20_animation.field_C_render_layer = 13;
            sControlledCharacter_5C1B8C->field_D6_scale = 0;
        }
        else
        {
            if (sControlledCharacter_5C1B8C->field_D6_scale == 0)
            {
                sControlledCharacter_5C1B8C->field_C4_velx *= FP_FromInteger(2);
                sControlledCharacter_5C1B8C->field_C8_vely *= FP_FromInteger(2);
            }
            sControlledCharacter_5C1B8C->field_CC_sprite_scale = FP_FromInteger(1);
            sControlledCharacter_5C1B8C->field_20_animation.field_C_render_layer = 32;
            sControlledCharacter_5C1B8C->field_D6_scale = 1;
        }

        // XPos = TLV xpos + TLV middle point
        sControlledCharacter_5C1B8C->field_B8_xpos = FP_FromInteger(pTeleporterTlv->field_8_top_left.field_0_x) +
            FP_FromInteger((pTeleporterTlv->field_C_bottom_right.field_0_x - pTeleporterTlv->field_8_top_left.field_0_x) / 2);

        sControlledCharacter_5C1B8C->MapFollowMe_408D10(TRUE);

        const BYTE lineType = sControlledCharacter_5C1B8C->field_D6_scale == 0 ? 0xF0 : 0x1F;

        PathLine* pPathLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_5C1128->Raycast_417A60(
            sControlledCharacter_5C1B8C->field_B8_xpos,
            FP_FromInteger(pTeleporterTlv->field_8_top_left.field_2_y),
            sControlledCharacter_5C1B8C->field_B8_xpos,
            FP_FromInteger(pTeleporterTlv->field_C_bottom_right.field_2_y),
            &pPathLine,
            &hitX,
            &hitY,
            lineType))
        {
            sControlledCharacter_5C1B8C->field_100_pCollisionLine = pPathLine;
            sControlledCharacter_5C1B8C->field_BC_ypos = hitY;
        }
        else
        {
            sControlledCharacter_5C1B8C->field_100_pCollisionLine = nullptr;
            sControlledCharacter_5C1B8C->field_BC_ypos = FP_FromInteger(pTeleporterTlv->field_8_top_left.field_2_y);
            sControlledCharacter_5C1B8C->field_F8_LastLineYPos = sControlledCharacter_5C1B8C->field_BC_ypos;
        }
        field_30_state = States::eState_Out_of_teleporter_4;
    }
    break;

    case States::eState_Out_of_teleporter_4:
    {
        // Visual effects
        PSX_RECT bRect = {};
        sControlledCharacter_5C1B8C->vGetBoundingRect_424FD0(&bRect, 1);

        // White flash in the middle of Abe's body
        New_DestroyOrCreateObject_Particle_426F40(
            FP_FromInteger((bRect.x + bRect.w) / 2),
            FP_FromInteger((bRect.y + bRect.h) / 2) + (sControlledCharacter_5C1B8C->field_CC_sprite_scale * FP_FromInteger(60)),
            sControlledCharacter_5C1B8C->field_CC_sprite_scale);

        // Spawn the falling "red" sparks from Abe's feet that appear after you've arrived at the destination
        if (sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            auto pParticleBurst = ae_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    sControlledCharacter_5C1B8C->field_B8_xpos,
                    sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(9),
                    6u,
                    FP_FromDouble(0.5),
                    BurstType::eBigRedSparks_3,
                    9);
            }
        }
        else
        {
            auto pParticleBurst = ae_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    sControlledCharacter_5C1B8C->field_B8_xpos,
                    sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(18),
                    6u,
                    FP_FromInteger(1),
                    BurstType::eBigRedSparks_3,
                    9);
            }
        }

        field_54_effect_created = 0;
        sControlledCharacter_5C1B8C->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        sControlledCharacter_5C1B8C->field_114_flags.Clear(Flags_114::e114_Bit10);
        field_2C_switch_state = SwitchStates_Get_466020(field_34_mTlvData.field_1A_trigger_id);
        field_30_state = States::eState_WaitForSwitchOn_0;
    }
    break;
    }
}
