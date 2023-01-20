#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class PathLine;

// Temp glue interface to make BaseAliveGameObject common piece by piece
class IBaseAliveGameObject : public BaseAnimatedWithPhysicsGameObject
{
public:
    using BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject;
    
    ~IBaseAliveGameObject();


    virtual void VUnPosses();
    virtual void VPossessed();
    virtual bool VTakeDamage(BaseGameObject* pFrom);
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
    virtual bool VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) = 0;

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

    bool GetCanBePossessed() const { return mCanBePossessed; }
    void SetCanBePossessed(bool val) { mCanBePossessed = val; }
    bool GetPossessed() const { return mPossessed; }
    void SetPossessed(bool val) { mPossessed = val; }
    bool GetZappedByShrykull() const { return mZappedByShrykull; }
    void SetZappedByShrykull(bool val) { mZappedByShrykull = val; }
    bool GetCanSetOffExplosives() const { return mCanSetOffExplosives; }
    void SetCanSetOffExplosives(bool val) { mCanSetOffExplosives = val; }
    bool GetElectrocuted() const { return mElectrocuted; }
    void SetElectrocuted(bool val) { mElectrocuted = val; }
    bool GetInvisible() const { return mInvisible; }
    void SetInvisible(bool val) { mInvisible = val; }
    bool GetRestoredFromQuickSave() const { return mRestoredFromQuickSave; }
    void SetRestoredFromQuickSave(bool val) { mRestoredFromQuickSave = val; }
    bool GetTeleporting() const { return mTeleporting; }
    void SetTeleporting(bool val) { mTeleporting = val; }
    const bool GetElectrocuting() const { return mElectrocuting; }
    void SetElectrocuting(bool val) { mElectrocuting = val; }
    bool GetCanBeesChase() const { return mCanBeesChase; }
    void SetCanBeesChase(bool val) { mCanBeesChase = val; }
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
    FP mHealth = FP_FromInteger(1);

    bool mbGotShot = false;
    bool mbMotionChanged = false;
    Guid BaseAliveGameObject_PlatformId = Guid{};

private:
    bool mCanBePossessed = false;
    bool mPossessed = false;
    bool mZappedByShrykull = false;
    bool mCanSetOffExplosives = false;
    bool mElectrocuted = false;
    bool mInvisible = false;
    bool mRestoredFromQuickSave = false;
    bool mTeleporting = false;
    bool mElectrocuting = false;
    bool mCanBeesChase = false;      // AO only: can the bees attack - can be above the value 1 but bee swarm only checks for non zero

};

extern DynamicArrayT<IBaseAliveGameObject>* gBaseAliveGameObjects;
