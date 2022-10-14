#include "stdafx_ao.h"
#include "LightEffect.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

LightEffect::LightEffect(relive::Path_LightEffect* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eNone);
    field_E4_tlvInfo = tlvId;

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Star));
    Animation_Init(GetAnimRes(AnimId::Star));

    field_EC_rnd1 = sGnFrame + Math_RandomRange(2, 8);
    field_F0_rnd2 = field_EC_rnd1 + Math_RandomRange(4, 8);
    field_F4_rnd3 = Math_RandomRange(96, 128);
    field_F8_rnd4 = Math_RandomRange(190, 255);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    GetAnimation().mFlags.Set(AnimFlags::eIgnorePosOffset);
    GetAnimation().mFlags.Set(AnimFlags::eSemiTrans);
    GetAnimation().mFlags.Set(AnimFlags::eBlending);

    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_0);
    GetAnimation().SetFrame(1);

    // maybe width height like in door effect
    field_E8 = 0;
    field_EA = 0;

    SetSpriteScale(FP_FromDouble(0.4));
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
}

LightEffect::~LightEffect()
{
    Path::TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
}

void LightEffect::VRender(PrimHeader** /*ppOt*/)
{
    // Pretty much the same as door effect render - commented out because OG is empty which means the "stock yard stars" are missing.
    // However rendering as-is produces over bright ugly looking stars and is probably why DD turned it off
    /*
    if (gNumCamSwappers == 0)
    {
        const FP xpos = FP_FromInteger(pScreenManager->mCamXOff) + mXPos - pScreenManager->mCamPos->x;
        const FP ypos = FP_FromInteger(pScreenManager->mCamYOff) + mYPos - pScreenManager->mCamPos->y;

        mAnim.mRed = static_cast<u8>(mRGB.r/4);
        mAnim.mGreen = static_cast<u8>(mRGB.g/4);
        mAnim.mBlue = static_cast<u8>(mRGB.b/4);

        mAnim.vRender(
            FP_GetExponent(FP_FromInteger((FP_GetExponent(xpos)))),
            FP_GetExponent(FP_FromInteger((FP_GetExponent(ypos)))),
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        mAnim.Get_Frame_Rect(&rect);
        pScreenManager->InvalidateRect_406E40(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager->mIdx);
    }
    */
}

void LightEffect::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}


void LightEffect::VUpdate()
{
    if (static_cast<s32>(sGnFrame) >= field_EC_rnd1)
    {
        const s32 v6 = field_F0_rnd2 - field_EC_rnd1;
        if (static_cast<s32>(sGnFrame) == v6 / 2)
        {
            field_F4_rnd3 = Math_RandomRange(96, 128);
        }

        const FP v9 = (FP_FromInteger(128) * FP_FromInteger(sGnFrame - field_EC_rnd1) / FP_FromInteger(v6));
        const FP v11 = -Math_Cosine(FP_GetExponent(v9) & 0xFF);

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

        mRGB.SetRGB(rgb, rgb, rgb);
    }
    else if (static_cast<s32>(sGnFrame) > field_F0_rnd2)
    {
        field_EC_rnd1 = sGnFrame + Math_RandomRange(2, 8);
        field_F0_rnd2 = field_EC_rnd1 + Math_RandomRange(4, 8);
        field_F8_rnd4 = Math_RandomRange(150, 180);
        u8 rgb = 96;

        mRGB.SetRGB(rgb, rgb, rgb);
    }
}

} // namespace AO
