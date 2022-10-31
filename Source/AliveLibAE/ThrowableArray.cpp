#include "stdafx.h"
#include "ThrowableArray.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "Throwable.hpp"

ThrowableArray* gpThrowableArray = nullptr;

void FreeResourceArray_49AEC0(DynamicArrayT<u8*>* pArray)
{
    while (pArray->Size())
    {
        auto item = pArray->ItemAt(0);
        pArray->Remove_Item(item);
    }
}

void LoadRockTypes_49AB30(EReliveLevelIds levelNumber, u16 pathNumber)
{
    bool bDoLoadingLoop = false;
    const u8 throwableTypeIdx = Path_Get_Bly_Record(levelNumber, pathNumber)->field_C_overlay_id & 0xFF;

    switch (throwable_types_55FAFC[throwableTypeIdx])
    {
        case AETypes::eBone_11:
            break;

        case AETypes::eMetal_24:

            break;

        case AETypes::eGrenade_65:

            break;

        case AETypes::eMeat_84:
            break;

        case AETypes::eRock_105:
            break;

        default:
            break;
    }

    if (bDoLoadingLoop)
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(false);
    }
}

ThrowableArray::ThrowableArray()
    : BaseGameObject(true, 0)
    , field_24_throwables(0)
{
    mBaseGameObjectFlags.Clear(BaseGameObject::eUpdatable_Bit2);
    field_20_count = 0;
    gpThrowableArray = this;
    field_22_flags.Clear(Flags_22::eBit1_Unknown);
    field_22_flags.Clear(Flags_22::eBit2_Unknown);
    field_22_flags.Clear(Flags_22::eBit3_Unknown);
}

ThrowableArray::~ThrowableArray()
{
    gpThrowableArray = nullptr;
    FreeResourceArray_49AEC0(&field_24_throwables);
}

void ThrowableArray::Remove(s16 count)
{
    field_20_count -= count;
    if (field_20_count > 0)
    {
        if (field_22_flags.Get(Flags_22::eBit1_Unknown) && field_22_flags.Get(Flags_22::eBit2_Unknown))
        {
            FreeResourceArray_49AEC0(&field_10_resources_array);
            field_22_flags.Clear(Flags_22::eBit2_Unknown);
        }
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ThrowableArray::VUpdate()
{
    if (field_22_flags.Get(Flags_22::eBit1_Unknown))
    {
        LoadRockTypes_49AB30(gMap.mCurrentLevel, gMap.mCurrentPath);
        Add(0);
        field_22_flags.Clear(Flags_22::eBit1_Unknown);
        mBaseGameObjectFlags.Clear(BaseGameObject::eUpdatable_Bit2);
    }
}

s32 ThrowableArray::VGetSaveState(u8* pSaveBuffer)
{
    ThrowableArray_SaveState* pState = reinterpret_cast<ThrowableArray_SaveState*>(pSaveBuffer);
    pState->field_0_unused = 102; // never gets read back, no idea what it means :)
    pState->field_2_item_count = field_20_count;
    return sizeof(ThrowableArray_SaveState);
}

void ThrowableArray::VScreenChanged()
{
    if (gMap.mNextLevel != EReliveLevelIds::eMenu && gMap.mNextLevel != EReliveLevelIds::eCredits)
    {
        if (throwable_types_55FAFC[gMap.mOverlayId] != throwable_types_55FAFC[gMap.GetOverlayId()])
        {
            if (!(field_22_flags.Get(Flags_22::eBit1_Unknown)))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eUpdatable_Bit2);
                field_22_flags.Set(Flags_22::eBit1_Unknown);
                Remove(0);
            }
        }
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ThrowableArray::Add(s16 count)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDead);
    }

    if (field_20_count == 0)
    {
        if (!field_22_flags.Get(Flags_22::eBit3_Unknown))
        {


            field_22_flags.Set(Flags_22::eBit3_Unknown);
        }
    }

    if (field_20_count == 0 || field_22_flags.Get(Flags_22::eBit1_Unknown))
    {
        if (!field_22_flags.Get(Flags_22::eBit2_Unknown))
        {
            switch (throwable_types_55FAFC[gMap.mOverlayId])
            {
                case AETypes::eBone_11:
                    break;

                case AETypes::eMetal_24:
                    break;

                case AETypes::eGrenade_65:
                    break;

                case AETypes::eMeat_84:
                    break;

                case AETypes::eRock_105:
                    break;

                default:
                    break;
            }

            field_22_flags.Set(Flags_22::eBit2_Unknown);
        }
    }

    field_20_count += count;
}

s32 ThrowableArray::CreateFromSaveState(const u8* pState)
{
    LoadRockTypes_49AB30(gMap.mCurrentLevel, gMap.mCurrentPath);
    auto pArray = relive_new ThrowableArray();
    pArray->Add(reinterpret_cast<const ThrowableArray_SaveState*>(pState)->field_2_item_count);
    return sizeof(ThrowableArray_SaveState);
}

void ThrowableArray::VRender(PrimHeader** /*ppOt*/)
{
    // Nothing to do here
}

