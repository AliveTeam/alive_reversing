#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "SwitchStates.hpp"

enum class LeverState : s16
{
    eWaiting_0 = 0,
    ePulled_1 = 1,
    eFinished_2 = 2
};

enum class LeverSoundType : s16
{
    eNone = 0,
    eWell_1 = 1,
    eSwitchBellHammer_2 = 2,
    eDoor_3 = 3,
    eElectricWall_4 = 4,
    eSecurityOrb_5 = 5,
    eLift_6 = 6
};

enum class LeverSoundDirection : s16
{
    eLeftAndRight_0 = 0,
    eLeft_1 = 1,
    eRight_2 = 2,
};

struct Path_Lever final : public Path_TLV
{
    SwitchOp mAction;
    Scale_short mScale;
    LeverSoundType mOnSound;
    LeverSoundType mOffSound;
    LeverSoundDirection mSoundDirection;
    s16 mSwitchId;
    Choice_short mPersistOffscreen;
    s16 field_1E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Lever, 0x20);

class Lever final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Lever(relive::Path_Lever* pTlv, const TLVUniqueId& tlvId);
    ~Lever();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s16 VPull(s16 bLeftDirection);

private:
    s16 field_F4_switch_id = 0;
    LeverState field_F8_state = LeverState::eWaiting_0;
    TLVUniqueId field_FC_tlvInfo;
    enum Flags_100
    {
        eBit1_lever_anim_left_direction = 0x1,
        eBit2_persist_offscreen = 0x2,
    };
    BitField16<Flags_100> field_100_flags = {};
    relive::reliveSwitchOp field_102_action = relive::reliveSwitchOp::eSetTrue;
    relive::Path_Lever::LeverSoundType field_104_on_sound = relive::Path_Lever::LeverSoundType::eNone;
    relive::Path_Lever::LeverSoundType field_106_off_sound = relive::Path_Lever::LeverSoundType::eNone;
    relive::Path_Lever::LeverSoundDirection field_108_sound_direction = relive::Path_Lever::LeverSoundDirection::eLeftAndRight;
};
ALIVE_ASSERT_SIZEOF(Lever, 0x10C);
