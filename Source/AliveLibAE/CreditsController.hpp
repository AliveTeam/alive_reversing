#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

struct Path_TLV;

#pragma pack(push)
#pragma pack(2)
class CreditsController : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;

    EXPORT CreditsController* ctor_418A10(Path_TLV* pTlv, int tlvInfo);
private:
    EXPORT void dtor_418A90();
    EXPORT BaseGameObject* vdtor_418A60(s32 flags);
    EXPORT void Update_418AC0();
public:
    int field_20_next_cam_frame;
    s16 field_24_camera_number;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(CreditsController, 0x26);

ALIVE_VAR_EXTERN(s16, sDoesCreditsControllerExist_5C1B90);