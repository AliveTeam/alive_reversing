#pragma once

#include "../relive_lib/GameObjects/PlatformBase.hpp"
#include "../relive_lib/SaveStateBase.hpp"

namespace relive
{
    struct Path_TrapDoor;
}

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

struct TrapDoorSaveState final : public SaveStateBase
{
    TrapDoorSaveState()
        : SaveStateBase(ReliveTypes::eTrapDoor, sizeof(*this))
    { }
    TrapDoorState mState;
    s32 mOpenTime;
    Guid mTlvId;
};

namespace relive
{
    struct Path_TrapDoor;
}

class TrapDoor final : public PlatformBase
{
public:
    TrapDoor(relive::Path_TrapDoor* pTlv, const Guid& tlvId);
    ~TrapDoor();

    void LoadAnimationsAO();
    void LoadAnimationsAE();

    TrapDoor_Data GetTrapDoorData() const;

    virtual PSX_RECT VGetBoundingRect() override;
    virtual void VScreenChanged() override;
    virtual void VAdd(BaseAliveGameObject* pObj) override;
    virtual void VRemove(BaseAliveGameObject* pObj) override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VUpdate() override;

    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    static void CreateFromSaveState(SerializedObjectData& pData);

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
