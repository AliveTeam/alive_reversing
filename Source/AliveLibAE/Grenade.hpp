#pragma once

#include "Throwable.hpp"

enum class GrenadeStates : s16
{
    eFallingToBeCollected_0 = 0,
    eWaitToBeCollected_1 = 1,
    eDoesNothing_2 = 2,
    eCountingDown_3 = 3,
    eFalling_4 = 4,
    eHitGround_5 = 5,
    eWaitForExplodeEnd_6 = 6,
    eExploded_7 = 7,
    eDoesNothing_8 = 8,
    eFallingBlowUpOnGround_9 = 9,
};

struct GrenadeSaveState final
{
    ReliveTypes field_0_type;
    s16 field_2_pad;
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
    bool mExplodeNow;
    bool mBlowUpOnCollision;
    s16 field_22_padding;
    Guid field_24_base_id;
    s16 field_28_line_type;
    s16 field_2A_savedcount;
    GrenadeStates field_2C_state;
    s16 field_2E;
    s16 field_30_explode_timer;
    s16 field_32_padding;
    FP field_34_xpos;
    FP field_38_ypos;
};

class Grenade final : public BaseThrowable
{
public:
    Grenade(FP xpos, FP ypos, s32 numGrenades, bool bBlowUpOnCollision, BaseGameObject* pOwner);
    ~Grenade();

    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VUpdate() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual bool VCanThrow() override;
    virtual bool VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;


    static s32 CreateFromSaveState(const u8* pBuffer);

    virtual s16 VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override
    {
        if (mDoBounceOff)
        {
            return OnCollision_BounceOff(pPlatform);
        }
        else
        {
            return OnCollision_InstantExplode(pPlatform);
        }
    }

private:
    void Init(FP xpos, FP ypos);
    void BlowUp(s16 bSmallExplosion);
    s16 TimeToBlowUp();
    s16 InTheAir(s16 blowUpOnFloorTouch);
    void AddToPlatform();
    s16 OnCollision_BounceOff(BaseGameObject* pHit);
    s16 OnCollision_InstantExplode(BaseGameObject* pHit);


private:
    Guid field_11C_explosion_id;
    GrenadeStates field_120_state = GrenadeStates::eFallingToBeCollected_0;
    s16 field_122_explode_timer = 0;
    s16 field_124 = 0;
    FP field_128_xpos = {};
    FP field_12C_ypos = {};
    s16 field_130_unused = 0;
    s16 field_132_bBlowUpOnCollision = 0;
    s16 field_134_bExplodeNow = 0;
    BaseGameObject* field_138_pOwner = nullptr;
    bool mDoBounceOff = false;
};
