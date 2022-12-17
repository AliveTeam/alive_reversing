#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "MusicController.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class MusicTrigger final : public ::BaseGameObject
{
public:
    MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType type, relive::Path_MusicTrigger::TriggeredBy triggeredBy, s32 switchId, s32 delay);
    MusicTrigger(relive::Path_MusicTrigger* pTlv, const Guid& tlvId);

    void Init(relive::Path_MusicTrigger::MusicTriggerMusicType type, relive::Path_MusicTrigger::TriggeredBy triggeredBy, u16 switchId, s16 delay);

    ~MusicTrigger();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    Guid mTlvId;
    bool mHasSwitchId = false;
    bool mSetMusicToNoneOnDtor = false;
    bool mUnknown = false;
    s32 mCounter = 0;
    MusicController::MusicTypes mMusicType = MusicController::MusicTypes::eType0;
    u16 mSwitchId = 0;
};

} // namespace AO
