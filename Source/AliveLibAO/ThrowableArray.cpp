#include "stdafx_ao.h"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Throwable.hpp"

START_NS_AO;

ALIVE_VAR(1, 0x50E26C, ThrowableArray*, gpThrowableArray_50E26C, nullptr);


EXPORT void CC LoadRockTypes_454370(LevelIds levelNumber, unsigned __int16 path)
{
    BOOL bDoLoadingLoop = FALSE;
    const BYTE throwableTypeIdx = Path_Get_Bly_Record_434650(levelNumber, path)->field_C_overlay_id & 0xFF;

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 12, 0, 0))
    {
        bDoLoadingLoop = TRUE;
        ResourceManager::LoadResourceFile_4551E0("ABEPICK.BAN", 0, 0, 0);
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 14, 0, 0))
    {
        bDoLoadingLoop = TRUE;
        ResourceManager::LoadResourceFile_4551E0("ABETHROW.BAN", 0, 0, 0);
    }

    switch (word_4CF158[throwableTypeIdx])
    {
    case Types::eGrenade_40:
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_4551E0("EXPLO2.BAN", 0, 0, 0);
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_4551E0("ABEBLOW.BAN", 0, 0, 0);
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 576, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_4551E0("DOGBLOW.BAN", 0, 0, 0);
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 365, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_4551E0("METAL.BAN", 0, 0, 0);
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6005, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_4551E0("GRENADE.BAN", 0, 0, 0);
        }
        break;

    case Types::eMeat_54:
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2023, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_4551E0("MEAT.BAN", 0, 0, 0);
        }
        break;

    case Types::eRock_70:
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 350, 0, 0))
        {
            bDoLoadingLoop = TRUE;
            ResourceManager::LoadResourceFile_4551E0("PUIROCK.BAN", 0, 0, 0);
        }
        break;
    }

    if (bDoLoadingLoop)
    {
        ResourceManager::LoadingLoop_41EAD0(FALSE);
    }
}

BaseGameObject* ThrowableArray::VDestructor(signed int flags)
{
    return Vdtor_454690(flags);
}

void ThrowableArray::VUpdate()
{
    VUpdate_4542B0();
}

void ThrowableArray::VRender(int** /*pOrderingTable*/)
{
    //Nothing to do here
}

void ThrowableArray::VScreenChanged()
{
    vScreenChange_454300();
}

BaseGameObject* ThrowableArray::Vdtor_454690(signed int flags)
{
    dtor_453F10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void ThrowableArray::VUpdate_4542B0()
{
    if (field_12_flags & 1)
    {
        LoadRockTypes_454370(gMap_507BA8.field_0_current_level, gMap_507BA8.field_2_current_path);
        Add_453F70(0);
        field_12_flags &= ~1u;
        field_6_flags.Clear(Options::eUpdatable_Bit2);
    }
}

void ThrowableArray::vScreenChange_454300()
{
    if (gMap_507BA8.field_A_level != LevelIds::eMenu_0 && gMap_507BA8.field_A_level != LevelIds::eCredits_10)
    {
        if (word_4CF158[gMap_507BA8.field_28_cd_or_overlay_num] != word_4CF158[gMap_507BA8.GetOverlayId_4440B0()])
        {
            if (!(field_12_flags & 1))
            {
                field_6_flags.Set(Options::eUpdatable_Bit2);
                field_12_flags |= 1;
                Remove_4540D0(0);
            }
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

BaseGameObject* ThrowableArray::dtor_453F10()
{
    gpThrowableArray_50E26C = nullptr;
    if (field_10_count > 0)
    {
        Remove_4540D0(field_10_count);
    }
    return dtor_487DF0();
}

ThrowableArray* ThrowableArray::ctor_453EE0()
{
    ctor_487E10(1);
    SetVTable(this, 0x4BC040);
    field_6_flags.Clear(Options::eUpdatable_Bit2);
    field_10_count = 0;
    gpThrowableArray_50E26C = this;
    field_12_flags &= ~7u;
    return this;
}

void ThrowableArray::Remove_4540D0(__int16 count)
{
    field_10_count -= count;

    if (field_10_count > 0)
    {
        if (!(field_12_flags & 1))
        {
            return;
        }
    }
    else
    {
        if (field_12_flags & 4)
        {
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 12, 0, 0));
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 14, 0, 0));
            field_12_flags &= ~4;
        }
    }

    BYTE** ppRes = nullptr;
    if (field_12_flags & 2)
    {
        switch (word_4CF158[gMap_507BA8.field_28_cd_or_overlay_num])
        {
        case Types::eGrenade_40:
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 0, 0));
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0));
            ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 576, 0, 0);
            if (ppRes)
            {
                ResourceManager::FreeResource_455550(ppRes);
            }

            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 365, 0, 0));
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6005, 0, 0));
            ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 6005, 0, 0);
            if (ppRes)
            {
                ResourceManager::FreeResource_455550(ppRes);
            }
            break;

        case Types::eMeat_54:
            ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2023, 0, 0);
              ResourceManager::FreeResource_455550(ppRes);
            break;

        case Types::eRock_70:
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 350, 0, 0));
            ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 350, 0, 0);
            if (ppRes)
            {
                 ResourceManager::FreeResource_455550(ppRes);
            }
            break;

        default:
            break;
        }
        field_12_flags &= ~2;
    }
}

void ThrowableArray::Add_453F70(__int16 count)
{
    if (field_10_count == 0)
    {
        if (!(field_12_flags & 4))
        {
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 12, 1, 0);
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 14, 1, 0);
            field_12_flags |= 4;
        }

        if (field_12_flags & 1)
        {
            if (!(field_12_flags & 2))
            {
                switch (word_4CF158[gMap_507BA8.field_28_cd_or_overlay_num])
                {
                case Types::eGrenade_40:
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 1, 0);
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 1, 0);
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 576, 1, 0);
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 365, 1, 0);
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6005, 1, 0);
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 6005, 1, 0);
                    break;

                case Types::eMeat_54:
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2023, 1, 0);
                    break;

                case Types::eRock_70:
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 350, 1, 0);
                    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 350, 1, 0);
                    break;

                default:
                    break;
                }

                field_12_flags |= 2;
            }
        }
    }

    field_10_count += count;
}

END_NS_AO;
