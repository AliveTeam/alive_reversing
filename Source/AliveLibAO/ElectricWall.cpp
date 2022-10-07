#include "stdafx_ao.h"
#include "Function.hpp"
#include "ElectricWall.hpp"
#include "SwitchStates.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Electrocute.hpp"
#include "Flash.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

ElectricWall::ElectricWall(relive::Path_ElectricWall* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eElectricWall);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Electric_Wall));
    Animation_Init(GetAnimRes(AnimId::Electric_Wall));

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mAnim.mRenderMode = TPageAbr::eBlend_1;
    mAnim.mRenderLayer = Layer::eLayer_Foreground_36;

    if (mAnim.Get_Frame_Count() > 0)
    {
        mAnim.SetFrame(Math_NextRandom() % mAnim.Get_Frame_Count());
    }

    mRGB.SetRGB(80, 80, 80);
    mTlvInfo = tlvId;
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    mSwitchId = pTlv->mSwitchId;
    mStartState = pTlv->mStartState;

    if (SwitchStates_Get(pTlv->mSwitchId) == mStartState)
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    }

    mSoundTimer = 0;
}

ElectricWall::~ElectricWall()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void ElectricWall::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) == CameraPos::eCamInvalid_m1)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ElectricWall::VUpdate()
{
    const CameraPos soundDirection = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (SwitchStates_Get(mSwitchId) == mStartState)
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        mAnim.mFlags.Set(AnimFlags::eBit3_Render);

        // Keep flipping direction
        if (!(sGnFrame % 8))
        {
            mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
        }

        // Play sound every so often
        if (static_cast<s32>(sGnFrame) >= mSoundTimer)
        {
            // set a random starting frame
            SFX_Play_Camera(relive::SoundEffects::BirdPortalSpark, 45, soundDirection);
            mSoundTimer = sGnFrame + Math_RandomRange(24, 40);
        }


        const PSX_RECT bRect = VGetBoundingRect();

        PSX_RECT bRectBigger = {};
        bRectBigger.x = FP_GetExponent(mXPos - FP_FromInteger(4));
        bRectBigger.y = static_cast<s16>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(mXPos + FP_FromInteger(4));
        bRectBigger.h = static_cast<s16>(bRect.h + 5);

        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            // Don't kill nades
            if (pObjIter->Type() != ReliveTypes::eGrenade)
            {
                PSX_RECT objRect = pObjIter->VGetBoundingRect();

                if (!RectsOverlap(bRectBigger, objRect))
                {
                    // Not touching, so every so often check if we are near
                    if (!(sGnFrame % 3))
                    {
                        // Make each side of the rect wider
                        objRect.x -= 50;
                        objRect.w += 50;

                        if (RectsOverlap(bRectBigger, objRect) && pObjIter->mHealth > FP_FromInteger(0))
                        {
                            SFX_Play_Camera(relive::SoundEffects::ElectricGateLoud, 45, soundDirection);
                        }
                    }
                }
                else
                {
                    // Touching the wall, rip
                    if (!pObjIter->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit5_Electrocuted))
                    {
                        pObjIter->mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit5_Electrocuted);
                        relive_new Electrocute(pObjIter, 1);

                        pObjIter->VTakeDamage(this);

                        SFX_Play_Camera(relive::SoundEffects::ElectricZap, 127, soundDirection);

                        relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
                    }
                }
            }
        }
    }
}

} // namespace AO
