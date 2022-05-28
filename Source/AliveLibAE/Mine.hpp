#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"

void Mine_ForceLink();

struct Path_Mine final : public Path_TLV
{
    s16 field_10_num_patterns;
    s16 field_12_pattern;
    Scale_short field_14_scale;
    s16 field_16_disabled_resources;
    Choice_short field_18_persist_offscreen;
    s16 pad;
};
ALIVE_ASSERT_SIZEOF(Path_Mine, 0x1C);

enum Mine_Flags_1BC
{
    eBit0_Unused = 0x1,
    eBit1_PersistOffscreen = 0x2
};

class Mine final : public BaseAliveGameObject
{
public:
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    Mine(Path_Mine* pPath, TlvItemInfoUnion tlv);
    ~Mine();

private:

    EXPORT void Update_46B5D0();
    EXPORT void Render_46B7A0(PrimHeader** ppOt);
    EXPORT void ScreenChanged_46BAE0();
    EXPORT void vOnPickUpOrSlapped_46B880();
    EXPORT void vOnThrowableHit_46BA40(BaseGameObject* pFrom);
    EXPORT s16 vTakeDamage_46BB20(BaseGameObject* pFrom);
    EXPORT bool IsColliding_46B8C0();

public:
    u16 field_118_detonating;
    u16 field_11A_disabled_resources;
    TlvItemInfoUnion field_11C_tlv;
    u32 field_120_gnframe;
    Animation field_124_animation;
    BitField32<Mine_Flags_1BC> field_1BC_flags;
};
ALIVE_ASSERT_SIZEOF(Mine, 0x1C0);
