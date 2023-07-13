#pragma once

#include "Primitives.hpp"
#include "Layer.hpp"
#include "ResourceManagerWrapper.hpp"
#include "DynamicArray.hpp"
#include "RGB16.hpp"

class OrderingTable;

class AnimationBase
{
public:
    static void CreateAnimationArray();
    static void FreeAnimationArray();

    virtual ~AnimationBase() { }

    virtual void VDecode() = 0;

    virtual void VRender(s32 xpos, s32 ypos, OrderingTable& ot, s16 width, s32 height) = 0;

    virtual void VCleanUp() = 0;

    static void AnimateAll(DynamicArrayT<AnimationBase>* pAnimations);

    AnimResource mAnimRes;

    bool GetIgnorePosOffset() const { return mIgnorePosOffset; }
    void SetIgnorePosOffset(bool val) { mIgnorePosOffset = val; }
    bool GetLoopBackwards() const { return mLoopBackwards; }
    void SetLoopBackwards(bool val) { mLoopBackwards = val; }
    bool GetIsLastFrame() const { return mIsLastFrame; }
    void SetIsLastFrame(bool val) { mIsLastFrame = val; }
    bool GetBlending() const { return mBlending; }
    void SetBlending(bool val) { mBlending = val; }
    bool GetSemiTrans() const { return mSemiTrans; }
    void SetSemiTrans(bool val) { mSemiTrans = val; }
    bool GetForwardLoopCompleted() const { return mForwardLoopCompleted; }
    void SetForwardLoopCompleted(bool val) { mForwardLoopCompleted = val; }
    bool GetLoop() const { return mLoop; }
    void SetLoop(bool val) { mLoop = val; }
    bool GetSwapXY() const { return mSwapXY; }
    void SetSwapXY(bool val) { mSwapXY = val; }
    bool GetFlipY() const { return mFlipY; }
    void SetFlipY(bool val) { mFlipY = val; }
    void ToggleFlipX() { mFlipX = !mFlipX; }
    bool GetFlipX() const { return mFlipX; }
    void SetFlipX(bool val) { mFlipX = val; }
    bool GetRender() const { return mRender; }
    void SetRender(bool val) { mRender = val; }
    bool GetAnimate() const { return mAnimate; }
    void SetAnimate(bool val) { mAnimate = val; }
    const relive::TBlendModes& GetBlendMode() const { return mBlendMode; }
    void SetBlendMode(relive::TBlendModes val) { mBlendMode = val; }
    const Layer& GetRenderLayer() const { return mRenderLayer; }
    void SetRenderLayer(Layer val) { mRenderLayer = val; }
    u32 GetFrameChangeCounter() const { return mFrameChangeCounter; }
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
    relive::TBlendModes mBlendMode = relive::TBlendModes::eBlend_0;
    Layer mRenderLayer = Layer::eLayer_0;
    u32 mFrameChangeCounter = 0;

protected:
    RGB16 mRgb;

};
