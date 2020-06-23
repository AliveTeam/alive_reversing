#include "stdafx_ao.h"
#include "Function.hpp"
#include "Slig.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "Math.hpp"

START_NS_AO

ALIVE_VAR(1, 0x9F11BC, int, dword_9F11BC, 0);
ALIVE_VAR(1, 0x9F11C0, int, dword_9F11C0, 0);

TintEntry stru_4CFB10[3] =
{
  { 5, 127u, 127u, 127u },
  { 6, 127u, 127u, 127u },
  { -1, 102u, 127u, 118u }
};

short* CC Animation_OnFrame_Slig_46F610(void*, __int16* )
{
    NOT_IMPLEMENTED();
    return 0;
}

// TODO: Needs to be part of 1 array like in AE
TFrameCallBackType sSligFrameCallBacks_4CEBF0[] =
{ 
    Animation_OnFrame_Slig_46F610
};


Slig* Slig::ctor_464D40(Path_Slig* pTlv, int tlvInfo)
{
    ctor_401090(); // Note: Empty base skipped

    field_15C = -1;
    field_172 = -1;
    field_170 = 0;

    SetVTable(this, 0x4BCA70);

    memset(field_210_resources, 0, sizeof(field_210_resources));

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 412, 1, 0);
    field_210_resources[0] = ppRes;
    Animation_Init_417FD0(
        132740,
        160,
        68,
        ppRes,
        1);

    field_10A_flags.Clear(Flags_10A::e10A_Bit2);
    field_10A_flags.Set(Flags_10A::e10A_Bit1);
    field_10A_flags.Set(Flags_10A::e10A_Bit4);

    field_4_typeId = Types::eSlig_88;

    field_114 = 0;
    field_118 = 0;
    field_11C = -1;
    field_10C = 0;
    field_10E = 0;
    field_FE_next_state = 0;
    field_EC_oldY = 3;
    field_158 = 0;
    field_154 = 0;
    field_F8_pLiftPoint = nullptr;
    field_FC_state = 7;
    field_11E = 0;
    field_144 = 0;
    field_12C = 0;

    field_110 = 45 * ((Math_NextRandom() % 5) - 2);

    field_F0_pTlv = pTlv;
    field_174_tlv = *pTlv;
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
    field_134_tlvInfo = tlvInfo;
    field_254 &= ~7u;
    field_126 = 0;
    field_130 = 100;
    dword_9F11BC = 0;
    dword_9F11C0 = 0;
    field_200 = 0;
    field_204 = 0;
    field_208 = 0;
    field_20C = 0;
    field_13A = -1;
    field_138 = 0;

    field_10_anim.field_1C_fn_ptrs = sSligFrameCallBacks_4CEBF0;

    if (pTlv->field_18_scale == 0)
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = 33;
        field_C6_scale = 1;
    }
    else
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 14;
        field_C6_scale = 0;
    }

    SetBaseAnimPaletteTint_4187C0(
        &stru_4CFB10[0],
        gMap_507BA8.field_0_current_level,
        412);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos,
        field_A8_xpos,
        field_AC_ypos + FP_FromInteger(24),
        &field_F4_pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10) == 1)
    {
        field_FC_state = 0;
        field_AC_ypos = hitY;
    }

    MapFollowMe_401D30(TRUE);

    Init_46B890();
    
    VStackOnObjectsOfType_418930(static_cast<short>(Types::eSlig_88));

    field_10A_flags.Set(Flags_10A::e10A_Bit6);

    field_146 = 0;
    field_14A = 0;
    field_148 = 0;
    field_122 = 0;

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseAliveGameObject* Slig::dtor_465320()
{
    NOT_IMPLEMENTED();

    SetVTable(this, 0x4BCA70);

    /*
    if (sControlledCharacter_50767C == this)
    {
        if (field_14E_level != gMap_507BA8.field_0_current_level
            || field_150_path != gMap_507BA8.field_2_current_path
            || field_152_camera != gMap_507BA8.field_4_current_camera)
        {
            Event_Broadcast_417220(7, this);
        }

        sControlledCharacter_50767C = sActiveHero_507678;
        MusicController::sub_443810(0, this, 0, 0);

        if (gMap_507BA8.field_A_level != LevelIds::eMenu_0 && gMap_507BA8.field_A_level != LevelIds::eNone)
            gMap_507BA8.SetActiveCam_444660(
                field_14E_level,
                field_150_path,
                field_152_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
    }
    */

    auto pTlv = gMap_507BA8.TLV_Get_At_446260(
        field_174_tlv.field_C_sound_pos.field_0_x,
        field_174_tlv.field_C_sound_pos.field_2_y,
        field_174_tlv.field_C_sound_pos.field_0_x,
        field_174_tlv.field_C_sound_pos.field_2_y,
        static_cast<short>(field_174_tlv.field_4_type));

    if (field_100_health <= FP_FromInteger(0))
    {
        if (pTlv)
        {
            pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
            pTlv->field_0_flags.Set(TLV_Flags::eBit2_Unknown);
        }
    }
    else if (pTlv)
    {
        pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
        pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Unknown);
    }

    /*
    ppResourcesIter = field_210_resources;
    k17Counter = 17;
    do
    {
        if (field_10_anim.field_20_ppBlock != *ppResourcesIter)
        {
            if (*ppResourcesIter)
            {
                ResourceManager::FreeResource_455550(*ppResourcesIter);
                *ppResourcesIter = 0;
            }
        }
        ++ppResourcesIter;
        --k17Counter;
    } while (k17Counter);
    */

    return dtor_401000(); // Note: Empty dtor skipped
}

BaseGameObject* Slig::VDestructor(signed int flags)
{
    return Vdtor_465DC0(flags);
}

Slig* Slig::Vdtor_465DC0(signed int flags)
{
    dtor_465320();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Slig::Init_46B890()
{
    NOT_IMPLEMENTED();
}

END_NS_AO

