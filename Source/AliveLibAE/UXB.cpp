#include "stdafx.h"
#include "UXB.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Abe.hpp"
#include "GroundExplosion.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"

#include <math.h>

const TintEntry sTintMap_UXB_563A3C[19] = {
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
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

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
        AnimId::Explosion_Rocks,
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

void UXB::InitBlinkAnim(Animation* pAnimation)
{
    if (pAnimation->Init(GetAnimRes(AnimId::Bomb_RedGreenTick), this))
    {
        pAnimation->SetRenderLayer(GetAnimation().GetRenderLayer());
        pAnimation->mFlags.Set(AnimFlags::eSemiTrans);
        pAnimation->mFlags.Set(AnimFlags::eBlending);
        pAnimation->SetSpriteScale(GetSpriteScale());
        pAnimation->SetRGB(128, 128, 128);
        pAnimation->SetRenderMode(TPageAbr::eBlend_1);
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
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

s32 UXB::IsColliding()
{
    const PSX_RECT uxbBound = VGetBoundingRect();

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseAliveGameObjectFlags.Get(eCanSetOffExplosives) && pObj->GetAnimation().mFlags.Get(AnimFlags::eRender))
        {
            const PSX_RECT objBound = pObj->VGetBoundingRect();

            const s32 objX = FP_GetExponent(pObj->mXPos);
            const s32 objY = FP_GetExponent(pObj->mYPos);

            if (objX > uxbBound.x && objX < uxbBound.w && objY < uxbBound.h + 5 && uxbBound.x <= objBound.w && uxbBound.w >= objBound.x && uxbBound.h >= objBound.y && uxbBound.y <= objBound.h && pObj->GetSpriteScale() == GetSpriteScale())
            {
                return 1;
            }
        }
    }

    return 0;
}



UXB::UXB(relive::Path_UXB* tlv_params, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eUXB);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::UXB_Active));
    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::GreenFlash));

    GetAnimation().mFlags.Set(AnimFlags::eSemiTrans);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_0);

    SetTint(sTintMap_UXB_563A3C, gMap.mCurrentLevel);

    mBaseGameObjectFlags.Set(BaseGameObject::Options::eInteractive_Bit8);
    mCurrentState = UXBState::eDelay;

    mPatternLength = tlv_params->mPatternLength;
    if (tlv_params->mPatternLength < 1 || tlv_params->mPatternLength > 4)
    {
        mPatternLength = 1;
    }


    mPattern = tlv_params->mPattern;
    if (!tlv_params->mPattern) // If no pattern set, go to a default one.
    {
        mPattern = 11111;
    }

    mPatternIndex = 0;
    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
    mRedBlinkCount = (mPattern / static_cast<s32>(pow(10, mPatternLength - 1))) % 10;

    if (tlv_params->mScale != relive::reliveScale::eFull)
    {
        if (tlv_params->mScale == relive::reliveScale::eHalf)
        {
            SetSpriteScale(FP_FromDouble(0.5));
            GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
            SetScale(Scale::Bg);
        }
    }
    else
    {
        SetSpriteScale(FP_FromDouble(1.0));
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
        SetScale(Scale::Fg);
    }

    InitBlinkAnim(&mFlashAnim);
    if (tlv_params->mTlvSpecificMeaning) // Stores the activated/deactivated state for UXB.
    {
        if (tlv_params->mStartState == relive::Path_UXB::StartState::eOn)
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
        if (tlv_params->mStartState == relive::Path_UXB::StartState::eOn)
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

    FP hitX = {};
    FP hitY = {};

    mXPos = FP_FromInteger((tlv_params->mTopLeftX + tlv_params->mBottomRightX) / 2);
    mYPos = FP_FromInteger(tlv_params->mTopLeftY);

    // Raycasts on ctor to place perfectly on the floor.
    if (sCollisions->Raycast(
            mXPos,
            FP_FromInteger(tlv_params->mTopLeftY),
            mXPos,
            FP_FromInteger(tlv_params->mTopLeftY + 24),
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
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    mCollectionRect.x = mXPos - (gridSnap / FP_FromDouble(2.0));
    mCollectionRect.y = mYPos - gridSnap;
    mCollectionRect.w = (gridSnap / FP_FromDouble(2.0)) + mXPos;
    mCollectionRect.h = mYPos;
}


void UXB::VOnPickUpOrSlapped()
{
    if (mCurrentState != UXBState::eExploding)
    {
        if (mCurrentState != UXBState::eDeactivated || mNextStateTimer > sGnFrame)
        {
            if (mRedBlinkCount)
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = sGnFrame + 2;
            }
            else
            {
                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));
                PlaySFX(relive::SoundEffects::GreenTick);

                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::UXB_Toggle));
                mCurrentState = UXBState::eDeactivated;

                mNextStateTimer = sGnFrame + 10;
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

void UXB::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
    mCurrentState = UXBState::eExploding;
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    mNextStateTimer = sGnFrame;
}

s16 UXB::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eAbe:
        case ReliveTypes::eMudokon:
            if (mCurrentState == UXBState::eDeactivated)
            {
                return 0;
            }
            break;

        case ReliveTypes::eMineCar:
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eAirExplosion:
        case ReliveTypes::eShrykull:
            break;

        default:
            return 0;
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
    mCurrentState = UXBState::eExploding;
    mNextStateTimer = sGnFrame;

    return 1;
}

UXB::~UXB()
{
    if (mCurrentState != UXBState::eExploding || sGnFrame < mNextStateTimer)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }

    mFlashAnim.VCleanUp();

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);
}

void UXB::VUpdate()
{
    switch (mCurrentState)
    {
        case UXBState::eDelay:
            if (IsColliding())
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = sGnFrame + 2;
            }
            else if (mNextStateTimer <= sGnFrame)
            {
                mCurrentState = UXBState::eActive;
                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_Flash));
                mNextStateTimer = sGnFrame + 2;
            }
            break;

        case UXBState::eDeactivated:
            // Do nothing
            break;

        case UXBState::eActive:
            if (IsColliding())
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = sGnFrame + 2;
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
                    // TODO: Restore original pal
                    //const PerFrameInfo* pFrameInfo = mFlashAnim.Get_FrameHeader(-1);
                    //mFlashAnim.LoadPal(mFlashAnim.field_20_ppBlock, pFrameHeader->field_0_clut_offset);
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
                mNextStateTimer = sGnFrame + 10; // UXB change color delay.
            }
            break;

        case UXBState::eExploding:
            if (sGnFrame >= mNextStateTimer)
            {
                relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            break;
    }

    if (mCurrentState != UXBState::eExploding)
    {
        if (EventGet(kEventDeathReset))
        {
            if (mStartingState != UXBState::eDeactivated || mCurrentState == UXBState::eDeactivated)
            {
                if (mStartingState != UXBState::eDelay || mCurrentState != UXBState::eDeactivated)
                {
                    Path::TLV_Reset(mTlvInfo, 0, 1, 0);
                }
                else
                {
                    Path::TLV_Reset(mTlvInfo, 1, 1, 0);
                }
            }
            else
            {
                Path::TLV_Reset(mTlvInfo, 1, 1, 0);
            }
            mBaseGameObjectFlags.Set(Options::eDead);
        }
    }
}

void UXB::VRender(PrimHeader** ppOt)
{
    if (GetAnimation().mFlags.Get(AnimFlags::eRender))
    {
        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            mFlashAnim.VRender(
                FP_GetExponent((mXPos - pScreenManager->CamXPos())),
                FP_GetExponent((mYPos - pScreenManager->CamYPos() - FP_NoFractional(GetSpriteScale() * FP_FromInteger(17)))),
                ppOt,
                0,
                0);

            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
    }
}

void UXB::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    if (sControlledCharacter == nullptr || FP_Abs(sControlledCharacter->mYPos - mYPos) > FP_FromInteger(520) || FP_Abs(sControlledCharacter->mXPos - mXPos) > FP_FromInteger(750))
    {
        if (mStartingState != UXBState::eDeactivated || mCurrentState == UXBState::eDeactivated)
        {
            if (mStartingState != UXBState::eDelay || mCurrentState != UXBState::eDeactivated)
            {
                Path::TLV_Reset(mTlvInfo, 0, 1, 0);
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            else
            {
                Path::TLV_Reset(mTlvInfo, 1, 1, 0);
                mBaseGameObjectFlags.Set(Options::eDead);
            }
        }
        else
        {
            Path::TLV_Reset(mTlvInfo, 1, 1, 0);
            mBaseGameObjectFlags.Set(Options::eDead);
        }
    }
}

s32 UXB::VGetSaveState(u8* __pSaveBuffer)
{
    SaveState_UXB* pSaveState = reinterpret_cast<SaveState_UXB*>(__pSaveBuffer);

    pSaveState->mType = AETypes::eUXB_143;
    pSaveState->mTlvInfo = mTlvInfo;
    pSaveState->mNextStateTimer = mNextStateTimer;
    pSaveState->mCurrentState = mCurrentState;
    pSaveState->mStartingState = mStartingState;
    pSaveState->mPatternIndex = mPatternIndex;
    pSaveState->mRedBlinkCount = mRedBlinkCount;
    pSaveState->mIsRed = mIsRed;

    return sizeof(SaveState_UXB);
}

s32 UXB::CreateFromSaveState(const u8* __pSaveState)
{
    const SaveState_UXB* pSaveState = reinterpret_cast<const SaveState_UXB*>(__pSaveState);

    relive::Path_UXB* uxbPath = reinterpret_cast<relive::Path_UXB*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->mTlvInfo));

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

    return sizeof(SaveState_UXB);
}

