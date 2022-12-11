#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"

enum class LevelIds : s16;

void FreeResourceArray_49AEC0(DynamicArrayT<u8*>* pArray);
void LoadRockTypes_49AB30(EReliveLevelIds levelNumber, u16 pathNumber);

struct ThrowableArraySaveState final
{
    ReliveTypes mType;
    s16 field_2_item_count;
};

class ThrowableArray final : public BaseGameObject
{
public:
    ThrowableArray();
    ~ThrowableArray();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VScreenChanged() override;

    static s32 CreateFromSaveState(const u8* pState);

    void Remove(s16 count);
    void Add(s16 count);

public:
    s16 mCount = 0;

private:
    bool mUnknown1 = false;
    bool mUnknown2 = false;
    bool mUnknown3 = false;
    DynamicArrayT<u8*> field_24_throwables;
};

extern ThrowableArray* gpThrowableArray;
