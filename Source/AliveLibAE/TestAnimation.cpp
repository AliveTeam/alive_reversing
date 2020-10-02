#include "stdafx.h"
#include "TestAnimation.hpp"
#include "Abe.hpp"
#include "ResourceManager.hpp"
#include "AnimResources.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

void TestAnimation::ctor()
{
    DisableVTableHack h;

    const AnimRecord& animRec = AnimRec(AnimId::AbeHeadGib);

    BaseAnimatedWithPhysicsGameObject_ctor_424930(1);
    field_4_typeId = Types::eNone_0;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, animRec.mResourceId, 0, 0))
    {
        ResourceManager::LoadResourceFile_49C170(animRec.mBanName, nullptr);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, animRec.mResourceId);
    Animation_Init_424E10(animRec.mFrameTableOffset, animRec.mMaxW, animRec.mMaxH, ppRes, 1, 1);

    field_DC_bApplyShadows &= ~1u;

    //auto ft = AnimRec(AnimId::AbeBodyGib).mFrameTableOffset;
    //field_20_animation.Set_Animation_Data_409C80(ft, nullptr);

    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);
    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
}

void TestAnimation::SyncToAbePos()
{
    field_B8_xpos = sActiveHero_5C1B68->field_B8_xpos + FP_FromInteger(30);
    field_BC_ypos = sActiveHero_5C1B68->field_BC_ypos - FP_FromInteger(30);

    field_20_animation.field_C_render_layer = sActiveHero_5C1B68->field_20_animation.field_C_render_layer;

    field_C0_path_number = gMap_5C3030.field_2_current_path;
    field_C2_lvl_number = gMap_5C3030.field_0_current_level;
}

void TestAnimation::VUpdate()
{
    SyncToAbePos();
}

void TestAnimation::VScreenChanged()
{

}

BaseGameObject* TestAnimation::VDestructor(signed int flags)
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

void TestAnimation::VRender(int** pOrderingTable)
{
    BaseAnimatedWithPhysicsGameObject::VRender(pOrderingTable);
}
