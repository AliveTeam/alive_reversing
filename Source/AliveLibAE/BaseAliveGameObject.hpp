#pragma once

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

    virtual void VOnPathTransition(s32 cameraWorldXPos, s32 cameraWorldYPos, CameraPos direction) override;
    virtual void VCheckCollisionLineStillValid(s32 distance);

    static bool IsInInvisibleZone(IBaseAliveGameObject* pObj);
    virtual BirdPortal* VIntoBirdPortal(s16 gridBlocks);
    bool MapFollowMe(bool snapToGrid);

    virtual bool VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override;

protected:

    bool Check_IsOnEndOfLine(s16 direction, s16 distance);
    bool WallHit(FP offY, FP offX);
    bool InAirCollision(PathLine** ppPathLine, FP* hitX, FP* hitY, FP velY);
    BaseGameObject* FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos);

    BaseAliveGameObject* GetStackedSlapTarget(const Guid& idToFind, ReliveTypes typeToFind, FP xpos, FP ypos);
public:
    s16 BaseAliveGameObjectCollisionLineType = 0; // AE only, quick save data
};
