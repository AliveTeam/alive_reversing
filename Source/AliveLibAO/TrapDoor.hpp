#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "PlatformBase.hpp"
#include "Path.hpp"

class PathLine;

namespace AO {

enum class TrapDoorState : s16
{
    eClosed_0 = 0,
    eOpening_1 = 1,
    eOpen_2 = 2,
    eClosing_3 = 3,
};

class TrapDoor final : public PlatformBase
{
public:
    TrapDoor(relive::Path_TrapDoor* pTlv, Map* pMap, const Guid& tlvId);
    ~TrapDoor();

    void LoadAnimations();

    void Open();

    virtual PSX_RECT VGetBoundingRect() override;
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
