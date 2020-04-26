#include "stdafx.h"
#include "Game.hpp"
#include "logger.hpp"
#include "Function.hpp"

void Game_ForceLink()
{

}


EXPORT void* alloc_450740(size_t)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT void* alloc_447520(size_t)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

template<typename T, typename... Args>
inline T* alive_new(Args&&... args)
{
    void* buffer = alloc_447520(sizeof(T));
    if (buffer)
    {
        return new (buffer) T(std::forward<Args>(args)...);
    }
    return nullptr;
}

EXPORT void Mem_Free_450770(void*)
{
    NOT_IMPLEMENTED();
}

class DynamicArray
{
public:
    EXPORT DynamicArray* ctor_4043E0(__int16 startingSize)
    {
        NOT_IMPLEMENTED();

        const auto sizeInBytes = startingSize * sizeof(void*);
        field_0_array = reinterpret_cast<void**>(alloc_450740(sizeInBytes));

        field_4_used_size = 0;
        field_6_max_size = 0;

        if (field_0_array)
        {
            memset(field_0_array, 0, sizeInBytes);
            field_4_used_size = 0;
            field_6_max_size = startingSize;
            field_8_expand_size = 10;
        }

        return this;
    }

    EXPORT void dtor_404440()
    {
        NOT_IMPLEMENTED();

        Mem_Free_450770(field_0_array);
    }

public:
    EXPORT __int16 Push_Back_404450(void *item)
    {
        //NOT_IMPLEMENTED();

        if (!item || !field_0_array)
        {
            return 0;
        }

        if (field_4_used_size == field_6_max_size)
        {
            // Inlined expand call?
            const __int16 newSizeElements = field_6_max_size + field_8_expand_size;
            const auto newArraySizeBytes = newSizeElements * sizeof(void*);
            auto pNewArray = reinterpret_cast<void**>(alloc_450740(newArraySizeBytes));
            if (!pNewArray)
            {
                return 0;
            }

            // Clear new memory + copy in old array
            memset(pNewArray, 0, newArraySizeBytes);
            memcpy(pNewArray, field_0_array, field_4_used_size * sizeof(void*));

            // Free old array + assign new values
            Mem_Free_450770(field_0_array);
            field_0_array = pNewArray;
            field_6_max_size = newSizeElements;
        }
        
        field_0_array[field_4_used_size++] = item;

        return 1;
    }

protected:

    EXPORT __int16 Remove_Item_404520(void *item)
    {
        // TODO: FIX ME
        NOT_IMPLEMENTED();

        int counter = 0;
        void* pCurItem = nullptr;
        for (;;)
        {
            if (counter < field_4_used_size)
            {
                counter++;
                pCurItem = field_0_array[counter];
            }
            else
            {
                pCurItem = nullptr;
            }

            if (!pCurItem)
            {
                break;
            }

            if (pCurItem == item)
            {
                field_4_used_size--;
                field_0_array[counter - 1] = field_0_array[field_4_used_size];
                field_0_array[field_4_used_size] = nullptr;
                return 1;
            }
        }
        return 0;
    }

    void** field_0_array;
public:
    __int16 field_4_used_size;
private:
    __int16 field_6_max_size;
    __int16 field_8_expand_size;
    __int16 field_A_pad;
};

ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);

ALIVE_VAR(1, 0x9F2DF0, DynamicArray*, gBaseGameObject_list_9F2DF0, nullptr);

EXPORT void CC Remove_Item_417350(void*)
{
    NOT_IMPLEMENTED();
}

class BaseGameObject
{
public:
    EXPORT BaseGameObject* ctor_487E10(__int16 arraySize)
    {
        SetVTable(this, 0x4BD488); // vTable_BaseGameObject_4BD488

        if (!this) // Compiler code or hack
        {
            return this;
        }

        field_8_update_delay = 0;
        field_4_typeId = 0;
        field_C_bCanKill = 0;

        field_6_flags &= ~0x3F5u;
        field_6_flags |= 2;

        if (!arraySize)
        {
            return this;
        }

        if (!gBaseGameObject_list_9F2DF0->Push_Back_404450(this))
        {
            field_6_flags |= 1u;
        }

        return this;
    }

    void dtor_487DF0()
    {
        SetVTable(this, 0x4BD488); // vTable_BaseGameObject_4BD488
        Remove_Item_417350(this);
    }

    virtual BaseGameObject* VDestructor(signed int flags) = 0;

    virtual void VUpdate()
    {

    }

    virtual void VRender(int**)
    {

    }

    virtual void VScreenChanged()
    {

    }

protected:
    //int field_0_VTable;
    __int16 field_4_typeId;
    unsigned __int16 field_6_flags;
    int field_8_update_delay;
    char field_C_bCanKill;
    char field_D;
    __int16 field_E;
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x10);

class ResourceManager
{
public:
    EXPORT void static CC Free_447540(void*)
    {
        NOT_IMPLEMENTED();
    }
};

class ObjectDumper : public BaseGameObject
{
public:
    ObjectDumper()
    {
        DisableVTableHack disableHack;

        ctor_487E10(1);
        LOG_INFO(field_6_flags);

        field_6_flags |= 0x300;
    }


    ~ObjectDumper()
    {
        LOG_INFO("dtor");
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        dtor_487DF0();
        if (flags & 1)
        {
            ResourceManager::Free_447540(this);
        }
        return this;
    }

    virtual void VUpdate() override
    {
        LOG_INFO("update");
    }

    virtual void VScreenChanged() override
    {
        LOG_INFO("Don't kill me!");
    }
};

EXPORT int CC Nop_49A750()
{
    LOG_INFO("Test");
    alive_new<ObjectDumper>();
    return 0;
}
