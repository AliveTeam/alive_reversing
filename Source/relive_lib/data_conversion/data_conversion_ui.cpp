#include "data_conversion_ui.hpp"
#include "Primitives.hpp"
#include "data_conversion.hpp"
#include <functional>

DataConversionUI::DataConversionUI(GameType gameType)
    : BaseGameObject(FALSE, 0)
    , mGameType(gameType)
{
    PolyG4_Init(&mPoly);
    SetXYWH(&mPoly, 0, 0, 640, 240);
    SetRGB0(&mPoly, 255, 0, 0);
    SetRGB1(&mPoly, 0, 255, 255);
    SetRGB2(&mPoly, 0, 0, 255);
    SetRGB3(&mPoly, 255, 0, 255);
}

DataConversionUI::~DataConversionUI()
{
    TRACE_ENTRYEXIT;
    if (mThread && mThread->joinable())
    {
        mThread->join();
    }
}

void DataConversionUI::ThreadFunc()
{
    TRACE_ENTRYEXIT;

    // TODO: The thread saftey here is questionable and we also need
    // to be able to incrementally call/poll the conversion for info/progress/errors
    DataConversion dataConversion;
    if (mGameType == GameType::eAe)
    {
        dataConversion.ConvertDataAE();
    }
    else
    {
        dataConversion.ConvertDataAO();
    }

    mDone = true;
}

void DataConversionUI::VUpdate()
{
    if (!mThread)
    {
        mThread = std::make_unique<std::thread>(std::bind(&DataConversionUI::ThreadFunc, this));
    }

    if (mDone)
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mIncrementing)
    {
        mVal += 1;
        if (mVal >= 130)
        {
            mVal = 130;
            mIncrementing = false;
        }
    }
    else
    {
        mVal -= 1;
        if (mVal <= 40)
        {
            mVal = 40;
            mIncrementing = true;
        }
    }

    const u8 col = static_cast<u8>(mVal);

    SetRGB0(&mPoly, 0, col, col);
    SetRGB1(&mPoly, col, 0, col);
    SetRGB2(&mPoly, col, col, 0);
    SetRGB3(&mPoly, 0, col, col);
}

void DataConversionUI::VRender(PrimHeader** ppOt)
{
    OrderingTable_Add(ppOt, &mPoly.mBase.header);
}

bool DataConversionUI::ConversionRequired()
{
    DataConversion dataConversion;
    if (mGameType == GameType::eAe)
    {
        return dataConversion.DataVersionAE() != DataConversion::kVersion;
    }
    else
    {
        return dataConversion.DataVersionAO() != DataConversion::kVersion;
    }
}
