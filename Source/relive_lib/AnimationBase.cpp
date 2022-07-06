#include "AnimationBase.hpp"

DynamicArrayT<AnimationBase>* AnimationBase::gAnimations;

void AnimationBase::CreateAnimationArray()
{
    gAnimations = relive_new DynamicArrayT<AnimationBase>(80);
}

void AnimationBase::FreeAnimationArray()
{
    relive_delete gAnimations;
    gAnimations = nullptr;
}

void AnimationBase::AnimateAll(DynamicArrayT<AnimationBase>* pAnims)
{
    for (auto i = 0; i < pAnims->Size(); i++)
    {
        AnimationBase* pAnim = pAnims->ItemAt(i);
        if (!pAnim)
        {
            break;
        }

        if (pAnim->mAnimFlags.Get(AnimFlags::eBit2_Animate))
        {
            if (pAnim->mFrameChangeCounter > 0)
            {
                pAnim->mFrameChangeCounter--;
                if (pAnim->mFrameChangeCounter == 0)
                {
                    pAnim->VDecode();
                }
            }
        }
    }
}
