#pragma once

namespace ODDIO {

// All patches in this file are based on Steams US version.

// VH files contain vag attributes
// VB files contain audio data

// SAMPLE RATE LOOKUP TABLES
// [0] == vag (unique per .VH, even if the audio sample is duplicated in other VH)
// [1] == sample rate of pc audio file
// if a sound is not found within these tables (which a lot aren't)
// the sample rate is the default psx sample rate of 8000hz

// VAG ATTRIBUTE FIXES
// The pc vag attributes were assumingly altered to account
// for higher sample rate audio files. Assumingly during that
// process volumes, root notes, pitches, etc were altered. These
// tables correct the pc sound attributes to be like psx.

// First column (vag) number can be correlated between rate and attr tables
// for a given VH/VB pair (Example, D1SNDFX.VH and D1SNDFX.VB)

// RATE TABLES
// {vag, sampleRate}

// VH TABLES
// {vag, vagOffset, [256 bits of vag data]}

// vagOffset is based on counting the index when loading the pc
// vag attributes from level files. There are many indexes that are
// skipped, so you may see numbers go in order from [2, 35, 123]


////////////////////////////////////////
// D1.lvl - D1SNDFX.VH
const std::vector<std::vector<s32>> AO_D1SNDFX_RATE = {{
    {41, 22050},
    {49, 16000},
    {50, 16000},
    {83, 16000},
    {107, 22050},
    {109, 16000},
    {110, 22050},
    {114, 22050},
    {118, 22050},
    {119, 22050}}};

const std::vector<std::vector<u32>> AO_D1SNDFX_VH = {{
    {106, 496, 0x407f007f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x6a003a, 0xc100c0, 0xc300c2},
    {0, 497, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {107, 512, 0x407f007f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6b003b, 0xc100c0, 0xc300c2},
    {109, 514, 0x407f007f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0xdfe380ff, 0x6d003b, 0xc100c0, 0xc300c2},
    {110, 515, 0x407f007f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0xdfe380ff, 0x6e003b, 0xc100c0, 0xc300c2},
    {114, 545, 0x407f007f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0xdfe380ff, 0x72003d, 0xc100c0, 0xc300c2},
    {118, 549, 0x407f007f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0xdfe380ff, 0x76003d, 0xc100c0, 0xc300c2},
    {119, 550, 0x407f007f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0xdfe380ff, 0x77003d, 0xc100c0, 0xc300c2},
    {49, 720, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x310050, 0xc100c0, 0xc300c2},
    {49, 721, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x310050, 0xc100c0, 0xc300c2},
    {41, 736, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0x5febb4ff, 0x290051, 0xc100c0, 0xc300c2},
    {69, 752, 0x7f047f, 0x7e000454, 0x0, 0xb2b10000, 0x5fefbbff, 0x450052, 0xc100c0, 0xc300c2},
    {69, 753, 0x7f7f045b, 0x7e000e54, 0x0, 0xb2b10000, 0x5fefbbff, 0x450052, 0xc100c0, 0xc300c2},
    {69, 800, 0x407f0465, 0x7f000054, 0x0, 0xb2b10000, 0xdfec80ff, 0x450055, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// D2.lvl - D2SNDFX.VH
const std::vector<std::vector<s32>> AO_D2SNDFX_RATE = {{
    {51, 22050}, 
    {61, 16000},
    {84, 16000},
    {98, 22050}, 
    {100, 16000},
    {101, 22050},
    {105, 22050},
    {109, 22050},
    {110, 22050}}};

const std::vector<std::vector<u32>> AO_D2SNDFX_VH = {{
    {93, 272, 0x2c7f047f, 0x24240060, 0x0, 0xb2b10000, 0x1fc080ff, 0x5d0014, 0xc100c0, 0xc300c2},
    {93, 274, 0x407f047f, 0x2b2b0060, 0x0, 0xb2b10000, 0x1fc080ff, 0x5d0014, 0xc100c0, 0xc300c2},
    {93, 276, 0x547f047f, 0x30300060, 0x0, 0xb2b10000, 0x1fc080ff, 0x5d0014, 0xc100c0, 0xc300c2},
    {97, 576, 0x407f047f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x61003a, 0xc100c0, 0xc300c2},
    {0, 577, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {98, 592, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x62003b, 0xc100c0, 0xc300c2},
    {100, 594, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x64003b, 0xc100c0, 0xc300c2},
    {101, 595, 0x407f047f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x65003b, 0xc100c0, 0xc300c2},
    {105, 625, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x69003d, 0xc100c0, 0xc300c2},
    {109, 629, 0x407f047f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6d003d, 0xc100c0, 0xc300c2},
    {110, 630, 0x407f047f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6e003d, 0xc100c0, 0xc300c2},
    {61, 800, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x3d0050, 0xc100c0, 0xc300c2},
    {61, 801, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x3d0050, 0xc100c0, 0xc300c2},
    {51, 816, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0x5febb4ff, 0x330051, 0xc100c0, 0xc300c2},
    {85, 832, 0x407f0465, 0x7f000460, 0x0, 0xb2b10000, 0xdfefbbff, 0x550052, 0xc100c0, 0xc300c2},
    {85, 864, 0x407f0465, 0x7f000454, 0x0, 0xb2b10000, 0xdfec80ff, 0x550055, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// D7.LVL - D2ENDER.VH
const std::vector<std::vector<s32>> AO_D2ENDER_RATE = {{
    {18, 22050},
    {25, 16000},
    {41, 16000},
    {46, 22050},
    {48, 16000},
    {49, 22050},
    {53, 22050},
    {57, 22050},
    {58, 22050}}};

const std::vector<std::vector<u32>> AO_D2ENDER_VH = {{
    {45, 320, 0x407f047f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x2d003a, 0xc100c0, 0xc300c2},
    {0, 321, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {46, 336, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x2e003b, 0xc100c0, 0xc300c2},
    {48, 338, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x30003b, 0xc100c0, 0xc300c2},
    {49, 339, 0x407f047f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x31003b, 0xc100c0, 0xc300c2},
    {53, 369, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x35003d, 0xc100c0, 0xc300c2},
    {57, 373, 0x407f047f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x39003d, 0xc100c0, 0xc300c2},
    {58, 374, 0x407f047f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x3a003d, 0xc100c0, 0xc300c2},
    {25, 608, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x190050, 0xc100c0, 0xc300c2},
    {25, 609, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x190050, 0xc100c0, 0xc300c2},
    {18, 624, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0x5febb4ff, 0x120051, 0xc100c0, 0xc300c2},
    {42, 640, 0x407f0465, 0x7f000460, 0x0, 0xb2b10000, 0xdfefbbff, 0x2a0052, 0xc100c0, 0xc300c2},
    {42, 656, 0x407f0465, 0x7f000454, 0x0, 0xb2b10000, 0xdfec80ff, 0x2a0055, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// E1.LVL - E1SNDFX.VH
const std::vector<std::vector<s32>> AO_E1SNDFX_RATE = {{
    {49, 44100},
    {50, 44100},
    {51, 22050},
    {60, 16000},
    {61, 16000},
    {85, 22050},
    {87, 16000},
    {88, 22050},
    {92, 22050},
    {96, 22050},
    {97, 22050}}};

const std::vector<std::vector<u32>> AO_E1SNDFX_VH = {{
    {84, 448, 0x407f007f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x54003a, 0xc100c0, 0xc300c2},
    {0, 449, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {85, 464, 0x407f007f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x55003b, 0xc100c0, 0xc300c2},
    {87, 466, 0x407f007f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x57003b, 0xc100c0, 0xc300c2},
    {88, 467, 0x407f007f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x58003b, 0xc100c0, 0xc300c2},
    {92, 497, 0x407f007f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x5c003d, 0xc100c0, 0xc300c2},
    {96, 501, 0x407f007f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x60003d, 0xc100c0, 0xc300c2},
    {97, 502, 0x407f007f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x61003d, 0xc100c0, 0xc300c2},
    {51, 608, 0x407f0400, 0x7f000066, 0x0, 0xb2b10000, 0xdfec80ff, 0x33004a, 0xc100c0, 0xc300c2},
    {49, 624, 0x7f007f, 0x30304659, 0x0, 0xb2b10101, 0xdff080ff, 0x31004b, 0xc100c0, 0xc300c2},
    {50, 625, 0x7f7f047f, 0x3c3c4665, 0x0, 0xb2b10101, 0xdff080ff, 0x32004b, 0xc100c0, 0xc300c2},
    {50, 626, 0x407f047f, 0x48484671, 0x0, 0xb2b10101, 0xdff080ff, 0x32004b, 0xc100c0, 0xc300c2},
    {51, 640, 0x407f044f, 0x7f00465a, 0x0, 0xb2b10101, 0xdff0c0ff, 0x33004c, 0xc100c0, 0xc300c2},
    {60, 704, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x3c0050, 0xc100c0, 0xc300c2},
    {60, 705, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x3c0050, 0xc100c0, 0xc300c2},
    {51, 720, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0x5febb4ff, 0x330051, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// E2.LVL - E2SNDFX.VH
const std::vector<std::vector<s32>> AO_E1SNDFX_RATE = {{
    {50, 44100},
    {51, 44100},
    {52, 22050},
    {63, 16000},
    {64, 16000},
    {76, 22050},
    {78, 16000},
    {79, 16000},
    {83, 22050},
    {87, 22050},
    {88, 22050}}};

const std::vector<std::vector<u32>> AO_E2SNDFX_VH = {{
    {75, 432, 0x407f007f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x4b003a, 0xc100c0, 0xc300c2},
    {0, 433, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {76, 448, 0x407f007f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4c003b, 0xc100c0, 0xc300c2},
    {78, 450, 0x407f007f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4e003b, 0xc100c0, 0xc300c2},
    {79, 451, 0x407f007f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4f003b, 0xc100c0, 0xc300c2},
    {83, 481, 0x407f007f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x53003d, 0xc100c0, 0xc300c2},
    {87, 485, 0x407f007f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x57003d, 0xc100c0, 0xc300c2},
    {88, 486, 0x407f007f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x58003d, 0xc100c0, 0xc300c2},
    {52, 608, 0x407f044f, 0x7f00465a, 0x0, 0xb2b10101, 0x5ff0c0ff, 0x34004c, 0xc100c0, 0xc300c2},
    {63, 672, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x3f0050, 0xc100c0, 0xc300c2},
    {63, 673, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x3f0050, 0xc100c0, 0xc300c2},
    {52, 688, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0x5febb4ff, 0x340051, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// F1.LVL - F1SNDFX.VH
const std::vector<std::vector<s32>> AO_F1SNDFX_RATE = {{
    {55, 16000},
    {56, 16000},
    {92, 22050},
    {94, 22050},
    {96, 16000},
    {97, 22050},
    {101, 22050},
    {105, 22050},
    {106, 22050}}};

const std::vector<std::vector<u32>> AO_F1SNDFX_VH = {{
    {93, 416, 0x407f007f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x5d003a, 0xc100c0, 0xc300c2},
    {0, 417, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {94, 432, 0x407f007f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x5e003b, 0xc100c0, 0xc300c2},
    {96, 434, 0x407f007f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x60003b, 0xc100c0, 0xc300c2},
    {97, 435, 0x407f007f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x61003b, 0xc100c0, 0xc300c2},
    {101, 465, 0x407f007f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x65003d, 0xc100c0, 0xc300c2},
    {105, 469, 0x407f007f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x69003d, 0xc100c0, 0xc300c2},
    {106, 470, 0x407f007f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6a003d, 0xc100c0, 0xc300c2},
    {55, 688, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x370050, 0xc100c0, 0xc300c2},
    {55, 689, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x370050, 0xc100c0, 0xc300c2},
    {92, 704, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0xdfebb4ff, 0x5c0051, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// F2.LVL - F2SNDFX.VH
const std::vector<std::vector<u32>> AO_F2SNDFX_VH = {{
    {6, 99, 0x407f0402, 0x2b240060, 0x0, 0xb2b10202, 0x5feb80ff, 0x60007, 0xc100c0, 0xc300c2},
    {10, 100, 0x407f0402, 0x2d2c0074, 0x0, 0xb2b10202, 0x5feb80ff, 0xa0007, 0xc100c0, 0xc300c2},
    {11, 101, 0x407f0402, 0x2e2e0060, 0x0, 0xb2b10202, 0x5ffd80ff, 0xb0007, 0xc100c0, 0xc300c2},
    {26, 102, 0x407f0402, 0x2f2f0060, 0x0, 0xb2b10202, 0x5feb80ff, 0x1a0007, 0xc100c0, 0xc300c2},
    {27, 103, 0x407f0402, 0x30300060, 0x0, 0xb2b10202, 0x5feb80ff, 0x1b0007, 0xc100c0, 0xc300c2},
    {40, 104, 0x407f0402, 0x37310060, 0x0, 0xb2b10202, 0x5feb80ff, 0x280007, 0xc100c0, 0xc300c2},
    {49, 105, 0x407f0402, 0x3938006c, 0x0, 0xb2b10202, 0x5feb80ff, 0x310007, 0xc100c0, 0xc300c2},
    {53, 106, 0x407f0402, 0x4341007f, 0x0, 0xb2b10202, 0x5feb80ff, 0x350007, 0xc100c0, 0xc300c2},
    {59, 108, 0x407f0402, 0x4b45007f, 0x0, 0xb2b10202, 0x5feb80ff, 0x3b0007, 0xc100c0, 0xc300c2},
    {97, 272, 0x2c7f047f, 0x24240060, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x610014, 0xc100c0, 0xc300c2},
    {97, 274, 0x407f047f, 0x2b2b0060, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x610014, 0xc100c0, 0xc300c2},
    {97, 276, 0x547f047f, 0x30300060, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x610014, 0xc100c0, 0xc300c2},
    {100, 608, 0x407f047f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x64003a, 0xc100c0, 0xc300c2},
    {0, 609, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {101, 624, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x65003b, 0xc100c0, 0xc300c2},
    {103, 626, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x67003b, 0xc100c0, 0xc300c2},
    {104, 627, 0x407f047f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x68003b, 0xc100c0, 0xc300c2},
    {108, 657, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6c003d, 0xc100c0, 0xc300c2},
    {112, 661, 0x407f047f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x70003d, 0xc100c0, 0xc300c2},
    {113, 662, 0x407f047f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x71003d, 0xc100c0, 0xc300c2},
    {57, 864, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x390050, 0xc100c0, 0xc300c2},
    {57, 865, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x390050, 0xc100c0, 0xc300c2},
    {46, 880, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0x5febb4ff, 0x2e0051, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// F4.LVL - F2ENDER.VH
const std::vector<std::vector<u32>> AO_F2ENDER_VH = {{
    {59, 352, 0x407f047f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x3b003a, 0xc100c0, 0xc300c2},
    {0, 353, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {60, 368, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x3c003b, 0xc100c0, 0xc300c2},
    {62, 370, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x3e003b, 0xc100c0, 0xc300c2},
    {63, 371, 0x407f047f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x3f003b, 0xc100c0, 0xc300c2},
    {67, 401, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x43003d, 0xc100c0, 0xc300c2},
    {71, 405, 0x407f047f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x47003d, 0xc100c0, 0xc300c2},
    {72, 406, 0x407f047f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x48003d, 0xc100c0, 0xc300c2},
    {35, 624, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x230050, 0xc100c0, 0xc300c2},
    {35, 625, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x230050, 0xc100c0, 0xc300c2},
    {27, 640, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0x5febb4ff, 0x1b0051, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// L1.LVL - MLSNDFX.VH
const std::vector<std::vector<s32>> AO_MLSNDFX_RATE = {{
    {41, 16000},
    {74, 16000},
    {96, 22050},
    {98, 16000},
    {99, 22050},
    {103, 22050},
    {107, 22050},
    {108, 22050},
    {130, 22050}}};

const std::vector<std::vector<u32>> AO_MLSNDFX_VH = {{
    {134, 224, 0x2c7f047f, 0x24240060, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x860014, 0xc100c0, 0xc300c2},
    {134, 226, 0x407f047f, 0x2b2b0060, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x860014, 0xc100c0, 0xc300c2},
    {134, 228, 0x547f047f, 0x30300060, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x860014, 0xc100c0, 0xc300c2},
    {95, 496, 0x407f047f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x5f003a, 0xc100c0, 0xc300c2},
    {0, 497, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {96, 512, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x60003b, 0xc100c0, 0xc300c2},
    {98, 514, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x62003b, 0xc100c0, 0xc300c2},
    {99, 515, 0x407f047f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x63003b, 0xc100c0, 0xc300c2},
    {103, 545, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x67003d, 0xc100c0, 0xc300c2},
    {107, 549, 0x407f047f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6b003d, 0xc100c0, 0xc300c2},
    {108, 550, 0x407f047f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6c003d, 0xc100c0, 0xc300c2},
    {74, 752, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x4a0050, 0xc100c0, 0xc300c2},
    {74, 753, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x4a0050, 0xc100c0, 0xc300c2},
    {130, 768, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0xdfebb4ff, 0x820051, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// S1.LVL / C1.LVL - OPTSNDEFX.VH and MUNK.VH
const std::vector<std::vector<s32>> AO_MUNK_RATE = {{}};

const std::vector<std::vector<u32>> AO_MUNK_VH = {{}};

const std::vector<std::vector<s32>> AO_OPTSNDFX_RATE = {{
    {1, 4000},
    {7, 44100},
    {8, 44100},
    {9, 22050},
    {14, 16000},
    {16, 22050},
    {18, 16000},
    {19, 22050},
    {23, 22050},
    {27, 22050},
    {28, 22050}}};

const std::vector<std::vector<u32>> AO_OPTSNDFX_VH = {{
    {7, 16, 0x7f007f, 0x30304659, 0x0, 0xb2b10101, 0xdfe680ff, 0x70001, 0xc100c0, 0xc300c2},
    {8, 17, 0x7f7f047f, 0x3c3c4665, 0x0, 0xb2b10101, 0xdfe680ff, 0x80001, 0xc100c0, 0xc300c2},
    {8, 18, 0x407f047f, 0x48484671, 0x0, 0xb2b10101, 0xdfe680ff, 0x80001, 0xc100c0, 0xc300c2},
    {9, 32, 0x407f044f, 0x7f00465a, 0x0, 0xb2b10101, 0xdff0c0ff, 0x90002, 0xc100c0, 0xc300c2},
    {14, 64, 0x6e047f, 0x7f005a48, 0x0, 0xb2b17f7f, 0xdfe780ff, 0xe0004, 0xc100c0, 0xc300c2},
    {14, 65, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b17f7f, 0xdfe780ff, 0xe0004, 0xc100c0, 0xc300c2},
    {15, 144, 0x407f007f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0xf003a, 0xc100c0, 0xc300c2},
    {0, 145, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {16, 160, 0x407f007f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0xdffd80ff, 0x10003b, 0xc100c0, 0xc300c2},
    {18, 162, 0x407f007f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0xdffd80ff, 0x12003b, 0xc100c0, 0xc300c2},
    {19, 163, 0x407f007f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0xdffd80ff, 0x13003b, 0xc100c0, 0xc300c2},
    {23, 193, 0x407f007f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0xdffd80ff, 0x17003d, 0xc100c0, 0xc300c2},
    {27, 197, 0x407f007f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0xdffd80ff, 0x1b003d, 0xc100c0, 0xc300c2},
    {28, 198, 0x407f007f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0xdffd80ff, 0x1c003d, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// R1.LVL / R2.LVL - RFSNDEFX.VH
const std::vector<std::vector<s32>> AO_RFSNDFX_RATE = {{
    {44, 22050},
    {54, 16000},
    {55, 16000},
    {100, 22050},
    {102, 16000},
    {103, 22050},
    {107, 22050},
    {111, 22050},
    {112, 22050}}};

const std::vector<std::vector<u32>> AO_RFSNDFX_VH = {{
    {88, 25, 0x407f0400, 0x39390045, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x580001, 0xc100c0, 0xc300c2},
    {132, 80, 0x4064047f, 0x3030006c, 0x0, 0xb2b17f7f, 0xdff580ff, 0x840006, 0xc100c0, 0xc300c2},
    {133, 81, 0x4053047f, 0x30300060, 0x0, 0xb2b17f7f, 0xd170bebf, 0x850006, 0xc100c0, 0xc300c2},
    {133, 82, 0x407f047f, 0x24240054, 0x0, 0xb2b17f7f, 0xdff0bdff, 0x850006, 0xc100c0, 0xc300c2},
    {94, 97, 0x407f0402, 0x30240054, 0x0, 0xb2b10000, 0xdfeb80ff, 0x5e0007, 0xc100c0, 0xc300c2},
    {1, 98, 0x407f0402, 0x23236a54, 0x0, 0xb2b10000, 0xdfe080ff, 0x10007, 0xc100c0, 0xc300c2},
    {2, 99, 0x407f0402, 0x2222005e, 0x0, 0xb2b10000, 0x1fc080ff, 0x20007, 0xc100c0, 0xc300c2},
    {3, 100, 0x407f0402, 0x21210051, 0x0, 0xb2b10000, 0x1fc080ff, 0x30007, 0xc100c0, 0xc300c2},
    {7, 101, 0x407f0402, 0x2020004a, 0x0, 0xb2b10000, 0x1fc080ff, 0x70007, 0xc100c0, 0xc300c2},
    {9, 102, 0x407f0402, 0x1f1f0049, 0x0, 0xb2b10000, 0x1fc080ff, 0x90007, 0xc100c0, 0xc300c2},
    {12, 103, 0x407f0402, 0x1e1e0042, 0x0, 0xb2b10000, 0x1fc080ff, 0xc0007, 0xc100c0, 0xc300c2},
    {16, 104, 0x407f0402, 0x33310061, 0x0, 0xb2b10000, 0x1fc080ff, 0x100007, 0xc100c0, 0xc300c2},
    {17, 105, 0x407f0402, 0x3f340060, 0x0, 0xb2b10000, 0x1fc080ff, 0x110007, 0xc100c0, 0xc300c2},
    {88, 193, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x58000f, 0xc100c0, 0xc300c2},
    {72, 194, 0x407f007f, 0x3c3c0048, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x48000f, 0xc100c0, 0xc300c2},
    {93, 226, 0x407f047f, 0x3b3b4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x5d0011, 0xc100c0, 0xc300c2},
    {73, 256, 0x407f047f, 0x30300054, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x490013, 0xc100c0, 0xc300c2},
    {73, 368, 0x407f047f, 0x3c3c0054, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x49001e, 0xc100c0, 0xc300c2},
    {78, 384, 0x407f0452, 0x3c3c0054, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4e001f, 0xc100c0, 0xc300c2},
    {85, 400, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x550020, 0xc100c0, 0xc300c2},
    {73, 448, 0x407f047f, 0x3c3c005a, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x490026, 0xc100c0, 0xc300c2},
    {68, 464, 0x407f047f, 0x3c3c0448, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x440027, 0xc100c0, 0xc300c2},
    {69, 465, 0x407f047f, 0x4040044c, 0x0, 0xb2b17f7f, 0x5feb80ff, 0x450027, 0xc100c0, 0xc300c2},
    {70, 466, 0x407f0451, 0x4242045a, 0x0, 0xb2b17f7f, 0x5feb80ff, 0x460027, 0xc100c0, 0xc300c2},
    {71, 467, 0x407f044b, 0x4343044f, 0x0, 0xb2b17f7f, 0x5feb80ff, 0x470027, 0xc100c0, 0xc300c2},
    {72, 468, 0x407f047f, 0x41410459, 0x0, 0xb2b17f7f, 0x5feb80ff, 0x480027, 0xc100c0, 0xc300c2},
    {74, 469, 0x4075047f, 0x4444005c, 0x0, 0xb2b17f7f, 0x5fe080ff, 0x4a0027, 0xc100c0, 0xc300c2},
    {75, 470, 0x407f047f, 0x3d3d0457, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4b0027, 0xc100c0, 0xc300c2},
    {76, 480, 0x407f047f, 0x3c3c0054, 0x0, 0xb2b17f7f, 0x5fe080ff, 0x4c0028, 0xc100c0, 0xc300c2},
    {77, 481, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4d0028, 0xc100c0, 0xc300c2},
    {87, 496, 0x407f047f, 0x40404659, 0x0, 0xb2b17f7f, 0xdffe80ff, 0x57002a, 0xc100c0, 0xc300c2},
    {129, 512, 0x407f0419, 0x3e3e465b, 0x0, 0xb2b17f7f, 0xdfe380ff, 0x81002c, 0xc100c0, 0xc300c2},
    {130, 513, 0x407f0419, 0x3f3f465c, 0x0, 0xb2b17f7f, 0xdfe380ff, 0x82002c, 0xc100c0, 0xc300c2},
    {84, 528, 0x407f047f, 0x30304659, 0x0, 0xb2b17f7f, 0xdfe380ff, 0x54002d, 0xc100c0, 0xc300c2},
    {86, 544, 0x407f0400, 0x3c3c003c, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x56002e, 0xc100c0, 0xc300c2},
    {89, 560, 0x407f047f, 0x7f000054, 0x0, 0xb2b10000, 0x1fc080ff, 0x59002f, 0xc100c0, 0xc300c2},
    {89, 561, 0x5450047f, 0x7f000048, 0x0, 0xb2b10000, 0x1fc080ff, 0x59002f, 0xc100c0, 0xc300c2},
    {89, 562, 0x4064047f, 0x7f00005a, 0x0, 0xb2b10000, 0x1fc080ff, 0x59002f, 0xc100c0, 0xc300c2},
    {90, 563, 0x2c6e047f, 0x7f000060, 0x0, 0xb2b10000, 0x1fc080ff, 0x5a002f, 0xc100c0, 0xc300c2},
    {90, 564, 0x2c55047f, 0x7f00006c, 0x0, 0xb2b10000, 0x1fc080ff, 0x5a002f, 0xc100c0, 0xc300c2},
    {90, 565, 0x544b047f, 0x7f000072, 0x0, 0xb2b10000, 0x1fc080ff, 0x5a002f, 0xc100c0, 0xc300c2},
    {91, 576, 0x407f047f, 0x24240054, 0x0, 0xb2b17f7f, 0x5fcb80ff, 0x5b0030, 0xc100c0, 0xc300c2},
    {92, 577, 0x407f047f, 0x2a2a0054, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x5c0030, 0xc100c0, 0xc300c2},
    {92, 578, 0x407f047f, 0x3c3c0054, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x5c0030, 0xc100c0, 0xc300c2},
    {91, 579, 0x407f047f, 0x3e3e0056, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x5b0030, 0xc100c0, 0xc300c2},
    {97, 580, 0x407f047f, 0x3f3f0057, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x610030, 0xc100c0, 0xc300c2},
    {95, 592, 0x407f047f, 0x3c3c0054, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x5f0031, 0xc100c0, 0xc300c2},
    {96, 593, 0x407f0440, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x600031, 0xc100c0, 0xc300c2},
    {98, 608, 0x407f047f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x62003a, 0xc100c0, 0xc300c2},
    {0, 609, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {99, 624, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x63003b, 0xc100c0, 0xc300c2},
    {100, 625, 0x407f047f, 0x3e3e465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x64003b, 0xc100c0, 0xc300c2},
    {101, 626, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x65003b, 0xc100c0, 0xc300c2},
    {102, 627, 0x407f047f, 0x3f3f005b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x66003b, 0xc100c0, 0xc300c2},
    {103, 640, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x67003c, 0xc100c0, 0xc300c2},
    {104, 641, 0x407f047f, 0x3f3f465c, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x68003c, 0xc100c0, 0xc300c2},
    {105, 656, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x69003d, 0xc100c0, 0xc300c2},
    {106, 657, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6a003d, 0xc100c0, 0xc300c2},
    {107, 658, 0x407f047f, 0x37374653, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6b003d, 0xc100c0, 0xc300c2},
    {108, 659, 0x407f047f, 0x38384655, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6c003d, 0xc100c0, 0xc300c2},
    {109, 660, 0x407f047f, 0x39394656, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6d003d, 0xc100c0, 0xc300c2},
    {110, 661, 0x407f047f, 0x3b3b4658, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6e003d, 0xc100c0, 0xc300c2},
    {111, 662, 0x407f047f, 0x3a3a4657, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6f003d, 0xc100c0, 0xc300c2},
    {112, 663, 0x407f047f, 0x4040465d, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x70003d, 0xc100c0, 0xc300c2},
    {113, 664, 0x407f047f, 0x4141004d, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x71003d, 0xc100c0, 0xc300c2},
    {114, 672, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x72003e, 0xc100c0, 0xc300c2},
    {115, 673, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x73003e, 0xc100c0, 0xc300c2},
    {116, 674, 0x407f047f, 0x3e3e465b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x74003e, 0xc100c0, 0xc300c2},
    {117, 675, 0x407f047f, 0x3f3f465c, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x75003e, 0xc100c0, 0xc300c2},
    {118, 688, 0x407f047f, 0x25254642, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x76003f, 0xc100c0, 0xc300c2},
    {119, 689, 0x407f047f, 0x26264643, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x77003f, 0xc100c0, 0xc300c2},
    {120, 690, 0x407f047f, 0x3c3c0054, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x78003f, 0xc100c0, 0xc300c2},
    {121, 691, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x79003f, 0xc100c0, 0xc300c2},
    {122, 704, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x7a0040, 0xc100c0, 0xc300c2},
    {123, 705, 0x407f047f, 0x3e3e465b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x7b0040, 0xc100c0, 0xc300c2},
    {124, 706, 0x407f047f, 0x27274644, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x7c0040, 0xc100c0, 0xc300c2},
    {125, 707, 0x407f047f, 0x3f3f465c, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x7d0040, 0xc100c0, 0xc300c2},
    {126, 720, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x7e0041, 0xc100c0, 0xc300c2},
    {127, 721, 0x407f047f, 0x4040465d, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x7f0041, 0xc100c0, 0xc300c2},
    {128, 736, 0x407f047f, 0x7f004667, 0x0, 0xb2b17f7f, 0xdfefaaff, 0x800042, 0xc100c0, 0xc300c2},
    {83, 752, 0x7f0404, 0x5050046c, 0x0, 0xb2b10000, 0xdfeb80ff, 0x530046, 0xc100c0, 0xc300c2},
    {83, 753, 0x7f7f0404, 0x5151046d, 0x0, 0xb2b10000, 0xdfeb80ff, 0x530046, 0xc100c0, 0xc300c2},
    {79, 768, 0x407f005a, 0x2f00003c, 0x0, 0xb2b10000, 0xdfe680ff, 0x4f004a, 0xc100c0, 0xc300c2},
    {80, 769, 0x407f045a, 0x36300048, 0x0, 0xb2b10000, 0xdfe680ff, 0x50004a, 0xc100c0, 0xc300c2},
    {81, 770, 0x407f0456, 0x43370054, 0x0, 0xb2b10000, 0xdfe680ff, 0x51004a, 0xc100c0, 0xc300c2},
    {82, 771, 0x407f0456, 0x4f440060, 0x0, 0xb2b10000, 0xdfe680ff, 0x52004a, 0xc100c0, 0xc300c2},
    {83, 772, 0x7f0455, 0x5050006c, 0x0, 0xb2b10000, 0xdfe680ff, 0x53004a, 0xc100c0, 0xc300c2},
    {83, 773, 0x7f7f0453, 0x5151006d, 0x0, 0xb2b10000, 0xdfe680ff, 0x53004a, 0xc100c0, 0xc300c2},
    {131, 786, 0x406e0055, 0x3c3c0048, 0x0, 0xb2b10000, 0xdfeb80ff, 0x83004d, 0xc100c0, 0xc300c2},
    {54, 832, 0x6e047f, 0x7f005a48, 0x0, 0xb2b10000, 0xdfe780ff, 0x360050, 0xc100c0, 0xc300c2},
    {54, 833, 0x7f6e047f, 0x7f006148, 0x0, 0xb2b10000, 0xdfe780ff, 0x360050, 0xc100c0, 0xc300c2},
    {44, 848, 0x407f047f, 0x7f00465a, 0x0, 0xb2b10000, 0x5febb4ff, 0x2c0051, 0xc100c0, 0xc300c2}}};


////////////////////////////////////////
// R6.LVL - RFENDER.VH
const std::vector<std::vector<u32>> AO_RFENDER_VH = {{
    {60, 25, 0x407f0400, 0x39390045, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x3c0001, 0xc100c0, 0xc300c2},
    {64, 81, 0x407f0402, 0x30240054, 0x0, 0xb2b10000, 0xdfeb80ff, 0x400007, 0xc100c0, 0xc300c2},
    {63, 178, 0x407f047f, 0x3b3b4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x3f0011, 0xc100c0, 0xc300c2},
    {54, 208, 0x407f047f, 0x30300054, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x360013, 0xc100c0, 0xc300c2},
    {107, 272, 0x2c7f047f, 0x24240030, 0x0, 0xb2b17f7f, 0x5ffe80ff, 0x6b0019, 0xc100c0, 0xc300c2},
    {107, 274, 0x547f047f, 0x24246e31, 0x0, 0xb2b17f7f, 0x5ffe80ff, 0x6b0019, 0xc100c0, 0xc300c2},
    {57, 304, 0x407f0452, 0x3c3c0054, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x39001f, 0xc100c0, 0xc300c2},
    {58, 320, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x3a0020, 0xc100c0, 0xc300c2},
    {54, 352, 0x407f047f, 0x3c3c005a, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x360026, 0xc100c0, 0xc300c2},
    {110, 368, 0x407f047f, 0x3c3c0448, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x6e0027, 0xc100c0, 0xc300c2},
    {111, 369, 0x4075047f, 0x4444005c, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x6f0027, 0xc100c0, 0xc300c2},
    {55, 384, 0x407f047f, 0x3c3c0054, 0x0, 0xb2b17f7f, 0x5fe080ff, 0x370028, 0xc100c0, 0xc300c2},
    {59, 400, 0x407f0400, 0x3c3c003c, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x3b002e, 0xc100c0, 0xc300c2},
    {0, 401, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x2e, 0xc100c0, 0xc300c2},
    {0, 402, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x2e, 0xc100c0, 0xc300c2},
    {0, 403, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x2e, 0xc100c0, 0xc300c2},
    {0, 404, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x2e, 0xc100c0, 0xc300c2},
    {61, 416, 0x407f047f, 0x24240054, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x3d0030, 0xc100c0, 0xc300c2},
    {62, 417, 0x407f047f, 0x2a2a0054, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x3e0030, 0xc100c0, 0xc300c2},
    {65, 432, 0x407f047f, 0x3c3c0054, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x410031, 0xc100c0, 0xc300c2},
    {66, 433, 0x407f0440, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x420031, 0xc100c0, 0xc300c2},
    {66, 434, 0x407f047f, 0x3e3e0056, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x420031, 0xc100c0, 0xc300c2},
    {100, 448, 0x407f047f, 0x24244641, 0x0, 0xb2b17f7f, 0x5fc380ff, 0x640032, 0xc100c0, 0xc300c2},
    {101, 449, 0x407f047e, 0x3030464d, 0x0, 0xb2b17f7f, 0x5fcb80ff, 0x650032, 0xc100c0, 0xc300c2},
    {68, 464, 0x407f047f, 0x7f000048, 0x0, 0xb2b17f7f, 0xdfeb80ff, 0x44003a, 0xc100c0, 0xc300c2},
    {0, 465, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {0, 466, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {0, 467, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3a, 0xc100c0, 0xc300c2},
    {69, 480, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x45003b, 0xc100c0, 0xc300c2},
    {70, 481, 0x407f047f, 0x3e3e465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x46003b, 0xc100c0, 0xc300c2},
    {73, 496, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x49003c, 0xc100c0, 0xc300c2},
    {74, 497, 0x407f047f, 0x3f3f465c, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4a003c, 0xc100c0, 0xc300c2},
    {0, 498, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3c, 0xc100c0, 0xc300c2},
    {0, 499, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3c, 0xc100c0, 0xc300c2},
    {0, 500, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3c, 0xc100c0, 0xc300c2},
    {0, 501, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3c, 0xc100c0, 0xc300c2},
    {0, 502, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3c, 0xc100c0, 0xc300c2},
    {0, 503, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3c, 0xc100c0, 0xc300c2},
    {0, 504, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x3c, 0xc100c0, 0xc300c2},
    {75, 512, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4b003d, 0xc100c0, 0xc300c2},
    {76, 513, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4c003d, 0xc100c0, 0xc300c2},
    {77, 514, 0x407f047f, 0x37374653, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4d003d, 0xc100c0, 0xc300c2},
    {78, 515, 0x407f047f, 0x38384655, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x4e003d, 0xc100c0, 0xc300c2},
    {84, 528, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x54003e, 0xc100c0, 0xc300c2},
    {85, 529, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x55003e, 0xc100c0, 0xc300c2},
    {86, 530, 0x407f047f, 0x3e3e465b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x56003e, 0xc100c0, 0xc300c2},
    {87, 531, 0x407f047f, 0x3f3f465c, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x57003e, 0xc100c0, 0xc300c2},
    {88, 544, 0x407f047f, 0x25254642, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x58003f, 0xc100c0, 0xc300c2},
    {89, 545, 0x407f047f, 0x26264643, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x59003f, 0xc100c0, 0xc300c2},
    {90, 546, 0x407f047f, 0x3c3c0054, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x5a003f, 0xc100c0, 0xc300c2},
    {91, 547, 0x407f047f, 0x3d3d0055, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x5b003f, 0xc100c0, 0xc300c2},
    {92, 560, 0x407f047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x5c0040, 0xc100c0, 0xc300c2},
    {93, 561, 0x407f047f, 0x3e3e465b, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x5d0040, 0xc100c0, 0xc300c2},
    {96, 576, 0x407f047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x600041, 0xc100c0, 0xc300c2},
    {98, 592, 0x407f047f, 0x7f004667, 0x0, 0xb2b17f7f, 0xdfefaaff, 0x620042, 0xc100c0, 0xc300c2},
    {0, 593, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x42, 0xc100c0, 0xc300c2},
    {0, 594, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x42, 0xc100c0, 0xc300c2},
    {0, 595, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x42, 0xc100c0, 0xc300c2},
    {102, 608, 0x4064047f, 0x3c3c4659, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x660043, 0xc100c0, 0xc300c2},
    {103, 609, 0x4064047f, 0x3d3d465a, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x670043, 0xc100c0, 0xc300c2},
    {104, 610, 0x4064047f, 0x3e3e465b, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x680043, 0xc100c0, 0xc300c2},
    {105, 611, 0x4064047f, 0x3f3f465c, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x690043, 0xc100c0, 0xc300c2},
    {108, 612, 0x4064047f, 0x40404660, 0x0, 0xb2b17f7f, 0x1fc080ff, 0x6c0043, 0xc100c0, 0xc300c2},
    {109, 613, 0x4064047f, 0x41414661, 0x0, 0xb2b17f7f, 0xdfe080ff, 0x6d0043, 0xc100c0, 0xc300c2},
    {0, 614, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x43, 0xc100c0, 0xc300c2},
    {0, 615, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x43, 0xc100c0, 0xc300c2},
    {0, 616, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x43, 0xc100c0, 0xc300c2},
    {0, 617, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x43, 0xc100c0, 0xc300c2},
    {0, 618, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x43, 0xc100c0, 0xc300c2},
    {0, 619, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x43, 0xc100c0, 0xc300c2},
    {112, 624, 0x7f0404, 0x5050046c, 0x0, 0xb2b10000, 0xdfeb80ff, 0x700046, 0xc100c0, 0xc300c2},
    {112, 625, 0x7f7f0404, 0x5151046d, 0x0, 0xb2b10000, 0xdfeb80ff, 0x700046, 0xc100c0, 0xc300c2},
    {0, 626, 0x0, 0x0, 0x0, 0xb2b10000, 0x5fc080ff, 0x46, 0xc100c0, 0xc300c2},
    {113, 640, 0x407f005a, 0x2f00003c, 0x0, 0xb2b10000, 0xdfe680ff, 0x71004a, 0xc100c0, 0xc300c2},
    {114, 641, 0x407f045a, 0x36300048, 0x0, 0xb2b10000, 0xdfe680ff, 0x72004a, 0xc100c0, 0xc300c2},
    {37, 656, 0x407f0454, 0x3f3f004b, 0x0, 0xb2b10000, 0x5feb80ff, 0x25004d, 0xc100c0, 0xc300c2},
    {38, 657, 0x407f005a, 0x3b00003c, 0x0, 0xb2b10000, 0xdfeb80ff, 0x26004d, 0xc100c0, 0xc300c2},
    {99, 658, 0x406e0055, 0x3c3c0048, 0x0, 0xb2b10000, 0xdfeb80ff, 0x63004d, 0xc100c0, 0xc300c2},
    {42, 672, 0x407f0478, 0x47005a49, 0x0, 0xb2b10000, 0xdff0a0ff, 0x2a004e, 0xc100c0, 0xc300c2},
    {43, 673, 0x407f0478, 0x7f48005d, 0x0, 0xb2b10000, 0xdfeb80ff, 0x2b004e, 0xc100c0, 0xc300c2},
    {44, 688, 0x407f0455, 0x7f250048, 0x0, 0xb2b10000, 0xdfeb80ff, 0x2c004f, 0xc100c0, 0xc300c2}}};

}