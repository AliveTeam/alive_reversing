#include "stdafx.h"
#include "ThrowableArray.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "Throwable.hpp"

ALIVE_VAR(1, 0x5d1e2c, ThrowableArray*, gpThrowableArray, nullptr);

void FreeResourceArray_49AEC0(DynamicArrayT<u8*>* pArray)
{
    while (pArray->Size())
    {
        auto item = pArray->ItemAt(0);
        ResourceManager::FreeResource_49C330(item);
        pArray->Remove_Item(item);
    }
}

void LoadRockTypes_49AB30(EReliveLevelIds levelNumber, u16 pathNumber)
{
    bool bDoLoadingLoop = FALSE;
    const u8 throwableTypeIdx = LOBYTE(Path_Get_Bly_Record(levelNumber, pathNumber)->field_C_overlay_id);

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbepickResID, 0, 0))
    {
        bDoLoadingLoop = TRUE;
        ResourceManager::LoadResourceFile_49C130("ABEPICK.BAN", 0, 0, 0);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID, 0, 0))
    {
        bDoLoadingLoop = TRUE;
        ResourceManager::LoadResourceFile_49C130("ABETHROW.BAN", 0, 0, 0);
    }

    switch (throwable_types_55FAFC[throwableTypeIdx])
    {
        case AETypes::eBone_11:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kBoneResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("BONE.BAN", 0, 0, 0);
            }
            break;

        case AETypes::eMetal_24:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("METAL.BAN", 0, 0, 0);
            }

            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("GRENADE.BAN", 0, 0, 0);
            }
            break;

        case AETypes::eGrenade_65:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("EXPLO2.BAN", 0, 0, 0);
            }

            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("ABEBLOW.BAN", 0, 0, 0);
            }

            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("DOGBLOW.BAN", 0, 0, 0);
            }

            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("METAL.BAN", 0, 0, 0);
            }

            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("GRENADE.BAN", 0, 0, 0);
            }
            break;

        case AETypes::eMeat_84:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMeatResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("MEAT.BAN", 0, 0, 0);
            }
            break;

        case AETypes::eRock_105:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAberockResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_49C130("PUIROCK.BAN", 0, 0, 0);
            }
            break;

        default:
            break;
    }

    if (bDoLoadingLoop)
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    }
}

ThrowableArray::ThrowableArray()
    : BaseGameObject(TRUE, 0)
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
            u8** ppRes1 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbepickResID, 1, 0);
            if (ppRes1)
            {
                field_24_throwables.Push_Back(ppRes1);
            }

            u8** ppRes2 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID, 1, 0);
            if (ppRes2)
            {
                field_24_throwables.Push_Back(ppRes2);
            }

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
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kBoneResID);
                    break;

                case AETypes::eMetal_24:
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID);
                    //Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kGrenadeResID);
                    break;

                case AETypes::eGrenade_65:
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID);
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID);
                    //Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kGrenadeResID);
                    break;

                case AETypes::eMeat_84:
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMeatResID);
                    break;

                case AETypes::eRock_105:
                    //Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAberockResID);
                    //Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kAberockResID);
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
    // Empty 0x4DBF80
}

