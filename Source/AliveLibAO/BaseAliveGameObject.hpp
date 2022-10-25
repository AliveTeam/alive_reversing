#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/BitField.hpp"
#include "../AliveLibAE/Psx.hpp"
#include "../relive_lib/IBaseAliveGameObject.hpp"

class PathLine;

enum class CameraPos : s16;

namespace AO {

class PlatformBase;
class BirdPortal;


enum Flags_10A
{
    e10A_Bit1_Can_Be_Possessed = 0x1,
    e10A_Bit2_bPossesed = 0x2,
    e10A_Bit3 = 0x4,
    e10A_Bit4_SetOffExplosives = 0x8,
    e10A_Bit5_Electrocuted = 0x10,
    e10A_Bit6 = 0x20,
};

class BaseAliveGameObject : public IBaseAliveGameObject
{
public:
    BaseAliveGameObject();
    ~BaseAliveGameObject();

    virtual void VSetMotion(s16 state);
    virtual void VOnPathTransition(s16 camWorldX, s32 camWorldY, CameraPos direction);
    virtual void VCheckCollisionLineStillValid(s32 distance);

    virtual void VSetXSpawn(s16 camWorldX, s32 screenXPos);
    virtual void VSetYSpawn(s32 camWorldY, s16 bLeft);
    BirdPortal* IntoBirdPortal_402350(s16 distance);

    void SetActiveCameraDelayedFromDir();
    s16 MapFollowMe(s16 snapToGrid);

    virtual s16 VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override;

protected:

    bool Check_IsOnEndOfLine(s16 direction, s16 distance);
    s16 WallHit(FP offY, FP offX);
    bool InAirCollision(PathLine** ppLine, FP* hitX, FP* hitY, FP vely);
    static BaseGameObject* FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos);

    void UsePathTransScale_4020D0();
    static void OnResourceLoaded_4019A0(BaseAliveGameObject* ppRes);

public:
  

public:
    s16 field_EC_bBeesCanChase = 0; // AO only: can the bees attack - can be above the value 1 but bee swarm only checks for non zero
    PlatformBase* mLiftPoint = nullptr; // AO only
    s16 field_106_shot = 0; // AE as flag
    s16 field_108_bMotionChanged = 0; // AE as flag
    BitField16<Flags_10A> mBaseAliveGameObjectFlags = {};
};

extern DynamicArrayT<BaseAliveGameObject>* gBaseAliveGameObjects;

} // namespace AO
