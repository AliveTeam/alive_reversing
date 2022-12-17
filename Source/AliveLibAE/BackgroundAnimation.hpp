#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_BackgroundAnimation;
}

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
