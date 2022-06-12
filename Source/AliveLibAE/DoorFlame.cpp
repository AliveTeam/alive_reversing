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

class FireBackgroundGlow final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    FireBackgroundGlow(FP xpos, FP ypos, FP scale)
        : BaseAnimatedWithPhysicsGameObject(0)
    {
        SetType(ReliveTypes::eNone);

        const AnimRecord& rec = AnimRec(AnimId::Door_FireBackgroundGlow);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

        mApplyShadows &= ~1u;

        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit20_use_xy_offset);

        mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
        mBaseAnimatedWithPhysicsGameObject_YPos = ypos + FP_FromInteger(4);

        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_FallingItemPortalClip_Half_12;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_3;

        mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(140, 90, 53);

        mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

        Calc_Rect_45DA00();
    }

    virtual void VRender(PrimHeader** ppOt) override
    {
        vRender_45DCD0(ppOt);
    }

    void Calc_Rect_45DA00()
    {
        PSX_Point xy = {};

        s16 frameW = 0;
        s16 frameH = 0;

        mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Width_Height(&frameW, &frameH);
        mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Offset(&xy.field_0_x, &xy.field_2_y);

        const FP screenX = mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos();
        const FP screenY = mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos();

        const FP frameWScaled = (FP_FromInteger(frameW) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        const FP frameHScaled = (FP_FromInteger(frameH) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        const FP offXScaled = (FP_FromInteger(xy.field_0_x) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        const s16 offYScaled = FP_GetExponent((FP_FromInteger(xy.field_2_y) * mBaseAnimatedWithPhysicsGameObject_SpriteScale));

        // TODO: Refactor PSX <> PC width conversion
        const FP frameWScaled_converted = (((frameWScaled * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40));
        const s16 offXScaled_converted = FP_GetExponent(((offXScaled * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40));

        field_F4_xPos = screenX + FP_FromInteger(offXScaled_converted) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
        field_F8_yPos = screenY + FP_FromInteger(offYScaled) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
        field_FC_xOff = screenX + FP_FromInteger(offXScaled_converted + FP_GetExponent(frameWScaled_converted)) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
        field_100_yOff = screenY + FP_FromInteger(offYScaled + FP_GetExponent(frameHScaled)) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
    }

    void vRender_45DCD0(PrimHeader** ppOt)
    {
        if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            //if (k1_dword_55EF90)
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mRed = mBaseAnimatedWithPhysicsGameObject_RGB.r & 0xFF;
                mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = mBaseAnimatedWithPhysicsGameObject_RGB.b & 0xFF;
                mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = mBaseAnimatedWithPhysicsGameObject_RGB.g & 0xFF;

                const FP xOff = field_FC_xOff - field_F4_xPos;
                const FP yOff = field_100_yOff - field_F8_yPos;

                mBaseAnimatedWithPhysicsGameObject_Anim.VRender(
                    FP_GetExponent(field_F4_xPos),
                    FP_GetExponent(field_F8_yPos),
                    ppOt,
                    FP_GetExponent(xOff) + 1,
                    FP_GetExponent(yOff) + 1);

                PSX_RECT frameRect = {};
                mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Rect(&frameRect);
                pScreenManager->InvalidateRectCurrentIdx(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h);
            }
        }
    }

private:
    FP field_F4_xPos;
    FP field_F8_yPos;
    FP field_FC_xOff;
    FP field_100_yOff;
};
ALIVE_ASSERT_SIZEOF(FireBackgroundGlow, 0x104);

struct FlameSpark final
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

// These flame sparks are extremely subtle and are easily missed!
class FlameSparks final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    FlameSparks(FP xpos, FP ypos)
        : BaseAnimatedWithPhysicsGameObject(0)
    {
        SetType(ReliveTypes::eNone);

        const AnimRecord& rec = AnimRec(AnimId::Zap_Sparks);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

        mApplyShadows |= 1u;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;

        mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
        mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
        field_410_xpos = xpos;
        field_414_ypos = ypos;

        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.3);

        for (auto& anim : field_F8_sparks)
        {
            anim.field_14.field_68_anim_ptr = &mBaseAnimatedWithPhysicsGameObject_Anim;

            anim.field_14.mAnimFlags.Set(AnimFlags::eBit3_Render);
            anim.field_14.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

            // TODO: clean this up
            const s32 rndLayer = static_cast<s32>(mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer) + Math_RandomRange(-1, 1);
            anim.field_14.mRenderLayer = static_cast<Layer>(rndLayer);
            anim.field_14.field_6C_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

            anim.field_0_x = mBaseAnimatedWithPhysicsGameObject_XPos;
            anim.field_4_y = mBaseAnimatedWithPhysicsGameObject_YPos;

            anim.field_8_off_x = FP_FromInteger(0);
            anim.field_C_off_y = FP_FromInteger(0);

            anim.field_10_random64 = Math_RandomRange(0, 64);
            anim.field_12_bVisible = 0;
        }

        field_F4_bRender = 0;
    }

    virtual void VUpdate() override
    {
        vUpdate_45DFE0();
    }

    virtual void VRender(PrimHeader** ppOt) override
    {
        vRender_45E260(ppOt);
    }

    void SetRenderEnabled_45E240(s16 bEnable)
    {
        field_F4_bRender = bEnable;
    }

private:
    void vUpdate_45DFE0()
    {
        // HACK/WTF this seems to move the base animation off screen so it can never been seen??
        PSX_RECT rect = {};
        gMap.Get_Camera_World_Rect(CameraPos::eCamCurrent_0, &rect);
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(rect.w + 16);
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(rect.y - 16);

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
                        anim.field_10_random64 = Math_RandomRange(7, 9);

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
                    anim.field_10_random64 = Math_RandomRange(90, 240);
                }
            }
        }
    }

    void vRender_45E260(PrimHeader** ppOt)
    {
        if (sNum_CamSwappers_5C1B66 == 0)
        {
            if (field_F4_bRender)
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mRed = 240;
                mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = 32;
                mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = 32;

                mBaseAnimatedWithPhysicsGameObject_Anim.VRender(
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos()),
                    FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos()),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Rect(&frameRect);
                pScreenManager->InvalidateRectCurrentIdx(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h);

                for (auto& anim : field_F8_sparks)
                {
                    // Visible?
                    if (anim.field_12_bVisible)
                    {
                        // And in screen bounds?
                        if (anim.field_0_x >= pScreenManager->CamXPos() && anim.field_0_x <= pScreenManager->CamXPos() + FP_FromInteger(368))
                        {
                            if (anim.field_4_y >= pScreenManager->CamYPos() && anim.field_4_y <= pScreenManager->CamYPos() + FP_FromInteger(240))
                            {
                                anim.field_14.VRender(
                                    FP_GetExponent(anim.field_0_x - pScreenManager->CamXPos()),
                                    FP_GetExponent(anim.field_4_y - pScreenManager->CamYPos()),
                                    ppOt,
                                    0,
                                    0);

                                anim.field_14.GetRenderedSize(&frameRect);
                                pScreenManager->InvalidateRectCurrentIdx(
                                    frameRect.x,
                                    frameRect.y,
                                    frameRect.w,
                                    frameRect.h);
                            }
                        }
                    }
                }
            }
        }
    }

private:
    s16 field_F4_bRender;
    FlameSpark field_F8_sparks[6];
    FP field_410_xpos;
    FP field_414_ypos;
};
ALIVE_ASSERT_SIZEOF(FlameSparks, 0x418);

DoorFlame::DoorFlame(Path_DoorFlame* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eNone);
    field_F4_tlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::Fire);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mApplyShadows |= 1u;
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    field_FA_frame_count = mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Count();
    mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(Math_RandomRange(0, field_FA_frame_count - 1));

    field_F8_switch_id = pTlv->field_10_switch_id;

    if (pTlv->field_12_scale != Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x) + (FP_FromInteger(12) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    field_108_fire_background_glow_id = -1;
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y) + (FP_FromInteger(15) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    if (SwitchStates_Get(field_F8_switch_id))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
        field_FC_state = States::eEnabled_1;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        field_FC_state = States::eDisabled_0;
    }

    field_FE_2_random = Math_NextRandom() % 2;

    auto pFlameSparks = relive_new FlameSparks(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
    if (pFlameSparks)
    {
        field_10C_flame_sparks_id = pFlameSparks->field_8_object_id;
    }
}

DoorFlame::~DoorFlame()
{
    BaseGameObject* pFireBackgroundGlow = sObjectIds.Find_Impl(field_108_fire_background_glow_id);
    BaseGameObject* pFlameSparks = sObjectIds.Find_Impl(field_10C_flame_sparks_id);

    if (pFireBackgroundGlow)
    {
        pFireBackgroundGlow->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_108_fire_background_glow_id = -1;
    }

    if (pFlameSparks)
    {
        pFlameSparks->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_10C_flame_sparks_id = -1;
    }

    VStopAudio();

    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
}

void DoorFlame::VStopAudio()
{
    if (pFlameControllingTheSound_5C2C6C == this)
    {
        pFlameControllingTheSound_5C2C6C = nullptr;
        SND_Stop_Channels_Mask(field_100_sounds_mask);
        field_100_sounds_mask = 0;
    }
}

void DoorFlame::VScreenChanged()
{
    BaseGameObject* pFireBackgroundGlow = sObjectIds.Find_Impl(field_108_fire_background_glow_id);
    BaseGameObject* pFlameSparks = sObjectIds.Find_Impl(field_10C_flame_sparks_id);

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

    if (pFireBackgroundGlow)
    {
        pFireBackgroundGlow->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_108_fire_background_glow_id = -1;
    }

    if (pFlameSparks)
    {
        pFlameSparks->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_10C_flame_sparks_id = -1;
    }
}

void DoorFlame::VUpdate()
{
    auto pFireBackgroundGlow = static_cast<FireBackgroundGlow*>(sObjectIds.Find_Impl(field_108_fire_background_glow_id));
    auto pFlameSparks = static_cast<FlameSparks*>(sObjectIds.Find_Impl(field_10C_flame_sparks_id));

    switch (field_FC_state)
    {
        case States::eDisabled_0:
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

            if (pFlameSparks)
            {
                pFlameSparks->SetRenderEnabled_45E240(0);
            }

            if (SwitchStates_Get(field_F8_switch_id))
            {
                field_FC_state = States::eEnabled_1;
            }

            if (pFireBackgroundGlow)
            {
                pFireBackgroundGlow->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                pFireBackgroundGlow = nullptr;
                field_108_fire_background_glow_id = -1;
            }
            break;

        case States::eEnabled_1:
            if (!pFlameControllingTheSound_5C2C6C)
            {
                pFlameControllingTheSound_5C2C6C = this;
                field_100_sounds_mask = SFX_Play_Mono(SoundEffect::Fire_59, 40);
            }

            if (--field_FE_2_random <= 0)
            {
                field_FE_2_random = 2;
                if (pFireBackgroundGlow)
                {
                    pFireBackgroundGlow->Calc_Rect_45DA00();
                }
            }

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

            if (pFlameSparks)
            {
                pFlameSparks->SetRenderEnabled_45E240(1);
            }

            if (!SwitchStates_Get(field_F8_switch_id))
            {
                field_FC_state = States::eDisabled_0;
            }

            if (!pFireBackgroundGlow)
            {
                pFireBackgroundGlow = relive_new FireBackgroundGlow(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                                 mBaseAnimatedWithPhysicsGameObject_YPos,
                                                                 mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                if (pFireBackgroundGlow)
                {
                    field_108_fire_background_glow_id = pFireBackgroundGlow->field_8_object_id;
                }
            }
            break;

        default:
            break;
    }

    if (!gMap.Is_Point_In_Current_Camera_4810D0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);

        if (pFireBackgroundGlow)
        {
            pFireBackgroundGlow->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            field_108_fire_background_glow_id = -1;
        }

        if (pFlameSparks)
        {
            pFlameSparks->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            field_10C_flame_sparks_id = -1;
        }
    }
}
