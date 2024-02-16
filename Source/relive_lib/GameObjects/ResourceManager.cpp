#include "stdafx.h"
#include "ResourceManager.hpp"

ResourceManager::ResourceManager()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eResourceManager);
}

void ResourceManager::VUpdate()
{
    // Empty
}

void ResourceManager::VScreenChanged()
{
    // Empty
}
