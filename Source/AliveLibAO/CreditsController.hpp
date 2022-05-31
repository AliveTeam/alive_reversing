#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_CreditsController final : public Path_TLV {};

class CreditsController final : public BaseGameObject
{
public:
    
    virtual void VUpdate() override;

    CreditsController(Path_CreditsController* pTlv, s32 tlvInfo);
    ~CreditsController();

    s32 field_10_next_cam_frame;
    s16 field_14_camera_number;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF(CreditsController, 0x18);

ALIVE_VAR_EXTERN(s16, gCreditsControllerExists_507684);

} // namespace AO
