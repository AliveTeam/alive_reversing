#include "stdafx_ao.h"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Throwable.hpp"

namespace AO {

ALIVE_VAR(1, 0x50E26C, ThrowableArray*, gpThrowableArray_50E26C, nullptr);

void LoadRockTypes(EReliveLevelIds levelNumber, u16 path)
{
    bool bDoLoadingLoop = FALSE;
    const u8 throwableTypeIdx = Path_Get_Bly_Record_434650(levelNumber, path)->field_C_overlay_id & 0xFF;

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID, 0, 0))
    {
        bDoLoadingLoop = TRUE;
        ResourceManager::LoadResourceFile_4551E0("ABEPICK.BAN", 0, 0, 0);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID, 0, 0))
    {
        bDoLoadingLoop = TRUE;
        ResourceManager::LoadResourceFile_4551E0("ABETHROW.BAN", 0, 0, 0);
    }

    switch (word_4CF158[throwableTypeIdx])
    {
        case AOTypes::eGrenade_40:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_4551E0("EXPLO2.BAN", 0, 0, 0);
            }

            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_4551E0("ABEBLOW.BAN", 0, 0, 0);
            }

            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_4551E0("DOGBLOW.BAN", 0, 0, 0);
            }

            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_4551E0("METAL.BAN", 0, 0, 0);
            }

            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kGrenadeAOResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_4551E0("GRENADE.BAN", 0, 0, 0);
            }
            break;

        case AOTypes::eMeat_54:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMeatAOResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_4551E0("MEAT.BAN", 0, 0, 0);
            }
            break;

        case AOTypes::eRock_70:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAberockAOResID, 0, 0))
            {
                bDoLoadingLoop = TRUE;
                ResourceManager::LoadResourceFile_4551E0("PUIROCK.BAN", 0, 0, 0);
            }
            break;

        default:
            break;
    }

    if (bDoLoadingLoop)
    {
        ResourceManager::LoadingLoop_41EAD0(FALSE);
    }
}

void ThrowableArray::VRender(PrimHeader** /*ppOt*/)
{
    //Nothing to do here
}

void ThrowableArray::VUpdate()
{
    if (field_12_flags & 1)
    {
        LoadRockTypes(gMap.mCurrentLevel, gMap.mCurrentPath);
        Add(0);
        field_12_flags &= ~1u;
        mBaseGameObjectFlags.Clear(Options::eUpdatable_Bit2);
    }
}

void ThrowableArray::VScreenChanged()
{
    if (gMap.mNextLevel != EReliveLevelIds::eMenu && gMap.mNextLevel != EReliveLevelIds::eCredits)
    {
        if (word_4CF158[gMap.mOverlayId] != word_4CF158[gMap.GetOverlayId()])
        {
            if (!(field_12_flags & 1))
            {
                mBaseGameObjectFlags.Set(Options::eUpdatable_Bit2);
                field_12_flags |= 1;
                Remove(0);
            }
        }
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

ThrowableArray::~ThrowableArray()
{
    gpThrowableArray_50E26C = nullptr;
    if (field_10_count > 0)
    {
        Remove(field_10_count);
    }

}

ThrowableArray::ThrowableArray()
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Clear(Options::eUpdatable_Bit2);
    field_10_count = 0;
    gpThrowableArray_50E26C = this;
    field_12_flags &= ~7u;
}

void ThrowableArray::Remove(s16 count)
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
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID, 0, 0));
            ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID, 0, 0));
            field_12_flags &= ~4;
        }
    }

    u8** ppRes = nullptr;
    if (field_12_flags & 2)
    {
        switch (word_4CF158[gMap.mOverlayId])
        {
            case AOTypes::eGrenade_40:
                ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, 0, 0));
                ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 0, 0));
                ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, 0, 0);
                if (ppRes)
                {
                    ResourceManager::FreeResource_455550(ppRes);
                }

                ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, 0, 0));
                ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kGrenadeAOResID, 0, 0));
                ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kGrenadeAOResID, 0, 0);
                if (ppRes)
                {
                    ResourceManager::FreeResource_455550(ppRes);
                }
                break;

            case AOTypes::eMeat_54:
                ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMeatAOResID, 0, 0);
                ResourceManager::FreeResource_455550(ppRes);
                break;

            case AOTypes::eRock_70:
                ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAberockAOResID, 0, 0));
                ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAberockAOResID, 0, 0);
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

void ThrowableArray::Add(s16 count)
{
    if (field_10_count == 0)
    {
        if (!(field_12_flags & 4))
        {
            ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID, 1, 0);
            ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID, 1, 0);
            field_12_flags |= 4;
        }
    }

    if (field_10_count == 0 || (field_12_flags & 1))
    {
        if (!(field_12_flags & 2))
        {
            switch (word_4CF158[gMap.mOverlayId])
            {
                case AOTypes::eGrenade_40:
                    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, 1, 0);
                    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0);
                    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, 1, 0);
                    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, 1, 0);
                    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kGrenadeAOResID, 1, 0);
                    ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kGrenadeAOResID, 1, 0);
                    break;

                case AOTypes::eMeat_54:
                    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kMeatAOResID, 1, 0);
                    break;

                case AOTypes::eRock_70:
                    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAberockAOResID, 1, 0);
                    ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAberockAOResID, 1, 0);
                    break;

                default:
                    break;
            }

            field_12_flags |= 2;
        }
    }

    field_10_count += count;
}

} // namespace AO
