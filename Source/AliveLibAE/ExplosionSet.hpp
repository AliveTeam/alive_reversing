#pragma once

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
    bool mSpawnAssets = false;
    bool mFlipX = false;
    bool mActive = false;
};

extern ExplosionSet* gExplosionSet;
extern bool gExplosionSetEnabled;
