#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

enum class SwitchOp : s16;

struct Path_CrawlingSligButton final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_id;
    SwitchOp field_14_action;
    s16 field_16_on_sound;
    s16 field_18_off_sound;
    s16 field_1A_sound_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_CrawlingSligButton, 0x1C);

class CrawlingSligButton final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT CrawlingSligButton* ctor_4148F0(Path_CrawlingSligButton* pTlv, s32 tlvInfo);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    EXPORT void UseButton_414C60();

private:
    EXPORT CrawlingSligButton* vdtor_414A60(s32 flags);
    EXPORT void dtor_414A90();
    EXPORT void vUpdate_414B20();

private:
    s32 field_F4_tlvInfo;
    s16 field_F8_id;
    SwitchOp field_FA_action;
    s16 field_FC_on_sound;
    s16 field_FE_off_sound;
    s16 field_100_sound_direction;
    s16 field_102_in_use;
};
ALIVE_ASSERT_SIZEOF(CrawlingSligButton, 0x104);
