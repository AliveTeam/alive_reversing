#include "stdafx.h"
#include "UXB.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Abe.hpp"
#include "../relive_lib/GameObjects/GroundExplosion.hpp"
#include "../relive_lib/Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "QuikSave.hpp"
#include "../relive_lib/GameType.hpp"

#include <math.h>

static const TintEntry sUXBTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

void UXB::LoadAnimations()
{
    const static AnimId kUxbAnims[] = 
    {
        AnimId::Bomb_RedGreenTick,
        AnimId::UXB_Disabled,
        AnimId::UXB_Toggle,
        AnimId::UXB_Active,
        AnimId::Bomb_Flash,
        AnimId::Mudokon_SlapBomb,
        AnimId::Explosion_Rock,
        AnimId::GroundExplosion,
        AnimId::Abe_Head_Gib,
        AnimId::Abe_Arm_Gib,
        AnimId::Abe_Body_Gib,
        AnimId::Slog_Head_Gib,
        AnimId::Slog_Body_Gib
    };

    for (const auto& animId : kUxbAnims)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

void UXB::PlaySFX(relive::SoundEffects sfxIdx)
{
    if (gMap.Is_Point_In_Current_Camera(
            this->mCurrentLevel,
            this->mCurrentPath,
            this->mXPos,
            this->mYPos,
            0))
    {
        SfxPlayMono(sfxIdx, 35);
    }
}

UXB::UXB(relive::Path_UXB* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eUXB);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::UXB_Active));
    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::GreenFlash));

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);

    if (GetGameType() == GameType::eAe)
    {
        SetTint(sUXBTints, gMap.mCurrentLevel);
    }

    SetInteractive(true);
    mCurrentState = UXBState::eDelay;

    mPatternLength = pTlv->mPatternLength;
    if (pTlv->mPatternLength < 1 || pTlv->mPatternLength > 4)
    {
        mPatternLength = 1;
    }

    mPattern = pTlv->mPattern;
    if (!pTlv->mPattern) // If no pattern set, go to a default one.
    {
        mPattern = 11111;
    }

    mPatternIndex = 0;

    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
    mRedBlinkCount = (mPattern / static_cast<s32>(pow(10, mPatternLength - 1))) % 10;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
        SetScale(Scale::Fg);
    }

    InitBlinkAnim(&mFlashAnim);

    if (pTlv->mTlvSpecificMeaning) // Stores the activated/deactivated state for UXB
    {
        if (pTlv->mStartState == relive::Path_UXB::StartState::eOn)
        {
            mFlashAnim.LoadPal(GetPalRes(PalId::GreenFlash));

            mIsRed = 0;

            mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));
            PlaySFX(relive::SoundEffects::GreenTick);

            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::UXB_Disabled));

            mCurrentState = UXBState::eDeactivated;
            mStartingState = UXBState::eDelay;
        }
        else
        {
            mStartingState = UXBState::eDeactivated;
        }
    }
    else
    {
        if (pTlv->mStartState == relive::Path_UXB::StartState::eOn)
        {
            mStartingState = UXBState::eDelay;
        }
        else
        {
            mIsRed = 0;

            mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));

            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::UXB_Disabled));

            mStartingState = UXBState::eDeactivated;
            mCurrentState = UXBState::eDeactivated;
        }
    }

    if (GetGameType() == GameType::eAo)
    {
        mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
        mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);
    }
    else
    {
        mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
        mYPos = FP_FromInteger(pTlv->mTopLeftY);
    }

    // Raycasts on ctor to place perfectly on the floor.
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            FP_FromInteger(pTlv->mTopLeftY),
            mXPos,
            FP_FromInteger(pTlv->mTopLeftY + 24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    mTlvInfo = tlvId;
    mNextStateTimer = sGnFrame;

    const FP gridSnap = ScaleToGridSize(GetSpriteScale());
    SetInteractive(true);

    if (GetGameType() == GameType::eAe)
    {
        SetDoPurpleLightEffect(true);
    }

    mCollectionRect.x = mXPos - (gridSnap / FP_FromInteger(2));
    mCollectionRect.y = mYPos - gridSnap;
    mCollectionRect.w = mXPos + (gridSnap / FP_FromInteger(2));
    mCollectionRect.h = mYPos;
}

void UXB::InitBlinkAnim(Animation* pAnimation)
{
    if (pAnimation->Init(GetAnimRes(AnimId::Bomb_RedGreenTick), this))
    {
        pAnimation->SetSemiTrans(true);
        pAnimation->SetBlending(true);

        pAnimation->SetRenderLayer(GetAnimation().GetRenderLayer());
        pAnimation->SetSpriteScale(GetSpriteScale());
        pAnimation->SetRGB(128, 128, 128);
        pAnimation->SetBlendMode(relive::TBlendModes::eBlend_1);
    }
    else
    {
        SetListAddFailed(true);
    }
}

void UXB::VOnAbeInteraction()
{
    if (mCurrentState != UXBState::eExploding)
    {
        if (mCurrentState != UXBState::eDeactivated || mNextStateTimer > sGnFrame)
        {
            if (mRedBlinkCount)
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = MakeTimer(2);
            }
            else
            {
                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));
                PlaySFX(relive::SoundEffects::GreenTick);

                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::UXB_Toggle));
                mCurrentState = UXBState::eDeactivated;

                mNextStateTimer = MakeTimer(10);
            }
        }
        else
        {
            mCurrentState = UXBState::eDelay;
            SetUpdateDelay(6);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::UXB_Active));
            PlaySFX(relive::SoundEffects::RedTick);
        }
    }
}

UXB::~UXB()
{
    if (mCurrentState != UXBState::eExploding || sGnFrame < mNextStateTimer)
    {
        Path::TLV_Reset(mTlvInfo);
    }
    else
    {
        Path::TLV_Delete(mTlvInfo);
    }

    mFlashAnim.VCleanUp();

    SetInteractive(false);
}

void UXB::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    if (sControlledCharacter == nullptr || FP_Abs(sControlledCharacter->mYPos - mYPos) > FP_FromInteger(520) || FP_Abs(sControlledCharacter->mXPos - mXPos) > FP_FromInteger(750))
    {
        if (mStartingState == UXBState::eDeactivated && mCurrentState != UXBState::eDeactivated)
        {
            Path::TLV_Persist(mTlvInfo, 1);
        }
        else if (mStartingState != UXBState::eDelay || mCurrentState != UXBState::eDeactivated)
        {
            Path::TLV_Persist(mTlvInfo, 0);
        }
        else
        {
            Path::TLV_Persist(mTlvInfo, 1);
        }
        SetDead(true);
    }
}

bool UXB::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead())
    {
        return false;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eAbe:
        case ReliveTypes::eMudokon:
            if (mCurrentState == UXBState::eDeactivated)
            {
                return false;
            }
            break;

        case ReliveTypes::eMineCar:
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eAirExplosion:
        case ReliveTypes::eShrykull:
            break;

        default:
            return false;
    }

    SetDead(true);

    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());

    mCurrentState = UXBState::eExploding;
    mNextStateTimer = sGnFrame;

    return true;
}

void UXB::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
    mCurrentState = UXBState::eExploding;
    SetDead(true);
    mNextStateTimer = sGnFrame;
}

void UXB::VUpdate()
{
    switch (mCurrentState)
    {
        case UXBState::eDelay:
            if (IsColliding())
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = MakeTimer(2);
            }
            else if (mNextStateTimer <= sGnFrame)
            {
                mCurrentState = UXBState::eActive;
                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_Flash));
                mNextStateTimer = MakeTimer(2);
            }
            break;

        case UXBState::eDeactivated:
            // Do nothing
            break;

        case UXBState::eActive:
            if (IsColliding())
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = MakeTimer(2);
            }
            else if (mNextStateTimer <= sGnFrame)
            {
                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));

                if (mRedBlinkCount)
                {
                    mRedBlinkCount--;
                    if (mRedBlinkCount == 0)
                    {
                        mFlashAnim.LoadPal(GetPalRes(PalId::GreenFlash));
                        mIsRed = 0;
                    }
                }
                else
                {
                    mFlashAnim.ReloadPal();

                    mIsRed = 1;

                    mPatternIndex++;

                    if (mPatternIndex >= mPatternLength)
                    {
                        mPatternIndex = 0;
                    }

                    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
                    mRedBlinkCount = (mPattern / static_cast<s32>(pow(10, mPatternLength - mPatternIndex - 1))) % 10;
                }

              
                if (mIsRed)
                {
                    PlaySFX(relive::SoundEffects::RedTick);
                }
                else
                {
                    PlaySFX(relive::SoundEffects::GreenTick);
                }

                mCurrentState = UXBState::eDelay;
                mNextStateTimer = MakeTimer(10); // UXB change color delay
            }
            break;

        case UXBState::eExploding:
            if (sGnFrame >= mNextStateTimer)
            {
                relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
                SetDead(true);
            }
            break;
    }

    if (mCurrentState != UXBState::eExploding)
    {
        if (EventGet(Event::kEventDeathReset))
        {
            if (mStartingState != UXBState::eDeactivated || mCurrentState == UXBState::eDeactivated)
            {
                if (mStartingState != UXBState::eDelay || mCurrentState != UXBState::eDeactivated)
                {
                    Path::TLV_Persist(mTlvInfo, 0);
                }
                else
                {
                    Path::TLV_Persist(mTlvInfo, 1);
                }
            }
            else
            {
                Path::TLV_Persist(mTlvInfo, 1);
            }
            SetDead(true);
        }
    }
}

bool UXB::IsColliding()
{
    const PSX_RECT uxbBound = VGetBoundingRect();

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetCanSetOffExplosives() && pObj->GetAnimation().GetRender())
        {
            const PSX_RECT objBound = pObj->VGetBoundingRect();

            const s32 objX = FP_GetExponent(pObj->mXPos);
            const s32 objY = FP_GetExponent(pObj->mYPos);

            if (objX > uxbBound.x && objX < uxbBound.w && objY < uxbBound.h + 5 && uxbBound.x <= objBound.w && uxbBound.w >= objBound.x && uxbBound.h >= objBound.y && uxbBound.y <= objBound.h && pObj->GetSpriteScale() == GetSpriteScale())
            {
                return true;
            }
        }
    }

    return false;
}

void UXB::VRender(OrderingTable& ot)
{
    if (GetAnimation().GetRender())
    {
        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            FP yOffset;
            if (GetGameType() == GameType::eAo)
            {
                yOffset = FP_FromInteger(12);
            }
            else
            {
                yOffset = FP_FromInteger(17);
            }

            mFlashAnim.VRender(
                FP_GetExponent((mXPos - gScreenManager->CamXPos())),
                FP_GetExponent((mYPos - gScreenManager->CamYPos() - FP_NoFractional(GetSpriteScale() * yOffset))),
                ot,
                0,
                0);

            BaseAnimatedWithPhysicsGameObject::VRender(ot);
        }
    }
}

void UXB::VGetSaveState(SerializedObjectData& __pSaveBuffer)
{
    UXBSaveState data = {};

    data.mType = ReliveTypes::eUXB;
    data.mTlvInfo = mTlvInfo;
    data.mNextStateTimer = mNextStateTimer;
    data.mCurrentState = mCurrentState;
    data.mStartingState = mStartingState;
    data.mPatternIndex = mPatternIndex;
    data.mRedBlinkCount = mRedBlinkCount;
    data.mIsRed = mIsRed;

    __pSaveBuffer.Write(data);
}

void UXB::CreateFromSaveState(SerializedObjectData& __pSaveState)
{
    const auto pSaveState = __pSaveState.ReadTmpPtr<UXBSaveState>();

    relive::Path_UXB* uxbPath = reinterpret_cast<relive::Path_UXB*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->mTlvInfo));

    UXB* pUXB = relive_new UXB(uxbPath, pSaveState->mTlvInfo);

    if (pSaveState->mCurrentState == UXBState::eDeactivated)
    {
        pUXB->mFlashAnim.LoadPal(pUXB->GetPalRes(PalId::GreenFlash));
        pUXB->mFlashAnim.Set_Animation_Data(pUXB->GetAnimRes(AnimId::Bomb_RedGreenTick));
        pUXB->GetAnimation().Set_Animation_Data(pUXB->GetAnimRes(AnimId::UXB_Disabled));
    }

    pUXB->mNextStateTimer = pSaveState->mNextStateTimer;
    pUXB->mCurrentState = pSaveState->mCurrentState;
    pUXB->mStartingState = pSaveState->mStartingState;
    pUXB->mPatternIndex = pSaveState->mPatternIndex;
    pUXB->mRedBlinkCount = pSaveState->mRedBlinkCount;

    pUXB->mIsRed = 0;

    if (pSaveState->mIsRed)
    {
        pUXB->mIsRed = 1;
    }
}

