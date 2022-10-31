#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "Factory.hpp"
#include "../relive_lib/Layer.hpp"

class BackgroundAnimation final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BackgroundAnimation(relive::Path_BackgroundAnimation* pTlv, const Guid& tlvId);
    ~BackgroundAnimation();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;


private:
    //AnimationFileHeader** field_F4_res = nullptr;
    Guid field_F8_tlvInfo;
    FP mObjectXPos = {};
    FP mObjectYPos = {};
};
