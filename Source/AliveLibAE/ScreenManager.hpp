#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Psx.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"

struct Prim_Sprt;

struct DirtyBits final
{
    u16 mData[20]; // 20 Columns

    bool GetTile(s32 x, s32 y) const
    {
        return mData[x] & (1 << y) ? true : false;
    }

    void SetTile(s32 x, s32 y, bool b)
    {
        if (b)
        {
            mData[x] |= 1 << y;
        }
        else
        {
            mData[x] &= ~(1 << y);
        }
    }
};

struct SprtTPage final
{
    Prim_Sprt mSprt;
    Prim_SetTPage mTPage;
};

class ScreenManager final : public BaseGameObject
{
public:
    // TODO
    void sub_40EE10();

    void MoveImage();

    void InvalidateRect(s32 x, s32 y, s32 width, s32 height, s32 idx);
    void InvalidateRectCurrentIdx(s32 x, s32 y, s32 width, s32 height);
    void InvalidateRect_Layer3(s32 x, s32 y, s32 width, s32 height);
    void InvalidateRect_IdxPlus4(s32 x, s32 y, s32 width, s32 height, s32 idx);

    s16 IsDirty(s32 idx, s32 x, s32 y);
    void UnsetDirtyBits(s32 idx);
    void UnsetDirtyBits_FG1();

    virtual void VUpdate() override;


    void DecompressCameraToVRam(u16** ppBits);

    ScreenManager(u8** ppBits, FP_Point* pCameraOffset);

    void Init(u8** ppBits);

    static s32 GetTPage(TPageMode tp, TPageAbr abr, s32* xpos, s32* ypos);

    virtual void VRender(PrimHeader** ppOt) override;
    void Render_Helper_40E9F0(s32 xpos, s32 ypos, Layer idx, s32 sprite_idx, PrimHeader** ppOt);
    void sub_40EE50();

    virtual void VScreenChanged() override;

    FP CamXPos() const
    {
        return mCamPos->x - FP_FromInteger(mCamXOff);
    }

    FP CamYPos() const
    {
        return mCamPos->y - FP_FromInteger(mCamYOff);
    }

    void DisableRendering()
    {
        mRenderingDisabled = true;
    }

    void EnableRendering()
    {
        mRenderingDisabled = false;
    }

    s32 Idx() const
    {
        return mIdx;
    }

private:
    void AddCurrentSPRT_TPage(PrimHeader** ppOt);

public:
    FP_Point* mCamPos = nullptr;
    s16 mCamXOff = 0;
    u16 mCamYOff = 0;

private:
    SprtTPage* mScreenSprites = nullptr;
    u16 mUPos = 0;
    u16 mVPos = 0;
    s16 mCamWidth = 0;
    s16 mCamHeight = 0;
    u16 mIdx = 0;
    u16 mYIdx = 0;
    u16 mXIdx = 0;
    bool mRenderingDisabled = false;
    DirtyBits mDirtyBits[8] = {};
};
ALIVE_ASSERT_SIZEOF(ScreenManager, 0x1A4u);

ALIVE_VAR_EXTERN(ScreenManager*, pScreenManager);
