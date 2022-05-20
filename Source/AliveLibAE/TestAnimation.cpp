#include "stdafx.h"
#include "TestAnimation.hpp"
#include "Abe.hpp"
#include "ResourceManager.hpp"
#include "AnimResources.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"


void TestAnimation::DelayLoad()
{
    // Trying to load on these lvls will result in a phat failure because they hardly have
    // any resource fiiles
    if (field_C2_lvl_number == LevelIds::eMenu_0 || field_C2_lvl_number == LevelIds::eCredits_16)
    {
        return;
    }

    LOG_INFO("Test anim loading...");

    mLoaded = true; // Only do code below once

    const AnimRecord& animRec = AnimRec(AnimId::Anim_Tester);

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, animRec.mResourceId, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170(animRec.mBanName, nullptr);
    }
    else
    {
        LOG_WARNING("Anim resource already loaded - BAN/BND name not verified by test animation");
    }

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, animRec.mResourceId);
    Animation_Init(animRec.mFrameTableOffset, animRec.mMaxW, animRec.mMaxH, ppRes, 1, 1);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);

    if (animRec.mPalOverride != PalId::Default)
    {
        const PalRecord& palRec = PalRec(animRec.mPalOverride);

        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, palRec.mResourceId, 0, 0))
        {
            ResourceManager::LoadResourceFile_49C170(palRec.mBanName, nullptr);
        }
        else
        {
            LOG_WARNING("Pal resource already loaded - BAN/BND name not verified by test animation");
        }

        u8** ppPal = Add_Resource(ResourceManager::Resource_Palt, palRec.mResourceId);
        if (ppPal)
        {
            field_20_animation.Load_Pal_40A530(ppPal, 0);
        }
    }
}

void TestAnimation::ctor()
{
    mLoaded = false;

    DisableVTableHack h;

    BaseAnimatedWithPhysicsGameObject_ctor_424930(1);
    SetType(AETypes::eNone_0);

    field_DC_bApplyShadows &= ~1u;

    mFlags.Set(BaseGameObject::eDrawable_Bit4);
    mFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
}

void TestAnimation::SyncToAbePos()
{
    field_B8_xpos = sActiveHero_5C1B68->field_B8_xpos + FP_FromInteger(30);
    field_BC_ypos = sActiveHero_5C1B68->field_BC_ypos - FP_FromInteger(30);

    field_20_animation.field_C_render_layer = sActiveHero_5C1B68->field_20_animation.field_C_render_layer;
}

void TestAnimation::VUpdate()
{
    field_C0_path_number = gMap.mCurrentPath;
    field_C2_lvl_number = gMap.mCurrentLevel;

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

BaseGameObject* TestAnimation::VDestructor(s32 flags)
{
    // TODO: Destruction of this object will likely crash, but this obj can be removed
    // once all animations are added to the table
    // Destruct();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void TestAnimation::VRender(PrimHeader** ppOt)
{
    if (mLoaded)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}
