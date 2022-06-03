#include "stdafx_ao.h"
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
    if (mLvlNumber == AO::LevelIds::eMenu_0 || mLvlNumber == AO::LevelIds::eCredits_10)
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

    static AO::DynamicArrayT<u8*> resourceArray{50};

    u8** ppRes = Add_Resource(resourceArray, AO::ResourceManager::Resource_Animation, animRec.mResourceId);
    Animation_Init_417FD0(animRec.mFrameTableOffset, animRec.mMaxW, animRec.mMaxH, ppRes, 1);
    mAnim.mAnimFlags.Set(AO::AnimFlags::eBit8_Loop);

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
            mAnim.LoadPal(ppPal, 0);
        }
    }
}

TestAnimation::TestAnimation()
{
    mLoaded = false;

    mTypeId = AO::Types::eNone_0;

    mApplyShadows &= ~1u;

    mGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mGameObjectFlags.Set(BaseGameObject::eCantKill_Bit11);
}

void TestAnimation::SyncToAbePos()
{
    mXPos = AO::sActiveHero->mXPos + FP_FromInteger(30);
    mYPos = AO::sActiveHero->mYPos - FP_FromInteger(30);
    mAnim.mRenderLayer = AO::sActiveHero->mAnim.mRenderLayer;
}

void TestAnimation::VUpdate()
{
    mPathNumber = AO::gMap.mCurrentPath;
    mLvlNumber = AO::gMap.mCurrentLevel;
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
