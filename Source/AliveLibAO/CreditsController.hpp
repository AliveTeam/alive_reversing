#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

struct Path_TLV;

class CreditsController : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;

    EXPORT CreditsController* ctor_40CFC0(Path_TLV* pTlv, int tlvInfo);
    EXPORT BaseGameObject* dtor_40D000();
    EXPORT CreditsController* Vdtor_40D090(s32 flags);
    EXPORT void VUpdate_40D020();

    int field_10_next_cam_frame;
    s16 field_14_camera_number;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF(CreditsController, 0x18);

ALIVE_VAR_EXTERN(s16, gCreditsControllerExists_507684);

}
