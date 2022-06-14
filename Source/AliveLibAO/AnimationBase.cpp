#include "AnimationBase.hpp"

namespace AO {
void AnimationBase::AnimateAll(DynamicArrayT<AnimationBase>* pAnimList)
{
    for (s32 i = 0; i < pAnimList->Size(); i++)
    {
        auto pAnim = pAnimList->ItemAt(i);
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
} // namespace AO
