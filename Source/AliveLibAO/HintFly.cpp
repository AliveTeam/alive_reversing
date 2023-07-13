#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "HintFly.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/PathDataExtensionsTypes.hpp"
#include "PathData.hpp"
#include "Path.hpp"
#include "Map.hpp"

namespace AO {

static const char_type* sHintFlyMessages[36] = {
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
    "THEY WONT CHASE WHAT THEY CANT SEE"};


static const StringTable* sPerLvlMessages[static_cast<u32>(LevelIds::eDesertEscape_15) + 1][99] = {};

void SetHintFlyMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId)
{
    sPerLvlMessages[static_cast<u32>(lvl)][pathId] = &msgs;
}

class HintFlyMessages final
{
public:
    const char_type* GetMessage(EReliveLevelIds lvlId, u32 pathId, u32 msgId) const
    {
        const StringTable* pTable = sPerLvlMessages[static_cast<u32>(MapWrapper::ToAO(lvlId))][pathId];
        if (pTable)
        {
            if (msgId < pTable->mStringCount)
            {
                return pTable->mStrings[msgId].string_ptr;
            }
            else
            {
                LOG_WARNING("HintFly message out of bounds, using original game message for id: %d", msgId);
            }
        }

        if (msgId < ALIVE_COUNTOF(sHintFlyMessages))
        {
            return sHintFlyMessages[msgId];
        }
        else
        {
            LOG_WARNING("HintFly message out of bounds using original message table id: %d", msgId);
            return sHintFlyMessages[0];
        }
    }
};
static HintFlyMessages gHintFlyMessages;

struct HintFlyParticle final
{
    FP mXPos;
    FP mYPos;
    s8 mState;
    FP mVelX;
    FP mVelY;
    FP mTargetX;
    FP mTargetY;
    s16 mSoundPitch;
    s16 mSoundPitchSpeed;
    s8 mAngle;
    s8 mAngleSpeed;
    Poly_FT4 mSprt[2];
};

static const u8 sHintFlyLetter_A[20] = {
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
    239u};

static const u8 sHintFlyLetter_B[32] = {
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
    236u};

static const u8 sHintFlyLetter_C[17] = {
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

static const u8 sHintFlyLetter_D[26] = {
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

static const u8 sHintFlyLetter_E[27] = {
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

static const u8 sHintFlyLetter_F[21] = {
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

static const u8 sHintFlyLetter_G[22] = {
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

static const u8 sHintFlyLetter_H[23] = {
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

static const u8 sHintFlyLetter_I[21] = {
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

static const u8 sHintFlyLetter_J[15] = {
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

static const u8 sHintFlyLetter_K[23] = {
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

static const u8 sHintFlyLetter_L[16] = {
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

static const u8 sHintFlyLetter_M[25] = {
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

static const u8 sHintFlyLetter_N[23] = {
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

static const u8 sHintFlyLetter_O[21] = {
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

static const u8 sHintFlyLetter_P[23] = {
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

static const u8 sHintFlyLetter_Q[24] = {
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
    239u};

static const u8 sHintFlyLetter_R[27] = {
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

static const u8 sHintFlyLetter_S[21] = {
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

static const u8 sHintFlyLetter_T[15] = {
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

static const u8 sHintFlyLetter_U[20] = {
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

static const u8 sHintFlyLetter_V[16] = {
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
    209u};

static const u8 sHintFlyLetter_W[25] = {
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

static const u8 sHintFlyLetter_X[17] = {
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

static const u8 sHintFlyLetter_Y[13] = {
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

static const u8 sHintFlyLetter_Z[22] = {
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

[[maybe_unused]] const u8 HintFlyLetter_A_diaeresis_4C7028[23] = {
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

// the letter that looks like AE
[[maybe_unused]] const u8 HintFlyLetter_ash_4C7040[29] = {
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

[[maybe_unused]] const u8 HintFlyLetter_A_grave_4C7060[21] = {
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

[[maybe_unused]] const u8 HintFlyLetter_A_acute_4C7078[21] = {
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

[[maybe_unused]] const u8 HintFlyLetter_A_circumflex_4C7090[21] = {
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

// something that looks like euro ?
[[maybe_unused]] const u8 byte_4C70A8[28] = {
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

[[maybe_unused]] const u8 HintFlyLetter_C_cedilla_4C70C8[20] = {
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

[[maybe_unused]] const u8 HintFlyLetter_E_grave_4C70E0[28] = {
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

[[maybe_unused]] const u8 HintFlyLetter_E_acute_4C7100[28] = {
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

[[maybe_unused]] const u8 HintFlyLetter_E_circumflex_4C7120[28] = {
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

// I with two dots??
[[maybe_unused]] const u8 byte_4C7140[24] = {
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
    223u};

// also looks like the one below
[[maybe_unused]] const u8 byte_4C7158[23] = {
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
    223u};

[[maybe_unused]] const u8 HintFlyLetter_I_circumflex_4C7170[23] = {
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

[[maybe_unused]] const u8 HintFlyLetter_N_tilde_4C7188[24] = {
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
    223u};

[[maybe_unused]] const u8 HintFlyLetter_O_diaeresis_4C71A0[21] = {
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

[[maybe_unused]] const u8 HintFlyLetter_O_grave_4C71B8[19] = {
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

[[maybe_unused]] const u8 HintFlyLetter_O_acute_4C71D0[19] = {
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

[[maybe_unused]] const u8 HintFlyLetter_O_circumflex_4C71E8[19] = {
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

// u with two dots at the top?
[[maybe_unused]] const u8 byte_4C7200[22] = {
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

[[maybe_unused]] const u8 HintFlyLetter_U_grave_4C7218[19] = {
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

[[maybe_unused]] const u8 HintFlyLetter_U_acute_4C7230[19] = {
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

[[maybe_unused]] const u8 HintFlyLetter_U_circumflex_4C7248[19] = {
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

[[maybe_unused]] const u8 HintFlyLetters_dot_4C725C[3] = {2u, 113u, 115u};

[[maybe_unused]] const u8 HintFlyLetter_underscore_4C7260[7] = {6u, 42u, 74u, 106u, 138u, 170u, 202u};


const u8* pHintFlyAlphabet_4C7268[] = {
    sHintFlyLetter_A,
    sHintFlyLetter_B,
    sHintFlyLetter_C,
    sHintFlyLetter_D,
    sHintFlyLetter_E,
    sHintFlyLetter_F,
    sHintFlyLetter_G,
    sHintFlyLetter_H,
    sHintFlyLetter_I,
    sHintFlyLetter_J,
    sHintFlyLetter_K,
    sHintFlyLetter_L,
    sHintFlyLetter_M,
    sHintFlyLetter_N,
    sHintFlyLetter_O,
    sHintFlyLetter_P,
    sHintFlyLetter_Q,
    sHintFlyLetter_R,
    sHintFlyLetter_S,
    sHintFlyLetter_T,
    sHintFlyLetter_U,
    sHintFlyLetter_V,
    sHintFlyLetter_W,
    sHintFlyLetter_X,
    sHintFlyLetter_Y,
    sHintFlyLetter_Z,
};

u8 sHintFlyRndSeed_4C6AA4 = 37;

static u8 HintFly_NextRandom()
{
    return gRandomBytes[sHintFlyRndSeed_4C6AA4++];
}

HintFly::HintFly(relive::Path_HintFly* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HintFly));
    Animation_Init(GetAnimRes(AnimId::HintFly));

    GetAnimation().SetSemiTrans(false);
    mTlvId = tlvId;
    mMsgIdx = 0;

    mMessageId = pTlv->mMessageId;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    const char_type* pMsg = gHintFlyMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, pTlv->mMessageId);

    mCounter = 20;
    mMsgLength = 0;

    // Find the length of the longest word (in particles)
    s16 longestWordLen = 0;
    s16 curWordLen = 0;
    for (;;)
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
            curWordLen += pHintFlyAlphabet_4C7268[(*pMsg) - 'A'][0];
        }
        pMsg++;
    }

    mMsgLength = longestWordLen;
    mMsgLength += 12;

    mHintFlyParticle = relive_new HintFlyParticle[mMsgLength];
    if (mHintFlyParticle)
    {
        mState = State::eIdleWaitForChanting_1;
        mTimer = 0;

        const auto pHeader = GetAnimation().Get_FrameHeader(-1);

        for (s32 i = 0; i < mMsgLength; i++)
        {
            for (s32 j = 0; j < 2; j++)
            {
                Poly_FT4* pSprt = &mHintFlyParticle[i].mSprt[j];

                pSprt->SetSemiTransparent(true);
                pSprt->DisableBlending(true);

                pSprt->SetUV0(0, 0);

                pSprt->mAnim = &GetAnimation();
                pSprt->SetXYWH(0, 0, static_cast<s16>(pHeader->mWidth - 1), static_cast<s16>(pHeader->mHeight - 1));
            }
        }


        mScreenX = FP_GetExponent(mXPos + FP_FromInteger(gScreenManager->mCamXOff) - gScreenManager->mCamPos->x);
        mScreenY = FP_GetExponent(mYPos + FP_FromInteger(gScreenManager->mCamYOff) - gScreenManager->mCamPos->y);

        for (s32 i = 0; i < mCounter; i++)
        {
            HintFlyParticle* pParticle = &mHintFlyParticle[i];
            InitParticle(pParticle);
        }
    }
    else
    {
        SetDead(true);
    }
}

void HintFly::InitParticle(HintFlyParticle* pParticle)
{
    pParticle->mState = 3;
    pParticle->mXPos = FP_FromInteger((HintFly_NextRandom() & 0x1F) + mScreenX - 16);
    pParticle->mYPos = FP_FromInteger((HintFly_NextRandom() & 0x1F) + mScreenY - 16);
    pParticle->mAngle = HintFly_NextRandom();
    pParticle->mAngleSpeed = (HintFly_NextRandom() % 4) + 12;
    if (HintFly_NextRandom() % 2)
    {
        pParticle->mAngleSpeed = -pParticle->mAngleSpeed;
    }
}

void HintFly::VScreenChanged()
{
    SetDead(true);
}

HintFly::~HintFly()
{
    relive_delete[] mHintFlyParticle;
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void HintFly::FormWordAndAdvanceToNextWord()
{
    const char_type* msgPtr = &gHintFlyMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, mMessageId)[mMsgIdx];
    LOG_INFO("Word is %s", msgPtr);

    // Find how long the word is
    s16 letterCount = 0;
    const char_type* pEndWord = msgPtr;
    for (; *pEndWord != ' '; pEndWord++)
    {
        if (!*pEndWord)
        {
            break;
        }
        letterCount++;
    }

    // Next time start at the next word
    mMsgIdx += letterCount;

    // Also skip the space if we had one else display will be screwed
    if (*pEndWord == ' ')
    {
        mMsgIdx++;
    }

    const s32 xBase = mScreenX - (16 * letterCount) / 2;
    const s32 yBase = mScreenY - 8;

    FP xBaseFP = FP_FromInteger(xBase);
    const FP yBaseFP = FP_FromInteger(yBase);

    s32 particleIdx = 0;
    for (s32 i = 0; i < letterCount; i++)
    {
        const auto pArray = pHintFlyAlphabet_4C7268[msgPtr[i] - 'A'];
        //const auto pArray = HintFlyLetter_E_circumflex_4C7120; // letter test code
        // First element is the count of "pixels" that make up a word
        const s32 total = pArray[0];
        for (s32 j = 0; j < total; j++)
        {
            // Position each "pixel" of the word
            HintFlyParticle* pParticleIter = &mHintFlyParticle[particleIdx++];

            auto xVal = FP_FromInteger(pArray[j + 1] >> 4);
            auto yVal = FP_FromInteger((pArray[j + 1]) & 0xF);

            pParticleIter->mTargetX = (FP_FromDouble(0.8) * xVal) + xBaseFP;
            pParticleIter->mTargetY = (FP_FromDouble(0.8) * yVal) + yBaseFP;
            pParticleIter->mVelX = FP_FromRaw((pParticleIter->mTargetX.fpValue - pParticleIter->mXPos.fpValue) >> 4);
            pParticleIter->mVelY = FP_FromRaw((pParticleIter->mTargetY.fpValue - pParticleIter->mYPos.fpValue) >> 4);
            pParticleIter->mState = 1;

            pParticleIter->mSoundPitch = Math_RandomRange(-127, 127);
            pParticleIter->mSoundPitchSpeed = pParticleIter->mSoundPitch >> 4;
        }
        xBaseFP += FP_FromInteger(16);
    }

    while (particleIdx < mCounter)
    {
        mHintFlyParticle[particleIdx++].mState = 3;
    }
}

void HintFly::UpdateParticles()
{
    for (s32 i = 0; i < mCounter; i++)
    {
        HintFlyParticle* pParticle = &mHintFlyParticle[i];
        if (pParticle->mState == 1)
        {
            const FP xTargetDelta = FP_Abs(pParticle->mTargetX - pParticle->mXPos);
            const FP yTargetDelta = FP_Abs(pParticle->mTargetY - pParticle->mYPos);

            if (xTargetDelta >= FP_FromInteger(1) || yTargetDelta >= FP_FromInteger(1))
            {
                pParticle->mXPos += pParticle->mVelX;
                pParticle->mYPos += pParticle->mVelY;
                pParticle->mSoundPitch += pParticle->mSoundPitchSpeed;
            }
            else
            {
                pParticle->mState = 2;
                pParticle->mSoundPitch = 0;
            }

            if (i < 1 && Math_RandomRange(0, 100) < 40)
            {
                SFX_Play_Pitch(relive::SoundEffects::HintFly, Math_RandomRange(24, 30), pParticle->mSoundPitch);
            }
        }
        else if (pParticle->mState == 2)
        {
            if (!(HintFly_NextRandom() & 3))
            {
                pParticle->mXPos = pParticle->mTargetX + FP_FromInteger(HintFly_NextRandom() & 1);
            }

            if (i < 1 && Math_RandomRange(0, 100) < 40)
            {
                SFX_Play_Pitch(relive::SoundEffects::HintFly, Math_RandomRange(24, 30), 0);
            }
        }
        else if (pParticle->mState == 3)
        {
            pParticle->mAngle = pParticle->mAngle + pParticle->mAngleSpeed;
            pParticle->mXPos += (Math_Sine(pParticle->mAngle) * FP_FromInteger(5));
            pParticle->mYPos += (Math_Cosine(pParticle->mAngle) * FP_FromInteger(2));
            if (i < 1 && Math_RandomRange(0, 100) < 20)
            {
                SfxPlayMono(relive::SoundEffects::HintFly, Math_RandomRange(18, 24));
            }
        }
    }
}

void HintFly::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mState)
    {
        case State::eIdleWaitForChanting_1:
            UpdateParticles();

            if (EventGet(kEventAbeOhm))
            {
                mMsgIdx = 0;
                mState = State::eState_3;
                mTimer = MakeTimer(15);
            }
            return;

        case State::eState_2:
            if (mCounter == mTargetCount)
            {
                UpdateParticles();

                if (!EventGet(kEventAbeOhm))
                {
                    for (s32 i = 0; i < mCounter; i++)
                    {
                        if (mHintFlyParticle[i].mState == 3)
                        {
                            break;
                        }
                        mHintFlyParticle[i].mState = 3;
                    }
                    mHintFlyIdx = 0;
                    mState = State::eState_5;
                    return;
                }

                if (mCounter == mTargetCount)
                {
                    FormWordAndAdvanceToNextWord();
                    mState = State::eState_4;
                    mTimer = MakeTimer(30);
                }
            }

            for (s32 i = 0; i < 8; i++)
            {
                if (mCounter < mTargetCount)
                {
                    InitParticle(&mHintFlyParticle[mCounter++]);
                }
                else
                {
                    mCounter--;
                    if (mHintFlyIdx < mCounter)
                    {
                        InitParticle(&mHintFlyParticle[mHintFlyIdx++]);
                    }
                }

                if (mCounter == mTargetCount)
                {
                    break;
                }
            }

            // TODO: This block is duplicated above
            UpdateParticles();

            if (!EventGet(kEventAbeOhm))
            {
                for (s32 i = 0; i < mCounter; i++)
                {
                    if (mHintFlyParticle[i].mState == 3)
                    {
                        break;
                    }
                    mHintFlyParticle[i].mState = 3;
                }
                mHintFlyIdx = 0;
                mState = State::eState_5;
                return;
            }

            if (mCounter == mTargetCount)
            {
                FormWordAndAdvanceToNextWord();
                mState = State::eState_4;
                mTimer = MakeTimer(30);
            }
            break;

        case State::eState_3:
            UpdateParticles();

            if (!EventGet(kEventAbeOhm))
            {
                for (s32 i = 0; i < mCounter; i++)
                {
                    if (mHintFlyParticle[i].mState == 3)
                    {
                        break;
                    }
                    mHintFlyParticle[i].mState = 3;
                }
                mHintFlyIdx = 0;
                mState = State::eState_5;
                return;
            }

            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                s16 len = 0;
                const char_type* pMsgIter = gHintFlyMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, mMessageId) + mMsgIdx;
                while (*pMsgIter != ' ' && *pMsgIter != '\0')
                {
                    len += pHintFlyAlphabet_4C7268[(*pMsgIter) - 'A'][0];
                    pMsgIter++;
                }
                mHintFlyIdx = 0;
                mTargetCount = len + 12;
                mState = State::eState_2;
            }
            return;

        case State::eState_4:
            UpdateParticles();

            if (EventGet(kEventAbeOhm))
            {
                if (static_cast<s32>(sGnFrame) > mTimer)
                {
                    for (s32 i = 0; i < mCounter; i++)
                    {
                        if (mHintFlyParticle[i].mState == 3)
                        {
                            break;
                        }
                        mHintFlyParticle[i].mState = 3;
                    }
                    mState = State::eState_3;
                    mTimer = MakeTimer(15);
                }
            }
            else
            {
                for (s32 i = 0; i < mCounter; i++)
                {
                    if (mHintFlyParticle[i].mState == 3)
                    {
                        break;
                    }
                    mHintFlyParticle[i].mState = 3;
                }
                mHintFlyIdx = 0;
                mState = State::eState_5;
            }
            return;

        case State::eState_5:
            if (mCounter == 20)
            {
                if (EventGet(kEventAbeOhm))
                {
                    mState = State::eIdleWaitForChanting_1;
                }
            }
            [[fallthrough]];

        case State::eState_6:
            UpdateParticles();
            if (mHintFlyIdx >= 20)
            {
                mCounter -= 8;
                if (mCounter < 20)
                {
                    mCounter = 20;
                }
            }
            else
            {
                InitParticle(&mHintFlyParticle[mHintFlyIdx++]);
            }

            if (mCounter == 20 && !EventGet(kEventAbeOhm))
            {
                mState = State::eIdleWaitForChanting_1;
            }
            return;

        default:
            return;
    }
}

void HintFly::VRender(OrderingTable& ot)
{
    for (s32 i = 0; i < mCounter; i++)
    {
        HintFlyParticle* pParticle = &mHintFlyParticle[i];
        Poly_FT4* pSprt = &pParticle->mSprt[gPsxDisplay.mBufferIndex];

        const s16 flyX = FP_GetExponent(PsxToPCX(pParticle->mXPos, FP_FromInteger(11)));
        const s16 flyY = FP_GetExponent(pParticle->mYPos);

        const s16 flyW = static_cast<s16>(abs(pSprt[0].X0() - pSprt[0].X3()));
        const s16 flyH = static_cast<s16>(abs(pSprt[0].Y0() - pSprt[0].Y3()));

        pSprt->SetXYWH(flyX, flyY, flyW, flyH);
        pSprt->SetBlendMode(relive::TBlendModes::eBlend_1);

        ot.Add(Layer::eLayer_Above_FG1_39, pSprt);
    }
}

} // namespace AO
