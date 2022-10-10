#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

class DemoPlayback final : public BaseGameObject
{
public:
    DemoPlayback();
    ~DemoPlayback();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

};
ALIVE_ASSERT_SIZEOF(DemoPlayback, 0x20);

extern DemoPlayback* sDemoObj_dword_5D1E20;
extern s16 gIsDemoStartedManually_5C1B9C;
