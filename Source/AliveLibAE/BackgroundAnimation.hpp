#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "../relive_lib/Layer.hpp"

struct Path_BackgroundAnimation final : public Path_TLV
{
    u16 mAnimId;
    Choice_short mIsSemiTrans;
    TPageAbr mSemiTransMode;
    // pad
    s16 field_16_sound_effect; // unused
    s16 field_18_id; // probably also unused
    Layer mLayer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BackgroundAnimation, 0x1C);

class BackgroundAnimation final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    BackgroundAnimation(relive::Path_BackgroundAnimation* pPathParams, const TLVUniqueId& tlvId);
    ~BackgroundAnimation();

private:
    AnimationFileHeader** field_F4_res = nullptr;
    TLVUniqueId field_F8_tlvInfo;
    FP mObjectXPos = {};
    FP mObjectYPos = {};
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x104);
