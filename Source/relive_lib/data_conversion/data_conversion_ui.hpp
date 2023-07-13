#pragma once

#include "../GameObjects/BaseGameObject.hpp"
#include "../Primitives.hpp"
#include "../../AliveLibAE/Font.hpp"
#include "../../AliveLibAE/LCDScreen.hpp"
#include "../../AliveLibAE/LCDStatusBoard.hpp"
#include "GameType.hpp"
#include <thread>
#include <atomic>

class DataConversionUI final : public BaseGameObject
{
public:
    explicit DataConversionUI(GameType gameType);
    ~DataConversionUI();

    void VUpdate() override;

    void VRender(OrderingTable& ot) override;

    bool ConversionRequired();

private:
    void ThreadFunc();

    GameType mGameType = GameType::eAe;
    Poly_G4 mPoly;
    std::unique_ptr<std::thread> mThread;
    std::atomic<bool> mDone{false};

    FontContext mFontContext;
    AliveFont mFont;
    PalResource mFontPal;

    u32 mTimer = 0;
    std::string mCurMessage;
    std::string mDots;
    /*
    relive::Path_LCDScreen mLcdScreenParams = {};
    std::unique_ptr<LCDScreen> mLcd;

    relive::Path_LCDStatusBoard mLcdStatusBoardParams = {};
    std::unique_ptr<LCDStatusBoard> mLcdStatusBoard;
    */
};
