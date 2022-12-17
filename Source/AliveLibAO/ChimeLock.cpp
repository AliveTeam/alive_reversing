#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "ChimeLock.hpp"
#include "Bells.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "CheatController.hpp"
#include "Math.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Events.hpp"
#include "MusicTrigger.hpp"
#include "Input.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/FatalError.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

// TODO: Index is always >=1 so first entry is redundant ??
const s32 dword_4C5054[11] = {0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

ChimeLock::ChimeLock(relive::Path_ChimeLock* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eChimeLock);

    mTlvId = tlvId;

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Chime_Ball));
    Animation_Init(GetAnimRes(AnimId::Chime_Ball));

    GetAnimation().SetRenderLayer(Layer::eLayer_FG1_37);

    FP scale = {};
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        scale = FP_FromDouble(0.5);
    }
    else
    {
        scale = FP_FromInteger(1);
    }

    mLeftBell = relive_new Bells(
        BellSize::eBig,
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        scale);
    if (mLeftBell)
    {
        mLeftBell->mBaseGameObjectRefCount++;
    }

    mCenterBell = relive_new Bells(
        BellSize::eMedium,
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        scale);
    if (mCenterBell)
    {
        mCenterBell->mBaseGameObjectRefCount++;
    }

    mRightBell = relive_new Bells(
        BellSize::eSmall,
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        scale);
    if (mRightBell)
    {
        mRightBell->mBaseGameObjectRefCount++;
    }

    field_124_code1 = pTlv->mCode1;

    s32 code2 = pTlv->mCode2;
    if (code2)
    {
        field_120_max_idx = 0;
        while (code2 / dword_4C5054[field_120_max_idx + 1])
        {
            field_120_max_idx++;
        }
        field_124_code1 = code2 + field_124_code1 * dword_4C5054[field_120_max_idx + 1];
    }

    field_120_max_idx = 0;
    for (s32 i = 0; i < 10; i++)
    {
        if (!(field_124_code1 / dword_4C5054[field_120_max_idx + 1]))
        {
            break;
        }
        field_120_max_idx++;
    }

    field_15E_ball_angle = 0;

    mTargetY = FP_FromInteger(pTlv->mTopLeftY + 40);
    mYPos = FP_FromInteger(pTlv->mTopLeftY + 40);

    mVelY = FP_FromInteger(0);

    mTargetX = FP_FromInteger(pTlv->mTopLeftX);
    mXPos = FP_FromInteger(pTlv->mTopLeftX);

    mHasBellSong = false;
    if (SwitchStates_Get(pTlv->mPasswordSwitchId))
    {
        if (!SwitchStates_Get(pTlv->mSolveSwitchId))
        {
            mHasBellSong = true;
        }
    }

    mHitAllBells = false;

    SetPossessed(false);
    SetCanBePossessed(true);

    mSolveSwitchId = pTlv->mSolveSwitchId;

    mBallState = BallStates::eIdle_0;
    field_128_idx = 0;
    field_164_ChimeLock_num[0] = BellPositions::eNone_0;
    field_164_ChimeLock_num[1] = BellPositions::eNone_0;
    mChimeLockState = ChimeLockStates::eIdle_0;
}


ChimeLock::~ChimeLock()
{
    if (mLeftBell)
    {
        mLeftBell->SetDead(true);
    }

    if (mCenterBell)
    {
        mCenterBell->SetDead(true);
    }

    if (mRightBell)
    {
        mRightBell->SetDead(true);
    }

    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void ChimeLock::VScreenChanged()
{
    if (mLeftBell)
    {
        mLeftBell->SetDead(true);
        mLeftBell->mBaseGameObjectRefCount--;
        mLeftBell = nullptr;
    }

    if (mCenterBell)
    {
        mCenterBell->SetDead(true);
        mCenterBell->mBaseGameObjectRefCount--;
        mCenterBell = nullptr;
    }

    if (mRightBell)
    {
        mRightBell->SetDead(true);
        mRightBell->mBaseGameObjectRefCount--;
        mRightBell = nullptr;
    }

    SetDead(true);
}

void ChimeLock::VUnPosses()
{
    SetPossessed(false);
    mChimeLockState = ChimeLockStates::eIdle_0;
    sActiveHero->SetActiveControlledCharacter();
    SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 70, 400);
}

s16 ChimeLock::DoNote(s16 note)
{
    if ((mHasBellSong || gVoiceCheat) && field_124_code1 / dword_4C5054[field_120_max_idx - field_128_idx] % 10 == note)
    {
        field_128_idx++;
        if (field_128_idx >= field_120_max_idx)
        {
            return 1;
        }
        return 0;
    }

    if ((!mHasBellSong && !gVoiceCheat) || (field_124_code1 / dword_4C5054[field_120_max_idx]) != note)
    {
        field_128_idx = 0;
        return 0;
    }

    field_128_idx = 1;

    if (field_120_max_idx > 1)
    {
        return 0;
    }

    return 1;
}

void ChimeLock::SetBallTarget(FP ballTargetX, FP ballTargetY, s16 timer, s16 xSize, s16 ySize, s16 bHitBell)
{
    if (timer > 0)
    {
        const FP timerFP = FP_FromInteger(timer);

        field_160_ball_timer = timer;
        field_15E_ball_angle = 0;

        mVelX = (ballTargetX - mXPos) / timerFP;
        mVelY = (ballTargetY - mYPos) / timerFP;

        mBallStartX = mXPos;
        mBallStartY = mYPos;

        mXPosOffset = FP_FromInteger(256) / timerFP;
        mYPosOffset = FP_FromInteger(256) / timerFP;

        mXSize = xSize;
        mYSize = ySize;

        if (bHitBell)
        {
            mBallState = BallStates::eMovingToBell_1;
        }
        else
        {
            mBallState = BallStates::eMovingBackToIdle_2;
        }
    }
    else
    {
        mBallState = BallStates::eIdle_0;
    }
}

s16 ChimeLock::UpdateBall()
{
    field_15E_ball_angle++;

    switch (mBallState)
    {
        case BallStates::eIdle_0:
            mXPos = (FP_FromInteger(5) * Math_Cosine((4 * field_15E_ball_angle) & 0xFF)) + mTargetX;
            mYPos = (FP_FromInteger(3) * Math_Cosine((3 * field_15E_ball_angle) & 0xFF)) + mTargetY;
            return 0;

        case BallStates::eMovingToBell_1:
        case BallStates::eMovingBackToIdle_2:
            mBallStartX += mVelX;
            mBallStartY += mVelY;
            mXPos = (FP_FromInteger(mXSize) * Math_Cosine(FP_GetExponent(FP_FromInteger(field_15E_ball_angle) * mXPosOffset) & 0xFF)) + mBallStartX;
            mYPos = (FP_FromInteger(mYSize) * Math_Cosine(FP_GetExponent(FP_FromInteger(field_15E_ball_angle) * mYPosOffset) & 0xFF)) + mBallStartY;
            if (field_15E_ball_angle >= field_160_ball_timer)
            {
                field_15E_ball_angle = 0;

                if (mBallState == BallStates::eMovingToBell_1)
                {
                    mBallState = BallStates::eMovingBackToIdle_2;
                    return 1;
                }
                mBallState = BallStates::eIdle_0;
            }
            return 0;

        case BallStates::eNeverRead_3:
        {
            if (true)
                ALIVE_FATAL("never expected BallStates::eNeverRead_3 to be called");

            mVelY += FP_FromInteger(1);
            mYPos += mVelY;

            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mXPos,
                    mVelY - mVelY,
                    mXPos,
                    mYPos,
                    &BaseAliveGameObjectCollisionLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor)
                == 1)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eFloor_0 ||
                    BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundFloor_4)
                {
                    mYPos = hitY - FP_FromInteger(1);
                    mVelY = -(mVelY * FP_FromDouble(0.4));
                }
            }
            return 0;
        }

        default:
            return 0;
    }
}

void ChimeLock::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mChimeLockState)
    {
        case ChimeLockStates::eIdle_0:
            UpdateBall();
            return;

        case ChimeLockStates::eUnused_1:
            if (UpdateBall())
            {
                mChimeLockState = ChimeLockStates::ePossessed_2;
                mBallState = BallStates::eIdle_0;
            }
            ALIVE_FATAL("never expected ChimeLockStates::eUnused_1 to be called");
            return;

        case ChimeLockStates::ePossessed_2:
            if (UpdateBall())
            {
                BellPositions Bell = field_164_ChimeLock_num[0];
                field_164_ChimeLock_num[0] = BellPositions::eNone_0;

                switch (Bell)
                {
                    case BellPositions::eLeftBell_1:
                        mLeftBell->Ring();
                        if (mHitAllBells)
                        {
                            SetTargetBellIfSpace(2);
                        }
                        break;

                    case BellPositions::eCenterBell_2:
                        mCenterBell->Ring();
                        if (mHitAllBells)
                        {
                            SetTargetBellIfSpace(3);
                        }
                        break;

                    case BellPositions::eRightBell_3:
                        mRightBell->Ring();
                        if (mHitAllBells)
                        {
                            mHitAllBells = false;
                        }
                        break;
                }

                const s16 ySize = Math_RandomRange(6, 9);
                const s16 xSize = Math_RandomRange(6, 9);
                SetBallTarget(
                    mTargetX,
                    mTargetY,
                    36,
                    xSize,
                    ySize,
                    0);

                if (DoNote(static_cast<s16>(Bell)))
                {
                    mChimeLockState = ChimeLockStates::eNeverRead_6;
                    SwitchStates_Do_Operation(mSolveSwitchId, relive::reliveSwitchOp::eSetTrue);
                    VUnPosses();
                    relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 15);
                    return;
                }
            }

            if (mBallState != BallStates::eMovingToBell_1)
            {
                switch (field_164_ChimeLock_num[0])
                {
                    case BellPositions::eLeftBell_1:
                        if (mLeftBell->CanSmash())
                        {
                            SetBallTarget(
                                mTargetX - FP_FromInteger(35),
                                mTargetY - FP_FromInteger(4),
                                2,
                                0,
                                0,
                                1);
                        }
                        break;

                    case BellPositions::eCenterBell_2:
                        if (mCenterBell->CanSmash())
                        {
                            SetBallTarget(
                                mTargetX - FP_FromInteger(4),
                                mTargetY - FP_FromInteger(16),
                                2,
                                0,
                                0,
                                1);
                        }
                        break;

                    case BellPositions::eRightBell_3:
                        if (mRightBell->CanSmash())
                        {
                            SetBallTarget(
                                mTargetX + FP_FromInteger(37),
                                mTargetY - FP_FromInteger(8),
                                2,
                                0,
                                0,
                                1);
                        }
                        break;

                    case BellPositions::eUnused_4:
                    {
                        const s16 ySize = Math_RandomRange(6, 9);
                        const s16 xSize = Math_RandomRange(6, 9);
                        SetBallTarget(
                            mTargetX,
                            mTargetY,
                            30,
                            xSize,
                            ySize,
                            1);
                        ALIVE_FATAL("never expected target bell num 4 in ChimeLockStates::ePossessed_2 to be called");
                        break;
                    }

                    default:
                        break;
                }
            }

            if (!mHasBellSong && !gVoiceCheat)
            {
                if (!Input_IsChanting())
                {
                    mCanUnpossess = true;
                }

                if (mCanUnpossess && Input_IsChanting())
                {
                    mUnpossessionCountdown = 30;
                    mChimeLockState = ChimeLockStates::eUnPossessing_3;
                    field_134_pressed = Input().Pressed();

                    field_164_ChimeLock_num[0] = BellPositions::eNone_0;
                    field_164_ChimeLock_num[1] = BellPositions::eNone_0;

                    SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
                    return;
                }

                const auto pressed = Input().Pressed();
                if (Input().IsAnyPressed(eThrowItem) && !(field_134_pressed & eThrowItem))
                {
                    SetTargetBellIfSpace(3);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eHop) && !(field_134_pressed & eHop))
                {
                    SetTargetBellIfSpace(2);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eDoAction) && !(field_134_pressed & eDoAction))
                {
                    SetTargetBellIfSpace(1);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eCrouchOrRoll) && !(field_134_pressed & eCrouchOrRoll))
                {
                    // hit all 3 bells from left to right
                    SetTargetBellIfSpace(1);
                    mHitAllBells = true;
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eRight) && !(field_134_pressed & eRight))
                {
                    SetTargetBellIfSpace(3);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eUp) && !(field_134_pressed & eUp))
                {
                    SetTargetBellIfSpace(2);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eLeft) && !(field_134_pressed & eLeft))
                {
                    SetTargetBellIfSpace(1);
                    field_134_pressed = pressed;
                    return;
                }

                if (Input().IsAnyPressed(eDown) && !(field_134_pressed & eDown))
                {
                    // hit all 3 bells from left to right
                    SetTargetBellIfSpace(1);
                    mHitAllBells = true;
                }
                field_134_pressed = pressed;
                return;
            }

            if (static_cast<s32>(sGnFrame) >= field_12C_timer)
            {
                SetTargetBellIfSpace(field_124_code1 / dword_4C5054[field_120_max_idx - field_128_idx] % 10);
                field_12C_timer = sGnFrame + 15;
            }
            return;

        case ChimeLockStates::eUnPossessing_3:
        {
            if (UpdateBall())
            {
                const s16 ySize = Math_RandomRange(6, 9);
                const s16 xSize = Math_RandomRange(6, 9);
                SetBallTarget(mTargetX, mTargetY, 36, xSize, ySize, 0);
            }

            if (!Input_IsChanting())
            {
                mChimeLockState = ChimeLockStates::ePossessed_2;
            }
            else
            {
                mUnpossessionCountdown--;

                if (mUnpossessionCountdown == 0)
                {
                    mChimeLockState = ChimeLockStates::eUnPossess_4;
                }
            }

            if (!(sGnFrame % 4))
            {
                return;
            }

            const FP y = mTargetY - (GetSpriteScale() * FP_FromInteger(Math_RandomRange(-20, 20)));
            const FP x = mTargetX + (GetSpriteScale() * FP_FromInteger(Math_RandomRange(-30, 30)));
            New_TintChant_Particle(
                x, y,
                GetSpriteScale(),
                Layer::eLayer_0);
            return;
        }

        case ChimeLockStates::eUnPossess_4:
            VUnPosses();
            return;

        case ChimeLockStates::eUnused_5:
            if (UpdateBall())
            {
                mChimeLockState = ChimeLockStates::eIdle_0;
            }
            ALIVE_FATAL("never expected ChimeLockStates::eUnused_5 to be called");
            return;

        default:
            return;
    }
}


void ChimeLock::SetTargetBellIfSpace(s16 targetNum)
{
    s32 magic = 0;
    auto pChimeLock_num = &field_164_ChimeLock_num[0];

    // looks like it's just doing: field_164_ChimeLock_num[0] = static_cast<BellPositions>(targetNum);
    // everytime. the while loop looks pointless to me
    while (*pChimeLock_num != BellPositions::eNone_0)
    {
        ++magic;
        ++pChimeLock_num;
        if (magic >= ALIVE_COUNTOF(field_164_ChimeLock_num))
        {
            return;
        }
    }
    field_164_ChimeLock_num[magic] = static_cast<BellPositions>(targetNum);
}

void ChimeLock::VPossessed()
{
    mCanUnpossess = false;
    mHitAllBells = false;
    SetPossessed(true);
    mChimeLockState = ChimeLockStates::ePossessed_2;
    field_128_idx = 0;
    field_12C_timer = sGnFrame + 45;
    mBallState = BallStates::eIdle_0;
    field_164_ChimeLock_num[0] = BellPositions::eNone_0;
    field_164_ChimeLock_num[1] = BellPositions::eNone_0;
}

} // namespace AO
