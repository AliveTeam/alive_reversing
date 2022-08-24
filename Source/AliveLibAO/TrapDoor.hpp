#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "PlatformBase.hpp"
#include "../AliveLibAE/Path.hpp"

class PathLine;

namespace AO {

enum class TrapDoorState : s16
{
    eClosed_0 = 0,
    eOpening_1 = 1,
    eOpen_2 = 2,
    eClosing_3 = 3,
};

struct Path_TrapDoor final : public Path_TLV
{
    s16 mSwitchId;
    enum StartState : s16
    {
        eOpen_0 = 0,
        eClosed_1 = 1,
    };
    StartState mStartState;
    Choice_short mSelfClosing;
    Scale_short mScale;
    s16 field_20_dest_level;
    XDirection_short mDirection;
    s16 mXOff;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrapDoor, 0x28);


class TrapDoor final : public PlatformBase
{
public:
    TrapDoor(relive::Path_TrapDoor* pTlv, Map* pMap, s32 tlvInfo);
    ~TrapDoor();

    void Open();

    virtual PSX_RECT VGetBoundingRect(s32 pointIdx) override;
    virtual void VScreenChanged() override;
    virtual void VAdd(BaseAliveGameObject* pObj) override;
    virtual void VRemove(BaseAliveGameObject* pObj) override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    s32 mStayOpenTimeTimer = 0;
    s16 mSwitchId = 0;
    TrapDoorState mState = TrapDoorState::eClosed_0;
    s16 mStartState = 0;
    s16 mTrapDoorXOffset = 0;
    relive::reliveChoice mSelfClosing = relive::reliveChoice::eNo;
    FP mTrapDoorX = {};
    FP mTrapDoorY = {};
    PSX_RECT mBoundingRect = {};
};
ALIVE_ASSERT_SIZEOF(TrapDoor, 0x150);


} // namespace AO
