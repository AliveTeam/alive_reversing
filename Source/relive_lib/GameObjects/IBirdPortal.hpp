#pragma once

#include "BaseGameObject.hpp"
#include "../MapWrapper.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../SaveStateBase.hpp"

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

// Temp glue interface
class IBirdPortal : public BaseGameObject
{
public:

    IBirdPortal();

private:
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;

protected:
    void LoadAnimations();
    void CreateDovesAndShrykullNumber();
    void CreateTerminators();

public:
    
    void MudSaved();
    bool IsActivePortalState();
    void DestroyPortalClippers();
    bool IsAbeInsidePortalState();
    bool IsAbExittingPortalState();
    void RemovePortalClipperDelayed();

    bool ClipPortal(bool bIgnoreClipping);

    virtual void VGiveShrykull(s16 bPlaySound) = 0;
    virtual void VExitPortal() = 0;
    virtual void VGetMapChange(EReliveLevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId) = 0;

public:
    relive::Path_BirdPortal::PortalType mPortalType = relive::Path_BirdPortal::PortalType::eAbe;
    relive::Path_BirdPortal::PortalSide mEnterSide = relive::Path_BirdPortal::PortalSide::eRight;
    PortalStates mState = PortalStates::CreatePortal_0;

    FP mXPos = {};
    FP mYPos = {};
    FP mExitX = {};
    FP mExitY = {};
    FP mHitY = {};
    Guid mTlvInfo;
protected:
    Guid mThrowableTotalIndicator = Guid{};
    Guid mDoveIds[6] = {};
    s32 mTimer = 0;
    FP mSpriteScale = {};
    s16 mMovieId = 0;

    Guid mTerminatorId1 = Guid{};
    Guid mTerminatorId2 = Guid{};
    Guid mScreenClipperId1 = Guid{};
    Guid mScreenClipperId2 = Guid{};
    EReliveLevelIds mExitLevel = EReliveLevelIds::eNone;
    s16 mExitPath = 0;
    s16 mExitCamera = 0;
    s16 mMudCountForShrykull = 0;
    s16 mDeletePortalSwitchId = 0; // AE only

    s16 mReceivedDovesCount = 0;
    class OrbWhirlWind* mOrbWhirlWind = nullptr;
    EReliveLevelIds mCurrentLevel = EReliveLevelIds::eNone;
    s16 mCurrentPath = 0;
    s32 mSfxPlaying = 0;
    bool mDovesExist = false;
};
