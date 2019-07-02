#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include <type_traits>

class BaseThrowable;

EXPORT BaseThrowable* CCSTD Make_Throwable_49AF30(FP xpos, FP ypos, short count);

// NOTE: This base type must exist but seems to have been decimated by the compiler, so this contains pure virtuals for
// non common virtuals, and virtuals for common virtuals.
class BaseThrowable : public BaseAliveGameObject
{
public:
    virtual void VOnPickUpOrSlapped() override;

    // New virtuals for throwables
    virtual void VThrow_49E460(FP velX, FP velY) = 0;
    virtual BOOL VCanThrow_49E350() = 0;
    virtual BOOL VIsFalling_49E330() = 0;
    virtual void VTimeToExplodeRandom_411490() = 0;

    virtual __int16 VGetCount_448080();

    virtual void VToDead_4114B0();

    using FnTypeMatcher = std::add_pointer<bool(Types type)>::type;
    void BaseAddToPlatform(FnTypeMatcher cb);
private:
    EXPORT void vToDead_4114B0();
    EXPORT void vOnPickUpOrSlapped_4114D0();

    EXPORT __int16 vGetCount_448080();
protected:
    __int16 field_118_count;
    __int16 field_11A_bDead;
};

ALIVE_VAR_EXTERN(WORD, gInfiniteGrenades_5C1BDE);
extern const Types word_55FAFC[252];
