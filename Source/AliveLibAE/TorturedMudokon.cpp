#include "stdafx.h"
#include "TorturedMudokon.hpp"
#include "Game.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Function.hpp"
#include "Abe.hpp"
#include "Path.hpp"
#include "Math.hpp"

void TorturedMudokon::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Tortured_Mudokon));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Tortured_Mudokon_Released));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Tortured_Mudokon_Tears));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Tortured_Mudokon_Zap));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Electric_Wall));
}

TorturedMudokon::TorturedMudokon(relive::Path_TorturedMudokon* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eTorturedMud);
    mTlvInfo = tlvId;

    LoadAnimations();

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    Animation_Init(GetAnimRes(AnimId::Tortured_Mudokon));
    GetAnimation().SetFrame(Math_RandomRange(0, static_cast<s16>(GetAnimation().Get_Frame_Count() - 1)));
    mKillSwitchId = pTlv->mKillSwitchId;
    mReleaseSwitchId = pTlv->mReleaseSwitchId;
    mState = TorturedMudokonState::eBeingTortured_0;
    SetupTearsAnimation(&mTearsAnim);
    SetupZapAnimation(&mZapAnim);
    mPainSoundPitch = Math_RandomRange(800, 1000);
    mFlashColourTimer = sGnFrame + 100;
    mFlashColourCounter = 0;
}

void TorturedMudokon::SetupTearsAnimation(Animation* pAnim)
{
    if (pAnim->Init(GetAnimRes(AnimId::Tortured_Mudokon_Tears), this))
    {
        pAnim->SetRenderLayer(GetAnimation().GetRenderLayer());
        pAnim->SetSpriteScale(GetSpriteScale());
        pAnim->SetRGB(128, 128, 128);
    }
    else
    {
        SetListAddFailed(true);
    }
}

void TorturedMudokon::SetupZapAnimation(Animation* pAnim)
{
    if (pAnim->Init(GetAnimRes(AnimId::Electric_Wall), this))
    {
        // TODO: clean this up
        const s32 layerM1 = static_cast<s32>(GetAnimation().GetRenderLayer()) - 1;
        pAnim->SetRenderLayer(static_cast<Layer>(layerM1));
        pAnim->SetSpriteScale(GetSpriteScale());
        pAnim->SetRGB(128, 128, 128);
    }
    else
    {
        SetListAddFailed(true);
    }
}

void TorturedMudokon::VScreenChanged()
{
    SetDead(true);
}

void TorturedMudokon::VRender(PrimHeader** ppOt)
{
    mTearsAnim.VRender(
        FP_GetExponent(mXPos - gScreenManager->CamXPos()),
        FP_GetExponent(mYPos - gScreenManager->CamYPos()),
        ppOt,
        0,
        0);
    if (mTearsAnim.GetRender())
    {
        mZapAnim.VRender(
            FP_GetExponent(mXPos - gScreenManager->CamXPos()),
            FP_GetExponent(mYPos - gScreenManager->CamYPos()),
            ppOt,
            0,
            0);
    }

    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}

TorturedMudokon::~TorturedMudokon()
{
    if (mState != TorturedMudokonState::eReleased_2)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }

    mTearsAnim.VCleanUp();
    mZapAnim.VCleanUp();
}

void TorturedMudokon::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
        return;
    }

    if (mFlashColourTimer == static_cast<s32>(sGnFrame))
    {
        mFlashColourCounter++;
        mFlashColourTimer = sGnFrame + 100;
        if (mFlashColourCounter == 4)
        {
            mFlashColourCounter = 0;
        }
    }

    switch (mState)
    {
        case TorturedMudokonState::eBeingTortured_0:
            if (SwitchStates_Get(mKillSwitchId))
            {
                mState = TorturedMudokonState::eKilled_1;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Tortured_Mudokon_Zap));
            }
            break;

        case TorturedMudokonState::eKilled_1:
            if (GetAnimation().GetIsLastFrame())
            {
                SetDead(true);
            }
            return;

        case TorturedMudokonState::eReleased_2:
            return;

        default:
            break;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (GetAnimation().GetFrameChangeCounter() == GetAnimation().GetFrameDelay())
        {
            mZapAnim.SetRender(false);
            if (!Math_RandomRange(0, 8))
            {
                Mudokon_SFX(MudSounds::eNoSad_22, 100, Math_RandomRange(mPainSoundPitch, mPainSoundPitch + 100), 0);
            }
        }
    }

    if (GetAnimation().GetCurrentFrame() == 6)
    {
        if (Math_RandomRange(0, 2))
        {
            GetAnimation().SetCurrentFrame(0);
        }
    }

    u8 rgbBase = 0;
    switch (mFlashColourCounter)
    {
        case 0:
            rgbBase = static_cast<u8>((mFlashColourTimer & 0xFF) - sGnFrame);
            break;

        case 1:
            rgbBase = 0;
            break;

        case 2:
            rgbBase = static_cast<u8>(sGnFrame - (mFlashColourTimer & 0xFF) + 100);
            break;

        case 3:
            rgbBase = 100;
            break;
    }

    if (GetAnimation().GetCurrentFrame() == 6 && GetAnimation().GetFrameChangeCounter() == GetAnimation().GetFrameDelay())
    {
        relive_new Flash(Layer::eLayer_Above_FG1_39, rgbBase + 50, rgbBase + 50, rgbBase + 110, TPageAbr::eBlend_1, 1);
        mZapAnim.SetRender(true);
        SfxPlayMono(relive::SoundEffects::ElectricZap, 70);
        const s16 sndRnd = Math_RandomRange(0, 3) - 1;
        if (sndRnd)
        {
            if (sndRnd == 1)
            {
                Mudokon_SFX(MudSounds::eHurt1_16, 127, Math_RandomRange(mPainSoundPitch, mPainSoundPitch + 100), 0);
            }
        }
        else
        {
            Mudokon_SFX(MudSounds::eHurt2_9, 127, Math_RandomRange(mPainSoundPitch, mPainSoundPitch + 100), 0);
        }
    }

    if (GetAnimation().GetCurrentFrame() >= 7 && !Math_RandomRange(0, 10))
    {
        relive_new Flash(Layer::eLayer_Above_FG1_39, rgbBase + 10, rgbBase + 10, rgbBase + 50, TPageAbr::eBlend_1, 1);
    }

    if (SwitchStates_Get(mReleaseSwitchId))
    {
        mState = TorturedMudokonState::eReleased_2;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Tortured_Mudokon_Released));
        mTearsAnim.SetRender(false);
        mZapAnim.SetRender(false);
        relive::Path_TLV* pTlv = gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo);
        if (pTlv)
        {
            pTlv->mTlvSpecificMeaning = 1;
        }
    }
}
