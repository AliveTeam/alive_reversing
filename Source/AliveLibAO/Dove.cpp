#include "stdafx_ao.h"
#include "Function.hpp"
#include "Dove.hpp"
#include "DynamicArray.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Midi.hpp"
#include "Game.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FF94C, short, bTheOneControllingTheMusic_4FF94C, 0);

ALIVE_VAR(1, 0x4FF938, DynamicArrayT<Dove>, gDovesArray_4FF938, {});
ALIVE_VAR(1, 0x4FF948, BYTE, byte_4FF948, 0);

static void Dove_static_ctor()
{
    gDovesArray_4FF938.ctor_4043E0(10);
}

void CC Dove_static_dtor_40EE30()
{
    if (!(byte_4FF948 & 1))
    {
        byte_4FF948 |= 1u;
        gDovesArray_4FF938.dtor_404440();
    }
}

void CC Dove_static_ctor_40EE10()
{
    Dove_static_ctor(); // inlined
    atexit(Dove_static_dtor_40EE30);
}

Dove* Dove::ctor_40EE50(int frameTableOffset, int maxW, int maxH, int resourceID, int tlvInfo, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BA858);
    field_4_typeId = Types::eBird_22;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, resourceID, 1, 0);
    Animation_Init_417FD0(
        frameTableOffset,
        maxW,
        maxH,
        ppRes,
        1);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    
    gDovesArray_4FF938.Push_Back(this);

    field_10_anim.field_14_scale = scale;
    field_BC_sprite_scale = scale;
    if (scale == FP_FromInteger(1))
    {
        field_C6_scale = 1;
        field_10_anim.field_C_layer = 27;
    }
    else
    {
        field_C6_scale = 0;
        field_10_anim.field_C_layer = 8;
    }

    field_B4_velx = FP_FromInteger((Math_NextRandom() / 12 - 11));
    if (field_B4_velx >= FP_FromInteger(0))
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_EE_state = State::State_0_OnGround;

    field_B8_vely = FP_FromInteger(-4 - (Math_NextRandom() & 3));
    field_10_anim.SetFrame_402AC0(Math_NextRandom() & 7);
    field_EC_keepInGlobalArray = FALSE;
    field_E8_tlvInfo = tlvInfo;

    if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
    {
        field_C4_b = 30;
        field_C2_g = 30;
        field_C0_r = 30;
    }

    if (bTheOneControllingTheMusic_4FF94C)
    {
        return this;
    }

    SND_SEQ_PlaySeq_4775A0(24u, 0, 1);
    bTheOneControllingTheMusic_4FF94C = 1;
    return this;
}

Dove* Dove::ctor_40EFF0(int frameTableOffset, int maxW, int maxH, int resourceID, FP xpos, FP ypos, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BA858);

    field_4_typeId = Types::eBird_22;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, resourceID, 1, 0);
    Animation_Init_417FD0(
        frameTableOffset,
        maxW,
        maxH,
        ppRes,
        1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_14_scale = scale;
    field_BC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 27;
    }
    else
    {
        field_10_anim.field_C_layer = 8;
    }

    field_B4_velx = FP_FromInteger(Math_NextRandom() / 12 - 11);
    if (scale >= FP_FromInteger(0))
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_B8_vely = FP_FromInteger(-4 - ((Math_NextRandom()) & 3));
    field_EE_state = Dove::State::State_1_FlyAway;
    field_EC_keepInGlobalArray = TRUE;
    field_E4_counter = 0;

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    field_100_prevX = xpos;
    field_104_prevY = ypos;

    field_E8_tlvInfo = 0;

    field_10_anim.SetFrame_402AC0((Math_NextRandom() & 6) + 1);

    if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
    {
        field_C4_b = 30;
        field_C2_g = 30;
        field_C0_r = 30;
    }

    if (bTheOneControllingTheMusic_4FF94C)
    {
        return this;
    }
    SND_SEQ_PlaySeq_4775A0(24u, 0, 1);
    bTheOneControllingTheMusic_4FF94C = 1;
    return this;
}

BaseGameObject* Dove::dtor_40F1B0()
{
    SetVTable(this, 0x4BA858);

    if (!field_EC_keepInGlobalArray)
    {
        gDovesArray_4FF938.Remove_Item(this);
        if (field_E8_tlvInfo)
        {
            gMap_507BA8.TLV_Reset_446870(field_E8_tlvInfo, -1, 0, 0);
        }
    }

    if (bTheOneControllingTheMusic_4FF94C)
    {
        SND_Seq_Stop_477A60(24u);
        bTheOneControllingTheMusic_4FF94C = 0;
    }

    return dtor_417D10();
}

void Dove::AsAlmostACircle_40F300(FP xpos, FP ypos, BYTE angle)
{
    AsACircle_40F280(xpos, ypos, angle);
    field_EE_state = State::State_4_AlmostACircle;
}

void Dove::AsACircle_40F280(FP xpos, FP ypos, BYTE angle)
{
    field_F0_xJoin = xpos;
    field_F4_yJoin = ypos;
    field_FC_angle = angle;
    field_EE_state = State::State_3_Circle;

    // TODO: Removed unused code
}

void Dove::AsJoin_40F250(FP xpos, FP ypos)
{
    field_F0_xJoin = xpos;
    field_F4_yJoin = ypos;
    field_EE_state = State::State_2_Join;
    field_F8_timer = gnFrameCount_507670 + 47;
}

void Dove::FlyAway_40F8F0(__int16 a2)
{
    if (field_EE_state != State::State_1_FlyAway)
    {
        field_EE_state = State::State_1_FlyAway;
        if (a2)
        {
            field_E4_counter = -1;
        }
        else
        {
            field_E4_counter = -10 - Math_NextRandom() % 10;
        }
    }
}

ALIVE_VAR(1, 0x4FF944, int, bExtraSeqStarted_4FF944, 0);

void Dove::All_FlyAway_40F390()
{
    for (int i = 0; i < gDovesArray_4FF938.Size(); i++)
    {
        Dove* pDove = gDovesArray_4FF938.ItemAt(i);
        if (!pDove)
        {
            break;
        }
        pDove->FlyAway_40F8F0(0);
    }

    bExtraSeqStarted_4FF944 = 0;
    if (bTheOneControllingTheMusic_4FF94C)
    {
        SND_Seq_Stop_477A60(24u);
        bTheOneControllingTheMusic_4FF94C = FALSE;
    }
}

void Dove::VRender(int** ppOt)
{
    VRender_40F960(ppOt);
}

void Dove::VRender_40F960(int** ppOt)
{
    BaseAnimatedWithPhysicsGameObject::VRender_417DA0(ppOt);
}

BaseGameObject* Dove::VDestructor(signed int flags)
{
    return Vdtor_40F970(flags);
}

Dove* Dove::Vdtor_40F970(unsigned int flags)
{
    dtor_40F1B0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
