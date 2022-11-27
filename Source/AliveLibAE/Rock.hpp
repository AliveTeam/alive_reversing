#pragma once

#include "Throwable.hpp"
#include "../AliveLibCommon/BitField.hpp"

enum class LevelIds : s16;

enum class RockStates : s16
{
    eNone_0 = 0,
    eFallingOutOfRockSack_1 = 1,
    eRolling_2 = 2,
    eOnGround_3 = 3,
    eBouncing_4 = 4,
    eFallingOutOfWorld_5 = 5,
};

struct RockSaveState final
{
    ReliveTypes field_0_type;
    s16 field_2_padding;
    Guid field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    FP field_18_sprite_scale;
    s16 field_1C_path_number;
    EReliveLevelIds field_1E_lvl_number;
    bool mRender;
    bool mDrawable;
    bool mLoop;
    bool mInteractive;
    s16 field_22_padding;
    Guid field_24_id;
    s16 field_28_line_type;
    s16 field_2A_count;
    RockStates field_2C_state;
    s16 field_2E_volume;
    FP field_30_xpos;
    FP field_34_ypos;
};

class Rock final : public BaseThrowable
{
public:
    Rock(FP xpos, FP ypos, s16 count);
    ~Rock();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual bool VCanThrow() override;
    virtual bool VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;

    static s32 CreateFromSaveState(const u8* pData);

    virtual s16 VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override
    {
        return OnCollision(pPlatform);
    }

private:
    void InTheAir();
    s16 OnCollision(BaseAnimatedWithPhysicsGameObject* pObj);

    void BounceHorizontally( FP hitX, FP hitY );

private:
    RockStates field_11C_state = RockStates::eNone_0;
    s16 field_11E_volume = 0;
    FP field_120_xpos = {};
    FP field_124_ypos = {};
    s32 field_128_shimmer_timer = 0;
};
