#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Psx_common.hpp"
#include "DynamicArray.hpp"

class PathLine;

enum AliveObjectFlags
{
    eCanBePossessed = 0x4,
    ePossessed = 0x8,
    eZappedByShrykull = 0x10,
    eCanSetOffExplosives = 0x20,
    eElectrocuted = 0x40,
    eInvisible = 0x80,
    eRestoredFromQuickSave = 0x100,
    eTeleporting = 0x200,
    eElectrocuting = 0x400,
};

// Temp glue interface to make BaseAliveGameObject common piece by piece
class IBaseAliveGameObject : public BaseAnimatedWithPhysicsGameObject
{
public:
    using BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject;
    
    virtual void VUnPosses();
    virtual void VPossessed();
    virtual s16 VTakeDamage(BaseGameObject* pFrom);
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv);
    virtual void VOnTrapDoorOpen();

    void OnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList);

    // TODO: Impl here after merge
    virtual s16 VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) = 0;

protected:
    template <class T>
    inline void SetCurrentMotion(T motion)
    {
        mCurrentMotion = static_cast<s16>(motion);
    }

    template <class T>
    inline void SetNextMotion(T motion)
    {
        mNextMotion = static_cast<s16>(motion);
    }

    template <class T>
    inline void SetPreviousMotion(T motion)
    {
        mPreviousMotion = static_cast<s16>(motion);
    }

    s16 SetBaseAnimPaletteTint(const TintEntry* pTintArray, EReliveLevelIds level_id, PalId resourceID);

public:
    FP_RECT mCollectionRect = {};
    s16 mPreviousMotion = 0;
    s32 mBaseAliveGameObjectLastAnimFrame = 0;
    FP BaseAliveGameObjectLastLineYPos = {};
    relive::Path_TLV* BaseAliveGameObjectPathTLV = nullptr;
    PathLine* BaseAliveGameObjectCollisionLine = nullptr;
    s16 mCurrentMotion = 0;
    s16 mNextMotion = 0;
    FP mHealth = {};

    bool mbGotShot = false;
    bool mbMotionChanged = false;
    BitField16<AliveObjectFlags> mBaseAliveGameObjectFlags = {};
};
