#include "stdafx_ao.h"
#include "Function.hpp"
#include "BirdPortal.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Abe.hpp"
#include "ScreenClipper.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Flash.hpp"
#include "Midi.hpp"

START_NS_AO

void BirdPortalTerminator::VScreenChanged()
{
    // Staying alive
}

BirdPortalTerminator* BirdPortalTerminator::ctor_451F70(FP xpos, FP ypos, FP scale, int /*portalType*/)
{
    ctor_417C10();
    SetVTable(this, 0x4BBFB0);

    field_4_typeId = Types::eClaw_48;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 313, 1, 0);
    Animation_Init_417FD0(3860, 31, 18, ppRes, 1);
    field_10_anim.field_B_render_mode = 1;
    field_BC_sprite_scale = scale;
    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 39;
    }
    else
    {
        field_10_anim.field_C_layer = 20;
    }

    field_CC_bApplyShadows &= ~1u;

    field_AC_ypos = ypos;
    field_A8_xpos = xpos;

    field_C0_r = 255;
    field_C2_g = 128;
    field_C4_b = 64;

    return this;
}

BaseGameObject* BirdPortalTerminator::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

// ==========================================================================

BaseGameObject* BirdPortal::VDestructor(signed int flags)
{
    return Vdtor_453990(flags);
}

BaseGameObject* BirdPortal::dtor_452230()
{
    SetVTable(this, 0x4BBFE8);

    if (field_3C_pTerminator1)
    {
        field_3C_pTerminator1->field_6_flags.Set(Options::eDead_Bit3);
    }
    if (field_40_pTerminator)
    {
        field_40_pTerminator->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_44_pScreenClipper)
    {
        field_44_pScreenClipper->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_48_pScreenClipper)
    {
        field_48_pScreenClipper->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_4C_pDovesArray)
    {
        for (int i = 0; i < field_4C_pDovesArray->Size(); i++)
        {
            BaseAliveGameObject* pObj = field_4C_pDovesArray->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            pObj->field_C_refCount--;
            pObj->field_6_flags.Set(Options::eDead_Bit3);
        }

        field_4C_pDovesArray->field_4_used_size = 0;

        if (field_4C_pDovesArray)
        {
            field_4C_pDovesArray->dtor_404440();
            ao_delete_free_447540(field_4C_pDovesArray);
        }
    }

    if (field_5C)
    {
        field_5C->field_6_flags.Set(Options::eDead_Bit3);
        field_5C->field_C_refCount--;
        field_5C = nullptr;
    }

    BYTE** ppRes = nullptr;
    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 313, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 351, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 353, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 60, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    if (field_68_sfx_ret)
    {
        SND_Stop_Channels_Mask_4774A0(field_68_sfx_ret);
        field_68_sfx_ret = 0;
    }

    gMap_507BA8.TLV_Reset_446870(field_2C_tlvInfo, -1, 0, 0);
    if (field_14_state >= 17)
    {
        if (sActiveHero_507678)
        {
            if (field_34_scale == FP_FromInteger(1))
            {
                sActiveHero_507678->field_10_anim.field_C_layer = 32;
            }
            else
            {
                sActiveHero_507678->field_10_anim.field_C_layer = 13;
            }
        }
    }
    return dtor_487DF0();
}

BirdPortal* BirdPortal::ctor_4520A0(Path_BirdPortal* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBFE8);

    field_4_typeId = Types::eBirdPortal_65;

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 313, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 60, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 351, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 353, 1, 0);

    field_2C_tlvInfo = tlvInfo;


    field_50_dest_level = pTlv->field_1A_dest_level;
    field_12_side = pTlv->field_18_side;
    field_54_dest_camera = pTlv->field_1E_dest_camera;
    field_52_dest_path = pTlv->field_1C_dest_path;
    field_10_portal_type = pTlv->field_24_portal_type;
    field_38_movie_id = pTlv->field_22_movie_id;
    field_56_num_muds_for_shrykul = pTlv->field_26_num_muds_for_shrykul;
    if (pTlv->field_20_scale == 1)
    {
        field_34_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_34_scale = FP_FromInteger(1);
    }

    field_66_path = gMap_507BA8.field_2_current_path;
    field_64_level = gMap_507BA8.field_0_current_level;
    field_14_state = 0;
    field_30_timer = 0;

    field_3C_pTerminator1 = nullptr;
    field_40_pTerminator = nullptr;
    field_44_pScreenClipper = nullptr;
    field_48_pScreenClipper = nullptr;
    field_4C_pDovesArray = nullptr;
    field_60_pOrbWhirlWind = nullptr;
    field_5C = 0;

    PathLine* pLine = nullptr;
    FP hitX = {};
    sCollisions_DArray_504C6C->RayCast_40C410(
        FP_FromInteger(pTlv->field_10_top_left.field_0_x),
        FP_FromInteger(pTlv->field_10_top_left.field_2_y),
        FP_FromInteger(pTlv->field_14_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
        &pLine,
        &hitX,
        &field_28_ypos,
        0xFFFFFFFF);  // -1 ??

    field_18_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_68_sfx_ret = 0;
    field_1C_ypos = field_28_ypos - (FP_FromInteger(55) * field_34_scale);
    return this;
}

BirdPortal* BirdPortal::Vdtor_453990(signed int flags)
{
    dtor_452230();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
