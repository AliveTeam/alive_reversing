#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "MusicController.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../AliveLibCommon/Psx_common.hpp"

class MusicTrigger final : public BaseGameObject
{
public:
    MusicTrigger(relive::Path_MusicTrigger* pTlv, const Guid& tlvId);
    MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType musicType, relive::Path_MusicTrigger::TriggeredBy triggeredBy, s32 /*not_used*/, s32 delay);
    ~MusicTrigger();

    void Init(relive::Path_MusicTrigger::MusicTriggerMusicType musicType, relive::Path_MusicTrigger::TriggeredBy triggeredBy, s16 delay);
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    Guid mTlvId;
    bool mTriggeredByTouching = false;
    bool mTriggeredByTimer = false;
    bool mSetMusicToNoneOnDtor = false;
    MusicController::MusicTypes mMusicType = MusicController::MusicTypes::eNone_0;
    s32 mCounter = 0;
    PSX_Point mTlvTopLeft = {};
    PSX_Point mTlvBottomRight = {};
};
