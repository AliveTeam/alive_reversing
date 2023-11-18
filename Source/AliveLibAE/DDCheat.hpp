#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"

struct FixedPoint;
using FP = FixedPoint;

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

class DDCheat final : public BaseGameObject
{
public:
    DDCheat();

    void Menu_Teleport();
    void Menu_Movies();
    static void ClearProperties();
    static void DebugStr(const char_type* pFormatStr, ...);

private:
    virtual void VUpdate() override;
    virtual void VScreenChanged() override
    { }

    static void AddPropertyEntry(const char_type* text, DDCheatValueType valueType, DDCheatValue valuePtr);

    s16 mFnIdx = 0;
    s16 mNextFnIdx = 0;
    s32 mInputPressed = 0;
    bool mUnknown1 = false;
    bool mTeleporting = false;
};


extern bool gDDCheat_FlyingEnabled;

extern s16 gRescuedMudokons;
extern s16 gKilledMudokons;

extern bool gVisitedBonewerkz;
extern bool gVisitedBarracks;
extern bool gVisitedFeecoEnder;
extern bool gDDCheat_ShowAI_Info;
