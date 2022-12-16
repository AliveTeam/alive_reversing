#pragma once

#include "../relive_lib/AnimationBase.hpp"
#include "../relive_lib/Primitives_common.hpp"

enum class AnimId;
struct AnimRecord;

const AnimRecord PerGameAnimRec(AnimId id);
class BaseGameObject;

using TFrameCallBackType = void(*)(BaseGameObject*, u32& pointIdx, const IndexedPoint&);

class Animation final : public AnimationBase
{
public:
    virtual void VDecode() override;
    virtual void VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s32 height) override;
    virtual void VCleanUp() override;

    s16 Set_Animation_Data(AnimResource& pAnimRes);
    s16 Init(const AnimResource& ppAnimData, BaseGameObject* pGameObj);

    void SetFrame(s32 newFrame);
    u32 Get_Frame_Count();
    const PerFrameInfo* Get_FrameHeader(s32 frame);
    void LoadPal(std::shared_ptr<AnimationPal>& pal);
    void LoadPal(const PalResource& pal);
    void ReloadPal();
    void Get_Frame_Rect(PSX_RECT* pRect);
    void Get_Frame_Width_Height(s16* pWidth, s16* pHeight);
    void Get_Frame_Offset(s16* pBoundingX, s16* pBoundingY);
    void Invoke_CallBacks();
    bool DecodeCommon();
    void DecompressFrame();

    u32 GetFrameDelay() const { return mFrameDelay; }
    void SetFrameDelay(u32 val) { mFrameDelay = val; }
    s32 GetCurrentFrame() const { return mCurrentFrame; }
    void SetCurrentFrame(s32 val) { mCurrentFrame = val; }
    TFrameCallBackType* GetFnPtrArray() const { return mFnPtrArray; }
    void SetFnPtrArray(TFrameCallBackType* val) { mFnPtrArray = val; }
    FP GetSpriteScale() const { return mSpriteScale; }
    void SetSpriteScale(FP val) { mSpriteScale = val; }

    Poly_FT4 mOtData[2] = {};

private:
    u32 mFrameDelay = 0;
    s32 mCurrentFrame = 0;
    TFrameCallBackType* mFnPtrArray = nullptr;
    FP mSpriteScale = {};
    BaseGameObject* mGameObj = nullptr;

};

inline bool IsLastFrame(const Animation* pAnim)
{
    return pAnim->GetCurrentFrame() == static_cast<s32>(pAnim->mAnimRes.mJsonPtr->mFrames.size() - 1);
}
