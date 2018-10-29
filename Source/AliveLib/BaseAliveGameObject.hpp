#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "Animation.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

struct TintEntry
{
    __int8 field_0_level;
    unsigned __int8 field_1_r;
    unsigned __int8 field_2_g;
    unsigned __int8 field_3_b;
};
ALIVE_ASSERT_SIZEOF(TintEntry, 0x4);

struct Path_TLV;
struct PathLine;

#pragma pack(push)
#pragma pack(2)
class BaseAliveGameObject : public BaseAnimatedWithPhysicsGameObject
{
public:
    // gVtbl_BaseAliveGameObject_544000

    EXPORT BaseAliveGameObject* ctor_408240(short resourceArraySize);
    EXPORT void dtor_4080B0();

    virtual void VRender(int** pOrderingTable) override;
    EXPORT void Render_424B90(int** pOrderingTable);

    virtual int Vnull_408F90();
    virtual unsigned __int16 Vnull_408F70();
    virtual __int16 Vsub_4081C0(__int16 a2);
    virtual  int Vsub_408320(__int16 a2, __int16 a3, int a4);
    virtual __int16 Vsub_408730(int arg0);
    virtual signed __int16 Vnull_4087F0(int a2a);
    virtual char Vsub_408A40(__int16 a2);
    virtual int Vsub_408FD0(__int16 a2);
    virtual int Vnull_4081F0();
  
protected:
    EXPORT int vnull_408F90();
    EXPORT unsigned __int16 vnull_408F70();
    EXPORT __int16 vsub_4081C0(__int16 a2);
    EXPORT int vsub_408320(__int16 a2, __int16 a3, int a4);
    EXPORT __int16 vsub_408730(int arg0);
    EXPORT signed __int16 vnull_4087F0(int a2a);
    EXPORT char vsub_408A40(__int16 a2);
    EXPORT int vsub_408FD0(__int16 a2);
    EXPORT int vnull_4081F0();
    EXPORT void SetTint_425600(TintEntry *pTintArray, __int16 level_id);
    EXPORT __int16 sub_408C40();

public:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0_prev_base;
    __int16 field_F4;
    __int16 field_F6;
    FP field_F8;
    Path_TLV* field_FC_pPathTLV;
    PathLine* field_100_pCollisionLine;
    __int16 field_104;
    __int16 field_106_animation_num;
    __int16 field_108;
    __int16 field_10A;
    FP field_10C_health;
    int field_110;
    __int16 field_114_flags;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(BaseAliveGameObject, 0x116);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_5C1B7C);
