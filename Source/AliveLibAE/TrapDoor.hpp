#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "PlatformBase.hpp"
#include "Path.hpp"

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
    s16 field_18_dest_level;
    XDirection_short mDirection;
    s16 mXOff;
    s16 mStayOpenTime;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrapDoor, 0x20);

enum class TrapDoorState : s16
{
    eClosed_0 = 0,
    eOpening_1 = 1,
    eOpen_2 = 2,
    eClosing_3 = 3,
};

struct TrapDoor_State final
{
    AETypes field_0_type;
    TrapDoorState field_2_state;
    s32 field_4_open_time;
    s32 field_8_tlvInfo;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TrapDoor_State, 0xC);

class TrapDoor final : public PlatformBase
{
public:
    TrapDoor(relive::Path_TrapDoor* pTlv, s32 tlvInfo);
    ~TrapDoor();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual PSX_RECT VGetBoundingRect(s32 pointIdx) override;
    virtual void VAdd(BaseAliveGameObject* pObj) override;
    virtual void VRemove(BaseAliveGameObject* pObj) override;

    static s32 CreateFromSaveState(const u8* pData);

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
    relive::reliveChoice mSelfClosing = relive::reliveChoice::eNo;
    FP mTrapDoorX = {};
    FP mTrapDoorY = {};
    PSX_RECT mBoundingRect = {};
};
ALIVE_ASSERT_SIZEOF(TrapDoor, 0x150);
