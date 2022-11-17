#include "stdafx.h"
#include "Glukkon.hpp"
#include "Function.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "../relive_lib/Shadow.hpp"
#include "MusicController.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Spark.hpp"
#include "Slig.hpp" //used for renderWithGlowingEyes() only - TODO refactor
#include "ParticleBurst.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Particle.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "Slurg.hpp"
#include "Fade.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Movie.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sound/Sound.hpp"
#include "MainMenu.hpp"
#include "GameSpeak.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Bullet.hpp"
#include "Sound/Midi.hpp"
#include "Grid.hpp"
#include "../relive_lib/Camera.hpp"

const TGlukkonMotionFn sGlukkonMotionTable[25] = {
    &Glukkon::Motion_0_Idle,
    &Glukkon::Motion_1_Walk,
    &Glukkon::Motion_2_Turn,
    &Glukkon::Motion_3_KnockBack,
    &Glukkon::Motion_4_Jump,
    &Glukkon::Motion_5_JumpToFall,
    &Glukkon::Motion_6_WalkToFall,
    &Glukkon::Motion_7_Fall,
    &Glukkon::Motion_8_DeathFall,
    &Glukkon::Motion_9_Land,
    &Glukkon::Motion_10_ChantShake,
    &Glukkon::Motion_11_Speak1,
    &Glukkon::Motion_12_Speak2,
    &Glukkon::Motion_13_LongLaugh,
    &Glukkon::Motion_14_BeginWalk,
    &Glukkon::Motion_15_EndWalk,
    &Glukkon::Motion_16_StandToJump,
    &Glukkon::Motion_17_JumpToStand,
    &Glukkon::Motion_18_WalkToJump,
    &Glukkon::Motion_19_JumpToWalk,
    &Glukkon::Motion_20_KnockBackStandBegin,
    &Glukkon::Motion_21_GetShot,
    &Glukkon::Motion_22_KnockBackStandEnd,
    &Glukkon::Motion_23_Speak3,
    &Glukkon::Motion_24_EndSingleStep};

const TGlukkonBrainFn sGlukkon_brain_table_5544A0[6] = {
    &Glukkon::Brain_0_Calm_WalkAround,
    &Glukkon::Brain_1_Panic,
    &Glukkon::Brain_2_Slapped,
    &Glukkon::Brain_3_PlayerControlled,
    &Glukkon::Brain_4_Death,
    &Glukkon::Brain_5_WaitToSpawn};

const AnimId sGlukkonsAnimIdTable[4][25] = {
    {AnimId::Glukkon_Normal_Idle, AnimId::Glukkon_Normal_Walk, AnimId::Glukkon_Normal_Turn, AnimId::Glukkon_Normal_KnockBack, AnimId::Glukkon_Normal_Jump,
     AnimId::Glukkon_Normal_JumpToFall, AnimId::Glukkon_Normal_WalkToFall, AnimId::Glukkon_Normal_Fall, AnimId::Glukkon_Normal_DeathFall, AnimId::Glukkon_Normal_Land,
     AnimId::Glukkon_Normal_ChantShake, AnimId::Glukkon_Normal_Speak1, AnimId::Glukkon_Normal_Speak2, AnimId::Glukkon_Normal_LongLaugh, AnimId::Glukkon_Normal_BeginWalk,
     AnimId::Glukkon_Normal_EndWalk, AnimId::Glukkon_Normal_StandToJump, AnimId::Glukkon_Normal_JumpToStand, AnimId::Glukkon_Normal_WalkToJump, AnimId::Glukkon_Normal_JumpToWalk,
     AnimId::Glukkon_Normal_KnockBackStandBegin, AnimId::Glukkon_Normal_GetShot, AnimId::Glukkon_Normal_KnockBackStandEnd, AnimId::Glukkon_Normal_Speak3, AnimId::Glukkon_Normal_EndSingleStep},

    {AnimId::Glukkon_Aslik_Idle, AnimId::Glukkon_Aslik_Walk, AnimId::Glukkon_Aslik_Turn, AnimId::Glukkon_Aslik_KnockBack, AnimId::Glukkon_Aslik_Jump,
     AnimId::Glukkon_Aslik_JumpToFall, AnimId::Glukkon_Aslik_WalkToFall, AnimId::Glukkon_Aslik_Fall, AnimId::Glukkon_Aslik_DeathFall, AnimId::Glukkon_Aslik_Land,
     AnimId::Glukkon_Aslik_ChantShake, AnimId::Glukkon_Aslik_Speak1, AnimId::Glukkon_Aslik_Speak2, AnimId::Glukkon_Aslik_LongLaugh, AnimId::Glukkon_Aslik_BeginWalk,
     AnimId::Glukkon_Aslik_EndWalk, AnimId::Glukkon_Aslik_StandToJump, AnimId::Glukkon_Aslik_JumpToStand, AnimId::Glukkon_Aslik_WalkToJump, AnimId::Glukkon_Aslik_JumpToWalk,
     AnimId::Glukkon_Aslik_KnockBackStandBegin, AnimId::Glukkon_Aslik_GetShot, AnimId::Glukkon_Aslik_KnockBackStandEnd, AnimId::Glukkon_Aslik_Speak3, AnimId::Glukkon_Aslik_EndSingleStep},

    {AnimId::Glukkon_Dripik_Idle, AnimId::Glukkon_Dripik_Walk, AnimId::Glukkon_Dripik_Turn, AnimId::Glukkon_Dripik_KnockBack, AnimId::Glukkon_Dripik_Jump,
     AnimId::Glukkon_Dripik_JumpToFall, AnimId::Glukkon_Dripik_WalkToFall, AnimId::Glukkon_Dripik_Fall, AnimId::Glukkon_Dripik_DeathFall, AnimId::Glukkon_Dripik_Land,
     AnimId::Glukkon_Dripik_ChantShake, AnimId::Glukkon_Dripik_Speak1, AnimId::Glukkon_Dripik_Speak2, AnimId::Glukkon_Dripik_LongLaugh, AnimId::Glukkon_Dripik_BeginWalk,
     AnimId::Glukkon_Dripik_EndWalk, AnimId::Glukkon_Dripik_StandToJump, AnimId::Glukkon_Dripik_JumpToStand, AnimId::Glukkon_Dripik_WalkToJump, AnimId::Glukkon_Dripik_JumpToWalk,
     AnimId::Glukkon_Dripik_KnockBackStandBegin, AnimId::Glukkon_Dripik_GetShot, AnimId::Glukkon_Dripik_KnockBackStandEnd, AnimId::Glukkon_Dripik_Speak3, AnimId::Glukkon_Dripik_EndSingleStep},

    {AnimId::Glukkon_Phleg_Idle, AnimId::Glukkon_Phleg_Walk, AnimId::Glukkon_Phleg_Turn, AnimId::Glukkon_Phleg_KnockBack, AnimId::Glukkon_Phleg_Jump,
     AnimId::Glukkon_Phleg_JumpToFall, AnimId::Glukkon_Phleg_WalkToFall, AnimId::Glukkon_Phleg_Fall, AnimId::Glukkon_Phleg_DeathFall, AnimId::Glukkon_Phleg_Land,
     AnimId::Glukkon_Phleg_ChantShake, AnimId::Glukkon_Phleg_Speak1, AnimId::Glukkon_Phleg_Speak2, AnimId::Glukkon_Phleg_LongLaugh, AnimId::Glukkon_Phleg_BeginWalk,
     AnimId::Glukkon_Phleg_EndWalk, AnimId::Glukkon_Phleg_StandToJump, AnimId::Glukkon_Phleg_JumpToStand, AnimId::Glukkon_Phleg_WalkToJump, AnimId::Glukkon_Phleg_JumpToWalk,
     AnimId::Glukkon_Phleg_KnockBackStandBegin, AnimId::Glukkon_Phleg_GetShot, AnimId::Glukkon_Phleg_KnockBackStandEnd, AnimId::Glukkon_Phleg_Speak3, AnimId::Glukkon_Phleg_EndSingleStep}};

const TintEntry kGlukkonTints_5546B4[16] = {
    {EReliveLevelIds::eMenu, 137u, 137u, 137u},
    {EReliveLevelIds::eMines, 137u, 137u, 137u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 137u, 137u, 137u},
    {EReliveLevelIds::eMudancheeVault, 137u, 137u, 137u},
    {EReliveLevelIds::eFeeCoDepot, 137u, 137u, 137u},
    {EReliveLevelIds::eBarracks, 137u, 137u, 137u},
    {EReliveLevelIds::eMudancheeVault_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eBonewerkz, 137u, 137u, 137u},
    {EReliveLevelIds::eBrewery, 137u, 137u, 137u},
    {EReliveLevelIds::eBrewery_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eBarracks_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eBonewerkz_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eCredits, 137u, 137u, 137u}};

s32 Glukkon::CreateFromSaveState(const u8* pData)
{
    const GlukkonSaveState* pSaveState = reinterpret_cast<const GlukkonSaveState*>(pData);
    auto pTlv = static_cast<relive::Path_Glukkon*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->mTlvId));

    auto pGlukkon = relive_new Glukkon(pTlv, pSaveState->mTlvId);
    if (pGlukkon)
    {
        pGlukkon->SetType(pSaveState->mCurrentType);
        pGlukkon->mBaseGameObjectTlvInfo = pSaveState->field_4_object_id;
        if (pSaveState->mIsActiveChar)
        {
            sControlledCharacter = pGlukkon;
        }

        pGlukkon->BaseAliveGameObjectPathTLV = nullptr;
        pGlukkon->BaseAliveGameObjectCollisionLine = nullptr;

        pGlukkon->mXPos = pSaveState->mXPos;
        pGlukkon->mYPos = pSaveState->mYPos;

        pGlukkon->mVelX = pSaveState->mVelX;
        pGlukkon->mVelY = pSaveState->mVelY;

        pGlukkon->mFallingVelXScaleFactor = pSaveState->mFallingVelXScaleFactor;
        pGlukkon->mCurrentPath = pSaveState->mCurrentPath;
        pGlukkon->mCurrentLevel = pSaveState->mCurrentLevel;
        pGlukkon->SetSpriteScale(pSaveState->mSpriteScale);

        pGlukkon->mRGB.SetRGB(pSaveState->mRed, pSaveState->mGreen, pSaveState->mBlue);

        pGlukkon->mRed = pSaveState->mRed;
        pGlukkon->mGreen = pSaveState->mGreen;
        pGlukkon->mBlue = pSaveState->mBlue;

        pGlukkon->mCurrentMotion = pSaveState->mCurrentMotion;

        relive::Path_Glukkon::GlukkonTypes glukType = pGlukkon->mTlvData.mGlukkonType;
        // TODO: This should really be a switch rather than comparing values
        if (glukType > relive::Path_Glukkon::GlukkonTypes::eStoryPhleg)
        {
            glukType = relive::Path_Glukkon::GlukkonTypes::eNormal;
        }

        pGlukkon->GetAnimation().Set_Animation_Data(pGlukkon->GetAnimRes(sGlukkonsAnimIdTable[static_cast<s32>(glukType)][pSaveState->mCurrentMotion]));

        pGlukkon->GetAnimation().SetCurrentFrame(pSaveState->mCurrentFrame);
        pGlukkon->GetAnimation().SetFrameChangeCounter(pSaveState->mFrameChangeCounter);
        pGlukkon->mBaseGameObjectFlags.Set(BaseGameObject::Options::eDrawable_Bit4, pSaveState->mDrawable & 1);
        pGlukkon->GetAnimation().mFlags.Set(AnimFlags::eFlipX, pSaveState->mFlipX & 1);
        pGlukkon->GetAnimation().mFlags.Set(AnimFlags::eRender, pSaveState->mRender & 1);

        if (IsLastFrame(&pGlukkon->GetAnimation()))
        {
            pGlukkon->GetAnimation().mFlags.Set(AnimFlags::eIsLastFrame);
        }

        pGlukkon->mHealth = pSaveState->mHealth;
        pGlukkon->mCurrentMotion = pSaveState->mCurrentMotion2;
        pGlukkon->mNextMotion = pSaveState->mNextMotion;
        pGlukkon->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pSaveState->field_38_last_line_ypos);
        pGlukkon->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eRestoredFromQuickSave);
        pGlukkon->field_1D4_timer = pSaveState->field_54_timer;
        pGlukkon->BaseAliveGameObjectCollisionLineType = pSaveState->mLineType;
        pGlukkon->mTlvId = pSaveState->mTlvId;
        pGlukkon->SetBrain(sGlukkon_brain_table_5544A0[pSaveState->field_48_brain_state_idx]);
        pGlukkon->mBrainSubState = pSaveState->mBrainSubState;
        pGlukkon->mPreventDepossession = pSaveState->mPreventDepossession;
        pGlukkon->mAbeLevel = pSaveState->mAbeLevel;
        pGlukkon->mAbePath = pSaveState->mAbePath;
        pGlukkon->mAbeCamera = pSaveState->mAbeCamera;
        pGlukkon->mSpeak = pSaveState->mSpeak;
        pGlukkon->mGamespeakPitch = pSaveState->mGamespeakPitch;
        pGlukkon->mPreviousYPos = pSaveState->mPreviousYPos;
        pGlukkon->mRandomishSpeakTimer = pSaveState->mRandomishSpeakTimer;
        pGlukkon->mTurnOrHelpTimer = pSaveState->mTurnOrHelpTimer;
        pGlukkon->mPanicTimer = pSaveState->mPanicTimer;
        pGlukkon->field_1FC = pSaveState->field_7C;
        pGlukkon->mKnockbackDelayAfterGettingShotTimer = pSaveState->mKnockbackDelayAfterGettingShotTimer;
        pGlukkon->mGettingShotTimer = pSaveState->mGettingShotTimer;
        pGlukkon->mFadeId = pSaveState->mFadeId;
        pGlukkon->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed, pSaveState->mCanBePossessed);
    }

    return sizeof(GlukkonSaveState);
}

void Glukkon::SetBrain(TGlukkonBrainFn fn)
{
    mBrainState = fn;
}

bool Glukkon::BrainIs(TGlukkonBrainFn fn)
{
    return mBrainState == fn;
}

void Glukkon::LoadAnimations()
{
    for (s32 i = 0; i < 4; i++)
    {
        for (s32 j = 0; j < 25; j++)
        {
            auto& animId = sGlukkonsAnimIdTable[i][j];
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
        }
    }
}

Glukkon::Glukkon(relive::Path_Glukkon* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    LoadAnimations();

    mTlvData = *pTlv;

    mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanSetOffExplosives);

    mTlvId = tlvId;

    mPal = std::make_shared<AnimationPal>();

    switch (mTlvData.mGlukkonType)
    {
        case relive::Path_Glukkon::GlukkonTypes::eNormal:
        {
            Animation_Init(GetAnimRes(AnimId::Glukkon_Normal_Idle));
            break;
        }

        case relive::Path_Glukkon::GlukkonTypes::eStoryAslik:
        {
            Animation_Init(GetAnimRes(AnimId::Glukkon_Aslik_Idle));
            break;
        }

        case relive::Path_Glukkon::GlukkonTypes::eStoryDripik:
        {
            Animation_Init(GetAnimRes(AnimId::Glukkon_Dripik_Idle));
            break;
        }

        case relive::Path_Glukkon::GlukkonTypes::eStoryPhleg:
        {
            Animation_Init(GetAnimRes(AnimId::Glukkon_Phleg_Idle));
            break;
        }

        default:
            break;
    }

    Init();
}

void Glukkon::VRender(PrimHeader** ot)
{
    s16 eyeIndices[1];
    if (mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryAslik || mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryPhleg)
    {
        eyeIndices[0] = 63;
    }
    else
    {
        eyeIndices[0] = 62;
    }
    renderWithGlowingEyes(ot, this, mPal, 64, mRed, mGreen, mBlue, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

s32 Glukkon::VGetSaveState(u8* pSaveBuffer)
{
    GlukkonSaveState* pSaveState = reinterpret_cast<GlukkonSaveState*>(pSaveBuffer);

    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
    {
        return 0;
    }
    pSaveState->mType = ReliveTypes::eGlukkon;
    pSaveState->field_4_object_id = mBaseGameObjectTlvInfo;
    pSaveState->mXPos = mXPos;
    pSaveState->mYPos = mYPos;
    pSaveState->mVelX = mVelX;
    pSaveState->mVelY = mVelY;
    pSaveState->mCurrentPath = mCurrentPath;
    pSaveState->mCurrentLevel = mCurrentLevel;
    pSaveState->mSpriteScale = GetSpriteScale();
    pSaveState->mRed = mRGB.r;
    pSaveState->mGreen = mRGB.g;
    pSaveState->mBlue = mRGB.b;
    pSaveState->mCurrentMotion = mCurrentMotion;
    pSaveState->mCurrentFrame = static_cast<u16>(GetAnimation().GetCurrentFrame());
    pSaveState->mFrameChangeCounter = static_cast<u16>(GetAnimation().GetFrameChangeCounter());
    pSaveState->mFlipX = GetAnimation().mFlags.Get(AnimFlags::eFlipX);
    pSaveState->mRender = GetAnimation().mFlags.Get(AnimFlags::eRender);
    pSaveState->mDrawable = mBaseGameObjectFlags.Get(BaseGameObject::Options::eDrawable_Bit4);
    pSaveState->mHealth = mHealth;
    pSaveState->mCurrentMotion2 = mCurrentMotion;
    pSaveState->mNextMotion = mNextMotion;
    pSaveState->field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    if (BaseAliveGameObjectCollisionLine)
    {
        pSaveState->mLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pSaveState->mLineType = -1;
    }
    pSaveState->mIsActiveChar = this == static_cast<Glukkon*>(sControlledCharacter);
    pSaveState->mTlvId = mTlvId;

    pSaveState->field_48_brain_state_idx = 0;

    s16 idx = 0;
    for (auto& fn : sGlukkon_brain_table_5544A0)
    {
        if (BrainIs(fn))
        {
            pSaveState->field_48_brain_state_idx = idx;
            break;
        }
        idx++;
    }

    pSaveState->mBrainSubState = mBrainSubState;
    pSaveState->field_54_timer = field_1D4_timer;
    pSaveState->mFallingVelXScaleFactor = mFallingVelXScaleFactor;
    pSaveState->mPreventDepossession = mPreventDepossession;
    pSaveState->mAbeLevel = mAbeLevel;
    pSaveState->mAbePath = mAbePath;
    pSaveState->mAbeCamera = mAbeCamera;
    pSaveState->mSpeak = mSpeak;
    pSaveState->mGamespeakPitch = mGamespeakPitch;
    pSaveState->mPreviousYPos = mPreviousYPos;
    pSaveState->mRandomishSpeakTimer = mRandomishSpeakTimer;
    pSaveState->mTurnOrHelpTimer = mTurnOrHelpTimer;
    pSaveState->mPanicTimer = mPanicTimer;
    pSaveState->field_7C = field_1FC;
    pSaveState->mKnockbackDelayAfterGettingShotTimer = mKnockbackDelayAfterGettingShotTimer;
    pSaveState->mGettingShotTimer = mGettingShotTimer;
    pSaveState->mFadeId = mFadeId;
    pSaveState->mCanBePossessed = mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eCanBePossessed);
    pSaveState->mCurrentType = Type();

    return sizeof(GlukkonSaveState);
}

void Glukkon::Motion_0_Idle()
{
    HandleInput();
}

void Glukkon::Motion_1_Walk()
{
    if (DoMovement())
    {
        switch (GetAnimation().GetCurrentFrame())
        {
            case 0:
            case 9:
                if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
                {
                    if (GetNextMotion() == eGlukkonMotions::Motion_4_Jump)
                    {
                        SetAnim(eGlukkonMotions::Motion_18_WalkToJump);
                    }
                }
                else if (Input().mPads[sCurrentControllerIndex].mPressed & InputCommands::Enum::eRun)
                {
                    SetAnim(eGlukkonMotions::Motion_18_WalkToJump, true);
                }

                MapFollowMe(true);
                return;

            case 2:
            case 6:
            case 10:
            case 14:
                PlaySound(0, this);
                mCurrentWalkPitch++;
                return;

            case 8:
            case 17:
                if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
                {
                    if (GetNextMotion() == eGlukkonMotions::Motion_0_Idle || GetNextMotion() == eGlukkonMotions::Motion_2_Turn || GetNextMotion() == eGlukkonMotions::Motion_11_Speak1 || GetNextMotion() == eGlukkonMotions::Motion_12_Speak2 || GetNextMotion() == eGlukkonMotions::Motion_23_Speak3 || GetNextMotion() == eGlukkonMotions::Motion_13_LongLaugh)
                    {
                        if (GetAnimation().GetCurrentFrame() != 8)
                        {
                            SetAnim(eGlukkonMotions::Motion_15_EndWalk);
                        }
                        else
                        {
                            SetAnim(eGlukkonMotions::Motion_24_EndSingleStep);
                        }
                    }
                }
                else
                {
                    if ((mVelX > FP_FromInteger(0) && (Input().mPads[sCurrentControllerIndex].mPressed & InputCommands::Enum::eLeft)) || (mVelX < FP_FromInteger(0) && (Input().mPads[sCurrentControllerIndex].mPressed & InputCommands::Enum::eRight)) || !(Input().mPads[sCurrentControllerIndex].mPressed & (InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
                    {
                        if (GetAnimation().GetCurrentFrame() == 8)
                        {
                            SetAnim(eGlukkonMotions::Motion_24_EndSingleStep, true);
                        }
                        else
                        {
                            SetAnim(eGlukkonMotions::Motion_15_EndWalk, true);
                        }
                    }
                }
                break;

            default:
                return;
        }
    }
}

void Glukkon::Motion_2_Turn()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
        ToStand();
    }
}

void Glukkon::Motion_3_KnockBack()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        PlaySound(2, this);
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        SlowDown(FP_FromDouble(0.35));
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            if (mHealth > FP_FromInteger(0))
            {
                SetAnim(eGlukkonMotions::Motion_20_KnockBackStandBegin, true);
            }
        }
    }
    else
    {
        Motion_7_Fall();
    }
}

const FP sGlukkonVelY_5453DC[10] = {
    FP_FromDouble(-10.011077880859375),
    FP_FromDouble(-10.583099365234375),
    FP_FromDouble(-7.2048492431640625),
    FP_FromDouble(-4.6619873046875),
    FP_FromInteger(-3),
    FP_FromInteger(3),
    FP_FromDouble(4.4048004150390625),
    FP_FromDouble(7.0737457275390625),
    FP_FromDouble(10.93603515625),
    FP_FromDouble(15.046417236328125)};

const FP sGlukkonJumpVelX_54539C[10] = {
    FP_FromDouble(10.335205078125),
    FP_FromDouble(21.649551391601562),
    FP_FromDouble(18.9296875),
    FP_FromDouble(12.507583618164062),
    FP_FromDouble(10.808151245117188),
    FP_FromDouble(7.7117919921875),
    FP_FromDouble(5.19439697265625),
    FP_FromDouble(4.020263671875),
    FP_FromDouble(2.5957489013671875),
    FP_FromDouble(0.920867919921875)};

void Glukkon::Motion_4_Jump()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    if (GetAnimation().GetCurrentFrame() >= 10)
    {
        JumpHelper();
        return;
    }

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -900);
        BaseAliveGameObjectLastLineYPos = mYPos;
        if (pPlatform)
        {
            pPlatform->VRemove(this);
            BaseAliveGameObject_PlatformId = Guid{};
        }
        BaseAliveGameObjectCollisionLine = nullptr;
    }

    mVelY = (GetSpriteScale() * sGlukkonVelY_5453DC[GetAnimation().GetCurrentFrame()]);

    FP velXTableValue = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        velXTableValue = -sGlukkonJumpVelX_54539C[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        velXTableValue = sGlukkonJumpVelX_54539C[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * velXTableValue);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(2), mVelX))
    {
        mVelY = FP_FromInteger(0);
        mFallingVelXScaleFactor = FP_FromDouble(0.35);
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        else
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        SetAnim(eGlukkonMotions::Motion_3_KnockBack, true);
        MapFollowMe(true);
        return;
    }

    if (sControlledCharacter == this)
    {
        SetActiveCameraDelayedFromDir();
    }

    bool bCollision = false;
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (BaseAliveGameObjectCollisionLine)
    {
        bCollision = false;
    }
    else
    {
        bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromInteger(0));
    }

    if (GetAnimation().GetCurrentFrame() == 0 || !bCollision)
    {
        if (GetAnimation().GetCurrentFrame() == 9 && !BaseAliveGameObjectCollisionLine)
        {
            SetAnim(eGlukkonMotions::Motion_7_Fall, true);
        }
        JumpHelper();
        return;
    }

    switch (pLine->mLineType)
    {
        case eLineTypes::eFloor_0:
        case eLineTypes::eBackgroundFloor_4:
        case eLineTypes::eDynamicCollision_32:
        case eLineTypes::eBackgroundDynamicCollision_36:
            PlaySound(1, this);
            mXPos = hitX;
            BaseAliveGameObjectCollisionLine = pLine;
            mYPos = hitY;
            MapFollowMe(true);
            GetOnPlatforms();
            break;

        case eLineTypes::eWallLeft_1:
        case eLineTypes::eWallRight_2:
            mVelX = (-mVelX / FP_FromInteger(2));
            break;

        default:
            if (GetAnimation().GetCurrentFrame() == 9 && !BaseAliveGameObjectCollisionLine)
            {
                SetAnim(eGlukkonMotions::Motion_7_Fall, true);
            }
            JumpHelper();
            return;
    }
}

void Glukkon::JumpHelper()
{
    if (GetAnimation().GetCurrentFrame() != 15)
    {
        return;
    }

    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        const auto input_pressed = Input().mPads[sCurrentControllerIndex].mPressed;

        if ((mVelX > FP_FromInteger(0) && (input_pressed & InputCommands::Enum::eLeft)) || (mVelX < FP_FromInteger(0) && (input_pressed & InputCommands::Enum::eRight)))
        {
            // Direction changed
            SetAnim(eGlukkonMotions::Motion_17_JumpToStand, true);
        }
        else if (input_pressed & (InputCommands::Enum::eLeft | InputCommands::Enum::eRight))
        {
            if (!(input_pressed & InputCommands::Enum::eRun))
            {
                // Still moving but not running, so start walking
                SetAnim(eGlukkonMotions::Motion_19_JumpToWalk, true);
            }
        }
        else if (!(input_pressed & InputCommands::Enum::eHop))
        {
            // Not changing direction or trying to move at all, back to standing
            SetAnim(eGlukkonMotions::Motion_17_JumpToStand, true);
        }
    }
    else
    {
        if (mNextMotion != 1)
        {
            if (GetNextMotion() == eGlukkonMotions::Motion_0_Idle || GetNextMotion() == eGlukkonMotions::Motion_2_Turn || GetNextMotion() == eGlukkonMotions::Motion_11_Speak1 || GetNextMotion() == eGlukkonMotions::Motion_12_Speak2 || GetNextMotion() == eGlukkonMotions::Motion_23_Speak3 || GetNextMotion() == eGlukkonMotions::Motion_13_LongLaugh)
            {
                // Leaving jump to a motion that requires standing
                SetAnim(eGlukkonMotions::Motion_17_JumpToStand);
            }
        }
        else
        {
            // No next motion so just stand
            SetAnim(eGlukkonMotions::Motion_19_JumpToWalk, true);
        }
    }

    MapFollowMe(true);
    return;
}

void Glukkon::Motion_5_JumpToFall()
{
    Motion_6_WalkToFall();
}

void Glukkon::Motion_6_WalkToFall()
{
    Motion_7_Fall();

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetAnim(eGlukkonMotions::Motion_7_Fall, true);
    }
}

void Glukkon::Motion_7_Fall()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (GetSpriteScale() * mFallingVelXScaleFactor);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (GetSpriteScale() * mFallingVelXScaleFactor) + mVelX;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (BrainIs(&Glukkon::Brain_3_PlayerControlled))
    {
        SetActiveCameraDelayedFromDir();
    }

    if (bCollision)
    {
        switch (pLine->mLineType)
        {
            case 0:
            case 4:
            case 32:
            case 36:
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = hitY;
                mXPos = hitX;
                mVelY = FP_FromInteger(0);

                GetOnPlatforms();

                if (hitY - BaseAliveGameObjectLastLineYPos > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(7)))
                {
                    SetAnim(eGlukkonMotions::Motion_8_DeathFall, true);
                    SetBrain(&Glukkon::Brain_4_Death);
                    mBrainSubState = 0;
                }
                else if (GetCurrentMotion() != eGlukkonMotions::Motion_3_KnockBack)
                {
                    SetAnim(eGlukkonMotions::Motion_9_Land, true);
                }
                break;

            case 1u:
            case 2u:
                mXPos = hitX;
                mVelX = -mVelX / FP_FromInteger(2);
                break;

            default:
                return;
        }
    }
}

void Glukkon::Motion_8_DeathFall()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::KillEffect, 85);
    }
}

void Glukkon::Motion_9_Land()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Glukkon::PlaySound(1, this);
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        HandleInput();
    }
}

void Glukkon::Motion_10_ChantShake()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_7_Fall();
    }
}

void Glukkon::Motion_11_Speak1()
{
    if (GetAnimation().GetCurrentFrame() == 2 && mSpeak != GlukkonSpeak::None)
    {
        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            if (field_1FC)
            {
                EventBroadcast(kEventGlukkonUnknown, this);
                field_1FC = 0;
            }

            if (BrainIs(&Glukkon::Brain_3_PlayerControlled))
            {
                GameSpeakEvents evToBePushed;
                switch (mSpeak)
                {
                    case GlukkonSpeak::Hey_0:
                        evToBePushed = GameSpeakEvents::Glukkon_Hey_36;
                        break;
                    case GlukkonSpeak::DoIt_1:
                        evToBePushed = GameSpeakEvents::Glukkon_DoIt_37;
                        break;
                    case GlukkonSpeak::StayHere_2:
                        evToBePushed = GameSpeakEvents::Glukkon_StayHere_38;
                        break;
                    case GlukkonSpeak::Commere_3:
                        evToBePushed = GameSpeakEvents::Glukkon_Commere_39;
                        break;
                    case GlukkonSpeak::AllOYa_4:
                        evToBePushed = GameSpeakEvents::Glukkon_AllOYa_40;
                        break;
                    case GlukkonSpeak::Heh_5:
                        evToBePushed = GameSpeakEvents::Glukkon_Heh_41;
                        break;
                    case GlukkonSpeak::Help_6:
                        evToBePushed = GameSpeakEvents::Glukkon_Help_42;
                        break;
                    case GlukkonSpeak::Laugh_7:
                        evToBePushed = GameSpeakEvents::Glukkon_Laugh_43;
                        break;
                    case GlukkonSpeak::KillEm_8:
                        evToBePushed = GameSpeakEvents::Glukkon_KillEm_44;
                        break;
                    case GlukkonSpeak::Unused_9:
                        evToBePushed = GameSpeakEvents::Glukkon_Unknown_45;
                        break;
                    case GlukkonSpeak::Unused_10:
                        evToBePushed = GameSpeakEvents::Glukkon_Unknown_46;
                        break;
                    case GlukkonSpeak::What_11:
                        evToBePushed = GameSpeakEvents::Glukkon_What_47;
                        break;
                    default:
                        evToBePushed = GameSpeakEvents::eUnknown_35; //GlukkonSpeak::None
                        break;
                }
                if (evToBePushed != GameSpeakEvents::eUnknown_35)
                {
                    gEventSystem->PushEvent(evToBePushed);
                }
            }

            PlaySound_GameSpeak(mSpeak, 0, 0, 0);

            if (mSpeak == GlukkonSpeak::Help_6)
            {
                SwitchStates_Do_Operation(mTlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetTrue);
            }
        }
        mSpeak = GlukkonSpeak::None;
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        HandleInput();
    }
}

void Glukkon::Motion_12_Speak2()
{
    Motion_11_Speak1();
}

void Glukkon::Motion_13_LongLaugh()
{
    Motion_11_Speak1();
}

void Glukkon::Motion_14_BeginWalk()
{
    Motion_19_JumpToWalk();
}

void Glukkon::Motion_15_EndWalk()
{
    Motion_17_JumpToStand();
}

void Glukkon::Motion_16_StandToJump()
{
    Motion_18_WalkToJump();
}

void Glukkon::Motion_17_JumpToStand()
{
    DoMovement();
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        Glukkon::HandleInput();
    }
}

void Glukkon::Motion_18_WalkToJump()
{
    DoMovement();
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetAnim(eGlukkonMotions::Motion_4_Jump);
    }
}

void Glukkon::Motion_19_JumpToWalk()
{
    DoMovement();
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetAnim(eGlukkonMotions::Motion_1_Walk);
    }
}

void Glukkon::Motion_20_KnockBackStandBegin()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
        PlaySound_GameSpeak(GlukkonSpeak::Heh_5, 0, 0, 0);
        SetAnim(eGlukkonMotions::Motion_22_KnockBackStandEnd, true);
    }
}

void Glukkon::Motion_21_GetShot()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_7_Fall();
        if (GetCurrentMotion() != eGlukkonMotions::Motion_21_GetShot)
        {
            SetAnim(eGlukkonMotions::Motion_21_GetShot, true);
        }
    }

    if (static_cast<s32>(sGnFrame) >= mGettingShotTimer)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eRender))
        {
            mBrainSubState = 2;
            const FP shotXVel = FP_FromInteger(20) * GetSpriteScale();
            if (mVelX >= FP_FromInteger(0))
            {
                mVelX = mVelX <= FP_FromInteger(0) ? FP_FromInteger(0) : shotXVel;
            }
            else
            {
                mVelX = -shotXVel;
            }
        }
    }

    if (static_cast<s32>(sGnFrame) > mKnockbackDelayAfterGettingShotTimer)
    {
        GetAnimation().mFlags.Set(AnimFlags::eFlipX, mVelX > FP_FromInteger(0));
        SetAnim(eGlukkonMotions::Motion_3_KnockBack, true);
    }
}

void Glukkon::Motion_22_KnockBackStandEnd()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetAnim(eGlukkonMotions::Motion_0_Idle, true);
    }
}

void Glukkon::Motion_23_Speak3()
{
    Motion_11_Speak1();
}

void Glukkon::Motion_24_EndSingleStep()
{
    Motion_17_JumpToStand();
}

s16 Glukkon::Brain_0_Calm_WalkAround()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    auto pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    LiftPoint* pLiftPoint = nullptr;
    if (pObj && pObj->Type() == ReliveTypes::eLiftPoint)
    {
        pLiftPoint = static_cast<LiftPoint*>(pObj);
        if (!pLiftPoint->vOnAnyFloor() && mBrainSubState != 7)
        {
            SetNextMotion(eGlukkonMotions::Motion_0_Idle);
            return 7;
        }
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        return 8;
    }

    if (sActiveHero->mHealth < FP_FromInteger(0))
    {
        Speak(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death);
        return 6;
    }

    BaseAnimatedWithPhysicsGameObject* pEvent17 = nullptr;

    switch (mBrainSubState)
    {
        case 0:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }

            if (ShouldPanic(false))
            {
                if (IsEventInRange(
                        kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + mTlvData.mScreamHelpDelay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic);
                return 0;
            }

            pEvent17 = IsEventInRange(
                kEventGlukkonUnknown,
                mXPos,
                mYPos,
                AsEventScale(GetScale()));
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = sGnFrame + 20;
                return 9;
            }

            if (mTlvData.mBehavior == relive::Path_Glukkon::Behavior::eCheckForWalls)
            {
                if (Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 1) || PathBlocked(mVelX, 1))
                {
                    SetNextMotion(eGlukkonMotions::Motion_2_Turn);
                    return 2;
                }
                SetNextMotion(eGlukkonMotions::Motion_14_BeginWalk);
                return 1;
            }
            else
            {
                SetNextMotion(eGlukkonMotions::Motion_0_Idle);
                return 1;
            }
            break;

        case 1:
            if (ShouldPanic(false))
            {
                if (IsEventInRange(
                        kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + mTlvData.mScreamHelpDelay;
                }

                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic);
                return 0;
            }
            else
            {
                auto pEvent17_1 = IsEventInRange(
                    kEventGlukkonUnknown,
                    mXPos,
                    mYPos,
                    AsEventScale(GetScale()));
                if (pEvent17_1 && pEvent17_1 != this)
                {
                    field_1D4_timer = sGnFrame + 20;
                    return 9;
                }

                if (mTlvData.mBehavior == relive::Path_Glukkon::Behavior::eCheckForWalls)
                {
                    if (Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 1) || PathBlocked(mVelX, 1))
                    {
                        if (static_cast<s32>(sGnFrame) <= mRandomishSpeakTimer)
                        {
                            SetNextMotion(eGlukkonMotions::Motion_0_Idle);
                            field_1D4_timer = sGnFrame + Math_RandomRange(30, 120);
                            return 4;
                        }
                        else
                        {
                            mRandomishSpeakTimer = sGnFrame + 120;
                            SpeakRandomish();
                            return 3;
                        }
                    }
                }
                else
                {
                    if (Math_NextRandom() < 5 && static_cast<s32>(sGnFrame) > mTurnOrHelpTimer)
                    {
                        mTurnOrHelpTimer = sGnFrame + 120;
                        SetNextMotion(eGlukkonMotions::Motion_2_Turn);
                        return 2;
                    }
                }

                if (Math_NextRandom() >= 5 || static_cast<s32>(sGnFrame) <= mRandomishSpeakTimer)
                {
                    return mBrainSubState;
                }

                mRandomishSpeakTimer = sGnFrame + 120;
                SpeakRandomish();
                return 6;
            }
            break;

        case 2:
        case 8:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return 0;

        case 3:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle || mSpeak != GlukkonSpeak::None)
            {
                return mBrainSubState;
            }
            field_1D4_timer = sGnFrame + Math_RandomRange(30, 120);
            return 4;

        case 4:
            if (ShouldPanic(false))
            {
                if (IsEventInRange(
                        kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + mTlvData.mScreamHelpDelay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic);
                return 0;
            }

            pEvent17 = IsEventInRange(
                kEventGlukkonUnknown,
                mXPos,
                mYPos,
                AsEventScale(GetScale()));
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = sGnFrame + 20;
                return 9;
            }
            else
            {
                if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
                {
                    return mBrainSubState;
                }
                SetNextMotion(eGlukkonMotions::Motion_2_Turn);
                return 2;
            }
            break;

        case 5:
            if (ShouldPanic(false))
            {
                if (IsEventInRange(
                        kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + mTlvData.mScreamHelpDelay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic);
                return 0;
            }
            else
            {
                auto pEvent17_3 = IsEventInRange(
                    kEventGlukkonUnknown,
                    mXPos,
                    mYPos,
                    AsEventScale(GetScale()));
                if (pEvent17_3 && pEvent17_3 != this)
                {
                    field_1D4_timer = sGnFrame + 20;
                    return 9;
                }
                else
                {
                    if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
                    {
                        return mBrainSubState;
                    }
                    return 0;
                }
            }
            break;

        case 6:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            field_1D4_timer = sGnFrame + Math_RandomRange(30, 120);
            return 5;

        case 7:
            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor())
                {
                    return mBrainSubState;
                }
                return 0;
            }
            else
            {
                BaseAliveGameObject_PlatformId = Guid{};
                return 0;
            }
            break;

        case 9:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
            {
                return mBrainSubState;
            }
            field_1FC = 1;
            Speak(GlukkonSpeak::Heh_5);
            return 6;

        default:
            return mBrainSubState;
    }
}

s16 Glukkon::Brain_1_Panic()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint && pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->vOnAnyFloor() && mBrainSubState != 6)
    {
        SetNextMotion(eGlukkonMotions::Motion_0_Idle);
        return 6;
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        return 7;
    }

    if (sActiveHero->mHealth < FP_FromInteger(0))
    {
        Glukkon::Speak(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death);
        return 6;
    }

    switch (mBrainSubState)
    {
        case 0:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer || GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            mPanicTimer = sGnFrame;
            Speak(GlukkonSpeak::Help_6);
            return 4;

        case 1:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }

            if (ShouldPanic(true))
            {
                mPanicTimer = sGnFrame;
            }
            else if (static_cast<s32>(sGnFrame) - mPanicTimer > mTlvData.mToCalmDelay)
            {
                SwitchStates_Do_Operation(mTlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);
                SetBrain(&Glukkon::Brain_0_Calm_WalkAround);
                return 0;
            }

            if (mTlvData.mBehavior != relive::Path_Glukkon::Behavior::eIgnoreWalls)
            {
                if (Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 1) || PathBlocked(mVelX, 1))
                {
                    SetNextMotion(eGlukkonMotions::Motion_2_Turn);
                    return 3;
                }
                SetNextMotion(eGlukkonMotions::Motion_14_BeginWalk);
            }
            else
            {
                SetNextMotion(eGlukkonMotions::Motion_0_Idle);
            }
            return 2;

        case 2:
            if (mTlvData.mBehavior == relive::Path_Glukkon::Behavior::eCheckForWalls)
            {
                if (Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 1) || PathBlocked(mVelX, 1))
                {
                    Glukkon::Speak(GlukkonSpeak::Help_6);
                    return 5;
                }
            }
            else
            {
                if (Math_NextRandom() < 5u && static_cast<s32>(sGnFrame) > mTurnOrHelpTimer)
                {
                    mTurnOrHelpTimer = sGnFrame + 120;
                    Glukkon::Speak(GlukkonSpeak::Help_6);
                    return 5;
                }
            }
            if (Math_NextRandom() >= 0xAu || static_cast<s32>(sGnFrame) <= mRandomishSpeakTimer)
            {
                return mBrainSubState;
            }
            Glukkon::Speak(GlukkonSpeak::Help_6);
            return 4;

        case 3:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return 1;

        case 4:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            mRandomishSpeakTimer = sGnFrame + 60;
            return 1;

        case 5:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle || mSpeak != GlukkonSpeak::None)
            {
                return mBrainSubState;
            }
            SetNextMotion(eGlukkonMotions::Motion_2_Turn);
            return 3;

        case 6:
            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor())
                {
                    return mBrainSubState;
                }
            }
            else
            {
                BaseAliveGameObject_PlatformId = Guid{};
            }
            return 1;

        case 7:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return 1;

        default:
            return mBrainSubState;
    }
}

s16 Glukkon::Brain_2_Slapped()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    if (GetCurrentMotion() == eGlukkonMotions::Motion_7_Fall)
    {
        return 3;
    }

    if (sActiveHero->mHealth < FP_FromInteger(0))
    {
        Glukkon::Speak(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death);
        return 6;
    }

    switch (mBrainSubState)
    {
        case 0:
            if (mCurrentMotion)
            {
                return mBrainSubState;
            }

            if (ShouldPanic(true))
            {
                mPanicTimer = sGnFrame;
            }
            else if ((s32)(sGnFrame - mPanicTimer) > mTlvData.mToCalmDelay)
            {
                SwitchStates_Do_Operation(mTlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);
                SetBrain(&Glukkon::Brain_0_Calm_WalkAround);

                if (FP_FromInteger(mTlvData.mTopLeftX) >= mXPos)
                {
                    if (!(GetAnimation().mFlags.Get(AnimFlags::eFlipX)))
                    {
                        return 2;
                    }
                    if (FP_FromInteger(mTlvData.mTopLeftX) >= mXPos)
                    {
                        return 0;
                    }
                }

                if (!(GetAnimation().mFlags.Get(AnimFlags::eFlipX)))
                {
                    return 0;
                }
                return 2;
            }

            if ((BaseAliveGameObjectCollisionLine && Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 4)) || PathBlocked(mVelX, 0))
            {
                SetNextMotion(eGlukkonMotions::Motion_2_Turn);
                return 2;
            }
            else
            {
                SetNextMotion(eGlukkonMotions::Motion_4_Jump);
                return 1;
            }
            break;

        case 1:
            if (ShouldPanic(true))
            {
                mPanicTimer = sGnFrame;
            }
            else if (static_cast<s32>(sGnFrame - mPanicTimer) > mTlvData.mToCalmDelay)
            {
                SetNextMotion(eGlukkonMotions::Motion_0_Idle);
                return 0;
            }

            if (!BaseAliveGameObjectCollisionLine || (!Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 4) && !PathBlocked(mVelX, 0)))
            {
                if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= mRandomishSpeakTimer)
                {
                    return mBrainSubState;
                }
                Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Help_6, 0, 0, 0);
                mRandomishSpeakTimer = sGnFrame + 40;
                return mBrainSubState;
            }

            SetNextMotion(eGlukkonMotions::Motion_2_Turn);
            return 2;

        case 2:
        case 3:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return 0;

        default:
            return mBrainSubState;
    }
}

s16 Glukkon::Brain_3_PlayerControlled()
{
    auto pFade = static_cast<Fade*>(sObjectIds.Find_Impl(mFadeId));
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case 0:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
            {
                return mBrainSubState;
            }
            ToStand();
            return 1;

        case 1:
        {
            if ((mTlvData.mGlukkonType != relive::Path_Glukkon::GlukkonTypes::eStoryAslik &&
                mTlvData.mGlukkonType != relive::Path_Glukkon::GlukkonTypes::eStoryDripik &&
                mTlvData.mGlukkonType != relive::Path_Glukkon::GlukkonTypes::eStoryPhleg) ||
                !SwitchStates_Get(mTlvData.mPlayMovieSwitchId))
            {
                if (Input_IsChanting_45F260() && GetCurrentMotion() != eGlukkonMotions::Motion_4_Jump && !mPreventDepossession)
                {
                    field_1D4_timer = sGnFrame + 30;
                    SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
                    SetAnim(eGlukkonMotions::Motion_10_ChantShake, true);
                    return 2;
                }
                return mBrainSubState;
            }

            if (mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryAslik)
            {
                gVisitedFeecoEnder = 1;
            }
            else if (mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryDripik)
            {
                gVisitedBarracks = 1;
            }
            else
            {
                gVisitedBonewerkz = 1;
            }

            auto pFadeMem = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeIn, 0, 8, TPageAbr::eBlend_2);
            if (pFadeMem)
            {
                mFadeId = pFadeMem->mBaseGameObjectId;
            }
        }
            return 3;

        case 2:
            if (Input_IsChanting_45F260()
                || ((mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryAslik || mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryDripik || mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryPhleg)
                    && SwitchStates_Get(mTlvData.mPlayMovieSwitchId)))
            {
                if (!(static_cast<s32>(sGnFrame) % 4))
                {
                    const FP xRand = FP_FromInteger(Math_RandomRange(-20, 20));
                    const FP yRand = FP_FromInteger(Math_RandomRange(20, 50));
                    New_TintChant_Particle(
                        (GetSpriteScale() * xRand) + mXPos,
                        mYPos - (GetSpriteScale() * yRand),
                        GetSpriteScale(),
                        Layer::eLayer_0);
                }

                if (static_cast<s32>(sGnFrame) > field_1D4_timer || sActiveHero->mHealth <= FP_FromInteger(0))
                {
                    mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::ePossessed);
                    SetBrain(&Glukkon::Brain_4_Death);
                    mBrainSubState = 2;
                    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
                }
                return mBrainSubState;
            }
            ToStand();
            return 1;

        case 3:
            if (pFade && !pFade->mDone)
            {
                return mBrainSubState;
            }

            for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
            {
                auto pObj = gBaseAliveGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == ReliveTypes::eSlig)
                {
                    pObj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }

            SetUpdateDelay(2);
            return 4;

        case 4:
        {
            pScreenManager->DisableRendering();
            sLevelId_dword_5CA408 = static_cast<u32>(MapWrapper::ToAE(gMap.mCurrentLevel));

            const FmvInfo* pFmvRec = Path_Get_FMV_Record(gMap.mCurrentLevel, mTlvData.mMovieId);
            relive_new Movie(pFmvRec->field_0_pName);
        }
            return 5;

        case 5:
            if (sMovie_ref_count_BB4AE4)
            {
                return mBrainSubState;
            }
            gPsxDisplay.PutCurrentDispEnv();
            pScreenManager->DecompressCameraToVRam(gMap.field_2C_camera_array[0]->field_C_pCamRes);
            if (pFade)
            {
                pFade->Init(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 1, 8);
            }
            return 6;

        case 6:
            GetSoundAPI().SND_Restart();
            pScreenManager->EnableRendering();
            field_1D4_timer = sGnFrame + 30;
            SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
            SetAnim(eGlukkonMotions::Motion_10_ChantShake, true);
            return 2;

        default:
            return mBrainSubState;
    }
}

static GibType AsGibType(relive::Path_Glukkon::GlukkonTypes glukkonType)
{
    switch (glukkonType)
    {
        case relive::Path_Glukkon::GlukkonTypes::eNormal:
            return GibType::Glukkon_6;

        case relive::Path_Glukkon::GlukkonTypes::eStoryAslik:
            return GibType::Aslik_7;

        case relive::Path_Glukkon::GlukkonTypes::eStoryDripik:
            return GibType::Dripik_8;

        case relive::Path_Glukkon::GlukkonTypes::eStoryPhleg:
            return GibType::Phleg_9;
        default:
            return GibType::Glukkon_6;
    }
}

s16 Glukkon::Brain_4_Death()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case 0:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_8_DeathFall || !(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            else
            {
                mHealth = FP_FromInteger(0);
                field_1D4_timer = sGnFrame + 90;
                return 1;
            }
            break;

        case 1:
            if (static_cast<s32>(sGnFrame) > field_1D4_timer)
            {
                ToDead();
                return mBrainSubState;
            }
            else
            {
                SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));

                mRGB.r -= 2;
                mRGB.g -= 2;
                mRGB.b -= 2;

                DeathSmokeEffect(true);

                return mBrainSubState;
            }
            break;

        case 2:
        {
            relive_new Gibs(
                AsGibType(mTlvData.mGlukkonType),
                mXPos,
                mYPos,
                mVelX,
                mVelY,
                GetSpriteScale(),
                0);

            relive_new Blood(
                mXPos,
                mYPos - (FP_FromInteger(30) * GetSpriteScale()),
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale(),
                20);

            New_Smoke_Particles(
                mXPos,
                mYPos - (FP_FromInteger(30) * GetSpriteScale()),
                GetSpriteScale(),
                3,
                RGB16{128, 128, 128});

            SfxPlayMono(relive::SoundEffects::KillEffect, 128, GetSpriteScale());
            SfxPlayMono(relive::SoundEffects::FallingItemHit, 90, GetSpriteScale());

            GetAnimation().mFlags.Clear(AnimFlags::eAnimate);
            GetAnimation().mFlags.Clear(AnimFlags::eRender);

            SetAnim(eGlukkonMotions::Motion_10_ChantShake, true);

            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            field_1D4_timer = sGnFrame + 40;
        }
            return 3;

        case 3:
            if (static_cast<s32>(sGnFrame) > field_1D4_timer)
            {
                ToDead();
            }
            return mBrainSubState;

        case 4:
        case 5:
            if (!BaseAliveGameObjectCollisionLine || GetCurrentMotion() != eGlukkonMotions::Motion_3_KnockBack || !(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            else
            {
                field_1D4_timer = sGnFrame + 90;
                return 1;
            }
            break;

        default:
            return mBrainSubState;
    }
}

const PSX_Point v00554768[8] = {
    {-25, -5},
    {-30, -22},
    {-25, -39},
    {-10, -51},
    {6, -39},
    {9, -23},
    {5, -6},
    {0, 0}};


s16 Glukkon::Brain_5_WaitToSpawn()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    if (mBrainSubState == 0)
    {
        if (!SwitchStates_Get(mTlvData.mSpawnSwitchId))
        {
            return mBrainSubState;
        }
        field_1D4_timer = sGnFrame + mTlvData.mSpawnDelay;
        return 1;
    }
    else if (mBrainSubState == 2)
    {
        if (GetCurrentMotion() != eGlukkonMotions::Motion_0_Idle || mSpeak != GlukkonSpeak::None)
        {
            return mBrainSubState;
        }
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround);
        mBrainSubState = 0;
        return mBrainSubState;
    }
    else if (mBrainSubState == 1)
    {
        if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
        {
            return mBrainSubState;
        }

        mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
        mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed);

        SetType(ReliveTypes::eGlukkon);

        if (mTlvData.mSpawnType == relive::Path_Glukkon::SpawnType::eFullSpawnEffects)
        {
            SFX_Play_Pitch(relive::SoundEffects::Zap1, 60, -300);

            for (const auto& p : v00554768)
            {
                const s16 sparkX = FP_GetExponent(FP_FromInteger(p.x) + mXPos + FP_FromInteger(13));
                const s16 sparkY = FP_GetExponent(mYPos + FP_FromInteger(p.y) - FP_FromInteger(11));
                relive_new Spark(FP_FromInteger(sparkX), FP_FromInteger(sparkY), FP_FromInteger(1), 9, -31, 159, SparkType::eBigChantParticle_1);
            }

            const PSX_RECT bRect = VGetBoundingRect();

            New_DestroyOrCreateObject_Particle(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2) + (GetSpriteScale() * FP_FromInteger(60)),
                GetSpriteScale());

            relive_new ParticleBurst(
                mXPos,
                mYPos - FP_FromInteger(18),
                6,
                FP_FromInteger(1),
                BurstType::eBigRedSparks_3,
                9);

            Speak(GlukkonSpeak::Heh_5);
            return 2;
        }
        else
        {
            SfxPlayMono(relive::SoundEffects::GlukkonSpawn, 0);
            Speak(GlukkonSpeak::Heh_5);
            return 2;
        }
    }
    return mBrainSubState;
}

void Glukkon::Init()
{
    GetAnimation().mFlags.Set(AnimFlags::eAnimate);
    GetAnimation().mFlags.Set(AnimFlags::eRender);

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    SetTint(&kGlukkonTints_5546B4[0], gMap.mCurrentLevel);
    mXPos = FP_FromInteger((mTlvData.mTopLeftX + mTlvData.mBottomRightX) / 2);
    mYPos = FP_FromInteger(mTlvData.mTopLeftY);

    if (mTlvData.mFacing == relive::Path_Glukkon::Facing::eLeft)
    {
        GetAnimation().mFlags.Set(AnimFlags::eFlipX);
    }

    if (mTlvData.mSpawnSwitchId)
    {
        if (mTlvData.mSpawnType == relive::Path_Glukkon::SpawnType::eFacingLeft)
        {
            mXPos -= ScaleToGridSize(GetSpriteScale());
            GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
        }
        else if (mTlvData.mSpawnType == relive::Path_Glukkon::SpawnType::eFacingRight)
        {
            mXPos += ScaleToGridSize(GetSpriteScale());
            GetAnimation().mFlags.Set(AnimFlags::eFlipX);
        }
        mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eCanBePossessed);
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        SetBrain(&Glukkon::Brain_5_WaitToSpawn);
        mBrainSubState = 0;
        SetType(ReliveTypes::eNone);
    }
    else
    {
        mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed);
        SetType(ReliveTypes::eGlukkon);
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround);
        mBrainSubState = 0;
    }

    if (mTlvData.mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
    }
    else if (mTlvData.mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(79),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    SetAnim(eGlukkonMotions::Motion_0_Idle, true);

    mFadeId = Guid{};
    BaseAliveGameObject_PlatformId = Guid{};
    field_1D4_timer = 0;
    mHealth = FP_FromInteger(1);
    mRandomishSpeakTimer = 0;
    mTurnOrHelpTimer = 0;
    mPanicTimer = 0;
    mSpeak = GlukkonSpeak::None;
    mGamespeakPitch = 0;
    field_1FC = 0;
    mKnockbackDelayAfterGettingShotTimer = 0;
    mGettingShotTimer = 0;
    mPreviousYPos = mYPos;

    if (!GetShadow())
    {
        CreateShadow();
    }
}

Glukkon::~Glukkon()
{
    if (mHealth <= FP_FromInteger(0))
    {
        Path::TLV_Reset(mTlvId, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(mTlvId, -1, 0, 0);
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4); // Seems wrong to do this here ??

    if (this == sControlledCharacter)
    {
        sControlledCharacter = sActiveHero;
    }
}

void Glukkon::VUpdate()
{
    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eRestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eRestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            sCollisions->Raycast(
                mXPos,
                mYPos - FP_FromInteger(20),
                mXPos,
                mYPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mXPos,
                &mYPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));

            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();

                PSX_Point xy = {bRect.x, bRect.y};
                PSX_Point wh = {bRect.w, bRect.h};
                xy.y += 5;
                wh.y += 5;

                OnCollisionWith(xy, wh, gPlatformsArray);
            }
        }
        mFadeId = BaseGameObject::RefreshId(mFadeId);
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (!Input_IsChanting_45F260())
        {
            mPreventDepossession = 0;
        }

        const auto oldMotion = mCurrentMotion;

        mBrainSubState = (this->*mBrainState)();

        const FP oldXPos = mXPos;
        const FP oldYPos = mYPos;

        (this->*sGlukkonMotionTable[mCurrentMotion])();

        // TODO: This is extra debug logging to figure out the motion names
        if (oldMotion != mCurrentMotion)
        {
            //LOG_INFO("Glukkon: Old motion = " << sGlukkonMotionNames[oldMotion] << " new motion = " << sGlukkonMotionNames[mCurrentMotion]);
        }

        if (oldXPos != mXPos || oldYPos != mYPos)
        {
            relive::Path_TLV* pTlv = sPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(pTlv);
        }

        Update_Slurg_WatchPoints();

        if (sControlledCharacter == this && BaseAliveGameObject_PlatformId != Guid{})
        {
            mVelY = mYPos - mPreviousYPos;
            SetActiveCameraDelayedFromDir();
        }
        mPreviousYPos = mYPos;
    }
}

void Glukkon::VPossessed()
{
    SwitchStates_Do_Operation(mTlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);
    mBaseAliveGameObjectFlags.Set(AliveObjectFlags::ePossessed);
    mPreventDepossession = 1;
    SetAnim(eGlukkonMotions::Motion_10_ChantShake, true);
    SetBrain(&Glukkon::Brain_3_PlayerControlled);
    mBrainSubState = 0;
    field_1D4_timer = sGnFrame + 35;
    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;
}

void Glukkon::Update_Slurg_WatchPoints()
{
    if (GetCurrentMotion() == eGlukkonMotions::Motion_1_Walk || (GetCurrentMotion() == eGlukkonMotions::Motion_4_Jump && GetAnimation().GetCurrentFrame() > 8))
    {
        if (sGnFrame & 1)
        {
            const s8 count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
            if (count < 5)
            {
                Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
                pPoints->mPoints[count].x = FP_GetExponent(mXPos);
                pPoints->mPoints[count].y = BaseAliveGameObjectCollisionLine->mRect.y - 5;
                sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
            }
        }
    }
}

void Glukkon::SetAnim(eGlukkonMotions currentMotion, s16 bClearNextMotion)
{
    s32 typeIndex = static_cast<s32>(mTlvData.mGlukkonType);
    if (typeIndex > 3)
    {
        typeIndex = 0;
    }

    s16 motionIdx = static_cast<s16>(currentMotion);
    GetAnimation().Set_Animation_Data(GetAnimRes(sGlukkonsAnimIdTable[typeIndex][motionIdx]));

    SetCurrentMotion(currentMotion);
    if (bClearNextMotion)
    {
        mNextMotion = -1;
    }
}

void Glukkon::Speak(GlukkonSpeak speak)
{
    switch (speak)
    {
        case GlukkonSpeak::Hey_0:
        case GlukkonSpeak::DoIt_1:
        case GlukkonSpeak::Heh_5:
        case GlukkonSpeak::Help_6:
            SetNextMotion(eGlukkonMotions::Motion_11_Speak1);
            mSpeak = speak;
            break;

        case GlukkonSpeak::StayHere_2:
            SetNextMotion(eGlukkonMotions::Motion_23_Speak3);
            mSpeak = speak;
            break;

        case GlukkonSpeak::Commere_3:
        case GlukkonSpeak::AllOYa_4:
        case GlukkonSpeak::KillEm_8:
        case GlukkonSpeak::Unused_9:
        case GlukkonSpeak::Unused_10:
            SetNextMotion(eGlukkonMotions::Motion_12_Speak2);
            mSpeak = speak;
            break;

        case GlukkonSpeak::Laugh_7:
            SetNextMotion(eGlukkonMotions::Motion_13_LongLaugh);
            mSpeak = speak;
            break;

        default:
            mSpeak = speak;
            break;
    }
}

void Glukkon::HandleInput()
{
    MapFollowMe(true);

    if (BrainIs(&Glukkon::Brain_3_PlayerControlled) && mBrainSubState == 1 && !(mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eTeleporting)))
    {
        const auto inputHeld = Input().mPads[sCurrentControllerIndex].mHeld;
        const auto matchButtons = InputCommands::Enum::eGameSpeak1 | InputCommands::Enum::eGameSpeak2 | InputCommands::Enum::eGameSpeak3 | InputCommands::Enum::eGameSpeak4 | InputCommands::Enum::eGameSpeak5 | InputCommands::Enum::eGameSpeak6 | InputCommands::Enum::eGameSpeak7 | InputCommands::Enum::eGameSpeak8 | InputCommands::Enum::eChant;

        if (inputHeld & matchButtons)
        {
            mSpeak = GlukkonSpeak::None;

            if (inputHeld & InputCommands::Enum::eGameSpeak1)
            {
                mSpeak = GlukkonSpeak::Hey_0;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak2)
            {
                mSpeak = GlukkonSpeak::Commere_3;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak3)
            {
                mSpeak = GlukkonSpeak::StayHere_2;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak4)
            {
                mSpeak = GlukkonSpeak::DoIt_1;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak5)
            {
                mSpeak = GlukkonSpeak::KillEm_8;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak7)
            {
                mSpeak = GlukkonSpeak::Help_6;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak8)
            {
                mSpeak = GlukkonSpeak::Laugh_7;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak6)
            {
                mSpeak = GlukkonSpeak::AllOYa_4;
            }

            if (mSpeak != GlukkonSpeak::None)
            {
                Speak(mSpeak);
            }
        }
        else
        {
            const auto inputPressed = Input().mPads[sCurrentControllerIndex].mPressed;
            if (inputPressed & InputCommands::Enum::eRight)
            {
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    SetNextMotion(eGlukkonMotions::Motion_2_Turn);
                }
                else
                {
                    SetNextMotion(eGlukkonMotions::Motion_14_BeginWalk);
                }
            }
            else if (inputPressed & InputCommands::Enum::eLeft)
            {
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    SetNextMotion(eGlukkonMotions::Motion_14_BeginWalk);
                }
                else
                {
                    SetNextMotion(eGlukkonMotions::Motion_2_Turn);
                }
            }

            if (inputPressed & InputCommands::Enum::eRun)
            {
                if (GetNextMotion() == eGlukkonMotions::Motion_1_Walk)
                {
                    SetNextMotion(eGlukkonMotions::Motion_4_Jump);
                }
            }

            if (inputPressed & InputCommands::Enum::eHop)
            {
                SetNextMotion(eGlukkonMotions::Motion_4_Jump);
            }
        }
    }

    switch (GetNextMotion())
    {
    case eGlukkonMotions::eNone_m1:
            if (GetCurrentMotion() == eGlukkonMotions::Motion_0_Idle)
            {
                return;
            }
            SetNextMotion(eGlukkonMotions::Motion_0_Idle);
            SetAnim(GetNextMotion(), true);
            break;

        case eGlukkonMotions::Motion_0_Idle:
        case eGlukkonMotions::Motion_2_Turn:
        case eGlukkonMotions::Motion_11_Speak1:
        case eGlukkonMotions::Motion_12_Speak2:
        case eGlukkonMotions::Motion_13_LongLaugh:
        case eGlukkonMotions::Motion_23_Speak3:
            SetAnim(GetNextMotion(), true);
            break;

        case eGlukkonMotions::Motion_1_Walk:
        case eGlukkonMotions::Motion_14_BeginWalk:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_1_Walk)
            {
                mCurrentWalkPitch = 0;
            }
            [[fallthrough]];

        case eGlukkonMotions::Motion_4_Jump:
        case eGlukkonMotions::Motion_16_StandToJump:
        {
            FP xOff = {};
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                xOff = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                xOff = ScaleToGridSize(GetSpriteScale());
            }
            if (!WallHit(GetSpriteScale() * FP_FromInteger(50), xOff))
            {
                SetAnim(GetNextMotion(), true);
            }
        }
        break;

        default:
            return;
    }
}

s16 Glukkon::ShouldPanic(s16 panicEvenIfNotFacingMe)
{
    if (IsLineOfSightBetween(this, sControlledCharacter)
        && !(sControlledCharacter->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
        && !BaseAliveGameObject::IsInInvisibleZone(sControlledCharacter)
        && !EventGet(kEventResetting)
        && gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0)
        && gMap.Is_Point_In_Current_Camera(
            sControlledCharacter->mCurrentLevel,
            sControlledCharacter->mCurrentPath,
            sControlledCharacter->mXPos,
            sControlledCharacter->mYPos,
            0)
        && (panicEvenIfNotFacingMe || VIsFacingMe(sControlledCharacter)))
    {
        return 1;
    }

    // Panic if Abe is chanting
    if (IsEventInRange(
            kEventAbeOhm,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        return 1;
    }

    // Panic if an alarm is on
    if (EventGet(kEventAlarm))
    {
        return 1;
    }

    // Panic if player is speaking (I guess it can be rather alarming?)
    auto pSpeakEvent = IsEventInRange(
        kEventSpeaking,
        mXPos,
        mYPos,
        AsEventScale(GetScale()));
    return pSpeakEvent && pSpeakEvent == sControlledCharacter;
}

s16 Glukkon::PathBlocked(FP /*a2*/, s16 checkBounds)
{
    FP gridSize = ScaleToGridSize(GetSpriteScale());
    if (GetCurrentMotion() == eGlukkonMotions::Motion_4_Jump)
    {
        gridSize = (gridSize * FP_FromInteger(4));
    }

    relive::Path_EnemyStopper::StopDirection direction = relive::Path_EnemyStopper::StopDirection::Left;
    ReliveTypes boundType = ReliveTypes::eScrabLeftBound;
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        boundType = ReliveTypes::eScrabLeftBound;
        direction = relive::Path_EnemyStopper::StopDirection::Left;
        gridSize = -gridSize;
    }
    else
    {
        boundType = ReliveTypes::eScrabRightBound;
        direction = relive::Path_EnemyStopper::StopDirection::Right;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), gridSize * FP_FromInteger(1)))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = sPathInfo->TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos), // TODO Abs() ??
        FP_GetExponent(mXPos + gridSize),
        FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
        ReliveTypes::eSlamDoor);

    auto pSlamDoorTlv = static_cast<relive::Path_SlamDoor*>(BaseAliveGameObjectPathTLV);

    if (pSlamDoorTlv && ((pSlamDoorTlv->mStartClosed == relive::reliveChoice::eYes && !SwitchStates_Get(pSlamDoorTlv->mSwitchId)) || (pSlamDoorTlv->mStartClosed == relive::reliveChoice::eNo && SwitchStates_Get(pSlamDoorTlv->mSwitchId))))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = sPathInfo->TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos + gridSize),
        FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
        ReliveTypes::eEnemyStopper);

    auto pEnemyStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
    if (pEnemyStopper
        && (pEnemyStopper->mStopDirection == direction || pEnemyStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
        && SwitchStates_Get(pEnemyStopper->mSwitchId))
    {
        return 1;
    }

    if (!checkBounds)
    {
        return 0;
    }

    if (sPathInfo->TLV_Get_At(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos), // TODO: Abs() ??
            FP_GetExponent(mXPos + gridSize),
            FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
            boundType))
    {
        return 1;
    }

    return 0;
}

void Glukkon::SpeakRandomish()
{
    if (sGnFrame & 1)
    {
        if ((sGnFrame & 1) == 1)
        {
            Speak(GlukkonSpeak::Heh_5);
        }
    }
    else
    {
        Speak(GlukkonSpeak::Laugh_7);
    }
}

const FP sWalkData_545354[18] = {
    FP_FromInteger(0),
    FP_FromDouble(2.1124420166015625),
    FP_FromDouble(3.923065185546875),
    FP_FromDouble(2.1124420166015625),
    FP_FromDouble(0.816680908203125),
    FP_FromDouble(4.986083984375),
    FP_FromDouble(6.017730712890625),
    FP_FromDouble(2.9228973388671875),
    FP_FromDouble(2.1124114990234375),
    FP_FromDouble(3.923095703125),
    FP_FromDouble(2.1124114990234375),
    FP_FromDouble(0.816680908203125),
    FP_FromDouble(4.986114501953125),
    FP_FromDouble(6.0177154541015625),
    FP_FromDouble(2.9228973388671875),
    FP_FromDouble(2.1124114990234375),
    FP_FromDouble(3.923095703125),
    FP_FromDouble(2.1124114990234375)};


// These tables just contain all zeros
// TODO/NOTE: These are all pointless - the logic in 0x444190 will use 0 if there is no table
const FP sJumpToFallData_54542C[10] = {};
const FP sWalkToFallData_54541C[4] = {};
const FP sBeginWalkData_545454[6] = {};
const FP sEndWalkData_54546C[6] = {};
const FP sStandToJumpData_545484[6] = {};
const FP sJumpToStandData_54549C[6] = {};
const FP sWalkToJumpData_5454B4[6] = {};
const FP sJumpToWalkData_5454CC[26] = {};

const FP* motion_velx_table_5547C4[25] = {
    nullptr,                 // Motion_0_Idle
    sWalkData_545354,        // Motion_1_Walk
    nullptr,                 // Motion_2_Turn
    nullptr,                 // Motion_3_KnockBack
    sGlukkonJumpVelX_54539C, // Motion_4_Jump
    sJumpToFallData_54542C,  // Motion_5_JumpToFall
    sWalkToFallData_54541C,  // Motion_6_WalkToFall
    nullptr,                 // Motion_7_Fall
    nullptr,                 // Motion_8_DeathFall
    nullptr,                 // Motion_9_Land
    nullptr,                 // Motion_10_ChantShake
    nullptr,                 // Motion_11_Speak1
    nullptr,                 // Motion_12_Speak2
    nullptr,                 // Motion_13_LongLaugh
    sBeginWalkData_545454,   // Motion_14_BeginWalk
    sEndWalkData_54546C,     // Motion_15_EndWalk
    sStandToJumpData_545484, // Motion_16_StandToJump
    sJumpToStandData_54549C, // Motion_17_JumpToStand
    sWalkToJumpData_5454B4,  // Motion_18_WalkToJump
    sJumpToWalkData_5454CC,  // Motion_19_JumpToWalk
    nullptr,                 // Motion_20_KnockBackStandBegin
    nullptr,                 // Motion_21_GetShot
    nullptr,                 // Motion_22_KnockBackStandEnd
    nullptr,                 // Motion_23_Speak3
    nullptr                  // Motion_24_EndSingleStep
};


s16 Glukkon::DoMovement()
{
    const FP* pTable = motion_velx_table_5547C4[mCurrentMotion];
    if (pTable)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = -pTable[GetAnimation().GetCurrentFrame()];
        }
        else
        {
            mVelX = pTable[GetAnimation().GetCurrentFrame()];
        }
    }
    else
    {
        mVelX = FP_FromInteger(0);
    }

    mVelX = mVelX * GetSpriteScale();

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        mFallingVelXScaleFactor = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        else
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        MapFollowMe(true);
        SetAnim(eGlukkonMotions::Motion_3_KnockBack, true);
        return false;
    }
    else
    {
        FollowLine();
        return BaseAliveGameObjectCollisionLine != nullptr;
    }
}

void Glukkon::FollowLine()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    const FP prevXPos = mXPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObject_PlatformId = Guid{};
                    mFallingVelXScaleFactor = FP_FromDouble(0.35);
                }
            }
            else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                GetOnPlatforms();
            }
        }
        else
        {
            BaseAliveGameObjectLastLineYPos = mYPos;

            VOnTrapDoorOpen();

            if (GetCurrentMotion() == eGlukkonMotions::Motion_1_Walk || GetCurrentMotion() == eGlukkonMotions::Motion_14_BeginWalk || GetCurrentMotion() == eGlukkonMotions::Motion_15_EndWalk || GetCurrentMotion() == eGlukkonMotions::Motion_18_WalkToJump)
            {
                SetAnim(eGlukkonMotions::Motion_6_WalkToFall, true);
            }
            else if (GetCurrentMotion() == eGlukkonMotions::Motion_4_Jump || GetCurrentMotion() == eGlukkonMotions::Motion_16_StandToJump || GetCurrentMotion() == eGlukkonMotions::Motion_17_JumpToStand || GetCurrentMotion() == eGlukkonMotions::Motion_19_JumpToWalk)
            {
                SetAnim(eGlukkonMotions::Motion_5_JumpToFall, true);
            }
            else
            {
                SetAnim(eGlukkonMotions::Motion_7_Fall, true);
            }

            mXPos = prevXPos + mVelX;
            mFallingVelXScaleFactor = FP_FromInteger(1);

            if (GetCurrentMotion() == eGlukkonMotions::Motion_3_KnockBack)
            {
                mFallingVelXScaleFactor = FP_FromDouble(0.67);
            }
        }
    }
    else
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        SetAnim(eGlukkonMotions::Motion_7_Fall, true);
    }
}

void Glukkon::GetOnPlatforms()
{
    OnCollisionWith(
        {FP_GetExponent(mXPos - FP_FromInteger(5)), FP_GetExponent(mYPos - FP_FromInteger(5))},
        {FP_GetExponent(mXPos + FP_FromInteger(5)), FP_GetExponent(mYPos + FP_FromInteger(5))},
        gPlatformsArray);
}

const relive::SfxDefinition stepSfx_554840[3] = {
    {0u, 8u, 36u, 25u, 1524, 1905},
    {0u, 3u, 59u, 60u, 0, 254},
    {0u, 3u, 72u, 120u, 0, 254}};

void Glukkon::PlaySound(s32 sndIdx, Glukkon* pGlukkon)
{
    s32 volumeLeft, volumeRight;
    s32 defaultSndIdxVol = stepSfx_554840[sndIdx].field_C_default_volume;

    s16 pitch;
    if (sndIdx || !pGlukkon)
    {
        pitch = Math_RandomRange(stepSfx_554840[sndIdx].field_E_pitch_min, stepSfx_554840[sndIdx].field_10_pitch_max);
    }
    else
    {
        s16 pitchCap = pGlukkon->mCurrentWalkPitch;
        if (pitchCap > 12)
        {
            pitchCap = pitchCap % 4 + 12;
        }
        pitch = 127 * pitchCap;
    }

    if (pGlukkon->GetSpriteScale() == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else // Glukkon in background layer? TODO figure out if this does actually happen
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap.GetDirection(
        pGlukkon->mCurrentLevel,
        pGlukkon->mCurrentPath,
        pGlukkon->mXPos,
        pGlukkon->mYPos);
    PSX_RECT worldRect;
    gMap.Get_Camera_World_Rect(direction, &worldRect);
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
        {
            volumeLeft = volumeRight;
            break;
        }
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
        break;
        case CameraPos::eCamLeft_3:
        {
            FP percentHowFar = (FP_FromInteger(worldRect.w) - pGlukkon->mXPos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            break;
        }
        case CameraPos::eCamRight_4:
        {
            FP percentHowFar = (pGlukkon->mXPos - FP_FromInteger(worldRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            break;
        }
        default:
            return;
    }

    if (pGlukkon->GetSpriteScale() == FP_FromDouble(0.5)) //TODO figure out if this does actually happen
    {
        volumeLeft = FP_GetExponent(FP_FromInteger(volumeLeft * 2) / FP_FromInteger(3));
        volumeRight = FP_GetExponent(FP_FromInteger(volumeRight * 2) / FP_FromInteger(3));
    }

    SFX_SfxDefinition_Play_Stereo(stepSfx_554840[sndIdx], (s16) volumeLeft, (s16) volumeRight, pitch, pitch);
}

void Glukkon::ToDead()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (sControlledCharacter == this)
    {
        // When its a player controlled gluk go back to the screen the player is in
        sControlledCharacter = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

        if (gMap.mNextLevel != EReliveLevelIds::eMenu)
        {
            gMap.SetActiveCam(
                mAbeLevel,
                mAbePath,
                mAbeCamera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    SwitchStates_Do_Operation(mTlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);

    if ((mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryAslik ||
        mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryDripik ||
        mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryPhleg) &&
        !SwitchStates_Get(mTlvData.mPlayMovieSwitchId))
    {
        // If an exec is dead trigger ze gas
        SwitchStates_Do_Operation(mTlvData.mDeathSwitchId, relive::reliveSwitchOp::eSetTrue);
    }

    if (mTlvData.mSpawnSwitchId == 0)
    {
        // Don't spawn again, dead
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (mTlvData.mSpawnSwitchId > 1)
        {
            // If above 1 then its an actual switch id
            SwitchStates_Do_Operation(mTlvData.mSpawnSwitchId, relive::reliveSwitchOp::eSetFalse);
        }

        // Spawn again if id is 1 or above
        Init();
    }
}

void Glukkon::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);

                PlaySound_GameSpeak(GlukkonSpeak::Unused_9, 0, 0, 0);
                ToDead();

                // Muds love it when people DIE
                EventBroadcast(kEventMudokonComfort, this);
            }
        }

        pTlv = sPathInfo->TlvGetAt(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

const relive::SfxDefinition gameSpeak_554858[15] = {
    {0u, 8u, 66u, 127u, 0, 0},
    {0u, 8u, 64u, 127u, 0, 0},
    {0u, 8u, 67u, 127u, 0, 0},
    {0u, 8u, 61u, 127u, 0, 0},
    {0u, 8u, 63u, 127u, 0, 0},
    {0u, 8u, 70u, 127u, 0, 0},
    {0u, 8u, 65u, 127u, 0, 0},
    {0u, 8u, 69u, 127u, 0, 0},
    {0u, 8u, 71u, 127u, 0, 0},
    {0u, 8u, 60u, 127u, 0, 0},
    {0u, 8u, 60u, 127u, 0, 0},
    {0u, 8u, 62u, 127u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

void Glukkon::PlaySound_GameSpeak(GlukkonSpeak sndIdx, s16 volume, s16 pitch, Glukkon* pGlukkon)
{
    auto sndIdxShort = (s16) sndIdx;
    s16 calcedVolume = volume;
    if (!calcedVolume)
    {
        calcedVolume = gameSpeak_554858[sndIdxShort].field_C_default_volume;
    }
    if (pGlukkon)
    {
        if (pGlukkon->GetSpriteScale() == FP_FromDouble(0.5))
        {
            calcedVolume = FP_GetExponent(FP_FromInteger(calcedVolume * 2) / FP_FromInteger(3));
        }
    }
    SFX_SfxDefinition_Play_Mono(gameSpeak_554858[sndIdxShort], calcedVolume, pitch, pitch);
}

bool Glukkon::IsLineOfSightBetween(Glukkon* pGlukkon, IBaseAliveGameObject* pOther)
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pathLine = nullptr;

    const PSX_RECT bRect = pOther->VGetBoundingRect();
    const FP X2 = FP_FromInteger((bRect.x + bRect.w) / 2);
    const FP Y2 = FP_FromInteger((bRect.y + bRect.y) / 2);

    return sCollisions->Raycast(
               pGlukkon->mXPos,
               pGlukkon->mYPos,
               X2,
               Y2,
               &pathLine,
               &hitX,
               &hitY,
               pGlukkon->GetScale() == Scale::Fg ? kFgFloorWallOrCeiling
               : kBgFloorWallOrCeiling)
        != 1;
}

void Glukkon::ToStand()
{
    mFallingVelXScaleFactor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    SetAnim(eGlukkonMotions::Motion_0_Idle);
    MapFollowMe(true);
}

void Glukkon::SlowDown(FP speed)
{
    if (FP_GetExponent(mVelX))
    {
        FollowLine();
        if (mVelX <= FP_FromInteger(0))
        {
            if (mVelX < FP_FromInteger(0))
            {
                mVelX = (GetSpriteScale() * speed) + mVelX;
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = FP_FromInteger(0);
                }
            }
        }
        else
        {
            mVelX = mVelX - (GetSpriteScale() * speed);
            if (mVelX < FP_FromInteger(0))
            {
                mVelX = FP_FromInteger(0);
            }
        }
    }
}

void Glukkon::VScreenChanged()
{
    BaseGameObject::VScreenChanged();
    SwitchStates_Do_Operation(mTlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);
    if (BrainIs(&Glukkon::Brain_5_WaitToSpawn) && !mBrainSubState)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Glukkon::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        SetAnim(eGlukkonMotions::Motion_6_WalkToFall, true);
    }
}

s16 Glukkon::VTakeDamage(BaseGameObject* pFrom)
{
    if (BrainIs(&Glukkon::Brain_4_Death))
    {
        return 1;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        {
            auto pBullet = static_cast<Bullet*>(pFrom);
            switch (pBullet->mBulletType)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                {
                    {
                        const FP yRand = (FP_FromInteger(Math_NextRandom() % 16)) - FP_FromInteger(8);
                        const FP xRand = FP_FromInteger(Math_NextRandom() & 0xF); // TODO: Might be wrong as was trying to make this abs() but result is unsigned anyway ??

                        const FP xPos = (GetSpriteScale() * (pBullet->mXDistance <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6)));
                        relive_new Blood(
                            xPos + mXPos,
                            mYPos - (FP_FromInteger(25) * GetSpriteScale()),
                            ((pBullet->mXDistance <= FP_FromInteger(0) ? -FP_FromInteger(1) : FP_FromInteger(1)) * xRand + FP_FromInteger(16)),
                            yRand,
                            GetSpriteScale(),
                            12);
                    }

                    {
                        const FP xPos = (GetSpriteScale() * (pBullet->mXDistance <= FP_FromInteger(0) ? -FP_FromInteger(12) : FP_FromInteger(12)));
                        relive_new Blood(
                            xPos + mXPos,
                            mYPos - (FP_FromInteger(25) * GetSpriteScale()),
                            pBullet->mXDistance <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6),
                            FP_FromInteger(0),
                            GetSpriteScale(),
                            8);
                    }
                }
                break;

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    relive_new Blood(
                        mXPos,
                        mYPos - (FP_FromInteger(25) * GetSpriteScale()),
                        FP_FromInteger(0),
                        FP_FromInteger(0),
                        GetSpriteScale(),
                        25);
                }
                break;

                default:
                    break;
            }

            mKnockbackDelayAfterGettingShotTimer = sGnFrame + 5;

            if (GetCurrentMotion() == eGlukkonMotions::Motion_21_GetShot)
            {
                mHealth = FP_FromInteger(0);
                EventBroadcast(kEventMudokonComfort, this);
                return 1;
            }

            if (GetCurrentMotion() == eGlukkonMotions::Motion_3_KnockBack)
            {
                if (mHealth > FP_FromInteger(0))
                {
                    SetBrain(&Glukkon::Brain_4_Death);
                    mBrainSubState = 4;
                }
                mHealth = FP_FromInteger(0);
                EventBroadcast(kEventMudokonComfort, this);
                return 1;
            }

            mGettingShotTimer = sGnFrame + 20;
            SetAnim(eGlukkonMotions::Motion_21_GetShot, true);

            SetBrain(&Glukkon::Brain_4_Death);
            mBrainSubState = 4;

            if (pBullet->mXDistance >= FP_FromInteger(0))
            {
                mVelX = FP_FromDouble(0.001);
            }
            else
            {
                mVelX = -FP_FromDouble(0.001);
            }
            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonComfort, this);
        }
        break;

        case ReliveTypes::eDrill:
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eAirExplosion:
            SetBrain(&Glukkon::Brain_4_Death);
            mBrainSubState = 2;
            EventBroadcast(kEventMudokonComfort, this);
            break;

        case ReliveTypes::eElectricWall:
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Unused_9, 0, mGamespeakPitch, this);
            return 1;

        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMineCar:
            SetAnim(eGlukkonMotions::Motion_8_DeathFall, true);
            SetBrain(&Glukkon::Brain_4_Death);
            mBrainSubState = 0;
            EventBroadcast(kEventMudokonComfort, this);
            break;

        case ReliveTypes::eAbe:
            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_62_Punch_454750)
            {
                if (Math_NextRandom() <= 32u)
                {
                    Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Unused_9, 0, 0, 0);
                }
                else
                {
                    Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Hey_0, 0, 0, 0);
                }
                mPanicTimer = sGnFrame;
                SetAnim(eGlukkonMotions::Motion_3_KnockBack, true);
                SetBrain(&Glukkon::Brain_2_Slapped);
                mBrainSubState = 0;
            }
            break;

        case ReliveTypes::eSlog:
            if (GetCurrentMotion() != eGlukkonMotions::Motion_3_KnockBack)
            {
                mHealth = FP_FromInteger(0);
                SetBrain(&Glukkon::Brain_4_Death);
                mBrainSubState = 5;
                Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
                EventBroadcast(kEventMudokonComfort, this);
                if (!VIsFacingMe(static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)))
                {
                    GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
                }
                mVelX = FP_FromInteger(0);
                SetAnim(eGlukkonMotions::Motion_3_KnockBack, true);
            }
            break;

        case ReliveTypes::eElectrocute:
            GetAnimation().mFlags.Clear(AnimFlags::eRender);
            mHealth = FP_FromInteger(0);
            SetBrain(&Glukkon::Brain_4_Death);
            mBrainSubState = 3;
            field_1D4_timer = sGnFrame + 1;
            EventBroadcast(kEventMudokonComfort, this);
            break;

        default:
            return 1;
    }

    return 1;
}
