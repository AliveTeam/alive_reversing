#include "AnimationBase.hpp"
#include "../AliveLibCommon/Function.hpp"

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

        if (pAnim->mFlags.Get(AnimFlags::eAnimate))
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
