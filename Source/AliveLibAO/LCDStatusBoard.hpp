#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "Map.hpp"

namespace AO {

struct Path_LCDStatusBoard final : public Path_TLV
{
};

class LCDStatusBoard final : public BaseGameObject
{
public:
    EXPORT LCDStatusBoard* ctor_4418E0(Path_LCDStatusBoard* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_4419E0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT LCDStatusBoard* Vdtor_441C80(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_441C70();

    virtual void VUpdate() override;

    EXPORT void VUpdate_441A90();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_441AB0(PrimHeader** ppOt);

    FontContext field_10_fontContext;
    AliveFont field_20_font1;
    AliveFont field_58_font2;
    AliveFont field_90_font3;
    s32 field_C8_tlv;
    s16 field_CC_xpos;
    s16 field_CE_ypos;
};
ALIVE_ASSERT_SIZEOF(LCDStatusBoard, 0xD0);

} // namespace AO
