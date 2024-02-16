#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

struct FixedPoint;
using FP = FixedPoint;

namespace AO {

enum class DDCheatValueType
{
    eShort = 1,
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

class DDCheat final : public ::BaseGameObject
{
public:
    DDCheat();

    virtual void VScreenChanged() override;

    static void ClearProperties();
    static s32 DebugStr(const char_type* pStr, ...);
    static s32 DebugFont_Printf(s32 idx, const char_type* formatStr, ...);

    void Misc();
    void Teleport();

private:
    void VUpdate() override;

    void AddPropertyEntry(const char_type* text, DDCheatValueType valueType, DDCheatValue valuePtr);

    s32 mCheatFnIdx = 0;
    s32 field_18_backInputPressed = 0;
    bool mTeleporting = false;
    s32 field_24_input = 0;
};

extern s16 gRescuedMudokons;
extern s16 gKilledMudokons;
extern bool gDDCheat_ShowAI_Info;
extern bool gDDCheat_FlyingEnabled;

} // namespace AO
