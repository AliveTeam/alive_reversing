#include "stdafx.h"
#include "MovingBomb.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Sound/Midi.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "BaseAliveGameObject.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"

const TintEntry kMovingBombTints_55C734[4] = {
    {LevelIds_s8::eBarracks_6, 97u, 97u, 97u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};

ALIVE_VAR(1, 0x5C300C, MovingBomb*, gMovingBomb_5C300C, nullptr);

MovingBomb* MovingBomb::ctor_46FD40(Path_MovingBomb* pTlv, s32 tlvInfo)
{
    ctor_408240(0);
    mFlags.Set(BaseGameObject::eCanExplode_Bit7);

    SetVTable(this, 0x546270);
    SetType(AETypes::eTimedMine_or_MovingBomb_10);

    const AnimRecord& rec = AnimRec(AnimId::MovingBomb);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = TPageAbr::eBlend_0;
    field_118_state = States::eTriggeredBySwitch_1;

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = Layer::eLayer_BombMineCar_Half_16;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = Layer::eLayer_BombMineCar_35;
    }

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_124_speed = FP_FromRaw(pTlv->field_10_speed << 8);
    field_C4_velx = FP_FromRaw(pTlv->field_1C_start_speed << 8);
    field_128_start_moving_switch_id = pTlv->field_12_start_moving_switch_id;
    field_120_timer = sGnFrame_5C1B84;
    field_11C_tlvInfo = tlvInfo;
    field_12C_max = 0;
    field_12A_min = 0;
    field_136_persist_offscreen = pTlv->field_1E_persist_offscreen;
    field_130_sound_channels = 0;

    if (pTlv->field_14_bTriggered_by_alarm == Choice_short::eYes_1)
    {
        field_118_state = States::eTriggeredByAlarm_0;
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    SetTint_425600(&kMovingBombTints_55C734[0], gMap.mCurrentLevel);

    field_134_disabled_resources = pTlv->field_1A_disabled_resources;
    if (!(field_134_disabled_resources & 1))
    {
        Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);
    }

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
    Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kAbeblowResID);
    Add_Resource(ResourceManager::Resource_Palt, AEResourceID::kSlogBlowResID);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos + FP_FromInteger(24),
            field_BC_ypos + FP_FromInteger(24),
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            0x100))
    {
        field_BC_ypos = hitY;
        field_B8_xpos = hitX;
    }

    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* MovingBomb::VDestructor(s32 flags)
{
    return vdtor_470040(flags);
}

void MovingBomb::VUpdate()
{
    vUpdate_4701E0();
}

void MovingBomb::VRender(PrimHeader** ppOt)
{
    vRender_4707D0(ppOt);
}

void MovingBomb::VOnThrowableHit(BaseGameObject* pFrom)
{
    vOnThrowableHit_470800(pFrom);
}

s16 MovingBomb::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_470990(pFrom);
}

void MovingBomb::VScreenChanged()
{
    vScreenChanged_470B90();
}

MovingBomb* MovingBomb::vdtor_470040(s32 flags)
{
    dtor_4700C0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void MovingBomb::dtor_4700C0()
{
    SetVTable(this, 0x546270);
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
    }

    if (field_118_state >= States::eBlowingUp_6)
    {
        Path::TLV_Reset_4DB8E0(field_11C_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_11C_tlvInfo, -1, 0, 0);
    }

    if (gMovingBomb_5C300C == this)
    {
        if (field_130_sound_channels)
        {
            SND_Stop_Channels_Mask_4CA810(field_130_sound_channels);
            field_130_sound_channels = 0;
        }
        gMovingBomb_5C300C = 0;
    }
    dtor_4080B0();
}

EXPORT void MovingBomb::BlowUp_470070()
{
    mFlags.Clear(BaseGameObject::eCanExplode_Bit7);
    field_118_state = States::eBlowingUp_6;
    field_C8_vely = FP_FromInteger(0);
    field_120_timer = sGnFrame_5C1B84 + 1;
    SFX_Play_46FA90(SoundEffect::GreenTick_2, 100, field_CC_sprite_scale);
}

void MovingBomb::vRender_4707D0(PrimHeader** ot)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void MovingBomb::vScreenChanged_470B90()
{
    BaseGameObject::VScreenChanged();

    if (field_136_persist_offscreen == Choice_short::eNo_0)
    {
        mFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP xDelta = FP_Abs(sControlledCharacter_5C1B8C->field_B8_xpos - field_B8_xpos);
    if (xDelta > FP_FromInteger(750))
    {
        mFlags.Set(BaseGameObject::eDead);
        return;
    }

    const FP yDelta = FP_Abs(sControlledCharacter_5C1B8C->field_BC_ypos - field_BC_ypos);
    if (yDelta > FP_FromInteger(520))
    {
        mFlags.Set(BaseGameObject::eDead);
        return;
    }
}

void MovingBomb::FollowLine_470950()
{
    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
    }
}

s16 MovingBomb::vTakeDamage_470990(BaseGameObject* pFrom)
{
    if (mFlags.Get(BaseGameObject::eDead) || field_10C_health <= FP_FromInteger(0))
    {
        return 1;
    }

    switch (pFrom->Type())
    {
        case AETypes::eAbilityRing_104:
        case AETypes::eExplosion_109:
        case AETypes::eShrykull_121:
        {
            field_10C_health = FP_FromInteger(0);
            auto pExplosion = ae_new<Explosion>();
            if (pExplosion)
            {
                pExplosion->ctor_4A1200(field_B8_xpos, field_BC_ypos, field_CC_sprite_scale, 0);
            }

            auto pGibs = ae_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_40FB40(GibType::Metal_5, field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, 0);
            }

            field_118_state = States::eKillMovingBomb_7;

            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_120_timer = sGnFrame_5C1B84 + 4;
        }
            return 0;

        case AETypes::eElectrocute_150:
            field_118_state = States::eKillMovingBomb_7;
            return 0;

        default:
            return 1;
    }
}

void MovingBomb::vOnThrowableHit_470800(BaseGameObject* /*pObj*/)
{
    if (!field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        BlowUp_470070();
    }
}

s16 MovingBomb::HitObject_470830()
{
    PSX_RECT bRect = {};
    vGetBoundingRect_424FD0(&bRect, 1);
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this)
        {
            if (pObj->field_114_flags.Get(Flags_114::e114_Bit6_SetOffExplosives))
            {
                if (pObj->field_10C_health > FP_FromInteger(0))
                {
                    PSX_RECT objRect = {};
                    pObj->vGetBoundingRect_424FD0(&objRect, 1);
                    if (bRect.x <= objRect.w && bRect.w >= objRect.x && bRect.h >= objRect.y && bRect.y <= objRect.h && pObj->field_CC_sprite_scale == field_CC_sprite_scale && pObj->field_C0_path_number == field_C0_path_number)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void MovingBomb::vUpdate_4701E0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (field_118_state < States::eBlowingUp_6)
    {
        if (HitObject_470830())
        {
            if (!field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
            {
                BlowUp_470070();
            }
        }
    }

    if (gMovingBomb_5C300C == nullptr || gMovingBomb_5C300C == this)
    {
        if (field_20_animation.field_92_current_frame != 0 && field_20_animation.field_92_current_frame != 7)
        {
            gMovingBomb_5C300C = this;
        }
        else
        {
            if (field_130_sound_channels)
            {
                SND_Stop_Channels_Mask_4CA810(field_130_sound_channels);
            }

            if (vIsObjNearby_4253B0(FP_FromInteger(700), sActiveHero_5C1B68))
            {
                const FP yDelta = FP_Abs(sActiveHero_5C1B68->field_BC_ypos - field_BC_ypos);
                if (yDelta <= FP_FromInteger(700))
                {
                    if (field_118_state == States::eWaitABit_4)
                    {
                        field_130_sound_channels = SFX_Play_46FA90(SoundEffect::SecurityOrb_48, 55, field_CC_sprite_scale);
                    }
                    else
                    {
                        field_130_sound_channels = SFX_Play_46FA90(SoundEffect::SecurityOrb_48, 80, field_CC_sprite_scale);
                    }
                    gMovingBomb_5C300C = this;
                }
                else
                {
                    if (field_118_state == States::eWaitABit_4)
                    {
                        field_130_sound_channels = 0;
                        gMovingBomb_5C300C = this;
                    }
                    else
                    {
                        field_130_sound_channels = SFX_Play_46FA90(SoundEffect::SecurityOrb_48, 12, field_CC_sprite_scale);
                        gMovingBomb_5C300C = this;
                    }
                }
            }
        }
    }

    switch (field_118_state)
    {
        case States::eTriggeredByAlarm_0:
            if (Event_Get_422C00(kEventAlarm))
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eTriggeredBySwitch_1:
            if (SwitchStates_Get_466020(field_128_start_moving_switch_id))
            {
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eMoving_2:
            if (field_C4_velx < field_124_speed)
            {
                field_C4_velx += (field_CC_sprite_scale * FP_FromDouble(0.5));
            }

            FollowLine_470950();

            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                TlvTypes::MovingBombStopper_53);

            if (field_FC_pPathTLV)
            {
                auto pStopper = static_cast<Path_MovingBombStopper*>(field_FC_pPathTLV);
                field_12A_min = pStopper->field_10_min;
                field_12C_max = pStopper->field_12_max;
                field_118_state = States::eStopMoving_3;
            }
            break;

        case States::eStopMoving_3:
            field_C4_velx -= (field_CC_sprite_scale * FP_FromDouble(0.5));
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_118_state = States::eWaitABit_4;
                field_120_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(field_12A_min, field_12C_max);
            }

            FollowLine_470950();
            break;

        case States::eWaitABit_4:
            if (field_120_timer <= static_cast<s32>(sGnFrame_5C1B84))
            {
                field_118_state = States::eToMoving_5;
            }
            break;

        case States::eToMoving_5:
            if (field_C4_velx < field_124_speed)
            {
                field_C4_velx += (field_CC_sprite_scale * FP_FromDouble(0.5));
            }

            FollowLine_470950();

            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(field_BC_ypos),
                TlvTypes::MovingBombStopper_53);

            if (!field_FC_pPathTLV)
            {
                field_118_state = States::eMoving_2;
            }
            break;

        case States::eBlowingUp_6:
            if (field_120_timer <= static_cast<s32>(sGnFrame_5C1B84))
            {
                SFX_Play_46FA90(SoundEffect::GreenTick_2, 100, field_CC_sprite_scale);

                field_10C_health = FP_FromInteger(0);

                auto pExplosion = ae_new<Explosion>();
                if (pExplosion)
                {
                    pExplosion->ctor_4A1200(
                        field_B8_xpos,
                        field_BC_ypos,
                        field_CC_sprite_scale,
                        0);
                }

                auto pGibs = ae_new<Gibs>();
                if (pGibs)
                {
                    pGibs->ctor_40FB40(
                        GibType::Metal_5,
                        field_B8_xpos,
                        field_BC_ypos,
                        FP_FromInteger(0),
                        FP_FromInteger(5),
                        field_CC_sprite_scale,
                        0);
                }

                field_118_state = States::eKillMovingBomb_7;
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_120_timer = sGnFrame_5C1B84 + 4;
            }
            break;

        case States::eKillMovingBomb_7:
            if (field_120_timer <= static_cast<s32>(sGnFrame_5C1B84))
            {
                mFlags.Set(BaseGameObject::eDead);
            }
            break;

        default:
            return;
    }
}
