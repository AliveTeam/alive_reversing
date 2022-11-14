#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Factory.hpp"
#include "../relive_lib/Animation.hpp"


enum Mine_Flags_1BC
{
    eBit0_Unused = 0x1,
    eBit1_PersistOffscreen = 0x2
};

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
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

private:
    bool IsColliding();

public:
    bool mDetonating = false;
    Guid mTlvId;
    u32 mExplosionTimer = 0;
    Animation mFlashAnim;
    BitField32<Mine_Flags_1BC> field_1BC_flags = {};
};
