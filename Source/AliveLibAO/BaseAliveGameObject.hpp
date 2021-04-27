#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"
#include "BitField.hpp"
#include "Psx.hpp"

namespace AO {

void BaseAliveGameObject_ForceLink();

EXPORT s32 CC SnapToXGrid_41FAA0(FP scale, s32 a2);


EXPORT FP CC CamX_VoidSkipper_418590(FP xpos, FP xvel, s16 xMargin, u16* pResult);
EXPORT FP CC CamY_VoidSkipper_418690(FP ypos, FP yvel, s16 yMargin, u16* pResult);


struct Path_TLV;
class PlatformBase;
class PathLine;
class BirdPortal;

enum class CameraPos : s16;

enum Flags_10A
{
    e10A_Bit1_Can_Be_Possessed = 0x1,
    e10A_Bit2_bPossesed = 0x2,
    e10A_Bit3 = 0x4,
    e10A_Bit4_SetOffExplosives = 0x8,
    e10A_Bit5_Electrocuted = 0x10,
    e10A_Bit6 = 0x20,
    e10A_Bit7 = 0x40,
    e10A_Bit8 = 0x80,
    e10A_Bit9 = 0x100,
    e10A_Bit10 = 0x200,
    e10A_Bit11 = 0x400,
    e10A_Bit12 = 0x800,
    e10A_Bit13 = 0x1000,
    e10A_Bit14 = 0x2000,
    e10A_Bit15 = 0x4000,
    e10A_Bit16 = 0x8000,
};

class BaseAliveGameObject : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BaseAliveGameObject* ctor_401090();

    EXPORT BaseGameObject* dtor_401000();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUnPosses();

    virtual void VPossessed();

    virtual void VSetMotion(s16 state);

    virtual void VSetXSpawn(s16 camWorldX, s32 screenXPos);

    virtual void VSetYSpawn(s32 camWorldY, s16 bLeft);

    virtual void VOnPathTransition(s16 camWorldX, s32 camWorldY, CameraPos direction);

    virtual s16 VTakeDamage(BaseGameObject* pFrom);

    virtual void VOn_TLV_Collision(Path_TLV* pTlv);

    virtual void VCheckCollisionLineStillValid(s32 distance);

    virtual void VOnTrapDoorOpen();

private:
    EXPORT BaseGameObject* Vdtor_402540(s32 flags);

    EXPORT s16 VTakeDamage_401920(BaseGameObject* pFrom);

    EXPORT void VOnPathTransition_401470(s16 camWorldX, s32 camWorldY, CameraPos direction);

    EXPORT void VSetMotion_402520(s16 state);

    EXPORT void VSetXSpawn_401150(s16 camWorldX, s32 screenXPos);

    EXPORT void VSetYSpawn_401380(s32 camWorldY, s16 bLeft);

    EXPORT void VCheckCollisionLineStillValid_401A90(s32 distance);

protected:
    EXPORT void SetActiveCameraDelayedFromDir_401C90();

    EXPORT static void CC OnResourceLoaded_4019A0(BaseAliveGameObject* ppRes);

    EXPORT s16 IsBeeSwarmChasingMe_4022B0();

    EXPORT void UsePathTransScale_4020D0();

    EXPORT static BaseGameObject* CC FindObjectOfType_418280(Types typeToFind, FP xpos, FP ypos);

    EXPORT BirdPortal* IntoBirdPortal_402350(s16 distance);



    EXPORT s16 WallHit_401930(FP offY, FP offX);

    EXPORT s16 InAirCollision_4019C0(PathLine** ppLine, FP* hitX, FP* hitY, FP vely);

    EXPORT s16 Check_IsOnEndOfLine_4021A0(s16 direction, s16 dist);

public:
    EXPORT s16 OnTrapDoorIntersection_401C10(PlatformBase* pPlatform);
    EXPORT s16 MapFollowMe_401D30(s16 snapToGrid);

public:
    FP_RECT field_D4_collection_rect;
    s16 field_E4_previous_motion;
    s16 field_E6_last_anim_frame;
    FP field_E8_LastLineYPos;
    s16 field_EC; // can the bees attack - multiple values so prob more unknown meanings as well
    s16 field_EE_type;
    Path_TLV* field_F0_pTlv;
    PathLine* field_F4_pLine;
    PlatformBase* field_F8_pLiftPoint;
    s16 field_FC_current_motion;
    s16 field_FE_next_state;
    FP field_100_health;
    s16 field_104_pending_resource_count;
    s16 field_106_shot;
    s16 field_108_bMotionChanged;
    BitField16<Flags_10A> field_10A_flags;
};
ALIVE_ASSERT_SIZEOF(BaseAliveGameObject, 0x10C);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_4FC8A0);

} // namespace AO
