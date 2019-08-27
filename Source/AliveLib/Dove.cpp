#include "stdafx.h"
#include "Dove.hpp"
#include "Function.hpp"
#include "Midi.hpp"
#include "Path.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5bc112, short, bTheOneControllingTheMusic_5BC112, 0);

ALIVE_VAR(1, 0x5BC100, DynamicArrayT<Dove>, gDovesArray_5BC100, {});
ALIVE_VAR(1, 0x5BC110, BYTE, byte_5BC110, 0);

EXPORT void CC Static_ctor_dynamic_array_41F3C0()
{
    gDovesArray_5BC100.ctor_40CA60(3);
}

EXPORT void CC Static_dtor_dynamic_array_41F400()
{
    // Flag to guard against multiple destruction - although seems impossible given its in the table once
    if (!(byte_5BC110 & 1))
    {
        byte_5BC110 |= 1u;
        gDovesArray_5BC100.dtor_40CAD0();
    }
}

EXPORT void CC Static_init_dynamic_array_41F3A0()
{
    Static_ctor_dynamic_array_41F3C0();
    atexit(Static_dtor_dynamic_array_41F400);
}

Dove* Dove::ctor_41F430(int frameTableOffset, int maxW, unsigned __int16 maxH, int resourceID, int tlvInfo, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x544A90);

    field_4_typeId = Types::eBird_35;
    
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, resourceID);

    Animation_Init_424E10(
        frameTableOffset,
        maxW,
        maxH,
        ppRes,
        1,
        1);
    
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    gDovesArray_5BC100.Push_Back_40CAF0(this);

    field_20_animation.field_14_scale = scale;
    field_CC_sprite_scale = scale;
    if (scale == FP_FromInteger(1))
    {
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 27;
    }
    else
    {
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 8;
    }

    field_C4_velx = FP_FromInteger(Math_NextRandom() / 12 - 11);
    if (field_C4_velx >= FP_FromInteger(0))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_C8_vely = FP_FromInteger(-4 - (Math_NextRandom() & 3));
    field_FE_state = State::State_0_OnGround;
    field_20_animation.SetFrame_409D50(Math_NextRandom() & 7);
    field_FC_keepInGlobalArray = FALSE;
    field_F8_tlvInfo = tlvInfo;

    if (bTheOneControllingTheMusic_5BC112)
    {
        return this;
    }

    SND_SEQ_PlaySeq_4CA960(SeqId::NecrumAmbient2_17, 0, 1);
    bTheOneControllingTheMusic_5BC112 = 1;
    return this;
}

Dove* Dove::ctor_41F660(int frameTableOffset, int maxW, __int16 maxH, int resourceID, FP xpos, FP ypos, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x544A90); // vTbl_Dove_544A90

    field_4_typeId = Types::eBird_35;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, resourceID);
    Animation_Init_424E10(
        frameTableOffset,
        maxW,
        maxH,
        ppRes,
        1,
        1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_14_scale = scale;
    field_CC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 27;
    }
    else
    {
        field_20_animation.field_C_render_layer = 8;
    }

    field_C4_velx = FP_FromInteger(Math_NextRandom() / 12 - 11);
    if (field_C4_velx >= FP_FromInteger(0))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_C8_vely = FP_FromInteger(-4 - (Math_NextRandom() & 3));
    field_FE_state = State::State_1_FlyAway;
    field_FC_keepInGlobalArray = 1;
    field_F4_counter = 0;

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;
    field_110_prevX = xpos;
    field_114_prevY = ypos;

    field_F8_tlvInfo = 0;

    field_20_animation.SetFrame_409D50(Math_NextRandom() & 6);

    if (bTheOneControllingTheMusic_5BC112)
    {
        return this;
    }
    SND_SEQ_PlaySeq_4CA960(SeqId::NecrumAmbient2_17, 0, 1);
    bTheOneControllingTheMusic_5BC112 = 1;

    return this;
}

BaseGameObject* Dove::VDestructor(signed int flags)
{
    return vdtor_41F630(flags);
}

void Dove::VUpdate()
{
    vUpdate_41FAE0();
}

void Dove::VRender(int** pOrderingTable)
{
    vRender_4200B0(pOrderingTable);
}

void Dove::vRender_4200B0(int** ot)
{
    // Kind of pointless, the override just calls base
    Render_424B90(ot);
}

void Dove::dtor_41F870()
{
    SetVTable(this, 0x544A90); // vTbl_Dove_544A90

    if (!field_FC_keepInGlobalArray)
    {
        gDovesArray_5BC100.Remove_Item(this);
        if (field_F8_tlvInfo)
        {
            Path::TLV_Reset_4DB8E0(field_F8_tlvInfo, -1, 0, 0);
        }
    }

    if (bTheOneControllingTheMusic_5BC112)
    {
        SND_SEQ_Stop_4CAE60(SeqId::NecrumAmbient2_17);
        bTheOneControllingTheMusic_5BC112 = 0;
    }

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

Dove* Dove::vdtor_41F630(signed int flags)
{
    dtor_41F870();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Dove::AsAlmostACircle_41FA20(FP xpos, FP ypos, BYTE angle)
{
    AsACircle_41F980(xpos, ypos, angle);
    field_FE_state = State::State_4_AlmostACircle;
}

void Dove::AsACircle_41F980(FP xpos, FP ypos, BYTE angle)
{
    field_100_xJoin = xpos;
    field_104_yJoin = ypos;
    field_10C_angle = angle;
    field_FE_state = State::State_3_Circle;

    // TODO: Result thrown away.. some old removed behavior ??
    //(Math_Sine_496DD0(field_10C) * FP_FromInteger(30)) * field_CC_sprite_scale;
    //(Math_Cosine_496CD0(field_10C) * FP_FromInteger(35)) * field_CC_sprite_scale;
}

void Dove::AsJoin_41F940(FP xpos, FP ypos)
{
    field_100_xJoin = xpos;
    field_104_yJoin = ypos;
    field_FE_state = State::State_2_Join;
    field_108_timer = sGnFrame_5C1B84 + 47;
}

void Dove::FlyAway_420020(__int16 a2)
{
    if (field_FE_state != State::State_1_FlyAway)
    {
        field_FE_state = State::State_1_FlyAway;
        if (a2)
        {
            field_F4_counter = -1;
        }
        else
        {
            field_F4_counter = -10 - Math_NextRandom() % 10;
        }
    }
}

ALIVE_VAR(1, 0x5BC10C, int, bExtraSeqStarted_5BC10C, 0);

static int dword_5BC114 = 0;
static short word_551546 = 0;
static short word_551544 = 0;

void Dove::vUpdate_41FAE0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (!bTheOneControllingTheMusic_5BC112)
    {
        SND_SEQ_PlaySeq_4CA960(SeqId::NecrumAmbient2_17, 0, 1);
        bTheOneControllingTheMusic_5BC112 = 1;
    }

    switch (field_FE_state)
    {
    case State::State_0_OnGround:
        if (Event_Get_422C00(kEventSpeaking))
        {
            Dove::All_FlyAway_41FA60(0); // something is speaking, leg it
        }

        if (Event_Get_422C00(0))
        {
            // player getting near
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(2), sControlledCharacter_5C1B8C))
            {
                Dove::All_FlyAway_41FA60(1);
            }
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(4), sControlledCharacter_5C1B8C))
            {
                // noise is too near, leg it
                Dove::All_FlyAway_41FA60(0);
            }
        }
        break;

    case State::State_1_FlyAway:
        field_F4_counter++;
        if (field_F4_counter == 0)
        {
            field_20_animation.Set_Animation_Data_409C80(5516, 0);
            if (!bExtraSeqStarted_5BC10C)
            {
                bExtraSeqStarted_5BC10C = 13;
                SFX_Play_46FA90(13u, 0);
            }
        }

        if (field_F4_counter > 0)
        {
            field_B8_xpos += field_C4_velx;
            field_BC_ypos += field_C8_vely;
        }

        field_C8_vely = (field_C8_vely * FP_FromDouble(1.03));
        field_C4_velx = (field_C4_velx * FP_FromDouble(1.03));

        if (field_F4_counter >= (25 - (Math_NextRandom() & 7)))
        {
            field_F4_counter += (Math_NextRandom() & 7) - 25;
            field_C4_velx = -field_C4_velx;
        }

        if (field_C4_velx >= FP_FromInteger(0))
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }
        else
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        break;

    case State::State_2_Join:
    {
        if (static_cast<int>(sGnFrame_5C1B84) > field_108_timer)
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }

        FP xOff = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            xOff = FP_FromInteger(4);
        }
        else
        {
            xOff = FP_FromInteger(-4);
        }

        field_C4_velx = (xOff+field_100_xJoin - field_B8_xpos) / FP_FromInteger(8);
        field_C8_vely = (field_104_yJoin - field_BC_ypos) / FP_FromInteger(8);
        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;
    }
        return;

    case State::State_3_Circle:
        field_110_prevX = field_B8_xpos;
        field_114_prevY = field_BC_ypos;

        field_10C_angle += 4;

        // Spin around this point
        field_B8_xpos = ((Math_Sine_496DD0(field_10C_angle) * FP_FromInteger(30)) * field_CC_sprite_scale) + field_100_xJoin;
        field_BC_ypos = ((Math_Cosine_496CD0(field_10C_angle) * FP_FromInteger(35)) * field_CC_sprite_scale) + field_104_yJoin;
        return;

    case State::State_4_AlmostACircle:
        if (dword_5BC114 != static_cast<int>(sGnFrame_5C1B84))
        {
            const int v22 = word_551546 + word_551544;
            dword_5BC114 = sGnFrame_5C1B84;
            word_551544 += word_551546;
            if (word_551546 + word_551544 == 0)
            {
                word_551546 = 1;
            }
            else if (v22 == 30)
            {
                word_551546 = -1;
            }
        }
        field_114_prevY = field_BC_ypos;
        field_10C_angle += 4;
        field_110_prevX = field_B8_xpos;
        field_B8_xpos = ((Math_Sine_496DD0(field_10C_angle) * FP_FromInteger(word_551544)) * field_CC_sprite_scale) + field_100_xJoin;
        field_BC_ypos = ((Math_Cosine_496CD0(field_10C_angle) * FP_FromInteger(35)) * field_CC_sprite_scale) + field_104_yJoin;
        return;

    default:
        break;
    }

    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void CC Dove::All_FlyAway_41FA60(__int16 a1)
{
    for (int i = 0; i < gDovesArray_5BC100.Size(); i++)
    {
        Dove* pDove = gDovesArray_5BC100.ItemAt(i);
        if (!pDove)
        {
            break;
        }
        pDove->FlyAway_420020(a1);
    }

    bExtraSeqStarted_5BC10C = 0; // TODO: Never read ??
    if (bTheOneControllingTheMusic_5BC112)
    {
        SND_SEQ_Stop_4CAE60(SeqId::NecrumAmbient2_17);
        bTheOneControllingTheMusic_5BC112 = 0;
    }
}
