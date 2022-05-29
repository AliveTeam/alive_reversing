#include "stdafx_ao.h"
#include "Function.hpp"
#include "UXB.hpp"
#include "Sfx.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "BaseBomb.hpp"
#include "Events.hpp"
#include "ScreenManager.hpp"
#include <math.h>

namespace AO {

UXB::UXB(Path_UXB* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    field_4_typeId = Types::eUXB_99;

    const AnimRecord& rec = AO::AnimRec(AnimId::UXB_Active);
    u8** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes2, 1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_B_render_mode = TPageAbr::eBlend_0;

    mFlags.Set(Options::eInteractive_Bit8);
    field_1BC_flags.Clear(flags_1BC::eUnused_Bit0);
    field_10C_state = UXBState::eDelay_0;

    field_1B4_pattern_length = pTlv->field_18_pattern_length;
    if (pTlv->field_18_pattern_length < 1u || pTlv->field_18_pattern_length > 4u)
    {
        field_1B4_pattern_length = 1;
    }

    field_1B8_pattern = pTlv->field_1A_pattern;
    if (!pTlv->field_1A_pattern) // If no pattern set, go to a default one.
    {
        field_1B8_pattern = 11111;
    }

    field_1B6_pattern_index = 0;

    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
    field_1BA_red_blink_count = (field_1B8_pattern / static_cast<s32>(pow(10, field_1B4_pattern_length - 1))) % 10;

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = Layer::eLayer_BombRollingBall_Half_16;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = Layer::eLayer_BombRollingBall_35;
        field_C6_scale = 1;
    }

    InitBlinkAnim();

    if (pTlv->field_1_unknown) // Stores the activated/deactivated state for UXB
    {
        if (pTlv->field_1E_state == UXBStartState::eOn_0)
        {
            u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 0, 0);
            field_11C_anim.LoadPal_403090(ppRes, 0);
            field_1BC_flags.Clear(flags_1BC::eIsRed_Bit1);

            const AnimRecord& flashRec = AO::AnimRec(AnimId::Bomb_RedGreenTick);
            field_11C_anim.Set_Animation_Data_402A40(flashRec.mFrameTableOffset, 0);

            if (gMap.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
            {
                SFX_Play_43AD70(SoundEffect::GreenTick_3, 35, 0);
            }

            const AnimRecord& animRec = AO::AnimRec(AnimId::UXB_Disabled);
            field_10_anim.Set_Animation_Data_402A40(animRec.mFrameTableOffset, 0);

            field_10C_state = UXBState::eDeactivated_3;
            field_10E_starting_state = UXBState::eDelay_0;
        }
        else
        {
            field_10E_starting_state = UXBState::eDeactivated_3;
        }
    }
    else
    {
        if (pTlv->field_1E_state == UXBStartState::eOn_0)
        {
            field_10E_starting_state = UXBState::eDelay_0;
        }
        else
        {
            u8** ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 0, 0);
            field_11C_anim.LoadPal_403090(ppPal, 0);
            field_1BC_flags.Clear(flags_1BC::eIsRed_Bit1);

            const AnimRecord& flashRec = AO::AnimRec(AnimId::Bomb_RedGreenTick);
            field_11C_anim.Set_Animation_Data_402A40(flashRec.mFrameTableOffset, 0);

            const AnimRecord& animRec = AO::AnimRec(AnimId::UXB_Disabled);
            field_10_anim.Set_Animation_Data_402A40(animRec.mFrameTableOffset, 0);

            field_10E_starting_state = UXBState::eDeactivated_3;
            field_10C_state = UXBState::eDeactivated_3;
        }
    }

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 24);

    field_114_tlvInfo = tlvInfo;
    field_118_next_state_frame = gnFrameCount_507670;
    field_110_disabled_resources = static_cast<s16>(pTlv->field_20_disabled_resources);

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbebombAOResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kBgexpldAOResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 1, 0);

    if (!(field_110_disabled_resources & 1))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0);
    }

    if (!(field_110_disabled_resources & 4))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 1, 0);
    }

    if (!(field_110_disabled_resources & 2))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, 1, 0);
    }

    if (gMap.mCurrentLevel == LevelIds::eStockYards_5 || gMap.mCurrentLevel == LevelIds::eStockYardsReturn_6)
    {
        field_1BC_flags.Clear(flags_1BC::eIsRed_Bit1);
        field_C0_r = 80;
        field_C2_g = 90;
        field_C4_b = 110;
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 1, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 1, 0);
    }

    const FP gridSnap = ScaleToGridSize_41FA30(field_BC_sprite_scale);
    mFlags.Set(Options::eInteractive_Bit8);

    field_D4_collection_rect.x = field_A8_xpos - (gridSnap / FP_FromInteger(2));
    field_D4_collection_rect.y = field_AC_ypos - gridSnap;
    field_D4_collection_rect.w = field_A8_xpos + (gridSnap / FP_FromInteger(2));
    field_D4_collection_rect.h = field_AC_ypos;
}

void UXB::InitBlinkAnim()
{
    const AnimRecord& tickRec = AO::AnimRec(AnimId::Bomb_RedGreenTick);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, tickRec.mResourceId, 1, 0);
    if (field_11C_anim.Init_402D20(
            tickRec.mFrameTableOffset,
            gObjList_animations_505564,
            this,
            tickRec.mMaxW,
            tickRec.mMaxH,
            ppRes,
            1,
            0,
            0))
    {
        field_11C_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_11C_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);

        field_11C_anim.field_C_layer = field_10_anim.field_C_layer;
        field_11C_anim.field_14_scale = field_BC_sprite_scale;
        field_11C_anim.field_8_r = 128;
        field_11C_anim.field_9_g = 128;
        field_11C_anim.field_A_b = 128;
        field_11C_anim.field_B_render_mode = TPageAbr::eBlend_1;
    }
    else
    {
        mFlags.Set(Options::eListAddFailed_Bit1);
    }
}


UXB::~UXB()
{
    if (field_10C_state != UXBState::eExploding_2 || static_cast<s32>(gnFrameCount_507670) < field_118_next_state_frame)
    {
        gMap.TLV_Reset_446870(field_114_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset_446870(field_114_tlvInfo, -1, 0, 1);
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbebombAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kBgexpldAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 0, 0));

    if (!(field_110_disabled_resources & 1))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 0, 0));
    }

    if (!(field_110_disabled_resources & 4))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 0, 0));
    }

    if (!(field_110_disabled_resources & 2))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, 0, 0));
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 0, 0));

    field_11C_anim.vCleanUp();

    mFlags.Clear(Options::eInteractive_Bit8);
}

void UXB::VScreenChanged()
{
    VScreenChanged_489BD0();
}

void UXB::VScreenChanged_489BD0()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        if (field_10E_starting_state == UXBState::eDeactivated_3 && field_10C_state != UXBState::eDeactivated_3)
        {
            gMap.TLV_Reset_446870(field_114_tlvInfo, 1, 1u, 0);
            mFlags.Set(BaseGameObject::eDead);
        }
        else if (field_10E_starting_state != UXBState::eDelay_0 || field_10C_state != UXBState::eDeactivated_3)
        {
            gMap.TLV_Reset_446870(field_114_tlvInfo, 0, 1u, 0);
            mFlags.Set(BaseGameObject::eDead);
        }
        else
        {
            gMap.TLV_Reset_446870(field_114_tlvInfo, 1, 1u, 0);
            mFlags.Set(BaseGameObject::eDead);
        }
    }
}

s16 UXB::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_489AB0(pFrom);
}

s16 UXB::VTakeDamage_489AB0(BaseGameObject* pFrom)
{
    if (mFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
        case Types::eAbe_43:
            if (field_10C_state == UXBState::eDeactivated_3)
            {
                return 0;
            }
            break;

        case Types::eAbilityRing_69:
        case Types::eExplosion_74:
        case Types::eShrykull_85:
            break;

        default:
            return 0;
    }

    mFlags.Set(BaseGameObject::eDead);

    ao_new<BaseBomb>(
        field_A8_xpos,
        field_AC_ypos,
        0,
        field_BC_sprite_scale);

    field_10C_state = UXBState::eExploding_2;
    field_118_next_state_frame = gnFrameCount_507670;

    return 1;
}

void UXB::VOnThrowableHit(BaseGameObject* pFrom)
{
    VOnThrowableHit_489A30(pFrom);
}

void UXB::VOnThrowableHit_489A30(BaseGameObject* /*pFrom*/)
{
    ao_new<BaseBomb>(
        field_A8_xpos,
        field_AC_ypos,
        0,
        field_BC_sprite_scale);

    mFlags.Set(BaseGameObject::eDead);
    field_10C_state = UXBState::eExploding_2;
    field_118_next_state_frame = gnFrameCount_507670;
}

void UXB::VOnPickUpOrSlapped()
{
    VOnPickUpOrSlapped_4897E0();
}

void UXB::VOnPickUpOrSlapped_4897E0()
{
    if (field_10C_state != UXBState::eExploding_2)
    {
        if (field_10C_state != UXBState::eDeactivated_3 || field_118_next_state_frame > static_cast<s32>(gnFrameCount_507670))
        {
            if (field_1BA_red_blink_count)
            {
                field_10C_state = UXBState::eExploding_2;
                field_118_next_state_frame = gnFrameCount_507670 + 2;
            }
            else
            {
                const AnimRecord& flashRec = AO::AnimRec(AnimId::Bomb_RedGreenTick);
                field_11C_anim.Set_Animation_Data_402A40(flashRec.mFrameTableOffset, 0);
                if (gMap.Is_Point_In_Current_Camera_4449C0(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_AC_ypos,
                        0))
                {
                    SFX_Play_43AD70(SoundEffect::GreenTick_3, 35, 0);
                }
                const AnimRecord& animRec = AO::AnimRec(AnimId::UXB_Toggle);
                field_10_anim.Set_Animation_Data_402A40(animRec.mFrameTableOffset, 0);
                field_10C_state = UXBState::eDeactivated_3;
                field_118_next_state_frame = gnFrameCount_507670 + 10;
            }
        }
        else
        {
            field_10C_state = UXBState::eDelay_0;
            field_8_update_delay = 6;
            const AnimRecord& animRec = AO::AnimRec(AnimId::UXB_Active);
            field_10_anim.Set_Animation_Data_402A40(animRec.mFrameTableOffset, 0);
            if (gMap.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
            {
                SFX_Play_43AD70(SoundEffect::RedTick_4, 35, 0);
            }
        }
    }
}


void UXB::VUpdate()
{
    VUpdate_489380();
}

void UXB::VUpdate_489380()
{
    switch (field_10C_state)
    {
        case UXBState::eDelay_0:
            if (IsColliding_489900())
            {
                field_10C_state = UXBState::eExploding_2;
                field_118_next_state_frame = gnFrameCount_507670 + 2;
            }
            else if (field_118_next_state_frame <= static_cast<s32>(gnFrameCount_507670))
            {
                field_10C_state = UXBState::eActive_1;
                const AnimRecord& rec = AO::AnimRec(AnimId::Bomb_Flash);
                field_11C_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, 0);
                field_118_next_state_frame = gnFrameCount_507670 + 2;
            }
            break;

        case UXBState::eActive_1:
            if (IsColliding_489900())
            {
                field_10C_state = UXBState::eExploding_2;
                field_118_next_state_frame = gnFrameCount_507670 + 2;
            }
            else if (field_118_next_state_frame <= static_cast<s32>(gnFrameCount_507670))
            {
                if (field_1BA_red_blink_count)
                {
                    field_1BA_red_blink_count--;
                    if (field_1BA_red_blink_count == 0)
                    {
                        field_11C_anim.LoadPal_403090(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 0, 0), 0);
                        field_1BC_flags.Clear(flags_1BC::eIsRed_Bit1);
                    }
                }
                else
                {
                    const FrameInfoHeader* pFrameInfo = field_11C_anim.Get_FrameHeader_403A00(-1);

                    const FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_11C_anim.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

                    field_11C_anim.LoadPal_403090(
                        field_11C_anim.field_20_ppBlock,
                        pFrameHeader->field_0_clut_offset);

                    field_1BC_flags.Set(flags_1BC::eIsRed_Bit1);

                    field_1B6_pattern_index++;

                    if (field_1B6_pattern_index >= field_1B4_pattern_length)
                    {
                        field_1B6_pattern_index = 0;
                    }

                    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
                    field_1BA_red_blink_count = (field_1B8_pattern / static_cast<s32>(pow(10, field_1B4_pattern_length - field_1B6_pattern_index - 1))) % 10;
                }

                const AnimRecord& rec = AO::AnimRec(AnimId::Bomb_RedGreenTick);
                field_11C_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, 0);
                
                if (field_1BC_flags.Get(flags_1BC::eIsRed_Bit1))
                {
                    if (gMap.Is_Point_In_Current_Camera_4449C0(
                            field_B2_lvl_number,
                            field_B0_path_number,
                            field_A8_xpos,
                            field_AC_ypos,
                            0))
                    {
                        SFX_Play_43AD70(SoundEffect::RedTick_4, 35, 0);
                    }
                }
                else if (gMap.Is_Point_In_Current_Camera_4449C0(
                             field_B2_lvl_number,
                             field_B0_path_number,
                             field_A8_xpos,
                             field_AC_ypos,
                             0))
                {
                    SFX_Play_43AD70(SoundEffect::GreenTick_3, 35, 0);
                }
                field_10C_state = UXBState::eDelay_0;
                field_118_next_state_frame = gnFrameCount_507670 + 10; // UXB change color delay
            }
            break;

        case UXBState::eExploding_2:
            if (static_cast<s32>(gnFrameCount_507670) >= field_118_next_state_frame)
            {
                ao_new<BaseBomb>(
                    field_A8_xpos,
                    field_AC_ypos,
                    0,
                    field_BC_sprite_scale);
                mFlags.Set(BaseGameObject::eDead);
            }
            break;
    }

    if (field_10C_state != UXBState::eExploding_2)
    {
        if (Event_Get_417250(kEventDeathReset_4))
        {
            if (field_10E_starting_state != UXBState::eDeactivated_3 || field_10C_state == UXBState::eDeactivated_3)
            {
                if (field_10E_starting_state != UXBState::eDelay_0 || field_10C_state != UXBState::eDeactivated_3)
                {
                    gMap.TLV_Reset_446870(field_114_tlvInfo, 0, 1u, 0);
                }
                else
                {
                    gMap.TLV_Reset_446870(field_114_tlvInfo, 1, 1u, 0);
                }
            }
            else
            {
                gMap.TLV_Reset_446870(field_114_tlvInfo, 1, 1u, 0);
            }
            mFlags.Set(BaseGameObject::eDead);
        }
    }
}

s16 UXB::IsColliding_489900()
{
    PSX_RECT uxbBound = {};
    VGetBoundingRect(&uxbBound, 1);

    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_10A_flags.Get(Flags_10A::e10A_Bit4_SetOffExplosives))
        {
            if (pObj->field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                PSX_RECT objBound = {};
                pObj->VGetBoundingRect(&objBound, 1);

                const s32 objX = FP_GetExponent(pObj->field_A8_xpos);
                const s32 objY = FP_GetExponent(pObj->field_AC_ypos);

                if (objX > uxbBound.x && objX < uxbBound.w && objY < uxbBound.h + 5 && uxbBound.x <= objBound.w && uxbBound.w >= objBound.x && uxbBound.h >= objBound.y && uxbBound.y <= objBound.h && pObj->field_BC_sprite_scale == field_BC_sprite_scale)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}


void UXB::VRender(PrimHeader** ppOt)
{
    VRender_4896C0(ppOt);
}

void UXB::VRender_4896C0(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
    {
        field_11C_anim.vRender(
            FP_GetExponent(field_A8_xpos
                           + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)
                           - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(field_AC_ypos
                           + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) - FP_NoFractional(field_BC_sprite_scale * FP_FromInteger(12)))
                           - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        field_11C_anim.Get_Frame_Rect_402B50(&rect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager_4FF7C8->field_2E_idx);

        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

} // namespace AO
