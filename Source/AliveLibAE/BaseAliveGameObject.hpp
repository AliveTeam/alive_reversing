#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/IBaseAliveGameObject.hpp"

namespace relive {
class Path_TLV;
}

class PathLine;

class PlatformBase;
class BirdPortal;

enum class CameraPos : s16;

class BaseAliveGameObject : public IBaseAliveGameObject
{

public:
    explicit BaseAliveGameObject(s16 resourceArraySize);
    ~BaseAliveGameObject();

    virtual void VOnPathTransition(s16 cameraWorldXPos, s16 cameraWorldYPos, CameraPos direction);
    virtual void VCheckCollisionLineStillValid(s32 distance);

    static s16 IsInInvisibleZone(BaseAliveGameObject* pObj);
    virtual BirdPortal* VIntoBirdPortal(s16 gridBlocks);
    void SetActiveCameraDelayedFromDir();
    s16 MapFollowMe(s16 snapToGrid);

    virtual s16 VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override;

protected:

    bool Check_IsOnEndOfLine(s16 direction, s16 distance);
    bool WallHit(FP offY, FP offX);
    bool InAirCollision(PathLine** ppPathLine, FP* hitX, FP* hitY, FP velY);
    BaseGameObject* FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos);

    BaseAliveGameObject* GetStackedSlapTarget(const Guid& idToFind, ReliveTypes typeToFind, FP xpos, FP ypos);
public:
    s16 BaseAliveGameObjectCollisionLineType = 0; // AE only, quick save data
    Guid BaseAliveGameObject_PlatformId; // AE only
};

extern DynamicArrayT<BaseAliveGameObject>* gBaseAliveGameObjects;
