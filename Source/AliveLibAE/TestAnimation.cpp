#include "stdafx.h"
#include "TestAnimation.hpp"
#include "Abe.hpp"
#include "AnimResources.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"

void TestAnimation::DelayLoad()
{
    // Trying to load on these lvls will result in a phat failure because they hardly have
    // any resource fiiles
    if (mCurrentLevel == EReliveLevelIds::eMenu || mCurrentLevel == EReliveLevelIds::eCredits)
    {
        return;
    }

    LOG_INFO("Test anim loading...");

    mLoaded = true; // Only do code below once

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Anim_Tester));
    Animation_Init(GetAnimRes(AnimId::Anim_Tester));
    mAnim.mFlags.Set(AnimFlags::eBit8_Loop);

    //if (animRec.mPalOverride != PalId::Default)
    //{
    //    const PalRecord& palRec = PalRec(animRec.mPalOverride);

    //    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, palRec.mResourceId, 0, 0))
    //    {
    //        ResourceManager::LoadResourceFile_49C170(palRec.mBanName, nullptr);
    //    }
    //    else
    //    {
    //        LOG_WARNING("Pal resource already loaded - BAN/BND name not verified by test animation");
    //    }

    //    u8** ppPal = Add_Resource(ResourceManager::Resource_Palt, palRec.mResourceId);
    //    if (ppPal)
    //    {
    //        mAnim.LoadPal(ppPal, 0);
    //    }
    //}
}

TestAnimation::TestAnimation()
    : BaseAnimatedWithPhysicsGameObject(1)
{
    mLoaded = false;

    SetType(ReliveTypes::eNone);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
}

void TestAnimation::SyncToAbePos()
{
    mXPos = sActiveHero->mXPos + FP_FromInteger(30);
    mYPos = sActiveHero->mYPos - FP_FromInteger(30);

    mAnim.mRenderLayer = sActiveHero->mAnim.mRenderLayer;
}

void TestAnimation::VUpdate()
{
    mCurrentPath = gMap.mCurrentPath;
    mCurrentLevel = gMap.mCurrentLevel;

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
