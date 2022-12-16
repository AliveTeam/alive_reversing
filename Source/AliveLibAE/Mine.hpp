#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/Animation.hpp"

namespace relive
{
    struct Path_Mine;
}

class Mine final : public BaseAliveGameObject
{
public:
    Mine(relive::Path_Mine* pPath, const Guid& tlvId);
    ~Mine();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;

private:
    bool IsColliding();

public:
    bool mDetonating = false;
    Guid mTlvId;
    u32 mExplosionTimer = 0;
    Animation mFlashAnim;
    bool mPersistOffscreen = false;
};
