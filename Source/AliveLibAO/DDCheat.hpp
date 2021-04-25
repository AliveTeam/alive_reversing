#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
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

struct DDCheatProperty
{
    const s8* Name;
    DDCheatValueType ValueType;
    DDCheatValue ValuePtr;
};
ALIVE_ASSERT_SIZEOF(DDCheatProperty, 12);

class DDCheat : public BaseGameObject
{
public:
    EXPORT DDCheat* ctor_4095D0();

    static EXPORT void ClearProperties_4095B0();

    EXPORT BaseGameObject* dtor_409710();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT DDCheat* Vdtor_40A380(s32 flags);

    void AddPropertyEntry(const s8* text, DDCheatValueType valueType, DDCheatValue valuePtr);

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_4098C0();

    EXPORT void ScreenShot_409720();

    EXPORT void Teleport_409CE0();

    EXPORT void Misc_409E90();

    EXPORT static s32 DebugFont_Printf_498B40(s32 idx, const s8* formatStr, ...);

    EXPORT static s32 DebugStr_495990(const s8* pStr, ...);

    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24_input;
};
ALIVE_ASSERT_SIZEOF(DDCheat, 0x28);

ALIVE_VAR_EXTERN(s32, gTweak_X_5076D8);
ALIVE_VAR_EXTERN(s32, gTweak_Y_5076DC);

ALIVE_VAR_EXTERN(s16, sRescuedMudokons_5076C0);
ALIVE_VAR_EXTERN(s16, sKilledMudokons_5076BC);
ALIVE_VAR_EXTERN(s16, showDebugCreatureInfo_5076E0);
ALIVE_VAR_EXTERN(s16, sDDCheat_FlyingEnabled_50771C);

}
