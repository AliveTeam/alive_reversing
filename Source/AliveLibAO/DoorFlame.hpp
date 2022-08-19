#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class FireBackgroundGlow;
class FlameSparks;

struct Path_DoorFlame final : public Path_TLV
{
    s16 mSwitchId;
    enum class Scale : s16
    {
        eFull_0 = 0,
        eHalf_1 = 1,
        eHalf_2 = 2
    };
    Scale mScale;

    enum class Colour : s16
    {
        default_0 = 0,
        red_1 = 1,
        green_2 = 2,
        blue_3 = 3
    };
    Colour mColour;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorFlame, 0x20);

class DoorFlame final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DoorFlame(Path_DoorFlame* pTlv, s32 tlvInfo);
    ~DoorFlame();

    virtual void VStopAudio() override;
    virtual void VUpdate() override;

    s32 field_E4_tlvInfo = 0;
    s16 field_E8_switch_id = 0;
    s16 field_EA_frame_count = 0;
    enum class States : u16
    {
        eDisabled_0 = 0,
        eEnabled_1 = 1
    };
    States field_EC_state = States::eDisabled_0;
    s16 field_EE_2_random = 0;
    s32 field_F0_sounds_mask = 0;
    FireBackgroundGlow* field_F8_pFireBackgroundGlow = nullptr;
    FlameSparks* field_FC_pFlameSparks = nullptr;
};
ALIVE_ASSERT_SIZEOF(DoorFlame, 0x100);

} // namespace AO
