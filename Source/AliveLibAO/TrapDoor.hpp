#pragma once

#include "../relive_lib/GameObjects/PlatformBase.hpp"

class PathLine;
class Map;

namespace relive {
struct Path_TrapDoor;
}

namespace AO {

struct TrapDoor_Data final
{
    AnimId mOpen;
    AnimId mClosed;
    AnimId mOpening;
    AnimId mClosing;
};

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
    TrapDoor(relive::Path_TrapDoor* pTlv, const Guid& tlvId);
    ~TrapDoor();

    TrapDoor_Data GetTrapDoorData() const;

    void LoadAnimationsAO();
    void LoadAnimationsAE();

    virtual PSX_RECT VGetBoundingRect() override;
    virtual void VScreenChanged() override;
    virtual void VAdd(BaseAliveGameObject* pObj) override;
    virtual void VRemove(BaseAliveGameObject* pObj) override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VUpdate() override;

private:
    void Add_To_Collisions_Array();
    void Open();

private:
    s32 mStayOpenTimeTimer = 0;
    s16 mSwitchId = 0;
    TrapDoorState mState = TrapDoorState::eClosed_0;
    s16 mStartState = 0;
    s16 mTrapDoorXOffset = 0;
    s16 mStayOpenTime = 0;
    bool mSelfClosing = false;
    FP mTrapDoorX = {};
    FP mTrapDoorY = {};
    PSX_RECT mBoundingRect = {};
};


} // namespace AO
