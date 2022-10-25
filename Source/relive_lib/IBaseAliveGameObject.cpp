#include "IBaseAliveGameObject.hpp"
#include "GameType.hpp"

void IBaseAliveGameObject::VUnPosses()
{
    // Empty
}

void IBaseAliveGameObject::VPossessed()
{
    // Empty
}

s16 IBaseAliveGameObject::VTakeDamage(BaseGameObject* /*pFrom*/)
{
    // Defaults to no damage.
    return 0;
}

void IBaseAliveGameObject::VOnTlvCollision(relive::Path_TLV* /*pTlv*/)
{
    // Empty
}

void IBaseAliveGameObject::VOnTrapDoorOpen()
{
    // Empty
}

void IBaseAliveGameObject::VSetMotion(s16 state)
{
    mbMotionChanged = true;
    mCurrentMotion = state;
}

void IBaseAliveGameObject::OnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList)
{
    if (pObjList)
    {
        for (s32 i = 0; i < pObjList->Size(); i++)
        {
            BaseGameObject* pObjIter = pObjList->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5))
            {
                if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
                {
                    BaseAnimatedWithPhysicsGameObject* pObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObjIter);
                    const PSX_RECT bRect = pObj->VGetBoundingRect();
                    if (xy.x <= bRect.w && wh.x >= bRect.x && wh.y >= bRect.y && xy.y <= bRect.h)
                    {
                        // NOTE: AO ignored scale here
                        if (GetGameType() == GameType::eAo || (GetGameType() == GameType::eAe && GetScale() == pObj->GetScale()))
                        {
                            if (!VOnPlatformIntersection(pObj))
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

s16 IBaseAliveGameObject::SetBaseAnimPaletteTint(const TintEntry* pTintArray, EReliveLevelIds level_id, PalId palId)
{
    SetTint(pTintArray, level_id); // Actually bugged for inputs that never happen as it should return 0

    if (palId != PalId::Default)
    {
        PalResource res = ResourceManagerWrapper::LoadPal(palId);
        GetAnimation().LoadPal(res);
    }
    return 1;
}
