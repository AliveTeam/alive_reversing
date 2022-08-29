#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "../relive_lib/Animation.hpp"

struct Path_Mine final : public Path_TLV
{
    s16 field_10_num_patterns; // unused
    s16 field_12_pattern; // unused
    Scale_short mScale;
    s16 mDisabledResources;
    Choice_short mPersistOffscreen;
    s16 pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Mine, 0x1C);

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

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

private:
    bool IsColliding();

public:
    u16 field_118_detonating = 0;
    u16 field_11A_disabled_resources = 0;
    Guid field_11C_tlv;
    u32 field_120_gnframe = 0;
    Animation field_124_animation = {};
    BitField32<Mine_Flags_1BC> field_1BC_flags = {};
};
ALIVE_ASSERT_SIZEOF(Mine, 0x1C0);
