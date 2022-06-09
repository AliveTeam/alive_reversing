#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Animation.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"
#include "../AliveLibCommon/Psx_common.hpp"
#include "../AliveLibCommon/Sys_common.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "PathData.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

class Shadow;
class BaseAliveGameObject;

struct TintEntry
{
    EReliveLevelIds field_0_level;
    u8 field_1_r;
    u8 field_2_g;
    u8 field_3_b;
};
ALIVE_ASSERT_SIZEOF(TintEntry, 0x4);

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
public: // Temp interface
    virtual s16 Scale() override
    {
        return mBaseAnimatedWithPhysicsGameObject_Scale;
    }

    virtual FP SpriteScale() override
    {
        return mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    }

    virtual FP XPos() override
    {
        return mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    virtual FP YPos() override
    {
        return mBaseAnimatedWithPhysicsGameObject_YPos;
    }

public:
    

    virtual void VRender(PrimHeader** ppOt) override;

    virtual void VOnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn);

    virtual PSX_RECT* VGetBoundingRect(PSX_RECT* pRect, s32 pointIdx);

    virtual s16 VIsObjNearby(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj);

    virtual s16 VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pOther);

    virtual s16 VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther);

    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther);

    virtual void VStackOnObjectsOfType(ReliveTypes typeToFind);

    virtual void VOnPickUpOrSlapped();

    virtual void VOnThrowableHit(BaseGameObject* pFrom);

    void SetTint_418750(const TintEntry* pTintArray, EReliveLevelIds levelId);

    BaseAnimatedWithPhysicsGameObject();

    CameraPos Is_In_Current_Camera_417CC0();

    void Animation_Init_417FD0(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, s16 bAddToDrawableList);

protected:
    // Note inlined in AO everywhere
    CameraPos Is_In_Current_Camera();
    void DeathSmokeEffect(bool bPlaySound);

    ~BaseAnimatedWithPhysicsGameObject();

    enum class BetweenCamPos : s16
    {
        None_0 = 0,
        Left_1 = 1,
        Right_2 = 2,
    };

    BetweenCamPos BetweenCameras_418500();

    s16 SetBaseAnimPaletteTint_4187C0(const TintEntry* pTintArray, EReliveLevelIds lvl, s32 palId);

private:
    void VRender_417DA0(PrimHeader** ppOt);

    s16 VOnSameYLevel_418450(BaseAnimatedWithPhysicsGameObject* pOther);

    s16 VIsFacingMe_4183F0(BaseAnimatedWithPhysicsGameObject* pOther);

    s16 VIsObj_GettingNear_On_X_418390(BaseAnimatedWithPhysicsGameObject* pOther);

    s16 VIsObjNearby_418330(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj);

    void VOnCollisionWith_418080(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn);

    PSX_RECT* VGetBoundingRect_418120(PSX_RECT* pRect, s32 pointIdx);

    void VStackOnObjectsOfType_418930(ReliveTypes typeToFind);

public:
    Animation mBaseAnimatedWithPhysicsGameObject_Anim;
    FP mBaseAnimatedWithPhysicsGameObject_XPos = {};
    FP mBaseAnimatedWithPhysicsGameObject_YPos = {};
    s16 mBaseAnimatedWithPhysicsGameObject_PathNumber = 0;
    EReliveLevelIds mBaseAnimatedWithPhysicsGameObject_LvlNumber = EReliveLevelIds::eNone;
    FP mBaseAnimatedWithPhysicsGameObject_VelX = {};
    FP mBaseAnimatedWithPhysicsGameObject_VelY = {};
    FP mBaseAnimatedWithPhysicsGameObject_SpriteScale = {};
    s16 mBaseAnimatedWithPhysicsGameObject_Red = 0;
    s16 mBaseAnimatedWithPhysicsGameObject_Green = 0;
    s16 mBaseAnimatedWithPhysicsGameObject_Blue = 0;
    s16 mBaseAnimatedWithPhysicsGameObject_Scale = 0;
    s16 mBaseAnimatedWithPhysicsGameObject_YOffset = 0;
    s16 mBaseAnimatedWithPhysicsGameObject_XOffset = 0;
    u16 mApplyShadows = 0;
    s16 field_CE_pad = 0;
    Shadow* mShadow = nullptr;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xD4);

} // namespace AO
