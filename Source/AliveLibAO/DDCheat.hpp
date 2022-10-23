#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibAE/FixedPoint.hpp"

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

    void VUpdate() override;

    void Teleport();
    void Misc();
    static s32 DebugFont_Printf(s32 idx, const char_type* formatStr, ...);
    static s32 DebugStr(const char_type* pStr, ...);

    s32 field_14_SelectedCheat = 0;
    s32 field_18_backInputPressed = 0;
    s32 field_1C_unused = 0;
    s32 field_20_bTeleportCheatActive = 0;
    s32 field_24_input = 0;
};

extern s32 gTweakX;
extern s32 gTweakY;

extern s16 sRescuedMudokons_5076C0;
extern s16 sKilledMudokons_5076BC;
extern s16 showDebugCreatureInfo_5076E0;
extern s16 sDDCheat_FlyingEnabled_50771C;

} // namespace AO
