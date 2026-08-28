#pragma once

#include "BaseAliveGameObject.hpp"

class BaseAbe : public ::BaseAliveGameObject
{
public:
    explicit BaseAbe(s32 resourceArraySize);
    virtual bool IsEnteringOrExitingDoor() const = 0;
    virtual s32 DoorId() const = 0;
};

BaseAbe* GetAbe();
