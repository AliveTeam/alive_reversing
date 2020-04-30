#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "Animation.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

// TODO: Move to own file
EXPORT int CC SnapToXGrid_449930(FP scale, int x);

struct Path_TLV;
struct PathLine;

class PlatformBase;
class BirdPortal;

enum class CameraPos : __int16;

enum Flags_114
{
    e114_Bit1_bShot = 0x1,
    e114_MotionChanged_Bit2 = 0x2,
    e114_Bit3_Can_Be_Possessed = 0x4,
    e114_Bit4_bPossesed = 0x8,
    e114_Bit5 = 0x10,
    e114_Bit6_SetOffExplosives = 0x20,
    e114_Bit7_Electrocuted = 0x40,
    e114_Bit8_bInvisible = 0x80,
    e114_Bit9 = 0x100,
    e114_Bit10 = 0x200,
    e114_Bit11 = 0x400,
};

class BaseAliveGameObject : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BaseAliveGameObject* ctor_408240(short resourceArraySize);
    EXPORT void dtor_4080B0();

    BaseGameObject* vdtor_408210(signed int flags);

    virtual void VRender(int** pOrderingTable) override;
    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUnPosses_408F90();
    virtual void VPossessed_408F70();
    virtual void VSetMotion_4081C0(__int16 state);
    virtual void VOnPathTransition_408320(__int16 cameraWorldXPos, __int16 cameraWorldYPos, CameraPos direction);
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom);
    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv);
    virtual void VCheckCollisionLineStillValid_408A40(__int16 distance);
    virtual BirdPortal* VIntoBirdPortal_408FD0(__int16 gridBlocks);
    virtual void VOnTrapDoorOpen();
  
    EXPORT static __int16 CCSTD IsInInvisibleZone_425710(BaseAliveGameObject* pObj);
private:
    EXPORT void vUnPosses_408F90();
    EXPORT void vPossessed_408F70();
    EXPORT void vSetMotion_4081C0(__int16 state);
    EXPORT void vOnPathTransition_408320(__int16 cameraWorldXPos, __int16 cameraWorldYPos, CameraPos direction);
    EXPORT __int16 vTakeDamage_408730(BaseGameObject* pFrom);
    EXPORT void vOn_TLV_Collision_4087F0(Path_TLV* pTlv);
    EXPORT void vCheckCollisionLineStillValid_408A40(__int16 distance);
    EXPORT BirdPortal* vIntoBirdPortal_408FD0(__int16 gridBlocks);
    EXPORT void vOnTrapDoorOpen_4081F0();
protected:
    EXPORT signed __int16 SetBaseAnimPaletteTint_425690(TintEntry *pTintArray, LevelIds level_id, int resourceID);

    EXPORT BOOL Check_IsOnEndOfLine_408E90(__int16 direction, __int16 distance);

    EXPORT BaseAliveGameObject* GetStackedSlapTarget_425290(int idToFind, Types typeToFind, FP xpos, FP ypos);


public:
    EXPORT void SetActiveCameraDelayedFromDir_408C40();
public:
    EXPORT __int16 MapFollowMe_408D10(__int16 snapToGrid);
protected:
    EXPORT BOOL WallHit_408750(FP offY, FP offX);
    EXPORT BOOL InAirCollision_408810(PathLine **ppPathLine, FP* hitX, FP* hitY, FP velY);
    EXPORT BaseGameObject* FindObjectOfType_425180(Types typeToFind, FP xpos, FP ypos);
public:
    EXPORT __int16 OnTrapDoorIntersection_408BA0(PlatformBase* pOther);
public:
    __int16 field_F4;
    __int16 field_F6_anim_frame;
    FP field_F8_LastLineYPos;
    Path_TLV* field_FC_pPathTLV;
    PathLine* field_100_pCollisionLine;
    __int16 field_104_collision_line_type;
    __int16 field_106_current_motion;
    __int16 field_108_next_motion;
    __int16 field_10A;
    FP field_10C_health;
    int field_110_id;
    BitField16<Flags_114> field_114_flags;
    __int16 field_116_pad;
};
ALIVE_ASSERT_SIZEOF(BaseAliveGameObject, 0x118);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_5C1B7C);
