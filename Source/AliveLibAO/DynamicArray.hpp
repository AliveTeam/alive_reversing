#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/Types.hpp"
#include "../AliveLibCommon/logger.hpp"

namespace AO {

class DynamicArray
{
public:
    explicit DynamicArray(s32 startingSize);
    ~DynamicArray();

    DynamicArray(const DynamicArray& rhs) = delete;
    DynamicArray& operator=(const DynamicArray&) const = delete;

protected:
    s16 Push_Back(void* item);
    s16 Remove_Item(void* item);

public:
    void** field_0_array = nullptr;

public:
    s16 field_4_used_size = 0;

private:
    s16 field_6_max_size = 0;
    s16 field_8_expand_size = 0;
};
ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);


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

    s16 Remove_Item(T* pItemToRemove)
    {
        return DynamicArray::Remove_Item(pItemToRemove);
    }

    T* ItemAt(s32 idx)
    {
        if (idx < 0 || idx >= field_4_used_size)
        {
            LOG_ERROR(idx << " is out of bounds max= " << field_4_used_size);
        }
        return reinterpret_cast<T*>(field_0_array[idx]);
    }

    void SetAt(s32 idx, T* itemToSet)
    {
        field_0_array[idx] = itemToSet;
    }

    s32 Size() const
    {
        return field_4_used_size;
    }

    bool Empty() const
    {
        return field_4_used_size == 0;
    }

    s32 RemoveAt(s32 idx)
    {
        field_4_used_size--;

        // Overwrite the items to remove with the item from the end
        field_0_array[idx] = field_0_array[field_4_used_size];

        return idx - 1;
    }
};

} // namespace AO
