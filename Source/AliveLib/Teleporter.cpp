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
#include "VRam.hpp"
#include "ParticleBurst.hpp"

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

// Overwrites a pallete 8 colours at a time one per update
class PalleteOverwriter : public BaseGameObject
{
public:
    EXPORT PalleteOverwriter* ctor_4228D0(PSX_Point palXY, __int16 palDepth, __int16 colour)
    {
        BaseGameObject_ctor_4DBFA0(FALSE, 0);

        SetVTable(this, 0x544BC4); // vTbl_Class_544BC4

        field_4_typeId = Types::eType_44;

        gObjList_drawables_5C1124->Push_Back(this);

        field_20_pal_xy = palXY;
        field_24_pal_colours_count = palDepth;

        field_6_flags.Set(BaseGameObject::eDrawable);

        for (auto& palBufferEntry : field_B8_palBuffer)
        {
            palBufferEntry = colour;
        }

        field_CA_pal_w = 8;
        field_C8_pal_x_index = 1;
        field_CC_bFirstUpdate = 1;
        field_CE_bDone = FALSE;

        return this;
    }

    virtual void VUpdate() override
    {
        vUpdate_422A70();
    }

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_422B30(pOrderingTable);
    }

    virtual void VScreenChanged() override
    {
        // Stayin' alive
    }

private:
    EXPORT void vUpdate_422A70()
    {
        if (field_CC_bFirstUpdate || field_CE_bDone)
        {
            // First time round or when done do nothing
            field_CC_bFirstUpdate = FALSE;
        }
        else
        {
            if (field_C8_pal_x_index == field_24_pal_colours_count - 1)
            {
                // Got to the end
                field_CE_bDone = TRUE;
            }
            else
            {
                field_C8_pal_x_index += 8;

                if (field_C8_pal_x_index >= field_24_pal_colours_count - 1)
                {
                    field_C8_pal_x_index = field_24_pal_colours_count - 1;
                }

                if (field_C8_pal_x_index + field_CA_pal_w >= field_24_pal_colours_count - 1)
                {
                    field_CA_pal_w = field_24_pal_colours_count - field_C8_pal_x_index;
                }
            }
        }
    }

    EXPORT void dtor_4229F0()
    {
        SetVTable(this, 0x544BC4); // vTbl_Class_544BC4
        gObjList_drawables_5C1124->Remove_Item(this);
        BaseGameObject_dtor_4DBEC0();
    }

    EXPORT PalleteOverwriter* vdtor_4229C0(signed int flags)
    {
        dtor_4229F0();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    EXPORT void vRender_422B30(int** /*pOt*/)
    {
        if (!field_CE_bDone)
        {
            PSX_RECT rect  = {};
            rect.y = field_20_pal_xy.field_2_y;
            rect.x = field_20_pal_xy.field_0_x + field_C8_pal_x_index;
            rect.w = field_CA_pal_w;
            rect.h = 1;
            PSX_LoadImage16_4F5E20(&rect, reinterpret_cast<BYTE*>(&field_B8_palBuffer[0]));
        }
    }

private:
    PSX_Point field_20_pal_xy;
    short field_24_pal_colours_count;
    // pad
    int field_28_not_used[36]; // TODO: Probably something used in PSX but not PC?
    __int16 field_B8_palBuffer[8];
    __int16 field_C8_pal_x_index;
    __int16 field_CA_pal_w;
    __int16 field_CC_bFirstUpdate;
    __int16 field_CE_bDone;
};
ALIVE_ASSERT_SIZEOF(PalleteOverwriter, 0xD0);


class Class_548100 : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_4E6060(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_4E6240();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_4E65E0();
    }

    // New virtuals
    virtual void VSub_4E6150()
    {
        vSub_4E6150();
    }

    virtual int VSub_4E6630()
    {
        return vSub_4E6630();
    }

    EXPORT Class_548100* ctor_4E5E80(BaseAliveGameObject* pTargetObj, __int16 b1, __int16 b2)
    {
        BaseGameObject_ctor_4DBFA0(TRUE, 0);

        SetVTable(this, 0x548100); // vTbl_Class_548100
        field_4_typeId = Types::eType_150;

        field_20_target_obj_id = pTargetObj->field_8_object_id;
        field_44_state = 0;
        field_3C_b1 = b1;
        field_2C_b2 = b2;
        field_2E_count = b1 ? 3 : 2;
        field_40_pPalData = nullptr;

        switch (pTargetObj->field_4_typeId)
        {
        case Types::eFlyingSlig_54:
        case Types::eGlukkon_67:
        case Types::eType_Abe_69:
        case Types::eSlig_125:
            field_40_pPalData = reinterpret_cast<WORD*>(malloc_non_zero_4954F0(sizeof(WORD) * pTargetObj->field_20_animation.field_90_pal_depth));
            Pal_Copy_483560(
                pTargetObj->field_20_animation.field_8C_pal_vram_x,
                pTargetObj->field_20_animation.field_90_pal_depth,
                field_40_pPalData,
                &field_4C_pal_rect);
            break;
        default:
            break;
        }

        // Note: Real game may leave a ptr un-inited depending on the count
        // we just do them all because its easier and safer.
        for (auto& pPalOverwriter : field_30_pPalOverwriters)
        {
            pPalOverwriter = nullptr;
        }

        return this;
    }

private:
    EXPORT Class_548100* vdtor_4E6060(signed int flags)
    {
        dtor_4E6090();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    EXPORT void dtor_4E6090()
    {
        SetVTable(this, 0x548100); // vTbl_Class_548100

        for (int i = 0; i < field_2E_count; i++)
        {
            if (field_30_pPalOverwriters[i])
            {
                field_30_pPalOverwriters[i]->VDestructor(1);
            }
        }

        field_20_target_obj_id = -1;

        if (field_40_pPalData)
        {
            Mem_Free_495560(field_40_pPalData);
        }

        // NOTE: omitted vtable vTbl_IClass_548128
        BaseGameObject_dtor_4DBEC0();
    }

    EXPORT void vScreenChanged_4E65E0()
    {
        BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_20_target_obj_id));
        // If the map has changed or target we are tracking has died then..
        if (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710() || (pTargetObj && pTargetObj->field_6_flags.Get(BaseGameObject::eDead)))
        {
            VSub_4E6150();
        }
    }

    EXPORT void vUpdate_4E6240()
    {
        NOT_IMPLEMENTED();
        // standalone hack
        field_6_flags.Set(BaseGameObject::eDead);
    }

    EXPORT void vSub_4E6150()
    {
        NOT_IMPLEMENTED();
    }

    EXPORT int vSub_4E6630()
    {
        NOT_IMPLEMENTED();
        return 0;
    }

private:
    int field_20_target_obj_id;
    __int16 field_24_r;
    __int16 field_26_g;
    __int16 field_28_b;
    __int16 field_2A_pad; // NOTE: Crashes if commented out - why?
    __int16 field_2C_b2;
    __int16 field_2E_count;
    PalleteOverwriter* field_30_pPalOverwriters[3];
    __int16 field_3C_b1;
    __int16 field_3E;
    WORD* field_40_pPalData;
    __int16 field_44_state;
    __int16 field_46_pad;   // Ditto
    int field_48;
    PSX_RECT field_4C_pal_rect;
};
ALIVE_ASSERT_SIZEOF(Class_548100, 0x54);

Class_548100* CC Teleporter::Create_obj_4DCEB0()
{
    auto pObj = alive_new<Class_548100>();
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

void Teleporter::SpawnRingSparks()
{
    PSX_Point abeSpawnPos = {};
    gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&abeSpawnPos);

    const short xOrg = field_34_mTlvData.field_22_eletric_x - abeSpawnPos.field_0_x;
    const short yOrg = field_34_mTlvData.field_24_electric_y - abeSpawnPos.field_2_y;

    for (auto& sparkOffs : kSparkOffs_563988)
    {
        int sparkX = 0;
        int sparkY = 0;
        if (field_34_mTlvData.field_1C_scale)
        {
            sparkX = xOrg + (sparkOffs.field_0_x / 2);
            sparkY = yOrg + (sparkOffs.field_2_y / 2);
        }
        else
        {
            sparkX = xOrg + (sparkOffs.field_0_x);
            sparkY = yOrg + (sparkOffs.field_2_y);
        }

        auto pSpark = alive_new<Spark>();
        if (pSpark)
        {
            pSpark->ctor_4CBBB0(FP_FromInteger(sparkX), FP_FromInteger(sparkY), FP_FromInteger(1), 9, -31, 159, 1);
        }
    }
}

void Teleporter::vUpdate_4DC400()
{
    Class_548100* pObj = static_cast<Class_548100*>(sObjectIds_5C1B70.Find_449CF0(field_50_objId));
    switch (field_30_state)
    {
    case States::eState_WaitForSwitchOn_0:
    {
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

        field_30_state = States::eState_Into_Teleporter_1;
        field_50_objId = Teleporter::Create_obj_4DCEB0()->field_8_object_id;

        SFX_Play_46FBA0(49u, 60, -400, 0x10000);
        sControlledCharacter_5C1B8C->field_114_flags.Set(Flags_114::e114_Bit10);
        
        SpawnRingSparks();
    }
    break;

    case States::eState_Into_Teleporter_1:
    {
        if (pObj)
        {
            if (pObj->VSub_4E6630() || field_54_effect_created)
            {
                if (!(pObj->field_6_flags.Get(BaseGameObject::eDead)))
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
                    New_Particles_426C70(
                        sControlledCharacter_5C1B8C->field_B8_xpos,
                        sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(9), // 18/2
                        sControlledCharacter_5C1B8C->field_CC_sprite_scale,
                        3,
                        128u,
                        128u,
                        128u);

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
                    // Steam/smoke effect at Abe's body
                    New_Particles_426C70(
                        sControlledCharacter_5C1B8C->field_B8_xpos,
                        sControlledCharacter_5C1B8C->field_BC_ypos - FP_FromInteger(18),
                        sControlledCharacter_5C1B8C->field_CC_sprite_scale,
                        3,
                        128u,
                        128u,
                        128u);

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
                field_54_effect_created = 1;
            }

            if (!(pObj->field_6_flags.Get(BaseGameObject::eDead)))
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

        Path_Teleporter* pTeleporterTlv = static_cast<Path_Teleporter*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(Path_Teleporter::kType, 0));
        Path_Teleporter_Data tlvData = pTeleporterTlv->field_10_data;
        if (tlvData.field_10_id != field_34_mTlvData.field_12_target_id)
        {
            while (pTeleporterTlv)
            {
                pTeleporterTlv = static_cast<Path_Teleporter*>(sPath_dword_BB47C0->TLV_Next_Of_Type_4DB720(pTeleporterTlv, Path_Teleporter::kType));
                tlvData = pTeleporterTlv->field_10_data;

                if (tlvData.field_10_id == field_34_mTlvData.field_12_target_id)
                {
                    break;
                }
            }
        }
        
        SFX_Play_46FBA0(0x31u, 60, -300, tlvData.field_1C_scale != 0 ? 0x8000 : 0x10000);
        SpawnRingSparks();

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
            sControlledCharacter_5C1B8C->field_F8 = sControlledCharacter_5C1B8C->field_BC_ypos;
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
        New_Particle_426F40(
            FP_FromInteger((bRect.x + bRect.w) / 2),
            FP_FromInteger((bRect.y + bRect.h) / 2) + (sControlledCharacter_5C1B8C->field_CC_sprite_scale * FP_FromInteger(60)),
            sControlledCharacter_5C1B8C->field_CC_sprite_scale);

        // Spawn the falling "red" sparks from Abe's feet that appear after you've arrived at the destination
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

        field_54_effect_created = 0;
        sControlledCharacter_5C1B8C->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        sControlledCharacter_5C1B8C->field_114_flags.Clear(Flags_114::e114_Bit10);
        field_2C_switch_state = SwitchStates_Get_466020(field_34_mTlvData.field_1A_trigger_id);
        field_30_state = States::eState_WaitForSwitchOn_0;
    }
    break;
    }
}
