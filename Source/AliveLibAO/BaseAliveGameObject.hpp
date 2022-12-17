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
    BaseAliveGameObject();
    ~BaseAliveGameObject();

    virtual void VOnPathTransition(s32 camWorldX, s32 camWorldY, CameraPos direction) override;
    virtual void VCheckCollisionLineStillValid(s32 distance);

    virtual void VSetXSpawn(s16 camWorldX, s32 screenXPos) override;
    virtual void VSetYSpawn(s32 camWorldY, s16 bLeft) override;
    virtual BirdPortal* VIntoBirdPortal(s16 distance);

    bool MapFollowMe(bool snapToGrid);

    virtual bool VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override;

protected:

    bool Check_IsOnEndOfLine(s16 direction, s16 distance);
    bool WallHit(FP offY, FP offX);
    bool InAirCollision(PathLine** ppLine, FP* hitX, FP* hitY, FP vely);
    BaseGameObject* FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos);

    void UsePathTransScale();
    static void OnResourceLoaded(BaseAliveGameObject* ppRes);
};

} // namespace AO
