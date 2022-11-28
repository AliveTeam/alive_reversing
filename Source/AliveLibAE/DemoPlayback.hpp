#pragma once

#include "../relive_lib/BaseGameObject.hpp"

class DemoPlayback final : public BaseGameObject
{
public:
    DemoPlayback();
    ~DemoPlayback();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

};

extern DemoPlayback* sDemoObj_dword_5D1E20;
extern s16 gIsDemoStartedManually_5C1B9C;
