#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "DoorFlame.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Midi.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/AnimationUnknown.hpp"
#include "Path.hpp"

namespace AO {

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

        SetApplyShadowZoneColour(true);

        GetAnimation().SetBlending(false);
        GetAnimation().SetSemiTrans(true);
        GetAnimation().SetIgnorePosOffset(true);

        mXPos = xpos;
        mYPos = ypos + FP_FromInteger(4);

        GetAnimation().SetRenderLayer(Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12);
        GetAnimation().SetRenderMode(TPageAbr::eBlend_3);

        GetAnimation().SetRGB(100, 100, 63);

        SetSpriteScale(scale);

        Calc_Rect();
    }

    virtual void VUpdate() override
    {
        // Empty
    }

    void Calc_Rect()
    {
        PSX_Point xy = {};

        s16 frameW = 0;
        s16 frameH = 0;

        GetAnimation().Get_Frame_Width_Height(&frameW, &frameH);
        GetAnimation().Get_Frame_Offset(&xy.x, &xy.y);

        const auto& pCamPos = gScreenManager->mCamPos;
        const FP screenX = FP_FromInteger(gScreenManager->mCamXOff) + mXPos - pCamPos->x;
        const FP screenY = FP_FromInteger(gScreenManager->mCamYOff) + mYPos - pCamPos->y;

        const FP frameWScaled = (FP_FromInteger(frameW) * GetSpriteScale());
        const FP frameHScaled = (FP_FromInteger(frameH) * GetSpriteScale());

        const s32 offXScaled = FP_GetExponent(FP_FromInteger(xy.x) * GetSpriteScale());
        const s32 offYScaled = FP_GetExponent(FP_FromInteger(xy.y) * GetSpriteScale());

        // TODO: Refactor PSX <> PC width conversion
        const FP frameWScaled_converted = ((frameWScaled * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40);
        // Why isn't this converted ??
        //const s16 offXScaled_converted = FP_GetExponent(((FP_FromInteger(offXScaled) * FP_FromInteger(23)) + FP_FromInteger(20)) / FP_FromInteger(40));

        field_E4_xPos = screenX + FP_FromInteger(offXScaled) + FP_FromInteger(Math_NextRandom() % 3);
        field_E8_yPos = screenY + FP_FromInteger(offYScaled) + FP_FromInteger((Math_NextRandom() % 3));
        field_EC_xOff = screenX + FP_FromInteger(offXScaled) + frameWScaled_converted + FP_FromInteger(Math_NextRandom() % 3);
        field_F0_yOff = screenY + FP_FromInteger(offYScaled) + frameHScaled + FP_FromInteger(Math_NextRandom() % 3);
    }

    virtual void VRender(PrimHeader** ppOt) override
    {
        if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            GetAnimation().SetRGB(mRGB.r, mRGB.g, mRGB.b);

            GetAnimation().VRender(
                FP_GetExponent(field_E4_xPos),
                FP_GetExponent(field_E8_yPos),
                ppOt,
                FP_GetExponent(field_EC_xOff - field_E4_xPos) + 1,
                FP_GetExponent(field_F0_yOff - field_E8_yPos) + 1);
        }
    }

private:
    FP field_E4_xPos;
    FP field_E8_yPos;
    FP field_EC_xOff;
    FP field_F0_yOff;
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

private:
    virtual void VUpdate() override
    {

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

                        anim.field_8_off_x = FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(96);
                        anim.field_C_off_y = FP_FromInteger(-Math_NextRandom()) / FP_FromInteger(96);
                    }
                }
                else if (anim.field_10_random64 > 0)
                {
                    anim.x += anim.field_8_off_x;
                    anim.y += anim.field_C_off_y;

                    if (!(anim.field_10_random64 % 3))
                    {
                        anim.field_8_off_x += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
                        anim.field_C_off_y += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
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

    virtual void VRender(PrimHeader** ppOt) override
    {
        if (gNumCamSwappers == 0)
        {
            if (mRender)
            {
                GetAnimation().SetRGB(240, 32, 32);

                const FP_Point* pCamPos = gScreenManager->mCamPos;

                const FP screen_left = pCamPos->x - FP_FromInteger(gScreenManager->mCamXOff);
                const FP screen_right = pCamPos->x + FP_FromInteger(gScreenManager->mCamXOff);
                const FP screen_top = pCamPos->y - FP_FromInteger(gScreenManager->mCamYOff);
                const FP screen_bottom = pCamPos->y + FP_FromInteger(gScreenManager->mCamYOff);

                GetAnimation().VRender(
                    FP_GetExponent(PsxToPCX(mXPos - screen_left)),
                    FP_GetExponent(mYPos - screen_top),
                    ppOt,
                    0,
                    0);

                for (auto& anim : mSparks)
                {
                    if (anim.field_12_bVisible)
                    {
                        if (anim.x >= screen_left && anim.x <= screen_right)
                        {
                            if (anim.y >= screen_top && anim.y <= screen_bottom)
                            {
                                anim.field_14.VRender(
                                    FP_GetExponent(PsxToPCX(anim.x - screen_left)),
                                    FP_GetExponent(anim.y - screen_top),
                                    ppOt, 0, 0);
                            }
                        }
                    }
                }
            }
        }
    }

public:
    bool mRender;
    FlameSpark mSparks[6];
    FP mStartXPos;
    FP mStartYPos;
};

DoorFlame::DoorFlame(relive::Path_DoorFlame* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eNone);
    mTlvInfo = tlvId;

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Fire));
    Animation_Init(GetAnimRes(AnimId::Fire));

    GetAnimation().SetSemiTrans(true);
    SetApplyShadowZoneColour(true);
    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
    mFrameCount = GetAnimation().Get_Frame_Count();
    mSwitchId = pTlv->mSwitchId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
        mYPos = FP_FromInteger(pTlv->mTopLeftY + 15);
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
        mYPos = FP_FromInteger(pTlv->mTopLeftY + 15);
    }

    switch (pTlv->mColour)
    {
        case relive::Path_DoorFlame::Colour::eRed:
            mRGB.r = 127;
            break;
        case relive::Path_DoorFlame::Colour::eGreen:
            mRGB.g = 127;
            break;
        case relive::Path_DoorFlame::Colour::eBlue:
            mRGB.b = 127;
            break;
        
        case relive::Path_DoorFlame::Colour::eDefault:
        default:
            break;
    }

    mFireBackgroundGlow = 0;

    if (SwitchStates_Get(pTlv->mSwitchId))
    {
        GetAnimation().SetRender(true);
        mState = States::eEnabled_1;
    }
    else
    {
        GetAnimation().SetRender(false);
        mState = States::eDisabled_0;
    }

    GetAnimation().SetAnimate(true);
    mRandom = Math_NextRandom() & 1;

    mFlameSparks = relive_new FlameSparks(mXPos, mYPos);
    if (mFlameSparks)
    {
        mFlameSparks->mBaseGameObjectRefCount++;
    }
}

DoorFlame::~DoorFlame()
{
    if (mFireBackgroundGlow)
    {
        mFireBackgroundGlow->mBaseGameObjectRefCount--;
        mFireBackgroundGlow->SetDead(true);
        mFireBackgroundGlow = nullptr;
    }

    if (mFlameSparks)
    {
        mFlameSparks->mBaseGameObjectRefCount--;
        mFlameSparks->SetDead(true);
        mFlameSparks = nullptr;
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
    }
}
void DoorFlame::VUpdate()
{
    switch (mState)
    {
        case States::eDisabled_0:
            GetAnimation().SetRender(false);

            if (mFlameSparks)
            {
                mFlameSparks->mRender = false;
            }

            if (SwitchStates_Get(mSwitchId))
            {
                mState = States::eEnabled_1;
            }

            if (mFireBackgroundGlow)
            {
                mFireBackgroundGlow->mBaseGameObjectRefCount--;
                mFireBackgroundGlow->SetDead(true);
                mFireBackgroundGlow = nullptr;
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
                if (mFireBackgroundGlow)
                {
                    mFireBackgroundGlow->Calc_Rect();
                }
            }

            GetAnimation().SetRender(true);
            if (mFlameSparks)
            {
                mFlameSparks->mRender = true;
            }

            if (!SwitchStates_Get(mSwitchId))
            {
                mState = States::eDisabled_0;
            }

            if (!mFireBackgroundGlow)
            {
                mFireBackgroundGlow = relive_new FireBackgroundGlow(mXPos,
                    mYPos + FP_FromInteger(4),
                    FP_FromDouble(0.5));
                if (mFireBackgroundGlow)
                {
                    mFireBackgroundGlow->mBaseGameObjectRefCount++;
                    mFireBackgroundGlow->mRGB = mRGB;
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
    }
}

} // namespace AO
