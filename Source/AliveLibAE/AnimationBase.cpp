#include "stdafx.h"
#include "AnimationBase.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Sys_common.hpp"

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
