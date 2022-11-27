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
    
    ~IBaseAliveGameObject();


    virtual void VUnPosses();
    virtual void VPossessed();
    virtual s16 VTakeDamage(BaseGameObject* pFrom);
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv);
    virtual void VOnTrapDoorOpen();
    virtual void VSetMotion(s16 state);

    void OnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList);

    // AO only currently
    virtual void VSetXSpawn(s16 /*camWorldX*/, s32 /*screenXPos*/)
    {

    }

    // AO only currently
    virtual void VSetYSpawn(s32 /*camWorldY*/, s16 /*bLeft*/)
    {

    }

    // TODO: Impl here after merge
    virtual void VOnPathTransition(s32 camWorldX, s32 camWorldY, CameraPos direction) = 0;
    virtual s16 VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) = 0;

    void SetActiveCameraDelayedFromDir();

    template <class T>
    inline void SetCurrentMotion(T motion)
    {
        mCurrentMotion = static_cast<s16>(motion);
    }

    template <class T>
    inline void SetPreviousMotion(T motion)
    {
        mPreviousMotion = static_cast<s16>(motion);
    }

protected:

    template <class T>
    inline void SetNextMotion(T motion)
    {
        mNextMotion = static_cast<s16>(motion);
    }


    s16 SetBaseAnimPaletteTint(const TintEntry* pTintArray, EReliveLevelIds level_id, PalId resourceID);

public:
    template <class BaseAliveGameObjectClass, class FnArray>
    static auto InvokeMemberFunction(BaseAliveGameObjectClass caller, const FnArray& fnArray, u32 idx)
    {
        auto function = fnArray[idx];
        return (caller->*function)();
    }

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
    Guid BaseAliveGameObject_PlatformId;
    s16 field_EC_bBeesCanChase = 0;      // AO only: can the bees attack - can be above the value 1 but bee swarm only checks for non zero
};

extern DynamicArrayT<IBaseAliveGameObject>* gBaseAliveGameObjects;
