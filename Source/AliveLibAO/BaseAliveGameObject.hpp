#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/BitField.hpp"
#include "../AliveLibAE/Psx.hpp"

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

using TCollisionCallBack = s16 (BaseGameObject::*)(BaseGameObject*); // Typically points to something in the derived type.. pretty strange, probably also why its a function pointer

class BaseAliveGameObject : public BaseAnimatedWithPhysicsGameObject
{
public:
    BaseAliveGameObject();
    ~BaseAliveGameObject();


    virtual void VUnPosses();
    virtual void VPossessed();
    virtual void VSetMotion(s16 state);
    virtual void VSetXSpawn(s16 camWorldX, s32 screenXPos);
    virtual void VSetYSpawn(s32 camWorldY, s16 bLeft);
    virtual void VOnPathTransition(s16 camWorldX, s32 camWorldY, CameraPos direction);
    virtual s16 VTakeDamage(BaseGameObject* pFrom);
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv);
    virtual void VCheckCollisionLineStillValid(s32 distance);
    virtual void VOnTrapDoorOpen();

    void OnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, TCollisionCallBack pFn);

protected:
    template<class T>
    inline void SetCurrentMotion(T motion)
    {
        mCurrentMotion = static_cast<s16>(motion);
    }
    template<class T>
    inline void SetNextMotion(T motion)
    {
        mNextMotion = static_cast<s16>(motion);
    }
    template<class T>
    inline void SetPreviousMotion(T motion)
    {
        mPreviousMotion = static_cast<s16>(motion);
    }

    void SetActiveCameraDelayedFromDir();

    static void OnResourceLoaded_4019A0(BaseAliveGameObject* ppRes);

    void UsePathTransScale_4020D0();

    static BaseGameObject* FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos);

    BirdPortal* IntoBirdPortal_402350(s16 distance);



    s16 WallHit(FP offY, FP offX);

    bool InAirCollision(PathLine** ppLine, FP* hitX, FP* hitY, FP vely);

    s16 SetBaseAnimPaletteTint(const TintEntry* pTintArray, EReliveLevelIds lvl, PalId palId);

    bool Check_IsOnEndOfLine(s16 direction, s16 distance);

public:
    s16 OnTrapDoorIntersection_401C10(PlatformBase* pPlatform);
    s16 MapFollowMe(s16 snapToGrid);

public:
    FP_RECT mCollectionRect = {};
    s16 mPreviousMotion = 0;
    s32 mBaseAliveGameObjectLastAnimFrame = 0;
    FP BaseAliveGameObjectLastLineYPos = {};
    s16 field_EC = 0; // can the bees attack - multiple values so prob more unknown meanings as well
    s16 field_EE_type = 0; // unused??
    relive::Path_TLV* BaseAliveGameObjectPathTLV = nullptr;
    PathLine* BaseAliveGameObjectCollisionLine = nullptr;
    PlatformBase* mLiftPoint = nullptr;
    s16 mCurrentMotion = 0;
    s16 mNextMotion = 0;
    FP mHealth = {};
    //s16 field_104_pending_resource_count = 0;
    s16 field_106_shot = 0;
    s16 field_108_bMotionChanged = 0;
    BitField16<Flags_10A> mBaseAliveGameObjectFlags = {};
};

extern DynamicArrayT<BaseAliveGameObject>* gBaseAliveGameObjects;

} // namespace AO
