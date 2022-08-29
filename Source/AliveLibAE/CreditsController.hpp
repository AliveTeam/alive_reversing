#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"

struct Path_CreditsController final : public Path_TLV
{
    // Empty
};

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
ALIVE_ASSERT_SIZEOF(CreditsController, 0x26);

ALIVE_VAR_EXTERN(s16, gCreditsControllerExists);