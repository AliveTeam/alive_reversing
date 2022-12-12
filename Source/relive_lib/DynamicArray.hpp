#pragma once

#include "../AliveLibCommon/Types.hpp"

// TODO: Can be made into a template when all usages are reversed.
class DynamicArray
{
public:
    explicit DynamicArray(s32 startingSize);
    ~DynamicArray();

    DynamicArray(const DynamicArray& rhs) = delete;
    DynamicArray& operator=(const DynamicArray& rhs) const = delete;

    s16 Expand(s16 expandSize);

    bool IsEmpty() const
    {
        return mUsedSize == 0;
    }

    s32 Size() const
    {
        return mUsedSize;
    }

    s32 RemoveAt(s32 idx)
    {
        mUsedSize--;

        // Overwrite the items to remove with the item from the end
        mArray[idx] = mArray[mUsedSize];

        return idx - 1;
    }

public:
    s16 Push_Back(void* pValue);

protected:
    void Remove_Item(void* pItemToRemove);
    void** mArray = nullptr;

public:
    s16 mUsedSize = 0;

private:
    s16 mMaxSize = 0;
    s16 mExpandSizeBy = 0;
};

// Typed wrapper for DynamicArray
template <class T>
class DynamicArrayT final : public DynamicArray
{
public:
    using DynamicArray::DynamicArray;

    s16 Push_Back(T* pValue)
    {
        return DynamicArray::Push_Back(pValue);
    }

    void Remove_Item(T* pItemToRemove)
    {
        DynamicArray::Remove_Item(pItemToRemove);
    }

    T* ItemAt(s32 idx)
    {
        return reinterpret_cast<T*>(mArray[idx]);
    }

    void SetAt(s32 idx, T* itemToSet)
    {
        mArray[idx] = itemToSet;
    }
};
