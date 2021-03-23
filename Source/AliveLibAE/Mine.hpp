#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"

void Mine_ForceLink();

struct Path_Mine : public Path_TLV
{
    __int16 field_10_num_patterns;
    __int16 field_12_pattern;
    Scale_short field_14_scale;
    __int16 field_16_disabled_resources;
    Choice_short field_18_persist_offscreen;
    __int16 pad;
};
ALIVE_ASSERT_SIZEOF(Path_Mine, 0x1C);

enum Mine_Flags_1BC
{
    eBit0_Unused = 0x1,
    eBit1_PersistOffscreen = 0x2
};

class Mine : public BaseAliveGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    EXPORT Mine* ctor_46B120(Path_Mine *pPath, TlvItemInfoUnion tlv);
private:
    EXPORT Mine* vdtor_46B4C0(signed int flags);
    EXPORT void dtor_46B4F0();
    EXPORT void Update_46B5D0();
    EXPORT void Render_46B7A0(PrimHeader** ppOt);
    EXPORT void ScreenChanged_46BAE0();
    EXPORT void vOnPickUpOrSlapped_46B880();
    EXPORT void vOnThrowableHit_46BA40(BaseGameObject* pFrom);
    EXPORT signed __int16 vTakeDamage_46BB20(BaseGameObject* pFrom);
    EXPORT bool IsColliding_46B8C0();
public:
    WORD field_118_detonating;
    WORD field_11A_disabled_resources;
    TlvItemInfoUnion field_11C_tlv;
    DWORD field_120_gnframe;
    Animation field_124_animation;
    BitField32<Mine_Flags_1BC> field_1BC_flags;
};
ALIVE_ASSERT_SIZEOF(Mine, 0x1C0);
