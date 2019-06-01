#include "stdafx.h"
#include "Paramite.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "ObjectIds.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"

TintEntry stru_55D73C[24] =
{
    { 3u, 105u, 105u, 105u },
    { 11u, 105u, 105u, 105u },
    { -1, 105u, 105u, 105u }
};

Paramite* Paramite::ctor_4879B0(Path_Paramite* pTlv, int tlvInfo)
{
    ctor_408240(16);
    field_160 = -1;
    field_174 = 0;
    field_176 = -1;

    SetVTable(this, 0x54640C);
    field_4_typeId = Types::eParamite_96;

    field_C_objectId = tlvInfo;

    field_10_resources_array.SetAt(0,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 600, 1, 0));
    field_10_resources_array.SetAt(5,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 605, 1, 0));
    field_10_resources_array.SetAt(14, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 614, 1, 0));
    field_10_resources_array.SetAt(4,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 604, 1, 0));
    field_10_resources_array.SetAt(1,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 601, 1, 0));
    field_10_resources_array.SetAt(15, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 615, 1, 0));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 610, 1, 0));
    field_10_resources_array.SetAt(9,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 609, 1, 0));
    
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 2034);
    Animation_Init_424E10(
        96696,
        137,
        65u,
        field_10_resources_array.ItemAt(0),
        1,
        1);

    SetTint_425600(&stru_55D73C[0], gMap_5C3030.sCurrentLevelId_5C3030);

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_110_id = -1;
    field_158 = -1;
    field_15A = -1;
    field_11C_obj_id = -1;
    field_118 = -1;
    field_120_obj_id = -1;
    field_124 = -1;
    field_130_timer = 0;
    field_12C_brain_ret = 0;
    field_108_next_motion = 0;
    field_106_current_motion = 0;
    field_154 = 0;
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    if (!vIsFacingMe_4254A0(sActiveHero_5C1B68))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }

    switch (pTlv->field_12_entrace_type)
    {
    case 1:
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        SetBrain(&Paramite::AI_3_4851B0);
        break;

    case 2:
        SetBrain(&Paramite::AI_9_48ED80);
        field_BC_ypos -= FP_FromInteger(20);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    case 3:
        SetBrain(&Paramite::AI_9_48ED80);
        field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    case 4:
        SetBrain(&Paramite::AI_9_48ED80);
        field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        break;

    default:
        SetBrain(&Paramite::AI_0_4835B0);
        break;
    }

    field_136_attack_delay = pTlv->field_14_attack_delay;
    field_12E_drop_delay = pTlv->field_16_drop_delay;
    field_134_meat_eating_time = pTlv->field_18_meat_eating_time;
    field_144_attack_duration = pTlv->field_1A_attack_duration;
    field_14C_id = pTlv->field_1E_id;

    field_178_flags.Set(Flags_178::eBit1, pTlv->field_20_hiss_before_attack & 1);
    field_178_flags.Clear(Flags_178::eBit2);
    field_178_flags.Set(Flags_178::eBit4, pTlv->field_22_delete_when_far_away & 1);
    field_178_flags.Clear(Flags_178::eBit5);
    field_178_flags.Set(Flags_178::eBit8, pTlv->field_24_deadly_scratch & 1);
    field_178_flags.Clear(Flags_178::eBit6);
    field_178_flags.Clear(Flags_178::eBit7);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_F8_LastLineYPos = hitY;
        field_BC_ypos = hitY;
        field_106_current_motion = 0;
        sub_489EA0();
    }

    field_F8_LastLineYPos = field_BC_ypos;
    field_140_tlvInfo = tlvInfo;
    MapFollowMe_408D10(TRUE);

    vStackOnObjectsOfType_425840(Types::eParamite_96);

    field_DC_bApplyShadows |= 2u;
    field_15C = field_DA_xOffset;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

BaseGameObject* Paramite::VDestructor(signed int flags)
{
    return vdtor_487F90(flags);
}

__int16 Paramite::AI_0_4835B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_1_484CD0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_2_4859D0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_3_4851B0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_4_48F8F0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_5_486880()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_6_484BC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_7_484FF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_8_48DFC0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Paramite::AI_9_48ED80()
{
    NOT_IMPLEMENTED(); 
    return 0;
}

const TParamiteAIFn sParamite_ai_table_55D710[10] =
{
    &Paramite::AI_0_4835B0,
    &Paramite::AI_1_484CD0,
    &Paramite::AI_2_4859D0,
    &Paramite::AI_3_4851B0,
    &Paramite::AI_4_48F8F0,
    &Paramite::AI_5_486880,
    &Paramite::AI_6_484BC0,
    &Paramite::AI_7_484FF0,
    &Paramite::AI_8_48DFC0,
    &Paramite::AI_9_48ED80
};


struct FnPair
{
    TParamiteAIFn mReimpl;
    DWORD mOriginal;
};

static FnPair sAiFns[10] = 
{
    { &Paramite::AI_0_4835B0, 0x402A7C },
    { &Paramite::AI_1_484CD0, 0x404223 },
    { &Paramite::AI_2_4859D0, 0x401799 },
    { &Paramite::AI_3_4851B0, 0x401645 },
    { &Paramite::AI_4_48F8F0, 0x48F8F0 }, // No stub ??
    { &Paramite::AI_5_486880, 0x4012E4 },
    { &Paramite::AI_6_484BC0, 0x40187F },
    { &Paramite::AI_7_484FF0, 0x4021A3 },
    { &Paramite::AI_8_48DFC0, 0x4010B4 },
    { &Paramite::AI_9_48ED80, 0x401EE7 },
};

#if _WIN32 || !_WIN64
static DWORD GetOriginalFn(TParamiteAIFn fn)
{
    // If not running as standalone set the address to be
    // the address of the real function rather than the reimpl as the real
    // game code compares the function pointer addresses (see IsBrain(x)).
    for (const auto& addrPair : sAiFns)
    {
        if (addrPair.mReimpl == fn)
        {
            const DWORD actualAddressToUse = addrPair.mOriginal;
            // Hack to overwrite the member function pointer bytes with arbitrary data
            return actualAddressToUse;
        }
    }
    ALIVE_FATAL("No matching address!");
}
#endif

void Paramite::SetBrain(TParamiteAIFn fn)
{
#if _WIN32 || !_WIN64
    if (IsAlive())
    {
        const DWORD actualAddressToUse = GetOriginalFn(fn);
        // Hack to overwrite the member function pointer bytes with arbitrary data
        memcpy(&field_128_fn_brainState, &actualAddressToUse, sizeof(DWORD));
        return;
    }
#endif
    field_128_fn_brainState = fn;
}

void Paramite::dtor_487FC0()
{
    SetVTable(this, 0x54640C);

    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_11C_obj_id);
    if (pObj)
    {
        pObj->field_6_flags.Set(BaseGameObject::eDead);
        field_11C_obj_id = -1;
    }

    field_118 = -1;
    VOnTrapDoorOpen();

    if (field_10C_health > FP_FromInteger(0) || field_178_flags.Get(Flags_178::eBit6))
    {
        Path::TLV_Reset_4DB8E0(field_140_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_140_tlvInfo, -1, 0, 1);
    }

    SND_SEQ_Stop_4CAE60(25u);
    MusicController::sub_47FD60(0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_14E_return_level,
                field_150_return_path,
                field_152_return_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }
    dtor_4080B0();
}

Paramite* Paramite::vdtor_487F90(signed int flags)
{
    dtor_487FC0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Paramite::sub_489EA0()
{
    NOT_IMPLEMENTED();
}
