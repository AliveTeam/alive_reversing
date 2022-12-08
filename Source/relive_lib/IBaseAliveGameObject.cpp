#include "IBaseAliveGameObject.hpp"
#include "GameType.hpp"
#include "ObjectIds.hpp"

DynamicArrayT<IBaseAliveGameObject>* gBaseAliveGameObjects = nullptr;

// TODO: Remove after abe.cpp merge
extern IBaseAliveGameObject* sControlledCharacter;


IBaseAliveGameObject::~IBaseAliveGameObject()
{
    IBaseAliveGameObject* pLiftPoint = static_cast<IBaseAliveGameObject*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    gBaseAliveGameObjects->Remove_Item(this);

    if (pLiftPoint)
    {
        pLiftPoint->VOnTrapDoorOpen();
        BaseAliveGameObject_PlatformId = Guid{};
    }
}

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

            if (pObjIter->GetIsBaseAnimatedWithPhysicsObj())
            {
                if (pObjIter->GetDrawable())
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
    // NOTE: in AO's SetBaseAnimPaletteTint() function we don't want to set the RGB values
    // when we hit the end of the tint array
    if (GetGameType() == GameType::eAe)
    {
        SetTint(pTintArray, level_id);
    }
    else
    {
        const TintEntry* pIter = pTintArray;
        while (pIter->field_0_level != level_id)
        {
            if (pIter->field_0_level == EReliveLevelIds::eNone) // End of entries
            {
                return 0;
            }
            pIter++;
        }

        mRGB.SetRGB(pIter->field_1_r, pIter->field_2_g, pIter->field_3_b);
    }

    if (palId != PalId::Default)
    {
        PalResource res = ResourceManagerWrapper::LoadPal(palId);
        GetAnimation().LoadPal(res);
    }
    return 1;
}


void IBaseAliveGameObject::SetActiveCameraDelayedFromDir()
{
    if (sControlledCharacter == this)
    {
        switch (Is_In_Current_Camera())
        {
            case CameraPos::eCamTop_1:
                if (mVelY < FP_FromInteger(0))
                {
                    GetMap().SetActiveCameraDelayed(MapDirections::eMapTop_2, this, -1);
                }
                break;

            case CameraPos::eCamBottom_2:
                if (mVelY > FP_FromInteger(0))
                {
                    GetMap().SetActiveCameraDelayed(MapDirections::eMapBottom_3, this, -1);
                }
                break;

            case CameraPos::eCamLeft_3:
                if (mVelX < FP_FromInteger(0))
                {
                    GetMap().SetActiveCameraDelayed(MapDirections::eMapLeft_0, this, -1);
                }
                break;

            case CameraPos::eCamRight_4:
                if (mVelX > FP_FromInteger(0))
                {
                    GetMap().SetActiveCameraDelayed(MapDirections::eMapRight_1, this, -1);
                }
                break;

            case CameraPos::eCamCurrent_0:
            case CameraPos::eCamNone_5:
                return;

            default:
                return;
        }
    }
}
