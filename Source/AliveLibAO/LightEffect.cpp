#include "stdafx_ao.h"
#include "Function.hpp"
#include "LightEffect.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "CameraSwapper.hpp"
#include "ScreenManager.hpp"

namespace AO {

LightEffect::LightEffect(Path_LightEffect* pTlv, s32 tlvInfo)
{
    mTypeId = Types::eNone_0;
    field_E4_tlvInfo = tlvInfo;

    const AnimRecord& rec = AO::AnimRec(AnimId::Star);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_EC_rnd1 = gnFrameCount_507670 + Math_RandomRange_450F20(2, 8);
    field_F0_rnd2 = field_EC_rnd1 + Math_RandomRange_450F20(4, 8);
    field_F4_rnd3 = Math_RandomRange_450F20(96, 128);
    field_F8_rnd4 = Math_RandomRange_450F20(190, 255);

    mApplyShadows &= ~1u;

    mAnim.mAnimFlags.Set(AnimFlags::eBit20_use_xy_offset);
    mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mAnim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

    mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    mAnim.mRenderMode = TPageAbr::eBlend_0;
    mAnim.SetFrame(1);

    // maybe width height like in door effect
    field_E8 = 0;
    field_EA = 0;

    mSpriteScale = FP_FromDouble(0.4);
    mXPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
}

LightEffect::~LightEffect()
{
    gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
}

void LightEffect::VRender(PrimHeader** /*ppOt*/)
{
    // Pretty much the same as door effect render - commented out because OG is empty which means the "stock yard stars" are missing.
    // However rendering as-is produces over bright ugly looking stars and is probably why DD turned it off
    /*
    if (sNumCamSwappers_507668 == 0)
    {
        const FP xpos = FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) + mXPos - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x;
        const FP ypos = FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) + mYPos - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y;

        mAnim.mRed = static_cast<u8>(mRed/4);
        mAnim.mGreen = static_cast<u8>(mGreen/4);
        mAnim.mBlue = static_cast<u8>(mBlue/4);

        mAnim.vRender(
            FP_GetExponent(FP_FromInteger((FP_GetExponent(xpos)))),
            FP_GetExponent(FP_FromInteger((FP_GetExponent(ypos)))),
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        mAnim.Get_Frame_Rect(&rect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager_4FF7C8->field_2E_idx);
    }
    */
}

void LightEffect::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}


void LightEffect::VUpdate()
{
    if (static_cast<s32>(gnFrameCount_507670) >= field_EC_rnd1)
    {
        const s32 v6 = field_F0_rnd2 - field_EC_rnd1;
        if (static_cast<s32>(gnFrameCount_507670) == v6 / 2)
        {
            field_F4_rnd3 = Math_RandomRange_450F20(96, 128);
        }

        const FP v9 = (FP_FromInteger(128) * FP_FromInteger(gnFrameCount_507670 - field_EC_rnd1) / FP_FromInteger(v6));
        const FP v11 = -Math_Cosine_4510A0(FP_GetExponent(v9) & 0xFF);

        s32 tmp = field_F4_rnd3 + FP_GetExponent(FP_FromInteger(field_F8_rnd4) * v11);

        u8 rgb = 0;
        if (tmp <= 255)
        {
            rgb = tmp & 0xFF;
        }
        else
        {
            rgb = 255;
        }

        mRed = rgb;
        mGreen = rgb;
        mBlue = rgb;
    }
    else if (static_cast<s32>(gnFrameCount_507670) > field_F0_rnd2)
    {
        field_EC_rnd1 = gnFrameCount_507670 + Math_RandomRange_450F20(2, 8);
        field_F0_rnd2 = field_EC_rnd1 + Math_RandomRange_450F20(4, 8);
        field_F8_rnd4 = Math_RandomRange_450F20(150, 180);
        u8 rgb = 96;

        mRed = rgb;
        mGreen = rgb;
        mBlue = rgb;
    }
}

} // namespace AO
