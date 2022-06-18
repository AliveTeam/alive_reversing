#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"

struct Path_Mine final : public Path_TLV
{
    s16 field_10_num_patterns;
    s16 field_12_pattern;
    Scale_short field_14_scale;
    s16 field_16_disabled_resources;
    Choice_short field_18_persist_offscreen;
    s16 pad;
};
ALIVE_ASSERT_SIZEOF(Path_Mine, 0x1C);

enum Mine_Flags_1BC
{
    eBit0_Unused = 0x1,
    eBit1_PersistOffscreen = 0x2
};

class Mine final : public BaseAliveGameObject
{
public:
    Mine(Path_Mine* pPath, TlvItemInfoUnion tlv);
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
    TlvItemInfoUnion field_11C_tlv = {};
    u32 field_120_gnframe = 0;
    Animation field_124_animation = {};
    BitField32<Mine_Flags_1BC> field_1BC_flags = {};
};
ALIVE_ASSERT_SIZEOF(Mine, 0x1C0);
