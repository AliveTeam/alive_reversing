#include "stdafx.h"
#include "ElectricWall.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "BaseAliveGameObject.hpp"
#include "Flash.hpp"
#include "Abe.hpp"
#include "Electrocute.hpp"
#include "Function.hpp"

const s16 sElecticWallFrames_55165C[6] = {0, 6, 10, 18, 22, 0};

ElectricWall::ElectricWall(relive::Path_ElectricWall* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eElectricWall);

    const AnimRecord& rec = AnimRec(AnimId::Electric_Wall);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Electric_Wall, ppRes);
    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mAnim.mRenderMode = TPageAbr::eBlend_1;
    mAnim.mRenderLayer = Layer::eLayer_Foreground_36;

    if (mAnim.Get_Frame_Count() > 0)
    {
        mAnim.SetFrame(sElecticWallFrames_55165C[Math_RandomRange(0, 4)]);
    }

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mRGB.SetRGB(80, 80, 80);

    field_F4_tlvInfo = tlvInfo;

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

    field_F8_switch_id = pTlv->mSwitchId;
    field_FA_start_state = pTlv->mStartState;

    if (SwitchStates_Get(field_F8_switch_id) == field_FA_start_state)
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    }

    field_FC_sound_timer = 0;
}

ElectricWall::~ElectricWall()
{
    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (SwitchStates_Get(field_F8_switch_id) == field_FA_start_state)
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        // If we are about to become visible set a random starting frame
        if (!(mAnim.mFlags.Get(AnimFlags::eBit3_Render)))
        {
            if (mAnim.Get_Frame_Count() > 0)
            {
                mAnim.SetFrame(sElecticWallFrames_55165C[Math_RandomRange(0, 4)]);
            }
        }

        mAnim.mFlags.Set(AnimFlags::eBit3_Render);

        // Keep flipping direction
        if (!(sGnFrame % 8))
        {
            mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
        }

        // Play sound every so often
        if (static_cast<s32>(sGnFrame) >= field_FC_sound_timer)
        {
            SFX_Play_Camera(SoundEffect::BirdPortalSpark_41, 45, soundDirection, mSpriteScale);
            field_FC_sound_timer = sGnFrame + Math_RandomRange(24, 40);
        }

        const PSX_RECT bRect = VGetBoundingRect();

        PSX_RECT bRectBigger;
        bRectBigger.x = FP_GetExponent(mXPos - FP_FromInteger(4));
        bRectBigger.y = static_cast<s16>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(mXPos + FP_FromInteger(4));
        bRectBigger.h = static_cast<s16>(bRect.h + 5);

        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            switch (pObj->Type())
            {
                // Can't zap this
                case ReliveTypes::eBone:
                case ReliveTypes::eRockSpawner:
                case ReliveTypes::eGrenade:
                case ReliveTypes::eMeat:
                case ReliveTypes::eRock:
                    break;

                default:
                    if (pObj->mScale == mScale)
                    {
                        PSX_RECT objRect = pObj->VGetBoundingRect();

                        // If touching rect then we are fried
                        if (!RectsOverlap(bRectBigger, objRect))
                        {
                            // Not touching, so every so often check if we are near
                            if (!(sGnFrame % 3))
                            {
                                // Make each side of the rect wider
                                objRect.x -= 50;
                                objRect.w += 50;

                                if (RectsOverlap(bRectBigger, objRect) && pObj->mHealth > FP_FromInteger(0))
                                {
                                    // When near play the buzzing sound
                                    SFX_Play_Camera(SoundEffect::ElectricGateLoud_40, 45, soundDirection, mSpriteScale);
                                }
                            }
                        }
                        else
                        {
                            // Touching the wall, rip
                            if (!(pObj->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted)) && (!IsActiveHero(pObj) || !gAbeBulletProof_5C1BDA))
                            {
                                pObj->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit7_Electrocuted);

                                relive_new Electrocute(pObj, 1, 1);

                                pObj->VTakeDamage(this);

                                SFX_Play_Camera(SoundEffect::ElectricZap_39, 127, soundDirection, mSpriteScale);

                                relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, TPageAbr::eBlend_3, 1);
                            }
                        }
                    }
                    break;
            }
        }
    }
}
