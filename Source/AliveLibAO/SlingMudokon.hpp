#pragma once

#include "BaseAliveGameObject.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/FatalError.hpp"

namespace relive
{
    struct Path_SlingMudokon;
}

namespace AO {

enum class eSlingMudMotions
{
    None_m1 = -1,
    Motion_0_Idle,
    Motion_1_Angry,
    Motion_2_Speak,
    Motion_3_ShootStart,
    Motion_4_ShootEnd,
    Motion_5_AngryToIdle
};

enum SlingMudBrainStates : u16
{
    Brain_0_GiveCode = 0,
    Brain_1_Spawn = 1,
    Brain_2_AskForPassword = 2
};

class SlingMudokon;

class ISlingMudokonBrain
{
public:
    enum class EBrainTypes
    {
        GiveCode = 0,
        Spawn = 1,
        AskForPassword = 2
    };
    virtual ~ISlingMudokonBrain() { }
    virtual void VUpdate() = 0;
    virtual EBrainTypes VGetBrain() = 0;
};

class GiveCodeBrain final : public ISlingMudokonBrain
{
public:
    enum EState
    {
        Init = 0,
        GiveCode = 1,
        PauseABit = 2,
        WaitForCode = 3,
        CheckCodeMatching = 4,
        RespondToProvidedCode = 5,
    };

    explicit GiveCodeBrain(SlingMudokon& slingMudokon) : mSlingMudokon(slingMudokon) {}

    void VUpdate() override;
    EBrainTypes VGetBrain() override { return EBrainTypes::GiveCode; }

    void SetState(EState state) { mBrainState = state; }
    EState State() { return mBrainState; }

private:
    SlingMudokon& mSlingMudokon;
    EState mBrainState = EState::Init;
};

class SpawnBrain final : public ISlingMudokonBrain
{
public:
    enum EState
    {
        Init = 0,
        CreateParticle = 1,
        CreateFlash = 2,
        GetAngry = 3,
        ObserveAbe = 4,
        PrepareToShoot = 5,
        Shoot = 6,
        DisappearAsDoves = 7
    };

    explicit SpawnBrain(SlingMudokon& slingMudokon) : mSlingMudokon(slingMudokon) {}

    void VUpdate() override;
    EBrainTypes VGetBrain() override { return EBrainTypes::Spawn; }

    void SetState(EState state) { mBrainState = state; }
    EState State() { return mBrainState; }

private:
    SlingMudokon& mSlingMudokon;
    EState mBrainState = EState::Init;
};

class AskForPasswordBrain final : public ISlingMudokonBrain
{
public:
    enum EState
    {
        Unknown_0 = 0,
        Unknown_1 = 1,
        Unknown_2 = 2,
        Unknown_3 = 3,
        Unknown_4 = 4,
        Unknown_5 = 5,
        Unknown_6 = 6,
        Unknown_7 = 7,
        Unknown_8 = 8,
        Unknown_9 = 9,
    };

    explicit AskForPasswordBrain(SlingMudokon& slingMudokon) : mSlingMudokon(slingMudokon) {}

    void VUpdate() override;
    EBrainTypes VGetBrain() override { return EBrainTypes::AskForPassword; }

    void SetState(EState state) { mBrainState = state; }
    EState State() { return mBrainState; }

private:
    SlingMudokon& mSlingMudokon;
    EState mBrainState = EState::Unknown_0;
};

class SlingMudokon final : public BaseAliveGameObject
{
public:
    static constexpr AnimId sSlingMudMotionAnimIds[6] = {
        AnimId::Mudokon_Sling_Idle,
        AnimId::Mudokon_Sling_Angry,
        AnimId::Mudokon_Sling_Speak,
        AnimId::Mudokon_Sling_ShootStart,
        AnimId::Mudokon_Sling_ShootEnd,
        AnimId::Mudokon_Sling_AngryToIdle};

    SlingMudokon(relive::Path_SlingMudokon* pTlv, const Guid& tlvId);
    ~SlingMudokon();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    virtual void VCallBrain();
    virtual void VCallMotion();
    virtual s16 VGetMotion(eMotionType motionType) override
    {
        switch (motionType)
        {
            case eMotionType::ePreviousMotion:
                return 0;
            case eMotionType::eCurrentMotion:
                return static_cast<s16>(mCurrentMotion);
            case eMotionType::eNextMotion:
                return static_cast<s16>(mNextMotion);
            default:
                ALIVE_FATAL("Invalid motion type %d", static_cast<s32>(motionType));
        }
    }

    void VUpdateAnimData();

    // Motions
    void Motion_0_Idle();
    void Motion_1_Angry();
    void Motion_2_Speak();
    void Motion_3_ShootStart();
    void Motion_4_ShootEnd();
    void Motion_5_AngryToIdle();

    GameSpeakEvents getLastIdx();

private:
    void SetBrain(ISlingMudokonBrain::EBrainTypes brain);

public:
    Guid mTlvId;
    s32 mCodeConverted = 0;
    s16 mCodeLength = 0;
    bool mDontSetDestroyed = false;
    bool mAbeGettingCloser = false;
    GameSpeakListener mListener;
    GameSpeakEvents mCodeBuffer[16] = {};
    s16 mBufferStart = 0;
    s16 mBufferIdx = 0;
    s32 field_140_timer = 0;
    s32 field_144_timer2 = 0;
    GiveCodeBrain::EState field_154_previous_brain_state = GiveCodeBrain::EState::Init;
    s16 mCodePos = 0;
    bool mCodeMatches = false;
    friend class GiveCodeBrain;
    friend class SpawnBrain;
    friend class AskForPasswordBrain;
    GiveCodeBrain mGiveCodeBrain;
    SpawnBrain mSpawnBrain;
    AskForPasswordBrain mAskForPasswordBrain;

    ISlingMudokonBrain* mCurrentBrain = nullptr;
    eSlingMudMotions mCurrentMotion = eSlingMudMotions::Motion_0_Idle;
    eSlingMudMotions mNextMotion = eSlingMudMotions::Motion_0_Idle;
};

} // namespace AO
