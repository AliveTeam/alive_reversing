#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

enum class LevelIds : s16;
enum class CameraSwapEffects : s16;




struct BirdPortal_State final
{
    AETypes mAEType;
    u8 mState;
    u8 mMudCountForShrykull;
    Guid mTlvInfo;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(BirdPortal_State, 8);

class BirdPortalTerminator final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    BirdPortalTerminator(FP xpos, FP ypos, FP scale, relive::Path_BirdPortal::PortalType portalType);
    virtual void VScreenChanged() override;
    
    void LoadAnimations();

    void Fadeout();
};
ALIVE_ASSERT_SIZEOF(BirdPortalTerminator, 0xF4);

class OrbWhirlWind;
enum Event : s16;

class BirdPortal final : public BaseGameObject
{
public:
    BirdPortal(relive::Path_BirdPortal* pTlv, const Guid& tlvId);
    ~BirdPortal();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    // New virtuals

    virtual s16 VPortalClipper(s16 bUnknown);
    virtual void VKillPortalClipper();
    virtual void VMudSaved();
    virtual bool VActivePortal();
    virtual void VGiveShrykull(s16 bPlaySound);
    virtual bool VAbeInsidePortal();
    virtual void VExitPortal();
    virtual bool VPortalExit_AbeExitting();
    virtual void VIncreaseTimerAndKillPortalClipper();
    virtual void VGetMapChange(EReliveLevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId);

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    void CreateDovesAndShrykullNumber();
    void CreateTerminators();
    s16 IsScaredAway();
    void KillTerminators();
    Event GetEvent();

private:
    Guid mTlvInfo;

public:
    relive::Path_BirdPortal::PortalType mPortalType = relive::Path_BirdPortal::PortalType::eAbe;
    relive::Path_BirdPortal::PortalSide mEnterSide = relive::Path_BirdPortal::PortalSide::eRight;

public:
    enum class PortalStates : s16
    {
        CreatePortal_0 = 0,
        IdlePortal_1 = 1,
        JoinDovesInCenter_2 = 2,
        KillDoves_3 = 3,
        CreateTerminators_4 = 4,
        ExpandTerminators_5 = 5,
        ActivePortal_6 = 6,
        ShrykullGetDoves_7 = 7,
        Unused_8 = 8,
        GetShrykull_9 = 9,
        CollapseTerminators_10 = 10,
        StopSound_11 = 11,
        CreateFlash1_12 = 12,
        CreateFlash2_13 = 13,
        CreateFlash3_14 = 14,
        KillPortal_15 = 15,
        AbeInsidePortal_16 = 16,
        PortalExit_SetPosition_17 = 17,
        PortalExit_CreateTerminators_18 = 18,
        PortalExit_ExpandTerminators_19 = 19,
        PortalExit_AbeExitting_20 = 20,
        KillPortalClipper_21 = 21,
        FadeoutTerminators_22 = 22,
    };
    PortalStates mState = PortalStates::CreatePortal_0;

public:
    FP mXPos = {};
    FP mYPos = {};
    FP mExitX = {};
    FP mExitY = {};

public:
    FP mHitY = {};

private:
    Guid mThrowableIndicatorId; // AE only
    Guid mDoveIds[6] = {}; // AE only
    s32 mTimer = 0;
    FP mSpriteScale = {};
    s16 mMovieId = 0;
    s16 mDeletePortalSwitchId = 0; // AE only
    s16 mDovesExist = 0;
    Guid mTerminatorId1;   // object pointer in AO
    Guid mTerminatorId2;   // object pointer in AO
    Guid mScreenClipperId1; // object pointer in AO
    Guid mScreenClipperId2; // object pointer in AO
    EReliveLevelIds mExitLevel = EReliveLevelIds::eNone;
    s16 mExitPath = 0;
    s16 mExitCamera = 0;
    s16 mMudCountForShrykull = 0;
    s16 mReceivedDovesCount = 0;
    OrbWhirlWind* mOrbWhirlWind = nullptr;
    EReliveLevelIds mCurrentLevel = EReliveLevelIds::eNone;
    s16 mCurrentPath = 0;
    s32 mSfxPlaying = 0;
};
ALIVE_ASSERT_SIZEOF(BirdPortal, 0x100);
