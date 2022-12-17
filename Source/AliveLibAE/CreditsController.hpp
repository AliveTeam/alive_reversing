#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

namespace relive
{
    class Path_TLV;
}

#pragma pack(push)
#pragma pack(2)
class CreditsController final : public BaseGameObject
{
public:
    
    virtual void VUpdate() override;

    CreditsController(relive::Path_TLV* pTlv, const Guid& tlvId);
    ~CreditsController();

public:
    s32 mNextCameraTimer = 0;
    s16 mCurrentCamera = 0;
};
#pragma pack(pop)

extern s16 gCreditsControllerExists;
