#pragma once

#include "Primitives.hpp"
#include "Layer.hpp"
#include "ResourceManagerWrapper.hpp"
#include "DynamicArray.hpp"
#include "../AliveLibCommon/RGB16.hpp"

class AnimationBase
{
public:
    static void CreateAnimationArray();
    static void FreeAnimationArray();

    virtual ~AnimationBase() { }

    virtual void VDecode() = 0;

    virtual void VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s32 height) = 0;

    virtual void VCleanUp() = 0;

    static void AnimateAll(DynamicArrayT<AnimationBase>* pAnimations);

    AnimResource mAnimRes;

    const bool& GetIgnorePosOffset() const { return mIgnorePosOffset; }
    void SetIgnorePosOffset(bool val) { mIgnorePosOffset = val; }
    const bool& GetLoopBackwards() const { return mLoopBackwards; }
    void SetLoopBackwards(bool val) { mLoopBackwards = val; }
    const bool& GetIsLastFrame() const { return mIsLastFrame; }
    void SetIsLastFrame(bool val) { mIsLastFrame = val; }
    const bool& GetBlending() const { return mBlending; }
    void SetBlending(bool val) { mBlending = val; }
    const bool& GetSemiTrans() const { return mSemiTrans; }
    void SetSemiTrans(bool val) { mSemiTrans = val; }
    const bool& GetForwardLoopCompleted() const { return mForwardLoopCompleted; }
    void SetForwardLoopCompleted(bool val) { mForwardLoopCompleted = val; }
    const bool& GetLoop() const { return mLoop; }
    void SetLoop(bool val) { mLoop = val; }
    const bool& GetSwapXY() const { return mSwapXY; }
    void SetSwapXY(bool val) { mSwapXY = val; }
    const bool& GetFlipY() const { return mFlipY; }
    void SetFlipY(bool val) { mFlipY = val; }
    void ToggleFlipX() { mFlipX = !mFlipX; }
    const bool& GetFlipX() const { return mFlipX; }
    void SetFlipX(bool val) { mFlipX = val; }
    const bool& GetRender() const { return mRender; }
    void SetRender(bool val) { mRender = val; }
    const bool& GetAnimate() const { return mAnimate; }
    void SetAnimate(bool val) { mAnimate = val; }
    const TPageAbr& GetRenderMode() const { return mRenderMode; }
    void SetRenderMode(TPageAbr val) { mRenderMode = val; }
    const Layer& GetRenderLayer() const { return mRenderLayer; }
    void SetRenderLayer(Layer val) { mRenderLayer = val; }
    const u32& GetFrameChangeCounter() const { return mFrameChangeCounter; }
    void SetFrameChangeCounter(u32 val) { mFrameChangeCounter = val; }
    void SetRGB(s16 rValue, s16 gValue, s16 bValue) { mRgb.SetRGB(rValue, gValue, bValue); }
    void SetRGB(const RGB16& rgb) { mRgb = rgb; }
    const RGB16& GetRgb() const { return mRgb; }
    RGB16& GetRgb() { return mRgb; }

    static DynamicArrayT<AnimationBase>* gAnimations;

private:
    bool mAnimate = false;
    bool mRender = false;
    bool mFlipX = false;
    bool mFlipY = false;
    bool mSwapXY = false;
    bool mLoop = false;
    bool mForwardLoopCompleted = false;
    bool mSemiTrans = false;
    bool mBlending = false;
    bool mIsLastFrame = false;
    bool mLoopBackwards = false;
    bool mIgnorePosOffset = false;
    TPageAbr mRenderMode = TPageAbr::eBlend_0;
    Layer mRenderLayer = Layer::eLayer_0;
    u32 mFrameChangeCounter = 0;

protected:
    RGB16 mRgb;

};
