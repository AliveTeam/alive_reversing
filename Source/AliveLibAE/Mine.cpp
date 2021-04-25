#include "stdafx.h"
#include "Mine.hpp"
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
#include "stdlib.hpp"
#include "BaseBomb.hpp"

void Mine_ForceLink()
{

}

ALIVE_VAR(0, 0x5C3008, Mine*, sMineSFXOwner_5C3008, nullptr);


void Mine::VUpdate()
{
    Update_46B5D0();
}

void Mine::VRender(PrimHeader** ppOt)
{
    Render_46B7A0(ppOt);
}

BaseGameObject* Mine::VDestructor(s32 flags)
{
    return vdtor_46B4C0(flags);
}

void Mine::VScreenChanged()
{
    ScreenChanged_46BAE0();
}


void Mine::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_46B880();
}

void Mine::VOnThrowableHit(BaseGameObject* pFrom)
{
    vOnThrowableHit_46BA40(pFrom);
}

s16 Mine::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_46BB20(pFrom);
}

Mine * Mine::ctor_46B120(Path_Mine * pPath, TlvItemInfoUnion tlv)
{
    ctor_408240(0);

    SetVTable(this, 0x546164);
    SetVTable(&field_124_animation, 0x544290);

    field_4_typeId = AETypes::eMine_88;

    const AnimRecord& rec = AnimRec(AnimId::Mine);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_118_detonating = 0;
    field_6_flags.Set(Options::eInteractive_Bit8);
    field_6_flags.Set(Options::eCanExplode_Bit7);

    if (pPath->field_14_scale != Scale_short::eFull_0)
    {
        if (pPath->field_14_scale == Scale_short::eHalf_1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = Layer::eLayer_16;
            field_D6_scale = 0;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1);
        field_20_animation.field_C_render_layer = Layer::eLayer_35;
        field_D6_scale = 1;
    }

    const s32 v7 = pPath->field_8_top_left.field_0_x + pPath->field_C_bottom_right.field_0_x;
    field_B8_xpos = FP_FromInteger(v7 / 2);
    const FP v8 = FP_FromInteger(pPath->field_8_top_left.field_2_y);
    field_BC_ypos = v8;

    FP hitY;
    FP hitX;

    if (sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(v7 / 2),
        v8,
        FP_FromInteger(v7 / 2),
        v8 + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }
    field_11C_tlv = tlv;
    field_120_gnframe = sGnFrame_5C1B84;
    field_124_animation.Init_40A030(400, gObjList_animations_5C1A24, this, 36, 8u, Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMineflshResID), 1u, 0, 0);

    field_124_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_124_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);

    field_124_animation.field_C_render_layer = field_20_animation.field_C_render_layer;
    field_124_animation.field_14_scale = field_CC_sprite_scale;
    field_124_animation.field_8_r = 128;
    field_124_animation.field_9_g = 128;
    field_124_animation.field_A_b = 128;

    field_11A_disabled_resources = pPath->field_16_disabled_resources;

    field_1BC_flags.Clear(Mine_Flags_1BC::eBit1_PersistOffscreen);
    if (pPath->field_18_persist_offscreen == Choice_short::eYes_1)
    {
        field_1BC_flags.Set(Mine_Flags_1BC::eBit1_PersistOffscreen);
    }

    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbebombResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDebrisID00);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kBgexpldResID);

    if (!(field_11A_disabled_resources & 1))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID);
    }
    if (!(field_11A_disabled_resources & 2))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID );
    }

    const FP gridSnap = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    field_6_flags.Set(Options::eInteractive_Bit8);
    field_DC_bApplyShadows |= 2u;

    field_E4_collection_rect.x = field_B8_xpos - (gridSnap / FP_FromDouble(2.0));
    field_E4_collection_rect.y = field_BC_ypos - gridSnap;
    field_E4_collection_rect.w = (gridSnap / FP_FromDouble(2.0)) + field_B8_xpos;
    field_E4_collection_rect.h = field_BC_ypos;

    return this;
}

Mine* Mine::vdtor_46B4C0(s32 flags)
{
    dtor_46B4F0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Mine::dtor_46B4F0()
{
    SetVTable(this, 0x546164); // vTbl_Mine_546164
    if (field_118_detonating == 1)
    {
        Path::TLV_Reset_4DB8E0(field_11C_tlv.all, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_11C_tlv.all, -1, 0, 0);
    }

    field_124_animation.vCleanUp_40C630();
    field_6_flags.Clear(BaseGameObject::eInteractive_Bit8);

    if (sMineSFXOwner_5C3008 == this)
    {
        sMineSFXOwner_5C3008 = nullptr;
    }
    dtor_4080B0();
}

void Mine::Update_46B5D0()
{
    const s16 onScreen = gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0);

    if (field_118_detonating)
    {
        if (field_118_detonating == 1 && sGnFrame_5C1B84 >= field_120_gnframe)
        {
            auto explosion = ae_new<BaseBomb>();
            if (explosion)
            {
                explosion->ctor_423E70(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
            }
            field_6_flags.Set(Options::eDead_Bit3);
        }
    }
    else
    {
        if (field_20_animation.field_92_current_frame == 1
            && (!sMineSFXOwner_5C3008 || sMineSFXOwner_5C3008 == this))
        {
            if (onScreen)
            {
                SFX_Play_46FA90(SoundEffect::RedTick_3, 35);
                sMineSFXOwner_5C3008 = this;
            }
            else
            {
                sMineSFXOwner_5C3008 = nullptr;
            }
        }
        if (Mine::IsColliding_46B8C0())
        {
            field_118_detonating = 1;
            field_120_gnframe = sGnFrame_5C1B84;
        }
    }
    if (field_118_detonating != 1)
    {
        BaseGameObject * pEventObj = Event_Get_422C00(kEventDeathReset);
        if (pEventObj || field_C2_lvl_number != gMap_5C3030.field_0_current_level ||
            field_C0_path_number != gMap_5C3030.field_2_current_path)
        {
            field_6_flags.Set(Options::eDead_Bit3);
        }
    }
}

void Mine::Render_46B7A0(PrimHeader** ppOt)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
        {
            this->field_124_animation.vRender_40B820(FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                FP_GetExponent(FP_FromInteger(field_D8_yOffset) + field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                ppOt,
                0,
                0);

            Render_424B90(ppOt);
        }
    }
}

void Mine::ScreenChanged_46BAE0()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level
        || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path
        || !field_1BC_flags.Get(Mine_Flags_1BC::eBit1_PersistOffscreen))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }
}

void Mine::vOnPickUpOrSlapped_46B880()
{
    if (field_118_detonating != 1)
    {
        field_118_detonating = 1;
        field_120_gnframe = sGnFrame_5C1B84 + 5;
    }
}

void Mine::vOnThrowableHit_46BA40(BaseGameObject* /*pFrom*/)
{
    auto pBomb = ae_new<BaseBomb>();
    if (pBomb)
    {
        pBomb->ctor_423E70(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
    }

    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    field_118_detonating = 1;
}

s16 Mine::vTakeDamage_46BB20(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
    default: return 0;

    case AETypes::eGreeter_64:
    case AETypes::eAbe_69:
    case AETypes::eMineCar_89:
    case AETypes::eAbilityRing_104:
    case AETypes::eExplosion_109:
    case AETypes::eMudokon_110:
    case AETypes::eShrykull_121:
        auto pBomb = ae_new<BaseBomb>();
        if (pBomb)
        {
            pBomb->ctor_423E70(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
        }

        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_118_detonating = 1;
        field_120_gnframe = sGnFrame_5C1B84;
        return 1;
    }
}

bool Mine::IsColliding_46B8C0()
{
    PSX_RECT mineBound;
    vGetBoundingRect_424FD0(&mineBound, 1);

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject * pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        // e114_Bit6 May be "can set off explosives?"
        if (pObj->field_114_flags.Get(e114_Bit6_SetOffExplosives) && pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            PSX_RECT objBound;
            pObj->vGetBoundingRect_424FD0(&objBound, 1);

            s32 objX = FP_GetExponent(pObj->field_B8_xpos);
            s32 objY = FP_GetExponent(pObj->field_BC_ypos);

            if (objX > mineBound.x &&
                objX < mineBound.w &&
                objY < mineBound.h + 12 &&
                mineBound.x <= objBound.w &&
                mineBound.w >= objBound.x &&
                mineBound.h >= objBound.y &&
                mineBound.y <= objBound.h &&
                pObj->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                return 1;
            }
        }
    }

    return 0;
}

