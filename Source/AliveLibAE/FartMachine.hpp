#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "Font.hpp"
#include "FunctionFwd.hpp"

struct Path_FartMachine : public Path_TLV
{
    s16 field_10_num_brews;
    s16 field_12_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FartMachine, 0x14);

class FartMachine : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FartMachine* ctor_413060(Path_FartMachine* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT FartMachine* vdtor_413290(s32 flags);
    EXPORT void dtor_413330();
    EXPORT void vUpdate_4132C0();
    EXPORT void vRender_4133F0(PrimHeader** ppOt);

private:
    Font_Context field_F4_font_context;
    Alive::Font field_104_font;
    s16 field_13C_textX;
    s16 field_13E_textY;
    s32 field_140_tlvInfo;
public:
    s16 field_144_total_brew_count;
private:
    s16 field_146_padding;
    s32 field_148_padding;
    Animation field_14C_anim;
    s16 field_1E4_remaining_brew_count;
    s16 field_1E6_cam_id;
};
ALIVE_ASSERT_SIZEOF(FartMachine, 0x1E8);
