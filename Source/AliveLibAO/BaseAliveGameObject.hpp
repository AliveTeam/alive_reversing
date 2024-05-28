#pragma once

#include "../relive_lib/GameObjects/IBaseAliveGameObject.hpp"

class PathLine;

enum class CameraPos : s16;

namespace AO {

class PlatformBase;
class BirdPortal;

class BaseAliveGameObject : public IBaseAliveGameObject
{
public:
    explicit BaseAliveGameObject(s16 resourceArraySize);
    ~BaseAliveGameObject();

    virtual void VOnPathTransition(s32 camWorldX, s32 camWorldY, CameraPos direction) override;
    virtual void VCheckCollisionLineStillValid(s32 distance);

    virtual void VSetXSpawn(s16 camWorldX, s32 screenXPos) override; // AO
    virtual void VSetYSpawn(s32 camWorldY, s16 bLeft) override; // AO

    bool MapFollowMe(bool snapToGrid);

    virtual bool VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override;

protected:

    bool InAirCollision(PathLine** ppLine, FP* hitX, FP* hitY, FP velY);
    BaseGameObject* FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos);

    void UsePathTransScale(); // AO
};

} // namespace AO
