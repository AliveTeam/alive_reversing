#include "stdafx.h"
#include "Abe.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Input.hpp"
#include "../relive_lib/Events.hpp"
#include "QuikSave.hpp"
#include "MainMenu.hpp"
#include "../relive_lib/GameObjects/ThrowableArray.hpp"
#include "MusicController.hpp"
#include "GameSpeak.hpp"
#include "MusicTrigger.hpp"
#include "../relive_lib/GameObjects/ScreenShake.hpp"
#include "InvisibleEffect.hpp"
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "../relive_lib/GameObjects/BaseThrowable.hpp"
#include "../relive_lib/GameObjects/ThrowableTotalIndicator.hpp"
#include "../relive_lib/GameObjects/AbilityRing.hpp"
#include "../relive_lib/GameObjects/OrbWhirlWind.hpp"
#include "../relive_lib/GameObjects/Blood.hpp"
#include "PullRingRope.hpp"
#include "../relive_lib/GameObjects/CircularFade.hpp"
#include "../relive_lib/GameObjects/Fade.hpp"
#include "Movie.hpp"
#include "../relive_lib/GameObjects/PossessionFlicker.hpp"
#include "Door.hpp"
#include "QuikSave.hpp"
#include "../relive_lib/GameObjects/DeathBirdParticle.hpp"
#include "WorkWheel.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "Lever.hpp"
#include "../relive_lib/GameObjects/BaseThrowable.hpp"
#include "LiftPoint.hpp"
#include "PullRingRope.hpp"
#include "BrewMachine.hpp"
#include "MineCar.hpp"
#include "EvilFart.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "Mudokon.hpp"
#include "../relive_lib/GameObjects/Electrocute.hpp"
#include "BirdPortal.hpp"
#include "../relive_lib/GameObjects/BoomMachine.hpp"
#include "Shrykull.hpp"
#include "Bullet.hpp"
#include "../relive_lib/GameObjects/Spark.hpp"
#include "../relive_lib/Grid.hpp"
#include "../relive_lib/Math.hpp"
#include "AnimationCallBacks.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/Camera.hpp"
#include "Path.hpp"
#include "Game.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/Collisions.hpp"
#include "RollingBall.hpp"

Abe* gAbe = nullptr;

const relive::SfxDefinition sAbeSFXList_555250[41] = {
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 24u, 60u, 115u, 0, 0},
    {0u, 24u, 61u, 110u, 0, 0},
    {0u, 24u, 62u, 127u, 0, 0},
    {0u, 24u, 63u, 90u, 0, 0},
    {0u, 24u, 66u, 127u, 0, 0},
    {0u, 24u, 67u, 100u, 0, 0},
    {0u, 24u, 56u, 127u, 0, 0},
    {0u, 24u, 57u, 100u, 0, 0},
    {0u, 23u, 48u, 127u, 0, 0},
    {0u, 24u, 59u, 127u, 0, 0},
    {0u, 24u, 58u, 127u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 24u, 65u, 127u, 0, 0},
    {0u, 24u, 64u, 80u, 0, 0},
    {0u, 23u, 60u, 90u, 0, 0},
    {0u, 23u, 68u, 127u, 0, 0},
    {0u, 23u, 69u, 127u, 0, 0},
    {0u, 23u, 70u, 127u, 0, 0},
    {0u, 23u, 72u, 127u, 0, 0},
    {0u, 23u, 73u, 127u, 0, 0},
    {0u, 23u, 58u, 127u, 0, 0},
    {0u, 23u, 64u, 127u, 0, 0},
    {0u, 23u, 66u, 115u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 23u, 63u, 115u, 0, 0},
    {0u, 23u, 62u, 90u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

const TintEntry sAbeTintTable[16] = {
    {EReliveLevelIds::eMenu, 102u, 102u, 102u},
    {EReliveLevelIds::eMines, 102u, 102u, 102u},
    {EReliveLevelIds::eNecrum, 102u, 102u, 80u},
    {EReliveLevelIds::eMudomoVault, 120u, 90u, 120u},
    {EReliveLevelIds::eMudancheeVault, 102u, 70u, 90u},
    {EReliveLevelIds::eFeeCoDepot, 120u, 102u, 82u},
    {EReliveLevelIds::eBarracks, 102u, 102u, 102u},
    {EReliveLevelIds::eMudancheeVault_Ender, 102u, 70u, 90u},
    {EReliveLevelIds::eBonewerkz, 102u, 102u, 102u},
    {EReliveLevelIds::eBrewery, 102u, 102u, 102u},
    {EReliveLevelIds::eBrewery_Ender, 102u, 102u, 102u},
    {EReliveLevelIds::eMudomoVault_Ender, 120u, 90u, 120u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 120u, 102u, 82u},
    {EReliveLevelIds::eBarracks_Ender, 102u, 102u, 102u},
    {EReliveLevelIds::eBonewerkz_Ender, 120u, 90u, 80u},
    {EReliveLevelIds::eCredits, 102u, 102u, 102u}};

const relive::SfxDefinition sSFXList_555160[] = {
    {0u, 3u, 69u, 60u, -1, 1},
    {0u, 3u, 70u, 60u, -1, 1},
    {0u, 3u, 59u, 67u, -1, 1},
    {0u, 3u, 61u, 67u, -1, 1},
    {0u, 3u, 58u, 52u, -1, 1},
    {0u, 3u, 67u, 45u, -1, 1},
    {0u, 3u, 57u, 60u, -1, 1},
    {0u, 3u, 65u, 50u, -1, 1},
    {0u, 3u, 68u, 60u, -1, 1},
    {0u, 3u, 72u, 70u, -1, 1},
    {0u, 3u, 36u, 50u, -127, 0},
    {0u, 3u, 37u, 50u, -127, 0},
    {0u, 3u, 38u, 50u, -127, 0},
    {0u, 3u, 36u, 55u, 0, 127},
    {0u, 3u, 37u, 55u, 0, 127},
    {0u, 3u, 38u, 55u, 0, 127},
    {0u, 3u, 61u, 50u, -1, 1},
    {0u, 3u, 73u, 127u, -1, 1},
    {0u, 4u, 80u, 127u, -1, 1},
    {0u, 65u, 64u, 60u, -1, 0},
    {0u, 11u, 62u, 60u, -1, 1},
    {0u, 11u, 64u, 60u, -1, 1},
    {0u, 11u, 65u, 50u, -1, 1},
    {0u, 11u, 64u, 50u, -1, 1},
    {0u, 11u, 66u, 50u, -1, 1},
    {0u, 23u, 60u, 90u, 0, 0},
    {0u, 23u, 61u, 90u, 0, 0},
    {0u, 23u, 62u, 90u, 0, 0},
    {0u, 23u, 63u, 90u, 0, 0},
    {0u, 23u, 64u, 90u, 0, 0}};


bool IsAbe(BaseGameObject* pObj)
{
    if (gAbe)
    {
        return gAbe == pObj;
    }
    return false;
}

s32 Environment_SFX(EnvironmentSfx sfxId, s32 volume, s32 pitchMin, BaseAliveGameObject* pAliveObj)
{
    s32 sndVolume;
    s16 sndIndex = 0;

    switch (sfxId)
    {
        case EnvironmentSfx::eSlideStop_0:
            sndIndex = 11;
            sndVolume = 40;
            break;

        case EnvironmentSfx::eWalkingFootstep_1:
            sndVolume = volume;
            sndIndex = Math_RandomRange(10, 12);
            if (volume)
            {
                break;
            }
            sndVolume = Math_RandomRange(54, 58);
            if (pAliveObj)
            {
                if (pAliveObj->GetInvisible())
                {
                    sndVolume *= 3;
                }
            }
            break;

        case EnvironmentSfx::eRunningFootstep_2:
            sndVolume = volume;
            sndIndex = Math_RandomRange(13, 15);
            if (volume)
            {
                break;
            }

            sndVolume = Math_RandomRange(66, 70);

            if (pAliveObj)
            {
                if (pAliveObj->GetInvisible())
                {
                    sndVolume *= 3;
                }
            }
            break;

        case EnvironmentSfx::eSneakFootstep_3:
            sndIndex = 5;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eRunSlide_4:
            sndIndex = 4;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eLandingSoft_5:
            if (volume || !pAliveObj || pAliveObj->GetSpriteScale() != FP_FromDouble(0.5))
            {
                return SFX_SfxDefinition_Play_Mono(sSFXList_555160[2], static_cast<s16>(volume), static_cast<s16>(pitchMin), 0x7FFF) | SFX_SfxDefinition_Play_Mono(sAbeSFXList_555250[16], static_cast<s16>(volume), static_cast<s16>(pitchMin), 0x7FFF);
            }
            else
            {
                return SFX_SfxDefinition_Play_Mono(sSFXList_555160[2], sSFXList_555160[2].mDefaultVolume / 2, static_cast<s16>(pitchMin), 0x7FFF) | SFX_SfxDefinition_Play_Mono(sAbeSFXList_555250[16], sSFXList_555160[16].mDefaultVolume / 2, static_cast<s16>(pitchMin), 0x7FFF);
            }

        case EnvironmentSfx::eHitGroundSoft_6:
            sndIndex = 2;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eDeathNoise_7:
            sndIndex = 16;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eRollingNoise_8:
            sndIndex = 6;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eGenericMovement_9:
        case EnvironmentSfx::eRunJumpOrLedgeHoist_11:
            if (pAliveObj && pAliveObj->GetSpriteScale() == FP_FromDouble(0.5))
            {
                return SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 20);
            }
            else
            {
                return SfxPlayMono(relive::SoundEffects::AbeGenericMovement, 35);
            }

        case EnvironmentSfx::eExhaustingHoistNoise_10:
            sndIndex = 19;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eAllOYa_12:
            sndIndex = 25;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eKnockback_13:
            if (gMap.mCurrentLevel == EReliveLevelIds::eMines
                || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz
                || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot
                || gMap.mCurrentLevel == EReliveLevelIds::eBarracks
                || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
            {
                sndIndex = 2;
            }
            else
            {
                sndIndex = 9;
            }
            sndVolume = volume;
            break;

        case EnvironmentSfx::eElumHitWall_14:
            sndIndex = 17;
            sndVolume = volume;
            break;

        case EnvironmentSfx::eFallingDeathScreamHitGround_15:
            return 0;

        default:
            sndVolume = volume;
            break;
    }

    if (!sndVolume)
    {
        sndVolume = sSFXList_555160[sndIndex].mDefaultVolume;
    }

    if (!pAliveObj)
    {
        return SFX_SfxDefinition_Play_Mono(sSFXList_555160[sndIndex], static_cast<s16>(sndVolume), static_cast<s16>(pitchMin), 0x7FFF);
    }

    if (pAliveObj->GetSpriteScale() == FP_FromDouble(0.5))
    {
        sndVolume = 2 * sndVolume / 3;
    }

    if (!IsAbe(pAliveObj))
    {
        switch (gMap.GetDirection(
            pAliveObj->mCurrentLevel,
            pAliveObj->mCurrentPath,
            pAliveObj->mXPos,
            pAliveObj->mYPos))
        {
            case CameraPos::eCamCurrent_0:
                return SFX_SfxDefinition_Play_Mono(sSFXList_555160[sndIndex], static_cast<s16>(sndVolume), static_cast<s16>(pitchMin), 0x7FFF);
            case CameraPos::eCamTop_1:
            case CameraPos::eCamBottom_2:
                return SFX_SfxDefinition_Play_Mono(sSFXList_555160[sndIndex], static_cast<s16>(2 * sndVolume / 3), static_cast<s16>(pitchMin), 0x7FFF);
            case CameraPos::eCamLeft_3:
                return SFX_SfxDefinition_Play_Stereo(
                    sSFXList_555160[sndIndex],
                    static_cast<s16>(2 * sndVolume / 9),
                    static_cast<s16>(2 * sndVolume / 9),
                    static_cast<s16>(pitchMin),
                    0x7FFF);
                break;
            case CameraPos::eCamRight_4:
                return SFX_SfxDefinition_Play_Stereo(
                    sSFXList_555160[sndIndex],
                    static_cast<s16>(2 * sndVolume / 3),
                    static_cast<s16>(2 * sndVolume / 3),
                    static_cast<s16>(pitchMin),
                    0x7FFF);
                break;
            default:
                return 0;
        }
    }
    else
    {
        return SFX_SfxDefinition_Play_Mono(sSFXList_555160[sndIndex], static_cast<s16>(sndVolume), static_cast<s16>(pitchMin), 0x7FFF);
    }
}

static const FP_Point sThrowVelocities[9] = {
    {FP_FromInteger(3), FP_FromInteger(-14)},
    {FP_FromInteger(10), FP_FromInteger(-10)},
    {FP_FromInteger(15), FP_FromInteger(-8)},
    {FP_FromInteger(10), FP_FromInteger(3)},
    {FP_FromInteger(10), FP_FromInteger(-4)},
    {FP_FromInteger(4), FP_FromInteger(-3)},
    {FP_FromInteger(0), FP_FromInteger(0)},
    {FP_FromInteger(0), FP_FromInteger(0)},
    {FP_FromInteger(0), FP_FromInteger(0)}};


void Animation_OnFrame_Abe(BaseGameObject* pPtr, u32&, const IndexedPoint& point)
{
    auto pAbe = static_cast<Abe*>(pPtr);

    auto pThrowable = static_cast<BaseThrowable*>(sObjectIds.Find_Impl(gAbe->mThrowableId));

    auto tableX = sThrowVelocities[pAbe->mThrowDirection].x * pAbe->GetSpriteScale();
    const auto tableY = sThrowVelocities[pAbe->mThrowDirection].y * pAbe->GetSpriteScale();

    FP xOff = {};
    if (gAbe->GetAnimation().GetFlipX())
    {
        tableX = -tableX;
        xOff = -(pAbe->GetSpriteScale() * FP_FromInteger(point.mPoint.x));
    }
    else
    {
        xOff = pAbe->GetSpriteScale() * FP_FromInteger(point.mPoint.x);
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            pAbe->mXPos,
            pAbe->mYPos + FP_FromInteger(point.mPoint.y),
            xOff + pAbe->mXPos,
            pAbe->mYPos + FP_FromInteger(point.mPoint.y),
            &pLine,
            &hitX,
            &hitY,
            pAbe->GetScale() == Scale::Fg ? kFgWalls : kBgWalls))
    {
        xOff = hitX - pAbe->mXPos;
        tableX = -tableX;
    }

    if (pThrowable)
    {
        pThrowable->mXPos = xOff + gAbe->mXPos;
        pThrowable->mYPos = (pAbe->GetSpriteScale() * FP_FromInteger(point.mPoint.y)) + gAbe->mYPos;
        pThrowable->VThrow(tableX, tableY);
        pThrowable->SetSpriteScale(pAbe->GetSpriteScale());
        pThrowable->SetScale(pAbe->GetScale());
        gAbe->mThrowableId = Guid{};
    }
}

static constexpr s32 kResourceArraySize = 28;

void Abe::LoadAnimations()
{
    for (auto& animId : sAbeAnimIdTable)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Abe::Abe() :
    BaseAliveGameObject(kResourceArraySize)
{
    SetType(ReliveTypes::eAbe);

    SetSurviveDeathReset(true);

    // Set the well level to the current level for the path start quick save
    mDstWellLevel = gMap.mCurrentLevel;

    Init_GameStates();

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Mudokon_Idle));

    GetAnimation().SetFnPtrArray(gAbe_Anim_Frame_Fns);

    PSX_Point point = {};
    gMap.GetCurrentCamCoords(&point);

    mXPos = FP_FromInteger(point.x + XGrid_Index_To_XPos_AE(GetSpriteScale(), 4));
    mYPos = FP_FromInteger(point.y + 120);

    BaseAliveGameObjectLastLineYPos = mYPos;

    mCurrentMotion = eAbeMotions::Motion_3_Fall_459B60;
    GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);

    SetCanSetOffExplosives(true);

    // Changes Abe's "default" colour depending on the level we are in
    SetTint(&sAbeTintTable[0], gMap.mCurrentLevel);

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);

    field_120_state.raw = 0;
    field_124_timer = sGnFrame;

    // Set Abe to be the current player controlled object
    sControlledCharacter = this;

    CreateShadow();
}

Abe::~Abe()
{
    BaseGameObject* pFadeObject = sObjectIds.Find_Impl(mFadeId);
    BaseGameObject* pCircularFade = sObjectIds.Find_Impl(mCircularFadeId);
    BaseGameObject* pOrbWhirlWind = sObjectIds.Find_Impl(mOrbWhirlWindId);
    BaseGameObject* pPossessedObject = sObjectIds.Find_Impl(mPossessedObjectId);
    BaseGameObject* pThrowable = sObjectIds.Find_Impl(mThrowableId);
    BaseGameObject* pPullRope = sObjectIds.Find_Impl(mPullRingRopeId);
    BaseGameObject* pItem = sObjectIds.Find_Impl(mSlappableOrPickupId);
    BaseGameObject* pInvisibleEffect = sObjectIds.Find_Impl(mInvisibleEffectId);

    SND_SEQ_Stop(SeqId::MudokonChant1_10);

    if (pFadeObject)
    {
        pFadeObject->SetDead(true);
        mFadeId = Guid{};
    }

    if (pItem)
    {
        mSlappableOrPickupId = Guid{};
    }

    if (pPullRope)
    {
        pPullRope->SetDead(true);
        mPullRingRopeId = Guid{};
    }

    if (pCircularFade)
    {
        pCircularFade->SetDead(true);
        mCircularFadeId = Guid{};
    }

    if (pOrbWhirlWind)
    {
        pOrbWhirlWind->SetDead(true);
        mOrbWhirlWindId = Guid{};
    }

    if (pPossessedObject)
    {
        mPossessedObjectId = Guid{};
    }

    if (pThrowable)
    {
        pThrowable->SetDead(true);
        mThrowableId = Guid{};
    }

    if (pInvisibleEffect)
    {
        pInvisibleEffect->SetDead(true);
        mInvisibleEffectId = Guid{};
    }

    mWorkWheelId = Guid{};

    if (sControlledCharacter == this)
    {
        sControlledCharacter = nullptr;
    }
    gAbe = nullptr;
}


void Abe::CreateFromSaveState(SerializedObjectData& pData)
{
    const AbeSaveState* pSaveState = pData.ReadTmpPtr<AbeSaveState>();
    Abe::CreateFromSaveState(*pSaveState);
}

void Abe::CreateFromSaveState(const AbeSaveState& pData)
{
    Abe* pAbe = gAbe;
    if (!gAbe)
    {
        pAbe = relive_new Abe();
        gAbe = pAbe;
    }

    if (pData.mIsAbeControlled)
    {
        sControlledCharacter = pAbe;
    }

    gAbe->BaseAliveGameObjectPathTLV = nullptr;
    gAbe->BaseAliveGameObjectCollisionLine = nullptr;
    gAbe->mXPos = pData.mXPos;
    gAbe->mYPos = pData.mYPos;
    gAbe->mVelX = pData.mVelX;
    gAbe->mVelY = pData.mVelY;
    gAbe->field_8_x_vel_slow_by = pData.field_48_x_vel_slow_by;
    gAbe->mCurrentPath = pData.mCurrentPath;
    gAbe->mCurrentLevel = pData.mCurrentLevel;
    gAbe->SetSpriteScale(pData.mSpriteScale);
    gAbe->SetScale(pData.mScale);

    gAbe->mCurrentMotion = pData.mCurrentMotion;

    /* TODO: Async load if res unloaded
    u8** animFromState = gAbe->MotionToAnimResource_44AAB0(gAbe->mCurrentMotion);
    if (!animFromState)
    {
        u32 id = sAbeResourceIDTable_554D60[gAbe->field_10_resource_index];
        // ResourceManager::LoadResourceFile_49C170(sAbe_ResNames_545830[gAbe->field_10_resource_index], 0);
        gAbe->mBaseGameObjectResArray.SetAt(gAbe->field_10_resource_index, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, id, true, false));
        animFromState = gAbe->mBaseGameObjectResArray.ItemAt(gAbe->field_10_resource_index);
    }*/

    gAbe->GetAnimation().Set_Animation_Data(gAbe->GetAnimRes(sAbeAnimIdTable[static_cast<u32>(gAbe->mCurrentMotion)])); // TODO: remove cast later
    // gAbe->mAnim.Set_Animation_Data_409C80(sAbeAnimIdTable[gAbe->mCurrentMotion], animFromState);

    gAbe->GetAnimation().SetCurrentFrame(pData.mCurrentFrame);
    gAbe->GetAnimation().SetFrameChangeCounter(pData.mFrameChangeCounter);

    gAbe->GetAnimation().SetFlipX(pData.bAnimFlipX);
    gAbe->GetAnimation().SetRender(pData.mAnimRender);
    gAbe->SetDrawable(pData.mIsDrawable);

    gAbe->GetAnimation().SetRenderLayer(static_cast<Layer>(pData.mRenderLayer));

    if (IsLastFrame(&gAbe->GetAnimation()))
    {
        gAbe->GetAnimation().SetIsLastFrame(true);
    }

    // TODO: Pal re-loading
    /*
    const PerFrameInfo* pFrameInfoHeader = gAbe->mAnim.Get_FrameHeader(-1);
    gAbe->mAnim.LoadPal(gAbe->mAnim.field_20_ppBlock, pFrameHeader->field_0_clut_offset);
    */
    gAbe->GetAnimation().ReloadPal();

    gAbe->SetTint(sAbeTintTable, gMap.mCurrentLevel);
    gAbe->GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);
    gAbe->GetAnimation().SetSemiTrans(true);
    gAbe->GetAnimation().SetBlending(false);
    gAbe->mHealth = pData.mHealth;
    gAbe->mCurrentMotion = pData.mCurrentMotion2;
    gAbe->mNextMotion = pData.mNextMotion;
    gAbe->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pData.mLastLineYPos);
    gAbe->BaseAliveGameObject_PlatformId = pData.mPlatformId;
    gAbe->field_120_state.raw = static_cast<u16>(pData.field_50_state);
    gAbe->field_124_timer = pData.field_54_timer;
    gAbe->field_0_abe_timer = pData.field_58_abe_timer;
    gAbe->mRegenHealthTimer = pData.mRegenHealthTimer;
    gAbe->mMood = pData.mMood;
    gAbe->mSay = pData.mSay;
    gAbe->mAutoSayTimer = pData.mAutoSayTimer;
    gAbe->mBaseThrowableCount = pData.mBaseThrowableCount;
    gAbe->mRingPulseTimer = pData.mRingPulseTimer;
    gAbe->mHaveShrykull = pData.mHaveShrykull;

    if (gAbe->mRingPulseTimer && gAbe->mHaveShrykull)
    {
        if (!gAbe->mBaseGameObjectResArray.ItemAt(25))
        {
        }
    }
    else
    {
        if (gAbe->mBaseGameObjectResArray.ItemAt(25))
        {
            gAbe->Free_Shrykull_Resources_45AA90();
        }
    }

    gAbe->mHaveInvisibility = pData.bHaveInvisiblity;
    gAbe->BaseAliveGameObjectCollisionLineType = pData.mCollisionLineType;

    gAbe->mPrevInput = InputObject::PsxButtonsToKeyboardInput_45EE40(pData.mPrevInput);
    gAbe->mReleasedButtons = InputObject::PsxButtonsToKeyboardInput_45EE40(pData.mReleasedButtons);
    gAbe->mKnockdownMotion = pData.mKnockdownMotion;
    gAbe->mRollingMotionTimer = sGnFrame - pData.mRollingMotionTimer;

    gAbe->mFadeId = pData.mDeathFadeOutId;
    gAbe->mCircularFadeId = pData.mCircularFadeId;
    gAbe->mOrbWhirlWindId = pData.mOrbWhirlWindId;
    gAbe->mPossessedObjectId = pData.mPossessedObjectId;
    gAbe->mThrowableId = pData.mThrowableId;
    gAbe->mPullRingRopeId = pData.mPullRingRopeId;
    gAbe->mSlappableOrPickupId = pData.mSlappableOrPickupId;
    gAbe->mWorkWheelId = pData.mWorkWheelId;
    gAbe->mInvisibleEffectId = Guid{};

    gAbe->mInvisibilityTimer = pData.mInvisibilityTimer;
    gAbe->mInvisibilityDuration = pData.mInvisibilityDuration;

    gAbe->mHandStoneCamIdx = pData.mHandStoneCamIdx;
    gAbe->mHandStoneType = pData.mHandStoneType;
    gAbe->mFmvId = pData.mFmvId;
    gAbe->mHandStoneCams[0] = pData.mHandStoneCam1;
    gAbe->mHandStoneCams[1] = pData.mHandStoneCam2;
    gAbe->mHandStoneCams[2] = pData.mHandStoneCam3;

    gAbe->mHasEvilFart = pData.mHasEvilFart;
    gAbe->mDstWellLevel = pData.mDstWellLevel;
    gAbe->mDstWellPath = pData.mDstWellPath;
    gAbe->mDstWellCamera = pData.mDstWellCamera;
    gAbe->field_1A0_door_id = pData.door_id;
    gAbe->mThrowDirection = pData.mThrowDirection;
    gAbe->mBirdPortalSubState = static_cast<PortalSubStates>(pData.mBirdPortalSubState);
    gAbe->mBirdPortalId = pData.mBirdPortalId;

    gAbe->SetElectrocuted(pData.mIsElectrocuted);
    gAbe->SetInvisible(pData.mIsInvisible);
    gAbe->SetTeleporting(pData.mTeleporting);

    gAbe->mReturnToPreviousMotion = pData.mReturnToPreviousMotion;
    gAbe->mShrivel = pData.mShrivel;
    gAbe->mPreventChanting = pData.mPreventChanting;
    gAbe->mLandSoftly = pData.mLandSoftly;
    gAbe->mLaughAtChantEnd = pData.mLaughAtChantEnd;

    gAbe->mPlayLedgeGrabSounds = pData.mPlayLedgeGrabSounds;
    gAbe->mHaveHealing = pData.mHaveHealing;
    gAbe->SetTeleporting(pData.mTeleporting);

    gAbe->mMudancheeDone = pData.mMudancheeDone;
    gAbe->mMudomoDone = pData.mMudomoDone;

    gAbe->GetShadow()->mEnabled = pData.mShadowEnabled;
    gAbe->GetShadow()->mShadowAtBottom = pData.mShadowAtBottom;
}


const FP sAbe_xVel_table_545770[8] = {
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4)};

const FP sAbe_yVel_table_545790[8] = {
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(4)};

bool gAbeInvincible = false;


void Abe::HandleDDCheat()
{
    VOnTrapDoorOpen();

    BaseAliveGameObjectLastLineYPos = mYPos;
    mShrivel = false;
    mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
    BaseAliveGameObjectCollisionLine = nullptr;

    if (Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft | InputCommands::eDown | InputCommands::eUp))
    {
        mVelX = sAbe_xVel_table_545770[(u8) Input().mPads[sCurrentControllerIndex].mDir >> 5];
        mVelY = sAbe_yVel_table_545790[(u8) Input().mPads[sCurrentControllerIndex].mDir >> 5];

        if (Input().IsAnyHeld(InputCommands::eRun))
        {
            mVelX += sAbe_xVel_table_545770[(u8) Input().mPads[sCurrentControllerIndex].mDir >> 5];
            mVelX += sAbe_xVel_table_545770[(u8) Input().mPads[sCurrentControllerIndex].mDir >> 5];
            mVelY += sAbe_yVel_table_545790[(u8) Input().mPads[sCurrentControllerIndex].mDir >> 5];
        }

        mXPos += mVelX;
        mYPos += mVelY;

        // Keep within map max min bounds
        if (mXPos < FP_FromInteger(0))
        {
            mXPos = FP_FromInteger(0);
        }

        if (mYPos < FP_FromInteger(0))
        {
            mYPos = FP_FromInteger(0);
        }

        // Keep within map max bounds
        PSX_Point mapSize = {};
        gMap.Get_map_size(&mapSize);

        FP mapWidth = FP_FromInteger(mapSize.x);
        if (mXPos >= mapWidth)
        {
            mXPos = mapWidth - FP_FromDouble(1.0);
        }

        FP mapHeight = FP_FromInteger(mapSize.y);
        if (mYPos >= mapHeight)
        {
            mYPos = mapHeight - FP_FromDouble(1.0);
        }
    }
    else
    {
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
    }

    SetActiveCameraDelayedFromDir();
}

void Abe::VUpdate()
{
    if (gAbeInvincible)
    {
        SetElectrocuted(false);
        mHealth = FP_FromDouble(1.0);
    }

    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);
        if (BaseAliveGameObjectCollisionLineType != -1)
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

            BaseAliveGameObjectCollisionLineType = -1;
        }

        BaseAliveGameObject_PlatformId = BaseGameObject::RefreshId(BaseAliveGameObject_PlatformId);
        mFadeId = BaseGameObject::RefreshId(mFadeId);
        mCircularFadeId = BaseGameObject::RefreshId(mCircularFadeId);
        mBirdPortalId = BaseGameObject::RefreshId(mBirdPortalId);
        mOrbWhirlWindId = BaseGameObject::RefreshId(mOrbWhirlWindId);
        mPossessedObjectId = BaseGameObject::RefreshId(mPossessedObjectId);
        mThrowableId = BaseGameObject::RefreshId(mThrowableId);
        mPullRingRopeId = BaseGameObject::RefreshId(mPullRingRopeId);
        mSlappableOrPickupId = BaseGameObject::RefreshId(mSlappableOrPickupId);
        mWorkWheelId = BaseGameObject::RefreshId(mWorkWheelId);

        if (GetInvisible())
        {
            if (!mInvisibilityTimer)
            {
                mInvisibilityTimer = MakeTimer(2);
            }

            auto pInvisibleEffect = relive_new InvisibleEffect(this);
            mInvisibleEffectId = pInvisibleEffect->mBaseGameObjectId;
            pInvisibleEffect->InstantInvisibility();
        }
    }

    if (gAbeInvincible)
    {
        mHealth = FP_FromDouble(1.0);
    }

    if (!Input_IsChanting())
    {
        mPreventChanting = false;
    }

    const s32 totalAliveSavedMuds = gRescuedMudokons - gKilledMudokons;
    if (totalAliveSavedMuds > 0)
    {
        // "Happy" voice
        mMood = Mud_Emotion::eHappy_5;
    }
    else if (totalAliveSavedMuds < 0)
    {
        // "Sad" voice
        mMood = Mud_Emotion::eSad_3;
    }
    else
    {
        // "Normal" voice
        mMood = Mud_Emotion::eNormal_0;
    }

    if (gDDCheat_FlyingEnabled && sControlledCharacter == this)
    {
        HandleDDCheat();
    }
    else
    {
        
        GetAnimation().SetAnimate(true);

        eAbeMotions motion_idx = mCurrentMotion;

        // Execute the current motion
        const FP oldXPos = mXPos;
        const FP oldYPos = mYPos;
        InvokeMemberFunction(this, sAbeMotionMachineTable_554910, motion_idx);

        if (GetRestoredFromQuickSave() || mShrivel)
        {
            return;
        }

        if (BaseAliveGameObjectCollisionLine)
        {
            // Snap to a whole number so we are "on" the line
            mYPos = FP_NoFractional(mYPos);
        }

        // Did position change?
        if (oldXPos != mXPos || oldYPos != mYPos)
        {
            // Get the TLV we are on
            BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (mbGotShot)
        {
            motion_idx = mKnockdownMotion;
            ToKnockback_44E700(1, 1);
            if (motion_idx != eAbeMotions::None_m1)
            {
                mCurrentMotion = motion_idx;
            }

            mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
            mReturnToPreviousMotion = false;
            mKnockdownMotion = eAbeMotions::Motion_0_Idle_44EEB0;
            mbGotShot = false;
            mbMotionChanged = true;
        }

        if (EventGet(Event::kEventScreenShake) && mHealth > FP_FromInteger(0))
        {
            if (IsStanding_449D30())
            {
                ToKnockback_44E700(1, 0);
            }
        }

        if (mSay != MudSounds::eNone && static_cast<s32>(sGnFrame) >= mAutoSayTimer)
        {
            if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0)
                || (mCurrentMotion == eAbeMotions::Motion_112_Chant)
                || mCurrentMotion == eAbeMotions::Motion_7_Speak_45B140
                || mCurrentMotion == eAbeMotions::Motion_8_Speak_45B160
                || mCurrentMotion == eAbeMotions::Motion_9_Speak_45B180
                || mCurrentMotion == eAbeMotions::Motion_10_Fart_45B1A0)
            {
                // Prevents double laugh when blowing up a slig as we prevent saying anything while chanting or various other states
                // or when abe isn't in the active screen
                mSay = MudSounds::eNone;
            }
            else
            {
                if (mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0 || mCurrentMotion == eAbeMotions::Motion_12_Null_4569C0)
                {
                    mbMotionChanged = true;
                    switch (mSay)
                    {
                        case MudSounds::eOops_14:
                            mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                            break;
                        case MudSounds::eAnger_5:
                            mCurrentMotion = eAbeMotions::Motion_10_Fart_45B1A0;
                            break;
                        case MudSounds::eSadUgh_28:
                            mCurrentMotion = eAbeMotions::Motion_10_Fart_45B1A0;
                            break;
                        default:
                            mCurrentMotion = eAbeMotions::Motion_9_Speak_45B180;
                            break;
                    }
                }

                if (mSay == MudSounds::eAnger_5)
                {
                    // Other evil muds laugh at the abe grr
                    EventBroadcast(Event::kEventMudokonLaugh, gAbe);
                }

                if (mSay == MudSounds::eSadUgh_28)
                {
                    // This one has another volume for whatever reason
                    Mudokon_SFX(mSay, 80, 0, this);
                }
                else
                {
                    Mudokon_SFX(mSay, 0, 0, this);
                }

                mSay = MudSounds::eNone;
            }
        }

        if (motion_idx != mCurrentMotion || mbMotionChanged)
        {
            mbMotionChanged = false;
            if (mCurrentMotion != eAbeMotions::Motion_12_Null_4569C0 && !mShrivel)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(sAbeAnimIdTable[static_cast<u32>(mCurrentMotion)]));

                mRollingMotionTimer = sGnFrame;

                if (motion_idx == eAbeMotions::Motion_12_Null_4569C0 || motion_idx == eAbeMotions::Motion_60_Unused_4A3200)
                {
                    GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                }
            }
        }

        if (mReturnToPreviousMotion)
        {
            mCurrentMotion = mPreviousMotion;

            GetAnimation().Set_Animation_Data(GetAnimRes(sAbeAnimIdTable[static_cast<u32>(mCurrentMotion)]));
            mRollingMotionTimer = sGnFrame;
            GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
            mReturnToPreviousMotion = false;
        }

        if (mRegenHealthTimer <= static_cast<s32>(sGnFrame) && mHealth > FP_FromInteger(0))
        {
            mHealth = FP_FromDouble(1.0);
        }

        // Draw power up ring "pulse"
        if (mRingPulseTimer > 0)
        {
            if (GetAnimation().GetRender())
            {
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    if (mRingPulseTimer > static_cast<s32>(sGnFrame))
                    {
                        if (!(sGnFrame % 32))
                        {
                            RingTypes ringType = RingTypes::eExplosive_Pulse_0;
                            if (mHaveShrykull)
                            {
                                ringType = RingTypes::eShrykull_Pulse_Small_4;
                            }
                            else if (mHaveInvisibility)
                            {
                                ringType = RingTypes::eInvisible_Pulse_Small_7;
                            }
                            else if (mHaveHealing)
                            {
                                ringType = RingTypes::eHealing_Pulse_14;
                            }

                            const PSX_RECT rect = VGetBoundingRect();
                            AbilityRing::Factory(
                                FP_FromInteger((rect.x + rect.w) / 2),
                                FP_FromInteger((rect.y + rect.h) / 2),
                                ringType, GetSpriteScale());

                            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 25, 2650);
                        }
                    }
                    else
                    {
                        if (mRingPulseTimer > 0 && mHaveShrykull > 0)
                        {
                            Free_Shrykull_Resources_45AA90();
                        }
                        mRingPulseTimer = 0;
                    }
                }
            }
        }


        InvisibleEffect* pObj_field_178 = static_cast<InvisibleEffect*>(sObjectIds.Find(mInvisibleEffectId, ReliveTypes::eInvisibleEffect));
        if (pObj_field_178 && mInvisibilityTimer > 0)
        {
            if (static_cast<s32>(sGnFrame) > mInvisibilityTimer)
            {
                mInvisibilityTimer = 0;
                pObj_field_178->BecomeVisible();
            }
        }

        // After the trials give Abe the healing power for the Necrum muds.
        if (mMudancheeDone)
        {
            if (mMudomoDone)
            {
                if (gMap.mCurrentLevel == EReliveLevelIds::eNecrum)
                {
                    mRingPulseTimer = MakeTimer(200000);
                    mHaveShrykull = 0;
                    mHaveInvisibility = 0;
                    mMudomoDone = false;
                    mMudancheeDone = false;
                    mHaveHealing = true;
                }
            }
        }

        if (EventGet(Event::kEventMudokonDied))
        {
            mSay = MudSounds::eOops_14;
            mAutoSayTimer = MakeTimer(Math_RandomRange(22, 30));

            relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort, relive::Path_MusicTrigger::TriggeredBy::eTimer, 0);
        }

        if (EventGet(Event::kEventMudokonComfort))
        {
            mSay = MudSounds::eGiggle_8;
            mAutoSayTimer = MakeTimer(Math_RandomRange(22, 30));
        }

        if (EventGet(Event::kScrabOrParamiteDied))
        {
            mSay = MudSounds::eOops_14;
            mAutoSayTimer = MakeTimer(Math_RandomRange(22, 30));
        }

        if (mDoQuicksave)
        {
            mDoQuicksave = false;
            QuikSave::gActiveQuicksaveData.mWorldInfo.mSaveFileId = mSaveFileId;
            QuikSave::SaveWorldInfo(&QuikSave::gActiveQuicksaveData.mRestartPathWorldInfo);
            GetSaveState(QuikSave::gActiveQuicksaveData.mRestartPathAbeState);
            QuikSave::gActiveQuicksaveData.mRestartPathSwitchStates = gSwitchStates;
            QuikSave::DoQuicksave();
        }
    }
}

IBirdPortal* Abe::VIntoBirdPortal(s16 gridBlocks)
{
    auto pPortal = BaseAliveGameObject::VIntoBirdPortal(gridBlocks);
    if (pPortal && pPortal->mPortalType == relive::Path_BirdPortal::PortalType::eAbe)
    {
        return pPortal;
    }
    return nullptr;
}

void Abe::VOnTrapDoorOpen()
{
    // Handles falling when previously was on a platform, stop turning a wheel if we where turning one etc.
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds.Find(mWorkWheelId, ReliveTypes::eWorkWheel));
    if (pPlatform)
    {
        if (!mShrivel)
        {
            mCurrentMotion = eAbeMotions::Motion_93_WalkOffEdge;
            mbMotionChanged = true;
        }

        pPlatform->VRemove(this);

        BaseAliveGameObject_PlatformId = Guid{};
        BaseAliveGameObjectLastLineYPos = mYPos;

        if (pWheel)
        {
            pWheel->VStopTurning(false);
        }
    }
}

void Abe::ToKnockback_44E700(s16 bKnockbackSound, s16 bDelayedAnger)
{
    OrbWhirlWind* pfield_150 = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(mOrbWhirlWindId));
    BaseThrowable* pfield_158 = static_cast<BaseThrowable*>(sObjectIds.Find_Impl(mThrowableId));
    WorkWheel* pfield_164 = static_cast<WorkWheel*>(sObjectIds.Find(mWorkWheelId, ReliveTypes::eWorkWheel));
    if (sControlledCharacter == this || mHealth <= FP_FromInteger(0))
    {
        // Chant music/orb kill ?
        SND_SEQ_Stop(SeqId::MudokonChant1_10);
        if (pfield_150)
        {
            pfield_150->ToStop();
            mOrbWhirlWindId = Guid{};
        }

        if (pfield_164)
        {
            pfield_164->VStopTurning(1);
            mWorkWheelId = Guid{};
        }

        if (mVelX != FP_FromInteger(0))
        {
            mXPos -= mVelX;
        }

        MapFollowMe(true);

        mVelX = FP_FromInteger(0);

        if (mVelY < FP_FromInteger(0))
        {
            mVelY = FP_FromInteger(0);
        }

        // NOTE: This always seems to be set to true. This parameter might not be needed.
        if (bKnockbackSound)
        {
            Mudokon_SFX(MudSounds::eHurt2_9, 0, Math_RandomRange(-127, 127), this);
            Environment_SFX(EnvironmentSfx::eKnockback_13, 0, 32767, this);
        }

        mCurrentMotion = eAbeMotions::Motion_71_Knockback_455090;

        if (bDelayedAnger)
        {
            mSay = MudSounds::eAnger_5;    // anger in..
            mAutoSayTimer = MakeTimer(27);
        }

        if (pfield_158)
        {
            pfield_158->VToDead();
            mThrowableId = Guid{};
            if (!gInfiniteThrowables)
            {
                mBaseThrowableCount++;
            }
        }
    }
}

void Abe::VRender(OrderingTable& ot)
{
    // When in death shrivel don't reset scale else can't shrivel into a black blob
    if (!mShrivel)
    {
        GetAnimation().SetSpriteScale(GetSpriteScale());
    }

    if (mCurrentMotion != eAbeMotions::Motion_79_InsideWellLocal_45CA60 && mCurrentMotion != eAbeMotions::Motion_82_InsideWellExpress_45CC80 && mCurrentMotion != eAbeMotions::Motion_76_ToInsideOfAWellLocal_45CA40)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void Abe::VScreenChanged()
{
    if (sControlledCharacter == this)
    {
        mCurrentLevel = gMap.mNextLevel;
        mCurrentPath = gMap.mNextPath;
    }

    // Level has changed?
    if (gMap.LevelChanged())
    {
        // Hack to make Abe mSay hello in the first screen of the mines
        if (gMap.mNextLevel == EReliveLevelIds::eMines && !gAttract)
        {
            mSay = MudSounds::eHelloNeutral_3;
            mAutoSayTimer = MakeTimer(35);
        }

        // Set the correct tint for this map
        SetTint(sAbeTintTable, gMap.mNextLevel);

        if (gMap.mCurrentLevel != EReliveLevelIds::eNone)
        {
            if (mBaseThrowableCount > 0)
            {
                if (gThrowableArray)
                {
                    gThrowableArray->Remove(mBaseThrowableCount);
                }
            }

            mBaseThrowableCount = 0;

            if (mRingPulseTimer > 0 && mHaveShrykull)
            {
                Free_Shrykull_Resources_45AA90();
            }

            mRingPulseTimer = 0;
        }

        if (gMap.mNextLevel == EReliveLevelIds::eNecrum)
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault_Ender)
            {
                mMudancheeDone = true;
            }

            if (gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault_Ender)
            {
                mMudomoDone = true;
            }
        }

        if (gMap.mNextLevel == EReliveLevelIds::eCredits || gMap.mNextLevel == EReliveLevelIds::eMenu)
        {
            // Remove Abe for menu/credits levels?
            SetDead(true);
        }
    }

    // If level or path changed then kill rings and farts
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        mRingPulseTimer = 0;
        if (gMap.mCurrentLevel != EReliveLevelIds::eNone)
        {
            mHasEvilFart = false;
        }
    }

    if (gMap.LevelChanged() && !(GetRestoredFromQuickSave()))
    {
        for (s8& val : gSavedKilledMudsPerZulag.mData)
        {
            val = 0;
        }
    }
}

void Abe::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    AbeSaveState data = {};
    GetSaveState(data);
    pSaveBuffer.Write(data);
}

void Abe::GetSaveState(AbeSaveState& pSaveState)
{
    pSaveState.mType = ReliveTypes::eAbe;
    pSaveState.mXPos = mXPos;
    pSaveState.mYPos = mYPos;
    pSaveState.mVelX = mVelX;
    pSaveState.mVelY = mVelY;
    pSaveState.field_48_x_vel_slow_by = field_8_x_vel_slow_by;
    pSaveState.mCurrentPath = mCurrentPath;
    pSaveState.mCurrentLevel = mCurrentLevel;
    pSaveState.mSpriteScale = GetSpriteScale();
    pSaveState.mScale = GetScale();
    pSaveState.mRed = mRGB.r;
    pSaveState.mGreen = mRGB.g;
    pSaveState.mBlue = mRGB.b;

    if (GetElectrocuting())
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            auto pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->Type() == ReliveTypes::eElectrocute)
            {
                auto pElectrocute = static_cast<const Electrocute*>(pObj);
                if (pElectrocute->mTargetObjId == mBaseGameObjectId)
                {
                    pSaveState.mRed = pElectrocute->mTargetRed;
                    pSaveState.mGreen = pElectrocute->mTargetGreen;
                    pSaveState.mBlue = pElectrocute->mTargetBlue;
                    break;
                }
            }
        }
    }

    pSaveState.bAnimFlipX = GetAnimation().GetFlipX();
    pSaveState.mCurrentMotion = mCurrentMotion;
    pSaveState.mCurrentFrame = GetAnimation().GetCurrentFrame();
    pSaveState.mFrameChangeCounter = static_cast<u16>(GetAnimation().GetFrameChangeCounter());

    if (GetAnimation().GetFrameChangeCounter() == 0)
    {
        pSaveState.mFrameChangeCounter = 1;
    }

    pSaveState.mIsDrawable = GetDrawable();
    pSaveState.mAnimRender = GetAnimation().GetRender();
    pSaveState.mRenderLayer = static_cast<s8>(GetAnimation().GetRenderLayer());
    pSaveState.mHealth = mHealth;
    pSaveState.mCurrentMotion2 = mCurrentMotion;
    pSaveState.mNextMotion = mNextMotion;
    pSaveState.mLastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    pSaveState.mIsElectrocuted = GetElectrocuted();

    pSaveState.mIsInvisible = GetInvisible();

    if (BaseAliveGameObjectCollisionLine)
    {
        pSaveState.mCollisionLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pSaveState.mCollisionLineType = eLineTypes::eNone_m1;
    }

    pSaveState.mPlatformId = BaseAliveGameObject_PlatformId;

    if (BaseAliveGameObject_PlatformId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pObj)
        {
            pSaveState.mPlatformId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState.mIsAbeControlled = (this == sControlledCharacter);
    pSaveState.field_50_state = field_120_state.raw;
    pSaveState.field_54_timer = field_124_timer;
    pSaveState.field_58_abe_timer = field_0_abe_timer;
    pSaveState.mRegenHealthTimer = mRegenHealthTimer;
    pSaveState.mMood = mMood;
    pSaveState.mSay = mSay;
    pSaveState.mAutoSayTimer = mAutoSayTimer;
    pSaveState.mRingPulseTimer = mRingPulseTimer;
    pSaveState.mBaseThrowableCount = mBaseThrowableCount;
    pSaveState.mHaveShrykull = static_cast<s8>(mHaveShrykull);
    pSaveState.bHaveInvisiblity = static_cast<s8>(mHaveInvisibility);

    pSaveState.mPrevInput = InputObject::KeyboardInputToPsxButtons_45EF70(mPrevInput);
    pSaveState.mReleasedButtons = InputObject::KeyboardInputToPsxButtons_45EF70(mReleasedButtons);

    pSaveState.mKnockdownMotion = mKnockdownMotion;
    pSaveState.mRollingMotionTimer = sGnFrame - mRollingMotionTimer;
    pSaveState.mDeathFadeOutId = mFadeId;

    if (mFadeId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(mFadeId);
        if (pObj)
        {
            pSaveState.mDeathFadeOutId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState.mCircularFadeId = mCircularFadeId;

    if (mCircularFadeId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(mCircularFadeId);
        if (pObj)
        {
            pSaveState.mCircularFadeId = pObj->mBaseGameObjectTlvInfo;
        }
    }
    pSaveState.mOrbWhirlWindId = mOrbWhirlWindId;

    if (mOrbWhirlWindId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(mOrbWhirlWindId);
        if (pObj)
        {
            pSaveState.mOrbWhirlWindId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState.mPossessedObjectId = mPossessedObjectId;

    if (mPossessedObjectId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(mPossessedObjectId);
        if (pObj)
        {
            pSaveState.mPossessedObjectId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState.mThrowableId = mThrowableId;

    if (mThrowableId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(mThrowableId);
        if (pObj)
        {
            pSaveState.mThrowableId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState.mPullRingRopeId = mPullRingRopeId;

    if (mPullRingRopeId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(mPullRingRopeId);
        if (pObj)
        {
            pSaveState.mPullRingRopeId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState.mSlappableOrPickupId = mSlappableOrPickupId;

    if (mSlappableOrPickupId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(mSlappableOrPickupId);
        if (pObj)
        {
            pSaveState.mSlappableOrPickupId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState.mWorkWheelId = mWorkWheelId;

    if (mWorkWheelId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(mWorkWheelId);
        if (pObj)
        {
            pSaveState.mWorkWheelId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState.mBirdPortalId = mBirdPortalId;

    if (mBirdPortalId != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(mBirdPortalId);
        if (pObj)
        {
            pSaveState.mBirdPortalId = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pSaveState.mInvisibilityTimer = mInvisibilityTimer;
    pSaveState.mInvisibilityDuration = mInvisibilityDuration;
    pSaveState.mHandStoneCamIdx = mHandStoneCamIdx;
    pSaveState.mHandStoneType = mHandStoneType;
    pSaveState.mFmvId = mFmvId;
    pSaveState.mHandStoneCam1 = mHandStoneCams[0];
    pSaveState.mHandStoneCam2 = mHandStoneCams[1];
    pSaveState.mHandStoneCam3 = mHandStoneCams[2];
    pSaveState.mHasEvilFart = mHasEvilFart;
    pSaveState.mDstWellLevel = mDstWellLevel;
    pSaveState.mDstWellPath = mDstWellPath;
    pSaveState.mDstWellCamera = mDstWellCamera;
    pSaveState.door_id = field_1A0_door_id;
    pSaveState.mThrowDirection = mThrowDirection;
    pSaveState.mBirdPortalSubState = static_cast<u16>(mBirdPortalSubState);

    pSaveState.mIsElectrocuted = GetElectrocuted();
    pSaveState.mIsInvisible = GetInvisible();
    pSaveState.mTeleporting = gAbe->GetTeleporting();

    pSaveState.mReturnToPreviousMotion = mReturnToPreviousMotion;
    pSaveState.mShrivel = mShrivel;
    pSaveState.mPreventChanting = mPreventChanting;
    pSaveState.mLandSoftly = mLandSoftly;
    pSaveState.mLaughAtChantEnd = mLaughAtChantEnd;

    pSaveState.mPlayLedgeGrabSounds = mPlayLedgeGrabSounds;
    pSaveState.mHaveHealing = mHaveHealing;
    pSaveState.mTeleporting = GetTeleporting();
    pSaveState.mMudancheeDone = mMudancheeDone;
    pSaveState.mMudomoDone = mMudomoDone;

    pSaveState.mShadowEnabled = GetShadow()->mEnabled;
    pSaveState.mShadowAtBottom = GetShadow()->mShadowAtBottom;
}

bool Abe::VTakeDamage(BaseGameObject* pFrom)
{
    // Stop chant sound music.
    SND_SEQ_Stop(SeqId::MudokonChant1_10);

    const MudSounds oldSay = mSay;
    mSay = MudSounds::eNone;

    OrbWhirlWind* pWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(mOrbWhirlWindId));
    if (pWhirlWind)
    {
        pWhirlWind->ToStop();
        mOrbWhirlWindId = Guid{};
    }

    if (CantBeDamaged_44BAB0())
    {
        return false;
    }

    if (gAbeInvincible)
    {
        return false;
    }

    if (GetTeleporting())
    {
        return false;
    }

    mRegenHealthTimer = MakeTimer(180);
    bool ret = mHealth > FP_FromInteger(0);

    switch (pFrom->Type())
    {
        case ReliveTypes::eGasCountDown:
            if (mHealth > FP_FromInteger(0))
            {
                if (ForceDownIfHoisting_44BA30())
                {
                    return true;
                }

                if (IsStanding_449D30())
                {
                    mbMotionChanged = true;
                    mHealth = FP_FromInteger(0);
                    mCurrentMotion = eAbeMotions::Motion_129_PoisonGasDeath;
                    field_124_timer = 1;
                }
                else
                {
                    if (mCurrentMotion != eAbeMotions::Motion_71_Knockback_455090 && mCurrentMotion != eAbeMotions::Motion_72_KnockbackGetUp_455340)
                    {
                        ToKnockback_44E700(1, 1);
                        mbMotionChanged = true;
                    }
                }
            }
            break;

        case ReliveTypes::eDrill:
        {
            if (mHealth <= FP_FromInteger(0))
            {
                return false;
            }

            mbMotionChanged = true;
            mHealth = FP_FromInteger(0);
            ToKnockback_44E700(1, 1);

            mRGB.SetRGB(30, 30, 30);

            relive_new Gibs(GibType::eAbe,
                mXPos,
                mYPos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale(),
                false);

            relive_new Gibs(GibType::eAbe,
                mXPos,
                mYPos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale(),
                false);

            GetAnimation().SetRender(false);
        }
        break;

        case ReliveTypes::eElectricWall:
            Mudokon_SFX(MudSounds::eOops_14, 0, 0, this);
            break;

        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eMineCar:
        case ReliveTypes::eAirExplosion:
            if (GetAnimation().GetRender())
            {
                if (mHealth > FP_FromInteger(0))
                {
                    mbGotShot = true;
                    mKnockdownMotion = eAbeMotions::Motion_71_Knockback_455090;
                    mNextMotion = eAbeMotions::Motion_71_Knockback_455090;
                }

                mHealth = FP_FromInteger(0);

                mRGB.SetRGB(30, 30, 30);

                relive_new Gibs(GibType::eAbe,
                    mXPos,
                    mYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    GetSpriteScale(),
                    0);
                GetAnimation().SetRender(false);
                GetShadow()->mEnabled = false;
            }
            break;

        case ReliveTypes::eSecurityClaw:
        case ReliveTypes::eSecurityOrb:
            mSay = MudSounds::eAnger_5;
            mAutoSayTimer = MakeTimer(27);
            if (mCurrentMotion != eAbeMotions::Motion_123_LiftGrabIdle && mCurrentMotion != eAbeMotions::Motion_124_LiftUseUp && mCurrentMotion != eAbeMotions::Motion_125_LiftUseDown)
            {
                ToKnockback_44E700(1, 1);
                mbMotionChanged = true;
            }

            // The zap makes Abe drop his stuff everywhere
            for (s32 i = 0; i < mBaseThrowableCount; i++)
            {
                BaseThrowable* pThrowable = Make_Throwable(
                    mXPos,
                    mYPos - FP_FromInteger(30),
                    0);

                // Random explode time ?
                const FP rand1 = FP_FromRaw((Math_NextRandom() - 127) << 11); // TODO: Wat?
                const FP rand2 = (FP_FromDouble(0.03125) * FP_FromRaw(Math_NextRandom())) - FP_FromInteger(2);
                pThrowable->VThrow(rand1, rand2);

                pThrowable->SetSpriteScale(GetSpriteScale());
                pThrowable->SetScale(GetScale());
                pThrowable->VTimeToExplodeRandom(); // Start count down ?
            }
            mBaseThrowableCount = 0;
            break;

        case ReliveTypes::eRockSpawner:
            if (mHealth > FP_FromInteger(0))
            {
                mbMotionChanged = true;
                mHealth = FP_FromInteger(0);

                if (ForceDownIfHoisting_44BA30())
                {
                    return true;
                }
                ToKnockback_44E700(1, 1);
                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
            }
            break;

        case ReliveTypes::eFleech:
            if (mHealth > FP_FromInteger(0))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);

                mHealth -= FP_FromDouble(0.15075); // Yes it has to be this accurate to match

                if (mHealth < FP_FromInteger(0))
                {
                    mHealth = FP_FromInteger(0);
                    Mudokon_SFX(MudSounds::eLaugh_10, 0, 1000, this);
                }

                if (mHealth > FP_FromInteger(0))
                {
                    // The damage sound from a Fleech keeps getting high and higher pitch till death
                    const FP hpRandSoundRange = (FP_FromInteger(1) - gAbe->mHealth) / FP_FromDouble(0.15);
                    const s16 pitchRand = Math_RandomRange(
                        200 * (FP_GetExponent(hpRandSoundRange)),
                        40 * (5 * (FP_GetExponent(hpRandSoundRange)) + 5));

                    Mudokon_SFX(MudSounds::eHurt2_9, 0, pitchRand, this);
                    return true;
                }

                const PSX_RECT bRect = VGetBoundingRect();

                relive_new Blood(mXPos,
                    // Put YPos in the middle of who is getting damaged
                    FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2),
                    // Put the blood on the left or the right depending on where the damage is coming from
                    FP_FromInteger((mXPos - pAliveObj->mXPos < FP_FromInteger(0)) ? -24 : 24),
                    FP_FromInteger(0),
                    GetSpriteScale(),
                    50);

                if (ForceDownIfHoisting_44BA30())
                {
                    return true;
                }

                ToKnockback_44E700(1, 1);
                mbMotionChanged = true;

                if (pAliveObj->mXPos < mXPos && GetAnimation().GetFlipX())
                {
                    mCurrentMotion = eAbeMotions::Motion_101_KnockForward;
                }

                if (pAliveObj->mXPos > mXPos && !(GetAnimation().GetFlipX()))
                {
                    mCurrentMotion = eAbeMotions::Motion_101_KnockForward;
                }

                if (pAliveObj->GetAnimation().GetFlipX())
                {
                    mVelX = GetSpriteScale() * FP_FromDouble(-7.8);
                }
                else
                {
                    mVelX = GetSpriteScale() * FP_FromDouble(7.8);
                }

                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
            }
            break;

        case ReliveTypes::eGreeter:
            if (mHealth <= FP_FromInteger(0))
            {
                break;
            }

            Mudokon_SFX(MudSounds::eHurt2_9, 0, 0, this);
            Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, this);
            mbGotShot = true;
            mKnockdownMotion = eAbeMotions::Motion_101_KnockForward;
            mNextMotion = eAbeMotions::Motion_101_KnockForward;
            mHealth = FP_FromInteger(0);

            if (!ForceDownIfHoisting_44BA30())
            {
                break;
            }
            return true;

        case ReliveTypes::eParamite:
        case ReliveTypes::eScrab:
        case ReliveTypes::eSlog:
            if (mHealth > FP_FromInteger(0))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pFrom);

                mHealth = FP_FromInteger(0);

                const PSX_RECT bRect = VGetBoundingRect();

                relive_new Blood(
                    mXPos,
                    FP_FromInteger(bRect.y + bRect.h) / FP_FromInteger(2),
                    // Put the blood on the left or the right depending on where the damage is coming from
                    (pAliveObj->mVelX <= FP_FromInteger(0)) ? FP_FromInteger(-24) : FP_FromInteger(24),
                    FP_FromInteger(0),
                    GetSpriteScale(),
                    50);

                if (ForceDownIfHoisting_44BA30())
                {
                    return true;
                }

                ToKnockback_44E700(1, 1);
                mbMotionChanged = true;

                if (pAliveObj->mXPos < mXPos)
                {
                    if (!(GetAnimation().GetFlipX()))
                    {
                        mCurrentMotion = eAbeMotions::Motion_101_KnockForward;
                    }
                }

                if (pAliveObj->mXPos > mXPos)
                {
                    if (GetAnimation().GetFlipX())
                    {
                        mCurrentMotion = eAbeMotions::Motion_101_KnockForward;
                    }
                }

                if (pAliveObj->GetAnimation().GetFlipX())
                {
                    mVelX = GetSpriteScale() * FP_FromDouble(-7.8);
                }
                else
                {
                    mVelX = GetSpriteScale() * FP_FromDouble(7.8);
                }

                SfxPlayMono(relive::SoundEffects::KillEffect, 127);

                if (pFrom->Type() != ReliveTypes::eParamite)
                {
                    SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
                }
            }
            break;

        case ReliveTypes::eAbilityRing:
            return false;

        case ReliveTypes::eMudokon:
            if (mHealth > FP_FromInteger(0) && mCurrentMotion != eAbeMotions::Motion_71_Knockback_455090)
            {
                mHealth -= FP_FromDouble(0.07);
                if (mHealth <= FP_FromInteger(0) || (mCurrentMotion != eAbeMotions::Motion_123_LiftGrabIdle && mCurrentMotion != eAbeMotions::Motion_124_LiftUseUp && mCurrentMotion != eAbeMotions::Motion_125_LiftUseDown))
                {
                    ToKnockback_44E700(1, 1);
                    mbMotionChanged = true;
                    if (mHealth <= FP_FromInteger(0))
                    {
                        Mudokon_SFX(MudSounds::eHurt2_9, 0, 1000, this);
                        Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
                        mHealth = FP_FromInteger(0);
                    }
                }
            }
            break;

        case ReliveTypes::eSlamDoor:
            if (mHealth > FP_FromInteger(0))
            {
                ToKnockback_44E700(1, 1);
                VCheckCollisionLineStillValid(5);
                mbMotionChanged = true;
            }
            break;

        case ReliveTypes::eElectrocute:
            GetAnimation().SetRender(false);
            ToDieFinal_458910();
            break;
        case ReliveTypes::eRollingBall:
            if (mHealth > FP_FromInteger(0))
            {
                mbMotionChanged = true;
                mHealth = FP_FromInteger(0);
                if (!ForceDownIfHoisting_44BA30())
                {
                    auto pRollingBall = static_cast<BaseAliveGameObject*>(pFrom);

                    ToKnockback_44E700(1, 1);
                    if (pRollingBall->mXPos < mXPos) // abe is facing left while the rolling ball approaches us from behind
                    {
                        if (!GetAnimation().GetFlipX())
                        {
                            mCurrentMotion = eAbeMotions::Motion_101_KnockForward;
                        }
                    }
                    else if (pRollingBall->mXPos > mXPos) // abe is facing right while the rolling ball approaches us from behind
                    {
                        if (GetAnimation().GetFlipX())
                        {
                            mCurrentMotion = eAbeMotions::Motion_101_KnockForward;
                        }
                    }
                    // if we're facing the rolling ball and it is in front of us,
                    // keep the eAbeMotions::Motion_71_Knockback_455090 motion from the ToKnockback() call

                    if (pRollingBall->mVelX >= FP_FromInteger(0))
                    {
                        mVelX = (GetSpriteScale() * FP_FromDouble(7.8));
                    }
                    else
                    {
                        mVelX = (GetSpriteScale() * FP_FromDouble(-7.8));
                    }

                    SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                }
            }
            break;
        case ReliveTypes::eBullet:
            // NOTE: This was in the default case! The type may not be bullet in there which would corrupt memory or crash
            BulletDamage_44C980(static_cast<Bullet*>(pFrom));
            if (!mbGotShot)
            {
                ret = false;
                mSay = oldSay;
            }
            break;

        default:
            break;
    }

    if (sControlledCharacter->GetPossessed())
    {
        if (mHealth == FP_FromInteger(0))
        {
            sControlledCharacter->VUnPosses();
            // We are the "active" player again
            GiveControlBackToMe_44BA10();
        }
    }

    return ret;
}

void Abe::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    for (; pTlv; pTlv = gPathInfo->TLV_Get_At(
                     pTlv,
                     mXPos,
                     mYPos,
                     mXPos,
                     mYPos))
    {
        if (pTlv->mTlvType == ReliveTypes::eContinuePoint)
        {
            auto pContinuePoint = static_cast<relive::Path_ContinuePoint*>(pTlv);
            if (pContinuePoint->mTlvSpecificMeaning == 0)
            {
                if ((pContinuePoint->mScale != relive::Path_ContinuePoint::Scale::eHalf || GetSpriteScale() == FP_FromInteger(1)) && (pContinuePoint->mScale != relive::Path_ContinuePoint::Scale::eFull || GetSpriteScale() == FP_FromDouble(0.5))
                    && mHealth > FP_FromInteger(0) && !(GetElectrocuted()))
                {
                    pContinuePoint->mTlvSpecificMeaning = 1;
                    mDoQuicksave = true;
                    mSaveFileId = pContinuePoint->mSaveFileId;
                }
            }
        }
        else if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            if (sControlledCharacter->Type() != ReliveTypes::eMineCar || gMap.mCurrentLevel != EReliveLevelIds::eMines)
            {
                Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
                EventBroadcast(Event::kEventNoise, this);
                EventBroadcast(Event::kEventSuspiciousNoise, this);
                EventBroadcast(Event::kEventLoudNoise, this);
                EventBroadcast(Event::kEventSpeaking, this);
                ToDie_4588D0();
            }
        }
        else if (pTlv->mTlvType == ReliveTypes::eResetPath)
        {
            auto pResetSwitchRange = static_cast<relive::Path_ResetPath*>(pTlv);
            if (pResetSwitchRange->mTlvSpecificMeaning == 0 || pResetSwitchRange->mEnabled)
            {
                pResetSwitchRange->mTlvSpecificMeaning = 1;
                if (pResetSwitchRange->mClearIds)
                {
                    for (s16 i = pResetSwitchRange->mFrom; i <= pResetSwitchRange->mTo; i++)
                    {
                        if (i != pResetSwitchRange->mExclude && i > 1)
                        {
                            SwitchStates_Set(i, 0);
                        }
                    }
                }
                if (pResetSwitchRange->mClearObjects)
                {
                    Path::Reset_TLVs(pResetSwitchRange->mPath);
                }
            }
        }
    }
}

BaseAliveGameObject* Abe::FindObjectToPossess_44B7B0()
{
    BaseAliveGameObject* pTargetObj = nullptr;
    BaseAliveGameObject* pInRangeFart = nullptr;
    BaseAliveGameObject* pInRangeGlukkon = nullptr;

    s16 maxDistance = 32767;
    FP lastScale = {};
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetCanBePossessed())
        {
            switch (pObj->Type())
            {
                    // Third priority
                case ReliveTypes::eCrawlingSlig:
                case ReliveTypes::eFlyingSlig:
                case ReliveTypes::eParamite:
                case ReliveTypes::eScrab:
                case ReliveTypes::eSlig:
                    if (pObj->Is_In_Current_Camera() == CameraPos::eCamCurrent_0 && pObj->mHealth > FP_FromInteger(0))
                    {
                        const s16 distance = static_cast<s16>(Math_Distance(
                            FP_GetExponent(mXPos),
                            FP_GetExponent(mYPos),
                            FP_GetExponent(pObj->mXPos),
                            FP_GetExponent(pObj->mYPos)));

                        if (lastScale == GetSpriteScale())
                        {
                            if (pObj->GetSpriteScale() == GetSpriteScale() && distance < maxDistance)
                            {
                                pTargetObj = pObj;
                                maxDistance = distance;
                            }
                        }
                        else if (pObj->GetSpriteScale() == GetSpriteScale())
                        {
                            pTargetObj = pObj;
                            maxDistance = distance;
                            lastScale = pObj->GetSpriteScale();
                        }
                        else if (distance < maxDistance)
                        {
                            pTargetObj = pObj;
                            maxDistance = distance;
                            lastScale = pObj->GetSpriteScale();
                        }
                    }
                    break;

                    // First priority
                case ReliveTypes::eEvilFart:
                    pInRangeFart = pObj;
                    break;

                    // Second priority
                case ReliveTypes::eGlukkon:
                    if (pObj->Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                    {
                        pInRangeGlukkon = pObj;
                    }
                    break;

                default:
                    continue;
            }
        }
    }

    if (pInRangeFart)
    {
        return pInRangeFart;
    }

    if (pInRangeGlukkon)
    {
        return pInRangeGlukkon;
    }

    return pTargetObj;
}

void Abe::Free_Resources_44D420()
{
    if (mBaseGameObjectResArray.mUsedSize)
    {
        if (mBaseGameObjectResArray.ItemAt(0))
        {
            mBaseGameObjectResArray.SetAt(0, nullptr);
        }
    }
}

bool Abe::IsStanding_449D30()
{
    return mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0
        || mCurrentMotion == eAbeMotions::Motion_2_StandingTurn_451830
        || mCurrentMotion == eAbeMotions::Motion_34_DunnoBegin_44ECF0
        || mCurrentMotion == eAbeMotions::Motion_35_DunnoEnd_44ED10
        || mCurrentMotion == eAbeMotions::Motion_11_Speak_45B0A0
        || mCurrentMotion == eAbeMotions::Motion_7_Speak_45B140
        || mCurrentMotion == eAbeMotions::Motion_8_Speak_45B160
        || mCurrentMotion == eAbeMotions::Motion_9_Speak_45B180
        || mCurrentMotion == eAbeMotions::Motion_10_Fart_45B1A0
        || mCurrentMotion == eAbeMotions::Motion_99_LeverUse
        || mCurrentMotion == eAbeMotions::Motion_105_RockThrowStandingThrow
        || mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold
        || mCurrentMotion == eAbeMotions::Motion_106_RockThrowStandingEnd
        || mCurrentMotion == eAbeMotions::Motion_112_Chant
        || mCurrentMotion == eAbeMotions::Motion_113_ChantEnd;
}

void Abe::Free_Shrykull_Resources_45AA90()
{
    mBaseGameObjectResArray.SetAt(25, nullptr);

    mBaseGameObjectResArray.SetAt(26, nullptr);

    mBaseGameObjectResArray.SetAt(27, nullptr);
}


static bool IsSameScaleAsHoist(relive::Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    if (pHoist->mScale == relive::reliveScale::eFull && pObj->GetScale() == Scale::Bg)
    {
        return false;
    }
    else if (pHoist->mScale == relive::reliveScale::eHalf && pObj->GetScale() == Scale::Fg)
    {
        return false;
    }
    return true;
}

static bool IsSameScaleAsEdge(relive::Path_Edge* pEdge, BaseAliveGameObject* pObj)
{
    if (pEdge->mScale == relive::reliveScale::eFull && pObj->GetScale() == Scale::Bg)
    {
        return false;
    }
    else if (pEdge->mScale == relive::reliveScale::eHalf && pObj->GetScale() == Scale::Fg)
    {
        return false;
    }
    return true;
}

static bool IsFacingSameDirectionAsHoist(relive::Path_Hoist* pHoist, BaseAliveGameObject* pObj)
{
    if (pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingLeft && !pObj->GetAnimation().GetFlipX())
    {
        return false;
    }
    else if (pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingRight && pObj->GetAnimation().GetFlipX())
    {
        return false;
    }

    return true;
}

static bool isEdgeGrabbable(relive::Path_Edge* pEdge, BaseAliveGameObject* pObj)
{
    if (pEdge->mGrabDirection == relive::Path_Edge::GrabDirection::eFacingLeft && pObj->GetAnimation().GetFlipX())
    {
        return true;
    }
    else if (pEdge->mGrabDirection == relive::Path_Edge::GrabDirection::eFacingRight && !pObj->GetAnimation().GetFlipX())
    {
        return true;
    }
    else if (pEdge->mGrabDirection == relive::Path_Edge::GrabDirection::eFacingAnyDirection)
    {
        return true;
    }

    return false;
}

void Abe::Motion_0_Idle_44EEB0()
{
    if (Input_IsChanting() && !mPreventChanting)
    {
        if (mRingPulseTimer && mHaveShrykull)
        {
            mCurrentMotion = eAbeMotions::Motion_119_ToShrykull;
        }
        else
        {
            // Go to chanting.
            field_124_timer = MakeTimer(90);
            mCurrentMotion = eAbeMotions::Motion_112_Chant;
            SND_SEQ_PlaySeq(SeqId::MudokonChant1_10, 0, 1);
        }
        field_120_state.raw = 0;
        return;
    }

    // Go to game speak motion.
    const u32 pressed = Input().GetPressed();
    if (pressed & (InputCommands::eChant | InputCommands::eGameSpeak8 | InputCommands::eGameSpeak7 | InputCommands::eGameSpeak6 | InputCommands::eGameSpeak5 | InputCommands::eGameSpeak4 | InputCommands::eGameSpeak3 | InputCommands::eGameSpeak2 | InputCommands::eGameSpeak1))
    {
        mPrevInput = pressed;
        mCurrentMotion = eAbeMotions::Motion_11_Speak_45B0A0;
        return;
    }

    if (Input().IsAnyHeld(InputCommands::eHop))
    {
        // Some strange alternative way of hoisting, hangover from PSX AO Demo?
        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            TryHoist_44ED30();
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_27_HopBegin_4521C0;

            BaseGameObject* pObj = VIntoBirdPortal(2);
            if (pObj)
            {
                mBirdPortalSubState = PortalSubStates::eJumpingInsidePortal_0;
                mBirdPortalId = pObj->mBaseGameObjectId;
            }
            else
            {
                mBirdPortalId = Guid{};
            }
        }
        return;
    }

    if (ToLeftRightMovement_44E340())
    {
        // To turn/walk/sneak/run
        return;
    }

    if (Input().IsAnyHeld(InputCommands::eDown))
    {
        // Check for a lift rope (going down)
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pLiftPoint)
        {
            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
            {
                const FP halfGrid = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                const FP xPosToMidLiftPlatformDistance = (mXPos - liftPlatformXMidPoint) >= FP_FromInteger(0) ? mXPos - liftPlatformXMidPoint : liftPlatformXMidPoint - mXPos;
                if (xPosToMidLiftPlatformDistance < halfGrid)
                {
                    mCurrentMotion = eAbeMotions::Motion_121_LiftGrabBegin;
                    return;
                }
            }
        }

        // Look below for a down hoist
        relive::Path_Hoist* pHoist = static_cast<relive::Path_Hoist*>(gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos) + 16,
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos) + 16,
            ReliveTypes::eHoist));

        if (pHoist)
        {
            if (!IsSameScaleAsHoist(pHoist, this))
            {
                return;
            }

            if (IsFacingSameDirectionAsHoist(pHoist, this))
            {
                // Yeah go down
                mCurrentMotion = eAbeMotions::Motion_66_LedgeDescend_454970;
            }
            else
            {
                if (pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingAnyDirection)
                {
                    // We can hoist down from any side
                    mCurrentMotion = eAbeMotions::Motion_66_LedgeDescend_454970;
                }
                else
                {
                    // Otherwise gotta turn around
                    mNextMotion = eAbeMotions::Motion_66_LedgeDescend_454970;
                    mCurrentMotion = eAbeMotions::Motion_2_StandingTurn_451830;
                }
            }
        }
        else
        {
            // Isn't a hoist so just crouch
            mCurrentMotion = eAbeMotions::Motion_19_StandToCrouch_453DC0;
        }
        return;
    }

    if (Input().IsAnyPressed(InputCommands::eFartOrRoll))
    {
        // Do the fart sound
        Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Fart);

        // Let others hear the fart
        EventBroadcast(Event::kEventNoise, this);
        EventBroadcast(Event::kEventSuspiciousNoise, this);

        if (mHasEvilFart)
        {
            // An evil fart
            mHasEvilFart = false;
            Create_Fart_421D20();
        }
        else
        {
            const FP fartScale = FP_FromDouble(0.5) * GetSpriteScale();
            const FP fartYPos = mYPos - (FP_FromInteger(24) * GetSpriteScale());
            FP fartXPos = {};

            // A normal fart, figure out the direction of Abe's Arse
            if (GetAnimation().GetFlipX())
            {
                fartXPos = mXPos + (FP_FromInteger(12) * GetSpriteScale());
            }
            else
            {
                fartXPos = mXPos - (FP_FromInteger(12) * GetSpriteScale());
            }

            New_Smoke_Particles(fartXPos, fartYPos, fartScale, 3, RGB16{ 32, 128, 32 });
        }

        mCurrentMotion = eAbeMotions::Motion_10_Fart_45B1A0;
        return;
    }

    bool handleDoActionOrThrow = false;
    if (Input().IsAnyHeld(InputCommands::eUp))
    {
        // Check for lift rope.. (going up)
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (pLiftPoint)
        {
            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
            {
                const FP halfGrid = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
                const FP liftPlatformXMidPoint = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                const FP xPosToMidLiftPlatformDistance = FP_Abs(mXPos - liftPlatformXMidPoint);
                if (xPosToMidLiftPlatformDistance < halfGrid)
                {
                    mCurrentMotion = eAbeMotions::Motion_121_LiftGrabBegin;
                    return;
                }
            }
        }

        // Get the first TLV
        relive::Path_TLV* pTlv = gPathInfo->TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        // Handle objects that accept "up"
        while (pTlv)
        {
            switch (pTlv->mTlvType)
            {
                case ReliveTypes::eDoor:
                    if (NearDoorIsOpen_44EE10() && !GetElectrocuted())
                    {
                        BaseAliveGameObjectPathTLV = pTlv;
                        field_120_state.door = AbeDoorStates::eAbeComesIn_0;
                        mCurrentMotion = eAbeMotions::Motion_114_DoorEnter;
                    }
                    else
                    {
                        // Check for pressed + held so that Abe does a dunno only once, otherwise he will
                        // loop the dunno while up button is down.
                        if (Input().IsAnyPressed(InputCommands::eUp))
                        {
                            mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                        }
                    }
                    return;

                case ReliveTypes::eWellLocal:
                {
                    if (GetTeleporting())
                    {
                        break;
                    }

                    // Bail if scale doesn't match
                    relive::Path_WellLocal* pWell = static_cast<relive::Path_WellLocal*>(pTlv);
                    if ((pWell->mScale != relive::reliveScale::eFull || GetSpriteScale() != FP_FromDouble(1.0)) && (pWell->mScale != relive::reliveScale::eHalf || GetSpriteScale() != FP_FromDouble(0.5)))
                    {
                        break;
                    }

                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_78_WellBegin_45C810;
                }
                    return;

                case ReliveTypes::eWellExpress:
                {
                    if (GetTeleporting())
                    {
                        break;
                    }

                    // Bail if scale doesn't match
                    relive::Path_WellBase* pWell = static_cast<relive::Path_WellBase*>(pTlv);
                    if ((pWell->mScale != relive::reliveScale::eFull || GetSpriteScale() != FP_FromDouble(1.0)) && (pWell->mScale != relive::reliveScale::eHalf || GetSpriteScale() != FP_FromDouble(0.5)))
                    {
                        break;
                    }

                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::jMotion_81_WellBegin_45C7F0;
                }
                    return;

                case ReliveTypes::eMovieHandStone:
                case ReliveTypes::eHandStone:
                    BaseAliveGameObjectPathTLV = pTlv;
                    mCurrentMotion = eAbeMotions::Motion_86_HandstoneBegin;
                    field_120_state.stone = StoneStates::eHandstoneBegin_0;
                    return;

                case ReliveTypes::eBoomMachine:
                {
                    auto pMachineButton = static_cast<BoomMachine*>(FindObjectOfType(
                        ReliveTypes::eBoomMachine,
                        mXPos,
                        mYPos - GetSpriteScale() * FP_FromInteger(25)));
                    if (pMachineButton)
                    {
                        pMachineButton->VHandleButton();
                        mCurrentMotion = eAbeMotions::Motion_88_GrenadeMachineUse;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                    }
                }
                break;

                case ReliveTypes::eWorkWheel:
                {
                    bool bCanUseWheel = true;
                    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
                    {
                        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
                        if (!pObj)
                        {
                            break;
                        }

                        if (pObj->Type() == ReliveTypes::eMudokon && pObj->GetScale() == GetScale())
                        {
                            FP xDiff = pObj->mXPos - mXPos;
                            if (xDiff < FP_FromInteger(0))
                            {
                                xDiff = mXPos - pObj->mXPos;
                            }

                            FP gridWidth = ScaleToGridSize(GetSpriteScale());
                            if (xDiff < gridWidth)
                            {
                                FP yDiff = pObj->mYPos - mYPos;
                                if (yDiff < FP_FromInteger(0))
                                {
                                    yDiff = mYPos - pObj->mYPos;
                                }

                                if (yDiff < gridWidth)
                                {
                                    bCanUseWheel = false;
                                    break;
                                }
                            }
                        }
                    }

                    if (bCanUseWheel)
                    {
                        mCurrentMotion = eAbeMotions::Motion_126_TurnWheelBegin;
                        BaseGameObject* pObj_148 = FindObjectOfType(ReliveTypes::eWorkWheel, mXPos, mYPos - (GetSpriteScale() * FP_FromInteger(50)));
                        if (pObj_148)
                        {
                            mWorkWheelId = pObj_148->mBaseGameObjectId;
                        }
                    }
                }
                break;

                case ReliveTypes::eBrewMachine:
                    mCurrentMotion = eAbeMotions::Motion_89_BrewMachineBegin;
                    field_120_state.raw = 0;
                    break;

                default:
                    break;
            }

            // To next TLV (if any)
            pTlv = gPathInfo->TLV_Get_At(
                pTlv,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
        }

        if (!TryEnterMineCar_4569E0())
        {
            if (mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0)
            {
                TryHoist_44ED30();
            }
            handleDoActionOrThrow = true;
        }
    }

    if (!Input().IsAnyHeld(InputCommands::eUp) || handleDoActionOrThrow)
    {
        if ((InputCommands::eThrowItem & pressed) && mCurrentMotion == eAbeMotions::Motion_0_Idle_44EEB0)
        {
            if (mBaseThrowableCount > 0 || gInfiniteThrowables)
            {
                mThrowableId = Make_Throwable(
                                             mXPos,
                                             mYPos - FP_FromInteger(40),
                                             0)
                                             ->mBaseGameObjectId;

                if (!gThrowableIndicatorExists)
                {
                    const FP xOffSet = ((GetAnimation().GetFlipX()) ? FP_FromInteger(15) : FP_FromInteger(-15)) * GetSpriteScale();
                    relive_new ThrowableTotalIndicator(
                        mXPos + xOffSet,
                        mYPos + (GetSpriteScale() * FP_FromInteger(-50)),
                        GetAnimation().GetRenderLayer(),
                        GetAnimation().GetSpriteScale(),
                        mBaseThrowableCount,
                        true);
                }

                mCurrentMotion = eAbeMotions::Motion_104_RockThrowStandingHold;

                if (!gInfiniteThrowables)
                {
                    mBaseThrowableCount--;
                }
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
            }
        }
        else
        {
            if (Input().IsAnyPressed(InputCommands::eDoAction)) // not throwing, maybe pressing up and pressing action, so do action
            {
                mCurrentMotion = HandleDoAction_455BD0();
            }
        }
    }
}

void Abe::Motion_1_WalkLoop_44FBA0()
{
    mPrevInput |= Input().GetHeld();

    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    MoveForward_44E9A0();

    if (mCurrentMotion != eAbeMotions::Motion_1_WalkLoop_44FBA0)
    {
        return;
    }

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9);
    }

    const u32 pressed = Input().GetHeld();

    if (GetAnimation().GetCurrentFrame() != 2 && GetAnimation().GetCurrentFrame() != 11)
    {
        if (GetAnimation().GetCurrentFrame() != 5 && GetAnimation().GetCurrentFrame() != 14)
        {
            return;
        }

        Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);

        MapFollowMe(true);

        if (InputCommands::eRun & pressed)
        {
            // Walk to run
            if (GetAnimation().GetCurrentFrame() == 5)
            {
                mCurrentMotion = eAbeMotions::Motion_49_MidWalkToRun_450200;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_48_WalkToRun_4500A0;
            }
        }
        else if (pressed & InputCommands::eSneak)
        {
            // Walk to sneak
            if (GetAnimation().GetCurrentFrame() != 5)
            {
                mCurrentMotion = eAbeMotions::Motion_43_MidWalkToSneak_450380;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_41_WalkToSneak_450250;
            }
        }
        return;
    }

    // Direction changed
    bool bEndWalking = false;
    if ((mVelX > FP_FromInteger(0) && (InputCommands::eLeft & pressed)) || (mVelX < FP_FromInteger(0) && (InputCommands::eRight & pressed)))
    {
        bEndWalking = true;
    }

    // Hopping
    if (InputCommands::eHop & mPrevInput)
    {
        mNextMotion = eAbeMotions::Motion_28_HopMid_451C50;
        bEndWalking = true;
    }

    if (!bEndWalking)
    {
        if ((InputCommands::eLeft | InputCommands::eRight) & pressed)
        {
            // Check if walking into something
            FP gridSize = {};
            if (GetAnimation().GetFlipX())
            {
                gridSize = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSize = ScaleToGridSize(GetSpriteScale());
            }

            const FP offX = gridSize * FP_FromDouble(1.5);
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), offX) || WallHit(GetSpriteScale() * FP_FromInteger(20), offX))
            {
                bEndWalking = true;
            }
        }
        else
        {
            // No longer trying to walk
            bEndWalking = true;
        }
    }

    if (bEndWalking)
    {
        if (GetAnimation().GetCurrentFrame() == 2)
        {
            mCurrentMotion = eAbeMotions::Motion_5_MidWalkToIdle_450080;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_4_WalkToIdle_44FFC0;
        }
        mPrevInput = 0;
    }
}

void Abe::Motion_2_StandingTurn_451830()
{
    const auto currentFrame = GetAnimation().GetCurrentFrame();
    const u32 pressed = Input().GetHeld();

    if (currentFrame == 4 && (InputCommands::eRun & pressed) && (InputCommands::eRight | InputCommands::eLeft) & pressed)
    {
#if ORIGINAL_GAME_FIXES || ORIGINAL_GAME_FIX_AUTO_TURN
        mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0; // OG Change - Fixes "Auto-Turn" bug
#endif

        mCurrentMotion = eAbeMotions::Motion_61_TurnToRun_456530;
        GetAnimation().ToggleFlipX();

        if (GetAnimation().GetFlipX())
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }
        else
        {
            mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
        }
    }
    else
    {
        // Play "turning" sound effect
        if (currentFrame == 0)
        {
            Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
        }

        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().ToggleFlipX();
            if (mNextMotion != eAbeMotions::Motion_0_Idle_44EEB0) // Note: used to be if (mNextMotion)
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
            }
            else
            {
                ToIdle_44E6B0();
            }
        }
    }
}

void Abe::Motion_3_Fall_459B60()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (GetSpriteScale() * field_8_x_vel_slow_by);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (GetSpriteScale() * field_8_x_vel_slow_by);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;
    const s32 bCollision = InAirCollision(&pPathLine, &hitX, &hitY, FP_FromDouble(1.80));
    SetActiveCameraDelayedFromDir();

    // Are we falling into a local well?
    BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellLocal);

    if (!BaseAliveGameObjectPathTLV)
    {
        // No, are we falling into an express well?
        BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellExpress);
    }

    // Handle falling into a well
    if (BaseAliveGameObjectPathTLV)
    {
        if (mHealth > FP_FromInteger(0))
        {
            if (BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eWellLocal || BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eWellExpress)
            {
                // The well must be on the same scale/layer
                relive::Path_WellBase* pWellBase = static_cast<relive::Path_WellBase*>(BaseAliveGameObjectPathTLV);
                if ((pWellBase->mScale == relive::reliveScale::eFull && GetSpriteScale() == FP_FromInteger(1))
                    || (pWellBase->mScale == relive::reliveScale::eHalf && GetSpriteScale() == FP_FromDouble(0.5)))
                {
                    mCurrentMotion = eAbeMotions::Motion_75_JumpIntoWell_45C7B0;
                    return;
                }
            }
        }
    }

    if (bCollision)
    {
        switch (pPathLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                mXPos = hitX;
                mYPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                BaseAliveGameObjectCollisionLine = pPathLine;
                MapFollowMe(true);
                field_124_timer = MakeTimer(30);

                // See if there is a soft landing at our feet (given we known we just hit the floor)
                relive::Path_SoftLanding* pSoftLanding = static_cast<relive::Path_SoftLanding*>(gPathInfo->VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eSoftLanding));

                if (pSoftLanding)
                {
                    if (!SwitchStates_Get(static_cast<s16>(pSoftLanding->mSwitchId)))
                    {
                        pSoftLanding = nullptr;
                    }
                }

                if (mLandSoftly
                    || (pSoftLanding && mHealth > FP_FromInteger(0))                                   // If we are dead soft landing won't save us
                    || ((mYPos - BaseAliveGameObjectLastLineYPos) < (GetSpriteScale() * FP_FromInteger(180)) // Check we didn't fall far enough to be killed
                        && (mHealth > FP_FromInteger(0) || gAbeInvincible)))                   //TODO possibly OG bug: those conditions should probably be grouped the following way: ((A || B || C ) && D)
                {
                    mCurrentMotion = eAbeMotions::Motion_16_LandSoft_45A360;
                }
                else
                {
                    // Slam into the floor and break all your bones
                    mHealth = FP_FromInteger(0);
                    mCurrentMotion = eAbeMotions::Motion_84_FallLandDie_45A420;
                    mRegenHealthTimer = MakeTimer(900);
                }

                mPreviousMotion = eAbeMotions::Motion_3_Fall_459B60;

                PSX_Point xy{FP_GetExponent(mXPos - FP_FromInteger(10)), FP_GetExponent(mYPos - FP_FromInteger(10))};
                PSX_Point wh{FP_GetExponent(mXPos + FP_FromInteger(10)), FP_GetExponent(mYPos + FP_FromInteger(10))};
                OnCollisionWith(xy, wh, gPlatformsArray);
            }
            break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mXPos = hitX;
                mYPos = hitY;
                ToKnockback_44E700(1, 1);
                break;
        }
        return;
    }

    if (mHealth <= FP_FromInteger(0))
    {
        return;
    }

    // Look down 75 for an edge
    relive::Path_Edge* pEdge = static_cast<relive::Path_Edge*>(gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos - (GetSpriteScale() * FP_FromInteger(75))),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eEdge));

    bool tryToHang = false;
    if (pEdge)
    {
        if (pEdge->mCanGrab && IsSameScaleAsEdge(pEdge, this) && (isEdgeGrabbable(pEdge, this)))
        {
            tryToHang = true;
        }
        BaseAliveGameObjectPathTLV = pEdge;
    }
    else // Didn't find and edge to grab so check if falling onto a hoist
    {
        // Look down 20 for a hoist
        relive::Path_Hoist* pHoist = static_cast<relive::Path_Hoist*>(gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(20)),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos - GetSpriteScale() * FP_FromInteger(20)),
            ReliveTypes::eHoist));

        if (pHoist)
        {
            if (IsFacingSameDirectionAsHoist(pHoist, this) && IsSameScaleAsHoist(pHoist, this))
            {
                tryToHang = true;
            }
        }

        BaseAliveGameObjectPathTLV = pHoist;
    }

    if (tryToHang)
    {
        if (mVelX == FP_FromInteger(0))
        {
            return;
        }

        mXPos = FP_FromInteger((BaseAliveGameObjectPathTLV->mTopLeftX + BaseAliveGameObjectPathTLV->mBottomRightX) / 2);

        MapFollowMe(true);

        if (!gCollisions->Raycast(
                mXPos,
                FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY - 10), // TODO: Negative ??
                mXPos,
                FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY + 10),
                &pPathLine,
                &hitX,
                &hitY,
                GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
        {
            return;
        }

        mYPos = hitY;
        BaseAliveGameObjectCollisionLine = pPathLine;
        mVelY = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);
        if (BaseAliveGameObjectPathTLV->mTlvType != ReliveTypes::eHoist || (FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY - 1 * BaseAliveGameObjectPathTLV->mTopLeftY)) >= (GetSpriteScale() * FP_FromInteger(70)))
        {
            mCurrentMotion = eAbeMotions::Motion_69_LedgeHangWobble_454EF0;
            GetShadow()->mShadowAtBottom = true;
        }
        else
        {
            mReturnToPreviousMotion = true;
            mPreviousMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
            mBaseAliveGameObjectLastAnimFrame = 12;
            GetShadow()->mShadowAtBottom = true;
        }
    }
}

void Abe::Motion_4_WalkToIdle_44FFC0()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    MoveForward_44E9A0();

    if (GetAnimation().GetCurrentFrame())
    {
        if (GetAnimation().GetIsLastFrame())
        {
            MapFollowMe(true);

            if (mNextMotion == eAbeMotions::Motion_28_HopMid_451C50)
            {
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
                mCurrentMotion = eAbeMotions::Motion_27_HopBegin_4521C0;
                BaseGameObject* pObj = VIntoBirdPortal(2);
                if (pObj)
                {
                    mBirdPortalSubState = PortalSubStates::eJumpingInsidePortal_0;
                    mBirdPortalId = pObj->mBaseGameObjectId;
                }
            }
            else
            {
                ToIdle_44E6B0();
            }
        }
    }
    else
    {
        Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 32767, this);
    }
}

void Abe::Motion_5_MidWalkToIdle_450080()
{
    // TODO Jump func omitted jState_WalkEndLeftFoot_40459
    Motion_4_WalkToIdle_44FFC0();
}

void Abe::Motion_6_WalkBegin_44FEE0()
{
    mPrevInput |= Input().GetHeld();

    // They hear me walking, they hating
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_7_Speak_45B140()
{
    // TODO: Note jMotion_11_Speak_40388C omitted
    Motion_11_ToSpeak_45B0A0();
}

void Abe::Motion_8_Speak_45B160()
{
    // TODO: Note jMotion_11_Speak_40388C omitted
    Motion_11_ToSpeak_45B0A0();
}

void Abe::Motion_9_Speak_45B180()
{
    // TODO: Note jMotion_11_Speak_40388C omitted
    Motion_11_ToSpeak_45B0A0();
}

void Abe::Motion_10_Fart_45B1A0()
{
    // TODO: Note jMotion_11_Speak_40388C omitted
    Motion_11_ToSpeak_45B0A0();
}

void Abe::Motion_11_ToSpeak_45B0A0()
{
    mPrevInput |= Input().GetPressed();
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = DoGameSpeak_45AB70(mPrevInput);
        if (mCurrentMotion == eAbeMotions::None_m1)
        {
            ToIdle_44E6B0();
        }
        else
        {
            if (mCurrentMotion != eAbeMotions::Motion_62_Punch_454750)
            {
                EventBroadcast(Event::kEventSpeaking, this);
            }
            mbMotionChanged = true;
        }
        mPrevInput = 0;
    }
}

void Abe::Motion_12_Null_4569C0()
{
}

void Abe::Motion_13_HoistBegin_452B20()
{
    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        const FP velY = GetSpriteScale() * FP_FromInteger(-8);
        mVelY = velY;
        mYPos += velY;
        VOnTrapDoorOpen();
        mCurrentMotion = eAbeMotions::Motion_14_HoistIdle_452440;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_14_HoistIdle_452440()
{
    //sObjectIds_5C1B70.Find_449CF0(mPullRingRopeId); // NOTE: Return never used
    BaseGameObject* pPlatform = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    if (Is_Celling_Above_44E8D0())
    {
        ToKnockback_44E700(1, 1);
        return;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
    SetActiveCameraDelayedFromDir();

    if (bCollision)
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                mXPos = hitX;
                mYPos = FP_NoFractional(hitY + FP_FromDouble(0.5));

                MapFollowMe(true);

                BaseAliveGameObjectCollisionLine = pLine;

                mCurrentMotion = eAbeMotions::Motion_15_HoistLand_452BA0;
                mPreviousMotion = eAbeMotions::Motion_14_HoistIdle_452440;

                OnCollisionWith(
                    {FP_GetExponent(mXPos), FP_GetExponent(mYPos)},
                    {FP_GetExponent(mXPos), FP_GetExponent((mYPos + FP_FromInteger(5)))},
                    gPlatformsArray);
                break;
            }
        }
        return;
    }

    PullRingRope* pPullRope = GetPullRope();
    if (pPullRope)
    {
        if (pPullRope->VPull(this))
        {
            mCurrentMotion = eAbeMotions::Motion_70_RingRopePullHang_455AF0;
            mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
            mPullRingRopeId = pPullRope->mBaseGameObjectId;
            return;
        }
        mPullRingRopeId = Guid{};
    }

    relive::Path_Hoist* pHoist = static_cast<relive::Path_Hoist*>(gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eHoist));
    BaseAliveGameObjectPathTLV = pHoist;

    if (mVelY < FP_FromInteger(0))
    {
        // Going down and no hoist, can't do anything
        if (!pHoist)
        {
            return;
        }

        // Hoist is too far away
        if (FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY + -BaseAliveGameObjectPathTLV->mTopLeftY) > (GetSpriteScale() * FP_FromInteger(90)) || GetAnimation().GetCurrentFrame())
        {
            return;
        }
    }

    if (pHoist)
    {
        if (IsSameScaleAsHoist(pHoist, this) && (IsFacingSameDirectionAsHoist(pHoist, this) || pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingAnyDirection))
        {
            if (pHoist->mHoistType == relive::Path_Hoist::Type::eOffScreen)
            {
                if (gMap.SetActiveCameraDelayed(MapDirections::eMapTop_2, this, -1))
                {
                    PSX_Prevent_Rendering();
                    mCurrentMotion = eAbeMotions::Motion_68_ToOffScreenHoist_454B80;
                    return;
                }

                Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);

                if (FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY - 1 * BaseAliveGameObjectPathTLV->mTopLeftY)
                    >= GetSpriteScale() * FP_FromInteger(70))
                {
                    mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
                }
                else
                {
                    mReturnToPreviousMotion = true;
                    mPreviousMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
                    mBaseAliveGameObjectLastAnimFrame = 12;
                }
                mYPos -= GetSpriteScale() * FP_FromInteger(75);
                GetShadow()->mShadowAtBottom = true;
            }
            else
            {
                Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);
                if (FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY - 1 * BaseAliveGameObjectPathTLV->mTopLeftY) >= GetSpriteScale() * FP_FromInteger(70))
                {
                    mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
                }
                else
                {
                    mReturnToPreviousMotion = true;
                    mPreviousMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
                    mBaseAliveGameObjectLastAnimFrame = 12;
                }
            }

            if (gCollisions->Raycast(
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY - 10),
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY + 10),
                    &pLine,
                    &hitX,
                    &hitY,
                    GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                mVelY = FP_FromInteger(0);
                if (pPlatform)
                {
                    if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || 
                        BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                    {
                        OnCollisionWith(
                            {FP_GetExponent(mXPos), FP_GetExponent(mYPos)},
                            {FP_GetExponent(mXPos), FP_GetExponent(mYPos + FP_FromInteger(5))},
                            gPlatformsArray);
                    }
                }
                GetShadow()->mShadowAtBottom = true;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_14_HoistIdle_452440;
            }
        }
    }
    else
    {
        BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

void Abe::Motion_15_HoistLand_452BA0()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (mPreviousMotion == eAbeMotions::Motion_3_Fall_459B60)
        {
            Environment_SFX(EnvironmentSfx::eLandingSoft_5, 0, 32767, this);
        }
        else
        {
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        // Landing and standing
        ToIdle_44E6B0();
    }
}

void Abe::Motion_16_LandSoft_45A360()
{
    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (!mLandSoftly)
        {
            // Hitting the floor events.
            EventBroadcast(Event::kEventNoise, this);
            EventBroadcast(Event::kEventSuspiciousNoise, this);
        }

        mLandSoftly = false;

        // Hitting the floor sounds.
        if (mPreviousMotion == eAbeMotions::Motion_3_Fall_459B60)
        {
            Environment_SFX(EnvironmentSfx::eLandingSoft_5, 0, 0x7FFF, this);
        }
        else
        {
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 0x7FFF, this);
        }

        if (sControlledCharacter != this)
        {
            // If Abe is controlling something else then must be standing and chanting.
            mCurrentMotion = eAbeMotions::Motion_112_Chant;
        }

        if (Input().IsAnyHeld((InputCommands::eRight | InputCommands::eLeft)))
        {
            ToLeftRightMovement_44E340();
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_17_CrouchIdle_456BC0()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        mCurrentMotion = eAbeMotions::Motion_98_RollOffEdge;
        return;
    }

    mPrevInput = 0;
    mReleasedButtons = 0;

    // Crouching game speak
    if (CrouchingGameSpeak_453E10())
    {
        mPrevInput = 0;
        EventBroadcast(Event::kEventSpeaking, this);
        return;
    }

    // Hit bombs/pick up items ?
    if (Input().IsAnyPressed(InputCommands::eDoAction))
    {
        if (!(Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight)))
        {
            FP gridSize = {};
            if (GetAnimation().GetFlipX())
            {
                gridSize = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSize = ScaleToGridSize(GetSpriteScale());
            }
            PickUpThrowabe_Or_PressBomb_454090(gridSize + mXPos, FP_GetExponent(mYPos - FP_FromInteger(5)), 0);
        }
    }

    const u32 pressed = Input().GetPressed();

    // Crouching throw stuff
    if (InputCommands::eThrowItem & pressed
        && mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0
        && (mBaseThrowableCount > 0 || gInfiniteThrowables))
    {
        mThrowableId = Make_Throwable(mXPos, mYPos - FP_FromInteger(40), 0)->mBaseGameObjectId;
        if (!gThrowableIndicatorExists)
        {
            const FP yOff = mYPos + (GetSpriteScale() * FP_FromInteger(-30));
            const FP xOff = GetSpriteScale() * (GetAnimation().GetFlipX() ? FP_FromInteger(-10) : FP_FromInteger(10));
            relive_new ThrowableTotalIndicator(
                mXPos + xOff,
                yOff,
                GetAnimation().GetRenderLayer(),
                GetAnimation().GetSpriteScale(),
                mBaseThrowableCount,
                1);
        }

        mCurrentMotion = eAbeMotions::Motion_107_RockThrowCrouchingHold;

        if (!gInfiniteThrowables)
        {
            mBaseThrowableCount--;
        }
    }
    else
    {
        // Try to stand up
        if (pressed & InputCommands::eUp && !Is_Celling_Above_44E8D0())
        {
            mCurrentMotion = eAbeMotions::Motion_18_CrouchToStand_454600;
            return;
        }

        // Crouching farts
        if (Input().IsAnyPressed(InputCommands::eFartOrRoll))
        {
            gEventSystem->PushEvent(GameSpeakEvents::eAbe_Fart);

            Mudokon_SFX(MudSounds::eFart_7, 0, 0, this);

            if (mHasEvilFart)
            {
                mHasEvilFart = false;
                Create_Fart_421D20();

                if (mBaseGameObjectResArray.ItemAt(22))
                {
                    mBaseGameObjectResArray.SetAt(22, nullptr);
                }
            }
            else
            {
                const FP scale = GetSpriteScale() * FP_FromDouble(0.5);
                const FP ypos = mYPos - (FP_FromInteger(6) * GetSpriteScale());
                FP xpos = {};
                if (GetAnimation().GetFlipX())
                {
                    xpos = mXPos + (FP_FromInteger(10) * GetSpriteScale());
                }
                else
                {
                    xpos = mXPos - (FP_FromInteger(10) * GetSpriteScale());
                }
                New_Smoke_Particles(xpos, ypos, scale, 3, RGB16{ 32, 128, 32 });
            }

            mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        }
        else
        {
            // Crouching turns
            if (Input().IsAnyHeld(InputCommands::eRight))
            {
                if (GetAnimation().GetFlipX())
                {
                    mCurrentMotion = eAbeMotions::Motion_37_CrouchTurn_454390;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_22_RollBegin_4539A0;
                    mReleasedButtons = 0;
                }
            }

            if (Input().IsAnyHeld(InputCommands::eLeft))
            {
                if (GetAnimation().GetFlipX())
                {
                    mCurrentMotion = eAbeMotions::Motion_22_RollBegin_4539A0;
                    mReleasedButtons = 0;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_37_CrouchTurn_454390;
                }
            }
        }
    }
}

void Abe::Motion_18_CrouchToStand_454600()
{
    if (GetAnimation().GetCurrentFrame() == 3)
    {
        if (Input().IsAnyHeld((InputCommands::eRight | InputCommands::eLeft)))
        {
            ToLeftRightMovement_44E340();
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
        mPrevInput = 0;
    }
}

void Abe::Motion_19_StandToCrouch_453DC0()
{
    if (GetAnimation().GetIsLastFrame())
    {
        PickUpThrowabe_Or_PressBomb_454090(mXPos, FP_GetExponent(mYPos - FP_FromInteger(5)), 1);
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
    }
}

// TODO: End crouching game speak/fart?
void Abe::Motion_20_CrouchSpeak_454550()
{
    mPrevInput |= Input().GetPressed();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;

        CrouchingGameSpeak_453E10();

        if (mCurrentMotion == eAbeMotions::Motion_20_CrouchSpeak_454550 || mCurrentMotion == eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0)
        {
            EventBroadcast(Event::kEventSpeaking, this);
        }
        mPrevInput = 0;
    }
}

void Abe::jMotion_21_ToCrouchSpeak_4545E0()
{
    // Note j_401389 omitted
    Motion_20_CrouchSpeak_454550();
}

void Abe::Motion_22_RollBegin_4539A0()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
    }

    const FP xpos = GetSpriteScale() * FP_FromInteger(20);
    if (WallHit(xpos, mVelX))
    {
        ToKnockback_44E700(1, 1);
        mCurrentMotion = eAbeMotions::Motion_74_RollingKnockback_455290;
    }
    else
    {
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_23_RollLoop_453A90;
        }
        MoveForward_44E9A0();
    }
}

void Abe::Motion_23_RollLoop_453A90()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    mReleasedButtons |= Input().GetReleased();

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback_44E700(1, 1);
        mCurrentMotion = eAbeMotions::Motion_74_RollingKnockback_455290;
    }
    else
    {
        MoveForward_44E9A0();

        if (mCurrentMotion == eAbeMotions::Motion_23_RollLoop_453A90)
        {
            if (GetAnimation().GetCurrentFrame() == 1 || GetAnimation().GetCurrentFrame() == 5 || GetAnimation().GetCurrentFrame() == 9)
            {
                if (!Input().IsAnyHeld(InputCommands::eRun)
                    || Input().IsAnyHeld(InputCommands::eFartOrRoll)
                    || Is_Celling_Above_44E8D0()
                    || mRollingMotionTimer + 9 >= static_cast<s32>(sGnFrame))
                {
                    // TODO: See AE func - likely OG issue here of not checking for the roll key
                    if (mReleasedButtons)
                    {
                        if (!Is_Celling_Above_44E8D0() && mRollingMotionTimer + 9 < static_cast<s32>(sGnFrame))
                        {
                            ToLeftRightMovement_44E340();
                            mReleasedButtons = 0;
                        }
                    }
                }
                else
                {
                    ToLeftRightMovement_44E340();
                    mPrevInput = 0;
                }
            }
            else if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 8)
            {
                MapFollowMe(true);

                if ((mVelX > FP_FromInteger(0) && !Input().IsAnyHeld(InputCommands::eRight)) || (mVelX < FP_FromInteger(0) && !Input().IsAnyHeld(InputCommands::eLeft)))
                {
                    mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
                    mVelX = FP_FromInteger(0);
                }
            }

            if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 6)
            {
                Environment_SFX(EnvironmentSfx::eRollingNoise_8, 0, 32767, this);
            }
        }
    }
}

//TODO: probably unused?
void Abe::Motion_24_453D00()
{
    LOG_WARNING("never expected Motion_24_453D00 (roll loop end) to be called");
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);
    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback_44E700(1, 1);
        mCurrentMotion = eAbeMotions::Motion_74_RollingKnockback_455290;
    }
    else
    {
        MoveForward_44E9A0();
        if (mCurrentMotion == eAbeMotions::Motion_24_453D00)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(true);
                mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
                mVelX = FP_FromInteger(0);
            }
        }
    }
}

void Abe::Motion_25_RunSlideStop_451330()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveWithVelocity_450FA0(FP_FromDouble(0.375));

        if (mCurrentMotion == eAbeMotions::Motion_25_RunSlideStop_451330 && !RunTryEnterWell_451060() && !RunTryEnterDoor_451220())
        {
            if (GetAnimation().GetCurrentFrame() != 15)
            {
                MapFollowMe(false);
            }

            const u32 held = Input().GetHeld();
            if (GetAnimation().GetCurrentFrame() >= 9)
            {
                if (GetAnimation().GetCurrentFrame() == 15)
                {
                    Environment_SFX(EnvironmentSfx::eSlideStop_0, 0, 32767, this);
                    MapFollowMe(true);

                    if (!ToLeftRightMovement_44E340())
                    {
                        ToIdle_44E6B0();
                    }
                }
            }
            else if ((GetAnimation().GetFlipX() && (InputCommands::eRight & held)) || (!GetAnimation().GetFlipX() && (InputCommands::eLeft & held)))
            {
                mReturnToPreviousMotion = true;
                mPreviousMotion = eAbeMotions::Motion_26_RunTurn_451500;
                mBaseAliveGameObjectLastAnimFrame = GetAnimation().GetCurrentFrame();
            }
        }
    }
}

void Abe::Motion_26_RunTurn_451500()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveWithVelocity_450FA0(FP_FromDouble(0.375));
        if (mCurrentMotion == eAbeMotions::Motion_26_RunTurn_451500
            && !RunTryEnterWell_451060()
            && !RunTryEnterDoor_451220())
        {
            if (GetAnimation().GetIsLastFrame() || (MapFollowMe(false) && GetAnimation().GetIsLastFrame()))
            {
                MapFollowMe(true);

                if (GetAnimation().GetFlipX())
                {
                    if (Input().IsAnyHeld(InputCommands::eRun))
                    {
                        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
                        mCurrentMotion = eAbeMotions::Motion_52_RunTurnToRun_451710;
                    }
                    else
                    {
                        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9);
                        mCurrentMotion = eAbeMotions::Motion_53_RunTurnToWalk_451800;
                    }
                }
                else
                {
                    if (Input().IsAnyHeld(InputCommands::eRun))
                    {
                        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                        mCurrentMotion = eAbeMotions::Motion_52_RunTurnToRun_451710;
                    }
                    else
                    {
                        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                        mCurrentMotion = eAbeMotions::Motion_53_RunTurnToWalk_451800;
                    }
                }
            }
        }
    }
}

void Abe::Motion_27_HopBegin_4521C0()
{
    if (GetAnimation().GetCurrentFrame() == 9)
    {
        // Change velocity at this frame
        const FP velX = GetSpriteScale() * (GetAnimation().GetFlipX() ? FP_FromInteger(-17) : FP_FromInteger(17));

        mVelX = velX;

        if (mBirdPortalId == Guid{})
        {
            // Check for hopping into a wall
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
            {
                EventBroadcast(Event::kEventNoise, this);
                EventBroadcast(Event::kEventSuspiciousNoise, this);
                mVelX = FP_FromInteger(0);
                ToKnockback_44E700(1, 1);
                return;
            }
        }
        mXPos += mVelX;
        SetActiveCameraDelayedFromDir();
    }

    if (GetAnimation().GetIsLastFrame())
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        const FP velX = GetSpriteScale() * (GetAnimation().GetFlipX() ? FP_FromDouble(-13.57) : FP_FromDouble(13.57));
        mVelX = velX;
        mXPos += velX;

        const FP velY = GetSpriteScale() * FP_FromDouble(-2.7);
        mVelY = velY;
        mYPos += velY;

        VOnTrapDoorOpen();

        mCurrentMotion = eAbeMotions::Motion_28_HopMid_451C50;
        BaseAliveGameObjectCollisionLine = nullptr;

        if (mBirdPortalId == Guid{})
        {
            BaseGameObject* pObj = VIntoBirdPortal(2);
            if (pObj)
            {
                mBirdPortalSubState = PortalSubStates::eJumpingInsidePortal_0;
                mBirdPortalId = pObj->mBaseGameObjectId;
            }
        }
    }
}

void Abe::Motion_28_HopMid_451C50()
{
    if (mBirdPortalId != Guid{})
    {
        IntoPortalStates_451990();
        return;
    }

    // Hopped into a wall?
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        EventBroadcast(Event::kEventNoise, this);
        EventBroadcast(Event::kEventSuspiciousNoise, this);
        mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
        ToKnockback_44E700(1, 1);
        return;
    }

    // See if we've hit a path line floor type
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.80));
    SetActiveCameraDelayedFromDir();

    if (bCollision)
    {
        EventBroadcast(Event::kEventNoise, this);
        EventBroadcast(Event::kEventSuspiciousNoise, this);

        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);
                BaseAliveGameObjectCollisionLine = pLine;
                mXPos = hitX;
                mYPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);

                OnCollisionWith(
                    {FP_GetExponent(mXPos), FP_GetExponent(mYPos)},
                    {FP_GetExponent(mXPos), FP_GetExponent(mYPos + FP_FromInteger(5))},
                    gPlatformsArray);

                MapFollowMe(true);
                mCurrentMotion = eAbeMotions::Motion_29_HopLand_4523D0;
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
                return;
        }
    }
    else
    {
        // Dead so can't continue
        if (mHealth <= FP_FromInteger(0))
        {
            return;
        }

        // Check for an edge
        relive::Path_Edge* pEdgeTlv = static_cast<relive::Path_Edge*>(gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos - (GetSpriteScale() * FP_FromInteger(75))),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eEdge));

        BaseAliveGameObjectPathTLV = pEdgeTlv;

        if (pEdgeTlv && pEdgeTlv->mCanGrab && IsSameScaleAsEdge(pEdgeTlv, this) && ((isEdgeGrabbable(pEdgeTlv, this) && mVelX != FP_FromInteger(0))))
        {
            mXPos = FP_FromInteger((pEdgeTlv->mTopLeftX + pEdgeTlv->mBottomRightX) / 2);

            MapFollowMe(true);

            if (gCollisions->Raycast(
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY - 10),
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY + 10),
                    &pLine,
                    &hitX,
                    &hitY,
                    GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                mCurrentMotion = eAbeMotions::Motion_69_LedgeHangWobble_454EF0;
                GetShadow()->mShadowAtBottom = true;
            }
        }
    }

    if (!(GetAnimation().GetIsLastFrame()))
    {
        return;
    }

    if (mCurrentMotion != eAbeMotions::Motion_28_HopMid_451C50)
    {
        return;
    }

    mVelX = FP_FromRaw(mVelX.fpValue / 2);

    if (GetAnimation().GetFlipX())
    {
        mXPos = (GetSpriteScale() * FP_FromInteger(5)) + mXPos;
    }
    else
    {
        mXPos = mXPos - (GetSpriteScale() * FP_FromInteger(5));
    }

    mYPos += GetSpriteScale() * FP_FromInteger(2);

    field_8_x_vel_slow_by = FP_FromDouble(0.55);
    mCurrentMotion = eAbeMotions::Motion_96_HopToFall;
    mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
}

void Abe::Motion_29_HopLand_4523D0()
{
    if (GetAnimation().GetCurrentFrame() == 2
        && Input().IsAnyHeld(InputCommands::eHop))
    {
        mReturnToPreviousMotion = true;
        mPreviousMotion = eAbeMotions::Motion_27_HopBegin_4521C0;
        mBaseAliveGameObjectLastAnimFrame = 5;
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_30_RunJumpBegin_4532E0()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    // Check if about to jump into a wall
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(FP_FromInteger(10), mVelX)) // TODO: OG bug, why no scaling?
    {
        ToKnockback_44E700(1, 1);
    }
    else
    {
        mXPos += mVelX;

        if (GetAnimation().GetCurrentFrame() == 0)
        {
            Environment_SFX(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
        }

        if (GetAnimation().GetIsLastFrame())
        {
            BaseAliveGameObjectLastLineYPos = mYPos;

            const FP velX = GetAnimation().GetFlipX() ? FP_FromDouble(-7.6) : FP_FromDouble(7.6);
            mVelX = GetSpriteScale() * velX;

            const FP velY = GetSpriteScale() * FP_FromDouble(-9.6);
            mVelY = velY;
            mYPos += velY;

            VOnTrapDoorOpen();

            mCurrentMotion = eAbeMotions::Motion_31_RunJumpMid_452C10;
            BaseAliveGameObjectCollisionLine = nullptr;
        }
    }
}

void Abe::Motion_31_RunJumpMid_452C10()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);
    if (mBirdPortalId != Guid{})
    {
        IntoPortalStates_451990();
        return;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(FP_FromInteger(10), mVelX) || Is_Celling_Above_44E8D0())
    {
        mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
        ToKnockback_44E700(1, 1);
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.80));

    SetActiveCameraDelayedFromDir();

    if (bCollision)
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                mCurrentMotion = eAbeMotions::Motion_32_RunJumpLand_453460;
                mXPos = hitX;
                mYPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                if (pLine->mLineType == eLineTypes::eDynamicCollision_32)
                {
                    OnCollisionWith(
                        {FP_GetExponent(mXPos), FP_GetExponent(mYPos)},
                        {FP_GetExponent(mXPos), FP_GetExponent(mYPos + FP_FromInteger(5))},
                        gPlatformsArray);
                }
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
                return;
        }
    }
    else
    {
        relive::Path_Hoist* pHoist = static_cast<relive::Path_Hoist*>(gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos - mVelX),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos - mVelX),
            FP_GetExponent(mYPos),
            ReliveTypes::eHoist));

        BaseAliveGameObjectPathTLV = pHoist;

        bool checkCollision = false;
        if (pHoist)
        {
            BaseAliveGameObjectPathTLV = pHoist;

            if (IsSameScaleAsHoist(pHoist, this) && (IsFacingSameDirectionAsHoist(pHoist, this) || pHoist->mGrabDirection == relive::Path_Hoist::GrabDirection::eFacingAnyDirection) && pHoist->mHoistType != relive::Path_Hoist::Type::eOffScreen)
            {
                checkCollision = true;
            }
        }
        else
        {
            relive::Path_Edge* pEdgeTlv = static_cast<relive::Path_Edge*>(gPathInfo->VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos - (GetSpriteScale() * FP_FromInteger(60))),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEdge));

            BaseAliveGameObjectPathTLV = pEdgeTlv;

            if (pEdgeTlv && pEdgeTlv->mCanGrab)
            {
                if (IsSameScaleAsEdge(pEdgeTlv, this) && (isEdgeGrabbable(pEdgeTlv, this)))
                {
                    checkCollision = true;
                }
            }
        }

        if (checkCollision)
        {
            if (gCollisions->Raycast(
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY - 10),
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY + 10),
                    &pLine,
                    &hitX,
                    &hitY,
                    GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mXPos = FP_FromInteger((BaseAliveGameObjectPathTLV->mTopLeftX + BaseAliveGameObjectPathTLV->mBottomRightX) / 2);

                MapFollowMe(true);
                mYPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
                GetShadow()->mShadowAtBottom = true;
                BaseAliveGameObjectCollisionLine = pLine;
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);

                if (BaseAliveGameObjectPathTLV->mTlvType != ReliveTypes::eHoist || FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY - 1 * BaseAliveGameObjectPathTLV->mTopLeftY) >= GetSpriteScale() * FP_FromInteger(70))
                {
                    mCurrentMotion = eAbeMotions::Motion_69_LedgeHangWobble_454EF0;
                }
                else
                {
                    mReturnToPreviousMotion = true;
                    mPreviousMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
                    mBaseAliveGameObjectLastAnimFrame = 12;
                }

                if (!pLiftPoint)
                {
                    if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                        BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                    {
                        OnCollisionWith(
                            {FP_GetExponent(mXPos), FP_GetExponent(mYPos)},
                            {FP_GetExponent(mXPos), FP_GetExponent(mYPos + FP_FromInteger(5))},
                            gPlatformsArray);
                    }
                }
            }
        }
        else
        {
            BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
        }
    }

    if (!(GetAnimation().GetIsLastFrame()))
    {
        return;
    }

    if (mCurrentMotion != eAbeMotions::Motion_31_RunJumpMid_452C10)
    {
        return;
    }

    field_8_x_vel_slow_by = FP_FromDouble(0.75);
    mCurrentMotion = eAbeMotions::Motion_97_RunJumpToFall;
    mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
}

void Abe::Motion_32_RunJumpLand_453460()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, 32767, this);

        MapFollowMe(true);

        const u32 held = Input().GetHeld();
        if (InputCommands::eLeft & held)
        {
            if (InputCommands::eHop & mPrevInput)
            {
                BaseGameObject* pPortal = VIntoBirdPortal(3);
                if (pPortal)
                {
                    mBirdPortalSubState = PortalSubStates::eJumpingInsidePortal_0;
                    mBirdPortalId = pPortal->mBaseGameObjectId;
                    mCurrentMotion = eAbeMotions::Motion_30_RunJumpBegin_4532E0;
                    mPrevInput = 0;
                    return;
                }
                mCurrentMotion = eAbeMotions::Motion_30_RunJumpBegin_4532E0;
                mPrevInput = 0;
                return;
            }

            if (GetAnimation().GetFlipX())
            {
                if (held & InputCommands::eRun)
                {
                    mCurrentMotion = eAbeMotions::Motion_54_RunJumpLandRun_4538F0;
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }
                else
                {
                    if (WallHit(GetSpriteScale() * FP_FromInteger(50), -ScaleToGridSize(GetSpriteScale())))
                    {
                        mCurrentMotion = eAbeMotions::Motion_25_RunSlideStop_451330;
                        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::Motion_55_RunJumpLandWalk_453970;
                        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                    }
                }
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_26_RunTurn_451500;
                mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
                Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, this);
            }
        }
        else if (held & InputCommands::eRight)
        {
            if (InputCommands::eHop & mPrevInput)
            {
                BaseGameObject* pPortal = VIntoBirdPortal(3);
                if (pPortal)
                {
                    mBirdPortalSubState = PortalSubStates::eJumpingInsidePortal_0;
                    mBirdPortalId = pPortal->mBaseGameObjectId;
                }
                mCurrentMotion = eAbeMotions::Motion_30_RunJumpBegin_4532E0;
                mPrevInput = 0;
                return;
            }

            if (GetAnimation().GetFlipX())
            {
                mCurrentMotion = eAbeMotions::Motion_26_RunTurn_451500;
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
            }
            else if (held & InputCommands::eRun)
            {
                mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
                mCurrentMotion = eAbeMotions::Motion_54_RunJumpLandRun_4538F0;
            }
            else
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(50), ScaleToGridSize(GetSpriteScale())))
                {
                    mCurrentMotion = eAbeMotions::Motion_25_RunSlideStop_451330;
                    mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
                }
                else
                {
                    mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9);
                    mCurrentMotion = eAbeMotions::Motion_55_RunJumpLandWalk_453970;
                }
            }
        }
        else if (InputCommands::eHop & mPrevInput)
        {
            BaseGameObject* pPortal = VIntoBirdPortal(2);
            if (pPortal)
            {
                mBirdPortalSubState = PortalSubStates::eJumpingInsidePortal_0;
                mBirdPortalId = pPortal->mBaseGameObjectId;
            }
            mCurrentMotion = eAbeMotions::Motion_27_HopBegin_4521C0;
            mPrevInput = 0;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_25_RunSlideStop_451330;
            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
            }
            Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
        }
    }
}

void Abe::DoRunJump()
{
    BaseGameObject* pObj = VIntoBirdPortal(3);
    if (pObj)
    {
        mBirdPortalSubState = PortalSubStates::eJumpingInsidePortal_0;
        mBirdPortalId = pObj->mBaseGameObjectId;
    }

    mCurrentMotion = eAbeMotions::Motion_30_RunJumpBegin_4532E0;
    mPrevInput = 0;
}

void Abe::Motion_33_RunLoop_4508E0()
{
    mPrevInput |= Input().GetHeld();

    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
    }

    // Ran into wall?
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockback_44E700(1, 1);
        return;
    }

    MoveForward_44E9A0();

    if (mCurrentMotion != eAbeMotions::Motion_33_RunLoop_4508E0)
    {
        return;
    }

    if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 8)
    {
        MapFollowMe(true);
        if (mPrevInput & InputCommands::eHop)
        {
            DoRunJump();
        }
    }
    else if (GetAnimation().GetCurrentFrame() == 4 || GetAnimation().GetCurrentFrame() == 12)
    {
        Environment_SFX(EnvironmentSfx::eRunningFootstep_2, 0, 32767, this);

        MapFollowMe(true);

        // Turning around?
        if ((mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)))
        {
            mCurrentMotion = eAbeMotions::Motion_26_RunTurn_451500;

            Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
            mPrevInput = 0;
            return;
        }

        if (mPrevInput & InputCommands::eHop)
        {
            DoRunJump();
            return;
        }

        // Run to roll
        if (mPrevInput & InputCommands::eFartOrRoll)
        {
            mCurrentMotion = eAbeMotions::jMotion_38_RunToRoll_453A70;
            mReleasedButtons = 0;
            mPrevInput = 0;
            return;
        }

        if (Input().IsAnyHeld(InputCommands::eLeft) || Input().IsAnyHeld(InputCommands::eRight))
        {
            if (Input().IsAnyHeld(InputCommands::eRun))
            {
                mPrevInput = 0;
                return;
            }

            FP gridSize = {};
            if (mVelX >= FP_FromInteger(0))
            {
                gridSize = ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridSize = -ScaleToGridSize(GetSpriteScale());
            }

            // Run to walk and hit wall
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), gridSize) || WallHit(GetSpriteScale() * FP_FromInteger(20), gridSize))
            {
                ToKnockback_44E700(1, 1);
            }
            else
            {
                // Run to walk
                if (GetAnimation().GetCurrentFrame() == 4)
                {
                    mCurrentMotion = eAbeMotions::Motion_50_RunToWalk_450E20;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_51_MidRunToWalk_450F50;
                }
            }
        }
        else
        {
            // No longer running or even moving, so slide stop
            mCurrentMotion = eAbeMotions::Motion_25_RunSlideStop_451330;
            Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 32767, this);
        }

        mPrevInput = 0;
    }
}

void Abe::Motion_34_DunnoBegin_44ECF0()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_35_DunnoEnd_44ED10;
    }
}

void Abe::Motion_35_DunnoEnd_44ED10()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_36_Null_45BC50()
{
}

void Abe::Motion_37_CrouchTurn_454390()
{
    if (GetAnimation().GetCurrentFrame() != 0)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().ToggleFlipX();
            if (mNextMotion != eAbeMotions::Motion_0_Idle_44EEB0)
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = eAbeMotions::Motion_0_Idle_44EEB0;
            }
            else
            {
                mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
            }
        }
    }
    else
    {
        Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
    }
}

void Abe::jMotion_38_RunToRoll_453A70()
{
    Motion_22_RollBegin_4539A0();
}

// Almost the same as Motion_6_WalkBegin_44FEE0
void Abe::Motion_39_StandingToRun_450D40()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
    }

    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    mPrevInput |= Input().GetHeld();

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_40_SneakLoop_450550()
{
    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(true);
    }
    else
    {
        MoveForward_44E9A0();

        if (mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550)
        {
            const u32 held = Input().GetHeld();

            if (GetAnimation().GetCurrentFrame() == 3 || GetAnimation().GetCurrentFrame() == 13)
            {
                FP gridSize = {};
                if (GetAnimation().GetFlipX())
                {
                    gridSize = -ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    gridSize = ScaleToGridSize(GetSpriteScale());
                }

                // Hit wall, changed direction or no longer trying to sneak
                if (WallHit(GetSpriteScale() * FP_FromInteger(50), gridSize) || WallHit(GetSpriteScale() * FP_FromInteger(20), gridSize)
                    || (mVelX > FP_FromInteger(0) && (InputCommands::eLeft & held))
                    || (mVelX < FP_FromInteger(0) && (InputCommands::eRight & held))
                    || !((InputCommands::eLeft | InputCommands::eRight) & held))
                {
                    if (GetAnimation().GetCurrentFrame() != 3)
                    {
                        mCurrentMotion = eAbeMotions::Motion_46_SneakToIdle_450870;
                    }
                    else
                    {
                        mCurrentMotion = eAbeMotions::jMotion_47_MidSneakToIdle_4508C0;
                    }
                }
            }
            else if (GetAnimation().GetCurrentFrame() == 6 || GetAnimation().GetCurrentFrame() == 16)
            {
                Environment_SFX(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
                MapFollowMe(true);
                if ((InputCommands::eLeft | InputCommands::eRight) & held)
                {
                    // Sneak to walk
                    if (!(held & InputCommands::eSneak))
                    {
                        if (GetAnimation().GetCurrentFrame() != 6)
                        {
                            mCurrentMotion = eAbeMotions::Motion_44_MidSneakToWalk_450500;
                        }
                        else
                        {
                            mCurrentMotion = eAbeMotions::Motion_42_SneakToWalk_4503D0;
                        }
                    }
                }
                mPrevInput = 0;
            }
        }
    }
}

void Abe::Motion_41_WalkToSneak_450250()
{
    mPrevInput |= Input().GetHeld();

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10));
    }
    else
    {
        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(10);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_40_SneakLoop_450550;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(true);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_42_SneakToWalk_4503D0()
{
    mPrevInput |= Input().GetHeld();

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(true);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_43_MidWalkToSneak_450380()
{
    Motion_41_WalkToSneak_450250();

    if (mCurrentMotion == eAbeMotions::Motion_40_SneakLoop_450550)
    {
        mReturnToPreviousMotion = true;
        mCurrentMotion = eAbeMotions::Motion_43_MidWalkToSneak_450380;
        mPreviousMotion = eAbeMotions::Motion_40_SneakLoop_450550;
        mBaseAliveGameObjectLastAnimFrame = 10;
    }
}

void Abe::Motion_44_MidSneakToWalk_450500()
{
    Motion_42_SneakToWalk_4503D0();

    if (mCurrentMotion == eAbeMotions::Motion_1_WalkLoop_44FBA0)
    {
        mReturnToPreviousMotion = true;
        mCurrentMotion = eAbeMotions::Motion_44_MidSneakToWalk_450500;
        mPreviousMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
        mBaseAliveGameObjectLastAnimFrame = 9;
    }
}

void Abe::Motion_45_SneakBegin_4507A0()
{
    mPrevInput |= Input().GetHeld();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_40_SneakLoop_450550;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(true);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_46_SneakToIdle_450870()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX(EnvironmentSfx::eSneakFootstep_3, 0, 32767, this);
    }

    MoveForward_44E9A0();

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(true);
        ToIdle_44E6B0();
    }
}

void Abe::jMotion_47_MidSneakToIdle_4508C0()
{
    // TODO: Skipped jmp func jAbe::State_SneakEnd_40330F
    Motion_46_SneakToIdle_450870();
}

void Abe::Motion_48_WalkToRun_4500A0()
{
    mPrevInput |= Input().GetHeld();

    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
        mPrevInput = 0;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToIdle_44E6B0();
        MapFollowMe(true);
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_49_MidWalkToRun_450200()
{
    Motion_48_WalkToRun_4500A0();

    if (mCurrentMotion == eAbeMotions::Motion_33_RunLoop_4508E0)
    {
        mReturnToPreviousMotion = true;
        mCurrentMotion = eAbeMotions::Motion_49_MidWalkToRun_450200;
        mPreviousMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
        mBaseAliveGameObjectLastAnimFrame = 8;
    }
}

void Abe::Motion_50_RunToWalk_450E20()
{
    mPrevInput |= Input().GetHeld();

    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
    }
}

void Abe::Motion_51_MidRunToWalk_450F50()
{
    Motion_50_RunToWalk_450E20();

    if (mCurrentMotion == eAbeMotions::Motion_1_WalkLoop_44FBA0)
    {
        mReturnToPreviousMotion = true;
        mCurrentMotion = eAbeMotions::Motion_51_MidRunToWalk_450F50;
        mPreviousMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
        mBaseAliveGameObjectLastAnimFrame = 9;
    }
}

void Abe::Motion_52_RunTurnToRun_451710()
{
    mPrevInput |= Input().GetHeld();

    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
            GetAnimation().ToggleFlipX();
        }
    }
}

void Abe::Motion_53_RunTurnToWalk_451800()
{
    Motion_52_RunTurnToRun_451710();

    if (mCurrentMotion == eAbeMotions::Motion_33_RunLoop_4508E0)
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }
}

void Abe::Motion_54_RunJumpLandRun_4538F0()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        ToIdle_44E6B0();
    }
    else
    {
        MoveForward_44E9A0();
        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
        }
    }
}

void Abe::Motion_55_RunJumpLandWalk_453970()
{
    Motion_54_RunJumpLandRun_4538F0();

    if (mCurrentMotion == eAbeMotions::Motion_33_RunLoop_4508E0)
    {
        mCurrentMotion = eAbeMotions::Motion_1_WalkLoop_44FBA0;
    }
}

void Abe::Motion_56_DeathDropFall_4591F0()
{
    GetAnimation().SetAnimate(false);
    if (field_124_timer == 0)
    {
        field_8_x_vel_slow_by = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
        field_0_abe_timer = MakeTimer(90);
        field_124_timer++;
    }
    else if (field_124_timer == 1)
    {
        if (static_cast<s32>(sGnFrame) == field_0_abe_timer - 30)
        {
            SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
        }
        else if (static_cast<s32>(sGnFrame) == field_0_abe_timer - 24)
        {
            SfxPlayMono(relive::SoundEffects::KillEffect, 85);
            relive_new ScreenShake(true, false);
        }
        else if (static_cast<s32>(sGnFrame) >= field_0_abe_timer)
        {
            ToDieFinal_458910();
        }
    }
}

void Abe::Motion_57_Dead_4589A0()
{
    Fade* pFade_1 = static_cast<Fade*>(sObjectIds.Find_Impl(mFadeId));
    CircularFade* pCircularFade = static_cast<CircularFade*>(sObjectIds.Find_Impl(mCircularFadeId));

    GetAnimation().SetAnimate(false);

    if (BaseAliveGameObject_PlatformId != Guid{})
    {
        BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
        if (!pLiftPoint)
        {
            BaseAliveGameObject_PlatformId = Guid{};
        }
        else if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
        {
            static_cast<LiftPoint*>(pLiftPoint)->Move(FP_FromInteger(0), FP_FromInteger(0));
        }
    }

    switch (field_124_timer)
    {
        case 0:
            EventBroadcast(Event::kEventAbeDead, this);
            EventBroadcast(Event::kEventHeroDying, this);
            field_8_x_vel_slow_by = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            mPrevInput = 0;
            field_124_timer++;
            field_0_abe_timer = MakeTimer(30);
            if (BaseAliveGameObjectPathTLV && BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
            {
                const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mYPos + FP_FromInteger(15);
                const FP xpos = FP_FromInteger((Math_NextRandom() % 64) - 32) + mXPos;
                relive_new DeathBirdParticle(
                    xpos,
                    ypos,
                    (Math_NextRandom() % 8) + field_0_abe_timer + 60,
                    1,
                    GetSpriteScale());
            }
            else
            {
                const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mYPos + FP_FromInteger(15);
                const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mXPos;
                relive_new DeathBirdParticle(
                    xpos,
                    ypos,
                    (Math_NextRandom() % 8) + field_0_abe_timer + 15,
                    1,
                    GetSpriteScale());
            }
            return;

        case 1:
            EventBroadcast(Event::kEventHeroDying, this);
            if (!(sGnFrame % 4))
            {
                if (BaseAliveGameObjectPathTLV && BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
                {
                    const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mYPos + FP_FromInteger(15);
                    const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mXPos;
                    relive_new DeathBirdParticle(
                        xpos,
                        ypos,
                        (Math_NextRandom() % 8) + field_0_abe_timer + 60,
                        0,
                        GetSpriteScale());
                }
                else
                {
                    const FP ypos = FP_FromInteger(Math_NextRandom() % 10) + mYPos + FP_FromInteger(15);
                    const FP xpos = FP_FromInteger(((Math_NextRandom() % 64) - 32)) + mXPos;
                    relive_new DeathBirdParticle(
                        xpos,
                        ypos,
                        (Math_NextRandom() % 8) + field_0_abe_timer + 15,
                        0,
                        GetSpriteScale());
                }
            }

            SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));

            mRGB.r -= 2;
            mRGB.g -= 2;
            mRGB.b -= 2;

            if (static_cast<s32>(sGnFrame) > field_0_abe_timer)
            {
                field_0_abe_timer = MakeTimer(60);

                if (BaseAliveGameObjectPathTLV)
                {
                    if (BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eDeathDrop)
                    {
                        field_0_abe_timer = (sGnFrame + 60) + 45;
                    }
                }
                ++field_124_timer;
                MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_11, this, 1, 0);
            }
            return;
        case 2:
            EventBroadcast(Event::kEventHeroDying, this);
            if (static_cast<s32>(sGnFrame) > field_0_abe_timer)
            {
                ++field_124_timer;
            }
            return;
        case 3:
        {
            EventBroadcast(Event::kEventHeroDying, this);
            if (pFade_1)
            {
                pFade_1->SetDead(true);
                mFadeId = Guid{};
            }
            auto pFade = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeIn, false, 8, relive::TBlendModes::eBlend_2);
            if (pFade)
            {
                mFadeId = pFade->mBaseGameObjectId;
            }

            if (pCircularFade)
            {
                pCircularFade->SetDead(true);
            }
            ++field_124_timer;
        }
            return;

        case 4:
            EventBroadcast(Event::kEventHeroDying, this);

            if (!pFade_1->mDone)
            {
                return;
            }

            VOnTrapDoorOpen();

            BaseAliveGameObjectCollisionLine = nullptr;
            field_0_abe_timer = MakeTimer(8);
            ++field_124_timer;
            //dword_5C2C64 = 0; // TODO: Never read ?
            return;

        case 5:
            EventBroadcast(Event::kEventDeathReset, this);
            if (static_cast<s32>(sGnFrame) <= field_0_abe_timer)
            {
                return;
            }
            Make_Circular_Fade(
                FP_FromInteger(QuikSave::gActiveQuicksaveData.mWorldInfo.mControlledCharX),
                FP_FromInteger(QuikSave::gActiveQuicksaveData.mWorldInfo.mControlledCharY),
                QuikSave::gActiveQuicksaveData.mWorldInfo.mControlledCharScale != 0 ? FP_FromDouble(1.0) : FP_FromDouble(0.5),
                0,
                true,
                true);
            QuikSave::LoadActive();
            return;
        default:
            return;
    }
}

void Abe::Motion_58_DeadPre_4593E0()
{
    if (field_120_state.raw == 1)
    {
        mCurrentMotion = eAbeMotions::Motion_57_Dead_4589A0;
        field_124_timer = 2;
        field_0_abe_timer = MakeTimer(60);
    }
    else
    {
        EventBroadcast(Event::kEventHeroDying, this);
    }
}

void Abe::Motion_59_Null_459450()
{
}

void Abe::Motion_60_Unused_4A3200()
{
    
}

void Abe::Motion_61_TurnToRun_456530()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        // Was going to run into something
        ToKnockback_44E700(1, 1);
    }
    else
    {
        MoveForward_44E9A0();

        if (mCurrentMotion == eAbeMotions::Motion_61_TurnToRun_456530)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                mCurrentMotion = eAbeMotions::Motion_33_RunLoop_4508E0;
            }
        }
    }
}

void Abe::Motion_62_Punch_454750()
{
    if (GetAnimation().GetCurrentFrame() == 5)
    {
        FP gridSize = {};
        if (GetAnimation().GetFlipX())
        {
            gridSize = mXPos - ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize = ScaleToGridSize(GetSpriteScale()) + mXPos;
        }

        const FP kFP5 = FP_FromInteger(5);
        BaseGameObject* pSlapTarget = FindObjectOfType(ReliveTypes::eMudokon, gridSize, mYPos - kFP5);
        while (pSlapTarget)
        {
            // Is it in a motion where we can slap it?
            const eMudMotions mud_motion = static_cast<Mudokon*>(pSlapTarget)->mCurrentMotion;
            if (mud_motion != eMudMotions::Motion_46_Knockback && mud_motion != eMudMotions::Motion_47_KnockbackGetUp)
            {
                // Can slap, so exit
                break;
            }

            // Try to get the next "stacked" mud - e.g if we have like 20 muds on 1 grid block this will iterate through them
            pSlapTarget = GetStackedSlapTarget(pSlapTarget->mBaseGameObjectId, ReliveTypes::eMudokon, gridSize, mYPos - kFP5);
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType(ReliveTypes::eSlapLock, gridSize, mYPos - (FP_FromInteger(30) * GetSpriteScale()));
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType(ReliveTypes::eSlig, gridSize, mYPos - kFP5);
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType(ReliveTypes::eSlig, mXPos, mYPos - kFP5);
        }

        if (!pSlapTarget)
        {
            pSlapTarget = FindObjectOfType(ReliveTypes::eGlukkon, gridSize, mYPos - kFP5);
        }

        if (pSlapTarget)
        {
            static_cast<BaseAliveGameObject*>(pSlapTarget)->VTakeDamage(this);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_63_Sorry_454670()
{
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        FP yOff = {};
        FP xOff = {};
        if (GetAnimation().GetFlipX())
        {
            yOff = mYPos - FP_FromInteger(5);
            xOff = mXPos - ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            yOff = mYPos - FP_FromInteger(5);
            xOff = ScaleToGridSize(GetSpriteScale()) + mXPos;
        }

        auto pMud = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eMudokon, xOff, yOff));
        if (pMud)
        {
            pMud->VTakeDamage(this);
        }

        Mudokon_SFX(MudSounds::eSorry_27, 0, 0, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_64_AfterSorry_454730;
    }
}

void Abe::Motion_64_AfterSorry_454730()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_65_LedgeAscend_4548E0()
{
    const s32 curFrameNum = GetAnimation().GetCurrentFrame();
    if (curFrameNum == 0)
    {
        Environment_SFX(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 32767, this);
    }
    else if (curFrameNum == 4)
    {
        Environment_SFX(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
        GetShadow()->mShadowAtBottom = false;
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        // Now the ascend is done go back to stand idle
        GetShadow()->mShadowAtBottom = false;
        MapFollowMe(true);
        ToIdle_44E6B0();
    }
}

void Abe::Motion_66_LedgeDescend_454970()
{
    const s32 curFrameNum = GetAnimation().GetCurrentFrame();
    if (curFrameNum == 2)
    {
        Environment_SFX(EnvironmentSfx::eRunJumpOrLedgeHoist_11, 0, 32767, this);
        GetShadow()->mShadowAtBottom = true;
    }
    else if (curFrameNum == 21)
    {
        relive::Path_Hoist* pHoist = static_cast<relive::Path_Hoist*>(gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos + FP_FromInteger(16)),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos + FP_FromInteger(16)),
            ReliveTypes::eHoist));

        if (pHoist)
        {
            if (FP_FromInteger((pHoist->mBottomRightY - 1 * pHoist->mTopLeftY)) < GetSpriteScale() * FP_FromInteger(70))
            {
                mYPos = (FP_FromInteger(60) * GetSpriteScale()) + mYPos;

                PathLine* pLine = nullptr;
                FP hitX = {};
                FP hitY = {};

                if (gCollisions->Raycast(
                        mXPos,
                        mYPos - FP_FromInteger(5),
                        mXPos,
                        mYPos + FP_FromInteger(5),
                        &pLine,
                        &hitX,
                        &hitY,
                        GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
                {
                    BaseAliveGameObjectCollisionLine = pLine;
                    mYPos = hitY;
                    mCurrentMotion = eAbeMotions::Motion_16_LandSoft_45A360;
                    GetShadow()->mShadowAtBottom = false;
                }
                else
                {
                    mYPos -= FP_FromInteger(60) * GetSpriteScale();
                }
            }
        }
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
    }
}

void Abe::Motion_67_LedgeHang_454E20()
{
    GetShadow()->mShadowAtBottom = true;
    const s32 held = Input().GetHeld();
    if (InputCommands::eUp & held || GetTeleporting())
    {
        mCurrentMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
    }
    else if (held & InputCommands::eDown)
    {
        VOnTrapDoorOpen();
        BaseAliveGameObjectCollisionLine = nullptr;
        mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab;
        mYPos += GetSpriteScale() * FP_FromInteger(75);
        GetShadow()->mShadowAtBottom = false;
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
}

void Abe::Motion_68_ToOffScreenHoist_454B80()
{
    BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);

    // Get the current hoist - even though there is no need to?
    relive::Path_TLV* pHoist = gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eHoist);

    // Find the hoist we are "connecting" to
    BaseAliveGameObjectPathTLV = pHoist;
    mYPos -= GetSpriteScale() * FP_FromInteger(75);
    GetShadow()->mShadowAtBottom = true;

    const FP ypos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY) - (FP_FromInteger(40) * GetSpriteScale());
    pHoist = gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(ypos),
        FP_GetExponent(mXPos),
        FP_GetExponent(ypos),
        ReliveTypes::eHoist);

    BaseAliveGameObjectPathTLV = pHoist;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (pHoist && gCollisions->Raycast(mXPos, FP_FromInteger(pHoist->mTopLeftY - 10), mXPos, FP_FromInteger(pHoist->mTopLeftY + 10), &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
    {
        BaseAliveGameObjectCollisionLine = pLine;
        mYPos = FP_NoFractional(hitY + FP_FromDouble(0.5));
        mVelY = FP_FromInteger(0);
        if (!pLiftPoint)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                OnCollisionWith(
                    {FP_GetExponent(mXPos), FP_GetExponent(mYPos)},
                    {FP_GetExponent(mXPos), FP_GetExponent(mYPos + FP_FromInteger(5))},
                    gPlatformsArray);
            }
        }
        mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
    }
    else
    {
        mCurrentMotion = eAbeMotions::Motion_14_HoistIdle_452440;
    }
}

void Abe::Motion_69_LedgeHangWobble_454EF0()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        if (!mPlayLedgeGrabSounds)
        {
            mPlayLedgeGrabSounds = true;
            Environment_SFX(EnvironmentSfx::eWalkingFootstep_1, 0, 127, this);
        }
    }
    else
    {
        if (GetAnimation().GetCurrentFrame() == 2)
        {
            if (!mPlayLedgeGrabSounds)
            {
                mPlayLedgeGrabSounds = true;
                Mudokon_SFX(MudSounds::eHurt1_16, 45, -200, this);
            }
        }
        else
        {
            mPlayLedgeGrabSounds = false;
        }
    }

    // Going up the ledge on wobble?
    const u32 held = Input().GetHeld();
    if (InputCommands::eUp & held || GetTeleporting())
    {
        mPlayLedgeGrabSounds = false;
        mCurrentMotion = eAbeMotions::Motion_65_LedgeAscend_4548E0;
    }
    // Going down the ledge wobble?
    else if (held & InputCommands::eDown)
    {
        mPlayLedgeGrabSounds = false;
        VOnTrapDoorOpen();
        BaseAliveGameObjectCollisionLine = nullptr;
        mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab;
        mYPos += GetSpriteScale() * FP_FromInteger(75);
        GetShadow()->mShadowAtBottom = false;
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
    // Now stabilized when wobble anim is done
    else if (GetAnimation().GetIsLastFrame())
    {
        mPlayLedgeGrabSounds = false;
        mCurrentMotion = eAbeMotions::Motion_67_LedgeHang_454E20;
    }
}

void Abe::Motion_70_RingRopePullHang_455AF0()
{
    PullRingRope* pPullRing = static_cast<PullRingRope*>(sObjectIds.Find(mPullRingRopeId, ReliveTypes::ePullRingRope));
    if (pPullRing)
    {
        if (!pPullRing->VIsNotBeingPulled())
        {
            return;
        }
        pPullRing->VMarkAsPulled();
    }

    mPullRingRopeId = Guid{};
    mVelY = FP_FromInteger(0);
    mCurrentMotion = eAbeMotions::Motion_91_FallingFromGrab;
}

void Abe::Motion_71_Knockback_455090()
{
    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs_44EC10();
    }

    if (GetAnimation().GetRender())
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
            {
                mVelX = FP_FromInteger(0);
            }

            MoveWithVelocity_450FA0(FP_FromDouble(0.67));

            if ((gMap.mCurrentLevel == EReliveLevelIds::eMines
                 || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz
                 || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot
                 || gMap.mCurrentLevel == EReliveLevelIds::eBarracks
                 || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
                && GetAnimation().GetCurrentFrame() == 7)
            {
                Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
                EventBroadcast(Event::kEventNoise, this);
                EventBroadcast(Event::kEventSuspiciousNoise, this);
            }
        }
        else
        {
            if (GetAnimation().GetCurrentFrame() >= 6)
            {
                GetAnimation().SetFrame(5);
            }

            Motion_3_Fall_459B60();

            if (mCurrentMotion == eAbeMotions::Motion_84_FallLandDie_45A420)
            {
                mCurrentMotion = eAbeMotions::Motion_71_Knockback_455090;
                SfxPlayMono(relive::SoundEffects::KillEffect, 85);
                SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 95, 95);
            }
            else if (mCurrentMotion == eAbeMotions::Motion_16_LandSoft_45A360)
            {
                mCurrentMotion = eAbeMotions::Motion_71_Knockback_455090;
                Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
            }
        }
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if (!mbMotionChanged &&(BaseAliveGameObjectCollisionLine || !(GetAnimation().GetRender())))
        {
            if (mHealth > FP_FromInteger(0) || gAbeInvincible || GetElectrocuted())
            {
                mCurrentMotion = eAbeMotions::Motion_72_KnockbackGetUp_455340;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }
}

void Abe::Motion_72_KnockbackGetUp_455340()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_73_PushWall_4553A0()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);
    if (GetAnimation().GetCurrentFrame() == 10)
    {
        if (Math_NextRandom() <= 127)
        {
            Environment_SFX(EnvironmentSfx::eExhaustingHoistNoise_10, 0, 32767, this);
        }
    }
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_74_RollingKnockback_455290()
{
    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs_44EC10();
    }

    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if (!mbMotionChanged)
        {
            if (mHealth > FP_FromInteger(0) || gAbeInvincible)
            {
                mCurrentMotion = eAbeMotions::Motion_72_KnockbackGetUp_455340;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }
}

void Abe::Motion_75_JumpIntoWell_45C7B0()
{
    GetShadow()->mEnabled = false;

    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
    }
    jMotion_81_WellBegin_4017F8();
}

void Abe::Motion_76_ToInsideOfAWellLocal_45CA40()
{
    Motion_79_InsideWellLocal_45CA60();
}

void Abe::Motion_77_ToWellShotOut_45D130()
{
    Motion_80_WellShotOut_45D150();
}

void Abe::Motion_78_WellBegin_45C810()
{
    if (GetAnimation().GetCurrentFrame() > 10)
    {
        GetShadow()->mEnabled = false;

        // Get a local well
        BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellLocal);

        if (!BaseAliveGameObjectPathTLV)
        {
            // No local well, must be an express well
            BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eWellExpress);
        }

        const s16 xpos = FP_GetExponent(mXPos);
        const s16 tlv_mid_x = (BaseAliveGameObjectPathTLV->mTopLeftX + BaseAliveGameObjectPathTLV->mBottomRightX) / 2;
        if (xpos > tlv_mid_x)
        {
            mXPos -= GetSpriteScale();
        }
        else if (xpos < tlv_mid_x)
        {
            mXPos += GetSpriteScale();
        }
    }

    if (GetAnimation().GetCurrentFrame() == 11)
    {
        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        field_124_timer = 15;

        SfxPlayMono(relive::SoundEffects::WellEnter, 0, GetSpriteScale());

        if (gPathInfo->VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eWellExpress))
        {
            mCurrentMotion = eAbeMotions::Motion_82_InsideWellExpress_45CC80;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_79_InsideWellLocal_45CA60;
        }
    }
}

void Abe::Motion_79_InsideWellLocal_45CA60()
{
    const s32 gnFrame = field_124_timer;
    field_124_timer = gnFrame - 1;
    if (!gnFrame)
    {
        BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellLocal);

        if (!BaseAliveGameObjectPathTLV)
        {
            BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eWellExpress);
        }

        field_8_x_vel_slow_by = FP_FromInteger(0);

        relive::Path_WellBase* pBaseWell = static_cast<relive::Path_WellBase*>(BaseAliveGameObjectPathTLV);
        if (pBaseWell->mTlvType == ReliveTypes::eWellLocal)
        {
            relive::Path_WellLocal* pLocal = static_cast<relive::Path_WellLocal*>(pBaseWell);
            if (SwitchStates_Get(pBaseWell->mSwitchId))
            {
                Calc_Well_Velocity_45C530(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    pLocal->mOnDestX,
                    pLocal->mOnDestY);
            }
            else
            {
                Calc_Well_Velocity_45C530(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    pLocal->mOffDestX,
                    pLocal->mOffDestY);
            }
        }
        else if (pBaseWell->mTlvType == ReliveTypes::eWellExpress)
        {
            relive::Path_WellExpress* pExpress = static_cast<relive::Path_WellExpress*>(pBaseWell);
            Calc_Well_Velocity_45C530(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                pExpress->mExitX,
                pExpress->mExitY);
        }

        MapFollowMe(true);

        mYPos += mVelY;

        if (mVelX >= FP_FromInteger(0))
        {
            GetAnimation().SetFlipX(false);
        }
        else
        {
            GetAnimation().SetFlipX(true);
        }

        SfxPlayMono(relive::SoundEffects::WellExit, 0, GetSpriteScale());

        // TODO: handle this better
        s16 nextMotion = static_cast<s16>(mCurrentMotion);
        nextMotion++;
        mCurrentMotion = static_cast<eAbeMotions>(nextMotion); // Note: used to be mCurrentMotion++

        BaseAliveGameObjectLastLineYPos = mYPos;

        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
        }

        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Abe::Motion_80_WellShotOut_45D150()
{
    if (mVelY >= FP_FromInteger(0))
    {
        Motion_3_Fall_459B60();
    }
    else
    {
        mVelY += GetSpriteScale() * FP_FromDouble(1.8);

        mXPos += mVelX;
        mYPos += mVelY;

        SetActiveCameraDelayedFromDir();

        BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }

    if (GetAnimation().GetForwardLoopCompleted() || (mCurrentMotion != eAbeMotions::Motion_80_WellShotOut_45D150 && mCurrentMotion != eAbeMotions::Motion_77_ToWellShotOut_45D130))
    {
        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
        }

        GetShadow()->mEnabled = true;
    }

    if (mCurrentMotion == eAbeMotions::Motion_84_FallLandDie_45A420)
    {
        mHealth = FP_FromInteger(1);
        mCurrentMotion = eAbeMotions::Motion_16_LandSoft_45A360;
    }
}

void Abe::jMotion_81_WellBegin_45C7F0()
{
    jMotion_81_WellBegin_4017F8();
}

void Abe::Motion_82_InsideWellExpress_45CC80()
{
    BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellLocal);

    if (!BaseAliveGameObjectPathTLV)
    {
        BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos),
            ReliveTypes::eWellExpress);
    }

    relive::Path_WellExpress* pExpressWell = static_cast<relive::Path_WellExpress*>(BaseAliveGameObjectPathTLV);
    if (SwitchStates_Get(pExpressWell->mSwitchId))
    {
        mDstWellLevel = pExpressWell->mOnDestLevel;
        mDstWellPath = pExpressWell->mOnDestPath;
        mDstWellCamera = pExpressWell->mOnDestCamera;
        field_1A0_door_id = pExpressWell->mOnOtherWellId;
    }
    else
    {
        mDstWellLevel = pExpressWell->mOffDestLevel;
        mDstWellPath = pExpressWell->mOffDestPath;
        mDstWellCamera = pExpressWell->mOffDestCamera;
        field_1A0_door_id = pExpressWell->mOffOtherWellId;
    }

    field_8_x_vel_slow_by = FP_FromInteger(0);
    BaseAliveGameObjectLastLineYPos = mYPos;

    if (mDstWellLevel != gMap.mCurrentLevel || mDstWellPath != gMap.mCurrentPath || mDstWellCamera != gMap.mCurrentCamera)
    {
        field_124_timer = 1;

        if (pExpressWell->mMovieId)
        {
            gMap.SetActiveCam(mDstWellLevel, mDstWellPath, mDstWellCamera, CameraSwapEffects::ePlay1FMV_5, pExpressWell->mMovieId, 0);
        }
        else
        {
            gMap.SetActiveCam(mDstWellLevel, mDstWellPath, mDstWellCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }

        // FeeCo hack!
        if (mDstWellLevel == EReliveLevelIds::eFeeCoDepot && mDstWellPath == 1 && mDstWellCamera == 1)
        {
            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mXPos = FP_FromInteger(1187);
            mYPos = FP_FromInteger(270);
            GetAnimation().SetFlipX(false);
            mLandSoftly = true;
            mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
            GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
        }
        else
        {
            GetAnimation().SetRender(false);
            mCurrentMotion = eAbeMotions::Motion_83_WellExpressShotOut_45CF70;
        }
    }
    else
    {
        Motion_83_WellExpressShotOut_45CF70();
        mYPos -= mVelY * GetSpriteScale();
        mVelY = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);
        mCurrentMotion = eAbeMotions::Motion_79_InsideWellLocal_45CA60;
    }
}

void Abe::Motion_83_WellExpressShotOut_45CF70()
{
    PSX_Point camPos = {};
    gMap.GetCurrentCamCoords(&camPos);

    GetAnimation().SetRender(true);

    relive::Path_TLV* pTlvIter = nullptr;
    relive::Path_WellBase* pWell = nullptr;
    for (;;)
    {
        pTlvIter = gPathInfo->TLV_Get_At(
            pTlvIter,
            FP_FromInteger(camPos.x),
            FP_FromInteger(camPos.y),
            FP_FromInteger(camPos.x + 368),
            FP_FromInteger(camPos.y + 240));

        // At the end, exit.
        if (!pTlvIter)
        {
            break;
        }

        // Is it a well?
        if (pTlvIter->mTlvType == ReliveTypes::eWellLocal || pTlvIter->mTlvType == ReliveTypes::eWellExpress)
        {
            // Is it the target of the previous well?
            relive::Path_WellBase* pWellBase = static_cast<relive::Path_WellBase*>(pTlvIter);
            if (pWellBase->mOtherWellId == field_1A0_door_id)
            {
                pWell = pWellBase;
                break;
            }
        }
    }

    mCurrentLevel = gMap.mCurrentLevel;
    mCurrentPath = gMap.mCurrentPath;

    if (pWell)
    {
        if (pWell->mScale == relive::reliveScale::eHalf)
        {
            SetSpriteScale(FP_FromDouble(0.5));
            SetScale(Scale::Bg);
        }
        else
        {
            SetSpriteScale(FP_FromInteger(1));
            SetScale(Scale::Fg);
        }

        mXPos = FP_FromInteger((pWell->mTopLeftX + pWell->mBottomRightX) / 2);
        mYPos = FP_FromInteger(pWell->mBottomRightY);

        BaseAliveGameObjectPathTLV = pWell;

        mCurrentMotion = eAbeMotions::Motion_79_InsideWellLocal_45CA60;
    }
    else
    {
        mXPos = FP_FromInteger(camPos.x + 184);
        mYPos = FP_FromInteger(camPos.y + 80);
        mVelX = GetSpriteScale() * FP_FromDouble(-2.68);
        mVelY = GetSpriteScale() * FP_FromInteger(-15);
    }
}

void Abe::Motion_84_FallLandDie_45A420()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::KillEffect, 85);
        SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 95, 95);
        relive_new ScreenShake(true, false);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (static_cast<s32>(sGnFrame) >= field_124_timer)
        {
            ToDieFinal_458910();
        }
    }
}

void Abe::jMotion_85_Fall_455070()
{
    Motion_3_Fall_459B60();
}

s32 sHandstoneSoundChannels_5C2C68 = 0;

void Abe::Motion_86_HandstoneBegin()
{
    CircularFade* pCircularFade = static_cast<CircularFade*>(sObjectIds.Find_Impl(mCircularFadeId));
    Fade* pFade = static_cast<Fade*>(sObjectIds.Find_Impl(mFadeId));

    switch (field_120_state.stone)
    {
        case StoneStates::eHandstoneBegin_0:
            if (GetAnimation().GetForwardLoopCompleted())
            {
                CircularFade* pCircularFade2 = Make_Circular_Fade(
                    mXPos,
                    mYPos,
                    GetSpriteScale(),
                    1,
                    false,
                    false);

                if (GetAnimation().GetFlipX())
                {
                    pCircularFade2->GetAnimation().SetFlipX(true);
                }
                else
                {
                    pCircularFade2->GetAnimation().SetFlipX(false);
                }

                mCircularFadeId = pCircularFade2->mBaseGameObjectId;
                field_120_state.stone = StoneStates::eGetHandstoneType_1;

                SfxPlayMono(relive::SoundEffects::IngameTransition, 90);

                BaseAliveGameObjectPathTLV = gPathInfo->VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eMovieHandStone);

                sHandstoneSoundChannels_5C2C68 = SFX_Play_Pitch(relive::SoundEffects::HandstoneTransition, 127, -300);

                s32 switch_id = 0;
                relive::Path_MovieStone* pMovieStoneTlv = static_cast<relive::Path_MovieStone*>(BaseAliveGameObjectPathTLV);
                if (!pMovieStoneTlv)
                {
                    relive::Path_HandStone* pHandStoneTlv = static_cast<relive::Path_HandStone*>(gPathInfo->VTLV_Get_At_Of_Type(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        ReliveTypes::eHandStone));

                    BaseAliveGameObjectPathTLV = pHandStoneTlv;

                    if (pHandStoneTlv)
                    {
                        switch_id = pHandStoneTlv->mTriggerSwitchId;

                        mFmvId = static_cast<s16>(pHandStoneTlv->mScale); // TODO: Never used for this type?

                        mHandStoneCams[0] = pHandStoneTlv->mCameraId1;
                        mHandStoneCams[1] = pHandStoneTlv->mCameraId2;
                        mHandStoneCams[2] = pHandStoneTlv->mCameraId3;
                    }
                }
                else
                {
                    switch_id = pMovieStoneTlv->mTriggerSwitchId;

                    mFmvId = pMovieStoneTlv->mMovieId;
                    mHandStoneCams[0] = static_cast<s16>(pMovieStoneTlv->mScale); // TODO: Never used?
                    mHandStoneCams[1] = static_cast<s16>(pMovieStoneTlv->mTriggerSwitchId);    // TODO: Never used?
                }

                if (BaseAliveGameObjectPathTLV)
                {
                    if (switch_id > 1)
                    {
                        SwitchStates_Set(static_cast<s16>(switch_id), 1);
                    }

                    mHandStoneType = BaseAliveGameObjectPathTLV->mTlvType;
                }
                else
                {
                    mCurrentMotion = eAbeMotions::Motion_87_HandstoneEnd;
                }
            }
            break;

        case StoneStates::eGetHandstoneType_1:
            if (pCircularFade->VDone())
            {
                if (mHandStoneType == ReliveTypes::eMovieHandStone)
                {
                    gScreenManager->EnableRendering();

                    FmvInfo* pFmvRec = Path_Get_FMV_Record(gMap.mCurrentLevel, mFmvId);

                    relive_new Movie(pFmvRec->field_0_pName);
                    field_120_state.stone = StoneStates::eHandstoneMovieDone_2;
                }
                else if (mHandStoneType == ReliveTypes::eHandStone)
                {
                    GetAnimation().SetRender(false);
                    mHandStoneCamIdx = 1;
                    field_120_state.stone = StoneStates::eWaitForInput_4;
                    pCircularFade->SetDead(true);
                    mCircularFadeId = Guid{};
                    Fade* pFade33 = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 0, 8, relive::TBlendModes::eBlend_2);
                    if (pFade33)
                    {
                        mFadeId = pFade33->mBaseGameObjectId;
                    }

                    mDstWellCamera = gMap.mCurrentCamera;
                    gMap.SetActiveCam(mCurrentLevel, mCurrentPath, mHandStoneCams[0], CameraSwapEffects::eInstantChange_0, 0, 0);
                }
            }
            break;

        case StoneStates::eHandstoneMovieDone_2:
            if (gMovieRefCount == 0)
            {
                gPsxDisplay.PutCurrentDispEnv();
                gScreenManager->DecompressCameraToVRam(gMap.field_2C_camera_array[0]->mCamRes);
                gScreenManager->EnableRendering();
                pCircularFade->VFadeIn(0, 0);
                field_120_state.stone = StoneStates::eHandstoneEnd_3;
            }
            break;

        case StoneStates::eHandstoneEnd_3:
            if (pCircularFade->VDone())
            {
                pCircularFade->SetDead(true);
                mCircularFadeId = Guid{};

                mCurrentMotion = eAbeMotions::Motion_87_HandstoneEnd;

                if (sHandstoneSoundChannels_5C2C68)
                {
                    SND_Stop_Channels_Mask(sHandstoneSoundChannels_5C2C68);
                    sHandstoneSoundChannels_5C2C68 = 0;
                }

            }
            break;

        case StoneStates::eWaitForInput_4:
            if (pFade->mDone)
            {
                if (Input().IsAnyPressed(InputCommands::eBack | InputCommands::eUnPause_OrConfirm))
                {
                    pFade->Init(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeIn, 0, 8);
                    field_120_state.stone = StoneStates::eCamChangeTransition_5;
                    SfxPlayMono(relive::SoundEffects::IngameTransition, 90);
                }
            }
            break;

        case StoneStates::eCamChangeTransition_5:
            if (pFade->mDone)
            {
                if (mHandStoneCamIdx < 3 && mHandStoneCams[mHandStoneCamIdx] != 0)
                {
                    field_120_state.stone = StoneStates::eWaitForInput_4;

                    pFade->SetDead(true);
                    pFade = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeOut, 0, 8, relive::TBlendModes::eBlend_2);
                    if (pFade)
                    {
                        mFadeId = pFade->mBaseGameObjectId;
                    }

                    gMap.SetActiveCam(
                        mCurrentLevel,
                        mCurrentPath,
                        mHandStoneCams[mHandStoneCamIdx++],
                        CameraSwapEffects::eInstantChange_0,
                        0,
                        0);
                }
                else
                {
                    field_120_state.stone = StoneStates::eSetActiveCamToAbe_6;
                }
            }
            break;

        case StoneStates::eSetActiveCamToAbe_6:
            if (pFade->mDone)
            {
                GetAnimation().SetRender(true);
                field_120_state.stone = StoneStates::eCircularFadeExit_7;
                gMap.SetActiveCam(
                    mCurrentLevel,
                    mCurrentPath,
                    mDstWellCamera,
                    CameraSwapEffects::eInstantChange_0,
                    0,
                    0);
            }
            break;

        case StoneStates::eCircularFadeExit_7:
        {
            pFade->SetDead(true);
            mFadeId = Guid{};

            CircularFade* pCircularFade2 = Make_Circular_Fade(mXPos, mYPos, GetSpriteScale(), 0, false, false);
            if (GetAnimation().GetFlipX())
            {
                pCircularFade2->GetAnimation().SetFlipX(true);
            }
            else
            {
                pCircularFade2->GetAnimation().SetFlipX(false);
            }

            mCircularFadeId = pCircularFade2->mBaseGameObjectId;
            field_120_state.stone = StoneStates::eHandstoneEnd_3;

            if (sHandstoneSoundChannels_5C2C68)
            {
                SND_Stop_Channels_Mask(sHandstoneSoundChannels_5C2C68);
                sHandstoneSoundChannels_5C2C68 = 0;
            }
        }
        break;

        default:
            return;
    }
}

void Abe::Motion_87_HandstoneEnd()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_88_GrenadeMachineUse()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_89_BrewMachineBegin()
{
    if (field_120_state.raw > 0)
    {
        if (field_120_state.raw <= 36u)
        {
            if (field_120_state.raw > 11u && !((field_120_state.raw - 12) % 6))
            {
                SFX_Play_Pitch(relive::SoundEffects::BrewMachineUseEnd, 0, 32 * field_120_state.raw);
            }
            field_120_state.raw++;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_90_BrewMachineEnd;
        }
    }
    else if (GetAnimation().GetCurrentFrame() == 8)
    {
        if (GetEvilFart_4585F0(false))
        {
            SfxPlayMono(relive::SoundEffects::BrewMachineUseStart, 0);
        }
        else
        {
            SfxPlayMono(relive::SoundEffects::BrewMachineUseEmpty, 0);
        }
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        if (GetEvilFart_4585F0(true))
        {
            SfxPlayMono(relive::SoundEffects::BrewMachineUseMid, 0);
            field_120_state.raw = 1;
        }
        else
        {
            mCurrentMotion = eAbeMotions::Motion_90_BrewMachineEnd;
        }
    }
}

void Abe::Motion_90_BrewMachineEnd()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        mbMotionChanged = true;
        mCurrentMotion = eAbeMotions::Motion_0_Idle_44EEB0;
        field_124_timer = 1;
    }
}

// Let go of an edge/hoist/pull ring
void Abe::Motion_91_FallingFromGrab()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
    }

    Motion_3_Fall_459B60();

    if (mCurrentMotion == eAbeMotions::Motion_84_FallLandDie_45A420)
    {
        mCurrentMotion = eAbeMotions::Motion_16_LandSoft_45A360;
    }

    mPreviousMotion = eAbeMotions::Motion_91_FallingFromGrab;
}

void Abe::Motion_92_ForceDownFromHoist()
{
    if (!field_124_timer)
    {
        GetShadow()->mShadowAtBottom = false;
        VOnTrapDoorOpen();
        FP hitX = {};
        FP hitY = {};
        if (gCollisions->Raycast(
                mXPos,
                mYPos + (GetSpriteScale() * FP_FromInteger(75)),
                mXPos,
                mYPos + FP_FromInteger(10),
                &BaseAliveGameObjectCollisionLine,
                &hitX,
                &hitY,
                GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
            == 1)
        {
            mYPos = hitY;
            mCurrentMotion = eAbeMotions::Motion_84_FallLandDie_45A420;
            mRegenHealthTimer = MakeTimer(900);
            mPreviousMotion = eAbeMotions::Motion_3_Fall_459B60;
            return;
        }
        mYPos += (GetSpriteScale() * FP_FromInteger(75));
        BaseAliveGameObjectLastLineYPos = mYPos;
        BaseAliveGameObjectCollisionLine = nullptr;
        field_124_timer = field_124_timer + 1;
    }
    Motion_3_Fall_459B60();
}

void Abe::Motion_93_WalkOffEdge()
{
    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
    }
    Motion_3_Fall_459B60();
}

void Abe::Motion_94_RunOffEdge()
{
    Motion_93_WalkOffEdge();
}

void Abe::Motion_95_SneakOffEdge()
{
    Motion_93_WalkOffEdge();
}

void Abe::Motion_96_HopToFall()
{
    if (GetAnimation().GetFlipX())
    {
        mXPos = (GetSpriteScale() * FP_FromInteger(5)) + mXPos;
    }
    else
    {
        mXPos = mXPos - (GetSpriteScale() * FP_FromInteger(5));
    }

    mVelY += (GetSpriteScale() * FP_FromInteger(4));
    Motion_93_WalkOffEdge();
}

void Abe::Motion_97_RunJumpToFall()
{
    Motion_93_WalkOffEdge();
}

void Abe::Motion_98_RollOffEdge()
{
    Motion_93_WalkOffEdge();
}

void Abe::Motion_99_LeverUse()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (GetTeleporting())
        {
            mCurrentMotion = eAbeMotions::Motion_34_DunnoBegin_44ECF0;
        }
        else
        {
            ToIdle_44E6B0();
        }
    }
}

void Abe::Motion_100_SlapBomb()
{
    BaseAliveGameObject* pItem = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mSlappableOrPickupId));
    if (gAbe->GetAnimation().GetCurrentFrame() >= 6)
    {
        if (pItem)
        {
            pItem->VOnAbeInteraction();
            mSlappableOrPickupId = Guid{};
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
    }
}

void Abe::Motion_101_KnockForward()
{
    if (GetAnimation().GetCurrentFrame() == 12)
    {
        FallOnBombs_44EC10();
    }

    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);

    if (GetAnimation().GetRender())
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
            {
                mVelX = FP_FromInteger(0);
            }
            MoveWithVelocity_450FA0(FP_FromDouble(0.7));
        }
        else
        {
            Motion_3_Fall_459B60();
            if (mCurrentMotion == eAbeMotions::Motion_84_FallLandDie_45A420 || mCurrentMotion == eAbeMotions::Motion_16_LandSoft_45A360)
            {
                mCurrentMotion = eAbeMotions::Motion_101_KnockForward;
            }
        }
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if (!mbMotionChanged && (BaseAliveGameObjectCollisionLine || !GetAnimation().GetRender()))
        {
            if (mHealth > FP_FromInteger(0) || gAbeInvincible)
            {
                mCurrentMotion = eAbeMotions::jMotion_103_KnockForwardGetUp;
            }
            else
            {
                ToDieFinal_458910();
            }
        }
    }
}

void Abe::Motion_102_RollingKnockForward()
{
    Motion_74_RollingKnockback_455290();
    if (mCurrentMotion == eAbeMotions::Motion_72_KnockbackGetUp_455340)
    {
        mCurrentMotion = eAbeMotions::jMotion_103_KnockForwardGetUp;
    }
}

void Abe::jMotion_103_KnockForwardGetUp()
{
    Motion_72_KnockbackGetUp_455340();
}

void Abe::Motion_104_RockThrowStandingHold()
{
    auto pRock = static_cast<BaseThrowable*>(sObjectIds.Find_Impl(mThrowableId));
    if (GetAnimation().GetCurrentFrame() >= 4)
    {
        if (Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight | InputCommands::eUp | InputCommands::eDown))
        {
            if (Input().IsAnyHeld(InputCommands::eRight))
            {
                if (GetAnimation().GetFlipX())
                {
                    mThrowDirection = 0;
                }
                else
                {
                    mThrowDirection = 2;
                }
            }
            else if (Input().IsAnyHeld(InputCommands::eLeft))
            {
                if (GetAnimation().GetFlipX())
                {
                    mThrowDirection = 2;
                }
                else
                {
                    mThrowDirection = 0;
                }
            }
            else if (Input().IsAnyHeld(InputCommands::eUp))
            {
                mThrowDirection = 1;
            }
            else
            {
                // Down
                mThrowDirection = 3;
            }
            mCurrentMotion = eAbeMotions::Motion_105_RockThrowStandingThrow;
        }
    }

    if (!Input().IsAnyHeld(InputCommands::eThrowItem)) // ?? isn't released like in the crouching motion ??
    {
        pRock->VToDead();
        mThrowableId = Guid{};
        mCurrentMotion = eAbeMotions::Motion_106_RockThrowStandingEnd;
        if (!gInfiniteThrowables)
        {
            mBaseThrowableCount++;
        }
    }
}

void Abe::Motion_105_RockThrowStandingThrow()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::AirStream, 0, GetSpriteScale());
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_106_RockThrowStandingEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_107_RockThrowCrouchingHold()
{
    auto pRock = static_cast<BaseThrowable*>(sObjectIds.Find_Impl(mThrowableId));
    if (GetAnimation().GetCurrentFrame() >= 4)
    {
        if (Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight | InputCommands::eUp | InputCommands::eDown))
        {
            mThrowDirection = 4;
            mCurrentMotion = eAbeMotions::Motion_108_RockThrowCrouchingThrow;
            if (pRock->Type() == ReliveTypes::eMeat)
            {
                mThrowDirection = 5;
            }
        }
    }

    if (Input().IsAnyReleased(InputCommands::eThrowItem))
    {
        pRock->VToDead();
        mThrowableId = Guid{};
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
        if (!gInfiniteThrowables)
        {
            mBaseThrowableCount++;
        }
    }
}

void Abe::Motion_108_RockThrowCrouchingThrow()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::AirStream, 0, GetSpriteScale());
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
    }
}

void Abe::Motion_109_ZShotRolling()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);
    Motion_3_Fall_459B60();

    if (mCurrentMotion != eAbeMotions::Motion_109_ZShotRolling && !gAbeInvincible)
    {
        if (BaseAliveGameObject_PlatformId != Guid{})
        {
            VOnTrapDoorOpen();
        }
        mCurrentMotion = eAbeMotions::Motion_109_ZShotRolling;
        BaseAliveGameObjectCollisionLine = nullptr;
        mYPos += (GetSpriteScale() * FP_FromInteger(4));
    }

    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        if (GetAnimation().GetForwardLoopCompleted())
        {
            if (!mbMotionChanged)
            {
                mYPos += FP_FromInteger(240);
                Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
                ToDie_4588D0();
            }
        }
    }
}

void Abe::Motion_110_ZShot()
{
    EventBroadcast(Event::kEventNoise, this);
    EventBroadcast(Event::kEventSuspiciousNoise, this);
    Motion_3_Fall_459B60();

    if (mCurrentMotion != eAbeMotions::Motion_110_ZShot && !gAbeInvincible)
    {
        if (BaseAliveGameObject_PlatformId != Guid{})
        {
            BaseGameObject* pLiftPoint = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
            {
                static_cast<LiftPoint*>(pLiftPoint)->Move(FP_FromInteger(0), FP_FromInteger(0));
            }
            VOnTrapDoorOpen();
        }
        mCurrentMotion = eAbeMotions::Motion_110_ZShot;
        BaseAliveGameObjectCollisionLine = nullptr;
        mYPos += (GetSpriteScale() * FP_FromInteger(4));
    }

    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        mYPos += FP_FromInteger(240);
        Mudokon_SFX(MudSounds::eDeathDropScream_15, 0, 0, this);
        ToDie_4588D0();
    }
}

void Abe::Motion_111_PickupItem()
{
    auto pRock = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mSlappableOrPickupId));

    if (GetAnimation().GetCurrentFrame() == 7)
    {
        SfxPlayMono(relive::SoundEffects::PickupItem, 0, GetSpriteScale());
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
    }

    if (gAbe->GetAnimation().GetCurrentFrame() >= 5)
    {
        if (pRock)
        {
            pRock->VOnAbeInteraction();
            mSlappableOrPickupId = Guid{};
        }
    }
}

void Abe::Motion_112_Chant()
{
    BaseAliveGameObject* pPossessTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mPossessedObjectId));
    OrbWhirlWind* pOrbWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(mOrbWhirlWindId));

    if (field_120_state.chant != ChantStates::eWaitForUnpossessing_3 && field_120_state.chant != ChantStates::eUnpossessing_4)
    {
        SND_SEQ_PlaySeq(SeqId::MudokonChant1_10, 0, 0);
    }

    if (!pOrbWhirlWind)
    {
        mOrbWhirlWindId = Guid{};
    }

    switch (field_120_state.chant)
    {
        case ChantStates::eIdleChanting_0:
        {
            EventBroadcast(Event::kEventSpeaking, this);
            EventBroadcast(Event::kEventAbeOhm, this);

            BaseAliveGameObject* pObj = FindObjectToPossess_44B7B0(); // Find a victim.

            if (mRingPulseTimer)
            {
                if (!mHaveShrykull && !mHaveInvisibility && !mHaveHealing)
                {
                    const PSX_RECT bRect = VGetBoundingRect();

                    AbilityRing::Factory(
                        FP_FromInteger((bRect.x + bRect.w) / 2),
                        FP_FromInteger((bRect.y + bRect.h) / 2),
                        RingTypes::eExplosive_Emit_1,
                        GetSpriteScale());

                    mRingPulseTimer = 0;
                }
            }

            if (mRingPulseTimer)
            {
                if (mHaveInvisibility)
                {
                    mRingPulseTimer = 0;
                    mHaveInvisibility = 0;

                    if (mInvisibilityTimer)
                    {
                        mInvisibilityTimer += mInvisibilityDuration;
                    }
                    else
                    {
                        mInvisibilityTimer = MakeTimer(mInvisibilityDuration);
                    }

                    InvisibleEffect* pInvisible = static_cast<InvisibleEffect*>(sObjectIds.Find_Impl(mInvisibleEffectId));
                    if (!pInvisible || pInvisible->GetDead())
                    {
                        pInvisible = relive_new InvisibleEffect(this);
                        mInvisibleEffectId = pInvisible->mBaseGameObjectId;
                    }
                    pInvisible->BecomeInvisible();
                }

                if (mRingPulseTimer)
                {
                    if (mHaveHealing)
                    {
                        bool bAliveMudIsInSameScreen = false;
                        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
                        {
                            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
                            if (!pObjIter)
                            {
                                break;
                            }

                            if (pObjIter->Type() == ReliveTypes::eMudokon)
                            {
                                if (pObjIter->GetCanBePossessed()) // TODO: Is sick flag ?
                                {
                                    if (pObjIter->Is_In_Current_Camera() == CameraPos::eCamCurrent_0 && pObjIter->mHealth > FP_FromInteger(0))
                                    {
                                        bAliveMudIsInSameScreen = true;
                                    }
                                }
                            }
                        }

                        if (bAliveMudIsInSameScreen)
                        {
                            const PSX_RECT bRect = VGetBoundingRect();

                            AbilityRing::Factory(
                                FP_FromInteger((bRect.x + bRect.w) / 2),
                                FP_FromInteger((bRect.y + bRect.h) / 2),
                                RingTypes::eHealing_Emit_12,
                                GetSpriteScale());

                            mHaveHealing = false;
                            mRingPulseTimer = 0;
                        }
                    }
                }
            }

            // Stopped chanting?
            if ((GetAnimation().GetIsLastFrame() || GetAnimation().GetCurrentFrame() == 3) && !Input_IsChanting())
            {
                mCurrentMotion = eAbeMotions::Motion_113_ChantEnd;
                mPossessedObjectId = Guid{};
                if (pOrbWhirlWind)
                {
                    pOrbWhirlWind->ToStop();
                    mOrbWhirlWindId = Guid{};
                }
                return;
            }

            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }

            if (static_cast<s32>(sGnFrame) >= field_124_timer - 70)
            {
                if (pObj)
                {
                    if (!pOrbWhirlWind)
                    {
                        const FP yPos = mYPos - (GetSpriteScale() * FP_FromInteger(38));
                        const FP xOff = GetSpriteScale() * ((GetAnimation().GetFlipX()) != 0 ? FP_FromInteger(-4) : FP_FromInteger(4));
                        pOrbWhirlWind = relive_new OrbWhirlWind(
                            xOff + mXPos,
                            yPos,
                            GetSpriteScale(),
                            0);

                        mOrbWhirlWindId = pOrbWhirlWind->mBaseGameObjectId;
                    }
                }
                else
                {
                    field_124_timer = MakeTimer(70);
                    if (pOrbWhirlWind)
                    {
                        pOrbWhirlWind->ToStop();
                        pOrbWhirlWind = nullptr;
                        mOrbWhirlWindId = Guid{};
                    }
                }
            }


            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                return;
            }

            if (!pObj)
            {
                return;
            }

            mPossessedObjectId = pObj->mBaseGameObjectId;
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
            field_120_state.chant = ChantStates::ePossessVictim_1;
            field_124_timer = MakeTimer(30);

            const PSX_RECT bRect = pObj->VGetBoundingRect();
            pOrbWhirlWind->ToSpin(
                FP_FromInteger((bRect.w - bRect.x) / 2),
                FP_FromInteger((bRect.h - bRect.y) / 2),
                pObj->GetSpriteScale(),
                pObj);

            relive_new PossessionFlicker(gAbe, 30, 128, 255, 255);
        }
            return;

        case ChantStates::ePossessVictim_1:
        {
            EventBroadcast(Event::kEventSpeaking, this);
            EventBroadcast(Event::kEventAbeOhm, this);

            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                if (!pPossessTarget || pPossessTarget->GetDead() || pPossessTarget->mHealth <= FP_FromInteger(0) || pPossessTarget->Is_In_Current_Camera() != CameraPos::eCamCurrent_0)
                {
                    mCurrentMotion = eAbeMotions::Motion_113_ChantEnd;
                    mPossessedObjectId = Guid{};
                    if (pOrbWhirlWind)
                    {
                        pOrbWhirlWind->ToStop();
                        mOrbWhirlWindId = Guid{};
                    }
                }
            }
            else
            {
                field_120_state.chant = ChantStates::ePossessedVictim_2;
            }
        }
            return;

        case ChantStates::ePossessedVictim_2:
        {
            EventBroadcast(Event::kEventSpeaking, this);
            EventBroadcast(Event::kEventAbeOhm, this);
            mOrbWhirlWindId = Guid{};

            if (!pPossessTarget)
            {
                if (GetAnimation().GetIsLastFrame())
                {
                    mCurrentMotion = eAbeMotions::Motion_113_ChantEnd;
                }
                return;
            }

            if (pPossessTarget->GetDead())
            {
                mPossessedObjectId = Guid{};
            }

            if (pPossessTarget->mHealth <= FP_FromInteger(0))
            {
                if (GetAnimation().GetIsLastFrame())
                {
                    mCurrentMotion = eAbeMotions::Motion_113_ChantEnd;
                }
                return;
            }

            sControlledCharacter = pPossessTarget;

            pPossessTarget->VPossessed();

            mPossessedObjectId = Guid{};

            if (sControlledCharacter->Type() == ReliveTypes::eSlig || sControlledCharacter->Type() == ReliveTypes::eFlyingSlig || sControlledCharacter->Type() == ReliveTypes::eCrawlingSlig || sControlledCharacter->Type() == ReliveTypes::eGlukkon)
            {
                mLaughAtChantEnd = true;
            }

            relive_new PossessionFlicker(sControlledCharacter, 60, 128, 255, 255);

            SND_SEQ_Stop(SeqId::MudokonChant1_10);
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 70, 400);
            field_120_state.chant = ChantStates::eWaitForUnpossessing_3;
        }
            return;

        case ChantStates::eWaitForUnpossessing_3:
        {
            if (sControlledCharacter != this)
            {
                return;
            }

            relive_new PossessionFlicker(sControlledCharacter, 15, 128, 255, 255);

            field_120_state.chant = ChantStates::eUnpossessing_4;
            field_124_timer = MakeTimer(15);
        }
            return;

        case ChantStates::eUnpossessing_4:
        {
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }

            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                return;
            }

            if (GetAnimation().GetIsLastFrame())
            {
                mCurrentMotion = eAbeMotions::Motion_113_ChantEnd;
            }
        }
            return;

        case ChantStates::eChantingForBirdPortal_6:
        {
            EventBroadcast(Event::kEventSpeaking, this);
            EventBroadcast(Event::kEventAbeOhm, this);
            if (!(sGnFrame % 4))
            {
                New_RandomizedChant_Particle(this);
            }
        }
            return;

        default:
            return;
    }
}

void Abe::Motion_113_ChantEnd()
{
    SND_SEQ_Stop(SeqId::MudokonChant1_10);

    if (GetAnimation().GetIsLastFrame())
    {
        if (mLaughAtChantEnd)
        {
            mCurrentMotion = eAbeMotions::Motion_9_Speak_45B180;
            Mudokon_SFX(MudSounds::eGiggle_8, 0, 0, this);
            mLaughAtChantEnd = false;
        }
        else
        {
            ToIdle_44E6B0();
        }
    }
}

void Abe::Motion_114_DoorEnter()
{
    switch (field_120_state.door)
    {
        case AbeDoorStates::eAbeComesIn_0:
            if (GetAnimation().GetIsLastFrame())
            {
                field_120_state.door = AbeDoorStates::eWaitABit_2;
                GetAnimation().SetRender(false);
                field_0_abe_timer = MakeTimer(3);
            }
            return;

        case AbeDoorStates::eWaitABit_2:
            if (field_0_abe_timer <= static_cast<s32>(sGnFrame))
            {
                field_120_state.door = AbeDoorStates::eToState4_3;
                field_0_abe_timer = MakeTimer(3);
            }
            return;

        case AbeDoorStates::eToState4_3:
            if (field_0_abe_timer <= static_cast<s32>(sGnFrame))
            {
                field_120_state.door = AbeDoorStates::eSetNewActiveCamera_4;
            }
            return;

        case AbeDoorStates::eSetNewActiveCamera_4:
        {
            relive::Path_Door* pDoorTlv = static_cast<relive::Path_Door*>(gPathInfo->VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eDoor));

            BaseAliveGameObjectPathTLV = pDoorTlv;

            if (pDoorTlv->mClearThrowables)
            {
                if (mBaseThrowableCount > 0 && gThrowableArray)
                {
                    gThrowableArray->Remove(mBaseThrowableCount);
                    mBaseThrowableCount = 0;
                }
            }

            // An OWI hack. When both Mudomo and Mundanchee are done, force back to Necrum Mines.
            bool hackChange = false;
            if (gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault_Ender)
            {
                if (gMap.mCurrentPath == 13 && gMap.mCurrentCamera == 14 && mMudancheeDone)
                {
                    hackChange = true;
                }
            }
            else if (gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault_Ender)
            {
                if (gMap.mCurrentPath == 11 && gMap.mCurrentCamera == 2 && mMudomoDone)
                {
                    hackChange = true;
                }
            }

            if (hackChange)
            {
                // Plays FMV where the weirdos give Abe the drunk mud healing power and then dumps Abe at the portal that leads
                // back to Necrum mines.
                gMap.SetActiveCam(EReliveLevelIds::eNecrum, 3, 10, CameraSwapEffects::ePlay1FMV_5, 22, 0);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                mXPos = FP_FromInteger(2287);
                mYPos = FP_FromInteger(800);
                mLandSoftly = true;
                GetAnimation().SetRender(true);
                mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
                SetSpriteScale(FP_FromInteger(1));
                SetScale(Scale::Fg);
                GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
                return;
            }

            gMap.mDoorTransition = 1;
            s16 bForceChange = 0;
            const CameraSwapEffects effect = kPathChangeEffectToInternalScreenChangeEffect[pDoorTlv->mWipeEffect];
            if (effect == CameraSwapEffects::ePlay1FMV_5 || effect == CameraSwapEffects::eUnknown_11)
            {
                bForceChange = 1;
            }

            gMap.SetActiveCam(
                pDoorTlv->mNextLevel,
                pDoorTlv->mNextPath,
                pDoorTlv->mNextCamera,
                effect,
                pDoorTlv->mMovieId,
                bForceChange);

            field_120_state.door = AbeDoorStates::eSetNewAbePosition_5;
            field_1A0_door_id = pDoorTlv->mTargetDoorId;
        }
            return;

        case AbeDoorStates::eSetNewAbePosition_5:
        {
            gMap.mDoorTransition = 0;
            mCurrentLevel = gMap.mCurrentLevel;
            mCurrentPath = gMap.mCurrentPath;

            relive::Path_Door* pDoorTlv2 = static_cast<relive::Path_Door*>(gPathInfo->TLV_First_Of_Type_In_Camera(ReliveTypes::eDoor, 0));
            BaseAliveGameObjectPathTLV = pDoorTlv2;
            relive::Path_Door* pTargetDoorTlv = pDoorTlv2;
            if (pTargetDoorTlv->mDoorId != field_1A0_door_id)
            {
                do
                {
                    relive::Path_Door* pDoorIter = static_cast<relive::Path_Door*>(Path::TLV_Next_Of_Type(BaseAliveGameObjectPathTLV, ReliveTypes::eDoor));
                    BaseAliveGameObjectPathTLV = pDoorIter;
                    pTargetDoorTlv = pDoorIter;

                    if (!pTargetDoorTlv)
                    {
                        ALIVE_FATAL("Couldn't find target door. If this is a custom level, check if the level, path and camera is correct.");
                    }
                }
                while (pTargetDoorTlv->mDoorId != field_1A0_door_id);
            }

            if (pTargetDoorTlv->mScale == relive::reliveScale::eHalf)
            {
                SetSpriteScale(FP_FromDouble(0.5));
                GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
                SetScale(Scale::Bg);
            }
            else
            {
                SetSpriteScale(FP_FromDouble(1.0));
                GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
                SetScale(Scale::Fg);
            }

            // The door controls which way Abe faces when he exits it.
            if (pTargetDoorTlv->mExitDirection == relive::reliveXDirection::eRight)
            {
                GetAnimation().SetFlipX(true);
            }
            else
            {
                GetAnimation().SetFlipX(false);
            }

            mXPos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftX) + FP_FromInteger((BaseAliveGameObjectPathTLV->mBottomRightX - BaseAliveGameObjectPathTLV->mTopLeftX) / 2);


            MapFollowMe(true);

            PathLine* pathLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (gCollisions->Raycast(
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY),
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY),
                    &pathLine,
                    &hitX,
                    &hitY,
                    (GetScale() == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
            {
                BaseAliveGameObjectCollisionLine = pathLine;
                mYPos = hitY;
            }
            else
            {
                BaseAliveGameObjectCollisionLine = nullptr;
                mLandSoftly = true;
                mYPos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY);
                BaseAliveGameObjectLastLineYPos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY);
            }

            mRingPulseTimer = 0;
            InvisibleEffect* pInvisibleEffect = static_cast<InvisibleEffect*>(sObjectIds.Find_Impl(mInvisibleEffectId));
            if (pInvisibleEffect)
            {
                if (!(pInvisibleEffect->GetDead()))
                {
                    pInvisibleEffect->ClearInvisibility();
                    SetInvisible(false);
                    mInvisibleEffectId = Guid{};
                    mInvisibilityTimer = 0;
                }
            }

            mHealth = FP_FromInteger(1);
            field_120_state.door = AbeDoorStates::eAbeComesOut_6;
            field_0_abe_timer = MakeTimer(30);
        }
            return;

        case AbeDoorStates::eAbeComesOut_6:
            if (field_0_abe_timer > static_cast<s32>(sGnFrame))
            {
                return;
            }

            field_120_state.door = AbeDoorStates::eAbeComesIn_0;
            if (BaseAliveGameObjectCollisionLine)
            {
                GetAnimation().SetRender(true);
                mCurrentMotion = eAbeMotions::Motion_115_DoorExit;
            }
            else
            {
                GetAnimation().SetRender(true);
                // Abe opens the door and he falls through the floor.
                mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
            }
            return;

        default:
            return;
    }
}

void Abe::Motion_115_DoorExit()
{
    if (GetAnimation().GetIsLastFrame())
    {
        // Now that we've exited the door, should it close behind us?
        if (static_cast<relive::Path_Door*>(gPathInfo->VTLV_Get_At_Of_Type(
                                        FP_GetExponent(mXPos),
                                        FP_GetExponent(mYPos),
                                        FP_GetExponent(mXPos),
                                        FP_GetExponent(mYPos),
                                        ReliveTypes::eDoor))
                ->mCloseOnExit)
        {
            // TODO: Ret ignored even in real ??
            FindObjectOfType(
                ReliveTypes::eDoor,
                mXPos,
                mYPos - FP_FromInteger(5));

            // Yes, so find it
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == ReliveTypes::eDoor)
                {
                    Door* pDoor = static_cast<Door*>(pObj);
                    if (pDoor->mDoorId == field_1A0_door_id)
                    {
                        // And close it
                        pDoor->vClose();
                        break;
                    }
                }
            }
        }

        BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        VOnTlvCollision(BaseAliveGameObjectPathTLV);
        ToIdle_44E6B0();
    }
}

void Abe::Motion_116_MineCarEnter()
{
    if (field_120_state.raw == 0)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().SetRender(false);
            GetAnimation().SetAnimate(false);
            field_120_state.raw = 1;
            mCurrentMotion = eAbeMotions::Motion_117_InMineCar;
        }
    }
}

void Abe::Motion_117_InMineCar()
{
    if (Input().IsAnyHeld(InputCommands::eDoAction))
    {
        auto pMineCar = static_cast<MineCar*>(sControlledCharacter);
        if (pMineCar->field_11C_state == MineCarStates::eParkedWithAbe_1 && pMineCar->field_1BC_turn_direction == MineCarDirs::eUp_3)
        {
            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (gCollisions->Raycast(
                    mXPos,
                    mYPos - FP_FromInteger(2),
                    mXPos,
                    mYPos + FP_FromInteger(2),
                    &pLine,
                    &hitX,
                    &hitY,
                    GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                GetAnimation().SetAnimate(true);
                GetAnimation().SetRender(true);

                mCurrentMotion = eAbeMotions::Motion_118_MineCarExit;
            }
        }
    }
}

void Abe::Motion_118_MineCarExit()
{
    if (GetAnimation().GetForwardLoopCompleted())
    {
        mbMotionChanged = true;
        mCurrentMotion = eAbeMotions::Motion_0_Idle_44EEB0;
        field_124_timer = 1;
    }
}

void Abe::Motion_119_ToShrykull()
{
    if (field_120_state.raw == 0)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            GetAnimation().SetRender(false);
            GetAnimation().SetAnimate(false);

            field_120_state.raw = 1;

            relive_new Shrykull();
        }
    }
}

void Abe::Motion_120_EndShrykull()
{
    if (field_124_timer)
    {
        field_124_timer = field_124_timer - 1;
    }
    else if (GetAnimation().GetForwardLoopCompleted())
    {
        if (mRingPulseTimer == 0)
        {
            Free_Shrykull_Resources_45AA90();
        }

        // Abe finds transforming into a god rather funny.
        mCurrentMotion = eAbeMotions::Motion_9_Speak_45B180;
        Mudokon_SFX(MudSounds::eGiggle_8, 0, 0, this);
    }
}

void Abe::Motion_121_LiftGrabBegin()
{
    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint)
    {
        pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0));
    }

    mVelY = FP_FromInteger(0);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eAbeMotions::Motion_123_LiftGrabIdle;
    }
}

void Abe::Motion_122_LiftGrabEnd()
{
    mVelY = FP_FromInteger(0);
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_123_LiftGrabIdle()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    FollowLift_45A500();

    if (pLiftPoint)
    {
        pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0));
    }

    mVelY = FP_FromInteger(0);

    const u32 held = Input().GetHeld();
    if (InputCommands::eUp & held)
    {
        if (!pLiftPoint->OnTopFloor())
        {
            mCurrentMotion = eAbeMotions::Motion_124_LiftUseUp;
        }
    }
    else if (held & InputCommands::eDown)
    {
        if (!pLiftPoint->OnBottomFloor())
        {
            mCurrentMotion = eAbeMotions::Motion_125_LiftUseDown;
        }
    }
    else if (pLiftPoint->OnAnyFloor())
    {
        // You ain't letting go unless you are on a floor where you can walk off.
        mCurrentMotion = eAbeMotions::Motion_122_LiftGrabEnd;
    }
}

void Abe::Motion_124_LiftUseUp()
{
    mCurrentMotion = MoveLiftUpOrDown_45A7E0(FP_FromInteger(-4));
}

void Abe::Motion_125_LiftUseDown()
{
    mCurrentMotion = MoveLiftUpOrDown_45A7E0(FP_FromInteger(4));
}

void Abe::Motion_126_TurnWheelBegin()
{
    if (GetAnimation().GetIsLastFrame())
    {
        WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(mWorkWheelId));
        if (pWheel)
        {
            pWheel->VStartTurning();
        }
        mCurrentMotion = eAbeMotions::Motion_127_TurnWheelLoop;
        field_120_state.wheel = WorkWheelStates::eTurningWheel_0;
    }
}

void Abe::Motion_127_TurnWheelLoop()
{
    if (field_120_state.wheel == WorkWheelStates::eTurningWheel_0 || field_120_state.wheel == WorkWheelStates::eCheckForNoLongerTurningWheel_1) // The state we enter the main state at.
    {
        relive::Path_LevelLoader* pLevelLoader = static_cast<relive::Path_LevelLoader*>(gPathInfo->TLV_First_Of_Type_In_Camera(ReliveTypes::eLevelLoader, 0));
        if (pLevelLoader && SwitchStates_Get(pLevelLoader->mSwitchId))
        {
            field_120_state.wheel = WorkWheelStates::eMapChanging_2;
            SND_SEQ_Play(SeqId::SaveTriggerMusic_31, 1, 127, 127);
            relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, relive::Path_MusicTrigger::TriggeredBy::eTimer, 0);
            return;
        }
        else
        {
            // Must ALSO do logic below in this instance.
            field_120_state.wheel = WorkWheelStates::eCheckForNoLongerTurningWheel_1;
        }
    }

    if (field_120_state.wheel == WorkWheelStates::eCheckForNoLongerTurningWheel_1)
    {
        if (!(Input().IsAnyHeld(InputCommands::eUp)))
        {
            // Not holding up anymore, stop.
            WorkWheel* pWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(mWorkWheelId));
            if (pWheel)
            {
                pWheel->VStopTurning(1);
            }
            mWorkWheelId = Guid{};
            mCurrentMotion = eAbeMotions::Motion_128_TurnWheelEnd;
        }
    }
    else if (field_120_state.wheel == WorkWheelStates::eMapChanging_2)
    {
        // This happens for the Mines Tunnel 1 ender.
        if (!gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            // When we've changed from the camera with the wheels to tunnel 2 this forces the falling state into the well.
            // Another tasty OWI hack.
            auto pWorkWheel = static_cast<WorkWheel*>(sObjectIds.Find_Impl(mWorkWheelId));
            if (pWorkWheel) // Most likely always nullptr here, maybe the whole "stop wheel" was an inlined function.
            {
                pWorkWheel->VStopTurning(1);
            }
            mWorkWheelId = Guid{};

            auto pPathAbeStart = static_cast<relive::Path_AbeStart*>(gPathInfo->TLV_First_Of_Type_In_Camera(ReliveTypes::eAbeStart, 0));
            mXPos = FP_FromInteger((pPathAbeStart->mTopLeftX + pPathAbeStart->mBottomRightX) / 2);
            mYPos = FP_FromInteger(pPathAbeStart->mBottomRightY);
            BaseAliveGameObjectLastLineYPos = FP_FromInteger(pPathAbeStart->mBottomRightY);
            // TODO: OG bug, scale not read from the TLV ??

            GetAnimation().SetFlipX(false);
            mCurrentMotion = eAbeMotions::jMotion_85_Fall_455070;
            mLandSoftly = true;
        }
    }
}

void Abe::Motion_128_TurnWheelEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle_44E6B0();
    }
}

void Abe::Motion_129_PoisonGasDeath()
{
    // Play various choke/cough/death sounds as the dying animation progresses.
    switch (GetAnimation().GetCurrentFrame())
    {
        case 0:
            SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 128);
            break;
        case 9:
            SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 384);
            break;
        case 28:
            SFX_Play_Pitch(relive::SoundEffects::Choke, 127, 640);
            break;
        case 32:
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, 0, this);
            break;
        case 50:
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 100, -200, this);
            break;
        case 53:
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 50, -200, this);
            break;
        default:
            break;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        const s32 previousValue = field_124_timer;
        field_124_timer = previousValue - 1;
        if (previousValue == 0)
        {
            ToDieFinal_458910();
        }
    }
}

void Abe::jMotion_81_WellBegin_4017F8()
{
    Motion_78_WellBegin_45C810();
}

void Abe::FleechDeath_459350()
{
    BaseGameObject* pInvisibleEffect = sObjectIds.Find_Impl(mInvisibleEffectId);
    if (pInvisibleEffect)
    {
        if (!pInvisibleEffect->GetDead())
        {
            static_cast<InvisibleEffect*>(pInvisibleEffect)->ClearInvisibility();
            mInvisibleEffectId = Guid{};
        }
    }
    mShrivel = true;
    mCurrentMotion = eAbeMotions::Motion_58_DeadPre_4593E0;
    field_120_state.raw = 0;
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_11, this, 1, 0);
    BaseAliveGameObjectCollisionLine = nullptr;
}

void Abe::ToDie_4588D0()
{
    mShrivel = true;
    mCurrentMotion = eAbeMotions::Motion_56_DeathDropFall_4591F0;
    field_124_timer = 0;
    mHealth = FP_FromInteger(0);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 1, 0);
}

void Abe::ToIdle_44E6B0()
{
    field_8_x_vel_slow_by = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    field_124_timer = sGnFrame;
    mCurrentMotion = eAbeMotions::Motion_0_Idle_44EEB0;
    mPrevInput = 0;
    mReleasedButtons = 0;
    MapFollowMe(true);
}

void Abe::PickUpThrowabe_Or_PressBomb_454090(FP fpX, s32 fpY, s32 bStandToCrouch)
{
    BaseAliveGameObject* pSlappableOrCollectable = nullptr;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetInteractive())
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (fpX >= pAliveObj->mCollectionRect.x && fpX <= pAliveObj->mCollectionRect.w)
            {
                const FP yPos = FP_FromInteger(fpY);
                if (yPos >= pAliveObj->mCollectionRect.y && yPos <= pAliveObj->mCollectionRect.h)
                {
                    pSlappableOrCollectable = pAliveObj;
                    mSlappableOrPickupId = pAliveObj->mBaseGameObjectId;
                }
            }
        }
    }

    if (pSlappableOrCollectable)
    {
        bool trySlapOrCollect = false;
        switch (pSlappableOrCollectable->Type())
        {
            case ReliveTypes::eTimedMine_or_MovingBomb:
            case ReliveTypes::eUXB:
                mCurrentMotion = eAbeMotions::Motion_100_SlapBomb;
                if (bStandToCrouch)
                {
                    mSlappableOrPickupId = Guid{};
                }
                trySlapOrCollect = true;
                break;

            case ReliveTypes::eBone:
            case ReliveTypes::eGrenade:
            case ReliveTypes::eMeat:
            case ReliveTypes::eRock:
                mCurrentMotion = eAbeMotions::Motion_111_PickupItem;
                mBaseThrowableCount += static_cast<s8>(static_cast<BaseThrowable*>(pSlappableOrCollectable)->VGetCount()); // TODO: Check types are correct.
                if (!gThrowableIndicatorExists)
                {
                    const FP yoff = (GetSpriteScale() * FP_FromInteger(-30)) + mYPos;
                    const FP xoff = GetSpriteScale() * FP_FromInteger(0);
                    relive_new ThrowableTotalIndicator(
                        xoff + mXPos,
                        yoff,
                        GetAnimation().GetRenderLayer(),
                        GetAnimation().GetSpriteScale(),
                        mBaseThrowableCount,
                        1);
                }
                trySlapOrCollect = true;
                break;

            case ReliveTypes::eMine:
                mSlappableOrPickupId = Guid{};
                trySlapOrCollect = true;
                break;
            default:
                break;
        }

        if (trySlapOrCollect)
        {
            if (mCurrentMotion == eAbeMotions::Motion_111_PickupItem)
            {
                if (bStandToCrouch)
                {
                    SfxPlayMono(relive::SoundEffects::PickupItem, 0, GetSpriteScale());
                    pSlappableOrCollectable->VOnAbeInteraction();
                    mSlappableOrPickupId = Guid{};
                    mCurrentMotion = eAbeMotions::Motion_17_CrouchIdle_456BC0;
                }
            }
        }
    }
}

s16 Abe::ToLeftRightMovement_44E340()
{
    mVelY = FP_FromInteger(0);
    if (sControlledCharacter != this)
    {
        return 0;
    }

    const u32 held = Input().GetHeld();
    const FP gridSize = ScaleToGridSize(GetSpriteScale());
    const bool flipX = GetAnimation().GetFlipX();

    if ((flipX && (held & InputCommands::eRight)) || (!flipX && (held & InputCommands::eLeft)))
    {
        mCurrentMotion = eAbeMotions::Motion_2_StandingTurn_451830;
        return 1;
    }

    if ((held & InputCommands::eRight) || (held & InputCommands::eLeft))
    {
        const FP directionX = FP_FromInteger((held & InputCommands::eRight) ? 1 : -1);

        if (held & InputCommands::eRun)
        {
            mVelX = directionX * (gridSize / FP_FromInteger(4));
            mCurrentMotion = eAbeMotions::Motion_39_StandingToRun_450D40;
        }
        else if (held & InputCommands::eSneak)
        {
            mVelX = directionX * (gridSize / FP_FromInteger(10));
            mCurrentMotion = eAbeMotions::Motion_45_SneakBegin_4507A0;
        }
        else
        {
            mVelX = directionX * (gridSize / FP_FromInteger(9));
            mCurrentMotion = eAbeMotions::Motion_6_WalkBegin_44FEE0;
        }

        if (!WallHit(GetSpriteScale() * FP_FromInteger(50), directionX * gridSize))
        {
            if (!WallHit(GetSpriteScale() * FP_FromInteger(20), directionX * gridSize))
            {
                return 1;
            }
        }

        // Walking into wall?
        if (WallHit(GetSpriteScale() * FP_FromInteger(20), directionX * gridSize))
        {
            PushWall_44E890();
            return 0;
        }

        mCurrentMotion = eAbeMotions::Motion_19_StandToCrouch_453DC0;
        return 1;
    }

    return 0;
}

void Abe::TryHoist_44ED30()
{
    mCurrentMotion = eAbeMotions::Motion_13_HoistBegin_452B20;

    const FP xpos = mXPos;
    const FP ypos = mYPos - FP_FromInteger(10); // Look up 10 for a hoist.

    relive::Path_Hoist* pHoist = static_cast<relive::Path_Hoist*>(gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(xpos),
        FP_GetExponent(ypos),
        FP_GetExponent(xpos),
        FP_GetExponent(ypos),
        ReliveTypes::eHoist));

    if (pHoist)
    {
        if (IsSameScaleAsHoist(pHoist, this))
        {
            if (!IsFacingSameDirectionAsHoist(pHoist, this) && pHoist->mGrabDirection != relive::Path_Hoist::GrabDirection::eFacingAnyDirection)
            {
                // No, so auto turn around to face it.
                mNextMotion = mCurrentMotion;
                mCurrentMotion = eAbeMotions::Motion_2_StandingTurn_451830;
            }
        }

        BaseAliveGameObjectPathTLV = pHoist;
    }
}

void Abe::Create_Fart_421D20()
{
    relive_new EvilFart();
}

s16 Abe::TryEnterMineCar_4569E0()
{
    if (Input().IsAnyHeld(InputCommands::eUp))
    {
        for (s32 idx = 0; idx < gBaseAliveGameObjects->Size(); idx++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (pObj->GetAnimation().GetRender())
            {
                const PSX_RECT abeRect = VGetBoundingRect();
                const PSX_RECT mineCarRect = pObj->VGetBoundingRect();

                if (PSX_Rects_overlap_no_adjustment(&abeRect, &mineCarRect) && pObj->GetSpriteScale() == GetSpriteScale() && pObj->Type() == ReliveTypes::eMineCar)
                {
                    const FP distanceCheck = ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(0.5);
                    if (mXPos - pObj->mXPos < distanceCheck)
                    {
                        if (pObj->mXPos - mXPos < distanceCheck)
                        {
                            field_120_state.raw = 0;
                            mCurrentMotion = eAbeMotions::Motion_116_MineCarEnter;
                            mXPos = FP_FromInteger((mineCarRect.x + mineCarRect.w) / 2);
                            MapFollowMe(true);
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

s32 Abe::NearDoorIsOpen_44EE10()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eDoor)
        {
            auto pDoor = static_cast<Door*>(pObj);
            if (FP_Abs(mXPos - pDoor->mXPos) < FP_FromInteger(15) && FP_Abs(mYPos - pDoor->mYPos) < FP_FromInteger(20))
            {
                return pDoor->vIsOpen();
            }
        }
    }
    // We didn't find a door - so for some reason that makes no sense return that it is open...
    return true;
}

eAbeMotions Abe::HandleDoAction_455BD0()
{
    relive::Path_TLV* pTlv = gPathInfo->TLV_Get_At(
        nullptr,
        mXPos,
        mYPos,
        mXPos,
        mYPos);

    if (!pTlv)
    {
        // Why ya trying to use nothing?
        return eAbeMotions::Motion_34_DunnoBegin_44ECF0;
    }

    for (;;)
    {
        switch (pTlv->mTlvType)
        {
            case ReliveTypes::eWellLocal:
                BaseAliveGameObjectPathTLV = pTlv;
                return eAbeMotions::Motion_78_WellBegin_45C810;

            case ReliveTypes::eLever:
            {
                FP xpos = {};
                FP ypos = {};
                if (GetAnimation().GetFlipX())
                {
                    xpos = mYPos - FP_FromInteger(5);
                    ypos = mXPos - ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    xpos = mYPos - FP_FromInteger(5);
                    ypos = ScaleToGridSize(GetSpriteScale()) + mXPos;
                }

                Lever* pSwitch = static_cast<Lever*>(FindObjectOfType(ReliveTypes::eLever, ypos, xpos));
                if (!pSwitch || !(pSwitch->VPull(mXPos < pSwitch->mXPos)))
                {
                    return eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                }

                return eAbeMotions::Motion_99_LeverUse;
            }

            case ReliveTypes::eWellExpress:
                BaseAliveGameObjectPathTLV = pTlv;
                return eAbeMotions::Motion_78_WellBegin_45C810;

            case ReliveTypes::eBoomMachine:
            {
                auto pGrenadeMachine = static_cast<BoomMachine*>(FindObjectOfType(ReliveTypes::eBoomMachine, mXPos, mYPos - (GetSpriteScale() * FP_FromInteger(25))));
                if (!pGrenadeMachine || !(pGrenadeMachine->VIsButtonOn()))
                {
                    return eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                }
                pGrenadeMachine->VHandleButton();
                return eAbeMotions::Motion_88_GrenadeMachineUse;
            }

            default:
                // Next TLV, exit if we're at the end.
                pTlv = gPathInfo->TLV_Get_At(
                    pTlv,
                    mXPos,
                    mYPos,
                    mXPos,
                    mYPos);

                if (!pTlv)
                {
                    return eAbeMotions::Motion_34_DunnoBegin_44ECF0;
                }
                continue;
        }
    }
}

void Abe::PushWall_44E890()
{
    mVelX = FP_FromInteger(0);
    mCurrentMotion = eAbeMotions::Motion_73_PushWall_4553A0;
    Environment_SFX(EnvironmentSfx::eGenericMovement_9, 0, 32767, this);
}

void Abe::MoveForward_44E9A0()
{
    const FP oldXPos = mXPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mXPos,
            &mYPos,
            mVelX);
    }

    PlatformBase* pTrapdoor = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (BaseAliveGameObjectCollisionLine && (GetScale() == Scale::Fg ? kFgFloor : kBgFloor).Mask() == CollisionMask(BaseAliveGameObjectCollisionLine->mLineType).Mask())
    {
        // Handle trap door collision.
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            if (pTrapdoor)
            {
                pTrapdoor->VRemove(this);
                BaseAliveGameObject_PlatformId = Guid{};
            }

            const PSX_RECT bRect = VGetBoundingRect();
            OnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {static_cast<s16>(bRect.w + 5), 
                 static_cast<s16>(bRect.h + 5)}, // TODO: Is it really on both ??
                gPlatformsArray);
        }
        else if (pTrapdoor)
        {
            pTrapdoor->VRemove(this);
            BaseAliveGameObject_PlatformId = Guid{};
        }
    }
    else
    {
        BaseAliveGameObjectCollisionLine = 0;
        if (pTrapdoor)
        {
            pTrapdoor->VRemove(this);
            BaseAliveGameObject_PlatformId = Guid{};
        }

        mPrevInput = 0;

        switch (mCurrentMotion)
        {
            case eAbeMotions::Motion_1_WalkLoop_44FBA0:
            case eAbeMotions::Motion_6_WalkBegin_44FEE0:
            case eAbeMotions::Motion_4_WalkToIdle_44FFC0:
            case eAbeMotions::Motion_5_MidWalkToIdle_450080:
                mCurrentMotion = eAbeMotions::Motion_93_WalkOffEdge;
                break;

            case eAbeMotions::Motion_33_RunLoop_4508E0:
            case eAbeMotions::Motion_39_StandingToRun_450D40:
                mCurrentMotion = eAbeMotions::Motion_94_RunOffEdge;
                break;

            case eAbeMotions::jMotion_38_RunToRoll_453A70:
            case eAbeMotions::Motion_22_RollBegin_4539A0:
            case eAbeMotions::Motion_23_RollLoop_453A90:
            case eAbeMotions::Motion_24_453D00:
                mCurrentMotion = eAbeMotions::Motion_98_RollOffEdge;
                break;

            default:
                mCurrentMotion = eAbeMotions::Motion_95_SneakOffEdge;
                break;
        }

        field_8_x_vel_slow_by = FP_FromDouble(0.3); // TODO: Check.
        mXPos = oldXPos + mVelX;
        BaseAliveGameObjectLastLineYPos = mYPos;

        // TODO: OG bug, dead code due to switch default case?
        if (mCurrentMotion == eAbeMotions::Motion_71_Knockback_455090 || mCurrentMotion == eAbeMotions::Motion_101_KnockForward)
        {
            field_8_x_vel_slow_by = FP_FromDouble(0.67); // TODO: Check.
        }
    }
}

s16 Abe::CrouchingGameSpeak_453E10()
{
    mPrevInput |= Input().GetPressed();

    if (InputCommands::eGameSpeak2 & mPrevInput)
    {
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_FollowMe);
        Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
        mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        return true;
    }
    else if (InputCommands::eGameSpeak3 & mPrevInput)
    {
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Wait);
        Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
        mCurrentMotion = eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0;
        return true;
    }
    else if (mPrevInput & InputCommands::eGameSpeak1)
    {
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Hello);

        if (mMood == Mud_Emotion::eHappy_5 || mMood == Mud_Emotion::eWired_6)
        {
            Mudokon_SFX(MudSounds::eHiHappy_19, 0, 0, this);
            mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        }
        else
        {
            if (mMood == Mud_Emotion::eSad_3)
            {
                Mudokon_SFX(MudSounds::eHiSad_20, 0, 0, this);
            }
            else
            {
                Mudokon_SFX(MudSounds::eHelloNeutral_3, 0, 0, this);
            }
            mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        }
        return true;
    }
    else if (mPrevInput & InputCommands::eGameSpeak4)
    {
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Work);
        Mudokon_SFX(MudSounds::eWork_25, 0, 0, this);
        mCurrentMotion = eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0;
        return true;
    }
    else if (mPrevInput & InputCommands::eGameSpeak6)
    {
        Mudokon_SFX(MudSounds::eAllOYa_17, 0, 0, this);
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_AllYa);
        mCurrentMotion = eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0;
        return true;
    }
    else if (mPrevInput & InputCommands::eGameSpeak5)
    {
        Mudokon_SFX(MudSounds::eAnger_5, 0, 0, this);
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Anger);
        mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        return true;
    }
    else if (mPrevInput & InputCommands::eGameSpeak8)
    {
        Mudokon_SFX(MudSounds::eStopIt_26, 0, 0, this);
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_StopIt);
        mCurrentMotion = eAbeMotions::jMotion_21_ToCrouchSpeak_4545E0;
        return true;
    }
    else if (mPrevInput & InputCommands::eGameSpeak7)
    {
        Mudokon_SFX(MudSounds::eSadUgh_28, 0, 0, this);
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Sorry);
        mCurrentMotion = eAbeMotions::Motion_20_CrouchSpeak_454550;
        return true;
    }
    else
    {
        return false;
    }
}

bool Abe::Is_Celling_Above_44E8D0()
{
    FP hitY = {};
    FP hitX = {};
    PathLine* pLine = nullptr;
    return gCollisions->Raycast(
               mXPos,
               mYPos - FP_FromInteger(5),
               mXPos,
               mYPos - (GetSpriteScale() * FP_FromInteger(45)),
               &pLine,
               &hitX,
               &hitY,
               GetScale() == Scale::Fg ? kFgCeiling : kBgCeiling)
        != 0;
}

void Abe::MoveWithVelocity_450FA0(FP velocityX)
{
    if (mVelX > FP_FromInteger(0))
    {
        const FP newVelX = mVelX - (GetSpriteScale() * velocityX);
        mVelX = newVelX;
        if (newVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        const FP newVelX = (GetSpriteScale() * velocityX) + mVelX;
        mVelX = newVelX;
        if (newVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    if (FP_GetExponent(mVelX))
    {
        MoveForward_44E9A0();
    }
}

s16 Abe::RunTryEnterDoor_451220()
{
    // Can't be entering a door if we're not pressing up.
    if (!Input().IsAnyHeld(InputCommands::eUp))
    {
        return 0;
    }

    if (GetElectrocuted())
    {
        return 0;
    }

    if (GetAnimation().GetCurrentFrame() < 4)
    {
        return 0;
    }

    // Are we actually on a door?
    relive::Path_TLV* pDoorTlv = gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eDoor);

    if (!pDoorTlv)
    {
        return 0;
    }

    if (!NearDoorIsOpen_44EE10())
    {
        return 0;
    }

    BaseAliveGameObjectPathTLV = pDoorTlv;
    field_120_state.raw = 0;
    mCurrentMotion = eAbeMotions::Motion_114_DoorEnter;
    mXPos = FP_FromInteger((pDoorTlv->mTopLeftX + pDoorTlv->mBottomRightX) / 2);
    MapFollowMe(true);
    return 1;
}

s16 Abe::RunTryEnterWell_451060()
{
    if (!Input().IsAnyHeld(InputCommands::eUp) || GetElectrocuted() || GetAnimation().GetCurrentFrame() < 4)
    {
        return 0;
    }

    auto pWellLocal = static_cast<relive::Path_WellLocal*>(gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellLocal));
    if (pWellLocal)
    {
        if (!(GetTeleporting()))
        {
            if ((pWellLocal->mScale == relive::reliveScale::eFull && GetSpriteScale() == FP_FromInteger(1)) || (pWellLocal->mScale == relive::reliveScale::eHalf && GetSpriteScale() == FP_FromDouble(0.5)))
            {
                BaseAliveGameObjectPathTLV = pWellLocal;
                mCurrentMotion = eAbeMotions::Motion_78_WellBegin_45C810;
                return 1;
            }
        }
    }

    auto pWellExpress = static_cast<relive::Path_WellExpress*>(gPathInfo->VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eWellExpress));
    if (pWellExpress)
    {
        if (!(GetTeleporting()))
        {
            if ((pWellExpress->mScale == relive::reliveScale::eFull && GetSpriteScale() == FP_FromInteger(1)) || (pWellExpress->mScale == relive::reliveScale::eHalf && GetSpriteScale() == FP_FromDouble(0.5)))
            {
                BaseAliveGameObjectPathTLV = pWellExpress;
                mCurrentMotion = eAbeMotions::jMotion_81_WellBegin_45C7F0;
                return 1;
            }
        }
    }

    return 0;
}

void Abe::ToDieFinal_458910()
{
    InvisibleEffect* pInvisibleEffect = static_cast<InvisibleEffect*>(sObjectIds.Find_Impl(mInvisibleEffectId));
    if (pInvisibleEffect)
    {
        if (!(pInvisibleEffect->GetDead()))
        {
            pInvisibleEffect->ClearInvisibility();
            mInvisibleEffectId = Guid{};
        }
    }

    mShrivel = true;
    mbGotShot = false;
    mCurrentMotion = eAbeMotions::Motion_57_Dead_4589A0;
    field_124_timer = 0;
    mHealth = FP_FromInteger(0);

    MusicController::static_PlayMusic(MusicController::MusicTypes::eDeathLong_11, this, 1, 0);
}

eAbeMotions Abe::DoGameSpeak_45AB70(s32 input)
{
    eAbeMotions nextMotion = eAbeMotions::None_m1;
    if (Input_IsChanting())
    {
        // Fixes an OG bug where Abe doesn't transform into Shrykull when you immediately chant after using GameSpeak.
        if (!(mRingPulseTimer && mHaveShrykull))
        {
            field_124_timer = MakeTimer(90);
            SND_SEQ_PlaySeq(SeqId::MudokonChant1_10, 0, 1);
            field_120_state.chant = ChantStates::eIdleChanting_0;
            nextMotion = eAbeMotions::Motion_112_Chant;
        }
    }
    else if (input & InputCommands::eGameSpeak2)
    {
        Mudokon_SFX(MudSounds::eFollowMe_4, 0, 0, this);
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_FollowMe);
        nextMotion = eAbeMotions::Motion_7_Speak_45B140;
    }
    else if (input & InputCommands::eGameSpeak3)
    {
        Mudokon_SFX(MudSounds::eWait_6, 0, 0, this);
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Wait);
        nextMotion = eAbeMotions::Motion_8_Speak_45B160;
    }
    else if (input & InputCommands::eGameSpeak1)
    {
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Hello);
        if (mMood == Mud_Emotion::eHappy_5 || mMood == Mud_Emotion::eWired_6)
        {
            Mudokon_SFX(MudSounds::eHiHappy_19, 0, 0, this);
            nextMotion = eAbeMotions::Motion_9_Speak_45B180;
        }
        else
        {
            if (mMood == Mud_Emotion::eSad_3)
            {
                Mudokon_SFX(MudSounds::eHiSad_20, 0, 0, this);
            }
            else
            {
                Mudokon_SFX(MudSounds::eHelloNeutral_3, 0, 0, this);
            }
            nextMotion = eAbeMotions::Motion_9_Speak_45B180;
        }
    }
    else if (input & InputCommands::eGameSpeak4)
    {
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Work);
        Mudokon_SFX(MudSounds::eWork_25, 0, 0, this);
        nextMotion = eAbeMotions::Motion_10_Fart_45B1A0;
    }
    else if (input & InputCommands::eGameSpeak6)
    {
        Mudokon_SFX(MudSounds::eAllOYa_17, 0, 0, this);
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_AllYa);
        nextMotion = eAbeMotions::Motion_9_Speak_45B180;
    }
    else if (input & InputCommands::eGameSpeak5)
    {
        FP gridSize = {};
        if (GetAnimation().GetFlipX())
        {
            gridSize = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize = ScaleToGridSize(GetSpriteScale());
        }

        if (FindObjectOfType(ReliveTypes::eMudokon, mXPos + gridSize, mYPos - FP_FromInteger(5)))
        {
            nextMotion = eAbeMotions::Motion_62_Punch_454750;
        }
        else if (FindObjectOfType(ReliveTypes::eMudokon, mXPos + (gridSize * FP_FromInteger(2)), mYPos - FP_FromInteger(5)))
        {
            nextMotion = eAbeMotions::Motion_62_Punch_454750;
        }
        else
        {
            // NOTE: Extra check for locks, it must also be being rendered in order to for us to try to hit it.
            BaseAnimatedWithPhysicsGameObject* pLockedSoul = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eSlapLock, mXPos + gridSize, mYPos - (FP_FromInteger(30) * GetSpriteScale())));
            if (pLockedSoul && pLockedSoul->GetAnimation().GetRender())
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else if (FindObjectOfType(ReliveTypes::eSlig, mXPos + gridSize, mYPos - FP_FromInteger(5)))
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else if (FindObjectOfType(ReliveTypes::eSlig, mXPos + (gridSize * FP_FromInteger(2)), mYPos - FP_FromInteger(5)))
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else if (FindObjectOfType(ReliveTypes::eGlukkon, mXPos + gridSize, mYPos - FP_FromInteger(5)))
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else if (FindObjectOfType(ReliveTypes::eGlukkon, mXPos + (gridSize * FP_FromInteger(2)), mYPos - FP_FromInteger(5)))
            {
                nextMotion = eAbeMotions::Motion_62_Punch_454750;
            }
            else
            {
                // Didn't hit anything, just anger.
                Mudokon_SFX(MudSounds::eAnger_5, 0, 0, this);
                gEventSystem->PushEvent(GameSpeakEvents::eAbe_Anger);
                nextMotion = eAbeMotions::Motion_8_Speak_45B160;
            }
        }
    }
    else if (input & InputCommands::eGameSpeak8)
    {
        Mudokon_SFX(MudSounds::eStopIt_26, 0, 0, this);
        gEventSystem->PushEvent(GameSpeakEvents::eAbe_StopIt);
        nextMotion = eAbeMotions::Motion_7_Speak_45B140;
    }
    else if (input & InputCommands::eGameSpeak7)
    {
        FP gridSize = {};
        if (GetAnimation().GetFlipX())
        {
            gridSize = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize = ScaleToGridSize(GetSpriteScale());
        }

        gEventSystem->PushEvent(GameSpeakEvents::eAbe_Sorry);
        if (FindObjectOfType(ReliveTypes::eMudokon, mXPos + gridSize, mYPos - (GetSpriteScale() * FP_FromInteger(40))))
        {
            nextMotion = eAbeMotions::Motion_63_Sorry_454670;
        }
        else
        {
            Mudokon_SFX(MudSounds::eSadUgh_28, 0, 0, this);
            nextMotion = eAbeMotions::Motion_10_Fart_45B1A0; // TODO: Correct but isn't fart in this case ??
        }
    }

    return nextMotion;
}

s16 Abe::CantBeDamaged_44BAB0()
{
    switch (mCurrentMotion)
    {
        case eAbeMotions::Motion_75_JumpIntoWell_45C7B0:
        case eAbeMotions::Motion_76_ToInsideOfAWellLocal_45CA40:
        case eAbeMotions::Motion_77_ToWellShotOut_45D130:
        case eAbeMotions::Motion_78_WellBegin_45C810:
        case eAbeMotions::Motion_79_InsideWellLocal_45CA60:
        case eAbeMotions::Motion_80_WellShotOut_45D150:
        case eAbeMotions::jMotion_81_WellBegin_45C7F0:
        case eAbeMotions::Motion_82_InsideWellExpress_45CC80:
        case eAbeMotions::Motion_83_WellExpressShotOut_45CF70:
        case eAbeMotions::Motion_114_DoorEnter:
        case eAbeMotions::Motion_115_DoorExit:
        case eAbeMotions::Motion_119_ToShrykull:
        case eAbeMotions::Motion_120_EndShrykull:
            return true;
    }

    // TODO: Unknown what this is checking, condition should probably be inverted.
    if ((!mShrivel && GetAnimation().GetRender()) || GetElectrocuted())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Abe::FallOnBombs_44EC10()
{
    const PSX_RECT bOurRect = VGetBoundingRect();
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eMine || pObj->Type() == ReliveTypes::eUXB)
        {
            const PSX_RECT objRect = pObj->VGetBoundingRect();
            if (bOurRect.x <= objRect.w && bOurRect.w >= objRect.x && bOurRect.h >= objRect.y && bOurRect.y <= objRect.h)
            {
                pObj->VTakeDamage(this);
            }
        }
    }
}

s16 Abe::ForceDownIfHoisting_44BA30()
{
    if (mCurrentMotion != eAbeMotions::Motion_65_LedgeAscend_4548E0 && mCurrentMotion != eAbeMotions::Motion_67_LedgeHang_454E20 && mCurrentMotion != eAbeMotions::Motion_69_LedgeHangWobble_454EF0 && mCurrentMotion != eAbeMotions::Motion_66_LedgeDescend_454970 && mCurrentMotion != eAbeMotions::Motion_68_ToOffScreenHoist_454B80)
    {
        return 0;
    }

    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;

    field_124_timer = 0;

    mbGotShot = false;
    mbMotionChanged = true;

    return 1;
}

void Abe::BulletDamage_44C980(Bullet* pBullet)
{
    const PSX_RECT rect = VGetBoundingRect();

    enum class ShootKind
    {
        eEverythingElse_0 = 0,
        eHanging_1 = 1,
        eRolling_2 = 2,
    };

    if (Is_In_Current_Camera() != CameraPos::eCamCurrent_0 || mHealth <= FP_FromInteger(0))
    {
        return;
    }

    ShootKind shootKind = ShootKind::eEverythingElse_0;
    if (mCurrentMotion == eAbeMotions::Motion_22_RollBegin_4539A0 || mCurrentMotion == eAbeMotions::Motion_23_RollLoop_453A90 || mCurrentMotion == eAbeMotions::Motion_24_453D00 || mCurrentMotion == eAbeMotions::Motion_37_CrouchTurn_454390 || mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0)
    {
        shootKind = ShootKind::eRolling_2;
    }
    else if (mCurrentMotion == eAbeMotions::Motion_65_LedgeAscend_4548E0 || mCurrentMotion == eAbeMotions::Motion_67_LedgeHang_454E20 || mCurrentMotion == eAbeMotions::Motion_69_LedgeHangWobble_454EF0 || mCurrentMotion == eAbeMotions::Motion_66_LedgeDescend_454970 || mCurrentMotion == eAbeMotions::Motion_68_ToOffScreenHoist_454B80)
    {
        shootKind = ShootKind::eHanging_1;
    }

    mHealth -= FP_FromDouble(0.34);
    if (mHealth > FP_FromInteger(0))
    {
        FP xOffset = {};
        if (mHealth <= FP_FromDouble(0.5))
        {
            if (pBullet->XDistance() > FP_FromInteger(0))
            {
                xOffset = mXPos - ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                xOffset = ScaleToGridSize(GetSpriteScale()) + mXPos;
            }
        }
        else
        {
            if (pBullet->XDistance() > FP_FromInteger(0))
            {
                xOffset = mXPos - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2));
            }
            else
            {
                xOffset = mXPos + (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2));
            }
        }

        PathLine* pathLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (gCollisions->Raycast(
                xOffset,
                mYPos - FP_FromInteger(5),
                xOffset,
                mYPos + FP_FromInteger(5),
                &pathLine,
                &hitX,
                &hitY,
                GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
            == 1)
        {
            if (pBullet->mBulletType != BulletType::ePossessedSligZBullet_1 && pBullet->mBulletType != BulletType::eZBullet_3)
            {
                relive_new Spark(hitX, hitY, GetSpriteScale(), 9, -31, 159, SparkType::eSmallChantParticle_0);
                New_Smoke_Particles(hitX, hitY, GetSpriteScale(), 3, RGB16{ 128, 128, 128 });
            }
        }
        return;
    }

    mbGotShot = true;

    switch (pBullet->mBulletType)
    {
        case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
        case BulletType::eNormalBullet_2:
        {
            FP bloodXOffset = {};
            if (pBullet->XDistance() <= FP_FromInteger(0))
            {
                bloodXOffset = FP_FromInteger(-24);
            }
            else
            {
                bloodXOffset = FP_FromInteger(24);
            }
            
            relive_new Blood(
                mXPos,
                pBullet->ypos(),
                bloodXOffset,
                FP_FromInteger(0),
                GetSpriteScale(),
                50);

            switch (shootKind)
            {
                case ShootKind::eEverythingElse_0:
                {
                    ToKnockback_44E700(1, 1);
                    if (GetAnimation().GetFlipX() != (pBullet->XDistance() > FP_FromInteger(0)))
                    {
                        mCurrentMotion = eAbeMotions::Motion_101_KnockForward;
                    }
                    mbMotionChanged = true;
                    mbGotShot = false;
                    mVelX = GetSpriteScale() * FP_FromDouble(7.8);
                    if (pBullet->XDistance() < FP_FromInteger(0))
                    {
                        mVelX = -GetSpriteScale();
                    }
                    break;
                }
                case ShootKind::eHanging_1:
                {
                    mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                    field_124_timer = 0;
                    mbGotShot = false;
                    mbMotionChanged = true;
                    break;
                }
                case ShootKind::eRolling_2:
                {
                    if (GetAnimation().GetFlipX() == (pBullet->XDistance() > FP_FromInteger(0)))
                    {
                        mNextMotion = eAbeMotions::Motion_74_RollingKnockback_455290;
                    }
                    else
                    {
                        mNextMotion = eAbeMotions::Motion_102_RollingKnockForward;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }

        case BulletType::ePossessedSligZBullet_1:
        case BulletType::eZBullet_3:
        {
            if (GetSpriteScale() == FP_FromDouble(0.5))
            {
                mHealth = FP_FromInteger(1);
                mbGotShot = false;
                return;
            }

            const FP boundsY = FP_FromInteger(rect.y);
            if (Bullet::InZBulletCover(mXPos, boundsY, rect) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, boundsY, 0))
            {
                mbGotShot = false;
                mHealth = FP_FromInteger(1);
                return;
            }

            FP yOffset = {};
            if (shootKind == ShootKind::eEverythingElse_0)
            {
                yOffset = (FP_FromInteger(-45) * GetSpriteScale());
                mNextMotion = eAbeMotions::Motion_110_ZShot;
            }
            else if (shootKind == ShootKind::eHanging_1)
            {
                yOffset = (FP_FromInteger(45) * GetSpriteScale());
                mCurrentMotion = eAbeMotions::Motion_92_ForceDownFromHoist;
                mbGotShot = false;
                mbMotionChanged = true;
                field_124_timer = 0;
            }
            else if (shootKind == ShootKind::eRolling_2)
            {
                yOffset = (FP_FromInteger(-25) * GetSpriteScale());
                mNextMotion = eAbeMotions::Motion_109_ZShotRolling;
            }

            relive_new Blood(mXPos, yOffset + mYPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 50);
            break;
        }

        default:
            break;
    }

    if (mbGotShot)
    {
        mKnockdownMotion = mNextMotion;
    }

    Environment_SFX(EnvironmentSfx::eElumHitWall_14, 0, 32767, this);
    Mudokon_SFX(MudSounds::eHurt2_9, 127, 0, this);
    Environment_SFX(EnvironmentSfx::eDeathNoise_7, 0, 32767, this);
    SFX_Play_Pitch(relive::SoundEffects::Eating1, 0, -500, GetSpriteScale());
    SfxPlayMono(relive::SoundEffects::KillEffect, 0, GetSpriteScale());
}

void Abe::GiveControlBackToMe_44BA10()
{
    sControlledCharacter = this;
    mPreventChanting = true;
}

PullRingRope* Abe::GetPullRope()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find a rope.
        if (pObj->Type() == ReliveTypes::ePullRingRope)
        {
            // Is it on the same scale as us?
            PullRingRope* pRope = static_cast<PullRingRope*>(pObj);
            if (pRope->GetSpriteScale() == GetSpriteScale())
            {
                const PSX_RECT bRect = pRope->VGetBoundingRect();

                // Check we are near its ypos.
                if ((mYPos - (GetSpriteScale() * FP_FromInteger(75))) <= pRope->mYPos && mYPos > pRope->mYPos)
                {
                    // Check we are near its xpos.
                    if (mXPos > FP_FromInteger(bRect.x) && mXPos < FP_FromInteger(bRect.w))
                    {
                        // Found a rope we can pull.
                        return pRope;
                    }
                }
            }
        }
    }
    return nullptr;
}

void Abe::IntoPortalStates_451990()
{
    auto pBirdPortal = static_cast<BirdPortal*>(sObjectIds.Find_Impl(mBirdPortalId));
    if (pBirdPortal)
    {
        PSX_RECT bRect = {};
        switch (mBirdPortalSubState)
        {
            case PortalSubStates::eJumpingInsidePortal_0:
                bRect = VGetBoundingRect();
                if ((mVelX > FP_FromInteger(0) && FP_FromInteger(bRect.x) > pBirdPortal->mXPos) || (mVelX < FP_FromInteger(0) && FP_FromInteger(bRect.w) < pBirdPortal->mXPos))
                {
                    GetAnimation().SetRender(false);
                    mVelY = FP_FromInteger(0);
                    mVelX = FP_FromInteger(0);
                    pBirdPortal->DestroyPortalClippers();
                    pBirdPortal->VGiveShrykull(true);
                    mBirdPortalSubState = PortalSubStates::eSetNewActiveCamera_1;
                }

                mVelY += GetSpriteScale() * FP_FromDouble(1.8);
                mXPos += mVelX;
                mYPos += mVelY;
                return;

            case PortalSubStates::eSetNewActiveCamera_1:
                if (pBirdPortal->IsAbeInsidePortalState())
                {
                    EReliveLevelIds level = {};
                    u16 path = 0;
                    u16 camera = 0;
                    CameraSwapEffects screenChangeEffect = {};
                    u16 movieId = 0;

                    pBirdPortal->VGetMapChange(&level, &path, &camera, &screenChangeEffect, &movieId);
                    gMap.SetActiveCam(level, path, camera, screenChangeEffect, movieId, false);
                    mBirdPortalSubState = PortalSubStates::eSetNewAbePosition_4;
                }
                break;

            case PortalSubStates::eHopOutOfPortal_2:
                if (pBirdPortal->IsAbExittingPortalState())
                {
                    pBirdPortal->ClipPortal(0);
                    GetAnimation().SetRender(true);
                    mCurrentMotion = eAbeMotions::Motion_27_HopBegin_4521C0;
                    pBirdPortal->RemovePortalClipperDelayed();
                    mBirdPortalId = Guid{};
                }
                break;

            case PortalSubStates::eSetNewAbePosition_4:
                pBirdPortal->VExitPortal();
                mBirdPortalSubState = PortalSubStates::eHopOutOfPortal_2;

                GetAnimation().SetFlipX(pBirdPortal->mEnterSide == relive::Path_BirdPortal::PortalSide::eLeft);

                if (GetAnimation().GetFlipX())
                {
                    mXPos = ScaleToGridSize(GetSpriteScale()) + pBirdPortal->mExitX;
                }
                else
                {
                    mXPos = pBirdPortal->mExitX - ScaleToGridSize(GetSpriteScale());
                }
                mYPos = pBirdPortal->mExitY;
                BaseAliveGameObjectLastLineYPos = pBirdPortal->mExitY;
                mVelY = FP_FromInteger(0);
                field_8_x_vel_slow_by = FP_FromInteger(0);
                break;

            default:
                return;
        }
    }
}

void Abe::Calc_Well_Velocity_45C530(s16 xPosSource, s16 yPosSource, s16 xPosDest, s16 yPosDest)
{
    PSX_Point abeSpawnPos = {};
    gMap.Get_Abe_Spawn_Pos(&abeSpawnPos);

    const FP gravity = GetSpriteScale() == FP_FromInteger(1) ? FP_FromDouble(1.8) : FP_FromDouble(0.9);
    const FP xPosDistance = FP_FromInteger(xPosDest - xPosSource);
    FP yPosRealDistance = {};
    if (yPosDest > 0)
    {
        const s32 yPosSourceFull = abeSpawnPos.y + yPosSource;
        if (yPosDest > yPosSourceFull)
        {
            const FP yPosDistance = FP_FromInteger(yPosDest - yPosSourceFull);
            FP yPosDistanceOffset = {};
            if (yPosDistance <= (FP_FromInteger(41) * GetSpriteScale()))
            {
                yPosDistanceOffset = FP_FromInteger(0);
            }
            else
            {
                yPosDistanceOffset = yPosDistance - (FP_FromInteger(41) * GetSpriteScale());
            }

            const FP spriteScaleFactor = FP_FromInteger(20) * GetSpriteScale();
            FP yPosDistanceCalc = (yPosDistanceOffset / spriteScaleFactor) + FP_FromDouble(20.01);
            if (xPosDest > 0)
            {
                mVelX = (xPosDistance - FP_FromInteger(abeSpawnPos.x)) / yPosDistanceCalc;
            }
            else
            {
                mVelX = FP_FromDouble(2.796) * GetSpriteScale();
            }
            mVelY = FP_FromDouble(-16.1) * GetSpriteScale();
            return;
        }

        const s32 yPosFullDistanceInverse = yPosSourceFull - yPosDest;
        if (yPosFullDistanceInverse >= 0)
        {
            yPosRealDistance = FP_FromInteger(yPosFullDistanceInverse);
        }
        else
        {
            yPosRealDistance = FP_FromInteger(yPosDest - yPosSourceFull);
        }
    }
    else
    {
        yPosRealDistance = FP_FromInteger(0);
    }

    const FP yPosRealDistanceFull = (FP_FromInteger(80) * GetSpriteScale()) + yPosRealDistance;
    const FP gravityFactor = FP_FromInteger(8) * gravity;
    const FP gravityCalc = Math_SquareRoot_FP_Wrapper(((gravityFactor * yPosRealDistanceFull) + (gravity * gravity)));
    mVelY = (gravityCalc - gravity) * FP_FromDouble(0.5);

    const FP yVelocityAfterGravity = mVelY / gravity;
    mVelY = -mVelY;

    if (xPosDest > 0)
    {
        mVelX = (xPosDistance - FP_FromInteger(abeSpawnPos.x)) / (yVelocityAfterGravity + FP_FromDouble(8.9));
    }
    else
    {
        mVelX = FP_FromDouble(2.796) * GetSpriteScale();
    }
}

void Abe::FollowLift_45A500()
{
    LiftPoint* pLift = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLift)
    {
        mVelY = pLift->mVelY;
        if (pLift->GetDead())
        {
            VOnTrapDoorOpen();
        }
        SetActiveCameraDelayedFromDir();
    }
}

eAbeMotions Abe::MoveLiftUpOrDown_45A7E0(FP yVelocity)
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (!pLiftPoint)
    {
        return eAbeMotions::Motion_123_LiftGrabIdle;
    }

    pLiftPoint->Move(FP_FromInteger(0), yVelocity);
    FollowLift_45A500();

    if (sControlledCharacter == this && !(GetAnimation().GetIsLastFrame()) && GetAnimation().GetCurrentFrame() != 5)
    {
        return mCurrentMotion;
    }

    if (yVelocity >= FP_FromInteger(0))
    {
        if (yVelocity > FP_FromInteger(0))
        {
            if (pLiftPoint->OnBottomFloor())
            {
                return eAbeMotions::Motion_123_LiftGrabIdle;
            }

            const u32 held = Input().GetHeld();
            if (InputCommands::eDown & held)
            {
                return eAbeMotions::Motion_125_LiftUseDown;
            }

            if (held & InputCommands::eUp)
            {
                return eAbeMotions::Motion_124_LiftUseUp;
            }
        }
    }
    else
    {
        if (pLiftPoint->OnTopFloor())
        {
            return eAbeMotions::Motion_123_LiftGrabIdle;
        }

        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            return eAbeMotions::Motion_124_LiftUseUp;
        }

        if (Input().IsAnyHeld(InputCommands::eDown))
        {
            return eAbeMotions::Motion_125_LiftUseDown;
        }
    }

    if (Input().GetHeld() && pLiftPoint->OnAnyFloor() && !(pLiftPoint->mMoving & 1))
    {
        return eAbeMotions::Motion_122_LiftGrabEnd;
    }

    pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0));
    return eAbeMotions::Motion_123_LiftGrabIdle;
}

s16 Abe::GetEvilFart_4585F0(s16 bDontLoad)
{
    const PSX_RECT abeRect = VGetBoundingRect();

    BrewMachine* pBrewMachine = nullptr;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            return 0;
        }

        if (pObj->Type() == ReliveTypes::eBrewMachine)
        {
            pBrewMachine = static_cast<BrewMachine*>(pObj);

            const PSX_RECT bRect = pBrewMachine->VGetBoundingRect();

            if (RectsOverlap(abeRect, bRect) && pBrewMachine->GetSpriteScale() == GetSpriteScale() && pBrewMachine->GetRemainingBrewCount() > 0 && !mHasEvilFart)
            {
                break;
            }

            pBrewMachine = nullptr;
        }
    }

    if (!pBrewMachine)
    {
        return 0;
    }

    if (!bDontLoad)
    {
        return 1;
    }

    pBrewMachine->DecreaseRemainingBrewCount();
    mHasEvilFart = true;

    return 1;
}

void Abe::ChangeChantState(bool bLaughAtChantEnd)
{
    if (bLaughAtChantEnd)
    {
        mLaughAtChantEnd = true;
        field_120_state.chant = ChantStates::eChantingForBirdPortal_6; // Holds chant, then laughs.
    }
    else if (sControlledCharacter == this)
    {
        field_120_state.chant = ChantStates::eIdleChanting_0; // Chants briefly, then stops.
    }
    else
    {
        field_120_state.chant = ChantStates::eWaitForUnpossessing_3; // Chants briefly with a possession flicker, then stops.
    }
}

void Abe::SetAsDead()
{
    field_120_state.raw = 1;
}

void Abe::ExitShrykull(bool bResetRingTimer)
{
    GetAnimation().SetAnimate(true);
    GetAnimation().SetRender(true);

    mbMotionChanged = true;

    mCurrentMotion = eAbeMotions::Motion_120_EndShrykull;
    field_124_timer = 1;

    if (bResetRingTimer)
    {
        mRingPulseTimer = 0;
    }
}

static void playAbeSFX(MudSounds idx, s16 volume, s32 pitch)
{
    SFX_SfxDefinition_Play_Mono(
        sAbeSFXList_555250[static_cast<u8>(idx)],
        volume,
        static_cast<s16>(pitch), static_cast<s16>(pitch));
}

void Mudokon_SFX(MudSounds idx, s16 volume, s32 pitch, BaseAliveGameObject* pHero)
{
    switch (idx)
    {
        case MudSounds::eOops_14:
        {
            if (pHero && pHero->GetSpriteScale() == FP_FromDouble(0.5))
            {
                SND_SEQ_Play(SeqId::AbeOops_16, 1, 90, 90);
            }
            else
            {
                SND_SEQ_Play(SeqId::AbeOops_16, 1, 127, 127);
            }
            break;
        }
        case MudSounds::eStopIt_26:
        {
            if (pHero && pHero->GetSpriteScale() == FP_FromDouble(0.5))
            {
                SND_SEQ_Play(SeqId::AbeStopIt_18, 1, 80, 80);
            }
            else
            {
                SND_SEQ_Play(SeqId::AbeStopIt_18, 1, 115, 115);
            }
            break;
        }
        case MudSounds::eGiggle_8:
        {
            if (IsAbe(pHero) && gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                idx = MudSounds::eLaugh_10;
            }

            [[fallthrough]];
        }
        default:
        {
            auto idxToVal = static_cast<u8>(idx);
            if (!volume)
            {
                volume = sAbeSFXList_555250[idxToVal].mDefaultVolume;
            }

            // OG bug - isn't null checked in other cases before de-ref?
            if (!pHero)
            {
                playAbeSFX(idx, volume, pitch);
                return;
            }

            if (pHero->GetSpriteScale() == FP_FromDouble(0.5))
            {
                volume = 2 * volume / 3;
            }

            if (IsAbe(pHero))
            {
                playAbeSFX(idx, volume, pitch);
                return;
            }

            switch (gMap.GetDirection(
                pHero->mCurrentLevel,
                pHero->mCurrentPath,
                pHero->mXPos,
                pHero->mYPos))
            {
                case CameraPos::eCamCurrent_0:
                {
                    playAbeSFX(idx, volume, pitch);
                    break;
                }
                case CameraPos::eCamTop_1:
                case CameraPos::eCamBottom_2:
                {
                    playAbeSFX(idx, 2 * volume / 3, pitch);
                    break;
                }
                case CameraPos::eCamLeft_3:
                {
                    SFX_SfxDefinition_Play_Stereo(
                        sAbeSFXList_555250[idxToVal],
                        2 * volume / 3,
                        2 * volume / 9,
                        static_cast<s16>(pitch), static_cast<s16>(pitch));
                    break;
                }
                case CameraPos::eCamRight_4:
                {
                    SFX_SfxDefinition_Play_Stereo(
                        sAbeSFXList_555250[idxToVal],
                        2 * volume / 9,
                        2 * volume / 3,
                        static_cast<s16>(pitch), static_cast<s16>(pitch));
                    break;
                }
                default:
                    break;
            }
        }
    }
}
