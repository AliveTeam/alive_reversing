#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "../relive_lib/Layer.hpp"

class BackgroundAnimation final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    BackgroundAnimation(relive::Path_BackgroundAnimation* pPathParams, const Guid& tlvId);
    ~BackgroundAnimation();

private:
    //AnimationFileHeader** field_F4_res = nullptr;
    Guid field_F8_tlvInfo;
    FP mObjectXPos = {};
    FP mObjectYPos = {};
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x104);
