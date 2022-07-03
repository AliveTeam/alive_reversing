#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/Sys_common.hpp"
#include "Animation.hpp"
#include "AnimationUnknown.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

class Shadow;


#ifdef _MSC_VER
// NOTE: __single_inheritance required to workaround MSVC code gen bug
// https://stackoverflow.com/questions/8676879/member-function-pointer-runtime-error-the-value-of-esp-was-not-properly-saved
class __multiple_inheritance BaseAnimatedWithPhysicsGameObject;
#else
class BaseAnimatedWithPhysicsGameObject;
#endif

using TCollisionCallBack = s16 (BaseGameObject::*)(BaseGameObject*); // Typically points to something in the derived type.. pretty strange, probably also why its a function pointer

class BaseAnimatedWithPhysicsGameObject : public IBaseAnimatedWithPhysicsGameObject
{
public:
    explicit BaseAnimatedWithPhysicsGameObject(s16 resourceArraySize);
    ~BaseAnimatedWithPhysicsGameObject();

    virtual void VRender(PrimHeader** ppOt) override;

    void Animation_Init(s32 frametableoffset, u16 maxW, u16 maxH, u8** ppAnimData, bool bAddToDrawableList);
    void Animation_Init(AnimId animId, u8** ppAnimData, bool bAddToDrawableList);

    virtual void VOnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn);
    virtual PSX_RECT VGetBoundingRect(s32 pointIdx = 1);
    virtual s16 VIsObjNearby(FP radius, BaseAnimatedWithPhysicsGameObject* pObj);
    virtual s16 VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pObj);
    virtual s16 VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther);
    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther);
    virtual void VStackOnObjectsOfType(ReliveTypes typeToFind);
    virtual void VOnPickUpOrSlapped();
    virtual void VOnThrowableHit(BaseGameObject* pFrom);

    CameraPos Is_In_Current_Camera();
    void SetTint(const TintEntry* pTintArray, EReliveLevelIds level_id);

protected:
    void DeathSmokeEffect(bool bPlaySound);

public:
    Animation mBaseAnimatedWithPhysicsGameObject_Anim = {};

    Shadow* mShadow = nullptr;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xF4);
