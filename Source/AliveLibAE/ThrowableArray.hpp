#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/SaveStateBase.hpp"

enum class LevelIds : s16;

void FreeResourceArray(DynamicArrayT<u8*>* pArray);
void LoadRockTypes(EReliveLevelIds levelNumber, u16 pathNumber);

struct ThrowableArraySaveState final : public SaveStateBase
{
    ThrowableArraySaveState()
        : SaveStateBase(ReliveTypes::eThrowableArray, sizeof(*this))
    { }
    s16 mCount = 0;
};

class ThrowableArray final : public BaseGameObject
{
public:
    ThrowableArray();
    ~ThrowableArray();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    virtual void VScreenChanged() override;

    static void CreateFromSaveState(SerializedObjectData& pState);

    void Remove(s16 count);
    void Add(s16 count);

public:
    s16 mCount = 0;

private:
    bool mThrowableTypeChanged = false;
    bool mNewThrowableTypeLoaded = false;
    DynamicArrayT<u8*> field_24_throwables;
};

extern ThrowableArray* gThrowableArray;
