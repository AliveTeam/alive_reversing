#include "stdafx.h"
#include "DoorFlame.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "Game.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Sfx.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/AnimationUnknown.hpp"
#include "../relive_lib/FixedPoint.hpp"

static DoorFlame* sFlameControllingTheSound = nullptr;

class FireBackgroundGlow final : public BaseAnimatedWithPhysicsGameObject
{
public:
    FireBackgroundGlow(FP xpos, FP ypos, FP scale)
        : BaseAnimatedWithPhysicsGameObject(0)
    {
        SetType(ReliveTypes::eNone);

        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_FireBackgroundGlow));
        Animation_Init(GetAnimRes(AnimId::Door_FireBackgroundGlow));

        SetApplyShadowZoneColour(false);

        GetAnimation().SetBlending(false);
        GetAnimation().SetSemiTrans(true);
        GetAnimation().SetIgnorePosOffset(true);

        mXPos = xpos;
        mYPos = ypos + FP_FromInteger(4);

        GetAnimation().SetRenderLayer(Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12);
        GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_3);

        mRGB.SetRGB(140, 90, 53);

        SetSpriteScale(scale);

        Calc_Rect();
    }

    void Calc_Rect()
    {
        PSX_Point xy = {};

        s16 frameW = 0;
        s16 frameH = 0;

        GetAnimation().Get_Frame_Width_Height(&frameW, &frameH);
        GetAnimation().Get_Frame_Offset(&xy.x, &xy.y);

        const FP screenX = mXPos - gScreenManager->CamXPos();
        const FP screenY = mYPos - gScreenManager->CamYPos();

        const FP frameWScaled = (FP_FromInteger(frameW) * GetSpriteScale());
        const FP frameHScaled = (FP_FromInteger(frameH) * GetSpriteScale());

        const FP offXScaled = (FP_FromInteger(xy.x) * GetSpriteScale());
        const s16 offYScaled = FP_GetExponent((FP_FromInteger(xy.y) * GetSpriteScale()));

        // TODO: Refactor PSX <> PC width conversion
        const FP frameWScaled_converted = (((frameWScaled * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40));
        const s16 offXScaled_converted = FP_GetExponent(((offXScaled * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40));

        field_F4_xPos = screenX + FP_FromInteger(offXScaled_converted) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
        field_F8_yPos = screenY + FP_FromInteger(offYScaled) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
        field_FC_xOff = screenX + FP_FromInteger(offXScaled_converted + FP_GetExponent(frameWScaled_converted)) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
        field_100_yOff = screenY + FP_FromInteger(offYScaled + FP_GetExponent(frameHScaled)) + FP_FromInteger(Math_NextRandom() % 3) - FP_FromInteger(1);
    }

    virtual void VRender(OrderingTable& ot) override
    {
        if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            GetAnimation().SetRGB(mRGB.r & 0xFF, mRGB.g & 0xFF, mRGB.b & 0xFF);

            const FP xOff = field_FC_xOff - field_F4_xPos;
            const FP yOff = field_100_yOff - field_F8_yPos;

            GetAnimation().VRender(
                FP_GetExponent(field_F4_xPos),
                FP_GetExponent(field_F8_yPos),
                ppOt,
                FP_GetExponent(xOff) + 1,
                FP_GetExponent(yOff) + 1);
        }
    }

private:
    FP field_F4_xPos;
    FP field_F8_yPos;
    FP field_FC_xOff;
    FP field_100_yOff;
};

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
class FlameSparks final : public BaseAnimatedWithPhysicsGameObject
{
public:
    FlameSparks(FP xpos, FP ypos)
        : BaseAnimatedWithPhysicsGameObject(0)
    {
        SetType(ReliveTypes::eNone);

        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle_Small));
        Animation_Init(GetAnimRes(AnimId::ChantOrb_Particle_Small));

        GetAnimation().SetSemiTrans(true);

        SetApplyShadowZoneColour(true);
        GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);

        mXPos = xpos;
        mYPos = ypos;
        mStartXPos = xpos;
        mStartYPos = ypos;

        SetSpriteScale(FP_FromDouble(0.3));

        for (auto& anim : mSparks)
        {
            anim.field_14.mAnimPtr = &GetAnimation();

            anim.field_14.SetRender(true);
            anim.field_14.SetBlending(true);

            // TODO: clean this up
            const s32 rndLayer = static_cast<s32>(GetAnimation().GetRenderLayer()) + Math_RandomRange(-1, 1);
            anim.field_14.SetRenderLayer(static_cast<Layer>(rndLayer));
            anim.field_14.mSpriteScale = GetSpriteScale();

            anim.x = mXPos;
            anim.y = mYPos;

            anim.field_8_off_x = FP_FromInteger(0);
            anim.field_C_off_y = FP_FromInteger(0);

            anim.field_10_random64 = Math_RandomRange(0, 64);
            anim.field_12_bVisible = 0;
        }

        mRender = 0;
    }

    void SetRenderEnabled(bool bEnable)
    {
        mRender = bEnable;
    }

private:
    virtual void VUpdate() override
    {
        // HACK/WTF this seems to move the base animation off screen so it can never been seen??
        PSX_RECT rect = {};
        gMap.Get_Camera_World_Rect(CameraPos::eCamCurrent_0, &rect);
        mXPos = FP_FromInteger(rect.w + 16);
        mYPos = FP_FromInteger(rect.y - 16);

        if (mRender)
        {
            for (auto& anim : mSparks)
            {
                anim.field_10_random64--;
                if (anim.field_12_bVisible == 0)
                {
                    if (anim.field_10_random64 <= 0)
                    {
                        anim.field_12_bVisible = 1;
                        anim.field_10_random64 = Math_RandomRange(7, 9);

                        anim.x = mStartXPos;
                        anim.y = mStartYPos;

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

    virtual void VRender(OrderingTable& ot) override
    {
        if (gNumCamSwappers == 0)
        {
            if (mRender)
            {
                GetAnimation().SetRGB(240, 32, 32);

                GetAnimation().VRender(
                    FP_GetExponent(mXPos - gScreenManager->CamXPos()),
                    FP_GetExponent(mYPos - gScreenManager->CamYPos()),
                    ppOt,
                    0,
                    0);

                for (auto& anim : mSparks)
                {
                    if (anim.field_12_bVisible)
                    {
                        // In screen bounds?
                        if (anim.x >= gScreenManager->CamXPos() && anim.x <= gScreenManager->CamXPos() + FP_FromInteger(368))
                        {
                            if (anim.y >= gScreenManager->CamYPos() && anim.y <= gScreenManager->CamYPos() + FP_FromInteger(240))
                            {
                                anim.field_14.VRender(
                                    FP_GetExponent(anim.x - gScreenManager->CamXPos()),
                                    FP_GetExponent(anim.y - gScreenManager->CamYPos()),
                                    ppOt, 0, 0);
                            }
                        }
                    }
                }
            }
        }
    }

private:
    bool mRender;
    FlameSpark mSparks[6];
    FP mStartXPos;
    FP mStartYPos;
};

DoorFlame::DoorFlame(relive::Path_DoorFlame* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0),
    mTlvInfo(tlvId),
    mSwitchId(pTlv->mSwitchId)
{
    SetType(ReliveTypes::eNone);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Fire));
    Animation_Init(GetAnimRes(AnimId::Fire));

    GetAnimation().SetSemiTrans(true);
    SetApplyShadowZoneColour(true);
    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
    mFrameCount = static_cast<s16>(GetAnimation().Get_Frame_Count());
    GetAnimation().SetFrame(Math_RandomRange(0, mFrameCount - 1));

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromDouble(0.5));
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX) + (FP_FromInteger(12) * GetSpriteScale());
    mYPos = FP_FromInteger(pTlv->mTopLeftY) + (FP_FromInteger(15) * GetSpriteScale());

    if (SwitchStates_Get(mSwitchId))
    {
        GetAnimation().SetRender(true);
        mState = States::eEnabled_1;
    }
    else
    {
        GetAnimation().SetRender(false);
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
        pFireBackgroundGlow->SetDead(true);
        mFireBackgroundGlowId = Guid{};
    }

    if (pFlameSparks)
    {
        pFlameSparks->SetDead(true);
        mFlameSparksId = Guid{};
    }

    VStopAudio();

    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void DoorFlame::VStopAudio()
{
    if (sFlameControllingTheSound == this)
    {
        sFlameControllingTheSound = nullptr;
        SND_Stop_Channels_Mask(mSoundsMask);
        mSoundsMask = 0;
    }
}

void DoorFlame::VScreenChanged()
{
    BaseGameObject* pFireBackgroundGlow = sObjectIds.Find_Impl(mFireBackgroundGlowId);
    BaseGameObject* pFlameSparks = sObjectIds.Find_Impl(mFlameSparksId);

    SetDead(true);

    if (pFireBackgroundGlow)
    {
        pFireBackgroundGlow->SetDead(true);
        mFireBackgroundGlowId = Guid{};
    }

    if (pFlameSparks)
    {
        pFlameSparks->SetDead(true);
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
            GetAnimation().SetRender(false);

            if (pFlameSparks)
            {
                pFlameSparks->SetRenderEnabled(false);
            }

            if (SwitchStates_Get(mSwitchId))
            {
                mState = States::eEnabled_1;
            }

            if (pFireBackgroundGlow)
            {
                pFireBackgroundGlow->SetDead(true);
                pFireBackgroundGlow = nullptr;
                mFireBackgroundGlowId = Guid{};
            }
            break;

        case States::eEnabled_1:
            if (!sFlameControllingTheSound)
            {
                sFlameControllingTheSound = this;
                mSoundsMask = SfxPlayMono(relive::SoundEffects::Fire, 40);
            }

            if (--mRandom <= 0)
            {
                mRandom = 2;
                if (pFireBackgroundGlow)
                {
                    pFireBackgroundGlow->Calc_Rect();
                }
            }

            GetAnimation().SetRender(true);

            if (pFlameSparks)
            {
                pFlameSparks->SetRenderEnabled(true);
            }

            if (!SwitchStates_Get(mSwitchId))
            {
                mState = States::eDisabled_0;
            }

            if (!pFireBackgroundGlow)
            {
                pFireBackgroundGlow = relive_new FireBackgroundGlow(mXPos,
                                                                 mYPos,
                                                                 GetSpriteScale());
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
        SetDead(true);

        if (pFireBackgroundGlow)
        {
            pFireBackgroundGlow->SetDead(true);
            mFireBackgroundGlowId = Guid{};
        }

        if (pFlameSparks)
        {
            pFlameSparks->SetDead(true);
            mFlameSparksId = Guid{};
        }
    }
}
