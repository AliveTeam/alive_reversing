#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "GameSpeak.hpp"

namespace relive
{
    struct Path_SecurityDoor;
}

namespace AO {

enum SecurityDoorStates : s16
{
    eInactive_0 = 0,
    eSuccessChime_1 = 1,
    eSayingHi_2 = 2,
    eListeningForHi_3 = 3,
    eWaitingToSayPassword_4 = 4,
    ePreparingToSayPassword_5 = 5,
    eSayingPassword_6 = 6,
    ePausing_7 = 7,
    eListeningForPassword_8 = 8,
    eCheckingIfPasswordMatches_9 = 9,
    eLaughAtFailure_10 = 10
};

class SecurityDoor final : public BaseAnimatedWithPhysicsGameObject
{
public:
    SecurityDoor(relive::Path_SecurityDoor* pTlv, const Guid& tlvId);
    ~SecurityDoor();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    bool IsPlayerNear();

    Guid mTlvId;
    SecurityDoorStates mState = SecurityDoorStates::eInactive_0;
    s16 mSwitchId = 0;
    s32 mCodeConverted = 0;
    s16 mCodeLength = 0;
    GameSpeakListener mListener;
    GameSpeakEvents mPasswordBuffer[16] = {};
    s16 mMaxIdx = 0; // TODO: better name
    s16 mBufferStartIdx = 0;
    PSX_Point mTlvTopLeft = {};
    PSX_Point mTlvBottomRight = {};
    s32 mTimer = 0;
    s16 mMaxIdx2 = 0; // TODO: better name
};

} // namespace AO
