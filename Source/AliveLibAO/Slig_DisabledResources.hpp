#pragma once

// TODO: Delete this file after disabled resources removed

namespace AO {

enum SligFlags_DisabledRes
{
    eDisabledRes_Bit1_SligLever = 0x1,
    eDisabledRes_Bit2_SligLift = 0x2,
    eDisabledRes_Bit3_SligZ = 0x4,
    eDisabledRes_Bit4 = 0x8,
    eDisabledRes_Bit5 = 0x10,
    eDisabledRes_Bit6 = 0x20,
    eDisabledRes_Bit7_SligSleep = 0x40,
    eDisabledRes_Bit8_SligKnfd = 0x80,
    eDisabledRes_Bit9_SligEdge = 0x100,
    eDisabledRes_Bit10_SligSmash = 0x200,
    eDisabledRes_Bit11_SligBeat = 0x400
};
} // namespace AO
