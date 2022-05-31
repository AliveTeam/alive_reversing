#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/AddPointer.hpp"

class BaseThrowable;

BaseThrowable* Make_Throwable_49AF30(FP xpos, FP ypos, s16 count);

// NOTE: This base type must exist but seems to have been decimated by the compiler, so this contains pure virtuals for
// non common virtuals, and virtuals for common virtuals.
class BaseThrowable : public BaseAliveGameObject
{
public:
    explicit BaseThrowable(s16 resourceArraySize);

    virtual void VOnPickUpOrSlapped() override;

    // New virtuals for throwables
    virtual void VThrow_49E460(FP velX, FP velY) = 0;
    virtual Bool32 VCanThrow_49E350() = 0;
    virtual Bool32 VIsFalling_49E330() = 0;
    virtual void VTimeToExplodeRandom_411490() = 0;

    virtual s16 VGetCount_448080();

    virtual void VToDead_4114B0();

    using FnTypeMatcher = AddPointer_t<bool(AETypes type)>;
    void BaseAddToPlatform(FnTypeMatcher cb);

private:
    void vToDead_4114B0();
    void vOnPickUpOrSlapped_4114D0();

    s16 vGetCount_448080();

protected:
    s16 field_118_count;
    s16 field_11A_bDead;
};

ALIVE_VAR_EXTERN(u16, gInfiniteGrenades_5C1BDE);
extern const AETypes throwable_types_55FAFC[252];
