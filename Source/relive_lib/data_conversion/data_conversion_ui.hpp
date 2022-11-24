#pragma once

#include "../BaseGameObject.hpp"
#include "../../AliveLibCommon/Primitives_common.hpp"
#include "GameType.hpp"
#include <thread>
#include <atomic>

class DataConversionUI final : public BaseGameObject
{
public:
    explicit DataConversionUI(GameType gameType);
    ~DataConversionUI();

    void VUpdate() override;

    void VRender(PrimHeader** pOrderingTable) override;

    bool ConversionRequired();

private:
    void ThreadFunc();

    bool mIncrementing = true;
    s32 mVal = 0;
    GameType mGameType = GameType::eAe;
    Poly_G4 mPoly;
    std::unique_ptr<std::thread> mThread;
    std::atomic<bool> mDone{false};
};
