#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "Font.hpp"
#include "FunctionFwd.hpp"

struct Path_FartMachine : public Path_TLV
{
    __int16 field_10_num_brews;
    // pad
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FartMachine, 0x14);

class FartMachine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FartMachine* ctor_413060(Path_FartMachine* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;

private:
    EXPORT FartMachine* vdtor_413290(signed int flags);
    EXPORT void dtor_413330();
    EXPORT void vUpdate_4132C0();
    EXPORT void vRender_4133F0(int** pOt);

private:
    Font_Context field_F4_font_context;
    Alive::Font field_104_font;
    __int16 field_13C_textX;
    __int16 field_13E_textY;
    int field_140_tlvInfo;
public:
    __int16 field_144_total_brew_count;
private:
    __int16 field_146;
    int field_148;
    Animation field_14C_anim;
    __int16 field_1E4_remaining_brew_count;
    __int16 field_1E6_cam_id;
};
ALIVE_ASSERT_SIZEOF(FartMachine, 0x1E8);
