#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

enum class LevelIds : s16;

EXPORT void CCSTD FreeResourceArray_49AEC0(DynamicArrayT<u8*>* pArray);
EXPORT void CC LoadRockTypes_49AB30(LevelIds levelNumber, u16 pathNumber);

class ThrowableArray : public BaseGameObject
{
public:
    struct ThrowableArray_SaveState
    {
        s16 field_0_unused;
        s16 field_2_item_count;
    };
    ALIVE_ASSERT_SIZEOF_ALWAYS(ThrowableArray_SaveState, 0x4);

    EXPORT ThrowableArray* ctor_49A630();
    EXPORT void dtor_49A6F0();
    EXPORT BaseGameObject* vdtor_49A6C0(s32 flags);
    EXPORT void Remove_49AA00(s16 count);
    EXPORT void vUpdate_49AA50();
    EXPORT s32 vGetSaveState_49B2A0(u8* pSaveBuffer);
    EXPORT void vScreenChange_49AAA0();
    EXPORT void Add_49A7A0(s16 count);
    EXPORT static s32 CC CreateFromSaveState_49B200(const u8* pState);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VScreenChanged() override;
public:
    s16 field_20_count;
private:
    enum Flags_22
    {
        eBit1_Unknown = 0x1,
        eBit2_Unknown = 0x2,
        eBit3_Unknown = 0x4,
    };
    BitField16<Flags_22> field_22_flags;
    DynamicArrayT<u8*> field_24_throwables;
};
ALIVE_ASSERT_SIZEOF(ThrowableArray, 0x30);

ALIVE_VAR_EXTERN(ThrowableArray*, gpThrowableArray_5D1E2C);
