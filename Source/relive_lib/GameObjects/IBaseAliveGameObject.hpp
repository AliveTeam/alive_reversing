#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class PathLine;

enum class eMotionType
{
    ePreviousMotion,
    eCurrentMotion,
    eNextMotion
};

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

    // Note: this is only used for the GameAutoPlayer
    virtual s16 VGetMotion(eMotionType /*motionType*/)
    {
        return 0;
    }

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
    Scale PerGameScale();
    bool WallHit(FP offY, FP offX);
    bool Check_IsOnEndOfLine(s16 direction, s16 distance);

    template <class BaseAliveGameObjectClass, class FnArray, class BrainState>
    static auto InvokeMemberFunction(BaseAliveGameObjectClass caller, const FnArray& fnArray, BrainState brainState)
    {
        auto function = fnArray[static_cast<u32>(brainState)];
        return (caller->*function)();
    }

    static bool IsInInvisibleZone(IBaseAliveGameObject* pObj);

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
    bool GetElectrocuting() const { return mElectrocuting; }
    void SetElectrocuting(bool val) { mElectrocuting = val; }
    bool GetCanBeesChase() const { return mCanBeesChase; }
    void SetCanBeesChase(bool val) { mCanBeesChase = val; }

    FP mHealth = FP_FromInteger(1);
    PathLine* BaseAliveGameObjectCollisionLine = nullptr;
    FP BaseAliveGameObjectLastLineYPos = {};
    bool mbGotShot = false;
    FP_RECT mCollectionRect = {};
    s32 mBaseAliveGameObjectLastAnimFrame = 0;
    relive::Path_TLV* BaseAliveGameObjectPathTLV = nullptr;
    Guid BaseAliveGameObject_PlatformId = Guid{};

protected:
    s16 SetBaseAnimPaletteTint(const TintEntry* pTintArray, EReliveLevelIds level_id, PalId resourceID);

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
extern IBaseAliveGameObject* sControlledCharacter;