#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_CreditsController final : public Path_TLV {};

class CreditsController final : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;

    EXPORT CreditsController* ctor_40CFC0(Path_CreditsController* pTlv, s32 tlvInfo);
    EXPORT BaseGameObject* dtor_40D000();
    EXPORT CreditsController* Vdtor_40D090(s32 flags);
    EXPORT void VUpdate_40D020();

    s32 field_10_next_cam_frame;
    s16 field_14_camera_number;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF(CreditsController, 0x18);

ALIVE_VAR_EXTERN(s16, gCreditsControllerExists_507684);

} // namespace AO
