#include "stdafx_ao.h"
#include "TestAnimation.hpp"
#include "Abe.hpp"
#include "ResourceManager.hpp"
#include "AnimResources.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

static u8** Add_Resource(DynamicArrayT<u8 *>& resourceArray, u32 type, s32 resourceID)
{
    u8** ppRes = AO::ResourceManager::GetLoadedResource(type, resourceID, 1, 0);
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
    if (mBaseAnimatedWithPhysicsGameObject_LvlNumber == EReliveLevelIds::eMenu || mBaseAnimatedWithPhysicsGameObject_LvlNumber == EReliveLevelIds::eCredits)
    {
        return;
    }

    LOG_INFO("Test anim loading...");

    mLoaded = true; // Only do code below once

    const AnimRecord& animRec = AO::AnimRec(AnimId::Anim_Tester);
    if (!AO::ResourceManager::GetLoadedResource(AO::ResourceManager::Resource_Animation, animRec.mResourceId, 0, 0))
    {
        AO::ResourceManager::LoadResourceFileWrapper(animRec.mBanName, nullptr);
        LOG_INFO("Loading anim resource by BAN/BND name");
    }
    else
    {
        LOG_WARNING("Anim resource already loaded - BAN/BND name not verified/loaded by test animation");
    }

    static DynamicArrayT<u8*> resourceArray{50};

    u8** ppRes = ::Add_Resource(resourceArray, AO::ResourceManager::Resource_Animation, animRec.mResourceId);
    Animation_Init(animRec.mFrameTableOffset, animRec.mMaxW, animRec.mMaxH, ppRes, 1);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AO::AnimFlags::eBit8_Loop);

    if (animRec.mPalOverride != PalId::Default)
    {
        const PalRecord& palRec = PalRec(animRec.mPalOverride);

        if (!AO::ResourceManager::GetLoadedResource(AO::ResourceManager::Resource_Palt, palRec.mResourceId, 0, 0))
        {
            AO::ResourceManager::LoadResourceFileWrapper(palRec.mBanName, nullptr);
        }
        else
        {
            LOG_WARNING("Pal resource already loaded - BAN/BND name not verified by test animation");
        }

        u8** ppPal = ::Add_Resource(resourceArray, AO::ResourceManager::Resource_Palt, palRec.mResourceId);
        if (ppPal)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.LoadPal(ppPal, 0);
        }
    }
}

TestAnimation::TestAnimation()
{
    mLoaded = false;

    mBaseGameObjectTypeId = ReliveTypes::eNone;

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(BaseGameObject::eCantKill_Bit11);
}

void TestAnimation::SyncToAbePos()
{
    mBaseAnimatedWithPhysicsGameObject_XPos = AO::sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(30);
    mBaseAnimatedWithPhysicsGameObject_YPos = AO::sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(30);
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = AO::sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
}

void TestAnimation::VUpdate()
{
    mBaseAnimatedWithPhysicsGameObject_PathNumber = AO::gMap.mCurrentPath;
    mBaseAnimatedWithPhysicsGameObject_LvlNumber = AO::gMap.mCurrentLevel;
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
