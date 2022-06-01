#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "DynamicArray.hpp"
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

protected:
    s16 SetBaseAnimPaletteTint(TintEntry* pTintArray, LevelIds level_id, s32 resourceID);

    Bool32 Check_IsOnEndOfLine(s16 direction, s16 distance);

    BaseAliveGameObject* GetStackedSlapTarget(s32 idToFind, AETypes typeToFind, FP xpos, FP ypos);


public:
    void SetActiveCameraDelayedFromDir();

public:
    s16 MapFollowMe(s16 snapToGrid);

protected:
    Bool32 WallHit_408750(FP offY, FP offX);
    Bool32 InAirCollision_408810(PathLine** ppPathLine, FP* hitX, FP* hitY, FP velY);
    BaseGameObject* FindObjectOfType_425180(AETypes typeToFind, FP xpos, FP ypos);

public:
    s16 OnTrapDoorIntersection(PlatformBase* pOther);

public:
    s16 field_F4_previous_motion;
    s16 field_F6_anim_frame;
    FP field_F8_LastLineYPos;
    Path_TLV* field_FC_pPathTLV;
    PathLine* field_100_pCollisionLine;
    s16 field_104_collision_line_type;
    s16 field_106_current_motion;
    s16 field_108_next_motion;
    s16 field_10A_unused;
    FP field_10C_health;
    s32 field_110_id;
    BitField16<Flags_114> field_114_flags;
    s16 field_116_padding;
};
ALIVE_ASSERT_SIZEOF(BaseAliveGameObject, 0x118);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_5C1B7C);
