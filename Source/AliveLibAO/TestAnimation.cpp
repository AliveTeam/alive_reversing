#include "stdafx.h"
#include "TestAnimation.hpp"
#include "Abe.hpp"
#include "ResourceManager.hpp"
#include "AnimResources.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

static u8** Add_Resource(AO::DynamicArrayT<u8 *>& resourceArray, u32 type, s32 resourceID)
{
    u8** ppRes = AO::ResourceManager::GetLoadedResource_4554F0(type, resourceID, 1, 0);
    if (ppRes)
    {
        resourceArray.Push_Back(ppRes);
    }
    return ppRes;
}

void TestAnimation::DelayLoad()
{
    // Trying to load on these lvls will result in a phat failure because they hardly have
    // any resource files
    if (field_B2_lvl_number == AO::LevelIds::eMenu_0 || field_B2_lvl_number == AO::LevelIds::eCredits_10)
    {
        return;
    }

    LOG_INFO("Test anim loading...");

    mLoaded = true; // Only do code below once

    const AnimRecord& animRec = AO::AnimRec(AnimId::Anim_Tester);
    if (!AO::ResourceManager::GetLoadedResource_4554F0(AO::ResourceManager::Resource_Animation, animRec.mResourceId, 0, 0))
    {
        AO::ResourceManager::LoadResourceFileWrapper(animRec.mBanName, nullptr);
        LOG_INFO("Loading anim resource by BAN/BND name");
    }
    else
    {
        LOG_WARNING("Anim resource already loaded - BAN/BND name not verified/loaded by test animation");
    }

    static AO::DynamicArrayT<u8 *> resourceArray;

    u8** ppRes = Add_Resource(resourceArray, AO::ResourceManager::Resource_Animation, animRec.mResourceId);
    Animation_Init_417FD0(animRec.mFrameTableOffset, animRec.mMaxW, animRec.mMaxH, ppRes, 1);
    field_10_anim.field_4_flags.Set(AO::AnimFlags::eBit8_Loop);

    if (animRec.mPalOverride != PalId::Default)
    {
        const PalRecord& palRec = PalRec(animRec.mPalOverride);

        if (!AO::ResourceManager::GetLoadedResource_4554F0(AO::ResourceManager::Resource_Palt, palRec.mResourceId, 0, 0))
        {
            AO::ResourceManager::LoadResourceFileWrapper(palRec.mBanName, nullptr);
        }
        else
        {
            LOG_WARNING("Pal resource already loaded - BAN/BND name not verified by test animation");
        }

        u8** ppPal = Add_Resource(resourceArray, AO::ResourceManager::Resource_Palt, palRec.mResourceId);
        if (ppPal)
        {
            field_10_anim.LoadPal(ppPal, 0);
        }
    }
}

TestAnimation::TestAnimation()
{
    mLoaded = false;

    field_4_typeId = AO::Types::eNone_0;

    field_CC_bApplyShadows &= ~1u;

    mFlags.Set(BaseGameObject::eDrawable_Bit4);
    mFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mFlags.Set(BaseGameObject::eCantKill_Bit11);
}

void TestAnimation::SyncToAbePos()
{
    field_A8_xpos = AO::sActiveHero_507678->field_A8_xpos + FP_FromInteger(30);
    field_AC_ypos = AO::sActiveHero_507678->field_AC_ypos - FP_FromInteger(30);
    field_10_anim.field_C_layer = AO::sActiveHero_507678->field_10_anim.field_C_layer;
}

void TestAnimation::VUpdate()
{
    field_B0_path_number = AO::gMap.mCurrentPath;
    field_B2_lvl_number = AO::gMap.mCurrentLevel;
    if (mLoaded)
    {
        SyncToAbePos();
    }
    else
    {
        DelayLoad();
    }
}

void TestAnimation::VScreenChanged()
{
    // Keep alive
}

void TestAnimation::VRender(PrimHeader** ppOt)
{
    if (mLoaded)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}
