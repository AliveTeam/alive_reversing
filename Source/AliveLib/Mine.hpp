#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"

void Mine_ForceLink();

struct Path_Mine
{
    Path_TLV field_0_mBase;
    __int16 field_10_num_patterns;
    __int16 field_12_pattern;
    __int16 field_14_scale;
    __int16 field_16_disabled_resources;
    int field_18_persists_offscreen;
};
ALIVE_ASSERT_SIZEOF(Path_Mine, 0x1C);

enum Mine_Flags_1BC
{
    e1BC_Bit0 = 0x1,
    e1BC_Bit1 = 0x2,
};

class Mine : public BaseAliveGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void vnull_4081A0(BaseGameObject* pFrom) override;
    virtual __int16 VTakeDamage_408730(BaseAnimatedWithPhysicsGameObject* pFrom) override;

    EXPORT Mine* ctor_46B120(Path_Mine *pPath, TlvItemInfoUnion tlv);
private:
    EXPORT Mine* vdtor_46B4C0(signed int flags);
    EXPORT void dtor_46B4F0();
    EXPORT void Update_46B5D0();
    EXPORT void Render_46B7A0(int ** pOt);
    EXPORT void ScreenChanged_46BAE0();
    EXPORT void vOnPickUpOrSlapped_46B880();
    EXPORT void vsub_46BA40(BaseGameObject* pFrom);
    EXPORT signed __int16 vTakeDamage_46BB20(BaseAnimatedWithPhysicsGameObject* pFrom);
    EXPORT bool IsColliding_46B8C0();
public:
    BYTE field_116[2];
    WORD field_118;
    WORD field_11A_disabled_resources;
    TlvItemInfoUnion field_11C_tlv;
    DWORD field_120_gnframe;
    Animation field_124_animation;
    BitField32<Mine_Flags_1BC> field_1BC_flags;
};
ALIVE_ASSERT_SIZEOF(Mine, 0x1C0);