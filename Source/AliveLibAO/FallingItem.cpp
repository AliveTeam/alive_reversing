#include "stdafx_ao.h"
#include "Function.hpp"
#include "FallingItem.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "ScreenShake.hpp"
#include "ParticleBurst.hpp"
#include "Midi.hpp"
#include "SwitchStates.hpp"

namespace AO {

ALIVE_VAR(1, 0x4FFA54, FallingItem*, pPrimaryFallingItem_4FFA54, nullptr);

const FallingItem_Data sFallingItemData_4BAB20[16] = {
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // menu
    {AnimId::FallingMeat_Falling, AnimId::FallingMeat_Waiting, 66, 42},       // rupture farms
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // lines
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // forest
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // forest temple
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // stock yards
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // stock yards return
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // removed
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // desert
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // desert temple
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // credits
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // removed
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // board room
    {AnimId::FallingMeat_Falling, AnimId::FallingMeat_Waiting, 66, 42},       // rupture farms return
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}, // forest chase
    {AnimId::AO_FallingRock_Falling, AnimId::AO_FallingRock_Waiting, 76, 38}};// desert escape

FallingItem::FallingItem(Path_FallingItem* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);

    mBaseGameObjectTypeId = ReliveTypes::eRockSpawner;

    field_10C_tlvInfo = tlvInfo;

    const s32 lvlIdx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
    const AnimRecord& rec = AO::AnimRec(sFallingItemData_4BAB20[lvlIdx].field_0_falling_animId);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(
        rec.mFrameTableOffset,
        rec.mMaxW,
        rec.mMaxH,
        ppRes,
        1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_DoorFlameRollingBallPortalClip_Half_31;
    if (gMap.mCurrentLevel == EReliveLevelIds::eLines)
    {
        mBaseAnimatedWithPhysicsGameObject_RGB.r = 77;
        mBaseAnimatedWithPhysicsGameObject_RGB.g = 120;
        mBaseAnimatedWithPhysicsGameObject_RGB.b = 190;
    }

    field_112_switch_id = pTlv->field_18_switch_id;
    if (pTlv->field_1A_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }


    field_118_fall_interval = pTlv->field_1C_fall_interval;
    field_114_max_falling_items = pTlv->field_1E_max_falling_items;
    field_116_remaining_falling_items = pTlv->field_1E_max_falling_items;

    field_120_reset_switch_id_after_use = pTlv->field_20_reset_switch_id_after_use;
    field_122_do_sound_in_state_falling = 1;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_128_xpos = FP_FromInteger((pTlv->field_14_bottom_right.field_0_x + pTlv->field_10_top_left.field_0_x) / 2);
    field_12C_ypos = FP_FromInteger(pTlv->field_14_bottom_right.field_2_y);

    field_124_yPosStart = mBaseAnimatedWithPhysicsGameObject_YPos;
    field_110_state = State::eWaitForIdEnable_0;
    field_130_sound_channels = 0;

    // Not sure why this rupture farms primary item hack is required
    if (!pPrimaryFallingItem_4FFA54 && (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn))
    {
        pPrimaryFallingItem_4FFA54 = this;
        field_134_created_gnFrame = sGnFrame;
    }

    mShadow = relive_new Shadow();
}

FallingItem::~FallingItem()
{
    if (pPrimaryFallingItem_4FFA54 == this)
    {
        pPrimaryFallingItem_4FFA54 = nullptr;
    }
    gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
}

void FallingItem::DamageHitItems()
{
    for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj != this)
        {
            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
            {
                BaseAnimatedWithPhysicsGameObject* pAliveObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                const PSX_RECT fallingItemRect = VGetBoundingRect();
                const PSX_RECT objRect = pAliveObj->VGetBoundingRect();

                if (fallingItemRect.x <= objRect.w && fallingItemRect.w >= objRect.x && fallingItemRect.y <= objRect.h && fallingItemRect.h >= objRect.y)
                {
                    if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                    {
                        static_cast<BaseAliveGameObject*>(pAliveObj)->VTakeDamage(this);
                    }
                }
            }
        }
    }
}

void FallingItem::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (pPrimaryFallingItem_4FFA54 == this)
    {
        if (!((sGnFrame - field_134_created_gnFrame) % 87))
        {
            SFX_Play_Mono(SoundEffect::MeatsawOffscreen_88, 45, 0);
        }

        if (!((sGnFrame - field_134_created_gnFrame) % 25))
        {
            SFX_Play_Mono(SoundEffect::MeatsawIdle_89, 45, 0);
        }
    }

    switch (field_110_state)
    {
        case State::eWaitForIdEnable_0:
            if (!SwitchStates_Get(field_112_switch_id))
            {
                return;
            }
            [[fallthrough]];

        case State::eGoWaitForDelay_1:
        {
            mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
            field_110_state = State::eWaitForFallDelay_2;
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            const AnimRecord& rec = AO::AnimRec(sFallingItemData_4BAB20[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].field_4_waiting_animId);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
            field_11C_delay_timer = sGnFrame + field_118_fall_interval;
            break;
        }

        case State::eWaitForFallDelay_2:
            if (static_cast<s32>(sGnFrame) >= field_11C_delay_timer)
            {
                field_110_state = State::eFalling_3;
                field_122_do_sound_in_state_falling = TRUE;
                field_130_sound_channels = SFX_Play_Pitch(SoundEffect::AirStream_28, 50, -2600, 0);
            }
            break;

        case State::eFalling_3:
        {
            if (field_122_do_sound_in_state_falling)
            {
                if (mBaseAnimatedWithPhysicsGameObject_YPos >= sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(120))
                {
                    field_122_do_sound_in_state_falling = 0;
                    SFX_Play_Pitch(SoundEffect::AirStream_28, 127, -1300, 0);
                }
            }

            DamageHitItems();

            if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(20))
            {
                mBaseAnimatedWithPhysicsGameObject_VelY += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.8);
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_VelY + mBaseAnimatedWithPhysicsGameObject_YPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 1 : 16)
                == 1)
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                field_110_state = State::eSmashed_4;

                relive_new ScreenShake(0);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    relive_new ParticleBurst(
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        25,
                        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                        BurstType::eMeat_4);
                }
                else
                {
                    relive_new ParticleBurst(
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        25,
                        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                        BurstType::eFallingRocks_0);
                }

                relive_new ParticleBurst(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    25,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    BurstType::eSticks_1);
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
            }
            break;
        }

        case State::eSmashed_4:
        {
            if (field_130_sound_channels)
            {
                SND_Stop_Channels_Mask_4774A0(field_130_sound_channels);
                field_130_sound_channels = 0;
            }

            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                if (gMap.Is_Point_In_Current_Camera_4449C0(
                        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                        mBaseAnimatedWithPhysicsGameObject_PathNumber,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        0))
                {
                    SFX_Play_Pitch(SoundEffect::KillEffect_78, 127, -700, 0);
                    SFX_Play_Mono(SoundEffect::FallingItemHit_53, 110, 0);
                }
                else
                {
                    SND_SEQ_Play_477760(SeqId::Unknown_25, 1, 65, 65);
                    SFX_Play_Pitch(SoundEffect::KillEffect_78, 90, -700, 0);
                    SFX_Play_Mono(SoundEffect::FallingItemHit_53, 33, 0);
                    SFX_Play_Pitch(SoundEffect::MeatsawUp_90, 80, -400, 0);
                }
            }
            else
            {
                SFX_Play_Mono(SoundEffect::FallingItemLand_73, 0, 0);
                SFX_Play_Pitch(SoundEffect::FallingItemHit_53, 110, -1536, 0);
            }

            if (field_112_switch_id)
            {
                if (field_120_reset_switch_id_after_use == Choice_short::eYes_1)
                {
                    SwitchStates_Do_Operation(field_112_switch_id, SwitchOp::eSetFalse_1);
                }
            }

            field_116_remaining_falling_items--;

            if ((field_114_max_falling_items && field_116_remaining_falling_items <= 0) || !gMap.Is_Point_In_Current_Camera_4449C0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, field_128_xpos, field_12C_ypos, 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                const AnimRecord& rec = AO::AnimRec(sFallingItemData_4BAB20[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].field_0_falling_animId);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_YPos = field_124_yPosStart;
                field_110_state = State::eWaitForIdEnable_0;
            }
            break;
        }

        default:
            return;
    }
}

void FallingItem::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    // Empty
}

void FallingItem::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel
        || gMap.mCurrentPath != gMap.mPath
        || field_110_state != State::eFalling_3)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
