#include "data_conversion_ui.hpp"
#include "Primitives.hpp"
#include "data_conversion.hpp"
#include <functional>
#include "../../AliveLibAE/Resources.hpp"

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

    mFontContext.field_8_atlas_array = reinterpret_cast<Font_AtlasEntry*>(sDebugFontAtlas);

    mFontPal.mPal = std::make_shared<AnimationPal>();

    auto fontFile = reinterpret_cast<File_Font*>(sDebugFont);
    memcpy(&mFontPal.mPal->mPal[0], fontFile->field_8_palette, fontFile->field_6_palette_size * sizeof(u16));

    std::vector<u8> newData(fontFile->mWidth * fontFile->mHeight * 2);

    // Expand 4bit to 8bit
    std::size_t src = 0;
    std::size_t dst = 0;
    while (dst < newData.size())
    {
        newData[dst++] = (fontFile->field_28_pixel_buffer[src] & 0xF);
        newData[dst++] = ((fontFile->field_28_pixel_buffer[src++] & 0xF0) >> 4);
    }

    mFontContext.field_C_resource_id.mCurPal = mFontPal.mPal;
    mFontContext.field_C_resource_id.mTgaPtr = std::make_shared<TgaData>();
    mFontContext.field_C_resource_id.mTgaPtr->mWidth = fontFile->mWidth;
    mFontContext.field_C_resource_id.mTgaPtr->mHeight = fontFile->mHeight;

    mFontContext.field_C_resource_id.mTgaPtr->mPixels.resize(fontFile->mWidth * fontFile->mHeight);
    mFontContext.field_C_resource_id.mTgaPtr->mPixels = newData;


    mFont.Load(512, mFontPal, &mFontContext);
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
      //  mThread = std::make_unique<std::thread>(std::bind(&DataConversionUI::ThreadFunc, this));
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

  //  OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_0), &mPoly.mBase.header);
    mFont.DrawString(ppOt, "All right son?", 0, 0, TPageAbr::eBlend_0, 0, 0, Layer::eLayer_0, 127, 127, 127, 0, FP_FromInteger(1), 640, 0);
}

bool DataConversionUI::ConversionRequired()
{
    return true;
    /*
    DataConversion dataConversion;
    if (mGameType == GameType::eAe)
    {
        return dataConversion.DataVersionAE() != DataConversion::kVersion;
    }
    else
    {
        return dataConversion.DataVersionAO() != DataConversion::kVersion;
    }*/
}
