#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"

struct Path_ExplosionSet final : public Path_TLV
{
    Choice_short mStartEnabled;
    s16 mSwitchId;
    Choice_short mSpawnAssets;
    s16 mStartDelay;
    XDirection_short mStartDirection;
    s16 mAssetInterval;
    s16 mGridSpacing;
    s16 mIncreasingGridSpacing;
    Scale_short mScale;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ExplosionSet, 0x24);

class ExplosionSet final : public BaseGameObject
{
public:
    ExplosionSet();
    ~ExplosionSet();

    
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    void Init(Path_ExplosionSet* pTlv);
    void Start();

private:
    Prim_ScreenOffset field_20[2] = {};
    s16 field_40 = 0;
    s16 field_42 = 0;
    s16 field_44_start_delay = 0;
    s16 field_46_spacing_multiplicator = 0;
    PSX_RECT field_48_tlv_rect = {};
    FP field_50_scale = {};
    s16 field_54_switch_id = 0;
    s16 field_56_asset_interval = 0;
    s16 field_58_grid_spacing = 0;
    s16 field_5A_increasing_grid_spacing = 0;

    enum Flags_5C
    {
        eBit1_spawn_assets = 0x1,
        eBit2_flipX = 0x2,
        eBit3_Active = 0x4,
        eBit4 = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
        eBit8 = 0x80,
        eBit9 = 0x100,
        eBit10 = 0x200,
        eBit11 = 0x400,
        eBit12 = 0x800,
        eBit13 = 0x1000,
        eBit14 = 0x2000,
        eBit15 = 0x4000,
        eBit16 = 0x8000,
    };
    BitField16<Flags_5C> field_5C_flags = {};
    s16 field_5E = 0;
};
ALIVE_ASSERT_SIZEOF(ExplosionSet, 0x60);

ALIVE_VAR_EXTERN(ExplosionSet*, pExplosionSet_5BBF68);
ALIVE_VAR_EXTERN(s16, bEnabled_5C1BB6);
