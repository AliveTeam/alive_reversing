#include "stdafx.h"
#include "Teleporter.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/GameObjects/Spark.hpp"
#include "ParticleBurst.hpp"
#include "Electrocute.hpp"
#include "Path.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/FixedPoint.hpp"

void SetData(Relive_Path_Teleporter_Data& tlvData, const relive::Path_Teleporter& tlv)
{
    tlvData.mTeleporterId = tlv.mTeleporterId;
    tlvData.mOtherTeleporterId = tlv.mOtherTeleporterId;
    tlvData.mDestCamera = tlv.mDestCamera;
    tlvData.mDestPath = tlv.mDestPath;
    tlvData.mDestLevel = tlv.mDestLevel;
    tlvData.mSwitchId = tlv.mSwitchId;
    tlvData.mScale = tlv.mScale;
    tlvData.mWipeEffect = tlv.mWipeEffect;
    tlvData.mMovieId = tlv.mMovieId;
    tlvData.mElectricX = tlv.mElectricX;
    tlvData.mElectricY = tlv.mElectricY;
}

Teleporter::Teleporter(relive::Path_Teleporter* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    SetData(mTlvData, *pTlv);
    mTlvId = tlvId;

    mSwitchState = SwitchStates_Get(mTlvData.mSwitchId);

    mEffectsCreated = false;

    mDestroySelf = false;
    mState = TeleporterState::eWaitForSwitchOn_0;
    mElectrocuteId = Guid{};
}

Teleporter::~Teleporter()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void Teleporter::VScreenChanged()
{
    if (gMap.LevelChanged())
    {
        SetDead(true);
    }
    mDestroySelf = true;
}

Electrocute* Teleporter::Create_ElectrocuteEffect()
{
    return relive_new Electrocute(sControlledCharacter, true, false);
}

static const PSX_Point kSparkOffs[8] = {
    {-15, 46},
    {-20, 29},
    {-15, 12},
    {0, 2},
    {16, 12},
    {19, 28},
    {15, 45},
    {0, 0}};

void Teleporter::SpawnRingSparks(Relive_Path_Teleporter_Data* pTlvData)
{
    PSX_Point abeSpawnPos = {};
    gMap.Get_Abe_Spawn_Pos(&abeSpawnPos);

    const s16 xOrg = pTlvData->mElectricX - abeSpawnPos.x;
    const s16 yOrg = pTlvData->mElectricY - abeSpawnPos.y;

    for (auto& sparkOffs : kSparkOffs)
    {
        s32 sparkX = 0;
        s32 sparkY = 0;
        if (pTlvData->mScale != relive::reliveScale::eFull)
        {
            sparkX = xOrg + (sparkOffs.x / 2);
            sparkY = yOrg + (sparkOffs.y / 2);
        }
        else
        {
            sparkX = xOrg + (sparkOffs.x);
            sparkY = yOrg + (sparkOffs.y);
        }

        relive_new Spark(FP_FromInteger(sparkX), FP_FromInteger(sparkY), FP_FromInteger(1), 9, -31, 159, SparkType::eBigChantParticle_1);
    }
}

void Teleporter::VUpdate()
{
    Electrocute* pObj = static_cast<Electrocute*>(sObjectIds.Find(mElectrocuteId, ReliveTypes::eElectrocute));

    switch (mState)
    {
        case TeleporterState::eWaitForSwitchOn_0:
        {
            if (mDestroySelf)
            {
                SetDead(true);
            }

            if (SwitchStates_Get(mTlvData.mSwitchId) == mSwitchState)
            {
                return;
            }

            mSwitchState = SwitchStates_Get(mTlvData.mSwitchId);

            if (!gPathInfo->TLV_Get_At(
                    FP_GetExponent(sControlledCharacter->mXPos),
                    FP_GetExponent(sControlledCharacter->mYPos),
                    FP_GetExponent(sControlledCharacter->mXPos),
                    FP_GetExponent(sControlledCharacter->mYPos),
                    ReliveTypes::eTeleporter))
            {
                return;
            }

            if (sControlledCharacter->GetTeleporting())
            {
                return;
            }

            mState = TeleporterState::eIntoTeleporter_1;
            mElectrocuteId = Teleporter::Create_ElectrocuteEffect()->mBaseGameObjectId;

            SFX_Play_Pitch(relive::SoundEffects::Zap1, 60, -400);
            sControlledCharacter->SetTeleporting(true);

            SpawnRingSparks(&mTlvData);
        }
        break;

        case TeleporterState::eIntoTeleporter_1:
        {
            if (pObj)
            {
                if (pObj->VSub_4E6630() || mEffectsCreated)
                {
                    if (!pObj->GetDead())
                    {
                        return;
                    }
                }

                // Only create the effects once (disable this if you like a crazy amount of sparks and things)
                if (!mEffectsCreated)
                {
                    // Spawn the falling "red" sparks from Abe's feet that appear after you enter the teleporter
                    if (mTlvData.mScale != relive::reliveScale::eFull)
                    {
                        // Steam/smoke effect at Abe's body
                        New_Smoke_Particles(
                            sControlledCharacter->mXPos,
                            sControlledCharacter->mYPos - FP_FromInteger(9), // 18/2
                            sControlledCharacter->GetSpriteScale(),
                            3,
                            RGB16{ 128, 128, 128 });

                        relive_new ParticleBurst(sControlledCharacter->mXPos,
                                              sControlledCharacter->mYPos - FP_FromInteger(9), // 18/2
                                              9u,
                                              FP_FromDouble(0.5),
                                              BurstType::eBigRedSparks_3,
                                              9);
                    }
                    else
                    {
                        // Steam/smoke effect at Abe's body
                        New_Smoke_Particles(
                            sControlledCharacter->mXPos,
                            sControlledCharacter->mYPos - FP_FromInteger(18),
                            sControlledCharacter->GetSpriteScale(),
                            3,
                            RGB16{128, 128, 128});

                       relive_new ParticleBurst(sControlledCharacter->mXPos,
                                                                    sControlledCharacter->mYPos - FP_FromInteger(18),
                                                                    9u,
                                                                    FP_FromInteger(1),
                                                                    BurstType::eBigRedSparks_3,
                                                                    9);
                    }
                    mEffectsCreated = true;
                }

                if (!(pObj->GetDead()))
                {
                    return;
                }
            }

            sControlledCharacter->GetAnimation().SetRender(false);

            gMap.mTeleporterTransition = 1;

            const CameraSwapEffects effect = kPathChangeEffectToInternalScreenChangeEffect[mTlvData.mWipeEffect];
            s16 bForceChange = 0;
            if (effect == CameraSwapEffects::ePlay1FMV_5 || effect == CameraSwapEffects::eUnknown_11)
            {
                bForceChange = 1;
            }

            gMap.SetActiveCam(
                mTlvData.mDestLevel,
                mTlvData.mDestPath,
                mTlvData.mDestCamera,
                effect,
                mTlvData.mMovieId,
                bForceChange);

            sControlledCharacter->SetUpdateDelay(3);
            sActiveHero->field_1A0_door_id = mTlvData.mOtherTeleporterId;
            mState = TeleporterState::eTeleporting_2;
        }
        break;

        case TeleporterState::eTeleporting_2:
        {
            gMap.mTeleporterTransition = 0;

            relive::Path_Teleporter* pTeleporterTlv = static_cast<relive::Path_Teleporter*>(gPathInfo->TLV_First_Of_Type_In_Camera(ReliveTypes::eTeleporter, 0));
            Relive_Path_Teleporter_Data tlvData = {};
            SetData(tlvData, *pTeleporterTlv);
            if (tlvData.mTeleporterId != mTlvData.mOtherTeleporterId)
            {
                while (pTeleporterTlv)
                {
                    pTeleporterTlv = static_cast<relive::Path_Teleporter*>(gPathInfo->TLV_Next_Of_Type(pTeleporterTlv, ReliveTypes::eTeleporter));
                    SetData(tlvData, *pTeleporterTlv);

                    if (tlvData.mTeleporterId == mTlvData.mOtherTeleporterId)
                    {
                        break;
                    }
                }
            }

            SFX_Play_Pitch(relive::SoundEffects::Zap1, 60, -300, tlvData.mScale != relive::reliveScale::eFull ? FP_FromDouble(0.5) : FP_FromInteger(1));
            SpawnRingSparks(&tlvData);

            if (tlvData.mScale != relive::reliveScale::eFull)
            {
                if (sControlledCharacter->GetScale() == Scale::Fg)
                {
                    sControlledCharacter->mVelX *= FP_FromDouble(0.5);
                    sControlledCharacter->mVelY *= FP_FromDouble(0.5);
                }
                sControlledCharacter->SetSpriteScale(FP_FromDouble(0.5));
                sControlledCharacter->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
                sControlledCharacter->SetScale(Scale::Bg);
            }
            else
            {
                if (sControlledCharacter->GetScale() == Scale::Bg)
                {
                    sControlledCharacter->mVelX *= FP_FromInteger(2);
                    sControlledCharacter->mVelY *= FP_FromInteger(2);
                }
                sControlledCharacter->SetSpriteScale(FP_FromInteger(1));
                sControlledCharacter->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
                sControlledCharacter->SetScale(Scale::Fg);
            }

            // XPos = TLV xpos + TLV middle point
            sControlledCharacter->mXPos = FP_FromInteger(pTeleporterTlv->mTopLeftX) + FP_FromInteger((pTeleporterTlv->Width()) / 2);

            // HACK: Fix me when base class is merged
            static_cast<BaseAliveGameObject*>(sControlledCharacter)->MapFollowMe(true);

            PathLine* pPathLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (gCollisions->Raycast(
                    sControlledCharacter->mXPos,
                    FP_FromInteger(pTeleporterTlv->mTopLeftY),
                    sControlledCharacter->mXPos,
                    FP_FromInteger(pTeleporterTlv->mBottomRightY),
                    &pPathLine,
                    &hitX,
                    &hitY,
                    sControlledCharacter->GetScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
            {
                sControlledCharacter->BaseAliveGameObjectCollisionLine = pPathLine;
                sControlledCharacter->mYPos = hitY;
            }
            else
            {
                sControlledCharacter->BaseAliveGameObjectCollisionLine = nullptr;
                sControlledCharacter->mYPos = FP_FromInteger(pTeleporterTlv->mTopLeftY);
                sControlledCharacter->BaseAliveGameObjectLastLineYPos = sControlledCharacter->mYPos;
            }
            mState = TeleporterState::eOutOfTeleporter_4;
        }
        break;

        case TeleporterState::eOutOfTeleporter_4:
        {
            // Visual effects.
            const PSX_RECT bRect = sControlledCharacter->VGetBoundingRect();

            // White flash in the middle of Abe's body.
            New_DestroyOrCreateObject_Particle(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2) + (sControlledCharacter->GetSpriteScale() * FP_FromInteger(60)),
                sControlledCharacter->GetSpriteScale());

            // Spawn the falling "red" sparks from Abe's feet that appear after you've arrived at the destination.
            if (sControlledCharacter->GetSpriteScale() == FP_FromDouble(0.5))
            {
                relive_new ParticleBurst(sControlledCharacter->mXPos,
                                                            sControlledCharacter->mYPos - FP_FromInteger(9),
                                                            6u,
                                                            FP_FromDouble(0.5),
                                                            BurstType::eBigRedSparks_3,
                                                            9);
            }
            else
            {
                relive_new ParticleBurst(sControlledCharacter->mXPos,
                                                            sControlledCharacter->mYPos - FP_FromInteger(18),
                                                            6u,
                                                            FP_FromInteger(1),
                                                            BurstType::eBigRedSparks_3,
                                                            9);
            }

            mEffectsCreated = false;
            sControlledCharacter->GetAnimation().SetRender(true);
            sControlledCharacter->SetTeleporting(false);
            mSwitchState = SwitchStates_Get(mTlvData.mSwitchId);
            mState = TeleporterState::eWaitForSwitchOn_0;
        }
        break;
    }
}
