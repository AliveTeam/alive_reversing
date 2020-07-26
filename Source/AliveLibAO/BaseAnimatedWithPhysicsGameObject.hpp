#pragma once

#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "FixedPoint_common.hpp"
#include "Psx_common.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

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

class BaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
public:

    virtual void VOnCollisionWith(PSX_Point /*xy*/, PSX_Point /*wh*/, DynamicArray* /*pObjList*/, int /*startingPointIdx*/, TCollisionCallBack /*pFn*/) {}
    virtual PSX_RECT* VGetBoundingRect(PSX_RECT* /*pRect*/, int /*pointIdx*/) { return nullptr; }
    virtual __int16 VIsObjNearby(int /*radius*/, BaseAliveGameObject* /*pOtherObj*/) { return 0; }

    virtual __int16 VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* /*pOther*/) { return 0; }

    virtual __int16 VIsFacingMe(BaseAliveGameObject* /*pOther*/) { return 0; }

    virtual __int16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* /*pOther*/) { return 0; }

    virtual void VStackOnObjectsOfType(unsigned __int16 /*typeToFind*/) {}

    // TODO !!
    virtual void VOnPickUpOrSlapped() {}

    virtual void VOnThrowableHit(BaseGameObject* /*pFrom*/) {}


    EXPORT void SetTint_418750(const TintEntry* pTintArray, LevelIds levelId);


    EXPORT BaseAnimatedWithPhysicsGameObject* ctor_417C10();

    EXPORT void Animation_Init_417FD0(int frameTableOffset, int maxW, int maxH, BYTE** ppAnimData, __int16 a6);

    EXPORT void VRender_417DA0(int** ot);

    EXPORT BaseGameObject* dtor_417D10();

    EXPORT __int16 SetBaseAnimPaletteTint_4187C0(TintEntry* pTintArray, LevelIds level_id, int resourceID);

    EXPORT void VStackOnObjectsOfType_418930(unsigned __int16 typeToFind);

    EXPORT CameraPos Is_In_Current_Camera_417CC0();

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

END_NS_AO
