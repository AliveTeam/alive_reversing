#include "stdafx.h"
#include "Dove.hpp"
#include "Function.hpp"
#include "Midi.hpp"
#include "Path.hpp"
#include "Math.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5bc112, short, sSeqPlaying_5BC112, 0);

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
    field_FE_state = 1;
    field_FC = 1;
    field_F4_counter = 0;

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;
    field_110 = xpos;
    field_114 = ypos;

    field_F8_tlvInfo = 0;

    field_20_animation.SetFrame_409D50(Math_NextRandom() & 6);

    if (sSeqPlaying_5BC112)
    {
        return this;
    }
    SND_SEQ_PlaySeq_4CA960(17, 0, 1);
    sSeqPlaying_5BC112 = 1;

    return this;
}

void Dove::vRender_4200B0(int** ot)
{
    // Kind of pointless, the override just calls base
    Render_424B90(ot);
}

void Dove::dtor_41F870()
{
    SetVTable(this, 0x544A90); // vTbl_Dove_544A90

    if (!field_FC)
    {
        gDovesArray_5BC100.Remove_Item(this);
        if (field_F8_tlvInfo)
        {
            Path::TLV_Reset_4DB8E0(field_F8_tlvInfo, -1, 0, 0);
        }
    }

    if (sSeqPlaying_5BC112)
    {
        SND_SEQ_Stop_4CAE60(17);
        sSeqPlaying_5BC112 = 0;
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

void Dove::sub_41FA20(FP xpos, FP ypos, BYTE angle)
{
    sub_41F980(xpos, ypos, angle);
    field_FE_state = 4;
}

void Dove::sub_41F980(FP xpos, FP ypos, BYTE angle)
{
    field_100 = xpos;
    field_104 = ypos;
    field_10C = angle;
    field_FE_state = 3;

    // TODO: Result thrown away.. some old removed behavior ??
    //(Math_Sine_496DD0(field_10C) * FP_FromInteger(30)) * field_CC_sprite_scale;
    //(Math_Cosine_496CD0(field_10C) * FP_FromInteger(35)) * field_CC_sprite_scale;
}
