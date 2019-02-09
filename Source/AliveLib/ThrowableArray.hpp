#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

enum class LevelIds : __int16;

EXPORT void CCSTD FreeResourceArray_49AEC0(DynamicArray*);
EXPORT void CC LoadRockTypes_49AB30(LevelIds /*a1*/, unsigned __int16 /*a2*/);

class ThrowableArray : public BaseGameObject
{
public:
    struct ThrowableArray_SaveState
    {
        short field_0_never_used;
        short field_2_item_count;
    };
    ALIVE_ASSERT_SIZEOF(ThrowableArray_SaveState, 0x4);

    EXPORT ThrowableArray* ctor_49A630();
    EXPORT void dtor_49A6F0();
    EXPORT BaseGameObject* vdtor_49A6C0(signed int flags);
    EXPORT void Remove_49AA00(__int16 count);
    EXPORT void vUpdate_49AA50();
    EXPORT signed int vGetSaveState_49B2A0(BYTE* pSaveBuffer);
    EXPORT void vScreenChange_49AAA0();
    EXPORT void Add_49A7A0(__int16 count);
    EXPORT static signed int CC CreateFromSaveState_49B200(const BYTE* pState);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual void VScreenChanged() override;
private:
    __int16 field_20_count;
    __int16 field_22_flags;
    DynamicArray field_24_throwables;
};
ALIVE_ASSERT_SIZEOF(ThrowableArray, 0x30);

ALIVE_VAR_EXTERN(ThrowableArray*, gpThrowableArray_5D1E2C);
