#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "Input.hpp"

struct CheatEntry final
{
    u32 mLevelMask;
    s32 mCheatCodeLength;
    const InputCommands* mCheatCodeAry;
    s32 mSuccessIdx;
    void (*mCallback)(void);
};

class CheatController final : public BaseGameObject
{
public:
    CheatController();
    ~CheatController();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;
};

extern CheatController* gCheatController;
