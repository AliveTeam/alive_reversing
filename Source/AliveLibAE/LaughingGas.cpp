#include "stdafx.h"
#include "LaughingGas.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "stdlib.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Events.hpp"
#include <math.h>
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

s32 gLaughingGasInstanceCount = 0;
bool gLaughingGasOn = false;

static const u32 sRedShift = 11;
static const u32 sGreenShift = 6;
static const u32 sBlueShift = 0;

// On linux not using this random algorithm produces much bigger numbers
// which causes flickering in the gas rendering. Apparently this is the MSVC algorithm.
static u32 random_seed = 0;
static s32 gas_rand()
{
    random_seed = (random_seed * 214013 + 2531011) & 0xFFFFFFFF;
    return static_cast<s32>((random_seed >> 16) & 0x7FFF);
}

LaughingGas::LaughingGas(Layer layer, relive::Path_LaughingGas* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    gLaughingGasInstanceCount++;

    SetType(ReliveTypes::eLaughingGas);

    mLaughingGasSwitchId = pTlv->mLaughingGasSwitchId;

    if (SwitchStates_Get(pTlv->mLaughingGasSwitchId))
    {
        mAmountOn = FP_FromInteger(1);
    }
    else
    {
        mAmountOn = FP_FromInteger(0);
    }

    mTlvInfo = tlvId;
    mLayer = layer;

    gObjListDrawables->Push_Back(this);
    SetDrawable(true);

    mY = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftY) - gScreenManager->CamYPos());
    mX = FP_GetExponent(PsxToPCX(FP_FromInteger(pTlv->mTopLeftX) - gScreenManager->CamXPos()));

    mH = FP_GetExponent(FP_FromInteger(pTlv->mBottomRightY) - gScreenManager->CamYPos());
    mW = FP_GetExponent(PsxToPCX(FP_FromInteger(pTlv->mBottomRightX) - gScreenManager->CamXPos()));

    mWidthCount = (mW - mX) / 4;
    mHeightCount = (mH - mY + 2) / 2;

    mpGasPixels = relive_new u16[mHeightCount * mWidthCount];

    Init();
    VUpdate();
}

LaughingGas::~LaughingGas()
{
    GetMap().TLV_Reset(mTlvInfo);
    gObjListDrawables->Remove_Item(this);
    gLaughingGasOn = false;
    gLaughingGasInstanceCount--;
    relive_delete[] mpGasPixels;
}

void LaughingGas::VScreenChanged()
{
    SetDead(true);
}

const f32 dword_551C58[7] = {1.0, 5.0, 10.0, 10.0, 5.0, 1.0, 0.0};

void LaughingGas::Init()
{
    for (s32 i = 0; i < mWidthCount; i++)
    {
        const f32 v4 = (f32) i / (f32) mWidthCount;
        f32 val1 = 1.0f;
        for (s32 j = 0; j < ALIVE_COUNTOF(dword_551C58); j++)
        {
            mDataX[i].array_4[j] = val1 * dword_551C58[j];
            val1 *= v4;
        }

        f32* pElem_ptr = &mDataX[i].array_4[5];
        const f32 v1 = 1.0f - v4;
        f32 val2 = 1.0f;
        for (s32 j = 0; j < 6; j++)
        {
            const f32 calc2 = val2 * *pElem_ptr;
            *pElem_ptr = calc2;
            --pElem_ptr;
            val2 *= v1;
        }
    }

    for (s32 i = 0; i < mHeightCount; i++)
    {
        const f32 v4 = (f32) i / (f32) mHeightCount;
        f32 val1 = 1.0f;
        for (s32 j = 0; j < ALIVE_COUNTOF(dword_551C58); j++)
        {
            mDataY[i].array_4[j] = val1 * dword_551C58[j];
            val1 *= v4;
        }

        f32* pElem_ptr = &mDataY[i].array_4[5];
        const f32 v1 = 1.0f - v4;
        f32 val2 = 1.0f;
        for (s32 j = 0; j < 6; j++)
        {
            const f32 calc2 = val2 * *pElem_ptr;
            *pElem_ptr = calc2;
            --pElem_ptr;
            val2 *= v1;
        }
    }

    for (s32 i = 0; i < 6; i++)
    {
        for (s32 j = 0; j < 6; j++)
        {
            mGasX[i][j] = static_cast<f32>(gas_rand()) * 6.28f * (1.0f / 32767.0f);
        }
    }

    mGasPrim.x = mX;
    mGasPrim.y = mY;
    mGasPrim.w = mW;
    mGasPrim.h = mH;
    mGasPrim.pGasPixels = mpGasPixels;
}

void LaughingGas::VRender(OrderingTable& ot)
{
    if (mAmountOn > FP_FromDouble(0.1) && mpGasPixels)
    {
        if (gDisplayRenderFrame)
        {
            DoRender();
        }
        ot.Add(mLayer, &mGasPrim);
    }
}

void LaughingGas::DoRender()
{
    f32 local_array[6];

    u16* pixelPtr = mpGasPixels;
    s32 rgb_base = (1 << sRedShift) + (1 << sGreenShift);

    for (s32 yCount = 0; yCount < mHeightCount; ++yCount)
    {
        for (s32 p = 0; p < ALIVE_COUNTOF(local_array); p++)
        {
            local_array[p] = Calc_Y(&mGasY[p][0], yCount);
        }

        for (s32 xCount = 0; xCount < mWidthCount; ++pixelPtr)
        {
            f32 yValue = Calc_X(local_array, xCount);
            if (yValue > 0.0f)
            {
                if (yValue >= 3.0f)
                {
                    if (yValue > 31.0f)
                    {
                        yValue = 31.0f;
                    }
                }
                else
                {
                    yValue = 3.0f;
                }
            }
            else
            {
                yValue = 0.0f;
            }

            *pixelPtr = static_cast<u16>(rgb_base * (static_cast<u8>(yValue) & 30));
            ++xCount;
        }
    }
}

// This is exactly 1 second
bool LaughingGas::CounterOver() const
{
    return mAmountOn >= FP_FromDouble(0.3);
}

void LaughingGas::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    if (SwitchStates_Get(mLaughingGasSwitchId))
    {
        if (mAmountOn < FP_FromInteger(1))
        {
            mAmountOn += FP_FromDouble(0.01);
        }
    }
    else
    {
        if (mAmountOn > FP_FromInteger(0))
        {
            mAmountOn -= FP_FromDouble(0.01);
        }
    }

    gLaughingGasOn = CounterOver();
    UpdateGasPos();
}

f32 LaughingGas::Calc_X(f32* a2, s32 xIndex)
{
    f32 result = 0.0;
    f32* v4 = a2 + 1;

    for (s32 i = 0; i < 4; i++)
    {
        const f32 v5 = mDataX[xIndex].array_4[i + 1];
        result += v5 * *v4;
        v4++;
    }

    return result;
}

f32 LaughingGas::Calc_Y(f32* a2, s32 yIndex)
{
    f32 result = 0.0;
    f32* v4 = a2 + 1;

    for (s32 i = 0; i < 4; i++)
    {
        const f32 v5 = mDataY[yIndex].array_4[i + 1];
        result += v5 * *v4;
        v4++;
    }

    return result;
}

void LaughingGas::UpdateGasPos()
{
    for (s32 i = 0; i < 4; i++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            const s32 rnd = gas_rand() - 8191;
            mGasY[1 + i][1 + j] = (sin(mGasX[1 + i][1 + j]) * 50.0f + 30.0f) * static_cast<f32>(FP_GetDouble(mAmountOn));
            mGasX[1 + i][1 + j] += (f32)(rnd) *0.03f * (1.0f / 16383.0f);
        }
    }
}
