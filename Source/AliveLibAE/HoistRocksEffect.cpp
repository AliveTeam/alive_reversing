#include "stdafx.h"
#include "HoistRocksEffect.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"

const static AnimId HoistRocksAnimIdTable[4] = 
{AnimId::HoistRock1,
AnimId::HoistRock2,
AnimId::HoistRock3,
AnimId::HoistRock1};

const static s16 word_5556F0[12] = {5, 0, 10, 0, 30, 0, 5, 0, 0, 0, 0, 0};

HoistRocksEffect::HoistRocksEffect(Path_Hoist* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_24_tlvInfo = tlvInfo;

    field_20_xpos = (pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2;
    field_22_ypos = pTlv->mTopLeft.y;
    field_28_timer = 0;

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        field_2C_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_2C_scale = FP_FromDouble(1.0);
    }

    if (gObjListDrawables->Push_Back(this))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    }

    u8** ppAnimData = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kHoistRocks);
    for (HoistRockParticle& particle : field_30_rocks)
    {
        particle.field_10_mAnim.Init(
            AnimId::HoistRock1,
            this,
            ppAnimData);

        particle.field_10_mAnim.mRed = 255;
        particle.field_10_mAnim.mGreen = 255;
        particle.field_10_mAnim.mBlue = 255;

        if (pTlv->field_16_scale == Scale_short::eHalf_1)
        {
            particle.field_10_mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        }
        else
        {
            particle.field_10_mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        }

        particle.field_10_mAnim.mRenderMode = TPageAbr::eBlend_0;
        particle.field_10_mAnim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
        particle.field_10_mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
        particle.field_10_mAnim.field_14_scale = field_2C_scale;

        particle.field_0_state = 0;
    }
}

HoistRocksEffect::~HoistRocksEffect()
{
    gObjListDrawables->Remove_Item(this);

    for (HoistRockParticle& particle : field_30_rocks)
    {
        particle.field_10_mAnim.VCleanUp();
    }

    Path::TLV_Reset(field_24_tlvInfo, -1, 0, 0);
}

void HoistRocksEffect::VUpdate()
{
    if (field_28_timer <= static_cast<s32>(sGnFrame))
    {
        s32 idx = 0;
        while (field_30_rocks[idx].field_0_state != 0)
        {
            if (++idx >= 4)
            {
                break;
            }
        }

        if (idx < 4)
        {
            s32 randomXScaled = 0;
            if (field_2C_scale == FP_FromDouble(1.0))
            {
                randomXScaled = Math_RandomRange(-8, 8);
            }
            else
            {
                randomXScaled = Math_RandomRange(-4, 4);
            }

            field_30_rocks[idx].field_4_xpos = FP_FromInteger(field_20_xpos + randomXScaled);
            field_30_rocks[idx].field_8_ypos = FP_FromInteger(field_22_ypos + Math_RandomRange(-4, 4));

            field_30_rocks[idx].field_C_yVel = FP_FromInteger(0);
            field_30_rocks[idx].field_0_state = 1;

            const s32 randomAnimAndUpdate = 2 * Math_RandomRange(0, 3);
            field_30_rocks[idx].field_10_mAnim.Set_Animation_Data(HoistRocksAnimIdTable[randomAnimAndUpdate / 2], nullptr);
            field_28_timer = sGnFrame + Math_RandomRange(word_5556F0[randomAnimAndUpdate], 2 * word_5556F0[randomAnimAndUpdate]);
        }
    }

    for (HoistRockParticle& particle : field_30_rocks)
    {
        if (particle.field_0_state)
        {
            if (particle.field_C_yVel >= FP_FromInteger(10))
            {
                if (!gMap.Is_Point_In_Current_Camera(
                        gMap.mCurrentLevel,
                        gMap.mCurrentPath,
                        particle.field_4_xpos,
                        particle.field_8_ypos,
                        0))
                {
                    particle.field_0_state = 0;
                }
            }
            else
            {
                particle.field_C_yVel += FP_FromDouble(0.6);
            }

            particle.field_8_ypos += particle.field_C_yVel;

            if (particle.field_0_state == 1)
            {
                PathLine* pLine = nullptr;
                FP hitX = {};
                FP hitY = {};
                if (sCollisions->Raycast(
                        particle.field_4_xpos,
                        particle.field_8_ypos - particle.field_C_yVel,
                        particle.field_4_xpos,
                        particle.field_8_ypos,
                        &pLine,
                        &hitX,
                        &hitY,
                        field_2C_scale > FP_FromDouble(0.5) ? kFgFloor : kBgFloor))
                {
                    particle.field_8_ypos = hitY;
                    particle.field_C_yVel = particle.field_C_yVel * FP_FromDouble(-0.3);
                    particle.field_0_state = 2;
                }
            }
        }
    }
}

void HoistRocksEffect::VRender(PrimHeader** ppOt)
{
    for (HoistRockParticle& particle : field_30_rocks)
    {
        if (particle.field_0_state)
        {
            particle.field_10_mAnim.VRender(
                FP_GetExponent(particle.field_4_xpos - pScreenManager->CamXPos()),
                FP_GetExponent(particle.field_8_ypos - pScreenManager->CamYPos()),
                ppOt,
                0,
                0);

            PSX_RECT frameRect = {};
            particle.field_10_mAnim.Get_Frame_Rect(&frameRect);
            pScreenManager->InvalidateRectCurrentIdx(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h);
        }
    }
}

void HoistRocksEffect::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
