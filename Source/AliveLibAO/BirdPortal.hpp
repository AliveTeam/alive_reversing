#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

enum class EReliveLevelIds : s16;
class ScreenClipper;
class Dove;
class ThrowableTotalIndicator;
class OrbWhirlWind;

namespace AO {

enum class LevelIds : s16;

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
        State_8 = 8,
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
class BirdPortal final : public ::BaseGameObject
{
public:
    BirdPortal(relive::Path_BirdPortal* pTlv, const Guid& tlvId);
    ~BirdPortal();
    
    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;

    virtual bool VPortalClipper(bool bIgnoreClipping);
    virtual void VKillPortalClipper();
    virtual void VMudSaved();
    virtual bool VActivePortal();
    virtual void VGiveShrykull(s16 bPlaySound);
    virtual bool VAbeInsidePortal();
    virtual void VExitPortal();
    virtual bool VPortalExit_AbeExitting();
    virtual void VIncreaseTimerAndKillPortalClipper();
    virtual void VGetMapChange(EReliveLevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId);

private:
    void CreateDovesAndShrykullNumber();
    void CreateTerminators();
    s16 IsScaredAway();

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
private:
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
    s16 mReceivedDovesCount = 0;
    OrbWhirlWind* mOrbWhirlWind = nullptr;
    EReliveLevelIds mCurrentLevel = EReliveLevelIds::eNone;
    s16 mCurrentPath = 0;
    s32 mSfxPlaying = 0;
    bool mDovesExist = false;
};

} // namespace AO
