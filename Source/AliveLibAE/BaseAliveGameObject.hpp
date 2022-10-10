#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive {
class Path_TLV;
};

class PathLine;

class PlatformBase;
class BirdPortal;

enum class CameraPos : s16;

enum AliveObjectFlags
{
    eShot = 0x1,
    eMotionChanged = 0x2,
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

class BaseAliveGameObject : public ::BaseAnimatedWithPhysicsGameObject
{

public:
    explicit BaseAliveGameObject(s16 resourceArraySize);
    ~BaseAliveGameObject();

    virtual void VRender(PrimHeader** ppOt) override;

    virtual void VUnPosses();
    virtual void VPossessed();
    virtual void VSetMotion(s16 state);
    virtual void VOnPathTransition(s16 cameraWorldXPos, s16 cameraWorldYPos, CameraPos direction);
    virtual s16 VTakeDamage(BaseGameObject* pFrom);
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv);
    virtual void VCheckCollisionLineStillValid(s16 distance);
    virtual BirdPortal* VIntoBirdPortal(s16 gridBlocks);
    virtual void VOnTrapDoorOpen();

    static s16 IsInInvisibleZone(BaseAliveGameObject* pObj);
    void SetActiveCameraDelayedFromDir();
    s16 MapFollowMe(s16 snapToGrid);
    s16 OnTrapDoorIntersection(PlatformBase* pOther);

protected:
    template<class T>
    inline void SetCurrentMotion(T motion)
    {
        mCurrentMotion = static_cast<s16>(motion);
    }
    template<class T>
    inline void SetNextMotion(T motion)
    {
        mNextMotion = static_cast<s16>(motion);
    }
    template<class T>
    inline void SetPreviousMotion(T motion)
    {
        mPreviousMotion = static_cast<s16>(motion);
    }

    s16 SetBaseAnimPaletteTint(TintEntry* pTintArray, EReliveLevelIds level_id, PalId resourceID);
    bool Check_IsOnEndOfLine(s16 direction, s16 distance);
    BaseAliveGameObject* GetStackedSlapTarget(const Guid& idToFind, ReliveTypes typeToFind, FP xpos, FP ypos);
    bool WallHit(FP offY, FP offX);
    bool InAirCollision(PathLine** ppPathLine, FP* hitX, FP* hitY, FP velY);
    BaseGameObject* FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos);

public:
    FP_RECT mCollectionRect = {};
    s16 mPreviousMotion = 0;
    s32 mBaseAliveGameObjectLastAnimFrame = 0;
    FP BaseAliveGameObjectLastLineYPos = {};
    relive::Path_TLV* BaseAliveGameObjectPathTLV = nullptr;
    PathLine* BaseAliveGameObjectCollisionLine = nullptr;
    s16 BaseAliveGameObjectCollisionLineType = 0;
    s16 mCurrentMotion = 0;
    s16 mNextMotion = 0;
    FP mHealth = {};
    Guid BaseAliveGameObject_PlatformId;
    BitField16<AliveObjectFlags> mBaseAliveGameObjectFlags = {};
};
ALIVE_ASSERT_SIZEOF(BaseAliveGameObject, 0x118);

extern DynamicArrayT<BaseAliveGameObject>* gBaseAliveGameObjects;
