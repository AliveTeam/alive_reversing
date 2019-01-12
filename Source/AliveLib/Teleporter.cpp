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

    field_54 = 0;

    field_32_bDestroySelf = 0;
    field_30_state = States::eState_0;
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

Teleporter* Teleporter::vdtor_4DC350(signed int flags)
{
    dtor_4DC380();
    if (flags & 1)
    {
        Mem_Free_495540(this);
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
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    field_32_bDestroySelf = 1;
}

BaseGameObject* CC Teleporter::Create_obj_4DCEB0()
{
    NOT_IMPLEMENTED();
    return nullptr;
}

class ParticleBurst : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParticleBurst* ctor_41CF50(FP /*xpos*/, FP /*ypos*/, unsigned int /*unknown_count1*/, FP /*scale*/, __int16 /*type*/, signed __int16 /*a7*/)
    {
        return this;
    }
private:
    int field_E4_not_used[4];
    BYTE** field_F4_ppResBuffer;
    void* field_F8_ptr_unknown;
    __int16 field_FC_count1;
    //__int16 field_FE_pad;
    int field_100_alive_timer;
    __int16 field_104_type;
    __int16 field_106_count2;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst, 0x108);

EXPORT BaseGameObject* CC New_Particle_426F40(FP xpos, FP ypos, FP scale)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kDeathFlareResID, FALSE, FALSE);
    auto pParticle = alive_new<Particle>();
    
    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->ctor_4CC4C0(xpos, ypos, 9912, 122, 43, ppRes);
    pParticle->field_20_animation.field_B_render_mode = 1;
    pParticle->field_CC_sprite_scale = FP_FromRaw(scale.fpValue * 2);

    if (scale == FP_FromInteger(1))
    {
        pParticle->field_20_animation.field_C_render_layer = 39;
    }
    else
    {
        pParticle->field_20_animation.field_C_render_layer = 17;
    }

    pParticle->field_DC_bApplyShadows &= ~1u;

    return pParticle;
}

void Teleporter::vUpdate_4DC400()
{
    NOT_IMPLEMENTED();

    BaseAliveGameObject* pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_50_objId));
    PSX_Point abeSpawnPos = {};
    switch (field_30_state)
    {
    case States::eState_0:
        if (field_32_bDestroySelf)
        {
            field_6_flags.Set(BaseGameObject::eDead);
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
            Path_Teleporter::kType))
        {
            return;
        }

        if (sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit10))
        {
            return;
        }

        field_30_state = States::eState_1;
        field_50_objId = Teleporter::Create_obj_4DCEB0()->field_8_object_id;

        SFX_Play_46FBA0(49u, 60, -400, 0x10000);
        sControlledCharacter_5C1B8C->field_114_flags.Set(Flags_114::e114_Bit10);

        gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&abeSpawnPos);

        /*
        LOWORD(xOrg) = field_34_mTlvData.field_22_eletric_x - abeSpawnPos.field_0_x;
        LOWORD(yOrg) = field_34_mTlvData.field_24_electric_y - abeSpawnPos.field_2_y;
        v5 = &unk_56398A;
        for (i = yOrg; ; yOrg = i)
        {
            if (v31->field_34_mTlvData.field_1C_scale)
            {
                v6 = xOrg + (signed __int16)*(v5 - 1) / 2;
                v7 = yOrg + (signed __int16)*v5 / 2;
            }
            else
            {
                LOWORD(v6) = xOrg + *(v5 - 1);
                LOWORD(v7) = *v5 + yOrg;
            }
            v8 = (BaseGameObject *)malloc_4954D0(0x68u);
            not_used = 0;
            if (v8)
            {
                Spark::ctor_4CBBB0(v8, (signed __int16)v6 << 16, (signed __int16)v7 << 16, 0x10000, 9, -31, 159, 1);
            }
            v5 += 2;
            not_used = -1;
            if ((signed int)v5 >= (signed int)&unk_5639A6)
            {
                break;
            }
        }*/
        return;

    case States::eState_1:
    {
        if (!pObj)
        {
            goto LABEL_56;
        }

        /* WRONG TYPE and wrong virtual
        if (((int(*)(BaseGameObject *))pObj->field_0_VTbl->VBaseAliveGameObject.field_1C_vGetBoundingRect_424FD0)(pObj) || field_54)
        {
            goto LABEL_55;
        }*/

        if (field_34_mTlvData.field_1C_scale)
        {
            /*
            New_Particles_426C70(
                sControlledCharacter_5C1B8C->field_B8_xpos,
                sControlledCharacter_5C1B8C->field_BC_ypos - 0x90000,
                sControlledCharacter_5C1B8C->field_CC_sprite_scale,
                3,
                128u,
                128u,
                128u);
            */

            auto pParticleBurst = alive_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    sControlledCharacter_5C1B8C->field_B8_xpos,
                    sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(9), // 18/2
                    9u,
                    FP_FromDouble(0.5),
                    3,
                    9);
            }
        }
        else
        {
            /*
            New_Particles_426C70(
                sControlledCharacter_5C1B8C->field_B8_xpos,
                sControlledCharacter_5C1B8C->field_BC_ypos - 1179648,
                sControlledCharacter_5C1B8C->field_CC_sprite_scale,
                3,
                128u,
                128u,
                128u);
            */
            auto pParticleBurst = alive_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    sControlledCharacter_5C1B8C->field_B8_xpos,
                    sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(18),
                    9u,
                    FP_FromInteger(1),
                    3,
                    9);
            }
        }

        field_54 = 1;
        //LABEL_55:
        if (!(pObj->field_6_flags.Get(BaseGameObject::eDead)))
        {
            return;
        }
    LABEL_56:

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
        field_30_state = States::eState_2;
    }
        return;

    case States::eState_2:
        gMap_5C3030.field_20 = 0;
        /*
        pTeleporterTlv = (Path_Teleporter *)Path::TLV_First_Of_Type_In_Camera_4DB6D0(sPath_dword_BB47C0, 88u, 0);
        pTeleporterTlvNext = pTeleporterTlv;
        qmemcpy(&tlvData, &pTeleporterTlv->field_10_data, sizeof(tlvData));
        if (tlvData.field_10_id != field_34_mTlvData.field_12_target_id)
        {
            while (1)
            {
                pTeleporterTlvNext = (Path_Teleporter *)Path::TLV_Next_Of_Type_4DB720(&pTeleporterTlv->field_0_mBase, 88u);
                qmemcpy(&tlvData, &pTeleporterTlvNext->field_10_data, sizeof(tlvData));
                if (tlvData.field_10_id == field_34_mTlvData.field_12_target_id)
                {
                    break;
                }
                pTeleporterTlv = pTeleporterTlvNext;
            }
        }*/
        //SFX_Play_46FBA0(0x31u, 60, -300, tlvData.field_1C_scale != 0 ? 0x8000 : 0x10000);
        gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&abeSpawnPos);
        /*
        v14 = *(_DWORD *)&tlvData.field_22_eletric_x - *(_DWORD *)&abeSpawnPos;
        v32 = *(_DWORD *)&tlvData.field_22_eletric_x - *(_DWORD *)&abeSpawnPos;
        v15 = *(_DWORD *)&tlvData.field_24_electric_y - *(_DWORD *)((char *)&abeSpawnPos + 2);
        v16 = &unk_56398A;
        for (j = *(_DWORD *)&tlvData.field_24_electric_y - *(_DWORD *)((char *)&abeSpawnPos + 2); ; v15 = j)
        {
            if (tlvData.field_1C_scale)
            {
                v17 = v14 + (signed __int16)*(v16 - 1) / 2;
                v14 = v15 + (signed __int16)*v16 / 2;
            }
            else
            {
                LOWORD(v17) = v14 + *(v16 - 1);
                LOWORD(v14) = v15 + *v16;
            }
            pSpark = (BaseGameObject *)malloc_4954D0(0x68u);
            xy = (PSX_Point)pSpark;
            not_used = 3;
            if (pSpark)
            {
                Spark::ctor_4CBBB0(pSpark, (signed __int16)v17 << 16, (signed __int16)v14 << 16, 0x10000, 9, -31, 159, 1);
                this = v31;
            }
            v16 += 2;
            not_used = -1;
            if ((signed int)v16 >= (signed int)&unk_5639A6)
            {
                break;
            }
            v14 = v32;
        }*/
        /*
        pChar2 = sControlledCharacter_5C1B8C;
        if (tlvData.field_1C_scale)
        {
            if (sControlledCharacter_5C1B8C->field_D6_scale == 1)
            {
                sControlledCharacter_5C1B8C->field_C4_velx = Math_FixedPoint_Multiply_496C50(
                    sControlledCharacter_5C1B8C->field_C4_velx,
                    0x8000);
                sControlledCharacter_5C1B8C->field_C8_vely = Math_FixedPoint_Multiply_496C50(
                    sControlledCharacter_5C1B8C->field_C8_vely,
                    0x8000);
                pChar2 = sControlledCharacter_5C1B8C;
            }
            pChar2->field_CC_sprite_scale = 0x8000;
            sControlledCharacter_5C1B8C->field_20_animation.field_C_render_layer = 13;
            sControlledCharacter_5C1B8C->field_D6_scale = 0;
        }
        else
        {
            if (!sControlledCharacter_5C1B8C->field_D6_scale)
            {
                sControlledCharacter_5C1B8C->field_C4_velx = Math_FixedPoint_Multiply_496C50(
                    sControlledCharacter_5C1B8C->field_C4_velx,
                    0x20000);
                sControlledCharacter_5C1B8C->field_C8_vely = Math_FixedPoint_Multiply_496C50(
                    sControlledCharacter_5C1B8C->field_C8_vely,
                    0x20000);
                pChar2 = sControlledCharacter_5C1B8C;
            }
            pChar2->field_CC_sprite_scale = 0x10000;
            sControlledCharacter_5C1B8C->field_20_animation.field_C_render_layer = 32;
            sControlledCharacter_5C1B8C->field_D6_scale = 1;
        }
        sControlledCharacter_5C1B8C->field_B8_xpos = (pTeleporterTlvNext->field_8_top_left.field_0_x << 16)
            + ((pTeleporterTlvNext->field_C_bottom_right.field_0_x
                - pTeleporterTlvNext->field_8_top_left.field_0_x)
                / 2 << 16);
        
        MapFollowMe_408D10(TRUE);
        */

        /*
        lineType = -(sControlledCharacter_5C1B8C->field_D6_scale != 0);
        LOBYTE(lineType) = lineType & 0x1F;
        if (Collisions::Raycast_417A60(
            sCollisions_DArray_5C1128,
            sControlledCharacter_5C1B8C->field_B8_xpos,
            pTeleporterTlvNext->field_8_top_left.field_2_y << 16,
            sControlledCharacter_5C1B8C->field_B8_xpos,
            pTeleporterTlvNext->field_C_bottom_right.field_2_y << 16,
            &pPathLine,
            &hitX,
            &hitY,
            lineType + 0xF0))
        {
            sControlledCharacter_5C1B8C->field_100_pCollisionLine = pPathLine;
            sControlledCharacter_5C1B8C->field_BC_ypos = hitY;
        }
        else
        {
            sControlledCharacter_5C1B8C->field_100_pCollisionLine = 0;
            sControlledCharacter_5C1B8C->field_BC_ypos = pTeleporterTlvNext->field_8_top_left.field_2_y << 16;
            sControlledCharacter_5C1B8C->field_F8 = sControlledCharacter_5C1B8C->field_BC_ypos;
        }*/
        field_30_state = States::eState_4;
        return;

    case States::eState_4:
    {
        PSX_RECT bRect = {};
        sControlledCharacter_5C1B8C->vGetBoundingRect_424FD0(&bRect, 1);

        const FP yOff = sControlledCharacter_5C1B8C->field_CC_sprite_scale * FP_FromInteger(60);
        New_Particle_426F40(
            FP_FromInteger((bRect.x + bRect.w) / 2),
            FP_FromInteger((bRect.y + bRect.h) / 2) + yOff,
            sControlledCharacter_5C1B8C->field_CC_sprite_scale);

        if (sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(0.5))
        {
            auto pParticleBurst = alive_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    sControlledCharacter_5C1B8C->field_B8_xpos,
                    sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(9),
                    6u,
                    FP_FromDouble(0.5),
                    3,
                    9);
            }
        }
        else
        {
            auto pParticleBurst = alive_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    sControlledCharacter_5C1B8C->field_B8_xpos,
                    sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(18),
                    6u,
                    FP_FromInteger(1),
                    3,
                    9);
            }
        }
        field_54 = 0;
        sControlledCharacter_5C1B8C->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        sControlledCharacter_5C1B8C->field_114_flags.Clear(Flags_114::e114_Bit10);
        field_2C_switch_state = SwitchStates_Get_466020(field_34_mTlvData.field_1A_trigger_id);
        field_30_state = States::eState_0;
    }
        break;

    default:
        return;
    }
}
