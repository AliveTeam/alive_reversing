#pragma once

#include "../AliveLibCommon/Function.hpp"
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
    LCDStatusBoard(Path_LCDStatusBoard* pTlv, s32 tlvInfo);
    ~LCDStatusBoard();

    virtual void VScreenChanged() override;

    void VScreenChanged_441C70();

    virtual void VUpdate() override;

    void VUpdate_441A90();

    virtual void VRender(PrimHeader** ppOt) override;

    void VRender_441AB0(PrimHeader** ppOt);

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
