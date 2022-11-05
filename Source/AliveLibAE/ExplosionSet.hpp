#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"

namespace relive
{
    struct Path_ExplosionSet;
}

class ExplosionSet final : public BaseGameObject
{
public:
    ExplosionSet();
    ~ExplosionSet();

    
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    void Init(relive::Path_ExplosionSet* pTlv);
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
        eBit3_Active = 0x4
    };
    BitField16<Flags_5C> field_5C_flags = {};
};

extern ExplosionSet* gExplosionSet;
extern bool gExplosionSetEnabled;
