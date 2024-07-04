#pragma once

#include "BaseGameObject.hpp"
#include "../MapWrapper.hpp"
#include "../SaveStateBase.hpp"

enum class LevelIds : s16;

void LoadRockTypes(EReliveLevelIds level, u16 path);

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
};

extern ThrowableArray* gThrowableArray;
