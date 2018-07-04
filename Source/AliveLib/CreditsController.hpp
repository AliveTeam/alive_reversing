#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

void CreditsController_ForceLink();

#pragma pack(2)
class CreditsController : public BaseGameObject
{
public:
    virtual void VDestructor(signed int flags);
    virtual void VUpdate() override;

    EXPORT CreditsController* ctor_418A10(int a2, int a3);
    EXPORT void dtor_418A90();
    EXPORT void dtor_418A60(char flags);
    EXPORT void Update_418AC0();
public:
    int field_20_next_cam_frame;
    __int16 field_24;
};
ALIVE_ASSERT_SIZEOF(CreditsController, 0x26);

ALIVE_VAR_EXTERN(__int16, sDoesCreditsControllerExist_5C1B90);