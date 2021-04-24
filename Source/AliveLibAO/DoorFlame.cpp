#include "stdafx_ao.h"
#include "Function.hpp"
#include "DoorFlame.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "ResourceManager.hpp"
#include "Midi.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "Sfx.hpp"
#include "ScreenManager.hpp"
#include "CameraSwapper.hpp"
#include "PsxDisplay.hpp"

namespace AO {

ALIVE_VAR(1, 0x507734, DoorFlame*, pFlameControllingTheSound_507734, nullptr);

class FireBackgroundGlow : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FireBackgroundGlow* ctor_431F20(FP xpos, FP ypos, FP scale)
    {
        ctor_417C10();
        SetVTable(this, 0x4BB330);
        field_4_typeId = Types::eNone_0;

        BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kGlowResID, 1, 0);
        Animation_Init_417FD0(1344, 52, 30, ppRes, 1);

        field_CC_bApplyShadows |= 1u;

        field_10_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_10_anim.field_4_flags.Set(AnimFlags::eBit20_use_xy_offset);

        field_A8_xpos = xpos;
        field_AC_ypos = ypos + FP_FromInteger(4);

        field_10_anim.field_C_layer = Layer::eLayer_12;
        field_10_anim.field_B_render_mode = TPageAbr::eBlend_3;

        field_10_anim.field_8_r = 100;
        field_10_anim.field_9_g = 100;
        field_10_anim.field_A_b = 63;

        field_BC_sprite_scale = scale;

        Calc_Rect_432010();
        return this;
    }

    EXPORT BaseGameObject* dtor_431FF0()
    {
        SetVTable(this, 0x4BB330);
        return dtor_417D10();
    }

    virtual BaseGameObject* VDestructor(s32 flags) override
    {
        return Vdtor_432DB0(flags);
    }

    EXPORT FireBackgroundGlow* Vdtor_432DB0(s32 flags)
    {
        dtor_431FF0();
        if (flags & 1)
        {
            ao_delete_free_447540(this);
        }
        return this;
    }

    virtual void VUpdate() override
    {
        // Empty
    }

    virtual void VRender(PrimHeader** ppOt) override
    {
        VRender_432210(ppOt);
    }

    EXPORT void VRender_432210(PrimHeader** ppOt)
    {
        if (Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
        {
            field_10_anim.field_8_r = static_cast<BYTE>(field_C0_r);
            field_10_anim.field_9_g = static_cast<BYTE>(field_C2_g);
            field_10_anim.field_A_b = static_cast<BYTE>(field_C4_b);

            field_10_anim.VRender_403AE0(
                FP_GetExponent(field_E4_xPos),
                FP_GetExponent(field_E8_yPos),
                ppOt,
                FP_GetExponent(field_EC_xOff - field_E4_xPos) + 1,
                FP_GetExponent(field_F0_yOff - field_E8_yPos) + 1);

            PSX_RECT rect = {};
            field_10_anim.Get_Frame_Rect_402B50(&rect);
            pScreenManager_4FF7C8->InvalidateRect_406E40(
                rect.x,
                rect.y,
                rect.w,
                rect.h,
                pScreenManager_4FF7C8->field_2E_idx);
        }
    }

    EXPORT void Calc_Rect_432010()
    {
        PSX_Point xy = {};

        s16 frameW = 0;
        s16 frameH = 0;

        field_10_anim.Get_Frame_Width_Height_403E80(&frameW, &frameH);
        field_10_anim.Get_Frame_Offset_403EE0(&xy.field_0_x, &xy.field_2_y);

        const auto& pCamPos = pScreenManager_4FF7C8->field_10_pCamPos;
        const FP screenX = FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) + field_A8_xpos - pCamPos->field_0_x;
        const FP screenY = FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) + field_AC_ypos - pCamPos->field_4_y;

        const FP frameWScaled = (FP_FromInteger(frameW) * field_BC_sprite_scale);
        const FP frameHScaled = (FP_FromInteger(frameH) * field_BC_sprite_scale);

        const int offXScaled = FP_GetExponent(FP_FromInteger(xy.field_0_x) * field_BC_sprite_scale);
        const int offYScaled = FP_GetExponent(FP_FromInteger(xy.field_2_y) * field_BC_sprite_scale);
        
         // TODO: Refactor PSX <> PC width conversion
        const FP frameWScaled_converted = ((frameWScaled * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40); 
        // Why isn't this converted ??
        //const short offXScaled_converted = FP_GetExponent(((FP_FromInteger(offXScaled) * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40));

     
        field_E4_xPos = screenX + FP_FromInteger(offXScaled) + FP_FromInteger(Math_NextRandom() % 3);
        field_E8_yPos = screenY + FP_FromInteger(offYScaled) + FP_FromInteger((Math_NextRandom() % 3));
        field_EC_xOff = screenX + FP_FromInteger(offXScaled) + frameWScaled_converted + FP_FromInteger(Math_NextRandom() % 3);
        field_F0_yOff = screenY + FP_FromInteger(offYScaled) + frameHScaled + FP_FromInteger(Math_NextRandom() % 3);
    }

    int field_D4[4];

    FP field_E4_xPos;
    FP field_E8_yPos;
    FP field_EC_xOff;
    FP field_F0_yOff;
};
ALIVE_ASSERT_SIZEOF(FireBackgroundGlow, 0xF4);

struct FlameSpark
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_off_x;
    FP field_C_off_y;
    s16 field_10_random64;
    s16 field_12_bVisible;
    AnimationUnknown field_14;
};
ALIVE_ASSERT_SIZEOF(FlameSpark, 0x84);

class FlameSparks : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FlameSparks* ctor_4322F0(FP xpos, FP ypos)
    {
        ctor_417C10();
        for (auto& anim : field_E8_sparks)
        {
             SetVTable(&anim.field_14, 0x4BA470);
        }

        SetVTable(this, 0x4BB368);
        field_4_typeId = Types::eNone_0;
        BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kOmmflareResID, 1, 0);
        Animation_Init_417FD0(1532, 38, 21, ppRes, 1);
        field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_CC_bApplyShadows |= 1u;
        field_10_anim.field_C_layer = Layer::eLayer_17;

        field_A8_xpos = xpos;
        field_AC_ypos = ypos;
        field_400_xpos = xpos;
        field_404_ypos = ypos;

        field_BC_sprite_scale = FP_FromDouble(0.3);

        for (auto& anim : field_E8_sparks)
        {
            anim.field_14.field_68_anim_ptr = &field_10_anim;

            anim.field_14.field_4_flags.Set(AnimFlags::eBit3_Render);
            anim.field_14.field_4_flags.Set(AnimFlags::eBit16_bBlending);

            const short rndLayer = static_cast<short>(field_10_anim.field_C_layer) + Math_RandomRange_450F20(-1, 1);
            anim.field_14.field_C_layer = static_cast<Layer>(rndLayer);
            anim.field_14.field_6C_scale = field_BC_sprite_scale;

            anim.field_0_x = field_A8_xpos;
            anim.field_4_y = field_AC_ypos;

            anim.field_8_off_x = FP_FromInteger(0);
            anim.field_C_off_y = FP_FromInteger(0);

            anim.field_10_random64 =Math_RandomRange_450F20(0, 64);
            anim.field_12_bVisible = 0;
        }

        field_E4_bRender = 0;
        return this;
    }

    EXPORT BaseGameObject* dtor_432430()
    {
        SetVTable(this, 0x4BB368);
        return dtor_417D10();
    }

    virtual BaseGameObject* VDestructor(s32 flags) override
    {
        return Vdtor_432DD0(flags);
    }

    EXPORT FlameSparks* Vdtor_432DD0(s32 flags)
    {
        dtor_432430();
        if (flags & 1)
        {
            ao_delete_free_447540(this);
        }
        return this;
    }

    virtual void VUpdate() override
    {
        VUpdate_432440();
    }

    EXPORT void VUpdate_432440()
    {
        PSX_RECT rect = {};
        gMap_507BA8.Get_Camera_World_Rect_444C30(CameraPos::eCamCurrent_0, &rect);
        field_A8_xpos = FP_FromInteger(rect.w + 16);
        field_AC_ypos = FP_FromInteger(rect.y - 16);
        if (field_E4_bRender)
        {
            for (auto& anim : field_E8_sparks)
            {
                anim.field_10_random64--;
                if (anim.field_12_bVisible == 0)
                {
                    if (anim.field_10_random64 <= 0)
                    {
                        anim.field_12_bVisible = 1;
                        anim.field_10_random64 = Math_RandomRange_450F20(7, 9);

                        anim.field_0_x = field_400_xpos;
                        anim.field_4_y = field_404_ypos;

                        anim.field_8_off_x = FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(96);
                        anim.field_C_off_y = FP_FromInteger(-Math_NextRandom()) / FP_FromInteger(96);
                    }
                }
                else if (anim.field_10_random64 > 0)
                {
                    anim.field_0_x += anim.field_8_off_x;
                    anim.field_4_y += anim.field_C_off_y;

                    if (!(anim.field_10_random64 % 3))
                    {
                        anim.field_8_off_x += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
                        anim.field_C_off_y += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
                    }
                }
                else
                {
                    anim.field_12_bVisible = 0;
                    anim.field_10_random64 = Math_RandomRange_450F20(90, 240);
                }
            }
        }
    }

    virtual void VRender(PrimHeader** ppOt) override
    {
        VRender_432640(ppOt);
    }

    EXPORT void VRender_432640(PrimHeader** ppOt)
    {
        if (sNumCamSwappers_507668 == 0)
        {
            if (field_E4_bRender)
            {
                field_10_anim.field_9_g = 32;
                field_10_anim.field_A_b = 32;
                field_10_anim.field_8_r = 240;

                const FP_Point* pCamPos = pScreenManager_4FF7C8->field_10_pCamPos;

                const FP screen_left = pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos);
                const FP screen_right = pCamPos->field_0_x + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos);
                const FP screen_top = pCamPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos);
                const FP screen_bottom = pCamPos->field_4_y + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos);

                field_10_anim.VRender_403AE0(
                    FP_GetExponent(PsxToPCX(field_A8_xpos - screen_left)),
                    FP_GetExponent(field_AC_ypos - screen_top),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                field_10_anim.Get_Frame_Rect_402B50(&frameRect);
                pScreenManager_4FF7C8->InvalidateRect_406E40(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h,
                    pScreenManager_4FF7C8->field_2E_idx);

                for (auto& anim : field_E8_sparks)
                {
                    if (anim.field_12_bVisible)
                    {
                        if (anim.field_0_x >= screen_left && anim.field_0_x <= screen_right)
                        {
                            if (anim.field_4_y >= screen_top && anim.field_4_y <= screen_bottom)
                            {
                                anim.field_14.VRender2(
                                    FP_GetExponent(PsxToPCX(anim.field_0_x - screen_left)),
                                    FP_GetExponent(anim.field_4_y - screen_top),
                                    ppOt);

                               anim.field_14.GetRenderedSize_404220(&frameRect);
                                pScreenManager_4FF7C8->InvalidateRect_406E40(
                                    frameRect.x,
                                    frameRect.y,
                                    frameRect.w,
                                    frameRect.h,
                                    pScreenManager_4FF7C8->field_2E_idx);
                            }
                        }
                    }
                }
            }
        }

    }

    int field_D4_padding[4];
    s16 field_E4_bRender;
    s16 field_E6_padding;
    FlameSpark field_E8_sparks[6];
    FP field_400_xpos;
    FP field_404_ypos;
};
ALIVE_ASSERT_SIZEOF(FlameSparks, 0x408);

void DoorFlame::VUpdate()
{
    VUpdate_432BA0();
}

DoorFlame* DoorFlame::Vdtor_432DF0(s32 flags)
{
    dtor_432AA0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* DoorFlame::VDestructor(s32 flags)
{
    return Vdtor_432DF0(flags);
}

void DoorFlame::VStopAudio_432B60()
{
    if (pFlameControllingTheSound_507734 == this)
    {
        pFlameControllingTheSound_507734 = nullptr;
        SND_Stop_Channels_Mask_4774A0(field_F0_sounds_mask);
    }
}

void DoorFlame::VStopAudio()
{
    VStopAudio_432B60();
}

BaseGameObject* DoorFlame::dtor_432AA0()
{
    SetVTable(this, 0x4BB3A0);

    if (field_F8_pFireBackgroundGlow)
    {
        field_F8_pFireBackgroundGlow->field_C_refCount--;
        field_F8_pFireBackgroundGlow->field_6_flags.Set(Options::eDead_Bit3);
        field_F8_pFireBackgroundGlow = nullptr;
    }

    if (field_FC_pFlameSparks)
    {
        field_FC_pFlameSparks->field_C_refCount--;
        field_FC_pFlameSparks->field_6_flags.Set(Options::eDead_Bit3);
        field_FC_pFlameSparks = nullptr;
    }

    VStopAudio_432B60();

    gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

DoorFlame* DoorFlame::ctor_432860(Path_DoorFlame* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BB3A0);
    field_4_typeId = Types::eNone_0;
    field_E4_tlvInfo = tlvInfo;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kHubFireResID, 1, 0);
    Animation_Init_417FD0(5072, 51, 24, ppRes, 1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_CC_bApplyShadows |= 1u;
    field_10_anim.field_C_layer = Layer::eLayer_17;
    field_EA_frame_count = field_10_anim.Get_Frame_Count_403540();
    field_E8_switch_id = pTlv->field_18_id;

    if (pTlv->field_1A_scale == 1 || pTlv->field_1A_scale == 2)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
        field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 15);
    }
    else if (pTlv->field_1A_scale == 0)
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
        field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 15);
    }

    switch (pTlv->field_1C_colour)
    {
    case Path_DoorFlame::Colour::red_1:
        field_C0_r = 127;
        break;
    case Path_DoorFlame::Colour::green_2:
        field_C2_g = 127;
        break;
    case Path_DoorFlame::Colour::blue_3:
        field_C4_b = 127;
        break;
    default:
        break;
    }

    field_F8_pFireBackgroundGlow = 0;

    if (SwitchStates_Get(pTlv->field_18_id))
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
        field_EC_state = States::eEnabled_1;
    }
    else
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_EC_state = States::eDisabled_0;
    }

    field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_EE_2_random = Math_NextRandom() & 1;

    field_FC_pFlameSparks = ao_new<FlameSparks>();
    if (field_FC_pFlameSparks)
    {
        field_FC_pFlameSparks->ctor_4322F0(field_A8_xpos, field_AC_ypos);
        field_FC_pFlameSparks->field_C_refCount++;
    }
    return this;
}

void DoorFlame::VUpdate_432BA0()
{
    switch (field_EC_state)
    {
    case States::eDisabled_0:
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        if (field_FC_pFlameSparks)
        {
            field_FC_pFlameSparks->field_E4_bRender = 0;
        }

        if (SwitchStates_Get(field_E8_switch_id))
        {
            field_EC_state = States::eEnabled_1;
        }

        if (field_F8_pFireBackgroundGlow)
        {
            field_F8_pFireBackgroundGlow->field_C_refCount--;
            field_F8_pFireBackgroundGlow->field_6_flags.Set(Options::eDead_Bit3);
            field_F8_pFireBackgroundGlow = nullptr;
        }
        break;

    case States::eEnabled_1:
        if (!pFlameControllingTheSound_507734)
        {
            pFlameControllingTheSound_507734 = this;
            field_F0_sounds_mask = SFX_Play_43AD70(SoundEffect::Fire_69, 40, 0);
        }

        if (--field_EE_2_random <= 0)
        {
            field_EE_2_random = 2;
            if (field_F8_pFireBackgroundGlow)
            {
                field_F8_pFireBackgroundGlow->Calc_Rect_432010();
            }
        }

        field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
        if (field_FC_pFlameSparks)
        {
            field_FC_pFlameSparks->field_E4_bRender = 1;
        }

        if (!SwitchStates_Get(field_E8_switch_id))
        {
            field_EC_state = States::eDisabled_0;
        }

        if (!field_F8_pFireBackgroundGlow)
        {
            field_F8_pFireBackgroundGlow = ao_new<FireBackgroundGlow>();
            if (field_F8_pFireBackgroundGlow)
            {
                field_F8_pFireBackgroundGlow->ctor_431F20(
                    field_A8_xpos,
                    field_AC_ypos + FP_FromInteger(4),
                    FP_FromDouble(0.5));

                field_F8_pFireBackgroundGlow->field_C_refCount++;
                field_F8_pFireBackgroundGlow->field_C0_r = field_C0_r;
                field_F8_pFireBackgroundGlow->field_C2_g = field_C2_g;
                field_F8_pFireBackgroundGlow->field_C4_b = field_C4_b;
            }

        }
        break;

    default:
        break;
    }

    if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

}

