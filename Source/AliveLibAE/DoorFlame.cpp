#include "stdafx.h"
#include "DoorFlame.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Sound/Midi.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Sfx.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

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
        Animation_Init(AnimId::Door_FireBackgroundGlow, ppRes);

        mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

        mAnim.mFlags.Clear(AnimFlags::eBit16_bBlending);
        mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
        mAnim.mFlags.Set(AnimFlags::eBit20_use_xy_offset);

        mXPos = xpos;
        mYPos = ypos + FP_FromInteger(4);

        mAnim.mRenderLayer = Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12;
        mAnim.mRenderMode = TPageAbr::eBlend_3;

        mRGB.SetRGB(140, 90, 53);

        mSpriteScale = scale;

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

        mAnim.Get_Frame_Width_Height(&frameW, &frameH);
        mAnim.Get_Frame_Offset(&xy.x, &xy.y);

        const FP screenX = mXPos - pScreenManager->CamXPos();
        const FP screenY = mYPos - pScreenManager->CamYPos();

        const FP frameWScaled = (FP_FromInteger(frameW) * mSpriteScale);
        const FP frameHScaled = (FP_FromInteger(frameH) * mSpriteScale);

        const FP offXScaled = (FP_FromInteger(xy.x) * mSpriteScale);
        const s16 offYScaled = FP_GetExponent((FP_FromInteger(xy.y) * mSpriteScale));

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
                mAnim.mRed = mRGB.r & 0xFF;
                mAnim.mBlue = mRGB.b & 0xFF;
                mAnim.mGreen = mRGB.g & 0xFF;

                const FP xOff = field_FC_xOff - field_F4_xPos;
                const FP yOff = field_100_yOff - field_F8_yPos;

                mAnim.VRender(
                    FP_GetExponent(field_F4_xPos),
                    FP_GetExponent(field_F8_yPos),
                    ppOt,
                    FP_GetExponent(xOff) + 1,
                    FP_GetExponent(yOff) + 1);

                PSX_RECT frameRect = {};
                mAnim.Get_Frame_Rect(&frameRect);
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
    FP x;
    FP y;
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

        const AnimRecord& rec = AnimRec(AnimId::ChantOrb_Particle_Small);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init(AnimId::ChantOrb_Particle_Small, ppRes);

        mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

        mVisualFlags.Set(VisualFlags::eApplyShadowZoneColour);
        mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;

        mXPos = xpos;
        mYPos = ypos;
        field_410_xpos = xpos;
        field_414_ypos = ypos;

        mSpriteScale = FP_FromDouble(0.3);

        for (auto& anim : field_F8_sparks)
        {
            anim.field_14.field_68_anim_ptr = &mAnim;

            anim.field_14.mFlags.Set(AnimFlags::eBit3_Render);
            anim.field_14.mFlags.Set(AnimFlags::eBit16_bBlending);

            // TODO: clean this up
            const s32 rndLayer = static_cast<s32>(mAnim.mRenderLayer) + Math_RandomRange(-1, 1);
            anim.field_14.mRenderLayer = static_cast<Layer>(rndLayer);
            anim.field_14.field_6C_scale = mSpriteScale;

            anim.x = mXPos;
            anim.y = mYPos;

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
        mXPos = FP_FromInteger(rect.w + 16);
        mYPos = FP_FromInteger(rect.y - 16);

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

                        anim.x = field_410_xpos;
                        anim.y = field_414_ypos;

                        anim.field_8_off_x = (FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(96));
                        anim.field_C_off_y = (FP_FromInteger(-Math_NextRandom()) / FP_FromInteger(96)); // TODO: Check this is right ??
                    }
                }
                else if (anim.field_10_random64 > 0)
                {
                    anim.x += anim.field_8_off_x;
                    anim.y += anim.field_C_off_y;

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
                mAnim.mRed = 240;
                mAnim.mGreen = 32;
                mAnim.mBlue = 32;

                mAnim.VRender(
                    FP_GetExponent(mXPos - pScreenManager->CamXPos()),
                    FP_GetExponent(mYPos - pScreenManager->CamYPos()),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                mAnim.Get_Frame_Rect(&frameRect);
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
                        if (anim.x >= pScreenManager->CamXPos() && anim.x <= pScreenManager->CamXPos() + FP_FromInteger(368))
                        {
                            if (anim.y >= pScreenManager->CamYPos() && anim.y <= pScreenManager->CamYPos() + FP_FromInteger(240))
                            {
                                anim.field_14.VRender(
                                    FP_GetExponent(anim.x - pScreenManager->CamXPos()),
                                    FP_GetExponent(anim.y - pScreenManager->CamYPos()),
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

DoorFlame::DoorFlame(relive::Path_DoorFlame* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eNone);
    mTlvInfo = tlvId;

    const AnimRecord& rec = AnimRec(AnimId::Fire);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Fire, ppRes);

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mVisualFlags.Set(VisualFlags::eApplyShadowZoneColour);
    mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    mFrameCount = mAnim.Get_Frame_Count();
    mAnim.SetFrame(Math_RandomRange(0, mFrameCount - 1));

    mSwitchId = pTlv->mSwitchId;

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX) + (FP_FromInteger(12) * mSpriteScale);
    mFireBackgroundGlowId = Guid{};
    mYPos = FP_FromInteger(pTlv->mTopLeftY) + (FP_FromInteger(15) * mSpriteScale);

    if (SwitchStates_Get(mSwitchId))
    {
        mAnim.mFlags.Set(AnimFlags::eBit3_Render);
        mState = States::eEnabled_1;
    }
    else
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        mState = States::eDisabled_0;
    }

    mRandom = Math_NextRandom() % 2;

    auto pFlameSparks = relive_new FlameSparks(mXPos, mYPos);
    if (pFlameSparks)
    {
        mFlameSparksId = pFlameSparks->mBaseGameObjectId;
    }
}

DoorFlame::~DoorFlame()
{
    BaseGameObject* pFireBackgroundGlow = sObjectIds.Find_Impl(mFireBackgroundGlowId);
    BaseGameObject* pFlameSparks = sObjectIds.Find_Impl(mFlameSparksId);

    if (pFireBackgroundGlow)
    {
        pFireBackgroundGlow->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        mFireBackgroundGlowId = Guid{};
    }

    if (pFlameSparks)
    {
        pFlameSparks->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        mFlameSparksId = Guid{};
    }

    VStopAudio();

    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void DoorFlame::VStopAudio()
{
    if (pFlameControllingTheSound_5C2C6C == this)
    {
        pFlameControllingTheSound_5C2C6C = nullptr;
        SND_Stop_Channels_Mask(mSoundsMask);
        mSoundsMask = 0;
    }
}

void DoorFlame::VScreenChanged()
{
    BaseGameObject* pFireBackgroundGlow = sObjectIds.Find_Impl(mFireBackgroundGlowId);
    BaseGameObject* pFlameSparks = sObjectIds.Find_Impl(mFlameSparksId);

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

    if (pFireBackgroundGlow)
    {
        pFireBackgroundGlow->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        mFireBackgroundGlowId = Guid{};
    }

    if (pFlameSparks)
    {
        pFlameSparks->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        mFlameSparksId = Guid{};
    }
}

void DoorFlame::VUpdate()
{
    auto pFireBackgroundGlow = static_cast<FireBackgroundGlow*>(sObjectIds.Find_Impl(mFireBackgroundGlowId));
    auto pFlameSparks = static_cast<FlameSparks*>(sObjectIds.Find_Impl(mFlameSparksId));

    switch (mState)
    {
        case States::eDisabled_0:
            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

            if (pFlameSparks)
            {
                pFlameSparks->SetRenderEnabled_45E240(0);
            }

            if (SwitchStates_Get(mSwitchId))
            {
                mState = States::eEnabled_1;
            }

            if (pFireBackgroundGlow)
            {
                pFireBackgroundGlow->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                pFireBackgroundGlow = nullptr;
                mFireBackgroundGlowId = Guid{};
            }
            break;

        case States::eEnabled_1:
            if (!pFlameControllingTheSound_5C2C6C)
            {
                pFlameControllingTheSound_5C2C6C = this;
                mSoundsMask = SfxPlayMono(relive::SoundEffects::Fire, 40);
            }

            if (--mRandom <= 0)
            {
                mRandom = 2;
                if (pFireBackgroundGlow)
                {
                    pFireBackgroundGlow->Calc_Rect_45DA00();
                }
            }

            mAnim.mFlags.Set(AnimFlags::eBit3_Render);

            if (pFlameSparks)
            {
                pFlameSparks->SetRenderEnabled_45E240(1);
            }

            if (!SwitchStates_Get(mSwitchId))
            {
                mState = States::eDisabled_0;
            }

            if (!pFireBackgroundGlow)
            {
                pFireBackgroundGlow = relive_new FireBackgroundGlow(mXPos,
                                                                 mYPos,
                                                                 mSpriteScale);
                if (pFireBackgroundGlow)
                {
                    mFireBackgroundGlowId = pFireBackgroundGlow->mBaseGameObjectId;
                }
            }
            break;

        default:
            break;
    }

    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);

        if (pFireBackgroundGlow)
        {
            pFireBackgroundGlow->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mFireBackgroundGlowId = Guid{};
        }

        if (pFlameSparks)
        {
            pFlameSparks->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mFlameSparksId = Guid{};
        }
    }
}
