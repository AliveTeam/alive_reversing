#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"

struct Path_TLV;

#pragma pack(push)
#pragma pack(2)
class CreditsController final : public BaseGameObject
{
public:
    
    virtual void VUpdate() override;

    CreditsController(Path_TLV* pTlv, s32 tlvInfo);
    ~CreditsController();

public:
    s32 field_20_next_cam_frame;
    s16 field_24_camera_number;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(CreditsController, 0x26);

ALIVE_VAR_EXTERN(s16, sDoesCreditsControllerExist_5C1B90);