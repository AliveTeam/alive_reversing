#include "stdafx.h"
#include "BirdPortal.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Midi.hpp"
#include "Function.hpp"

BaseGameObject* BirdPortal::ctor_497E00(Path_BirdPortal* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x546970);
    field_68 = 0;
    field_4_typeId = Types::eBirdPortal_99;
    field_C_objectId = tlvInfo;
    field_40_throwable_indicator_id = -1;

    // TODO: Ids
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 313);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 60);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 351);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 353);

    field_20_tlvInfo = tlvInfo;

    field_26_side = pTlv->field_10_side;
    field_7C_dest_level = pTlv->field_12_dest_level;
    field_7E_dest_path = pTlv->field_14_dest_path;
    field_80_dest_camera = pTlv->field_16_dest_camera;
    field_64_movie_id = pTlv->field_1A_moive_id;
    field_24_portal_type = pTlv->field_1C_portal_type;
    field_82_num_muds_for_shrykul = pTlv->field_1E_num_muds_for_shrykul;
    field_66_delete_id = pTlv->field_22_delete_id;
    
    if (pTlv->field_18_scale == 1)
    {
        field_60_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_60_scale = FP_FromInteger(1);
    }

    field_8E_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_8C_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_28_state = 0;
    field_5C_timer = 0;
    field_6C_terminator_id = -1;
    field_70_terminator_id = -1;
    field_74_screen_clipper_id = -1;
    field_78_screen_clipper_id = -1;
    field_88 = 0;

    FP hitX = {};
    PathLine* pLine = nullptr;
    sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(pTlv->field_8_top_left.field_0_x),
        FP_FromInteger(pTlv->field_8_top_left.field_2_y),
        FP_FromInteger(pTlv->field_C_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_C_bottom_right.field_2_y),
        &pLine,
        &hitX,
        &field_3C_YPos,
        field_60_scale > FP_FromDouble(0.5) ? 1 : 16);

    field_2C_xpos = FP_FromInteger((pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x ) / 2);
    field_90_sfx_ret = 0;
    field_30_ypos = field_3C_YPos - (FP_FromInteger(55) * field_60_scale);
    return this;
}

BaseGameObject* BirdPortal::VDestructor(signed int flags)
{
    return vdtor_498050(flags);
}

void BirdPortal::VUpdate()
{
    vUpdate_498280();
}

BirdPortal* BirdPortal::vdtor_498050(signed int flags)
{
    dtor_4980A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void BirdPortal::vUpdate_498280()
{
    NOT_IMPLEMENTED();
}

void BirdPortal::vScreenChanged_499B50()
{
    NOT_IMPLEMENTED();
}

void BirdPortal::vStopAudio_499260()
{
    if (field_90_sfx_ret)
    {
        SND_Stop_Channels_Mask_4CA810(field_90_sfx_ret);
        field_90_sfx_ret = 0;
    }
}

signed int BirdPortal::vGetSaveState_499F50(BYTE* /*pState*/)
{
    NOT_IMPLEMENTED();
    return 8;
}

void BirdPortal::VRender(int** /*pOrderingTable*/)
{
    // Null
}

void BirdPortal::VScreenChanged()
{
    vScreenChanged_499B50();
}

void BirdPortal::VStopAudio()
{
    vStopAudio_499260();
}

signed int BirdPortal::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_499F50(pSaveBuffer);
}

signed __int16 BirdPortal::Vsub_499430(__int16 bUnknown)
{
    return vsub_499430(bUnknown);
}

BaseGameObject* BirdPortal::Vsub_499610()
{
    return vsub_499610();
}

void BirdPortal::VMudSaved_499A50()
{
    vMudSaved_499A50();
}

BOOL BirdPortal::VStateIs6_499830()
{
    return vStateIs6_499830();
}

void BirdPortal::VGiveShrukul_499680(__int16 bPlaySound)
{
    return vGiveShrukul_499680(bPlaySound);
}

BOOL BirdPortal::VStateIs16_499850()
{
    return vStateIs16_499850();
}

__int16 BirdPortal::VExitPortal_499870()
{
    return vExitPortal_499870();
}

BOOL BirdPortal::VIsState20_499A00()
{
    return vIsState20_499A00();
}

int BirdPortal::Vsub_499A20()
{
    return vsub_499A20();
}

void BirdPortal::VGetMapChange_499AE0(WORD* level, WORD* path, WORD* camera, WORD* screenChangeEffect, WORD* movieId)
{
    vGetMapChange_499AE0(level, path, camera, screenChangeEffect, movieId);
}

signed __int16 BirdPortal::vsub_499430(__int16 /*bUnknown*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

BaseGameObject* BirdPortal::vsub_499610()
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void BirdPortal::vMudSaved_499A50()
{
    field_82_num_muds_for_shrykul--;
}

BOOL BirdPortal::vStateIs6_499830()
{
    return field_28_state == 6;
}

void BirdPortal::vGiveShrukul_499680(__int16 /*bPlaySound*/)
{
    NOT_IMPLEMENTED();
}

BOOL BirdPortal::vStateIs16_499850()
{
    return field_28_state == 16;
}

__int16 BirdPortal::vExitPortal_499870()
{
    NOT_IMPLEMENTED();
    return 0;
}

BOOL BirdPortal::vIsState20_499A00()
{
    return field_28_state == 20;
}

int BirdPortal::vsub_499A20()
{
    NOT_IMPLEMENTED();
    return 0;
}

void BirdPortal::vGetMapChange_499AE0(WORD* level, WORD* path, WORD* camera, WORD* screenChangeEffect, WORD* movieId)
{
    // TODO: Strongly type change effect and level

    *level = field_7C_dest_level;
    *path = field_7E_dest_path;
    *camera = field_80_dest_camera;

    if (field_64_movie_id <= 0)
    {
        *screenChangeEffect = 0;
    }
    else
    {
        *movieId = field_64_movie_id;
        *screenChangeEffect = 5;
    }
}

void BirdPortal::dtor_4980A0()
{
    SetVTable(this, 0x546970);
    BaseGameObject* pTerminator1 = sObjectIds_5C1B70.Find_449CF0(field_6C_terminator_id);
    if (pTerminator1)
    {
        pTerminator1->field_6_flags.Set(BaseGameObject::eDead);
    }

    BaseGameObject* pTerminator2 = sObjectIds_5C1B70.Find_449CF0(field_70_terminator_id);
    if (pTerminator2)
    {
        pTerminator2->field_6_flags.Set(BaseGameObject::eDead);;
    }

    BaseGameObject* pClipper1 = sObjectIds_5C1B70.Find_449CF0(field_74_screen_clipper_id);
    if (pClipper1)
    {
        pClipper1->field_6_flags.Set(BaseGameObject::eDead);
    }

    BaseGameObject* pClipper2 = sObjectIds_5C1B70.Find_449CF0(field_78_screen_clipper_id);
    if (pClipper2)
    {
        pClipper2->field_6_flags.Set(BaseGameObject::eDead);
    }

    BaseGameObject* pDoves = sObjectIds_5C1B70.Find_449CF0(field_44_dove_ids[0]);
    if (pDoves)
    {
        if (field_68)
        {
            for (auto doveId : field_44_dove_ids)
            {
                BaseGameObject* pDove = sObjectIds_5C1B70.Find_449CF0(doveId);
                if (pDove)
                {
                    pDove->field_6_flags.Set(BaseGameObject::eDead);
                }
            }
        }
    }

    BaseGameObject* pThrowableIndicator = sObjectIds_5C1B70.Find_449CF0(field_40_throwable_indicator_id);
    if (pThrowableIndicator)
    {
        pThrowableIndicator->field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_90_sfx_ret)
    {
        SND_Stop_Channels_Mask_4CA810(field_90_sfx_ret);
        field_90_sfx_ret = 0;
    }

    if (SwitchStates_Get_466020(field_66_delete_id))
    {
        Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 0);
    }

    BaseGameObject_dtor_4DBEC0();
}
