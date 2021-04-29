#pragma once

#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "FunctionFwd.hpp"
#include "Animation.hpp"
#include "AnimationUnknown.hpp"
#include "Map.hpp"

namespace AETest::TestsBaseAnimatedWithPhysicsGameObject {
void BaseAnimatedWithPhysicsGameObjectTests();
}

class Shadow;

struct TintEntry final
{
    s8 field_0_level;
    u8 field_1_r;
    u8 field_2_g;
    u8 field_3_b;
};
ALIVE_ASSERT_SIZEOF(TintEntry, 0x4);

#ifdef _MSC_VER
// NOTE: __single_inheritance required to workaround MSVC code gen bug
// https://stackoverflow.com/questions/8676879/member-function-pointer-runtime-error-the-value-of-esp-was-not-properly-saved
class __single_inheritance BaseAnimatedWithPhysicsGameObject;
#else
class BaseAnimatedWithPhysicsGameObject;
#endif

using TCollisionCallBack = s16 (BaseGameObject::*)(BaseGameObject*); // Typically points to something in the derived type.. pretty strange, probably also why its a function pointer

class BaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
public:
    EXPORT BaseAnimatedWithPhysicsGameObject* BaseAnimatedWithPhysicsGameObject_ctor_424930(s16 resourceArraySize);
    EXPORT void BaseAnimatedWithPhysicsGameObject_dtor_424AD0();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT BaseGameObject* vdtor_424A40(s32 flags);
    EXPORT void Update_424AB0();
    EXPORT void Render_424B90(PrimHeader** ppOt);

    EXPORT void Animation_Init_424E10(s32 frameTableOffset, s32 maxW, u16 maxH, u8** ppAnimData, s16 a6, u8 a7);

    virtual void vOnCollisionWith_424EE0(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn);
    // TODO: Just return by value - this is odd optimization the compiler does
    virtual PSX_RECT* vGetBoundingRect_424FD0(PSX_RECT* pRect, s32 pointIdx);
    virtual s16 vIsObjNearby_4253B0(FP radius, BaseAnimatedWithPhysicsGameObject* pObj);
    virtual s16 vIsObj_GettingNear_425420(BaseAnimatedWithPhysicsGameObject* pObj);
    virtual s16 vIsFacingMe_4254A0(BaseAnimatedWithPhysicsGameObject* pOther);
    virtual s16 vOnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther);
    virtual void vStackOnObjectsOfType_425840(AETypes typeToFind);
    virtual void VOnPickUpOrSlapped();
    virtual void VOnThrowableHit(BaseGameObject* pFrom);

private:
    EXPORT PSX_RECT* GetBoundingRect_424FD0(PSX_RECT* pRect, s32 pointIdx);
    EXPORT s16 IsObjNearby_4253B0(FP radius, BaseAnimatedWithPhysicsGameObject* pObj);
    EXPORT s16 IsObj_GettingNear_On_X_425420(BaseAnimatedWithPhysicsGameObject* pOther);
    EXPORT s16 IsFacingMe_4254A0(BaseAnimatedWithPhysicsGameObject* pOther);
    EXPORT s16 OnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther);
    EXPORT void StackOnObjectsOfType_425840(AETypes typeToFind);
    EXPORT void vOnPickUpOrSlapped_408180();
    EXPORT void vOnThrowableHit_4081A0(BaseGameObject* pFrom);

public:
    EXPORT CameraPos Is_In_Current_Camera_424A70();
    EXPORT void DealDamageRect_4247A0(const PSX_RECT* pRect);
    EXPORT void OnCollisionWith_424EE0(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn);
    EXPORT void SetTint_425600(const TintEntry* pTintArray, LevelIds level_id);

protected:
    void SetRGB(s16 r, s16 g, s16 b);

public:
    Animation field_20_animation;
    FP field_B8_xpos;
    FP field_BC_ypos;
    s16 field_C0_path_number;
    LevelIds field_C2_lvl_number;
    FP field_C4_velx;
    FP field_C8_vely;
    FP field_CC_sprite_scale;
    s16 field_D0_r;
    s16 field_D2_g;
    s16 field_D4_b;
    s16 field_D6_scale;
    s16 field_D8_yOffset;
    s16 field_DA_xOffset;
    s16 field_DC_bApplyShadows;
    s16 field_DE_pad;
    Shadow* field_E0_pShadow;
    FP_RECT field_E4_collection_rect;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xF4);
