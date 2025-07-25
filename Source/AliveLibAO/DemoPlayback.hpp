#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "SaveGame.hpp"

namespace AO {

struct PlaybackData final
{
    SaveData saveData;
    u8 randomSeed;
    //TODO probably more variables after
};

class DemoPlayback final : public ::BaseGameObject
{
public:
    DemoPlayback();
    ~DemoPlayback();
    
    virtual void VScreenChanged() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VUpdate() override;

    enum class States : s32
    {
        eInit_0 = 0,
        ePlaying_1 = 1,
        eDone_2 = 2,
    };
    States mState = States::eInit_0;
    u8** mDemoRes = nullptr;
    SaveData* mSaveData = nullptr;
};

extern char_type gActiveDemoName[32];

} // namespace AO
