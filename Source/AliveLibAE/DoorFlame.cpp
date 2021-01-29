#include "stdafx.h"
#include "DoorFlame.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "ObjectIds.hpp"
#include "Sound/Midi.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "Sfx.hpp"

ALIVE_VAR(1, 0x5C2C6C, DoorFlame*, pFlameControllingTheSound_5C2C6C, nullptr);

class FireBackgroundGlow : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FireBackgroundGlow* ctor_45D890(FP xpos, FP ypos, FP scale)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x54592C);
        field_4_typeId = Types::eNone_0;
        
        const AnimRecord& rec = AnimRec(AnimId::Door_Flame_Glow);
        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

        field_DC_bApplyShadows &= ~1u;

        field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit20_use_xy_offset);
        
        field_B8_xpos = xpos;
        field_BC_ypos = ypos + FP_FromInteger(4);

        field_20_animation.field_C_render_layer = 12;
        field_20_animation.field_B_render_mode = 3;

        field_D0_r = 140;
        field_D2_g = 90;
        field_D4_b = 53;

        field_CC_sprite_scale = scale;
        
        Calc_Rect_45DA00();
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_45D9B0(flags);
    }

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_45DCD0(pOrderingTable);
    }

    EXPORT void Calc_Rect_45DA00()
    {
        PSX_Point xy = {};

        __int16 frameW = 0;
        __int16 frameH = 0;

        field_20_animation.Get_Frame_Width_Height_40C400(&frameW, &frameH);
        field_20_animation.Get_Frame_Offset_40C480(&xy.field_0_x, &xy.field_2_y);

        const auto& pCamPos = pScreenManager_5BB5F4->field_20_pCamPos;
        const FP screenX = field_B8_xpos - pCamPos->field_0_x;
        const FP screenY = field_BC_ypos - pCamPos->field_4_y;

        const FP frameWScaled = (FP_FromInteger(frameW) * field_CC_sprite_scale);
        const FP frameHScaled = (FP_FromInteger(frameH) * field_CC_sprite_scale);

        const FP offXScaled = (FP_FromInteger(xy.field_0_x) * field_CC_sprite_scale);
        const short offYScaled = FP_GetExponent((FP_FromInteger(xy.field_2_y) * field_CC_sprite_scale));

        // TODO: Refactor PSX <> PC width conversion
        const FP frameWScaled_converted = (((frameWScaled * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40));
        const short offXScaled_converted = FP_GetExponent(((offXScaled * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40));

        field_F4_xPos =  screenX + FP_FromInteger(offXScaled_converted) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
        field_F8_yPos =  screenY + FP_FromInteger(offYScaled) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
        field_FC_xOff =  screenX + FP_FromInteger(offXScaled_converted + FP_GetExponent(frameWScaled_converted)) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
        field_100_yOff = screenY + FP_FromInteger(offYScaled + FP_GetExponent(frameHScaled)) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
    }

    EXPORT void vRender_45DCD0(int **pOt)
    {
        if (Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
        {
            //if (k1_dword_55EF90)
            {
                field_20_animation.field_8_r = field_D0_r & 0xFF;
                field_20_animation.field_A_b = field_D4_b & 0xFF;
                field_20_animation.field_9_g = field_D2_g & 0xFF;

                const FP xOff = field_FC_xOff - field_F4_xPos;
                const FP yOff = field_100_yOff - field_F8_yPos;

                field_20_animation.vRender_40B820(
                    FP_GetExponent(field_F4_xPos),
                    FP_GetExponent(field_F8_yPos),
                    pOt,
                    FP_GetExponent(xOff) + 1,
                    FP_GetExponent(yOff) + 1);

                PSX_RECT frameRect = {};
                field_20_animation.Get_Frame_Rect_409E10(&frameRect);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h,
                    pScreenManager_5BB5F4->field_3A_idx);
            }
        }
    }

    EXPORT void dtor_45D9E0()
    {
        SetVTable(this, 0x54592C);
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT FireBackgroundGlow* vdtor_45D9B0(signed int flags)
    {
        dtor_45D9E0();
        if (flags & 1)
        {
            ae_delete_free_495540(this);
        }
        return this;
    }

private:
    FP field_F4_xPos;
    FP field_F8_yPos;
    FP field_FC_xOff;
    FP field_100_yOff;
};
ALIVE_ASSERT_SIZEOF(FireBackgroundGlow, 0x104);

struct FlameSpark
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_off_x;
    FP field_C_off_y;
    __int16 field_10_random64;
    __int16 field_12_bVisible;
    AnimationUnknown field_14;
};
ALIVE_ASSERT_SIZEOF(FlameSpark, 0x84);

// These flame sparks are extremely subtle and are easily missed!
class FlameSparks : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FlameSparks* ctor_45DE00(FP xpos, FP ypos)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        for (auto& anim : field_F8_sparks)
        {
            SetVTable(&anim.field_14, 0x5447CC);
        }

        SetVTable(this, 0x545974);
        field_4_typeId = Types::eNone_0;

        const AnimRecord& rec = AnimRec(AnimId::Door_Flame_Spark);
        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

        field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_DC_bApplyShadows |= 1u;
        field_20_animation.field_C_render_layer = 17;

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;
        field_410_xpos = xpos;
        field_414_ypos = ypos;

        field_CC_sprite_scale = FP_FromDouble(0.3);

        for (auto& anim : field_F8_sparks)
        {
            anim.field_14.field_68_anim_ptr = &field_20_animation;

            anim.field_14.field_4_flags.Set(AnimFlags::eBit3_Render);
            anim.field_14.field_4_flags.Set(AnimFlags::eBit16_bBlending);

            anim.field_14.field_C_render_layer = field_20_animation.field_C_render_layer + Math_RandomRange_496AB0(-1, 1);
            anim.field_14.field_6C_scale = field_CC_sprite_scale;

            anim.field_0_x = field_B8_xpos;
            anim.field_4_y = field_BC_ypos;

            anim.field_8_off_x = FP_FromInteger(0);
            anim.field_C_off_y = FP_FromInteger(0);

            anim.field_10_random64 = Math_RandomRange_496AB0(0, 64);
            anim.field_12_bVisible = 0;
        }

        field_F4_bRender = 0;
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_45DF90(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_45DFE0();
    }

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_45E260(pOrderingTable);
    }

    EXPORT void SetRenderEnabled_45E240(__int16 bEnable)
    {
        field_F4_bRender = bEnable;
    }

private:
    EXPORT void vUpdate_45DFE0()
    {
        // HACK/WTF this seems to move the base animation off screen so it can never been seen??
        PSX_RECT rect = {};
        gMap_5C3030.Get_Camera_World_Rect_481410(CameraPos::eCamCurrent_0, &rect);
        field_B8_xpos = FP_FromInteger(rect.w + 16);
        field_BC_ypos = FP_FromInteger(rect.y - 16);

        if (field_F4_bRender)
        {
            for (auto& anim : field_F8_sparks)
            {
                anim.field_10_random64--;
                if (anim.field_12_bVisible == 0)
                {
                    if (anim.field_10_random64 <= 0)
                    {
                        anim.field_12_bVisible = 1;
                        anim.field_10_random64 = Math_RandomRange_496AB0(7, 9);

                        anim.field_0_x = field_410_xpos;
                        anim.field_4_y = field_414_ypos;

                        anim.field_8_off_x = (FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(96));
                        anim.field_C_off_y = (FP_FromInteger(-Math_NextRandom()) / FP_FromInteger(96)); // TODO: Check this is right ??
                    }
                }
                else if (anim.field_10_random64 > 0)
                {
                    anim.field_0_x += anim.field_8_off_x;
                    anim.field_4_y += anim.field_C_off_y;

                    if (!(anim.field_10_random64 % 3))
                    {
                        anim.field_8_off_x += (FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64));
                        anim.field_C_off_y += (FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64));
                    }
                }
                else
                {
                    anim.field_12_bVisible = 0;
                    anim.field_10_random64 = Math_RandomRange_496AB0(90, 240);
                }
            }
        }
    }

    EXPORT void vRender_45E260(int **pOt)
    {
        if (sNum_CamSwappers_5C1B66 == 0)
        {
            if (field_F4_bRender)
            {
                field_20_animation.field_8_r = 240;
                field_20_animation.field_9_g = 32;
                field_20_animation.field_A_b = 32;

                field_20_animation.vRender_40B820(
                        FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                        FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                        pOt,
                        0,
                        0);

                PSX_RECT frameRect = {};
                field_20_animation.Get_Frame_Rect_409E10(&frameRect);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h,
                    pScreenManager_5BB5F4->field_3A_idx);

                for (auto& anim : field_F8_sparks)
                {
                    // Visible?
                    if (anim.field_12_bVisible)
                    {
                        // And in screen bounds?
                        if (anim.field_0_x >= pScreenManager_5BB5F4->field_20_pCamPos->field_0_x &&
                            anim.field_0_x <= pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + FP_FromInteger(368))
                        {
                            if (anim.field_4_y >= pScreenManager_5BB5F4->field_20_pCamPos->field_4_y && 
                                anim.field_4_y <= pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(240))
                            {
                                anim.field_14.vRender_40B820(
                                    FP_GetExponent(anim.field_0_x - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                                    FP_GetExponent(anim.field_4_y - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                                    pOt,
                                    0,
                                    0);

                                anim.field_14.GetRenderedSize_40C980(&frameRect);
                                pScreenManager_5BB5F4->InvalidateRect_40EC90(
                                    frameRect.x,
                                    frameRect.y,
                                    frameRect.w,
                                    frameRect.h,
                                    pScreenManager_5BB5F4->field_3A_idx);
                            }
                        }
                    }
                }
            }
        }
    }

    EXPORT void dtor_45DFC0()
    {
        SetVTable(this, 0x545974);
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT FlameSparks* vdtor_45DF90(signed int flags)
    {
        dtor_45DFC0();
        if (flags & 1)
        {
            ae_delete_free_495540(this);
        }
        return this;
    }


private:
    __int16 field_F4_bRender;
    //__int16 field_F6_pad;
    FlameSpark field_F8_sparks[6];
    FP field_410_xpos;
    FP field_414_ypos;
};
ALIVE_ASSERT_SIZEOF(FlameSparks, 0x418);

DoorFlame* DoorFlame::ctor_45E460(Path_DoorFlame* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5459BC);

    field_4_typeId = Types::eNone_0;
    field_F4_tlvInfo = tlvInfo;
    
    const AnimRecord& rec = AnimRec(AnimId::Door_Flame);
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    
    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_DC_bApplyShadows |= 1u;
    field_20_animation.field_C_render_layer = 17;
    field_FA_frame_count = field_20_animation.Get_Frame_Count_40AC70();
    field_20_animation.SetFrame_409D50(Math_RandomRange_496AB0(0, field_FA_frame_count - 1));
    
    field_F8_switch_id = pTlv->field_10_id;

    if (pTlv->field_12_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x)  + (FP_FromInteger(12) * field_CC_sprite_scale);
    field_108_fire_background_glow_id = -1;
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y) + (FP_FromInteger(15) * field_CC_sprite_scale);

    if (SwitchStates_Get_466020(field_F8_switch_id))
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        field_FC_state = States::eEnabled_1;
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_FC_state = States::eDisabled_0;
    }
    
    field_FE_2_random = Math_NextRandom() % 2;
    
    auto pFlameSparks = ae_new<FlameSparks>();
    if (pFlameSparks)
    {
        pFlameSparks->ctor_45DE00(field_B8_xpos, field_BC_ypos);
        field_10C_flame_sparks_id = pFlameSparks->field_8_object_id;
    }
    
    return this;
}

DoorFlame* DoorFlame::vdtor_45E690(signed int flags)
{
    dtor_45E6C0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void DoorFlame::dtor_45E6C0()
{
    SetVTable(this, 0x5459BC);

    BaseGameObject* pFireBackgroundGlow = sObjectIds_5C1B70.Find_449CF0(field_108_fire_background_glow_id);
    BaseGameObject* pFlameSparks = sObjectIds_5C1B70.Find_449CF0(field_10C_flame_sparks_id);
    
    if (pFireBackgroundGlow)
    {
        pFireBackgroundGlow->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_108_fire_background_glow_id = -1;
    }

    if (pFlameSparks)
    {
        pFlameSparks->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_10C_flame_sparks_id = -1;
    }

    vStopAudio_45E7E0();

    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void DoorFlame::vStopAudio_45E7E0()
{
    if (pFlameControllingTheSound_5C2C6C == this)
    {
        pFlameControllingTheSound_5C2C6C = nullptr;
        SND_Stop_Channels_Mask_4CA810(field_100_sounds_mask);
        field_100_sounds_mask = 0;
    }
}

void DoorFlame::vScreenChanged_45EA90()
{
    BaseGameObject* pFireBackgroundGlow = sObjectIds_5C1B70.Find_449CF0(field_108_fire_background_glow_id);
    BaseGameObject* pFlameSparks = sObjectIds_5C1B70.Find_449CF0(field_10C_flame_sparks_id);

    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    
    if (pFireBackgroundGlow)
    {
        pFireBackgroundGlow->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_108_fire_background_glow_id = -1;
    }

    if (pFlameSparks)
    {
        pFlameSparks->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_10C_flame_sparks_id = -1;
    }
}

void DoorFlame::vUpdate_45E830()
{
    auto pFireBackgroundGlow = static_cast<FireBackgroundGlow*>(sObjectIds_5C1B70.Find_449CF0(field_108_fire_background_glow_id));
    auto pFlameSparks = static_cast<FlameSparks*>(sObjectIds_5C1B70.Find_449CF0(field_10C_flame_sparks_id));

    switch (field_FC_state)
    {
    case States::eDisabled_0:
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

        if (pFlameSparks)
        {
            pFlameSparks->SetRenderEnabled_45E240(0);
        }

        if (SwitchStates_Get_466020(field_F8_switch_id))
        {
            field_FC_state = States::eEnabled_1;
        }

        if (pFireBackgroundGlow)
        {
            pFireBackgroundGlow->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            pFireBackgroundGlow = nullptr;
            field_108_fire_background_glow_id = -1;
        }
        break;

    case States::eEnabled_1:
        if (!pFlameControllingTheSound_5C2C6C)
        {
            pFlameControllingTheSound_5C2C6C = this;
            field_100_sounds_mask = SFX_Play_46FA90(SoundEffect::Fire_59, 40);
        }

        if (--field_FE_2_random <= 0)
        {
            field_FE_2_random = 2;
            if (pFireBackgroundGlow)
            {
                pFireBackgroundGlow->Calc_Rect_45DA00();
            }
        }

        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

        if (pFlameSparks)
        {
            pFlameSparks->SetRenderEnabled_45E240(1);
        }

        if (!SwitchStates_Get_466020(field_F8_switch_id))
        {
            field_FC_state = States::eDisabled_0;
        }

        if (!pFireBackgroundGlow)
        {
            pFireBackgroundGlow = ae_new<FireBackgroundGlow>();
            if (pFireBackgroundGlow)
            {
                pFireBackgroundGlow->ctor_45D890(
                    field_B8_xpos,
                    field_BC_ypos,
                    field_CC_sprite_scale);
                field_108_fire_background_glow_id = pFireBackgroundGlow->field_8_object_id;
            }
        }
        break;

    default:
        break;
    }

    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);

        if (pFireBackgroundGlow)
        {
            pFireBackgroundGlow->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_108_fire_background_glow_id = -1;
        }

        if (pFlameSparks)
        {
            pFlameSparks->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_10C_flame_sparks_id = -1;
        }
    }
}
