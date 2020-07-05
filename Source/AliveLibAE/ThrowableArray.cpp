#include "stdafx.h"
#include "ThrowableArray.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "Throwable.hpp"

ALIVE_VAR(1, 0x5d1e2c, ThrowableArray*, gpThrowableArray_5D1E2C, nullptr);

void CCSTD FreeResourceArray_49AEC0(DynamicArrayT<BYTE*>* pArray)
{
    while (pArray->Size())
    {
        auto item = pArray->ItemAt(0);
        ResourceManager::FreeResource_49C330(item);
        pArray->Remove_Item(item);
    }
}

void CC LoadRockTypes_49AB30(LevelIds levelNumber, unsigned __int16 pathNumber)
{
    BOOL bDoLoadingLoop = FALSE;
    const BYTE throwableTypeIdx = LOBYTE(Path_Get_Bly_Record_460F30(levelNumber, pathNumber)->field_C_overlay_id);

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbepickResID, 0, 0))
    {
        bDoLoadingLoop = TRUE;
        ResourceManager::LoadResourceFile_49C130("ABEPICK.BAN", 0, 0, 0);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbethrowResID, 0, 0))
    {
        bDoLoadingLoop = TRUE;
        ResourceManager::LoadResourceFile_49C130("ABETHROW.BAN", 0, 0, 0);
    }

    switch (word_55FAFC[throwableTypeIdx])
    {
    case Types::eBone_11:
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kUnknownResID_591, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_49C130("BONE.BAN", 0, 0, 0);
        }
        break;

    case Types::eType_24:
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMetalResID, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_49C130("METAL.BAN", 0, 0, 0);
        }

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kGrenadeResID, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_49C130("GRENADE.BAN", 0, 0, 0);
        }
        break;

    case Types::eGrenade_65:
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_49C130("EXPLO2.BAN", 0, 0, 0);
        }

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_49C130("ABEBLOW.BAN", 0, 0, 0);
        }

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_49C130("DOGBLOW.BAN", 0, 0, 0);
        }

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMetalResID, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_49C130("METAL.BAN", 0, 0, 0);
        }

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kGrenadeResID, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_49C130("GRENADE.BAN", 0, 0, 0);
        }
        break;

    case Types::eMeat_84:
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMeatResID, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_49C130("MEAT.BAN", 0, 0, 0);
        }
        break;

    case Types::eRock_105:
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAberockResID, 0, 0))
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

ThrowableArray* ThrowableArray::ctor_49A630()
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    field_24_throwables.ctor_40C9E0(0);
    field_6_flags.Clear(BaseGameObject::eUpdatable_Bit2);
    SetVTable(this, 0x5469E4);
    field_20_count = 0;
    gpThrowableArray_5D1E2C = this;
    field_22_flags &= ~7u;
    return this;
}

void ThrowableArray::dtor_49A6F0()
{
    SetVTable(this, 0x5469E4);
    gpThrowableArray_5D1E2C = 0;
    FreeResourceArray_49AEC0(&field_24_throwables);
    field_24_throwables.dtor_40CAD0();
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* ThrowableArray::vdtor_49A6C0(signed int flags)
{
    dtor_49A6F0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void ThrowableArray::Remove_49AA00(__int16 count)
{
    field_20_count -= count;
    if (field_20_count > 0)
    {
        if (field_22_flags & 1 && field_22_flags & 2)
        {
            FreeResourceArray_49AEC0(&field_10_resources_array);
            field_22_flags &= ~2u;
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void ThrowableArray::vUpdate_49AA50()
{
    if (field_22_flags & 1)
    {
        LoadRockTypes_49AB30(gMap_5C3030.field_0_current_level, gMap_5C3030.field_2_current_path);
        Add_49A7A0(0);
        field_22_flags &= ~1u;
        field_6_flags.Clear(BaseGameObject::eUpdatable_Bit2);
    }
}

signed int ThrowableArray::vGetSaveState_49B2A0(BYTE* pSaveBuffer)
{
    ThrowableArray_SaveState* pState = reinterpret_cast<ThrowableArray_SaveState*>(pSaveBuffer);
    pState->field_0_never_used = 102; // never gets read back, no idea what it means :)
    pState->field_2_item_count = field_20_count;
    return sizeof(ThrowableArray_SaveState);
}

void ThrowableArray::vScreenChange_49AAA0()
{
    if (gMap_5C3030.field_A_level != LevelIds::eMenu_0 && gMap_5C3030.field_A_level != LevelIds::eCredits_16)
    {
        if (word_55FAFC[gMap_5C3030.field_22_overlayID] != word_55FAFC[gMap_5C3030.GetOverlayId_480710()])
        {
            if (!(field_22_flags & 1))
            {
                field_6_flags.Set(BaseGameObject::eUpdatable_Bit2);
                field_22_flags |= 1;
                Remove_49AA00(0);
            }
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

}

void ThrowableArray::Add_49A7A0(__int16 count)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        field_6_flags.Clear(BaseGameObject::eDead_Bit3);
    }

    if (field_20_count == 0)
    {
        if (!(field_22_flags & 4))
        {
            BYTE** ppRes1 = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbepickResID, 1, 0);
            if (ppRes1)
            {
                field_24_throwables.Push_Back_40CAF0(ppRes1);
            }

            BYTE** ppRes2 = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbethrowResID, 1, 0);
            if (ppRes2)
            {
                field_24_throwables.Push_Back_40CAF0(ppRes2);
            }

            field_22_flags |= 4u;
        }
    }

    if (field_20_count == 0 || field_22_flags & 1)
    {
        if (!(field_22_flags & 2))
        {
            switch (word_55FAFC[gMap_5C3030.field_22_overlayID])
            {
            case Types::eBone_11:
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kUnknownResID_591);
                break;

            case Types::eType_24:
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMetalResID);
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kGrenadeResID);
                Add_Resource_4DC130(ResourceManager::Resource_Palt, ResourceID::kGrenadeResID);
                break;

            case Types::eGrenade_65:
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID);
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID);
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID);
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMetalResID);
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kGrenadeResID);
                Add_Resource_4DC130(ResourceManager::Resource_Palt, ResourceID::kGrenadeResID);
                break;

            case Types::eMeat_84:
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMeatResID);
                break;

            case Types::eRock_105:
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAberockResID);
                Add_Resource_4DC130(ResourceManager::Resource_Palt, ResourceID::kAberockResID);
                break;

            default:
                break;
            }

            field_22_flags |= 2u;
        }
    }

    field_20_count += count;
}

signed int CC ThrowableArray::CreateFromSaveState_49B200(const BYTE* pState)
{
    LoadRockTypes_49AB30(gMap_5C3030.field_0_current_level, gMap_5C3030.field_2_current_path);
    auto pArray = ae_new<ThrowableArray>();
    if (pArray)
    {
        pArray->ctor_49A630();
    }
    pArray->Add_49A7A0(reinterpret_cast<const ThrowableArray_SaveState*>(pState)->field_2_item_count);
    return sizeof(ThrowableArray_SaveState);
}

BaseGameObject* ThrowableArray::VDestructor(signed int flags)
{
    return vdtor_49A6C0(flags);
}

void ThrowableArray::VUpdate()
{
    vUpdate_49AA50();
}

void ThrowableArray::VRender(int** /*pOrderingTable*/)
{
    // Empty 0x4DBF80
}

int ThrowableArray::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_49B2A0(pSaveBuffer);
}

void ThrowableArray::VScreenChanged()
{
    vScreenChange_49AAA0();
}
