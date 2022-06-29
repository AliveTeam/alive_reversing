#include "stdafx.h"
#include "Door.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "MusicTrigger.hpp"
#include "Sound/Midi.hpp"
#include "DDCheat.hpp"
#include "BaseAliveGameObject.hpp"
#include "Grid.hpp"

struct Door_Info final
{
    s32 field_0_frameTableOffset_closed;
    s32 field_4_frameTableOffset_open;
    s32 field_8_maxW;
    s32 field_C_maxH;
};
ALIVE_ASSERT_SIZEOF(Door_Info, 0x10);

const AnimId sDoorAnimIdTable_544888[16][2] = {
    {AnimId::Door_Mines_Closed, AnimId::Door_Mines_Open},
    {AnimId::Door_Mines_Closed, AnimId::Door_Mines_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Feeco_Closed, AnimId::Door_Feeco_Open},
    {AnimId::Door_Barracks_Closed, AnimId::Door_Barracks_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Bonewerkz_Closed, AnimId::Door_Bonewerkz_Open},
    {AnimId::Door_Brewery_Closed, AnimId::Door_Brewery_Open},
    {AnimId::Door_Brewery_Closed, AnimId::Door_Brewery_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Feeco_Closed, AnimId::Door_Feeco_Open},
    {AnimId::Door_Barracks_Closed, AnimId::Door_Barracks_Open},
    {AnimId::Door_Bonewerkz_Closed, AnimId::Door_Bonewerkz_Open},
    {AnimId::Door_Mines_Closed, AnimId::Door_Mines_Open}};

Door::Door()
    : BaseAnimatedWithPhysicsGameObject(0)
{
}

Door::Door(Path_Door* pTlvData, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eDoor);

    field_F4_tlvInfo = tlvInfo;
    field_F8_door_type = pTlvData->field_1E_type;
    field_FE_start_state = pTlvData->field_20_start_state;

    if (pTlvData->field_40_close_on_exit == Choice_short::eYes_1)
    {
        if (pTlvData->mTlvState)
        {
            field_FE_start_state = eClosed_1;
        }
    }

    field_100_switch_id = pTlvData->field_1A_switch_id;
    if (pTlvData->field_1A_switch_id == 1)
    {
        field_100_switch_id = 0;
    }

    field_FA_door_number = pTlvData->field_18_door_number;
    if (gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot)
    {
        switch (field_FA_door_number)
        {
            case 30000:
                if (sVisitedBonewerks_5C1C02)
                {
                    field_FE_start_state = eClosed_1;
                }
                break;

            case 30001:
            case 30004:
                if (sVisitedBarracks_5C1C04)
                {
                    field_FE_start_state = eClosed_1;
                }
                break;

            case 30002:
                if (sVisitedBonewerks_5C1C02)
                {
                    SwitchStates_Set(255u, 1);
                }

                if (sVisitedBarracks_5C1C04)
                {
                    SwitchStates_Set(254u, 1);
                }

                if (sVisitedFeecoEnder_5C1C06)
                {
                    SwitchStates_Set(255u, 0);
                    SwitchStates_Set(254u, 0);
                }
                break;

            case 30003:
                if (sVisitedBonewerks_5C1C02)
                {
                    if (sVisitedBarracks_5C1C04)
                    {
                        if (sVisitedFeecoEnder_5C1C06)
                        {
                            field_FE_start_state = eOpen_0;
                        }
                    }
                    if (sVisitedBonewerks_5C1C02)
                    {
                        SwitchStates_Set(250u, 1);
                    }
                }

                if (sVisitedBarracks_5C1C04)
                {
                    SwitchStates_Set(251u, 1);
                }
                if (sVisitedFeecoEnder_5C1C06)
                {
                    SwitchStates_Set(253u, 1);
                }
                break;
            default:
                break;
        }
    }

    if (field_F8_door_type == DoorTypes::eTasksDoor_3)
    {
        field_102_hub_ids[0] = pTlvData->field_22_hub1;
        field_102_hub_ids[1] = pTlvData->field_22_hub2;
        field_102_hub_ids[2] = pTlvData->field_22_hub3;
        field_102_hub_ids[3] = pTlvData->field_22_hub4;
        field_102_hub_ids[4] = pTlvData->field_22_hub5;
        field_102_hub_ids[5] = pTlvData->field_22_hub6;
        field_102_hub_ids[6] = pTlvData->field_22_hub7;
        field_102_hub_ids[7] = pTlvData->field_22_hub8;

        if (SwitchStates_Get(field_102_hub_ids[0]) && SwitchStates_Get(field_102_hub_ids[1]) && SwitchStates_Get(field_102_hub_ids[2]) && SwitchStates_Get(field_102_hub_ids[3]) && SwitchStates_Get(field_102_hub_ids[4]) && SwitchStates_Get(field_102_hub_ids[5]) && SwitchStates_Get(field_102_hub_ids[6]) && SwitchStates_Get(field_102_hub_ids[7]))
        {
            SwitchStates_Do_Operation(field_100_switch_id, SwitchOp::eSetTrue_0);
        }
        else
        {
            SwitchStates_Do_Operation(field_100_switch_id, SwitchOp::eSetFalse_1);
        }
    }

    if (field_FE_start_state == eOpen_0)
    {
        if (SwitchStates_Get(field_100_switch_id))
        {
            // Its open but the switch id is on which causes it to close
            field_FC_current_state = eClosed_1;
        }
        else
        {
            field_FC_current_state = eOpen_0;
        }
    }
    else
    {
        if (SwitchStates_Get(field_100_switch_id))
        {
            // Its closed but the id is on which causes it to open
            field_FC_current_state = eOpen_0;
        }
        else
        {
            field_FC_current_state = eClosed_1;
        }
    }

    if ((sActiveHero->mCurrentMotion == eAbeMotions::Motion_114_DoorEnter_459470 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_115_DoorExit_459A40) && field_FC_current_state == eClosed_1 && field_FA_door_number == sActiveHero->field_1A0_door_id)
    {
        // Force open is abe is in the door
        field_FC_current_state = eOpen_0;
    }

    if (field_F8_door_type == DoorTypes::eTasksDoorWithSecretMusic_2)
    {
        field_102_hub_ids[0] = pTlvData->field_22_hub1;
        field_102_hub_ids[1] = pTlvData->field_22_hub2;
        field_102_hub_ids[2] = pTlvData->field_22_hub3;
        field_102_hub_ids[3] = pTlvData->field_22_hub4;
        field_102_hub_ids[4] = pTlvData->field_22_hub5;
        field_102_hub_ids[5] = pTlvData->field_22_hub6;
        field_102_hub_ids[6] = pTlvData->field_22_hub7;
        field_102_hub_ids[7] = pTlvData->field_22_hub8;
    }

    const AnimRecord& closedRec = AnimRec(sDoorAnimIdTable_544888[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0]);
    const AnimRecord& openRec = AnimRec(sDoorAnimIdTable_544888[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID);
    if (!ppRes || openRec.mFrameTableOffset == 0)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (field_FC_current_state == eOpen_0)
    {
        if (gMap.mOverlayId == 108)
        {
            const AnimRecord& rec = AnimRec(AnimId::Door_BarracksMetal_Open);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
        }
        else
        {
            Animation_Init(openRec.mFrameTableOffset, openRec.mMaxW, openRec.mMaxH, ppRes, 1);
        }
    }
    else
    {
        if (gMap.mOverlayId == 108)
        {
            const AnimRecord& rec = AnimRec(AnimId::Door_BarracksMetal_Closed);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
        }
        else
        {
            Animation_Init(closedRec.mFrameTableOffset, closedRec.mMaxW, closedRec.mMaxH, ppRes, 1);
        }
    }

    if (pTlvData->field_16_scale != Scale_short::eFull_0)
    {
        if (pTlvData->field_16_scale == Scale_short::eHalf_1)
        {
            mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
            mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        }
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }

    FP* xOff = &mBaseAnimatedWithPhysicsGameObject_XPos;
    FP* yOff = &mBaseAnimatedWithPhysicsGameObject_YPos;

    FP tlvXMid = FP_FromInteger((pTlvData->mTopLeft.x + pTlvData->mBottomRight.x) / 2);
    PathLine* pathLine = nullptr;

    if (sCollisions->Raycast(
            tlvXMid,
            FP_FromInteger(pTlvData->mTopLeft.y),
            tlvXMid,
            FP_FromInteger(pTlvData->mBottomRight.y),
            &pathLine,
            xOff,
            yOff,
            (mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {
        // Move up off the line we hit
        *yOff -= FP_FromInteger(12) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

        // Snap on X
        *xOff = FP_FromInteger(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(*xOff)));
    }
    else
    {
        // Couldn't glue to the floor.. just use the TLV pos
        *xOff = FP_FromInteger(pTlvData->mTopLeft.x + 12);
        *yOff = FP_FromInteger(pTlvData->mTopLeft.y + 24);
    }

    // Add on the TLV offset
    *xOff += FP_FromInteger(pTlvData->field_36_x_offset);
    *yOff += FP_FromInteger(pTlvData->field_38_y_offset);

    // Another OWI special
    FP yAdjustHack = {};
    if ((gMap.mCurrentLevel != EReliveLevelIds::eBarracks && gMap.mCurrentLevel != EReliveLevelIds::eBarracks_Ender) || gMap.mOverlayId == 108)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender)
        {
            yAdjustHack = FP_FromInteger(10) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        }
    }
    else
    {
        yAdjustHack = FP_FromInteger(14) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    }
    *yOff += yAdjustHack;

    if (field_FC_current_state == eOpen_0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    }

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
}

Bool32 Door::vIsOpen()
{
    return field_FC_current_state == eOpen_0;
}

void Door::vOpen()
{
    if (field_FC_current_state != eOpen_0)
    {
        field_FC_current_state = eOpening_2;
    }
}

void Door::vClose()
{
    if (field_FC_current_state != eClosed_1)
    {
        field_FE_start_state = eClosed_1;
        field_FC_current_state = eClosing_3;
        Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_F4_tlvInfo);
        pTlv->mTlvState = 1;
    }
}

void Door::vSetOpen()
{
    field_FC_current_state = eOpen_0;
}

void Door::vSetClosed()
{
    field_FC_current_state = eClosed_1;
}

void Door::PlaySound()
{
    s16 volume = 0;
    if (field_F8_door_type != DoorTypes::eBasicDoor_0 || mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1))
    {
        volume = 60;
    }
    else
    {
        volume = 90;
    }

    SFX_Play_Pitch(SoundEffect::DoorEffect_57, volume, 900);
    SFX_Play_Pitch(SoundEffect::DoorEffect_57, volume, 0);
}

Door::~Door()
{
    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
}

void Door::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_114_DoorEnter_459470 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_115_DoorExit_459A40)
    {
        if (field_FC_current_state == DoorStates::eClosed_1 && field_FA_door_number == sActiveHero->field_1A0_door_id)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            field_FC_current_state = DoorStates::eOpen_0;
        }
    }
    else
    {
        field_FA_door_number = -1;
        if (field_F8_door_type == DoorTypes::eTasksDoorWithSecretMusic_2 || field_F8_door_type == DoorTypes::eTasksDoor_3)
        {
            if (SwitchStates_Get(field_102_hub_ids[0])
                && SwitchStates_Get(field_102_hub_ids[1])
                && SwitchStates_Get(field_102_hub_ids[2])
                && SwitchStates_Get(field_102_hub_ids[3])
                && SwitchStates_Get(field_102_hub_ids[4])
                && SwitchStates_Get(field_102_hub_ids[5])
                && SwitchStates_Get(field_102_hub_ids[6])
                && SwitchStates_Get(field_102_hub_ids[7]))
            {
                if (!SwitchStates_Get(field_100_switch_id) && field_F8_door_type == DoorTypes::eTasksDoorWithSecretMusic_2)
                {
                    SND_SEQ_Play(SeqId::SecretMusic_32, 1, 127, 127);
                    relive_new MusicTrigger(MusicTriggerMusicType::eChime_5, TriggeredBy::eTimer_0, 0, 0);
                }
                SwitchStates_Do_Operation(field_100_switch_id, SwitchOp::eSetTrue_0);
            }
            else
            {
                SwitchStates_Do_Operation(field_100_switch_id, SwitchOp::eSetFalse_1);
            }
        }
        switch (field_FC_current_state)
        {
            case DoorStates::eOpen_0:
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

                if ((!field_FE_start_state && SwitchStates_Get(field_100_switch_id))
                    || (field_FE_start_state == DoorStates::eClosed_1 && !SwitchStates_Get(field_100_switch_id)))
                {
                    field_FC_current_state = DoorStates::eClosing_3;
                    if (gMap.mOverlayId == 108)
                    {
                        const AnimRecord& rec = AnimRec(AnimId::Door_BarracksMetal_Open);
                        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                    }
                    else
                    {
                        const AnimRecord& animRec = AnimRec(sDoorAnimIdTable_544888[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]);
                        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                    }

                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit19_LoopBackwards);
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                }
                break;

            case DoorStates::eClosed_1:
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);

                if ((field_FE_start_state == DoorStates::eClosed_1 && SwitchStates_Get(field_100_switch_id)) || (field_FE_start_state == DoorStates::eOpen_0 && !SwitchStates_Get(field_100_switch_id)))
                {
                    field_FC_current_state = DoorStates::eOpening_2;
                    if (gMap.mOverlayId == 108)
                    {
                        const AnimRecord& rec = AnimRec(AnimId::Door_BarracksMetal_Open);
                        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                    }
                    else
                    {
                        const AnimRecord& animRec = AnimRec(sDoorAnimIdTable_544888[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]);
                        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                    }

                    mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(3);
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit19_LoopBackwards);
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                    PlaySound();
                }
                break;

            case DoorStates::eOpening_2:
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    field_FC_current_state = DoorStates::eOpen_0;
                }
                break;

            case DoorStates::eClosing_3:
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    field_FC_current_state = DoorStates::eClosed_1;
                    PlaySound();
                }
                break;

            default:
                return;
        }
    }
}

void Door::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

// ================================================================================================

ALIVE_VAR(1, 0xBB4AA0, FP, sTrainDoorXPos_BB4AA0, {});
ALIVE_VAR(1, 0xBB4AA4, FP, sTrainDoorYPos_BB4AA4, {});

TrainDoor::TrainDoor(Path_TrainDoor* pTlv, s32 tlvInfo)
{
    SetType(ReliveTypes::eDoor);
    field_F4_tlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::Door_Train_Closing);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x + 12);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y + 24);

    sTrainDoorXPos_BB4AA0 = mBaseAnimatedWithPhysicsGameObject_XPos;
    sTrainDoorYPos_BB4AA4 = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (pTlv->mTlvState)
    {
        const AnimRecord& animRec = AnimRec(AnimId::Door_Train_Closed);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, 0);
        field_FC_current_state = eClosed_1;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        field_FC_current_state = eOpen_0;
    }
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pTlv->field_10_direction == XDirection_int::eRight_1);
}

TrainDoor::~TrainDoor()
{
    if (field_FC_current_state == eOpen_0)
    {
        Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_F4_tlvInfo, 1, 0, 0);
    }
}

void TrainDoor::VUpdate()
{
    if (field_FC_current_state == eOpen_0)
    {
        // Wait for Abe to GTFO
        if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_115_DoorExit_459A40 && sActiveHero->mCurrentMotion != eAbeMotions::Motion_114_DoorEnter_459470)
        {
            // Then close
            const AnimRecord& animRec = AnimRec(AnimId::Door_Train_Closing);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, 0);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
            field_FC_current_state = eClosed_1;
        }
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = sTrainDoorXPos_BB4AA0 + FP_FromInteger(sTweakX_5C1BD0);
    mBaseAnimatedWithPhysicsGameObject_YPos = sTrainDoorYPos_BB4AA4 + FP_FromInteger(sTweakY_5C1BD4);
}
