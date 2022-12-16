#include "stdafx.h"
#include "Font.hpp"
#include "../relive_lib/Function.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/GameType.hpp"
#include "../relive_lib/Primitives_common.hpp"
#include "../relive_lib/PsxDisplay.hpp"

bool gDisableFontFlicker = false;
bool gFontDrawScreenSpace = false;

const char_type* kEmptyString = "";

// OG bug: many entries seem to be misaligned
Font_AtlasEntry sPauseMenuFontAtlas[169] =
          {{0u, 0u, 2u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {43u, 80u, 6u, 23u, "!"},
           {182u, 0u, 11u, 10u, kEmptyString},
           {182u, 0u, 11u, 10u, kEmptyString},
           {182u, 0u, 11u, 10u, kEmptyString},
           {182u, 0u, 11u, 10u, kEmptyString},
           {182u, 0u, 11u, 10u, kEmptyString},
           {85u, 96u, 7u, 8u, ","},
           {52u, 79u, 11u, 23u, "("},
           {64u, 79u, 11u, 23u, ")"},
           {186u, 0u, 11u, 10u, kEmptyString},
           {0u, 79u, 23u, 17u, "+"},
           {86u, 79u, 7u, 23u, kEmptyString},
           {75u, 79u, 10u, 11u, kEmptyString},
           {94u, 79u, 6u, 23u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {193u, 51u, 17u, 23u, "0"},
           {32u, 51u, 17u, 23u, "1"},
           {50u, 51u, 17u, 23u, "2"},
           {69u, 51u, 17u, 23u, "3"},
           {87u, 51u, 17u, 23u, "4"},
           {106u, 51u, 17u, 23u, "5"},
           {124u, 51u, 17u, 23u, "6"},
           {142u, 51u, 17u, 23u, "7"},
           {158u, 51u, 17u, 23u, "8"},
           {176u, 51u, 17u, 23u, "9"},
           {110u, 79u, 7u, 22u, ":"},
           {102u, 79u, 7u, 22u, ";"},
           {61u, 79u, 10u, 23u, ")"},
           {25u, 79u, 15u, 16u, "="},
           {73u, 79u, 10u, 23u, "-"},
           {118u, 79u, 14u, 23u, "?"},
           {182u, 0u, 11u, 10u, kEmptyString},
           {9u, 0u, 17u, 23u, "Big_A"},
           {26u, 0u, 15u, 23u, "Big_B"},
           {41u, 0u, 15u, 23u, "Big_C"},
           {56u, 0u, 16u, 23u, "Big_D"},
           {73u, 0u, 12u, 23u, "Big_E"},
           {86u, 0u, 12u, 23u, "Big_F"},
           {98u, 0u, 15u, 23u, "Big_G"},
           {114u, 0u, 16u, 23u, "Big_H"},
           {132u, 0u, 7u, 23u, "Big_I"},
           {139u, 0u, 14u, 23u, "Big_J"},
           {154u, 0u, 15u, 23u, "Big_K"},
           {170u, 0u, 12u, 23u, "Big_L"},
           {0u, 25u, 21u, 23u, "Big_M"},
           {21u, 25u, 16u, 23u, "Big_N"},
           {37u, 25u, 18u, 23u, "Big_O"},
           {56u, 25u, 14u, 23u, "Big_P"},
           {71u, 25u, 18u, 23u, "Big_Q"},
           {90u, 25u, 14u, 23u, "Big_R"},
           {106u, 25u, 14u, 23u, "Big_S"},
           {121u, 25u, 15u, 23u, "Big_T"},
           {137u, 25u, 16u, 23u, "Big_U"},
           {153u, 25u, 17u, 23u, "Big_V"},
           {170u, 25u, 20u, 23u, "Big_W"},
           {191u, 25u, 15u, 23u, "Big_X"},
           {0u, 50u, 17u, 23u, "Big_Y"},
           {17u, 50u, 13u, 23u, "Big_Z"},
           {61u, 79u, 10u, 23u, ")"},
           {0u, 0u, 9u, 0u, kEmptyString},
           {73u, 79u, 10u, 23u, "-"},
           {186u, 0u, 11u, 10u, kEmptyString},
           {86u, 79u, 27u, 10u, kEmptyString},
           {186u, 0u, 7u, 11u, kEmptyString},
           {0u, 108u, 16u, 24u, "Medium_A"},
           {17u, 108u, 13u, 24u, "Medium_B"},
           {31u, 108u, 13u, 24u, "Medium_C"},
           {44u, 108u, 15u, 24u, "Medium_D"},
           {59u, 108u, 11u, 24u, "Medium_E"},
           {71u, 108u, 11u, 24u, "Medium_F"},
           {83u, 108u, 13u, 24u, "Medium_G"},
           {97u, 108u, 14u, 24u, "Medium_H"},
           {113u, 108u, 7u, 24u, "Medium_I"},
           {120u, 108u, 11u, 24u, "Medium_J"},
           {134u, 108u, 13u, 24u, "Medium_K"},
           {149u, 108u, 11u, 24u, "Medium_L"},
           {0u, 135u, 17u, 24u, "Medium_M"},
           {19u, 135u, 15u, 24u, "Medium_N"},
           {34u, 135u, 17u, 24u, "Medium_O"},
           {52u, 135u, 13u, 24u, "Medium_P"},
           {66u, 135u, 17u, 24u, "Medium_Q"},
           {84u, 135u, 13u, 24u, "Medium_R"},
           {97u, 135u, 12u, 24u, "Medium_S"},
           {111u, 135u, 14u, 24u, "Medium_T"},
           {125u, 135u, 14u, 24u, "Medium_U"},
           {141u, 135u, 15u, 24u, "Medium_V"},
           {157u, 135u, 17u, 24u, "Medium_W"},
           {1u, 163u, 13u, 24u, "Medium_X"},
           {16u, 163u, 15u, 24u, "Medium_Y"},
           {32u, 163u, 12u, 24u, "Medium_Z"},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {134u, 79u, 16u, 9u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {227u, 211u, 12u, 24u, kEmptyString},
           {239u, 187u, 14u, 24u, kEmptyString},
           {233u, 79u, 11u, 24u, "E_Acute"},
           {201u, 79u, 14u, 24u, kEmptyString},
           {217u, 79u, 14u, 24u, kEmptyString},
           {187u, 79u, 14u, 24u, kEmptyString},
           {0u, 110u, 16u, 22u, kEmptyString},
           {227u, 211u, 12u, 24u, kEmptyString},
           {211u, 52u, 11u, 24u, kEmptyString},
           {222u, 52u, 11u, 24u, kEmptyString},
           {244u, 79u, 11u, 24u, kEmptyString},
           {213u, 28u, 10u, 24u, kEmptyString},
           {175u, 136u, 8u, 24u, kEmptyString},
           {240u, 51u, 7u, 24u, kEmptyString},
           {218u, 79u, 17u, 24u, kEmptyString},
           {0u, 110u, 16u, 22u, kEmptyString},
           {233u, 79u, 11u, 24u, kEmptyString},
           {223u, 234u, 19u, 20u, kEmptyString},
           {223u, 234u, 19u, 20u, kEmptyString},
           {214u, 162u, 17u, 24u, kEmptyString},
           {230u, 162u, 17u, 24u, kEmptyString},
           {239u, 27u, 16u, 24u, kEmptyString},
           {225u, 187u, 14u, 24u, kEmptyString},
           {210u, 187u, 14u, 24u, kEmptyString},
           {16u, 164u, 15u, 22u, kEmptyString},
           {230u, 162u, 17u, 24u, kEmptyString},
           {239u, 187u, 14u, 24u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {170u, 79u, 16u, 24u, "A_Acute"},
           {234u, 51u, 7u, 24u, "I_Acute"},
           {223u, 27u, 16u, 24u, "O_Acute"},
           {197u, 187u, 14u, 24u, "U_Acute"},
           {238u, 211u, 15u, 24u, "N_Tilde"},
           {238u, 211u, 16u, 24u, kEmptyString}, // same as above with less width
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {197u, 212u, 14u, 24u, "Inverted_Question_Mark"},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {197u, 0u, 7u, 22u, "Inverted_Exclamation_Point"},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 186u, 36u, 22u, "Button_A"},
           {39u, 186u, 36u, 22u, "Button_B"},
           {78u, 186u, 36u, 22u, "Button_C"},
           {117u, 186u, 36u, 22u, "Button_D"},
           {156u, 186u, 36u, 22u, "Button_E"},
           {0u, 210u, 36u, 22u, "Button_F"},
           {39u, 210u, 36u, 22u, "Button_G"},
           {78u, 210u, 36u, 22u, "Button_H"},
           {189u, 124u, 30u, 19u, "Dpoad_Up"},
           {221u, 124u, 30u, 20u, "Dpad_Down"},
           {224u, 143u, 25u, 21u, "Dpad_Left"},
           {193u, 143u, 25u, 21u, "Dpad_Right"},
           {162u, 110u, 19u, 18u, kEmptyString},
           {211u, 0u, 44u, 26u, "Dpad"},
           {196u, 162u, 16u, 24u, "0"},
           {48u, 162u, 16u, 24u, "1"},
           {64u, 162u, 16u, 24u, "2"},
           {81u, 162u, 16u, 24u, "3"},
           {98u, 162u, 16u, 24u, "4"},
           {115u, 162u, 16u, 24u, "5"},
           {131u, 162u, 16u, 24u, "6"},
           {148u, 162u, 16u, 24u, "7"},
           {163u, 162u, 16u, 24u, "8"},
           {180u, 162u, 16u, 24u, "9"}};

Font_AtlasEntry sLcdFontAtlas[169] =
          {{0u, 0u, 2u, 0u, kEmptyString},
           {0u, 0u, 14u, 0u, kEmptyString},
           {105u, 42u, 14u, 13u, "!"},
           {105u, 70u, 14u, 4u, "Double_Prime"},
           {0u, 0u, 14u, 0u, kEmptyString},
           {75u, 70u, 14u, 13u, "Ash"}, // looks like AE
           {0u, 0u, 14u, 0u, kEmptyString},
           {0u, 0u, 14u, 0u, kEmptyString},
           {105u, 70u, 8u, 5u, "Acute_Accent_More_WH"},
           {45u, 70u, 14u, 13u, "("},
           {60u, 70u, 14u, 13u, ")"},
           {105u, 84u, 14u, 0u, kEmptyString},
           {75u, 84u, 14u, 13u, "+"},
           {45u, 42u, 14u, 13u, ","},
           {60u, 84u, 14u, 13u, "-"},
           {30u, 42u, 14u, 13u, "."},
           {0u, 0u, 14u, 0u, kEmptyString},
           {0u, 56u, 14u, 13u, "0"},
           {15u, 56u, 14u, 13u, "1"},
           {30u, 56u, 14u, 13u, "2"},
           {45u, 56u, 14u, 13u, "3"},
           {60u, 56u, 14u, 13u, "4"},
           {75u, 56u, 14u, 13u, "5"},
           {90u, 56u, 14u, 13u, "6"},
           {105u, 56u, 14u, 13u, "7"},
           {0u, 70u, 14u, 13u, "8"},
           {15u, 70u, 14u, 13u, "9"},
           {60u, 42u, 14u, 13u, ":"},
           {75u, 42u, 14u, 13u, ";"},
           {45u, 70u, 14u, 13u, "("},
           {30u, 70u, 14u, 13u, "="},
           {60u, 70u, 14u, 13u, ")"},
           {90u, 42u, 14u, 13u, "?"},
           {0u, 0u, 14u, 0u, kEmptyString},
           {0u, 0u, 14u, 13u, "A"},
           {15u, 0u, 14u, 13u, "B"},
           {30u, 0u, 14u, 13u, "C"},
           {45u, 0u, 14u, 13u, "D"},
           {60u, 0u, 14u, 13u, "E"},
           {75u, 0u, 14u, 13u, "F"},
           {90u, 0u, 14u, 13u, "G"},
           {105u, 0u, 14u, 13u, "H"},
           {0u, 14u, 14u, 13u, "I"},
           {15u, 14u, 14u, 13u, "J"},
           {30u, 14u, 14u, 13u, "K"},
           {45u, 14u, 14u, 13u, "L"},
           {60u, 14u, 14u, 13u, "M"},
           {75u, 14u, 14u, 13u, "N"},
           {90u, 14u, 14u, 13u, "O"},
           {105u, 14u, 14u, 13u, "P"},
           {0u, 28u, 14u, 13u, "Q"},
           {15u, 28u, 14u, 13u, "R"},
           {30u, 28u, 14u, 13u, "S"},
           {45u, 28u, 14u, 13u, "T"},
           {60u, 28u, 14u, 13u, "U"},
           {75u, 28u, 14u, 13u, "V"},
           {90u, 28u, 14u, 13u, "W"},
           {105u, 28u, 14u, 13u, "X"},
           {0u, 42u, 14u, 13u, "Y"},
           {15u, 42u, 14u, 13u, "Z"},
           {45u, 70u, 14u, 13u, "("},
           {90u, 70u, 14u, 13u, "\\"},
           {60u, 70u, 14u, 13u, ")"},
           {0u, 0u, 14u, 0u, kEmptyString},
           {0u, 0u, 14u, 0u, kEmptyString},
           {105u, 70u, 7u, 4u, "Acute_Accent"},
           {0u, 0u, 14u, 13u, "A"},
           {15u, 0u, 14u, 13u, "B"},
           {30u, 0u, 14u, 13u, "C"},
           {45u, 0u, 14u, 13u, "D"},
           {60u, 0u, 14u, 13u, "E"},
           {75u, 0u, 14u, 13u, "F"},
           {90u, 0u, 14u, 13u, "G"},
           {105u, 0u, 14u, 13u, "H"},
           {0u, 14u, 14u, 13u, "I"},
           {15u, 14u, 14u, 13u, "J"},
           {30u, 14u, 14u, 13u, "K"},
           {45u, 14u, 14u, 13u, "L"},
           {60u, 14u, 14u, 13u, "M"},
           {75u, 14u, 14u, 13u, "N"},
           {90u, 14u, 14u, 13u, "O"},
           {105u, 14u, 14u, 13u, "P"},
           {0u, 28u, 14u, 13u, "Q"},
           {15u, 28u, 14u, 13u, "R"},
           {30u, 28u, 14u, 13u, "S"},
           {45u, 28u, 14u, 13u, "T"},
           {60u, 28u, 14u, 13u, "U"},
           {75u, 28u, 14u, 13u, "V"},
           {90u, 28u, 14u, 13u, "W"},
           {105u, 28u, 14u, 13u, "X"},
           {0u, 42u, 14u, 13u, "Y"},
           {15u, 42u, 14u, 13u, "Z"},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 0u, 9u, 0u, kEmptyString},
           {0u, 98u, 20u, 15u, "Button_A"},
           {20u, 98u, 20u, 15u, "Button_B"},
           {40u, 98u, 20u, 15u, "Button_C"},
           {60u, 98u, 20u, 15u, "Button_D"},
           {80u, 98u, 20u, 15u, "Button_E"},
           {0u, 115u, 20u, 15u, "Button_F"},
           {20u, 115u, 20u, 15u, "Button_G"},
           {40u, 115u, 20u, 15u, "Button_H"},
           {0u, 132u, 20u, 15u, "Button_Up"},
           {20u, 132u, 20u, 15u, "Button_Down"},
           {40u, 132u, 20u, 15u, "Button_Left"},
           {60u, 132u, 20u, 15u, "Button_Right"},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString},
           {0u, 0u, 0u, 0u, kEmptyString}};

namespace AO {

Font_AtlasEntry sPauseMenuFontAtlas[116] = {
    {0, 0, 2, 0, kEmptyString},
    {0, 0, 9, 0, kEmptyString},
    {43, 80, 6, 23, "!"},
    {182, 0, 11, 10, kEmptyString},
    {182, 0, 11, 10, kEmptyString},
    {182, 0, 11, 10, kEmptyString},
    {182, 0, 11, 10, kEmptyString},
    {182, 0, 11, 10, kEmptyString},
    {85, 96, 7, 8, ","},
    {52, 79, 11, 23, "("},
    {64, 79, 11, 23, ")"},
    {186, 0, 11, 10, kEmptyString},
    {0, 79, 23, 17, "+"},
    {86, 79, 7, 23, ","},
    {75, 79, 10, 11, "_"}, // or minus?
    {94, 79, 6, 23, "."},
    {149, 79, 17, 23, "/"},
    {193, 51, 17, 23, "0"},
    {32, 51, 17, 23, "1"},
    {50, 51, 17, 23, "2"},
    {69, 51, 17, 23, "3"},
    {87, 51, 17, 23, "4"},
    {106, 51, 17, 23, "5"},
    {124, 51, 17, 23, "6"},
    {142, 51, 17, 23, "7"},
    {158, 51, 17, 23, "8"},
    {176, 51, 17, 23, "9"},
    {110, 79, 7, 22, ":"},
    {102, 79, 7, 22, ";"},
    {61, 79, 10, 23, ")"},
    {25, 79, 15, 16, "="},
    {73, 79, 10, 23, "-"},
    {118, 79, 14, 23, "?"},
    {182, 0, 11, 10, kEmptyString},
    {9, 0, 17, 23, "Big_A"},
    {26, 0, 15, 23, "Big_B"},
    {41, 0, 15, 23, "Big_C"},
    {56, 0, 16, 23, "Big_D"},
    {73, 0, 12, 23, "Big_E"},
    {86, 0, 12, 23, "Big_F"},
    {98, 0, 15, 23, "Big_G"},
    {114, 0, 16, 23, "Big_H"},
    {132, 0, 7, 23, "Big_I"},
    {139, 0, 14, 23, "Big_J"},
    {154, 0, 15, 23, "Big_K"},
    {170, 0, 12, 23, "Big_L"},
    {0, 25, 21, 23, "Big_M"},
    {21, 25, 16, 23, "Big_N"},
    {37, 25, 18, 23, "Big_O"},
    {56, 25, 14, 23, "Big_P"},
    {71, 25, 18, 23, "Big_Q"},
    {90, 25, 14, 23, "Big_R"},
    {106, 25, 14, 23, "Big_S"},
    {121, 25, 15, 23, "Big_T"},
    {137, 25, 16, 23, "Big_U"},
    {153, 25, 17, 23, "Big_V"},
    {170, 25, 20, 23, "Big_W"},
    {191, 25, 15, 23, "Big_X"},
    {0, 50, 17, 23, "Big_Y"},
    {17, 50, 13, 23, "Big_Z"},
    {61, 79, 10, 23, ")"},
    {131, 79, 17, 23, "\\"},
    {73, 79, 10, 23, "-"},
    {186, 0, 11, 10, kEmptyString},
    {86, 79, 27, 10, kEmptyString},
    {186, 0, 7, 11, kEmptyString},
    {0, 110, 16, 22, "Medium_A"},
    {17, 110, 13, 22, "Medium_B"},
    {31, 110, 13, 22, "Medium_C"},
    {44, 110, 15, 22, "Medium_D"},
    {59, 110, 11, 22, "Medium_E"},
    {71, 110, 11, 22, "Medium_F"},
    {83, 110, 13, 22, "Medium_G"},
    {97, 110, 14, 22, "Medium_H"},
    {113, 110, 7, 22, "Medium_I"},
    {120, 110, 11, 22, "Medium_J"},
    {134, 110, 13, 22, "Medium_K"},
    {149, 110, 11, 22, "Medium_L"},
    {0, 137, 17, 22, "Medium_M"},
    {19, 137, 15, 22, "Medium_N"},
    {34, 137, 17, 22, "Medium_O"},
    {52, 137, 13, 22, "Medium_P"},
    {66, 137, 17, 22, "Medium_Q"},
    {84, 137, 13, 22, "Medium_R"},
    {97, 137, 12, 22, "Medium_S"},
    {111, 137, 14, 22, "Medium_T"},
    {125, 137, 14, 22, "Medium_U"},
    {141, 137, 15, 22, "Medium_V"},
    {157, 137, 17, 22, "Medium_W"},
    {1, 164, 13, 22, "Medium_X"},
    {16, 164, 15, 22, "Medium_Y"},
    {32, 164, 12, 22, "Medium_Z"},
    {0, 186, 36, 22, "Button_A"},
    {39, 186, 36, 22, "Button_B"},
    {78, 186, 36, 22, "Button_C"},
    {117, 186, 36, 22, "Button_D"},
    {156, 186, 36, 22, "Button_E"},
    {0, 210, 36, 22, "Button_F"},
    {39, 210, 36, 22, "Button_G"},
    {78, 210, 36, 22, "Button_H"},
    {189, 124, 30, 19, "Dpad_Up"},
    {221, 124, 30, 20, "Dpad_Down"},
    {224, 143, 25, 21, "Dpad_Left"},
    {193, 143, 25, 21, "Dpad_Right"},
    {162, 110, 19, 22, "Or"},
    {211, 0, 44, 26, "Dpad"},
    {196, 164, 16, 22, "0"},
    {48, 164, 16, 22, "1"},
    {64, 164, 16, 22, "2"},
    {81, 164, 16, 22, "3"},
    {98, 164, 16, 22, "4"},
    {115, 164, 16, 22, "5"},
    {131, 164, 16, 22, "6"},
    {148, 164, 16, 22, "7"},
    {163, 164, 16, 22, "8"},
    {180, 164, 16, 22, "9"}};

Font_AtlasEntry sLcdFontAtlas[104] = {
    {0, 0, 2, 0, kEmptyString},
    {0, 0, 14, 0, kEmptyString},
    {105, 42, 14, 13, "!"},
    {105, 70, 14, 4, "Double_Prime"},
    {0, 0, 14, 0, kEmptyString},
    {75, 70, 14, 13, "$"},
    {0, 0, 14, 0, kEmptyString},
    {0, 0, 14, 0, kEmptyString},
    {105, 70, 8, 5, "Prime"},
    {45, 70, 14, 13, "("},
    {60, 70, 14, 13, ")"},
    {105, 84, 14, 0, kEmptyString},
    {75, 84, 14, 13, "+"},
    {45, 42, 14, 13, ","},
    {60, 84, 14, 13, "-"},
    {30, 42, 14, 13, "."},
    {0, 0, 14, 0, kEmptyString},
    {0, 56, 14, 13, "0"},
    {15, 56, 14, 13, "1"},
    {30, 56, 14, 13, "2"},
    {45, 56, 14, 13, "3"},
    {60, 56, 14, 13, "4"},
    {75, 56, 14, 13, "5"},
    {90, 56, 14, 13, "6"},
    {105, 56, 14, 13, "7"},
    {0, 70, 14, 13, "8"},
    {15, 70, 14, 13, "9"},
    {60, 42, 14, 13, ":"},
    {75, 42, 14, 13, ";"},
    {45, 70, 14, 13, "("},
    {30, 70, 14, 13, "="},
    {60, 70, 14, 13, ")"},
    {90, 42, 14, 13, "?"},
    {0, 0, 14, 0, kEmptyString},
    {0, 0, 14, 13, "A"},
    {15, 0, 14, 13, "B"},
    {30, 0, 14, 13, "C"},
    {45, 0, 14, 13, "D"},
    {60, 0, 14, 13, "E"},
    {75, 0, 14, 13, "F"},
    {90, 0, 14, 13, "G"},
    {105, 0, 14, 13, "H"},
    {0, 14, 14, 13, "I"},
    {15, 14, 14, 13, "J"},
    {30, 14, 14, 13, "K"},
    {45, 14, 14, 13, "L"},
    {60, 14, 14, 13, "M"},
    {75, 14, 14, 13, "N"},
    {90, 14, 14, 13, "O"},
    {105, 14, 14, 13, "P"},
    {0, 28, 14, 13, "Q"},
    {15, 28, 14, 13, "R"},
    {30, 28, 14, 13, "S"},
    {45, 28, 14, 13, "T"},
    {60, 28, 14, 13, "U"},
    {75, 28, 14, 13, "V"},
    {90, 28, 14, 13, "W"},
    {105, 28, 14, 13, "X"},
    {0, 42, 14, 13, "Y"},
    {15, 42, 14, 13, "Z"},
    {45, 70, 14, 13, "("},
    {90, 70, 14, 13, "\\"},
    {60, 70, 14, 13, ")"},
    {0, 0, 14, 0, kEmptyString},
    {0, 0, 14, 0, kEmptyString},
    {105, 70, 7, 4, "Prime"},
    {0, 0, 14, 13, "A"},
    {15, 0, 14, 13, "B"},
    {30, 0, 14, 13, "C"},
    {45, 0, 14, 13, "D"},
    {60, 0, 14, 13, "E"},
    {75, 0, 14, 13, "F"},
    {90, 0, 14, 13, "G"},
    {105, 0, 14, 13, "H"},
    {0, 14, 14, 13, "I"},
    {15, 14, 14, 13, "J"},
    {30, 14, 14, 13, "K"},
    {45, 14, 14, 13, "L"},
    {60, 14, 14, 13, "M"},
    {75, 14, 14, 13, "N"},
    {90, 14, 14, 13, "O"},
    {105, 14, 14, 13, "P"},
    {0, 28, 14, 13, "Q"},
    {15, 28, 14, 13, "R"},
    {30, 28, 14, 13, "S"},
    {45, 28, 14, 13, "T"},
    {60, 28, 14, 13, "U"},
    {75, 28, 14, 13, "V"},
    {90, 28, 14, 13, "W"},
    {105, 28, 14, 13, "X"},
    {0, 42, 14, 13, "Y"},
    {15, 42, 14, 13, "Z"},
    {0, 98, 20, 15, "Button_A"},
    {20, 98, 20, 15, "Button_B"},
    {40, 98, 20, 15, "Button_C"},
    {60, 98, 20, 15, "Button_D"},
    {80, 98, 20, 15, "Button_E"},
    {0, 115, 20, 15, "Button_F"},
    {20, 115, 20, 15, "Button_G"},
    {40, 115, 20, 15, "Button_H"},
    {0, 132, 20, 15, "Button_Up"},
    {20, 132, 20, 15, "Button_Down"},
    {40, 132, 20, 15, "Button_Left"},
    {60, 132, 20, 15, "Button_Right"}};
}

AliveFont::AliveFont()
{
}

AliveFont::AliveFont(s32 maxCharLength, const PalResource& pal, FontContext* fontContext)
{
    Load(maxCharLength, pal, fontContext);
}

void AliveFont::Load(s32 maxCharLength, const PalResource& pal, FontContext* fontContext)
{
    mFontContext = fontContext;
    mFontContext->mFntResource.mCurPal = pal.mPal;
    mPolyCount = maxCharLength;
    mFntPolyArray = relive_new Poly_FT4[maxCharLength * 2];
}

AliveFont::~AliveFont()
{
    relive_delete[] mFntPolyArray;
}

s32 AliveFont::DrawString(PrimHeader** ppOt, const char_type* text, s32 x, s16 y, TPageAbr abr, s32 bSemiTrans, s32 blendMode, Layer layer, u8 r, u8 g, u8 b, s32 polyOffset, FP scale, s32 maxRenderWidth, s16 colorRandomRange)
{
    if (!gFontDrawScreenSpace)
    {
        x = static_cast<s32>(x / 0.575); // 368 to 640. Convert world space to screen space coords.
    }

    s32 characterRenderCount = 0;
    const s32 maxRenderX = static_cast<s32>(maxRenderWidth / 0.575);
    s16 offsetX = static_cast<s16>(x);
    s32 charInfoIndex = 0;
    auto poly = &mFntPolyArray[gPsxDisplay.mBufferIndex + (2 * polyOffset)];

    for (u32 i = 0; i < strlen(text); i++)
    {
        if (offsetX >= maxRenderX)
        {
            break;
        }

        const u8 c = text[i];
        if (c <= 32 || c > 175)
        {
            if (c < 7u || c > 31)
            {
                offsetX += mFontContext->mAtlasArray[1].mWidth;
                continue;
            }
            charInfoIndex = c + 137;
        }
        else
        {
            charInfoIndex = c - 31;
        }

        const auto fContext = mFontContext;
        const auto atlasEntry = &fContext->mAtlasArray[charInfoIndex];

        const s8 charWidth = atlasEntry->mWidth;
        const auto charHeight = atlasEntry->mHeight;

        // TODO: Recalc when atlas is converted
        const s8 texture_u = static_cast<s8>(atlasEntry->x);
        const s8 texture_v = static_cast<s8>(atlasEntry->mY);

        const s16 widthScaled = static_cast<s16>(charWidth * FP_GetDouble(scale));
        const s16 heightScaled = static_cast<s16>(charHeight * FP_GetDouble(scale));

        PolyFT4_Init(poly);

        SetPrimExtraPointerHack(poly, nullptr);

        Poly_Set_SemiTrans(&poly->mBase.header, bSemiTrans);
        Poly_Set_Blending(&poly->mBase.header, blendMode);

        SetRGB0(
            poly,
            static_cast<u8>(r + Math_RandomRange(-colorRandomRange, colorRandomRange)),
            static_cast<u8>(g + Math_RandomRange(-colorRandomRange, colorRandomRange)),
            static_cast<u8>(b + Math_RandomRange(-colorRandomRange, colorRandomRange)));

        // Padding
        poly->mVerts[1].mUv.tpage_clut_pad = 0;
        poly->mVerts[2].mUv.tpage_clut_pad = 0;

        // P0
        SetXY0(poly, offsetX, y);
        SetUV0(poly, texture_u, texture_v);

        // P1
        SetXY1(poly, offsetX + widthScaled, y);
        SetUV1(poly, texture_u + charWidth, texture_v);

        // P2
        SetXY2(poly, offsetX, y + heightScaled);
        SetUV2(poly, texture_u, texture_v + charHeight);

        // P3
        SetXY3(poly, offsetX + widthScaled, y + heightScaled);
        SetUV3(poly, texture_u + charWidth, texture_v + charHeight);

        // TPage blend mode start
        u16 tpageEmptyBlend = GetTPage(poly) & 0xFFCF;
        u16 blendModeBit = ((u16) abr) << 4;
        SetTPage(poly, tpageEmptyBlend | blendModeBit);
        poly->mFont = mFontContext;
        // TPage blend mode start

        OrderingTable_Add(OtLayer(ppOt, layer), &poly->mBase.header);

        ++characterRenderCount;

        offsetX += widthScaled + FP_GetExponent(FP_FromInteger(mFontContext->mAtlasArray[0].mWidth) * scale);

        poly += 2;
    }

    return polyOffset + characterRenderCount;
}

s32 AliveFont::MeasureTextWidth(const char_type* text)
{
    s32 result = 0;

    for (u32 i = 0; i < strlen(text); i++)
    {
        const s8 c = text[i];
        s32 charIndex = 0;

        if (c <= 32 || static_cast<u8>(c) > 175)
        {
            if (c < 7 || c > 31)
            {
                result += mFontContext->mAtlasArray[1].mWidth;
                continue;
            }
            else
            {
                charIndex = c + 137;
            }
        }
        else
        {
            charIndex = c - 31;
        }

        result += mFontContext->mAtlasArray[0].mWidth;
        result += mFontContext->mAtlasArray[charIndex].mWidth;
    }

    if (!gFontDrawScreenSpace)
    {
        result = static_cast<s32>(result * 0.575); // Convert screen space to world space.
    }

    return result;
}

// Measures the width of a string with scale applied.
s32 AliveFont::MeasureScaledTextWidth(const char_type* text, FP scale)
{
    const FP width = FP_FromInteger(MeasureTextWidth(text));
    return FP_GetExponent((width * scale) + FP_FromDouble(0.5));
}

// Measures the width of a single character.
s32 AliveFont::MeasureCharacterWidth(char_type character)
{
    s32 result = 0;
    s32 charIndex = 0;

    if (character <= 32 || character > 175)
    {
        if (character < 7 || character > 31)
        {
            return mFontContext->mAtlasArray[1].mWidth;
        }
        charIndex = character + 137;
    }
    else
    {
        charIndex = character - 31;
    }
    result = mFontContext->mAtlasArray[charIndex].mWidth;

    if (!gFontDrawScreenSpace)
    {
        result = static_cast<s32>(result * 0.575); // Convert screen space to world space.
    }

    return result;
}

// Wasn't too sure what to call this. Returns the s8 offset of where the text is cut off. (left and right region)
const char_type* AliveFont::SliceText(const char_type* text, s32 left, FP scale, s32 right)
{
    s32 xOff = 0;
    s32 rightWorldSpace = static_cast<s32>(right * 0.575);

    if (gFontDrawScreenSpace)
    {
        xOff = left;
    }
    else
    {
        xOff = static_cast<s32>(left / 0.575);
    }


    for (const char_type* strPtr = text; *strPtr; strPtr++)
    {
        s32 atlasIdx = 0;
        char_type character = *strPtr;
        if (xOff >= rightWorldSpace)
        {
            break;
        }

        if (character <= 32 || character > 122)
        {
            if (character < 7 || character > 31)
            {
                xOff += mFontContext->mAtlasArray[1].mWidth;
                continue;
            }
            atlasIdx = character + 137;
        }
        else
        {
            atlasIdx = character - 31;
        }

        xOff += static_cast<s32>(mFontContext->mAtlasArray[atlasIdx].mWidth * FP_GetDouble(scale)) + mFontContext->mAtlasArray->mWidth;
    }

    return text;
}

void FontContext::LoadFontType(FontType resourceID)
{
    FontResource fontRes = ResourceManagerWrapper::LoadFont(resourceID);
    mFntResource = fontRes;

    // TODO: Will get moved to a json file in FontResource
    // which will remove the need for GetGameType() in here also
    switch (resourceID)
    {
        case FontType::PauseMenu:
            mAtlasArray = GetGameType() == GameType::eAe ? sPauseMenuFontAtlas : AO::sPauseMenuFontAtlas;
            break;
        case FontType::LcdFont:
            mAtlasArray = GetGameType() == GameType::eAe ? sLcdFontAtlas : AO::sLcdFontAtlas;
            break;
        default:
            ALIVE_FATAL("Unknown font resource ID !!!");
            break;
    }
}



