#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

namespace relive
{
    struct Path_Greeter;
}

enum class GreeterBrainStates : s16
{
    eBrain_0_Patrol,
    eBrain_1_PatrolTurn,
    eBrain_2_Speak,
    eBrain_3_ChaseSpeak,
    eBrain_4_Chase,
    eBrain_5_Knockback,
    eBrain_6_ToChase,
    eBrain_7_Fall
};

struct GreeterSaveState final
{
    ReliveTypes field_0_type;
    s16 field_2_r;
    s16 field_4_g;
    s16 field_6_b;
    s16 field_8_path_number;
    EReliveLevelIds field_A_lvl_number;
    FP field_C_xpos;
    FP field_10_ypos;
    FP field_14_velx;
    FP field_18_vely;
    FP field_1C_sprite_scale;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mAnimRender;
    s8 mDrawable;
    Guid mTlvId;
    s32 field_30_last_turn_time;
    s32 field_34_timer;
    s16 mTimesShot;
    s16 field_3A_bDontSetDestroyed;
    bool mChasing;
    FP field_40_speed;
    GreeterBrainStates mBrainState;
    s16 field_46_targetOnLeft;
    s16 field_48_targetOnRight;
    FP mMotionLaserXPos;
};

class PathLine;

enum class GreeterSpeak : s16
{
    eHi_0 = 0,
    eHereBoy_1 = 1,
    eGetHim_2 = 2,
    eLaugh_3 = 3,
    eWhat_9 = 9,
    eRandomized_1000 = 1000,
};

class Greeter final : public BaseAliveGameObject
{
public:
    Greeter(relive::Path_Greeter* pTlv, const Guid& tlvId);
    ~Greeter();
    
    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    void BlowUp();
    void ChangeDirection();
    void BounceBackFromShot();
    void HandleRollingAlong();
    void ZapTarget(FP xpos, FP ypos, IBaseAliveGameObject* pTarget);
    void RandomishSpeak(GreeterSpeak effect);
    bool ZapIsNotBlocked(IBaseAliveGameObject* pUs, IBaseAliveGameObject* pThem);
    IBaseAliveGameObject* GetMudToZap();

private:
    Guid mTlvId;
    Guid field_11C_motionDetectorId;
    s32 field_124_last_turn_time = 0;
    s32 field_128_timer = 0;
    s16 mTimesShot = 0;
    s16 field_12E_bDontSetDestroyed = 0;
    bool mChasing = false;
    FP field_134_speed = {};
    relive::Path_TLV* field_138_pTlv = nullptr;

public:
    GreeterBrainStates mBrainState = GreeterBrainStates::eBrain_0_Patrol;
    s16 field_13E_targetOnLeft = 0;
    s16 field_140_targetOnRight = 0;
};
