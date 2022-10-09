#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "PlatformBase.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

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
    Guid field_8_tlvInfo;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(TrapDoor_State, 0xC);

namespace relive
{
    struct Path_TrapDoor;
}

class TrapDoor final : public PlatformBase
{
public:
    TrapDoor(relive::Path_TrapDoor* pTlv, const Guid& tlvId);
    ~TrapDoor();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual PSX_RECT VGetBoundingRect() override;
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
