#include "stdafx.h"
#include "Teleporter.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ObjectIds.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Spark.hpp"
#include "ParticleBurst.hpp"
#include "Electrocute.hpp"

Teleporter::Teleporter(Path_Teleporter* pTlv, u32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_4C_pTlv = pTlv; // TODO: Don't think this is used, and it can become a dangling ptr?
    field_34_mTlvData = pTlv->field_10_data;
    field_20_tlvInfo = tlvInfo;

    field_24_global_y1 = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y));
    field_28_global_y2 = FP_GetExponent((FP_FromInteger(pTlv->field_C_bottom_right.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y));
    field_26_global_x1 = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));
    field_2A_global_x2 = FP_GetExponent((FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));

    field_2C_switch_state = SwitchStates_Get(field_34_mTlvData.field_1A_switch_id);

    field_54_effect_created = 0;

    field_32_bDestroySelf = 0;
    field_30_state = TeleporterState::eWaitForSwitchOn_0;
    field_50_objId = -1;
}

Teleporter::~Teleporter()
{
    Path::TLV_Reset(field_20_tlvInfo, -1, 0, 0);
}

void Teleporter::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel)
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
    field_32_bDestroySelf = 1;
}

Electrocute* Teleporter::Create_ElectrocuteEffect()
{
    return ae_new<Electrocute>(sControlledCharacter, TRUE, FALSE);
}

const PSX_Point kSparkOffs_563988[8] = {
    {-15, 46},
    {-20, 29},
    {-15, 12},
    {0, 2},
    {16, 12},
    {19, 28},
    {15, 45},
    {0, 0}};

void Teleporter::SpawnRingSparks(Path_Teleporter_Data* pTlvData)
{
    PSX_Point abeSpawnPos = {};
    gMap.Get_Abe_Spawn_Pos(&abeSpawnPos);

    const s16 xOrg = pTlvData->field_22_eletric_x - abeSpawnPos.field_0_x;
    const s16 yOrg = pTlvData->field_24_electric_y - abeSpawnPos.field_2_y;

    for (auto& sparkOffs : kSparkOffs_563988)
    {
        s32 sparkX = 0;
        s32 sparkY = 0;
        if (pTlvData->field_1C_scale != Scale_short::eFull_0)
        {
            sparkX = xOrg + (sparkOffs.field_0_x / 2);
            sparkY = yOrg + (sparkOffs.field_2_y / 2);
        }
        else
        {
            sparkX = xOrg + (sparkOffs.field_0_x);
            sparkY = yOrg + (sparkOffs.field_2_y);
        }

        ae_new<Spark>(FP_FromInteger(sparkX), FP_FromInteger(sparkY), FP_FromInteger(1), 9, -31, 159, SparkType::eBigChantParticle_1);
    }
}

void Teleporter::VUpdate()
{
    Electrocute* pObj = static_cast<Electrocute*>(sObjectIds.Find(field_50_objId, AETypes::eElectrocute_150));

    switch (field_30_state)
    {
        case TeleporterState::eWaitForSwitchOn_0:
        {
            if (field_32_bDestroySelf)
            {
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }

            if (SwitchStates_Get(field_34_mTlvData.field_1A_switch_id) == field_2C_switch_state)
            {
                return;
            }

            field_2C_switch_state = SwitchStates_Get(field_34_mTlvData.field_1A_switch_id);

            if (!sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                    FP_GetExponent(sControlledCharacter->mXPos),
                    FP_GetExponent(sControlledCharacter->mYPos),
                    FP_GetExponent(sControlledCharacter->mXPos),
                    FP_GetExponent(sControlledCharacter->mYPos),
                    TlvTypes::Teleporter_88))
            {
                return;
            }

            if (sControlledCharacter->mAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting))
            {
                return;
            }

            field_30_state = TeleporterState::eIntoTeleporter_1;
            field_50_objId = Teleporter::Create_ElectrocuteEffect()->field_8_object_id;

            SFX_Play_Pitch(SoundEffect::Zap1_49, 60, -400);
            sControlledCharacter->mAliveGameObjectFlags.Set(Flags_114::e114_Bit10_Teleporting);

            SpawnRingSparks(&field_34_mTlvData);
        }
        break;

        case TeleporterState::eIntoTeleporter_1:
        {
            if (pObj)
            {
                if (pObj->VSub_4E6630() || field_54_effect_created)
                {
                    if (!(pObj->mGameObjectFlags.Get(BaseGameObject::eDead)))
                    {
                        return;
                    }
                }

                // Only create the effects once (disable this if you like a crazy amount of sparks and things)
                if (!field_54_effect_created)
                {
                    // Spawn the falling "red" sparks from Abe's feet that appear after you enter the teleporter
                    if (field_34_mTlvData.field_1C_scale != Scale_short::eFull_0)
                    {
                        // Steam/smoke effect at Abe's body
                        New_Smoke_Particles(
                            sControlledCharacter->mXPos,
                            sControlledCharacter->mYPos - FP_FromInteger(9), // 18/2
                            sControlledCharacter->mSpriteScale,
                            3,
                            128u,
                            128u,
                            128u);

                        ae_new<ParticleBurst>(sControlledCharacter->mXPos,
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
                            sControlledCharacter->mSpriteScale,
                            3,
                            128u,
                            128u,
                            128u);

                       ae_new<ParticleBurst>(sControlledCharacter->mXPos,
                                                                    sControlledCharacter->mYPos - FP_FromInteger(18),
                                                                    9u,
                                                                    FP_FromInteger(1),
                                                                    BurstType::eBigRedSparks_3,
                                                                    9);
                    }
                    field_54_effect_created = 1;
                }

                if (!(pObj->mGameObjectFlags.Get(BaseGameObject::eDead)))
                {
                    return;
                }
            }

            sControlledCharacter->mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

            gMap.field_20 = 1;

            const CameraSwapEffects effect = kPathChangeEffectToInternalScreenChangeEffect_55D55C[field_34_mTlvData.field_1E_cam_swap_effect];
            s16 bForceChange = 0;
            if (effect == CameraSwapEffects::ePlay1FMV_5 || effect == CameraSwapEffects::eUnknown_11)
            {
                bForceChange = 1;
            }

            gMap.SetActiveCam(
                field_34_mTlvData.field_18_level,
                field_34_mTlvData.field_16_path,
                field_34_mTlvData.field_14_camera,
                effect,
                field_34_mTlvData.field_20_movie_number,
                bForceChange);

            sControlledCharacter->SetUpdateDelay(3);
            sActiveHero->field_1A0_door_id = field_34_mTlvData.field_12_other_teleporter_switch_id;
            field_30_state = TeleporterState::eTeleporting_2;
        }
        break;

        case TeleporterState::eTeleporting_2:
        {
            gMap.field_20 = 0;

            Path_Teleporter* pTeleporterTlv = static_cast<Path_Teleporter*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera(TlvTypes::Teleporter_88, 0));
            Path_Teleporter_Data tlvData = pTeleporterTlv->field_10_data;
            if (tlvData.field_10_teleporter_switch_id != field_34_mTlvData.field_12_other_teleporter_switch_id)
            {
                while (pTeleporterTlv)
                {
                    pTeleporterTlv = static_cast<Path_Teleporter*>(sPath_dword_BB47C0->TLV_Next_Of_Type(pTeleporterTlv, TlvTypes::Teleporter_88));
                    tlvData = pTeleporterTlv->field_10_data;

                    if (tlvData.field_10_teleporter_switch_id == field_34_mTlvData.field_12_other_teleporter_switch_id)
                    {
                        break;
                    }
                }
            }

            SFX_Play_Pitch(SoundEffect::Zap1_49, 60, -300, tlvData.field_1C_scale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1));
            SpawnRingSparks(&tlvData);

            if (tlvData.field_1C_scale != Scale_short::eFull_0)
            {
                if (sControlledCharacter->mScale == 1)
                {
                    sControlledCharacter->mVelX *= FP_FromDouble(0.5);
                    sControlledCharacter->mVelY *= FP_FromDouble(0.5);
                }
                sControlledCharacter->mSpriteScale = FP_FromDouble(0.5);
                sControlledCharacter->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
                sControlledCharacter->mScale = 0;
            }
            else
            {
                if (sControlledCharacter->mScale == 0)
                {
                    sControlledCharacter->mVelX *= FP_FromInteger(2);
                    sControlledCharacter->mVelY *= FP_FromInteger(2);
                }
                sControlledCharacter->mSpriteScale = FP_FromInteger(1);
                sControlledCharacter->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_32;
                sControlledCharacter->mScale = 1;
            }

            // XPos = TLV xpos + TLV middle point
            sControlledCharacter->mXPos = FP_FromInteger(pTeleporterTlv->field_8_top_left.field_0_x) + FP_FromInteger((pTeleporterTlv->field_C_bottom_right.field_0_x - pTeleporterTlv->field_8_top_left.field_0_x) / 2);

            sControlledCharacter->MapFollowMe(TRUE);

            const u8 lineType = sControlledCharacter->mScale == 0 ? 0xF0 : 0x1F;

            PathLine* pPathLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_5C1128->Raycast(
                    sControlledCharacter->mXPos,
                    FP_FromInteger(pTeleporterTlv->field_8_top_left.field_2_y),
                    sControlledCharacter->mXPos,
                    FP_FromInteger(pTeleporterTlv->field_C_bottom_right.field_2_y),
                    &pPathLine,
                    &hitX,
                    &hitY,
                    lineType))
            {
                sControlledCharacter->mCollisionLine = pPathLine;
                sControlledCharacter->mYPos = hitY;
            }
            else
            {
                sControlledCharacter->mCollisionLine = nullptr;
                sControlledCharacter->mYPos = FP_FromInteger(pTeleporterTlv->field_8_top_left.field_2_y);
                sControlledCharacter->mLastLineYPos = sControlledCharacter->mYPos;
            }
            field_30_state = TeleporterState::eOutOfTeleporter_4;
        }
        break;

        case TeleporterState::eOutOfTeleporter_4:
        {
            // Visual effects.
            PSX_RECT bRect = {};
            sControlledCharacter->VGetBoundingRect(&bRect, 1);

            // White flash in the middle of Abe's body.
            New_DestroyOrCreateObject_Particle(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2) + (sControlledCharacter->mSpriteScale * FP_FromInteger(60)),
                sControlledCharacter->mSpriteScale);

            // Spawn the falling "red" sparks from Abe's feet that appear after you've arrived at the destination.
            if (sControlledCharacter->mSpriteScale == FP_FromDouble(0.5))
            {
                ae_new<ParticleBurst>(sControlledCharacter->mXPos,
                                                            sControlledCharacter->mYPos - FP_FromInteger(9),
                                                            6u,
                                                            FP_FromDouble(0.5),
                                                            BurstType::eBigRedSparks_3,
                                                            9);
            }
            else
            {
                ae_new<ParticleBurst>(sControlledCharacter->mXPos,
                                                            sControlledCharacter->mYPos - FP_FromInteger(18),
                                                            6u,
                                                            FP_FromInteger(1),
                                                            BurstType::eBigRedSparks_3,
                                                            9);
            }

            field_54_effect_created = 0;
            sControlledCharacter->mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render);
            sControlledCharacter->mAliveGameObjectFlags.Clear(Flags_114::e114_Bit10_Teleporting);
            field_2C_switch_state = SwitchStates_Get(field_34_mTlvData.field_1A_switch_id);
            field_30_state = TeleporterState::eWaitForSwitchOn_0;
        }
        break;
    }
}
