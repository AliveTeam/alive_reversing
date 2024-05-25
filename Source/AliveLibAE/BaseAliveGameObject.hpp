#pragma once

#include "../relive_lib/GameObjects/IBaseAliveGameObject.hpp"

namespace relive {
class Path_TLV;
}

class PathLine;

enum class CameraPos : s16;
class PlatformBase;
class BirdPortal;


class BaseAliveGameObject : public IBaseAliveGameObject
{
public:
    explicit BaseAliveGameObject(s16 resourceArraySize);
    ~BaseAliveGameObject();

    virtual void VOnPathTransition(s32 camWorldX, s32 camWorldY, CameraPos direction) override;
    virtual void VCheckCollisionLineStillValid(s32 distance);

    virtual BirdPortal* VIntoBirdPortal(s16 gridBlocks);
    bool MapFollowMe(bool snapToGrid);

    virtual bool VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override;

protected:

    bool InAirCollision(PathLine** ppLine, FP* hitX, FP* hitY, FP velY);
    BaseGameObject* FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos);

    BaseAliveGameObject* GetStackedSlapTarget(const Guid& idToFind, ReliveTypes typeToFind, FP xpos, FP ypos); // AE
public:
    s16 BaseAliveGameObjectCollisionLineType = 0; // AE only, quick save data
};
