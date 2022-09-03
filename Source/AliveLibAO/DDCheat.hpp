#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"

namespace AO {

enum class DDCheatValueType : s32
{
    eShort_1 = 1,
    eInt_2 = 2,
    eShort_4 = 4,
    eInt_6 = 6,
};

union DDCheatValue
{
    s32* pInt;
    s16* pShort;
    FP* pFixedPoint;

    DDCheatValue()
    {
        pInt = nullptr;
    }

    DDCheatValue(s32* v)
    {
        pInt = v;
    }

    DDCheatValue(s16* v)
    {
        pShort = v;
    }

    DDCheatValue(FP* v)
    {
        pFixedPoint = v;
    }
};

struct DDCheatProperty final
{
    const char_type* Name;
    DDCheatValueType ValueType;
    DDCheatValue ValuePtr;
};
ALIVE_ASSERT_SIZEOF(DDCheatProperty, 12);

class DDCheat final : public ::BaseGameObject
{
public:
    DDCheat();

    static void ClearProperties();

    virtual void VScreenChanged() override;
    void AddPropertyEntry(const char_type* text, DDCheatValueType valueType, DDCheatValue valuePtr);

    void VUpdate();

    void ScreenShot();
    void Teleport();
    void Misc();
    static s32 DebugFont_Printf(s32 idx, const char_type* formatStr, ...);
    static s32 DebugStr(const char_type* pStr, ...);

    s32 field_10_bInvalidateRect = 0;
    s32 field_14_SelectedCheat = 0;
    s32 field_18_backInputPressed = 0;
    s32 field_1C_unused = 0;
    s32 field_20_bTeleportCheatActive = 0;
    s32 field_24_input = 0;
};
ALIVE_ASSERT_SIZEOF(DDCheat, 0x28);

ALIVE_VAR_EXTERN(s32, gTweak_X_5076D8);
ALIVE_VAR_EXTERN(s32, gTweak_Y_5076DC);

ALIVE_VAR_EXTERN(s16, sRescuedMudokons_5076C0);
ALIVE_VAR_EXTERN(s16, sKilledMudokons_5076BC);
ALIVE_VAR_EXTERN(s16, showDebugCreatureInfo_5076E0);
ALIVE_VAR_EXTERN(s16, sDDCheat_FlyingEnabled_50771C);

} // namespace AO
