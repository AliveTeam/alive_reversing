#include "stdafx_ao.h"
#include "Function.hpp"
#include "HintFly.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "PsxDisplay.hpp"

void HintFly_ForceLink() {}

namespace AO {

const char* gHintFlyMessages_4C6A10[] =
{
    "SNEAK TO BOMB",
    "ABE WAS HERE",
    "TEST THREE",
    "LEARN THE SECRET SONG",
    "BRAVE THE RING",
    "SNEAK IN AND OUT OF SHADOWS",
    "SEEK A ROCK",
    "SLOG GOES BOOM",
    "WATCH YOUR STEP",
    "FIND PLATFORM FOR ELUM",
    "DROP IN",
    "WATCH OUT FOR THAT BAT",
    "POWER WHISPERS FROM SHADOWS",
    "RESCUE LOST SOULS",
    "YOU CAN HOIST WHERE YOU SEE FALLING DEBRIS",
    "ALMOST HOME",
    "SNEAK UP BEHIND SLIG",
    "BLOW BOMBS WITH SPIRIT RINGS",
    "QUEST FOR SPIRIT RINGS",
    "THESE LIGHTS WILL GUIDE YOU",
    "TOUCH THE STONE",
    "FIND BELLSONG THEN POSSESS BELLS",
    "LEAP OF FAITH",
    "YOU WILL RETURN",
    "STEP LIGHTLY",
    "RETURN TO RUPTURE FARMS",
    "FAREWELL ELUM",
    "DO NOT FORGET YOUR ROCK",
    "ELUM WAIT HERE",
    "RUN ELUM RUN",
    "ELUMS LIKE HONEY BUT NOT BEES",
    "SNEAK BY SLIG",
    "WALK SOFTLY",
    "SCOUT FROM AFAR",
    "DO NOT FORGET THE FLINTLOCK",
    "THEY WONT CHASE WHAT THEY CANT SEE",
    nullptr
};

struct HintFlyParticle
{
    FP field_0_xpos;
    FP field_4_ypos;
    char field_8_state;
    char field_9_pad;
    __int16 field_A;
    FP field_C_velx;
    FP field_10_vely;
    FP field_14_targetX;
    FP field_18_targetY;
    __int16 field_1C_sound_pitch;
    __int16 field_1E_sound_pitch_speed;
    char field_20_angle;
    char field_21_angle_speed;
    char field_22_timer;
    char field_23_pad;
    Prim_Sprt field_24_sprt[2];
};
ALIVE_ASSERT_SIZEOF(HintFlyParticle, 0x54);

const BYTE byte_4C6DB0[20] =
{
    19u,
    15u,
    29u,
    43u,
    57u,
    71u,
    75u,
    85u,
    99u,
    107u,
    113u,
    131u,
    139u,
    149u,
    167u,
    171u,
    185u,
    203u,
    221u,
    239u
};

const BYTE byte_4C6DC8[32] =
{
    31u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    40u,
    49u,
    63u,
    72u,
    81u,
    95u,
    104u,
    113u,
    127u,
    136u,
    145u,
    159u,
    168u,
    177u,
    191u,
    199u,
    201u,
    210u,
    222u,
    228u,
    230u,
    234u,
    236u
};

const BYTE byte_4C6DE8[17] =
{
    16u,
    21u,
    23u,
    25u,
    27u,
    35u,
    45u,
    65u,
    78u,
    96u,
    111u,
    144u,
    159u,
    177u,
    190u,
    211u,
    221u,
};

const BYTE byte_4C6E00[26] =
{
    25u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    49u,
    63u,
    81u,
    95u,
    113u,
    127u,
    145u,
    159u,
    177u,
    191u,
    210u,
    222u,
    228u,
    230u,
    232u,
    234u,
    236u,
};

const BYTE byte_4C6E20[27] =
{
    26u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    40u,
    49u,
    63u,
    72u,
    81u,
    95u,
    104u,
    113u,
    127u,
    136u,
    145u,
    159u,
    168u,
    177u,
    191u,
    200u,
    209u,
    223u,
};

const BYTE byte_4C6E40[21] =
{
    20u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    40u,
    49u,
    72u,
    81u,
    104u,
    113u,
    136u,
    145u,
    168u,
    177u,
    200u,
    209u,
};

const BYTE byte_4C6E58[22] =
{
    21u,
    21u,
    23u,
    25u,
    27u,
    35u,
    45u,
    65u,
    78u,
    96u,
    111u,
    121u,
    144u,
    153u,
    159u,
    177u,
    185u,
    190u,
    211u,
    217u,
    219u,
    221u,
};

const BYTE byte_4C6E70[23] =
{
    22u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    40u,
    72u,
    104u,
    136u,
    168u,
    200u,
    209u,
    211u,
    213u,
    215u,
    217u,
    219u,
    221u,
    223u,
};

const BYTE byte_4C6E88[21] =
{
    20u,
    17u,
    31u,
    49u,
    63u,
    81u,
    95u,
    113u,
    115u,
    117u,
    119u,
    121u,
    123u,
    125u,
    127u,
    145u,
    159u,
    177u,
    191u,
    209u,
    223u,
};

const BYTE byte_4C6EA0[15] =
{
    14u,
    27u,
    29u,
    63u,
    95u,
    127u,
    159u,
    191u,
    209u,
    211u,
    213u,
    215u,
    217u,
    219u,
    221u,
};

const BYTE byte_4C6EB0[23] =
{
    22u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    55u,
    57u,
    70u,
    90u,
    101u,
    123u,
    132u,
    156u,
    163u,
    189u,
    194u,
    222u,
    225u,
    255u,
};

const BYTE byte_4C6EC8[16] =
{
    14u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    63u,
    95u,
    127u,
    159u,
    191u,
    223u,
};

const BYTE byte_4C6ED8[25] =
{
    24u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    51u,
    69u,
    87u,
    119u,
    121u,
    151u,
    165u,
    179u,
    209u,
    211u,
    213u,
    215u,
    217u,
    219u,
    221u,
    223u,
};

const BYTE byte_4C6EF8[23] =
{
    22u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    51u,
    85u,
    103u,
    137u,
    171u,
    189u,
    209u,
    211u,
    213u,
    215u,
    217u,
    219u,
    221u,
    223u,
};

const BYTE byte_4C6F10[21] =
{
    20u,
    21u,
    23u,
    25u,
    27u,
    35u,
    45u,
    65u,
    78u,
    96u,
    111u,
    144u,
    159u,
    177u,
    190u,
    211u,
    221u,
    229u,
    231u,
    233u,
    235u,
};

const BYTE byte_4C6F28[23] =
{
    22u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    40u,
    49u,
    72u,
    81u,
    104u,
    113u,
    136u,
    145u,
    168u,
    177u,
    199u,
    210u,
    228u,
    230u,
};

const BYTE byte_4C6F40[24] =
{
    23u,
    21u,
    23u,
    25u,
    27u,
    35u,
    45u,
    65u,
    78u,
    96u,
    111u,
    144u,
    159u,
    170u,
    177u,
    188u,
    190u,
    211u,
    221u,
    229u,
    231u,
    233u,
    235u,
    239u
};

const BYTE byte_4C6F58[27] =
{
    26u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    40u,
    49u,
    72u,
    81u,
    104u,
    113u,
    136u,
    145u,
    168u,
    177u,
    199u,
    201u,
    210u,
    219u,
    228u,
    230u,
    237u,
    255u,
};

const BYTE byte_4C6F78[21] =
{
    20u,
    21u,
    35u,
    39u,
    45u,
    65u,
    72u,
    78u,
    96u,
    104u,
    111u,
    136u,
    144u,
    159u,
    168u,
    177u,
    190u,
    201u,
    211u,
    219u,
    221u,
};

const BYTE byte_4C6F90[15] =
{
    14u,
    17u,
    49u,
    81u,
    113u,
    115u,
    117u,
    119u,
    121u,
    123u,
    125u,
    127u,
    145u,
    177u,
    209u,
};

const BYTE byte_4C6FA0[20] =
{
    19u,
    17u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    63u,
    95u,
    127u,
    159u,
    191u,
    209u,
    211u,
    213u,
    215u,
    217u,
    219u,
    221u,
};

const BYTE byte_4C6FB8[16] =
{
    15u,
    17u,
    35u,
    53u,
    71u,
    89u,
    107u,
    109u,
    127u,
    139u,
    141u,
    153u,
    167u,
    181u,
    195u,
    209u
};

const BYTE byte_4C6FC8[25] =
{
    24u,
    16u,
    18u,
    20u,
    22u,
    24u,
    26u,
    28u,
    30u,
    60u,
    74u,
    88u,
    118u,
    120u,
    152u,
    170u,
    188u,
    208u,
    210u,
    212u,
    214u,
    216u,
    218u,
    220u,
    222u,
};

const BYTE byte_4C6FE8[17] =
{
    16u,
    17u,
    31u,
    51u,
    61u,
    85u,
    91u,
    119u,
    121u,
    151u,
    153u,
    181u,
    187u,
    211u,
    221u,
    225u,
    255u,
};

const BYTE byte_4C7000[13] =
{
    12u,
    17u,
    51u,
    85u,
    119u,
    137u,
    139u,
    141u,
    143u,
    151u,
    181u,
    211u,
    225u,
};

const BYTE byte_4C7010[22] =
{
    21u,
    17u,
    31u,
    45u,
    49u,
    63u,
    76u,
    81u,
    95u,
    106u,
    113u,
    127u,
    136u,
    145u,
    159u,
    166u,
    177u,
    191u,
    196u,
    209u,
    211u,
    223u,
};

const BYTE byte_4C7028[23] =
{
    22u,
    15u,
    29u,
    43u,
    57u,
    64u,
    71u,
    75u,
    80u,
    85u,
    99u,
    107u,
    131u,
    139u,
    144u,
    149u,
    160u,
    167u,
    171u,
    185u,
    203u,
    221u,
    239u,
};

const BYTE byte_4C7040[29] =
{
    28u,
    15u,
    29u,
    40u,
    43u,
    57u,
    70u,
    72u,
    81u,
    84u,
    98u,
    104u,
    113u,
    115u,
    117u,
    119u,
    121u,
    123u,
    125u,
    127u,
    136u,
    145u,
    159u,
    168u,
    177u,
    191u,
    200u,
    209u,
    223u,
};

const BYTE byte_4C7060[21] =
{
    20u,
    15u,
    29u,
    43u,
    57u,
    71u,
    75u,
    80u,
    85u,
    97u,
    107u,
    114u,
    116u,
    139u,
    149u,
    167u,
    171u,
    185u,
    203u,
    221u,
    239u,
};

const BYTE byte_4C7078[21] =
{
    20u,
    15u,
    29u,
    43u,
    57u,
    71u,
    75u,
    85u,
    107u,
    116u,
    130u,
    139u,
    145u,
    149u,
    160u,
    167u,
    171u,
    185u,
    203u,
    221u,
    239u,
};

const BYTE byte_4C7090[21] =
{
    20u,
    15u,
    29u,
    43u,
    57u,
    71u,
    75u,
    81u,
    85u,
    107u,
    112u,
    116u,
    139u,
    145u,
    149u,
    167u,
    171u,
    185u,
    203u,
    221u,
    239u,
};

const BYTE byte_4C70A8[28] =
{
    27u,
    21u,
    23u,
    25u,
    27u,
    35u,
    45u,
    62u,
    65u,
    95u,
    96u,
    113u,
    115u,
    117u,
    119u,
    121u,
    123u,
    125u,
    127u,
    136u,
    144u,
    159u,
    168u,
    176u,
    191u,
    200u,
    208u,
    223u,
};

const BYTE byte_4C70C8[20] =
{
    19u,
    21u,
    23u,
    25u,
    35u,
    43u,
    65u,
    76u,
    95u,
    96u,
    108u,
    125u,
    127u,
    142u,
    144u,
    156u,
    177u,
    188u,
    211u,
    218u,
};

const BYTE byte_4C70E0[28] =
{
    27u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    51u,
    57u,
    63u,
    83u,
    89u,
    95u,
    96u,
    113u,
    115u,
    121u,
    127u,
    130u,
    147u,
    153u,
    159u,
    179u,
    185u,
    191u,
    211u,
    223u,
};

const BYTE byte_4C7100[28] =
{
    27u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    51u,
    57u,
    63u,
    83u,
    89u,
    95u,
    98u,
    113u,
    115u,
    121u,
    127u,
    128u,
    147u,
    153u,
    159u,
    179u,
    185u,
    191u,
    211u,
    223u,
};

const BYTE byte_4C7120[28] =
{
    27u,
    19u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    51u,
    57u,
    63u,
    81u,
    83u,
    89u,
    95u,
    112u,
    115u,
    121u,
    127u,
    145u,
    147u,
    153u,
    159u,
    179u,
    185u,
    191u,
    211u,
    223u,
};

const BYTE byte_4C7140[24] =
{
    23u,
    19u,
    31u,
    51u,
    63u,
    64u,
    80u,
    83u,
    95u,
    115u,
    117u,
    119u,
    121u,
    123u,
    125u,
    127u,
    144u,
    147u,
    159u,
    160u,
    179u,
    191u,
    211u,
    223u
};

const BYTE byte_4C7158[23] =
{
    22u,
    19u,
    31u,
    51u,
    63u,
    83u,
    95u,
    98u,
    113u,
    115u,
    117u,
    119u,
    121u,
    123u,
    125u,
    127u,
    128u,
    147u,
    159u,
    179u,
    191u,
    211u,
    223u
};

const BYTE byte_4C7170[23] =
{
    22u,
    19u,
    31u,
    51u,
    63u,
    81u,
    83u,
    95u,
    112u,
    115u,
    117u,
    119u,
    121u,
    123u,
    125u,
    127u,
    145u,
    147u,
    159u,
    179u,
    191u,
    211u,
    223u,
};

const BYTE byte_4C7188[24] =
{
    23u,
    21u,
    23u,
    25u,
    27u,
    29u,
    31u,
    49u,
    53u,
    71u,
    80u,
    105u,
    113u,
    139u,
    146u,
    157u,
    177u,
    191u,
    213u,
    215u,
    217u,
    219u,
    221u,
    223u
};

const BYTE byte_4C71A0[21] =
{
    20u,
    24u,
    27u,
    37u,
    45u,
    64u,
    67u,
    78u,
    80u,
    98u,
    111u,
    146u,
    159u,
    160u,
    176u,
    179u,
    190u,
    213u,
    221u,
    232u,
    235u,
};

const BYTE byte_4C71B8[19] =
{
    18u,
    24u,
    27u,
    38u,
    45u,
    68u,
    78u,
    80u,
    97u,
    111u,
    114u,
    131u,
    159u,
    180u,
    190u,
    214u,
    221u,
    232u,
    235u,
};

const BYTE byte_4C71D0[19] =
{
    18u,
    24u,
    27u,
    38u,
    45u,
    68u,
    78u,
    111u,
    115u,
    130u,
    145u,
    159u,
    160u,
    164u,
    190u,
    214u,
    221u,
    232u,
    235u,
};

const BYTE byte_4C71E8[19] =
{
    18u,
    24u,
    27u,
    38u,
    45u,
    68u,
    78u,
    81u,
    111u,
    112u,
    115u,
    145u,
    159u,
    180u,
    190u,
    214u,
    221u,
    233u,
    235u,
};

const BYTE byte_4C7200[22] =
{
    21u,
    35u,
    37u,
    39u,
    41u,
    43u,
    45u,
    63u,
    64u,
    80u,
    95u,
    127u,
    144u,
    159u,
    160u,
    191u,
    195u,
    197u,
    199u,
    201u,
    203u,
    205u,
};

const BYTE byte_4C7218[19] =
{
    18u,
    37u,
    39u,
    41u,
    43u,
    45u,
    63u,
    81u,
    95u,
    98u,
    115u,
    127u,
    159u,
    191u,
    197u,
    199u,
    201u,
    203u,
    205u,
};

const BYTE byte_4C7230[19] =
{
    18u,
    37u,
    39u,
    41u,
    43u,
    45u,
    63u,
    95u,
    115u,
    127u,
    130u,
    145u,
    159u,
    191u,
    197u,
    199u,
    201u,
    203u,
    205u,
};

const BYTE byte_4C7248[19] =
{
    18u,
    37u,
    39u,
    41u,
    43u,
    45u,
    63u,
    81u,
    95u,
    112u,
    127u,
    145u,
    159u,
    191u,
    197u,
    199u,
    201u,
    203u,
    205u,
};

const BYTE byte_4C725C[3] = { 2u, 113u, 115u };

const BYTE byte_4C7260[7] = { 6u, 42u, 74u, 106u, 138u, 170u, 202u };


const BYTE* pData_4C7268[] =
{
    byte_4C6DB0,
    byte_4C6DC8,
    byte_4C6DE8,
    byte_4C6E00,
    byte_4C6E20,
    byte_4C6E40,
    byte_4C6E58,
    byte_4C6E70,
    byte_4C6E88,
    byte_4C6EA0,
    byte_4C6EB0,
    byte_4C6EC8,
    byte_4C6ED8,
    byte_4C6EF8,
    byte_4C6F10,
    byte_4C6F28,
    byte_4C6F40,
    byte_4C6F58,
    byte_4C6F78,
    byte_4C6F90,
    byte_4C6FA0,
    byte_4C6FB8,
    byte_4C6FC8,
    byte_4C6FE8,
    byte_4C7000,
    byte_4C7010,
};

ALIVE_VAR(1, 0x4C6AA4, BYTE, sHintFlyRndSeed_4C6AA4, 37);

static BYTE HintFly_NextRandom()
{
    return sRandomBytes_4BBE30[sHintFlyRndSeed_4C6AA4++];
}

HintFly* HintFly::ctor_42A820(Path_HintFly* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BB200);
    field_E4_ppRes = nullptr;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 62, 1, 0);
    if (ppRes)
    {
        Animation_Init_417FD0(556, 10, 7, ppRes, 1);

        field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_124_tlvInfo = tlvInfo;
        field_11E_msg_idx = 0;

        field_11C_message_id = pTlv->field_18_mesage_id;

        field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
        field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

        const char* pMsg = gHintFlyMessages_4C6A10[pTlv->field_18_mesage_id];

        field_118_counter = 20;
        field_11A_msg_len = 0;

        // Find the length of the longest word (in particles)
        short longestWordLen = 0;
        short curWordLen = 0;
        for(;;)
        {
            // End of word?
            if (*pMsg == ' ' || *pMsg == 0)
            {
                if (curWordLen > longestWordLen)
                {
                    longestWordLen = curWordLen;
                }
                curWordLen = 0;
                if (!*pMsg)
                {
                    break;
                }
            }
            else
            {
                curWordLen += pData_4C7268[(*pMsg) - 'A'][0];
            }
            pMsg++;
        }

        field_11A_msg_len = longestWordLen;
        field_11A_msg_len += 12;

        field_E4_ppRes = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_HintFly, 0, sizeof(HintFlyParticle) * field_11A_msg_len);
        if (field_E4_ppRes)
        {
            field_E8_pRes = reinterpret_cast<HintFlyParticle*>(*field_E4_ppRes);
            field_112_state = State::eState_1;
            field_10C_timer = 0;

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit13_Is8Bit))
            {
                field_110_bitMode = 1;
            }
            else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
            {
                field_110_bitMode = 2;
            }
            else
            {
                field_110_bitMode = 0;
            }

            int vram_x = field_10_anim.field_84_vram_rect.x & 0x3F;
            if (field_110_bitMode == 1)
            {
                vram_x = 2 * vram_x;
            }
            else if (field_110_bitMode == 0)
            {
                vram_x = 4 * vram_x;
            }

            auto pHeader = reinterpret_cast<FrameHeader*>(&(*field_10_anim.field_20_ppBlock)[field_10_anim.Get_FrameHeader_403A00(-1)->field_0_frame_header_offset]);

            for (int i = 0; i < field_11A_msg_len; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    Prim_Sprt* pSprt = &field_E8_pRes[i].field_24_sprt[j];

                    Sprt_Init(pSprt);

                    Poly_Set_SemiTrans_498A40(&pSprt->mBase.header, 1);
                    Poly_Set_Blending_498A00(&pSprt->mBase.header, 1);

                    SetClut(pSprt, static_cast<short>(PSX_getClut_496840(
                        field_10_anim.field_8C_pal_vram_xy.field_0_x,
                        field_10_anim.field_8C_pal_vram_xy.field_2_y)));

                    SetUV0(pSprt, vram_x & 0xFF, field_10_anim.field_84_vram_rect.y & 0xFF);

                    pSprt->field_14_w = pHeader->field_4_width - 1;
                    pSprt->field_16_h = pHeader->field_5_height - 1;
                }
            }


            field_114_xScreen = FP_GetExponent(field_A8_xpos + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x);
            field_116_yScreen = FP_GetExponent(field_AC_ypos + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y);

            const PSX_RECT rect = { static_cast<short>(field_10_anim.field_8C_pal_vram_xy.field_0_x + 1), field_10_anim.field_8C_pal_vram_xy.field_2_y, 1, 1 };
            const BYTE data[] = { 0, 0, 0, 0 };
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
            {
                PSX_LoadImage16_4962A0(&rect, data);
            }
            else
            {
                PSX_LoadImage_496480(&rect, data);
            }

            for (int i = 0; i < field_118_counter; i++)
            {
                HintFlyParticle* pParticle = &field_E8_pRes[i];
                InitParticle(pParticle);

            }
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
    else
    {
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return this;
}

void HintFly::InitParticle(HintFlyParticle* pParticle)
{
    pParticle->field_8_state = 3;
    pParticle->field_0_xpos = FP_FromInteger((HintFly_NextRandom() & 0x1F) + field_114_xScreen - 16);
    pParticle->field_4_ypos = FP_FromInteger((HintFly_NextRandom() & 0x1F) + field_116_yScreen - 16);
    pParticle->field_20_angle = HintFly_NextRandom();
    pParticle->field_21_angle_speed = (HintFly_NextRandom() % 4) + 12;
    if (HintFly_NextRandom() % 2)
    {
        pParticle->field_21_angle_speed = -pParticle->field_21_angle_speed;
    }
}

BaseGameObject* HintFly::VDestructor(signed int flags)
{
    return Vdtor_42BCF0(flags);
}

HintFly* HintFly::Vdtor_42BCF0(signed int flags)
{
    dtor_42ADF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void HintFly::VScreenChanged()
{
    VScreenChanged_42BCE0();
}

void HintFly::VScreenChanged_42BCE0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

BaseGameObject* HintFly::dtor_42ADF0()
{
    SetVTable(this, 0x4BB200);
    if (field_E4_ppRes)
    {
        ResourceManager::FreeResource_455550(field_E4_ppRes);
    }
    gMap_507BA8.TLV_Reset_446870(field_124_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

void HintFly::FormWordAndAdvanceToNextWord_42AF90()
{
    const char* msgPtr = &gHintFlyMessages_4C6A10[field_11C_message_id][field_11E_msg_idx];
    LOG_INFO("Word is " << msgPtr);

    // Find how long the word is
    short letterCount = 0;
    const char* pEndWord = msgPtr;
    for (; *pEndWord != ' '; pEndWord++)
    {
        if (!*pEndWord)
        {
            break;
        }
        letterCount++;
    }

    // Next time start at the next word
    field_11E_msg_idx += letterCount;

    // Also skip the space if we had one else display will be screwed
    if (*pEndWord == ' ')
    {
        field_11E_msg_idx++;
    }

    const int xBase = field_114_xScreen - (16 * letterCount) / 2;
    const int yBase = field_116_yScreen - 8;
   
    FP xBaseFP = FP_FromInteger(xBase);
    const FP yBaseFP = FP_FromInteger(yBase);

    int particleIdx = 0;
    for (int i = 0; i < letterCount; i++)
    {
        const auto pArray = pData_4C7268[msgPtr[i] - 'A'];
        // First element is the count of "pixels" that make up a word
        const int total = pArray[0];
        for (int j = 0; j < total; j++)
        {
            // Position each "pixel" of the word
            HintFlyParticle* pParticleIter = &field_E8_pRes[particleIdx++];

            auto xVal = FP_FromInteger(pArray[j+1] >> 4);
            auto yVal = FP_FromInteger((pArray[j+1]) & 0xF);

            pParticleIter->field_14_targetX = (FP_FromDouble(0.8) * xVal) + xBaseFP;
            pParticleIter->field_18_targetY = (FP_FromDouble(0.8) * yVal) + yBaseFP;
            pParticleIter->field_C_velx =  FP_FromRaw((pParticleIter->field_14_targetX.fpValue - pParticleIter->field_0_xpos.fpValue) >> 4);
            pParticleIter->field_10_vely = FP_FromRaw((pParticleIter->field_18_targetY.fpValue - pParticleIter->field_4_ypos.fpValue) >> 4);
            pParticleIter->field_8_state = 1;

            pParticleIter->field_1C_sound_pitch = Math_RandomRange_450F20(-127, 127);
            pParticleIter->field_1E_sound_pitch_speed = pParticleIter->field_1C_sound_pitch >> 4;
        }
        xBaseFP += FP_FromInteger(16);
    }

    while (particleIdx < field_118_counter)
    {
        field_E8_pRes[particleIdx++].field_8_state = 3;
    }
}

void HintFly::UpdateParticles_42B1B0()
{
    for (int i = 0; i < field_118_counter; i++)
    {
        HintFlyParticle* pParticle = &field_E8_pRes[i];
        if (pParticle->field_8_state == 1)
        {
            const FP xTargetDelta = FP_Abs(pParticle->field_14_targetX - pParticle->field_0_xpos);
            const FP yTargetDelta = FP_Abs(pParticle->field_18_targetY - pParticle->field_4_ypos);

            if (xTargetDelta >= FP_FromInteger(1) || yTargetDelta >= FP_FromInteger(1))
            {
                pParticle->field_0_xpos += pParticle->field_C_velx;
                pParticle->field_4_ypos += pParticle->field_10_vely;
                pParticle->field_1C_sound_pitch += pParticle->field_1E_sound_pitch_speed;
            }
            else
            {
                pParticle->field_8_state = 2;
                pParticle->field_1C_sound_pitch = 0;
            }

            if (i < 1 && Math_RandomRange_450F20(0, 100) < 40)
            {
                SFX_Play_43AE60(SoundEffect::HintFly_60, Math_RandomRange_450F20(24, 30), pParticle->field_1C_sound_pitch, 0);
            }
        }
        else if (pParticle->field_8_state == 2)
        {
            if (!(HintFly_NextRandom() & 3))
            {
                pParticle->field_0_xpos = pParticle->field_14_targetX + FP_FromInteger(abs(HintFly_NextRandom()) & 1);
            }

            if (i < 1 && Math_RandomRange_450F20(0, 100) < 40)
            {
                SFX_Play_43AE60(SoundEffect::HintFly_60, Math_RandomRange_450F20(24, 30), 0, 0);
            }
        }
        else if (pParticle->field_8_state == 3)
        {
            pParticle->field_20_angle = pParticle->field_20_angle + pParticle->field_21_angle_speed;
            pParticle->field_0_xpos += (Math_Sine_451110(pParticle->field_20_angle) * FP_FromInteger(5));
            pParticle->field_4_ypos += (Math_Cosine_4510A0(pParticle->field_20_angle) * FP_FromInteger(2));
            if (i < 1 && Math_RandomRange_450F20(0, 100) < 20)
            {
                SFX_Play_43AD70(SoundEffect::HintFly_60, Math_RandomRange_450F20(18, 24), 0);
            }
        }
    }
}


void HintFly::VUpdate()
{
    VUpdate_42B3D0();
}

void HintFly::VUpdate_42B3D0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    switch (field_112_state)
    {
    case State::eState_1:
        UpdateParticles_42B1B0();

        if (Event_Get_417250(kEventAbeOhm_8))
        {
            field_11E_msg_idx = 0;
            field_112_state = State::eState_3;
            field_10C_timer = gnFrameCount_507670 + 15;
        }
        return;

    case State::eState_2:
        if (field_118_counter == field_122_target_count)
        {
            UpdateParticles_42B1B0();

            if (!Event_Get_417250(kEventAbeOhm_8))
            {
                for (int i = 0; i < field_118_counter; i++)
                {
                    if (field_E8_pRes[i].field_8_state == 3)
                    {
                        break;
                    }
                    field_E8_pRes[i].field_8_state = 3;
                }
                field_120_idx = 0;
                field_112_state = State::eState_5;
                return;
            }

            if (field_118_counter == field_122_target_count)
            {
                FormWordAndAdvanceToNextWord_42AF90();
                field_112_state = State::eState_4;
                field_10C_timer = gnFrameCount_507670 + 30;
            }
        }

        for (int i=0; i < 8; i++)
        {
            if (field_118_counter < field_122_target_count)
            {
                InitParticle(&field_E8_pRes[field_118_counter++]);
            }
            else
            {
                field_118_counter--;
                if (field_120_idx < field_118_counter)
                {

                    InitParticle(&field_E8_pRes[field_120_idx++]);
                }
            }

            if (field_118_counter == field_122_target_count)
            {
                break;
            }
        }

        // TODO: This block is duplicated above
        UpdateParticles_42B1B0();

        if (!Event_Get_417250(kEventAbeOhm_8))
        {
            for (int i = 0; i < field_118_counter; i++)
            {
                if (field_E8_pRes[i].field_8_state == 3)
                {
                    break;
                }
                field_E8_pRes[i].field_8_state = 3;
            }
            field_120_idx = 0;
            field_112_state = State::eState_5;
            return;
        }

        if (field_118_counter == field_122_target_count)
        {
            FormWordAndAdvanceToNextWord_42AF90();
            field_112_state = State::eState_4;
            field_10C_timer = gnFrameCount_507670 + 30;
        }
        break;

    case State::eState_3:
        UpdateParticles_42B1B0();

        if (!Event_Get_417250(kEventAbeOhm_8))
        {
            for (int i = 0; i < field_118_counter; i++)
            {
                if (field_E8_pRes[i].field_8_state == 3)
                {
                    break;
                }
                field_E8_pRes[i].field_8_state = 3;
            }
            field_120_idx = 0;
            field_112_state = State::eState_5;
            return;
        }

        if (static_cast<int>(gnFrameCount_507670) > field_10C_timer)
        {
            __int16 len = 0;
            const char* pMsgIter = gHintFlyMessages_4C6A10[field_11C_message_id] + field_11E_msg_idx;
            while (*pMsgIter != ' ' && *pMsgIter != '\0')
            {
                len += pData_4C7268[(*pMsgIter) - 'A'][0];
                pMsgIter++;
            }
            field_120_idx = 0;
            field_122_target_count = len + 12;
            field_112_state = State::eState_2;
        }
        return;

    case State::eState_4:
        UpdateParticles_42B1B0();

        if (Event_Get_417250(kEventAbeOhm_8))
        {
            if (static_cast<int>(gnFrameCount_507670) > field_10C_timer)
            {
                for (int i = 0; i < field_118_counter; i++)
                {
                    if (field_E8_pRes[i].field_8_state == 3)
                    {
                        break;
                    }
                    field_E8_pRes[i].field_8_state = 3;
                }
                field_112_state = State::eState_3;
                field_10C_timer = gnFrameCount_507670 + 15;
            }
        }
        else
        {
            for (int i = 0; i < field_118_counter; i++)
            {
                if (field_E8_pRes[i].field_8_state == 3)
                {
                    break;
                }
                field_E8_pRes[i].field_8_state = 3;
            }
            field_120_idx = 0;
            field_112_state = State::eState_5;
        }
        return;

    case State::eState_5:
        if (field_118_counter == 20)
        {
            if (Event_Get_417250(kEventAbeOhm_8))
            {
                field_112_state = State::eState_1;
            }
        }
        // Fall through

    case State::eState_6:
        UpdateParticles_42B1B0();
        if (field_120_idx >= 20)
        {
            field_118_counter -= 8;
            if (field_118_counter < 20)
            {
                field_118_counter = 20;
            }
        }
        else
        {
            InitParticle(&field_E8_pRes[field_120_idx++]);
        }

        if (field_118_counter == 20 && !Event_Get_417250(kEventAbeOhm_8))
        {
            field_112_state = State::eState_1;
        }
        return;

    default:
        return;
    }
}

void HintFly::VRender(int** pOrderingTable)
{
    VRender_42BAD0(pOrderingTable);
}

void HintFly::VRender_42BAD0(int** ppOt)
{
    Prim_SetTPage* pTPage = &field_EC_tPages[gPsxDisplay_504C78.field_A_buffer_index];

    PSX_RECT rect = {};
    rect.x = -32768;
    rect.w = -32767;
    rect.y = -32768;
    rect.h = -32767;

    for (int i = 0; i < field_118_counter; i++)
    {
        HintFlyParticle* pParticle = &field_E8_pRes[i];
        Prim_Sprt* pSprt = &pParticle->field_24_sprt[gPsxDisplay_504C78.field_A_buffer_index];

        const short flyX = FP_GetExponent(PsxToPCX(pParticle->field_0_xpos, FP_FromInteger(11)));
        const short flyY = FP_GetExponent(pParticle->field_4_ypos);

        SetXY0(pSprt, flyX, flyY);

        OrderingTable_Add_498A80(&ppOt[39], &pSprt->mBase.header);

        if (flyX < rect.x)
        {
            rect.x = flyX;
        }

        if (flyX > rect.w)
        {
            rect.w = flyX;
        }

        if (flyY < rect.y)
        {
            rect.y = flyY;
        }

        if (flyY > rect.h)
        {
            rect.h = flyY;
        }
    }

    short tPageY = 256;
    if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit10_alternating_flag) && field_10_anim.field_84_vram_rect.y < 256u)
    {
        tPageY = 0;
    }

    const int tpage = PSX_getTPage_4965D0(static_cast<char>(field_110_bitMode), 1, field_10_anim.field_84_vram_rect.x & 0xFFC0, tPageY);

    Init_SetTPage_495FB0(pTPage, 0, 0, tpage);
    OrderingTable_Add_498A80(&ppOt[39], &pTPage->mBase);

    pScreenManager_4FF7C8->InvalidateRect_406E40(
        rect.x - 6,
        rect.y - 6,
        rect.w + 9,
        rect.h + 6,
        pScreenManager_4FF7C8->field_2E_idx);
}

}
