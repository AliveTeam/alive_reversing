#include "stdafx.h"
#include "GasEmitter.hpp"
#include "Math.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Particle.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "GasCountDown.hpp"
#include "Function.hpp"

ALIVE_VAR(1, 0x5BD4C8, GasEmitter*, sMainGasEmitter_5BD4C8, nullptr);
ALIVE_VAR(1, 0x5BD4CC, DWORD, sGasEmiterAudioMask_5BD4CC, 0);

GasEmitter* GasEmitter::ctor_43CAA0(Path_GasEmitter* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5451D0);

    field_4_typeId = Types::eNone_0;

    field_28_draw_flipper = 1;

    field_2A_switch_id = pTlv->field_10_port_id;
    field_2C_gas_colour = pTlv->field_12_colour;

    field_20_tlvInfo = tlvInfo;

    field_30_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_34_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    // Probably scale ?
    field_38_fp_not_used = FP_FromInteger(1);

    field_24_emit_power = Math_NextRandom() % 4;

    return this;
}

BaseGameObject* GasEmitter::VDestructor(signed int flags)
{
    return vdtor_43CB40(flags);
}

void GasEmitter::VUpdate()
{
    vUpdate_43CB70();
}

void GasEmitter::VScreenChanged()
{
    vScreenChanged_43CE20();
}

void GasEmitter::VStopAudio()
{
    vStopAudio_43CDE0();
}

void GasEmitter::dtor_43CD20()
{
    SetVTable(this, 0x5451D0);
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 0);
    if (sMainGasEmitter_5BD4C8 == this)
    {
        sMainGasEmitter_5BD4C8 = 0;
        SND_Stop_Channels_Mask_4CA810(sGasEmiterAudioMask_5BD4CC);
        sGasEmiterAudioMask_5BD4CC = 0;
    }
    BaseGameObject_dtor_4DBEC0();
}

GasEmitter* GasEmitter::vdtor_43CB40(signed int flags)
{
    dtor_43CD20();
    if (flags & 1)
    {
        alive_delete_free(this);
    }
    return this;
}

void GasEmitter::vStopAudio_43CDE0()
{
    if (sMainGasEmitter_5BD4C8 == this)
    {
        sMainGasEmitter_5BD4C8 = 0;
        SND_Stop_Channels_Mask_4CA810(sGasEmiterAudioMask_5BD4CC);
        sGasEmiterAudioMask_5BD4CC = 0;
    }
}

void GasEmitter::vScreenChanged_43CE20()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void GasEmitter::vUpdate_43CB70()
{
    if ((gGasOn_5C1C00 &&
        !((sGnFrame_5C1B84 + field_24_emit_power) % 4))||
        (SwitchStates_Get_466020(field_2A_switch_id) && field_28_draw_flipper && Math_RandomRange_496AB0(0, 1)))
    {
        switch (field_2C_gas_colour)
        {
        case GasColour::Yellow_0:
            New_Particles_426C70(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 128, 128, 32);
            break;

        case GasColour::Red_1:
            New_Particles_426C70(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 128, 32, 32);
            break;

        case GasColour::Green_2:
            New_Particles_426C70(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 32, 128, 32);
            break;

        case GasColour::Blue_3:
            New_Particles_426C70(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 32, 32, 128);
            break;

        case GasColour::White_4:
            New_Particles_426C70(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 128, 128, 128);
            break;

        default:
            break;
        }

        if (!sMainGasEmitter_5BD4C8)
        {
            SFX_Play_46FA90(82u, 127);
            sMainGasEmitter_5BD4C8 = this;
            sGasEmiterAudioMask_5BD4CC = SFX_Play_46FA90(83u, 127);
        }

    }

    // Flip the value
    field_28_draw_flipper = !field_28_draw_flipper;
}
