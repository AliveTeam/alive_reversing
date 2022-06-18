#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "Animation.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

struct Path_TLV;
class PathLine;

class PlatformBase;
class BirdPortal;

enum class CameraPos : s16;

enum Flags_114
{
    e114_Bit1_bShot = 0x1,
    e114_MotionChanged_Bit2 = 0x2,
    e114_Bit3_Can_Be_Possessed = 0x4,
    e114_Bit4_bPossesed = 0x8,
    e114_Bit5_ZappedByShrykull = 0x10,
    e114_Bit6_SetOffExplosives = 0x20,
    e114_Bit7_Electrocuted = 0x40,
    e114_Bit8_bInvisible = 0x80,
    e114_Bit9_RestoredFromQuickSave = 0x100,
    e114_Bit10_Teleporting = 0x200,
    e114_Bit11_Electrocuting = 0x400,
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
    virtual void VOn_TLV_Collision(Path_TLV* pTlv);
    virtual void VCheckCollisionLineStillValid(s16 distance);
    virtual BirdPortal* VIntoBirdPortal(s16 gridBlocks);
    virtual void VOnTrapDoorOpen();

    static s16 IsInInvisibleZone(BaseAliveGameObject* pObj);
    void SetActiveCameraDelayedFromDir();
    s16 MapFollowMe(s16 snapToGrid);
    s16 OnTrapDoorIntersection(PlatformBase* pOther);

protected:
    s16 SetBaseAnimPaletteTint(TintEntry* pTintArray, EReliveLevelIds level_id, s32 resourceID);
    Bool32 Check_IsOnEndOfLine(s16 direction, s16 distance);
    BaseAliveGameObject* GetStackedSlapTarget(s32 idToFind, ReliveTypes typeToFind, FP xpos, FP ypos);
    Bool32 WallHit(FP offY, FP offX);
    Bool32 InAirCollision(PathLine** ppPathLine, FP* hitX, FP* hitY, FP velY);
    BaseGameObject* FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos);

public:
    FP_RECT mCollectionRect = {};
    s16 mPreviousMotion = 0;
    s16 mBaseAliveGameObjectLastAnimFrame = 0;
    FP BaseAliveGameObjectLastLineYPos = {};
    Path_TLV* BaseAliveGameObjectPathTLV = nullptr;
    PathLine* BaseAliveGameObjectCollisionLine = nullptr;
    s16 BaseAliveGameObjectCollisionLineType = 0;
    s16 mCurrentMotion = 0;
    s16 mNextMotion = 0;
    s16 field_10A_unused = 0;
    FP mHealth = {};
    s32 BaseAliveGameObject_PlatformId = 0;
    BitField16<Flags_114> mBaseAliveGameObjectFlags = {};
    s16 field_116_padding;
};
ALIVE_ASSERT_SIZEOF(BaseAliveGameObject, 0x118);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_5C1B7C);
