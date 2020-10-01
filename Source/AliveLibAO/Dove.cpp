#include "stdafx_ao.h"
#include "Function.hpp"
#include "Dove.hpp"
#include "DynamicArray.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Midi.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "ScreenManager.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FF94C, short, bTheOneControllingTheMusic_4FF94C, 0);

ALIVE_VAR(1, 0x4FF938, DynamicArrayT<Dove>, gDovesArray_4FF938, {});
ALIVE_VAR(1, 0x4FF948, BYTE, byte_4FF948, 0);
ALIVE_VAR(1, 0x4FF950, int, dword_4FF950, 0);
ALIVE_VAR(1, 0x4C50AC, short, word_4C50AC, 30);
ALIVE_VAR(1, 0x4C50B0, short, word_4C50B0, -1);


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

void Dove::VUpdate()
{
    VUpdate_40F430();
}

void Dove::VUpdate_40F430()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!bTheOneControllingTheMusic_4FF94C)
    {
        SND_SEQ_PlaySeq_4775A0(24u, 0, 1);
        bTheOneControllingTheMusic_4FF94C = 1;
    }

    switch (field_EE_state)
    {
    case State::State_0_OnGround:
        if (Event_Get_417250(kEventSpeaking_1))
        {
            for (int i = 0; i < gDovesArray_4FF938.Size(); i++)
            {
                Dove* pDoveIter = gDovesArray_4FF938.ItemAt(i);
                if (!pDoveIter)
                {
                    break;
                }
                pDoveIter->FlyAway_40F8F0(0); // something is speaking, leg it
            }

            bExtraSeqStarted_4FF944 = 0;
            if (bTheOneControllingTheMusic_4FF94C)
            {
                SND_Seq_Stop_477A60(24u);
                bTheOneControllingTheMusic_4FF94C = 0;
            }
        }

        if (FP_GetExponent(FP_Abs(field_A8_xpos - sControlledCharacter_50767C->field_A8_xpos)) < 100)
        {
            if (Event_Get_417250(kEventNoise_0))
            {
                for (int i = 0; i < gDovesArray_4FF938.Size(); i++)
                {
                    Dove* pDoveIter = gDovesArray_4FF938.ItemAt(i);
                    if (!pDoveIter)
                    {
                        break;
                    }
                    pDoveIter->FlyAway_40F8F0(0);
                }

                bExtraSeqStarted_4FF944 = 0;
                if (bTheOneControllingTheMusic_4FF94C)
                {
                    SND_Seq_Stop_477A60(24u);
                    bTheOneControllingTheMusic_4FF94C = 0;
                }
            }
        }
        break;

    case State::State_1_FlyAway:
        field_E4_counter++;
        if (field_E4_counter == 0)
        {
            field_10_anim.Set_Animation_Data_402A40(4988, nullptr);
            if (!bExtraSeqStarted_4FF944)
            {
                bExtraSeqStarted_4FF944 = 16;
                SFX_Play_43AD70(SoundEffect::Dove_16, 0, 0);
            }
        }

        if (field_E4_counter > 0)
        {
            field_A8_xpos += field_B4_velx;
            field_AC_ypos += field_B8_vely;
        }

        field_B8_vely = (field_B8_vely * FP_FromDouble(1.03));
        field_B4_velx = (field_B4_velx * FP_FromDouble(1.03));
 
        if (field_E4_counter >= 25 - (Math_NextRandom() & 7))
        {
            field_E4_counter = (Math_NextRandom() & 7) + field_E4_counter - 25;
            field_B4_velx = -field_B4_velx;
        }

        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_B4_velx >= FP_FromInteger(0));
        break;

    case State::State_2_Join:
    {
        if (static_cast<int>(gnFrameCount_507670) > field_F8_timer)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }

        const FP k4Directed = field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(4) : FP_FromInteger(-4);
        field_B4_velx = (k4Directed + field_F0_xJoin - field_A8_xpos) / FP_FromInteger(8);
        field_A8_xpos += field_B4_velx;
        field_B8_vely = (field_F4_yJoin - field_AC_ypos) / FP_FromInteger(8);
        field_AC_ypos += field_B8_vely;
    }
        return;

    case State::State_3_Circle:
        field_100_prevX = field_A8_xpos;
        field_104_prevY = field_AC_ypos;

        field_FC_angle += 4;

        // Spin around this point
        field_A8_xpos = ((Math_Sine_451110(field_FC_angle) * FP_FromInteger(30)) * field_BC_sprite_scale) + field_F0_xJoin;
        field_AC_ypos = ((Math_Cosine_4510A0(field_FC_angle) * FP_FromInteger(35)) * field_BC_sprite_scale) + field_F4_yJoin;
        return;

    case State::State_4_AlmostACircle:
        if (dword_4FF950 != static_cast<int>(gnFrameCount_507670))
        {
            dword_4FF950 = gnFrameCount_507670;
            word_4C50AC += word_4C50B0;

            if (word_4C50AC == 0)
            {
                word_4C50B0 = 1;
            }
            else if (word_4C50AC == 30)
            {
                word_4C50B0 = -1;
            }
        }
        field_100_prevX = field_A8_xpos;
        field_FC_angle += 4;
        field_104_prevY = field_AC_ypos;
        field_A8_xpos = ((Math_Sine_451110(field_FC_angle) * FP_FromInteger(word_4C50AC)) * field_BC_sprite_scale) + field_F0_xJoin;
        field_AC_ypos = ((Math_Cosine_4510A0(field_FC_angle) * FP_FromInteger(35)) * field_BC_sprite_scale) + field_F4_yJoin;
        return;

    default:
        break;
    }

    const int v11 = FP_GetExponent(FP_Abs(field_AC_ypos - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y));
    if (v11 > pScreenManager_4FF7C8->field_16_ypos)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const int v12 = FP_GetExponent(FP_Abs(field_A8_xpos - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x));
    if (v12 > pScreenManager_4FF7C8->field_14_xpos)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

END_NS_AO
