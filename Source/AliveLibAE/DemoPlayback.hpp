#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

class DemoPlayback final : public BaseGameObject
{
public:
    DemoPlayback();
    ~DemoPlayback();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

};

extern DemoPlayback* gActiveDemoPlayback;
extern s16 gIsDemoStartedManually;
