#pragma once

#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "FunctionFwd.hpp"
#include "Animation.hpp"
#include "Map.hpp"

namespace Test
{
    void BaseAnimatedWithPhysicsGameObjectTests();
}

class Shadow;


// NOTE: __single_inheritance required to workaround MSVC code gen bug
// https://stackoverflow.com/questions/8676879/member-function-pointer-runtime-error-the-value-of-esp-was-not-properly-saved
class __single_inheritance BaseAnimatedWithPhysicsGameObject;

using TCollisionCallBack = __int16(BaseGameObject::*)(BaseGameObject*); // Typically points to something in the derived type.. pretty strange, probably also why its a function pointer

class BaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
public:
    BaseAnimatedWithPhysicsGameObject();
    EXPORT BaseAnimatedWithPhysicsGameObject * BaseAnimatedWithPhysicsGameObject_ctor_424930(signed __int16 resourceArraySize);
    EXPORT void BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    
    virtual void VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;

    EXPORT void dtor_424A40(signed int flags);
    EXPORT void Update_424AB0();
    EXPORT void Render_424B90(int** pOrderingTable);

    EXPORT void Animation_Init_424E10(int frameTableOffset, int maxW, unsigned __int16 maxH, BYTE **ppAnimData, __int16 a6, unsigned __int8 a7);

    virtual void vOnCollisionWith_424EE0(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, int startingPointIdx, TCollisionCallBack pFn);
    // TODO: Just return by value - this is odd optimization the compiler does
    virtual PSX_RECT* vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx);
    virtual __int16 vIsObjNearby_4253B0(FP radius, BaseAnimatedWithPhysicsGameObject * pObj);
    virtual __int16 vIsObj_GettingNear_425420(BaseAnimatedWithPhysicsGameObject* pObj);
    virtual __int16 vsub_4254A0(int a2);
    virtual __int16 vsub_425520(int a2);
    virtual int vsub_425840(unsigned __int16 a2);
    virtual void vnull_408180();
    virtual void vnull_4081A0();

    EXPORT PSX_RECT* GetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx);
    EXPORT __int16 IsObjNearby_4253B0(FP radius, BaseAnimatedWithPhysicsGameObject * pObj);
    EXPORT __int16 IsObj_GettingNear_425420(BaseAnimatedWithPhysicsGameObject* pOther);
    EXPORT __int16 sub_4254A0(int a2);
    EXPORT __int16 sub_425520(int a2);
    EXPORT int sub_425840(unsigned __int16 a2);
    EXPORT void null_408180();
    EXPORT void null_4081A0();

    EXPORT Map::CameraPos Is_In_Current_Camera_424A70();

    EXPORT void OnCollisionWith_424EE0(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, int startingPointIdx, TCollisionCallBack pFn);
public:

    AnimationEx field_20_animation;
    FP field_B8_xpos;
    FP field_BC_ypos;
    __int16 field_C0_path_number;
    __int16 field_C2_lvl_number;
    FP field_C4_velx;
    FP field_C8_vely;
    FP field_CC_sprite_scale;
    __int16 field_D0_r;
    __int16 field_D2_g;
    __int16 field_D4_b;
    __int16 field_D6_scale;
    __int16 field_D8_yOffset;
    __int16 field_DA_xOffset;
    __int16 field_DC_bApplyShadows;
    __int16 field_DE_pad;
    Shadow* field_E0_176_ptr;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xE4);
