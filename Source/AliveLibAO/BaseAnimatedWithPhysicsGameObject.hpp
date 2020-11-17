#pragma once

#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "FixedPoint_common.hpp"
#include "Psx_common.hpp"
#include "FunctionFwd.hpp"

namespace AO {

class Shadow;
class BaseAliveGameObject;

struct TintEntry
{
    signed __int8 field_0_level;
    unsigned __int8 field_1_r;
    unsigned __int8 field_2_g;
    unsigned __int8 field_3_b;
};
ALIVE_ASSERT_SIZEOF(TintEntry, 0x4);

enum class LevelIds : __int16;
enum class CameraPos : __int16;

#ifdef _MSC_VER
// NOTE: __single_inheritance required to workaround MSVC code gen bug
// https://stackoverflow.com/questions/8676879/member-function-pointer-runtime-error-the-value-of-esp-was-not-properly-saved
class __single_inheritance BaseAnimatedWithPhysicsGameObject;
#else
class BaseAnimatedWithPhysicsGameObject;
#endif

using TCollisionCallBack = __int16(BaseGameObject::*)(BaseGameObject*); // Typically points to something in the derived type.. pretty strange, probably also why its a function pointer

EXPORT FP CC ScaleToGridSize_41FA30(FP scale);

class BaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
public:

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VRender(int** pOrderingTable) override;

    virtual void VOnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, int startingPointIdx, TCollisionCallBack pFn);

    virtual PSX_RECT* VGetBoundingRect(PSX_RECT* pRect, int pointIdx);

    virtual __int16 VIsObjNearby(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj);

    virtual __int16 VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pOther);

    virtual __int16 VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther);

    virtual __int16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther);

    virtual void VStackOnObjectsOfType(Types typeToFind);

    virtual void VOnPickUpOrSlapped();

    virtual void VOnThrowableHit(BaseGameObject* pFrom);

    EXPORT void SetTint_418750(const TintEntry* pTintArray, LevelIds levelId);

    EXPORT BaseAnimatedWithPhysicsGameObject* ctor_417C10();

    EXPORT CameraPos Is_In_Current_Camera_417CC0();

    EXPORT void Animation_Init_417FD0(int frameTableOffset, int maxW, int maxH, BYTE** ppAnimData, __int16 a6);

protected:
    // Note inlined in AO everywhere
    CameraPos Is_In_Current_Camera();

    EXPORT BaseGameObject* dtor_417D10();

    enum class BetweenCamPos : __int16
    {
        None_0 = 0,
        Left_1 = 1,
        Right_2 = 2,
    };

    EXPORT BetweenCamPos BetweenCameras_418500();

    EXPORT __int16 SetBaseAnimPaletteTint_4187C0(const TintEntry* pTintArray, LevelIds lvl, int palId);

private:

    EXPORT BaseAnimatedWithPhysicsGameObject* Vdtor_4189C0(signed int flags);
  
    EXPORT void VRender_417DA0(int** ppOt);

    EXPORT __int16 VOnSameYLevel_418450(BaseAnimatedWithPhysicsGameObject* pOther);

    EXPORT __int16 VIsFacingMe_4183F0(BaseAnimatedWithPhysicsGameObject* pOther);

    EXPORT __int16 VIsObj_GettingNear_On_X_418390(BaseAnimatedWithPhysicsGameObject* pOther);

    EXPORT __int16 VIsObjNearby_418330(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj);

    EXPORT void VOnCollisionWith_418080(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, int startingPointIdx, TCollisionCallBack pFn);

    EXPORT PSX_RECT* VGetBoundingRect_418120(PSX_RECT* pRect, int pointIdx);

    EXPORT void VStackOnObjectsOfType_418930(Types typeToFind);

public:
    Animation field_10_anim;
    FP field_A8_xpos;
    FP field_AC_ypos;
    __int16 field_B0_path_number;
    LevelIds field_B2_lvl_number;
    FP field_B4_velx;
    FP field_B8_vely;
    FP field_BC_sprite_scale;
    __int16 field_C0_r;
    __int16 field_C2_g;
    __int16 field_C4_b;
    __int16 field_C6_scale;
    __int16 field_C8_yOffset;
    __int16 field_CA_xOffset;
    WORD field_CC_bApplyShadows;
    __int16 field_CE_pad;
    Shadow* field_D0_pShadow;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xD4);

}
