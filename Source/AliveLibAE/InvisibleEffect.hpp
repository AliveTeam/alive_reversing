#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"

class BaseAliveGameObject;

class InvisibleEffect final : public BaseGameObject
{
public:
    enum class InvisibleState : u16
    {
        eSetRenderMode1_0 = 0,
        eSetInvisibile_1 = 1,
        eBecomeInvisible_2 = 2,
        eUnknown_3 = 3,
        eBecomeVisible_4 = 4,
        eClearInvisibility_5 = 5,
        eSetDead_6 = 6
    };

    explicit InvisibleEffect(BaseAliveGameObject* pTarget);
    ~InvisibleEffect();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void InstantInvisibility();
    void BecomeVisible();
    void ClearInvisibility();
    void BecomeInvisible();

private:
    InvisibleState mState = InvisibleState::eSetRenderMode1_0;

    //u16* field_24_pPal1 = nullptr;
    //PSX_RECT field_28_pal_rect1 = {};
    //u16* field_30_pPal2 = nullptr;
    //PSX_RECT field_34_pal_rect2 = {};
    PalResource mPal1;
    PalResource mPal2;

    u8 mTransitionFrameCount = 0;

    Guid mTargetId;
    TPageAbr mOldRenderMode = TPageAbr::eBlend_0;
    bool mSemiTrans = false;
    bool mBlending = false;
    bool mIsInvisible = false;
};
