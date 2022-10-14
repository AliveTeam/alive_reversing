#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/AnimationUnknown.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "BaseGameObject.hpp"
#include "MapWrapper.hpp"
#include "../relive_lib/Animation.hpp"
#include "../AliveLibCommon/AnimResources.hpp"
#include "../AliveLibCommon/RGB16.hpp"

namespace relive {
    class Path_TLV;
}

class Shadow;

enum class CameraPos : s16;

struct TintEntry
{
    EReliveLevelIds field_0_level;
    u8 field_1_r;
    u8 field_2_g;
    u8 field_3_b;
};

enum class Scale : s16
{
    Bg = 0,
    Fg = 1,
};

// Another glue/adapter interface
class IBaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
public:
    explicit IBaseAnimatedWithPhysicsGameObject(s16 resourceArraySize)
        : BaseGameObject(true, resourceArraySize)
    {
    }

    virtual ~IBaseAnimatedWithPhysicsGameObject()
    {
    }
    const Scale& GetScale() const { return mScale; }
    void SetScale(Scale val) { mScale = val; }
    const FP& GetSpriteScale() const { return mSpriteScale; }
    void SetSpriteScale(FP val) { mSpriteScale = val; }
    Animation& GetAnimation() { return mAnim; }
    void CreateShadow();

public:
    FP mXPos = {};
    FP mYPos = {};
    s16 mCurrentPath = 0;
    EReliveLevelIds mCurrentLevel = EReliveLevelIds::eNone;
    FP mVelX = {};
    FP mVelY = {};

    RGB16 mRGB;
    s16 mYOffset = 0;
    s16 mXOffset = 0;
    enum VisualFlags : s16
    {
        eApplyShadowZoneColour = 0x1,
        eDoPurpleLightEffect = 0x2
    };
    BitField16<VisualFlags> mVisualFlags = {};
    const Shadow* GetShadow() const { return mShadow; }
    Shadow* GetShadow() { return mShadow; }
private:
    Animation mAnim = {};
    Scale mScale = Scale::Fg;
    FP mSpriteScale = {};

protected:
    Shadow* mShadow = nullptr;
};

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
    static void MakeArray();
    static void FreeArray();

    explicit BaseAnimatedWithPhysicsGameObject(s16 resourceArraySize);
    ~BaseAnimatedWithPhysicsGameObject();

    virtual void VRender(PrimHeader** ppOt) override;

    virtual void VOnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, TCollisionCallBack pFn);
    virtual PSX_RECT VGetBoundingRect();
    virtual s16 VIsObjNearby(FP radius, BaseAnimatedWithPhysicsGameObject* pObj);
    virtual s16 VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pObj);
    virtual s16 VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther);
    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther);
    virtual void VStackOnObjectsOfType(ReliveTypes typeToFind);
    virtual void VOnPickUpOrSlapped();
    virtual void VOnThrowableHit(BaseGameObject* pFrom);

    void SetTint(const TintEntry* pTintArray, EReliveLevelIds levelId);

    // Note inlined in AO everywhere
    CameraPos Is_In_Current_Camera();

    void Animation_Init(const AnimResource& res);

protected:
    void DeathSmokeEffect(bool bPlaySound);

    enum class BetweenCamPos : s16
    {
        None_0 = 0,
        Left_1 = 1,
        Right_2 = 2,
    };

    BetweenCamPos BetweenCameras_418500();
};

extern DynamicArrayT<BaseGameObject>* gObjListDrawables;
