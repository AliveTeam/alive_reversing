#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"
#include "../AliveLibCommon/Psx_common.hpp"
#include "../AliveLibCommon/Sys_common.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "PathData.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"

enum class EReliveLevelIds : s16;
class Shadow;

namespace AO {

class BaseAliveGameObject;


enum class CameraPos : s16;

#ifdef _MSC_VER
// NOTE: __single_inheritance required to workaround MSVC code gen bug
// https://stackoverflow.com/questions/8676879/member-function-pointer-runtime-error-the-value-of-esp-was-not-properly-saved
class __multiple_inheritance BaseAnimatedWithPhysicsGameObject;
#else
class BaseAnimatedWithPhysicsGameObject;
#endif

using TCollisionCallBack = s16 (BaseGameObject::*)(BaseGameObject*); // Typically points to something in the derived type.. pretty strange, probably also why its a function pointer

FP ScaleToGridSize(FP scale);

class BaseAnimatedWithPhysicsGameObject : public IBaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VOnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn);
    virtual PSX_RECT VGetBoundingRect(s32 pointIdx = 1);
    virtual s16 VIsObjNearby(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj);
    virtual s16 VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pOther);
    virtual s16 VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther);
    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther);
    virtual void VStackOnObjectsOfType(ReliveTypes typeToFind);
    virtual void VOnPickUpOrSlapped();
    virtual void VOnThrowableHit(BaseGameObject* pFrom);

    void SetTint(const TintEntry* pTintArray, EReliveLevelIds levelId);

    BaseAnimatedWithPhysicsGameObject();

    // Note inlined in AO everywhere
    CameraPos Is_In_Current_Camera();

    void Animation_Init(AnimId animId, u8** ppAnimData);
    void Animation_Init(s32 frameTableOffset, u16 maxW, u16 maxH, u8** ppAnimData);

protected:
    void DeathSmokeEffect(bool bPlaySound);

    ~BaseAnimatedWithPhysicsGameObject();

    enum class BetweenCamPos : s16
    {
        None_0 = 0,
        Left_1 = 1,
        Right_2 = 2,
    };

    BetweenCamPos BetweenCameras_418500();


public:
    Animation mBaseAnimatedWithPhysicsGameObject_Anim;

    Shadow* mShadow = nullptr;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xD4);

} // namespace AO
