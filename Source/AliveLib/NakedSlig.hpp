#pragma once

#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "BaseAliveGameObject.hpp"

struct Path_NakedSlig : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_direction;
    __int16 field_14_state;
    __int16 field_16_locker_direction;
    __int16 field_18_panic_id;
    __int16 field_1A_reset_on_death;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_NakedSlig, 0x1C);

class NakedSlig;

using TNakedSligAIFn = __int16 (NakedSlig::*)();

enum class LevelIds : __int16;

class NakedSlig : public BaseAliveGameObject
{
public:
    EXPORT NakedSlig* ctor_418C70(Path_NakedSlig* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    EXPORT signed __int16 sub_419890(__int16 currentMotion, __int16 bClearNextMotion);

    EXPORT static __int16 CC NextRandom_4197D0();

private:
    void SetBrain(TNakedSligAIFn fn);
    bool BrainIs(TNakedSligAIFn fn);

    EXPORT void dtor_418FE0();

    EXPORT NakedSlig* vdtor_418FB0(signed int flags);


public:
    EXPORT __int16 AI_0_419DE0();
    EXPORT __int16 AI_1_419F60();

private:
    __int16 field_116;
    int field_118_tlvInfo;
    __int16 field_11C_pal_buf[64];
    PSX_RECT field_19C_pal_rect;

    __int16 field_1A4_r;
    __int16 field_1A6_g;
    __int16 field_1A8_b;

    __int16 field_1AA;
    int field_1AC_timer;
    int field_1B0;
    int field_1B4;
    __int16 field_1B8;

    LevelIds field_1BA_prev_level;
    __int16 field_1BC_prev_path;
    __int16 field_1BE_prev_camera;

    char field_1C0;
    char field_1C1;
    __int16 field_1C2_pitch;
    __int16 field_1C4;
    __int16 field_1C6;
    int field_1C8;
    __int16 field_1CC;
    __int16 field_1CE;
    int field_1D0_slig_button_id;
    int field_1D4_obj_id;
    int field_1D8_obj_id;
    int field_1DC;
    __int16 field_1E0;
    __int16 field_1E2;
    Path_TLV* field_1E4_pPantsOrWingsTlv;
    Path_NakedSlig field_1E8_tlv;
    TNakedSligAIFn field_204_brain_state;
    __int16 field_208_brain_sub_state;
    __int16 field_20A;
};
ALIVE_ASSERT_SIZEOF(NakedSlig, 0x20C);
