#include "stdafx.h"
#include "Glukkon.hpp"
#include "../relive_lib/Function.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "../relive_lib/Shadow.hpp"
#include "MusicController.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/GameObjects/Spark.hpp"
#include "Slig.hpp" //used for renderWithGlowingEyes() only - TODO refactor
#include "../relive_lib/GameObjects/ParticleBurst.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "Slurg.hpp"
#include "../relive_lib/GameObjects/Fade.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Movie.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Sound/Sound.hpp"
#include "MainMenu.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "../relive_lib/GameObjects/Blood.hpp"
#include "Bullet.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/Grid.hpp"
#include "../relive_lib/Camera.hpp"
#include "Path.hpp"
#include "QuikSave.hpp"
#include "../relive_lib/data_conversion/AnimationConverter.hpp"

constexpr TGlukkonMotionFn sGlukkonMotionTable[25] = {
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

static const TGlukkonBrainFn sGlukkonBrainTable[6] = {
    &Glukkon::Brain_0_Calm_WalkAround,
    &Glukkon::Brain_1_Panic,
    &Glukkon::Brain_2_Slapped,
    &Glukkon::Brain_3_PlayerControlled,
    &Glukkon::Brain_4_Death,
    &Glukkon::Brain_5_WaitToSpawn};

static const TintEntry kGlukkonTints[16] = {
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

enum Brain_0_Calm_WalkAround
{
    eBrain0_WalkAround0,
    eBrain0_Unknown1,
    eBrain0_Unknown2,
    eBrain0_Unknown3,
    eBrain0_Unknown4,
    eBrain0_Unknown5,
    eBrain0_Unknown6,
    eBrain0_OnMovingLift7,
    eBrain0_InAir8,
    eBrain0_SayHeh9,
};

enum Brain_1_Panic
{
    eBrain1_PanicStart0,
    eBrain1_PanicWalk1,
    eBrain1_ScreamForHelp2,
    eBrain1_WaitUntilIdleMotion3,
    eBrain1_SetupSpeakTimer4,
    eBrain1_TurnAround5,
    eBrain1_OnMovingLift6,
    eBrain1_InAir7
};

enum Brain_2_Slapped
{
    eBrain2_Unknown0,
    eBrain2_Unknown1,
    eBrain2_Unknown2,
    eBrain2_Unknown3
};

enum Brain_3_PlayerControlled
{
    eBrain3_ToStand0,
    eBrain3_Possessed1,
    eBrain3_TryDepossess2,
    eBrain3_SetAllSligsDeadHack3,
    eBrain3_PlayGlukkonMovie4,
    eBrain3_WaitForMovieToFinish5,
    eBrain3_AutoDepossessGlukkon6
};

enum Brain_4_Death
{
    eBrain4_ToVaporizeDeath0,
    eBrain4_Vaporize1,
    eBrain4_ShakeGibsDeath2,
    eBrain4_DeathFinish3,
    eBrain4_ToVaporize4,
    eBrain4_ToVaporizeKilledBySlog5, // TODO: can probably be merged with eBrain4_ToVaporize4
    eBrain4_AbeDead6
};

enum Brain_5_WaitToSpawn
{
    eBrain5_WaitUntilSwitchActivated0,
    eBrain5_Spawn1,
    eBrain5_ToWalking2
};

void Glukkon::CreateFromSaveState(SerializedObjectData& pSaveBuffer)
{
    const auto pSaveState = pSaveBuffer.ReadTmpPtr<GlukkonSaveState>();
    auto pTlv = static_cast<relive::Path_Glukkon*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->mTlvId));

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

        pGlukkon->GetAnimation().Set_Animation_Data(pGlukkon->GetAnimRes(sGlukkonsAnimIdTable[static_cast<s32>(glukType)][static_cast<s32>(pSaveState->mCurrentMotion)]));

        pGlukkon->GetAnimation().SetCurrentFrame(pSaveState->mCurrentFrame);
        pGlukkon->GetAnimation().SetFrameChangeCounter(pSaveState->mFrameChangeCounter);
        pGlukkon->SetDrawable(pSaveState->mDrawable & 1);
        pGlukkon->GetAnimation().SetFlipX(pSaveState->mFlipX & 1);
        pGlukkon->GetAnimation().SetRender(pSaveState->mRender & 1);

        if (IsLastFrame(&pGlukkon->GetAnimation()))
        {
            pGlukkon->GetAnimation().SetIsLastFrame(true);
        }

        pGlukkon->mHealth = pSaveState->mHealth;
        pGlukkon->mCurrentMotion = pSaveState->mCurrentMotion2;
        pGlukkon->mNextMotion = pSaveState->mNextMotion;
        pGlukkon->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pSaveState->field_38_last_line_ypos);
        pGlukkon->SetRestoredFromQuickSave(true);
        pGlukkon->field_1D4_timer = pSaveState->field_54_timer;
        pGlukkon->BaseAliveGameObjectCollisionLineType = pSaveState->mLineType;
        pGlukkon->mTlvId = pSaveState->mTlvId;
        pGlukkon->SetBrain(sGlukkonBrainTable[pSaveState->mBrainStateIdx]);
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
        pGlukkon->SetCanBePossessed(pSaveState->mCanBePossessed);
    }
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
    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eFeeCoDepot:
        case EReliveLevelIds::eFeeCoDepot_Ender:
            for (s32 i = 0; i < 25; i++)
            {
                const auto& anim = Glukkon::sGlukkonsAnimIdTable[1][i];
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(anim));
            }
            break;

        case EReliveLevelIds::eBarracks:
        case EReliveLevelIds::eBarracks_Ender:
            for (s32 i = 0; i < 25; i++)
            {
                const auto& anim = Glukkon::sGlukkonsAnimIdTable[2][i];
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(anim));
            }
            break;

        case EReliveLevelIds::eBonewerkz:
        case EReliveLevelIds::eBonewerkz_Ender:
            for (s32 i = 0; i < 25; i++)
            {
                const auto& anim = Glukkon::sGlukkonsAnimIdTable[3][i];
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(anim));
            }
            break;

        default:
            for (s32 i = 0; i < 25; i++)
            {
                const auto& anim = Glukkon::sGlukkonsAnimIdTable[0][i];
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(anim));
            }
            break;
    }
}

Glukkon::Glukkon(relive::Path_Glukkon* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    LoadAnimations();

    mTlvData = *pTlv;

    SetCanSetOffExplosives(true);

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

void Glukkon::VRender(OrderingTable& ot)
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

void Glukkon::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return;
    }

    GlukkonSaveState data = {};

    data.mType = ReliveTypes::eGlukkon;
    data.field_4_object_id = mBaseGameObjectTlvInfo;
    data.mXPos = mXPos;
    data.mYPos = mYPos;
    data.mVelX = mVelX;
    data.mVelY = mVelY;
    data.mCurrentPath = mCurrentPath;
    data.mCurrentLevel = mCurrentLevel;
    data.mSpriteScale = GetSpriteScale();
    data.mRed = mRGB.r;
    data.mGreen = mRGB.g;
    data.mBlue = mRGB.b;
    data.mCurrentMotion = mCurrentMotion;
    data.mCurrentFrame = GetAnimation().GetCurrentFrame();
    data.mFrameChangeCounter = static_cast<u16>(GetAnimation().GetFrameChangeCounter());
    data.mFlipX = GetAnimation().GetFlipX();
    data.mRender = GetAnimation().GetRender();
    data.mDrawable = GetDrawable();
    data.mHealth = mHealth;
    data.mCurrentMotion2 = mCurrentMotion;
    data.mNextMotion = mNextMotion;
    data.field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    if (BaseAliveGameObjectCollisionLine)
    {
        data.mLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        data.mLineType = eLineTypes::eNone_m1;
    }
    data.mIsActiveChar = this == sControlledCharacter;
    data.mTlvId = mTlvId;

    data.mBrainStateIdx = 0;

    s16 idx = 0;
    for (auto& fn : sGlukkonBrainTable)
    {
        if (BrainIs(fn))
        {
            data.mBrainStateIdx = idx;
            break;
        }
        idx++;
    }

    data.mBrainSubState = mBrainSubState;
    data.field_54_timer = field_1D4_timer;
    data.mFallingVelXScaleFactor = mFallingVelXScaleFactor;
    data.mPreventDepossession = mPreventDepossession;
    data.mAbeLevel = mAbeLevel;
    data.mAbePath = mAbePath;
    data.mAbeCamera = mAbeCamera;
    data.mSpeak = mSpeak;
    data.mGamespeakPitch = mGamespeakPitch;
    data.mPreviousYPos = mPreviousYPos;
    data.mRandomishSpeakTimer = mRandomishSpeakTimer;
    data.mTurnOrHelpTimer = mTurnOrHelpTimer;
    data.mPanicTimer = mPanicTimer;
    data.field_7C = field_1FC;
    data.mKnockbackDelayAfterGettingShotTimer = mKnockbackDelayAfterGettingShotTimer;
    data.mGettingShotTimer = mGettingShotTimer;
    data.mFadeId = mFadeId;
    data.mCanBePossessed = GetCanBePossessed();
    data.mCurrentType = Type();

    pSaveBuffer.Write(data);
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
                    if (mNextMotion == eGlukkonMotions::Motion_4_Jump)
                    {
                        SetAnim(eGlukkonMotions::Motion_18_WalkToJump);
                    }
                }
                else if (Input().IsAnyHeld(InputCommands::eRun))
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
                    if (mNextMotion == eGlukkonMotions::Motion_0_Idle || mNextMotion == eGlukkonMotions::Motion_2_Turn || mNextMotion == eGlukkonMotions::Motion_11_Speak1 || mNextMotion == eGlukkonMotions::Motion_12_Speak2 || mNextMotion == eGlukkonMotions::Motion_23_Speak3 || mNextMotion == eGlukkonMotions::Motion_13_LongLaugh)
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
                    if ((mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)) || !(Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight)))
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
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
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
        if (GetAnimation().GetIsLastFrame())
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
    if (GetAnimation().GetFlipX())
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
        if (GetAnimation().GetFlipX())
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
        const auto held = Input().GetHeld();

        if ((mVelX > FP_FromInteger(0) && (held & InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && (held & InputCommands::eRight)))
        {
            // Direction changed
            SetAnim(eGlukkonMotions::Motion_17_JumpToStand, true);
        }
        else if (held & (InputCommands::eLeft | InputCommands::eRight))
        {
            if (!(held & InputCommands::eRun))
            {
                // Still moving but not running, so start walking
                SetAnim(eGlukkonMotions::Motion_19_JumpToWalk, true);
            }
        }
        else if (!(held & InputCommands::eHop))
        {
            // Not changing direction or trying to move at all, back to standing
            SetAnim(eGlukkonMotions::Motion_17_JumpToStand, true);
        }
    }
    else
    {
        if (mNextMotion != eGlukkonMotions::Motion_1_Walk)
        {
            if (mNextMotion == eGlukkonMotions::Motion_0_Idle || mNextMotion == eGlukkonMotions::Motion_2_Turn || mNextMotion == eGlukkonMotions::Motion_11_Speak1 || mNextMotion == eGlukkonMotions::Motion_12_Speak2 || mNextMotion == eGlukkonMotions::Motion_23_Speak3 || mNextMotion == eGlukkonMotions::Motion_13_LongLaugh)
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

    if (GetAnimation().GetIsLastFrame())
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
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = hitY;
                mXPos = hitX;
                mVelY = FP_FromInteger(0);

                GetOnPlatforms();

                if (hitY - BaseAliveGameObjectLastLineYPos > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(7)))
                {
                    SetAnim(eGlukkonMotions::Motion_8_DeathFall, true);
                    SetBrain(&Glukkon::Brain_4_Death);
                    mBrainSubState = Brain_4_Death::eBrain4_ToVaporizeDeath0;
                }
                else if (mCurrentMotion != eGlukkonMotions::Motion_3_KnockBack)
                {
                    SetAnim(eGlukkonMotions::Motion_9_Land, true);
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
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

    if (GetAnimation().GetIsLastFrame())
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
                EventBroadcast(Event::kEventGlukkonUnknown, this);
                field_1FC = 0;
            }

            if (BrainIs(&Glukkon::Brain_3_PlayerControlled))
            {
                GameSpeakEvents evToBePushed;
                switch (mSpeak)
                {
                    case GlukkonSpeak::Hey_0:
                        evToBePushed = GameSpeakEvents::eGlukkon_Hey;
                        break;
                    case GlukkonSpeak::DoIt_1:
                        evToBePushed = GameSpeakEvents::eGlukkon_DoIt;
                        break;
                    case GlukkonSpeak::StayHere_2:
                        evToBePushed = GameSpeakEvents::eGlukkon_StayHere;
                        break;
                    case GlukkonSpeak::Commere_3:
                        evToBePushed = GameSpeakEvents::eGlukkon_Commere;
                        break;
                    case GlukkonSpeak::AllOYa_4:
                        evToBePushed = GameSpeakEvents::eGlukkon_AllOYa;
                        break;
                    case GlukkonSpeak::Heh_5:
                        evToBePushed = GameSpeakEvents::eGlukkon_Heh;
                        break;
                    case GlukkonSpeak::Help_6:
                        evToBePushed = GameSpeakEvents::eGlukkon_Help;
                        break;
                    case GlukkonSpeak::Laugh_7:
                        evToBePushed = GameSpeakEvents::eGlukkon_Laugh;
                        break;
                    case GlukkonSpeak::KillEm_8:
                        evToBePushed = GameSpeakEvents::eGlukkon_KillEm;
                        break;
                    case GlukkonSpeak::Unused_9:
                        evToBePushed = GameSpeakEvents::eGlukkon_Unknown;
                        break;
                    case GlukkonSpeak::What_11:
                        evToBePushed = GameSpeakEvents::eGlukkon_What;
                        break;
                    default:
                        evToBePushed = GameSpeakEvents::eGlukkon_None;
                        break;
                }
                if (evToBePushed != GameSpeakEvents::eGlukkon_None)
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

    if (GetAnimation().GetIsLastFrame())
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
    if (GetAnimation().GetIsLastFrame())
    {
        Glukkon::HandleInput();
    }
}

void Glukkon::Motion_18_WalkToJump()
{
    DoMovement();
    if (GetAnimation().GetIsLastFrame())
    {
        SetAnim(eGlukkonMotions::Motion_4_Jump);
    }
}

void Glukkon::Motion_19_JumpToWalk()
{
    DoMovement();
    if (GetAnimation().GetIsLastFrame())
    {
        SetAnim(eGlukkonMotions::Motion_1_Walk);
    }
}

void Glukkon::Motion_20_KnockBackStandBegin()
{
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        PlaySound_GameSpeak(GlukkonSpeak::Heh_5, 0, 0, 0);
        SetAnim(eGlukkonMotions::Motion_22_KnockBackStandEnd, true);
    }
}

void Glukkon::Motion_21_GetShot()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_7_Fall();
        if (mCurrentMotion != eGlukkonMotions::Motion_21_GetShot)
        {
            SetAnim(eGlukkonMotions::Motion_21_GetShot, true);
        }
    }

    if (static_cast<s32>(sGnFrame) >= mGettingShotTimer)
    {
        if (GetAnimation().GetRender())
        {
            mBrainSubState = 2; // TODO: check for which brain this sub state is
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
        GetAnimation().SetFlipX(mVelX > FP_FromInteger(0));
        SetAnim(eGlukkonMotions::Motion_3_KnockBack, true);
    }
}

void Glukkon::Motion_22_KnockBackStandEnd()
{
    if (GetAnimation().GetIsLastFrame())
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
        if (!pLiftPoint->OnAnyFloor() && mBrainSubState != Brain_0_Calm_WalkAround::eBrain0_OnMovingLift7)
        {
            mNextMotion = eGlukkonMotions::Motion_0_Idle;
            return Brain_0_Calm_WalkAround::eBrain0_OnMovingLift7;
        }
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        return Brain_0_Calm_WalkAround::eBrain0_InAir8;
    }

    if (gAbe->mHealth < FP_FromInteger(0))
    {
        Speak(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death);
        return Brain_4_Death::eBrain4_AbeDead6;
    }

    BaseAnimatedWithPhysicsGameObject* pEvent17 = nullptr;

    switch (mBrainSubState)
    {
        case Brain_0_Calm_WalkAround::eBrain0_WalkAround0:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }

            if (ShouldPanic(false))
            {
                if (IsEventInRange(
                        Event::kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = MakeTimer(10);
                }
                else
                {
                    field_1D4_timer = sGnFrame + mTlvData.mScreamHelpDelay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic);
                return Brain_1_Panic::eBrain1_PanicStart0;
            }

            pEvent17 = IsEventInRange(
                Event::kEventGlukkonUnknown,
                mXPos,
                mYPos,
                AsEventScale(GetScale()));
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = MakeTimer(20);
                return Brain_0_Calm_WalkAround::eBrain0_SayHeh9;
            }

            if (mTlvData.mBehavior == relive::Path_Glukkon::Behavior::eCheckForWalls)
            {
                if (Check_IsOnEndOfLine(GetAnimation().GetFlipX(), 1) || PathBlocked(mVelX, 1))
                {
                    mNextMotion = eGlukkonMotions::Motion_2_Turn;
                    return Brain_0_Calm_WalkAround::eBrain0_Unknown2;
                }
                mNextMotion = eGlukkonMotions::Motion_14_BeginWalk;
                return Brain_0_Calm_WalkAround::eBrain0_Unknown1;
            }
            else
            {
                mNextMotion = eGlukkonMotions::Motion_0_Idle;
                return Brain_0_Calm_WalkAround::eBrain0_Unknown1;
            }
            break;

        case Brain_0_Calm_WalkAround::eBrain0_Unknown1:
            if (ShouldPanic(false))
            {
                if (IsEventInRange(
                        Event::kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = MakeTimer(10);
                }
                else
                {
                    field_1D4_timer = sGnFrame + mTlvData.mScreamHelpDelay;
                }

                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic);
                return Brain_1_Panic::eBrain1_PanicStart0;
            }
            else
            {
                auto pEvent17_1 = IsEventInRange(
                    Event::kEventGlukkonUnknown,
                    mXPos,
                    mYPos,
                    AsEventScale(GetScale()));
                if (pEvent17_1 && pEvent17_1 != this)
                {
                    field_1D4_timer = MakeTimer(20);
                    return Brain_0_Calm_WalkAround::eBrain0_SayHeh9;
                }

                if (mTlvData.mBehavior == relive::Path_Glukkon::Behavior::eCheckForWalls)
                {
                    if (Check_IsOnEndOfLine(GetAnimation().GetFlipX(), 1) || PathBlocked(mVelX, 1))
                    {
                        if (static_cast<s32>(sGnFrame) <= mRandomishSpeakTimer)
                        {
                            mNextMotion = eGlukkonMotions::Motion_0_Idle;
                            field_1D4_timer = MakeTimer(Math_RandomRange(30, 120));
                            return Brain_0_Calm_WalkAround::eBrain0_Unknown4;
                        }
                        else
                        {
                            mRandomishSpeakTimer = MakeTimer(120);
                            SpeakRandomish();
                            return Brain_0_Calm_WalkAround::eBrain0_Unknown3;
                        }
                    }
                }
                else
                {
                    if (Math_NextRandom() < 5 && static_cast<s32>(sGnFrame) > mTurnOrHelpTimer)
                    {
                        mTurnOrHelpTimer = MakeTimer(120);
                        mNextMotion = eGlukkonMotions::Motion_2_Turn;
                        return Brain_0_Calm_WalkAround::eBrain0_Unknown2;
                    }
                }

                if (Math_NextRandom() >= 5 || static_cast<s32>(sGnFrame) <= mRandomishSpeakTimer)
                {
                    return mBrainSubState;
                }

                mRandomishSpeakTimer = MakeTimer(120);
                SpeakRandomish();
                return Brain_0_Calm_WalkAround::eBrain0_Unknown6;
            }
            break;

        case Brain_0_Calm_WalkAround::eBrain0_Unknown2:
        case Brain_0_Calm_WalkAround::eBrain0_InAir8:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return Brain_0_Calm_WalkAround::eBrain0_WalkAround0;

        case Brain_0_Calm_WalkAround::eBrain0_Unknown3:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle || mSpeak != GlukkonSpeak::None)
            {
                return mBrainSubState;
            }
            field_1D4_timer = MakeTimer(Math_RandomRange(30, 120));
            return Brain_0_Calm_WalkAround::eBrain0_Unknown4;

        case Brain_0_Calm_WalkAround::eBrain0_Unknown4:
            if (ShouldPanic(false))
            {
                if (IsEventInRange(
                        Event::kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = MakeTimer(10);
                }
                else
                {
                    field_1D4_timer = sGnFrame + mTlvData.mScreamHelpDelay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic);
                return Brain_1_Panic::eBrain1_PanicStart0;
            }

            pEvent17 = IsEventInRange(
                Event::kEventGlukkonUnknown,
                mXPos,
                mYPos,
                AsEventScale(GetScale()));
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = MakeTimer(20);
                return Brain_0_Calm_WalkAround::eBrain0_SayHeh9;
            }
            else
            {
                if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
                {
                    return mBrainSubState;
                }
                mNextMotion = eGlukkonMotions::Motion_2_Turn;
                return Brain_0_Calm_WalkAround::eBrain0_Unknown2;
            }
            break;

        case Brain_0_Calm_WalkAround::eBrain0_Unknown5:
            if (ShouldPanic(false))
            {
                if (IsEventInRange(
                        Event::kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = MakeTimer(10);
                }
                else
                {
                    field_1D4_timer = sGnFrame + mTlvData.mScreamHelpDelay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic);
                return Brain_1_Panic::eBrain1_PanicStart0;
            }
            else
            {
                auto pEvent17_3 = IsEventInRange(
                    Event::kEventGlukkonUnknown,
                    mXPos,
                    mYPos,
                    AsEventScale(GetScale()));
                if (pEvent17_3 && pEvent17_3 != this)
                {
                    field_1D4_timer = MakeTimer(20);
                    return Brain_0_Calm_WalkAround::eBrain0_SayHeh9;
                }
                else
                {
                    if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
                    {
                        return mBrainSubState;
                    }
                    return Brain_0_Calm_WalkAround::eBrain0_WalkAround0;
                }
            }
            break;

        case Brain_0_Calm_WalkAround::eBrain0_Unknown6:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            field_1D4_timer = MakeTimer(Math_RandomRange(30, 120));
            return Brain_0_Calm_WalkAround::eBrain0_Unknown5;

        case Brain_0_Calm_WalkAround::eBrain0_OnMovingLift7:
            if (pLiftPoint)
            {
                if (!pLiftPoint->OnAnyFloor())
                {
                    return mBrainSubState;
                }
                return Brain_0_Calm_WalkAround::eBrain0_WalkAround0;
            }
            else
            {
                BaseAliveGameObject_PlatformId = Guid{};
                return Brain_0_Calm_WalkAround::eBrain0_WalkAround0;
            }
            break;

        case Brain_0_Calm_WalkAround::eBrain0_SayHeh9:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
            {
                return mBrainSubState;
            }
            field_1FC = 1;
            Speak(GlukkonSpeak::Heh_5);
            return Brain_0_Calm_WalkAround::eBrain0_Unknown6;

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
    if (pLiftPoint && pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor() && mBrainSubState != Brain_1_Panic::eBrain1_OnMovingLift6)
    {
        mNextMotion = eGlukkonMotions::Motion_0_Idle;
        return Brain_1_Panic::eBrain1_OnMovingLift6;
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        return Brain_1_Panic::eBrain1_InAir7;
    }

    if (gAbe->mHealth < FP_FromInteger(0))
    {
        Glukkon::Speak(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death);
        return Brain_4_Death::eBrain4_AbeDead6;
    }

    switch (mBrainSubState)
    {
        case Brain_1_Panic::eBrain1_PanicStart0:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer || mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            mPanicTimer = sGnFrame;
            Speak(GlukkonSpeak::Help_6);
            return Brain_1_Panic::eBrain1_SetupSpeakTimer4;

        case Brain_1_Panic::eBrain1_PanicWalk1:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
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
                return Brain_0_Calm_WalkAround::eBrain0_WalkAround0;
            }

            if (mTlvData.mBehavior != relive::Path_Glukkon::Behavior::eIgnoreWalls)
            {
                if (Check_IsOnEndOfLine(GetAnimation().GetFlipX(), 1) || PathBlocked(mVelX, 1))
                {
                    mNextMotion = eGlukkonMotions::Motion_2_Turn;
                    return Brain_1_Panic::eBrain1_WaitUntilIdleMotion3;
                }
                mNextMotion = eGlukkonMotions::Motion_14_BeginWalk;
            }
            else
            {
                mNextMotion = eGlukkonMotions::Motion_0_Idle;
            }
            return Brain_1_Panic::eBrain1_ScreamForHelp2;

        case Brain_1_Panic::eBrain1_ScreamForHelp2:
            if (mTlvData.mBehavior == relive::Path_Glukkon::Behavior::eCheckForWalls)
            {
                if (Check_IsOnEndOfLine(GetAnimation().GetFlipX(), 1) || PathBlocked(mVelX, 1))
                {
                    Glukkon::Speak(GlukkonSpeak::Help_6);
                    return Brain_1_Panic::eBrain1_TurnAround5;
                }
            }
            else
            {
                if (Math_NextRandom() < 5u && static_cast<s32>(sGnFrame) > mTurnOrHelpTimer)
                {
                    mTurnOrHelpTimer = MakeTimer(120);
                    Glukkon::Speak(GlukkonSpeak::Help_6);
                    return Brain_1_Panic::eBrain1_TurnAround5;
                }
            }
            if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= mRandomishSpeakTimer)
            {
                return mBrainSubState;
            }
            Glukkon::Speak(GlukkonSpeak::Help_6);
            return Brain_1_Panic::eBrain1_SetupSpeakTimer4;

        case Brain_1_Panic::eBrain1_WaitUntilIdleMotion3:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return Brain_1_Panic::eBrain1_PanicWalk1;

        case Brain_1_Panic::eBrain1_SetupSpeakTimer4:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            mRandomishSpeakTimer = MakeTimer(60);
            return Brain_1_Panic::eBrain1_PanicWalk1;

        case Brain_1_Panic::eBrain1_TurnAround5:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle || mSpeak != GlukkonSpeak::None)
            {
                return mBrainSubState;
            }
            mNextMotion = eGlukkonMotions::Motion_2_Turn;
            return Brain_1_Panic::eBrain1_WaitUntilIdleMotion3;

        case Brain_1_Panic::eBrain1_OnMovingLift6:
            if (pLiftPoint)
            {
                if (!pLiftPoint->OnAnyFloor())
                {
                    return mBrainSubState;
                }
            }
            else
            {
                BaseAliveGameObject_PlatformId = Guid{};
            }
            return Brain_1_Panic::eBrain1_PanicWalk1;

        case Brain_1_Panic::eBrain1_InAir7:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return Brain_1_Panic::eBrain1_PanicWalk1;

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

    if (mCurrentMotion == eGlukkonMotions::Motion_7_Fall)
    {
        return Brain_2_Slapped::eBrain2_Unknown3;
    }

    if (gAbe->mHealth < FP_FromInteger(0))
    {
        Glukkon::Speak(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death);
        return Brain_4_Death::eBrain4_AbeDead6;
    }

    switch (mBrainSubState)
    {
        case Brain_2_Slapped::eBrain2_Unknown0:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
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
                    if (!(GetAnimation().GetFlipX()))
                    {
                        return Brain_0_Calm_WalkAround::eBrain0_Unknown2;
                    }
                    if (FP_FromInteger(mTlvData.mTopLeftX) >= mXPos)
                    {
                        return Brain_0_Calm_WalkAround::eBrain0_WalkAround0;
                    }
                }

                if (!(GetAnimation().GetFlipX()))
                {
                    return Brain_0_Calm_WalkAround::eBrain0_WalkAround0;
                }
                return Brain_0_Calm_WalkAround::eBrain0_Unknown2;
            }

            if ((BaseAliveGameObjectCollisionLine && Check_IsOnEndOfLine(GetAnimation().GetFlipX(), 4)) || PathBlocked(mVelX, 0))
            {
                mNextMotion = eGlukkonMotions::Motion_2_Turn;
                return Brain_2_Slapped::eBrain2_Unknown2;
            }
            else
            {
                mNextMotion = eGlukkonMotions::Motion_4_Jump;
                return Brain_2_Slapped::eBrain2_Unknown1;
            }
            break;

        case Brain_2_Slapped::eBrain2_Unknown1:
            if (ShouldPanic(true))
            {
                mPanicTimer = sGnFrame;
            }
            else if (static_cast<s32>(sGnFrame - mPanicTimer) > mTlvData.mToCalmDelay)
            {
                mNextMotion = eGlukkonMotions::Motion_0_Idle;
                return Brain_2_Slapped::eBrain2_Unknown0;
            }

            if (!BaseAliveGameObjectCollisionLine || (!Check_IsOnEndOfLine(GetAnimation().GetFlipX(), 4) && !PathBlocked(mVelX, 0)))
            {
                if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= mRandomishSpeakTimer)
                {
                    return mBrainSubState;
                }
                Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Help_6, 0, 0, 0);
                mRandomishSpeakTimer = MakeTimer(40);
                return mBrainSubState;
            }

            mNextMotion = eGlukkonMotions::Motion_2_Turn;
            return Brain_2_Slapped::eBrain2_Unknown2;

        case Brain_2_Slapped::eBrain2_Unknown2:
        case Brain_2_Slapped::eBrain2_Unknown3:
            if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return Brain_2_Slapped::eBrain2_Unknown0;

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
        case Brain_3_PlayerControlled::eBrain3_ToStand0:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
            {
                return mBrainSubState;
            }
            ToStand();
            return Brain_3_PlayerControlled::eBrain3_Possessed1;

        case Brain_3_PlayerControlled::eBrain3_Possessed1:
        {
            if ((mTlvData.mGlukkonType != relive::Path_Glukkon::GlukkonTypes::eStoryAslik &&
                mTlvData.mGlukkonType != relive::Path_Glukkon::GlukkonTypes::eStoryDripik &&
                mTlvData.mGlukkonType != relive::Path_Glukkon::GlukkonTypes::eStoryPhleg) ||
                !SwitchStates_Get(mTlvData.mPlayMovieSwitchId))
            {
                if (Input_IsChanting() && mCurrentMotion != eGlukkonMotions::Motion_4_Jump && !mPreventDepossession)
                {
                    field_1D4_timer = MakeTimer(30);
                    SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
                    SetAnim(eGlukkonMotions::Motion_10_ChantShake, true);
                    return Brain_3_PlayerControlled::eBrain3_TryDepossess2;
                }
                return mBrainSubState;
            }

            if (mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryAslik)
            {
                gVisitedFeecoEnder = true;
            }
            else if (mTlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryDripik)
            {
                gVisitedBarracks = true;
            }
            else
            {
                gVisitedBonewerkz = true;
            }

            auto pFadeMem = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeIn, 0, 8, relive::TBlendModes::eBlend_2);
            if (pFadeMem)
            {
                mFadeId = pFadeMem->mBaseGameObjectId;
            }
        }
        return Brain_3_PlayerControlled::eBrain3_SetAllSligsDeadHack3;

        case Brain_3_PlayerControlled::eBrain3_TryDepossess2:
            if (Input_IsChanting()
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

                if (static_cast<s32>(sGnFrame) > field_1D4_timer || gAbe->mHealth <= FP_FromInteger(0))
                {
                    SetPossessed(false);
                    SetBrain(&Glukkon::Brain_4_Death);
                    mBrainSubState = Brain_4_Death::eBrain4_ShakeGibsDeath2;
                    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
                }
                return mBrainSubState;
            }
            ToStand();
            return Brain_3_PlayerControlled::eBrain3_Possessed1;

        case Brain_3_PlayerControlled::eBrain3_SetAllSligsDeadHack3:
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
                    pObj->SetDead(true);
                }
            }

            SetUpdateDelay(2);
            return Brain_3_PlayerControlled::eBrain3_PlayGlukkonMovie4;

        case Brain_3_PlayerControlled::eBrain3_PlayGlukkonMovie4:
        {
            gScreenManager->DisableRendering();

            const FmvInfo* pFmvRec = Path_Get_FMV_Record(gMap.mCurrentLevel, mTlvData.mMovieId);
            relive_new Movie(pFmvRec->field_0_pName);
        }
        return Brain_3_PlayerControlled::eBrain3_WaitForMovieToFinish5;

        case Brain_3_PlayerControlled::eBrain3_WaitForMovieToFinish5:
            if (gMovieRefCount)
            {
                return mBrainSubState;
            }
            gPsxDisplay.PutCurrentDispEnv();
            gScreenManager->DecompressCameraToVRam(gMap.field_2C_camera_array[0]->mCamRes);
            if (pFade)
            {
                pFade->Init(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 1, 8);
            }
            return Brain_3_PlayerControlled::eBrain3_AutoDepossessGlukkon6;

        case Brain_3_PlayerControlled::eBrain3_AutoDepossessGlukkon6:
            GetSoundAPI().mSND_Restart();
            gScreenManager->EnableRendering();
            field_1D4_timer = MakeTimer(30);
            SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
            SetAnim(eGlukkonMotions::Motion_10_ChantShake, true);
            return Brain_3_PlayerControlled::eBrain3_TryDepossess2;

        default:
            return mBrainSubState;
    }
}

static GibType AsGibType(relive::Path_Glukkon::GlukkonTypes glukkonType)
{
    switch (glukkonType)
    {
        case relive::Path_Glukkon::GlukkonTypes::eNormal:
            return GibType::eGlukkon;

        case relive::Path_Glukkon::GlukkonTypes::eStoryAslik:
            return GibType::eAslik;

        case relive::Path_Glukkon::GlukkonTypes::eStoryDripik:
            return GibType::eDripik;

        case relive::Path_Glukkon::GlukkonTypes::eStoryPhleg:
            return GibType::ePhleg;
        default:
            return GibType::eGlukkon;
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
        case Brain_4_Death::eBrain4_ToVaporizeDeath0:
            if (mCurrentMotion != eGlukkonMotions::Motion_8_DeathFall || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            else
            {
                mHealth = FP_FromInteger(0);
                field_1D4_timer = MakeTimer(90);
                return Brain_4_Death::eBrain4_Vaporize1;
            }
            break;

        case Brain_4_Death::eBrain4_Vaporize1:
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

        case Brain_4_Death::eBrain4_ShakeGibsDeath2:
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

            GetAnimation().SetAnimate(false);
            GetAnimation().SetRender(false);

            SetAnim(eGlukkonMotions::Motion_10_ChantShake, true);

            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            field_1D4_timer = MakeTimer(40);
        }
            return Brain_4_Death::eBrain4_DeathFinish3;

        case Brain_4_Death::eBrain4_DeathFinish3:
            if (static_cast<s32>(sGnFrame) > field_1D4_timer)
            {
                ToDead();
            }
            return mBrainSubState;

        case Brain_4_Death::eBrain4_ToVaporize4:
        case Brain_4_Death::eBrain4_ToVaporizeKilledBySlog5:
            if (!BaseAliveGameObjectCollisionLine || mCurrentMotion != eGlukkonMotions::Motion_3_KnockBack || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            else
            {
                field_1D4_timer = MakeTimer(90);
                return Brain_4_Death::eBrain4_Vaporize1;
            }
            break;

        default:
            return mBrainSubState;
    }
}

static const PSX_Point sSpawnSparkOffsets[8] = {
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

    if (mBrainSubState == Brain_5_WaitToSpawn::eBrain5_WaitUntilSwitchActivated0)
    {
        if (!SwitchStates_Get(mTlvData.mSpawnSwitchId))
        {
            return mBrainSubState;
        }
        field_1D4_timer = sGnFrame + mTlvData.mSpawnDelay;
        return Brain_5_WaitToSpawn::eBrain5_Spawn1;
    }
    else if (mBrainSubState == Brain_5_WaitToSpawn::eBrain5_ToWalking2)
    {
        if (mCurrentMotion != eGlukkonMotions::Motion_0_Idle || mSpeak != GlukkonSpeak::None)
        {
            return mBrainSubState;
        }
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround);
        mBrainSubState = Brain_0_Calm_WalkAround::eBrain0_WalkAround0;
        return mBrainSubState;
    }
    else if (mBrainSubState == Brain_5_WaitToSpawn::eBrain5_Spawn1)
    {
        if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
        {
            return mBrainSubState;
        }

        SetDrawable(true);
        SetCanBePossessed(true);

        SetType(ReliveTypes::eGlukkon);

        if (mTlvData.mSpawnType == relive::Path_Glukkon::SpawnType::eFullSpawnEffects)
        {
            SFX_Play_Pitch(relive::SoundEffects::Zap1, 60, -300);

            for (const auto& p : sSpawnSparkOffsets)
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
                BurstType::eBigRedSparks,
                9, true);

            Speak(GlukkonSpeak::Heh_5);
            return Brain_5_WaitToSpawn::eBrain5_ToWalking2;
        }
        else
        {
            SfxPlayMono(relive::SoundEffects::GlukkonSpawn, 0);
            Speak(GlukkonSpeak::Heh_5);
            return Brain_5_WaitToSpawn::eBrain5_ToWalking2;
        }
    }
    return mBrainSubState;
}

void Glukkon::Init()
{
    GetAnimation().SetAnimate(true);
    GetAnimation().SetRender(true);

    SetDrawable(true);

    SetTint(&kGlukkonTints[0], gMap.mCurrentLevel);
    mXPos = FP_FromInteger((mTlvData.mTopLeftX + mTlvData.mBottomRightX) / 2);
    mYPos = FP_FromInteger(mTlvData.mTopLeftY);

    if (mTlvData.mFacing == relive::Path_Glukkon::Facing::eLeft)
    {
        GetAnimation().SetFlipX(true);
    }

    if (mTlvData.mSpawnSwitchId)
    {
        if (mTlvData.mSpawnType == relive::Path_Glukkon::SpawnType::eFacingLeft)
        {
            mXPos -= ScaleToGridSize(GetSpriteScale());
            GetAnimation().SetFlipX(false);
        }
        else if (mTlvData.mSpawnType == relive::Path_Glukkon::SpawnType::eFacingRight)
        {
            mXPos += ScaleToGridSize(GetSpriteScale());
            GetAnimation().SetFlipX(true);
        }
        SetCanBePossessed(false);
        SetDrawable(false);
        SetBrain(&Glukkon::Brain_5_WaitToSpawn);
        mBrainSubState = Brain_5_WaitToSpawn::eBrain5_WaitUntilSwitchActivated0;
        SetType(ReliveTypes::eNone);
    }
    else
    {
        SetCanBePossessed(true);
        SetType(ReliveTypes::eGlukkon);
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround);
        mBrainSubState = Brain_0_Calm_WalkAround::eBrain0_WalkAround0;
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
    if (gCollisions->Raycast(
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
        Path::TLV_Delete(mTlvId);
    }
    else
    {
        Path::TLV_Reset(mTlvId);
    }

    SetDrawable(true); // Seems wrong to do this here ??

    if (this == sControlledCharacter)
    {
        sControlledCharacter = gAbe;
    }
}

void Glukkon::VUpdate()
{
    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            gCollisions->Raycast(
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

    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }
    else
    {
        if (!Input_IsChanting())
        {
            mPreventDepossession = false;
        }

        const auto oldMotion = mCurrentMotion;

        mBrainSubState = (this->*mBrainState)();

        const FP oldXPos = mXPos;
        const FP oldYPos = mYPos;

        InvokeMemberFunction(this, sGlukkonMotionTable, mCurrentMotion);

        // TODO: This is extra debug logging to figure out the motion names
        if (oldMotion != mCurrentMotion)
        {
            //LOG_INFO("Glukkon: Old motion = " << sGlukkonMotionNames[oldMotion] << " new motion = " << sGlukkonMotionNames[mCurrentMotion]);
        }

        if (oldXPos != mXPos || oldYPos != mYPos)
        {
            relive::Path_TLV* pTlv = gPathInfo->TLV_Get_At(
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
    SetPossessed(true);
    mPreventDepossession = true;
    SetAnim(eGlukkonMotions::Motion_10_ChantShake, true);
    SetBrain(&Glukkon::Brain_3_PlayerControlled);
    mBrainSubState = Brain_3_PlayerControlled::eBrain3_ToStand0;
    field_1D4_timer = MakeTimer(35);
    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;
}

void Glukkon::Update_Slurg_WatchPoints()
{
    if (mCurrentMotion == eGlukkonMotions::Motion_1_Walk || (mCurrentMotion == eGlukkonMotions::Motion_4_Jump && GetAnimation().GetCurrentFrame() > 8))
    {
        if (sGnFrame & 1)
        {
            const s8 count = gSlurgStepWatchPointsCount[gSlurgStepWatchPointsIdx];
            if (count < 5)
            {
                Slurg_Step_Watch_Points* pPoints = &gSlurgStepWatchPoints[gSlurgStepWatchPointsIdx];
                pPoints->mPoints[count].x = FP_GetExponent(mXPos);
                pPoints->mPoints[count].y = BaseAliveGameObjectCollisionLine->mRect.y - 5;
                gSlurgStepWatchPointsCount[gSlurgStepWatchPointsIdx] = count + 1;
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

    mCurrentMotion = currentMotion;
    if (bClearNextMotion)
    {
        mNextMotion = eGlukkonMotions::eNone_m1;
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
            mNextMotion = eGlukkonMotions::Motion_11_Speak1;
            mSpeak = speak;
            break;

        case GlukkonSpeak::StayHere_2:
            mNextMotion = eGlukkonMotions::Motion_23_Speak3;
            mSpeak = speak;
            break;

        case GlukkonSpeak::Commere_3:
        case GlukkonSpeak::AllOYa_4:
        case GlukkonSpeak::KillEm_8:
        case GlukkonSpeak::Unused_9:
            mNextMotion = eGlukkonMotions::Motion_12_Speak2;
            mSpeak = speak;
            break;

        case GlukkonSpeak::Laugh_7:
            mNextMotion = eGlukkonMotions::Motion_13_LongLaugh;
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

    if (BrainIs(&Glukkon::Brain_3_PlayerControlled) && mBrainSubState == 1 && !(GetTeleporting()))
    {
        const auto inputPressed = Input().GetPressed();
        const auto matchButtons = InputCommands::eGameSpeak1 | InputCommands::eGameSpeak2 | InputCommands::eGameSpeak3 | InputCommands::eGameSpeak4 | InputCommands::eGameSpeak5 | InputCommands::eGameSpeak6 | InputCommands::eGameSpeak7 | InputCommands::eGameSpeak8 | InputCommands::eChant;

        if (inputPressed & matchButtons)
        {
            mSpeak = GlukkonSpeak::None;

            if (inputPressed & InputCommands::eGameSpeak1)
            {
                mSpeak = GlukkonSpeak::Hey_0;
            }
            else if (inputPressed & InputCommands::eGameSpeak2)
            {
                mSpeak = GlukkonSpeak::Commere_3;
            }
            else if (inputPressed & InputCommands::eGameSpeak3)
            {
                mSpeak = GlukkonSpeak::StayHere_2;
            }
            else if (inputPressed & InputCommands::eGameSpeak4)
            {
                mSpeak = GlukkonSpeak::DoIt_1;
            }
            else if (inputPressed & InputCommands::eGameSpeak5)
            {
                mSpeak = GlukkonSpeak::KillEm_8;
            }
            else if (inputPressed & InputCommands::eGameSpeak7)
            {
                mSpeak = GlukkonSpeak::Help_6;
            }
            else if (inputPressed & InputCommands::eGameSpeak8)
            {
                mSpeak = GlukkonSpeak::Laugh_7;
            }
            else if (inputPressed & InputCommands::eGameSpeak6)
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
            const auto held = Input().GetHeld();
            if (held & InputCommands::eRight)
            {
                if (GetAnimation().GetFlipX())
                {
                    mNextMotion = eGlukkonMotions::Motion_2_Turn;
                }
                else
                {
                    mNextMotion = eGlukkonMotions::Motion_14_BeginWalk;
                }
            }
            else if (held & InputCommands::eLeft)
            {
                if (GetAnimation().GetFlipX())
                {
                    mNextMotion = eGlukkonMotions::Motion_14_BeginWalk;
                }
                else
                {
                    mNextMotion = eGlukkonMotions::Motion_2_Turn;
                }
            }

            if (held & InputCommands::eRun)
            {
                if (mNextMotion == eGlukkonMotions::Motion_1_Walk)
                {
                    mNextMotion = eGlukkonMotions::Motion_4_Jump;
                }
            }

            if (held & InputCommands::eHop)
            {
                mNextMotion = eGlukkonMotions::Motion_4_Jump;
            }
        }
    }

    switch (mNextMotion)
    {
    case eGlukkonMotions::eNone_m1:
            if (mCurrentMotion == eGlukkonMotions::Motion_0_Idle)
            {
                return;
            }
            mNextMotion = eGlukkonMotions::Motion_0_Idle;
            SetAnim(mNextMotion, true);
            break;

        case eGlukkonMotions::Motion_0_Idle:
        case eGlukkonMotions::Motion_2_Turn:
        case eGlukkonMotions::Motion_11_Speak1:
        case eGlukkonMotions::Motion_12_Speak2:
        case eGlukkonMotions::Motion_13_LongLaugh:
        case eGlukkonMotions::Motion_23_Speak3:
            SetAnim(mNextMotion, true);
            break;

        case eGlukkonMotions::Motion_1_Walk:
        case eGlukkonMotions::Motion_14_BeginWalk:
            if (mCurrentMotion != eGlukkonMotions::Motion_1_Walk)
            {
                mCurrentWalkPitch = 0;
            }
            [[fallthrough]];

        case eGlukkonMotions::Motion_4_Jump:
        case eGlukkonMotions::Motion_16_StandToJump:
        {
            FP xOff = {};
            if (GetAnimation().GetFlipX())
            {
                xOff = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                xOff = ScaleToGridSize(GetSpriteScale());
            }
            if (!WallHit(GetSpriteScale() * FP_FromInteger(50), xOff))
            {
                SetAnim(mNextMotion, true);
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
        && !(sControlledCharacter->GetInvisible())
        && !BaseAliveGameObject::IsInInvisibleZone(sControlledCharacter)
        && !EventGet(Event::kEventResetting)
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
            Event::kEventAbeOhm,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        return 1;
    }

    // Panic if an alarm is on
    if (EventGet(Event::kEventAlarm))
    {
        return 1;
    }

    // Panic if player is speaking (I guess it can be rather alarming?)
    auto pSpeakEvent = IsEventInRange(
        Event::kEventSpeaking,
        mXPos,
        mYPos,
        AsEventScale(GetScale()));
    return pSpeakEvent && pSpeakEvent == sControlledCharacter;
}

s16 Glukkon::PathBlocked(FP /*a2*/, s16 checkBounds)
{
    FP gridSize = ScaleToGridSize(GetSpriteScale());
    if (mCurrentMotion == eGlukkonMotions::Motion_4_Jump)
    {
        gridSize = (gridSize * FP_FromInteger(4));
    }

    relive::Path_EnemyStopper::StopDirection direction = relive::Path_EnemyStopper::StopDirection::Left;
    ReliveTypes boundType = ReliveTypes::eScrabBoundLeft;
    if (GetAnimation().GetFlipX())
    {
        boundType = ReliveTypes::eScrabBoundLeft;
        direction = relive::Path_EnemyStopper::StopDirection::Left;
        gridSize = -gridSize;
    }
    else
    {
        boundType = ReliveTypes::eScrabBoundRight;
        direction = relive::Path_EnemyStopper::StopDirection::Right;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), gridSize * FP_FromInteger(1)))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos), // TODO Abs() ??
        FP_GetExponent(mXPos + gridSize),
        FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
        ReliveTypes::eSlamDoor);

    auto pSlamDoorTlv = static_cast<relive::Path_SlamDoor*>(BaseAliveGameObjectPathTLV);

    if (pSlamDoorTlv && ((pSlamDoorTlv->mStartClosed && !SwitchStates_Get(pSlamDoorTlv->mSwitchId)) || (!pSlamDoorTlv->mStartClosed && SwitchStates_Get(pSlamDoorTlv->mSwitchId))))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
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

    if (gPathInfo->VTLV_Get_At_Of_Type(
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
    const FP* pTable = motion_velx_table_5547C4[static_cast<u32>(mCurrentMotion)];
    if (pTable)
    {
        if (GetAnimation().GetFlipX())
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
        if (GetAnimation().GetFlipX())
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

            if (mCurrentMotion == eGlukkonMotions::Motion_1_Walk || mCurrentMotion == eGlukkonMotions::Motion_14_BeginWalk || mCurrentMotion == eGlukkonMotions::Motion_15_EndWalk || mCurrentMotion == eGlukkonMotions::Motion_18_WalkToJump)
            {
                SetAnim(eGlukkonMotions::Motion_6_WalkToFall, true);
            }
            else if (mCurrentMotion == eGlukkonMotions::Motion_4_Jump || mCurrentMotion == eGlukkonMotions::Motion_16_StandToJump || mCurrentMotion == eGlukkonMotions::Motion_17_JumpToStand || mCurrentMotion == eGlukkonMotions::Motion_19_JumpToWalk)
            {
                SetAnim(eGlukkonMotions::Motion_5_JumpToFall, true);
            }
            else
            {
                SetAnim(eGlukkonMotions::Motion_7_Fall, true);
            }

            mXPos = prevXPos + mVelX;
            mFallingVelXScaleFactor = FP_FromInteger(1);

            if (mCurrentMotion == eGlukkonMotions::Motion_3_KnockBack)
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
    s32 defaultSndIdxVol = stepSfx_554840[sndIdx].mDefaultVolume;

    s16 pitch;
    if (sndIdx || !pGlukkon)
    {
        pitch = Math_RandomRange(stepSfx_554840[sndIdx].mPitchMin, stepSfx_554840[sndIdx].mPitchMax);
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
        sControlledCharacter = gAbe;
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
        SetDead(true);
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
                EventBroadcast(Event::kEventMudokonComfort, this);
            }
        }

        pTlv = gPathInfo->TLV_Get_At(
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
        calcedVolume = gameSpeak_554858[sndIdxShort].mDefaultVolume;
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

    return gCollisions->Raycast(
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
        SetDead(true);
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

bool Glukkon::VTakeDamage(BaseGameObject* pFrom)
{
    if (BrainIs(&Glukkon::Brain_4_Death))
    {
        return true;
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

                        const FP xPos = (GetSpriteScale() * (pBullet->XDistance() <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6)));
                        relive_new Blood(
                            xPos + mXPos,
                            mYPos - (FP_FromInteger(25) * GetSpriteScale()),
                            ((pBullet->XDistance() <= FP_FromInteger(0) ? -FP_FromInteger(1) : FP_FromInteger(1)) * xRand + FP_FromInteger(16)),
                            yRand,
                            GetSpriteScale(),
                            12);
                    }

                    {
                        const FP xPos = (GetSpriteScale() * (pBullet->XDistance() <= FP_FromInteger(0) ? -FP_FromInteger(12) : FP_FromInteger(12)));
                        relive_new Blood(
                            xPos + mXPos,
                            mYPos - (FP_FromInteger(25) * GetSpriteScale()),
                            pBullet->XDistance() <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6),
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

            mKnockbackDelayAfterGettingShotTimer = MakeTimer(5);

            if (mCurrentMotion == eGlukkonMotions::Motion_21_GetShot)
            {
                mHealth = FP_FromInteger(0);
                EventBroadcast(Event::kEventMudokonComfort, this);
                return true;
            }

            if (mCurrentMotion == eGlukkonMotions::Motion_3_KnockBack)
            {
                if (mHealth > FP_FromInteger(0))
                {
                    SetBrain(&Glukkon::Brain_4_Death);
                    mBrainSubState = Brain_4_Death::eBrain4_ToVaporize4;
                }
                mHealth = FP_FromInteger(0);
                EventBroadcast(Event::kEventMudokonComfort, this);
                return true;
            }

            mGettingShotTimer = MakeTimer(20);
            SetAnim(eGlukkonMotions::Motion_21_GetShot, true);

            SetBrain(&Glukkon::Brain_4_Death);
            mBrainSubState = Brain_4_Death::eBrain4_ToVaporize4;

            if (pBullet->XDistance() >= FP_FromInteger(0))
            {
                mVelX = FP_FromDouble(0.001);
            }
            else
            {
                mVelX = -FP_FromDouble(0.001);
            }
            mHealth = FP_FromInteger(0);
            EventBroadcast(Event::kEventMudokonComfort, this);
        }
        break;

        case ReliveTypes::eDrill:
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eAirExplosion:
            SetBrain(&Glukkon::Brain_4_Death);
            mBrainSubState = Brain_4_Death::eBrain4_ShakeGibsDeath2;
            EventBroadcast(Event::kEventMudokonComfort, this);
            break;

        case ReliveTypes::eElectricWall:
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Unused_9, 0, mGamespeakPitch, this);
            return true;

        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMineCar:
            SetAnim(eGlukkonMotions::Motion_8_DeathFall, true);
            SetBrain(&Glukkon::Brain_4_Death);
            mBrainSubState = Brain_4_Death::eBrain4_ToVaporizeDeath0;
            EventBroadcast(Event::kEventMudokonComfort, this);
            break;

        case ReliveTypes::eAbe:
            if (gAbe->mCurrentMotion == eAbeMotions::Motion_62_Punch_454750)
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
                mBrainSubState = Brain_2_Slapped::eBrain2_Unknown0;
            }
            break;

        case ReliveTypes::eSlog:
            if (mCurrentMotion != eGlukkonMotions::Motion_3_KnockBack)
            {
                mHealth = FP_FromInteger(0);
                SetBrain(&Glukkon::Brain_4_Death);
                mBrainSubState = Brain_4_Death::eBrain4_ToVaporizeKilledBySlog5;
                Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 32767, this);
                EventBroadcast(Event::kEventMudokonComfort, this);
                if (!VIsFacingMe(static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)))
                {
                    GetAnimation().ToggleFlipX();
                }
                mVelX = FP_FromInteger(0);
                SetAnim(eGlukkonMotions::Motion_3_KnockBack, true);
            }
            break;

        case ReliveTypes::eElectrocute:
            GetAnimation().SetRender(false);
            mHealth = FP_FromInteger(0);
            SetBrain(&Glukkon::Brain_4_Death);
            mBrainSubState = Brain_4_Death::eBrain4_DeathFinish3;
            field_1D4_timer = MakeTimer(1);
            EventBroadcast(Event::kEventMudokonComfort, this);
            break;

        default:
            return true;
    }

    return true;
}
