#include "data_conversion_ui.hpp"
#include "Primitives.hpp"
#include "data_conversion.hpp"
#include <functional>
#include "../../AliveLibAE/Resources.hpp"
#include "../../AliveLibAE/Map.hpp"
#include "../../AliveLibAE/PsxRender.hpp"
#include "GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "AnimationConverter.hpp"

DataConversionUI::DataConversionUI(GameType gameType)
    : BaseGameObject(FALSE, 0)
    , mGameType(gameType)
{
    mPoly.SetXYWH(0, 0, 640, 240);
    mPoly.SetRGB0(255, 0, 0);
    mPoly.SetRGB1(0, 255, 255);
    mPoly.SetRGB2(0, 0, 255);
    mPoly.SetRGB3(255, 0, 255);

    /*
    mFontContext.LoadFontType(FontType::LcdFont);
    mFontPal.mPal = mFontContext.mFntResource.mPngPtr->mPal;
    mFont.Load(512, mFontPal, &mFontContext);
    */

    mFontContext.mAtlasArray = sDebugFontAtlas;

    mFontPal.mPal = std::make_shared<AnimationPal>();
    
    auto fontFile = reinterpret_cast<File_Font*>(sDebugFont);
    for (s32 i = 0; i < fontFile->mPaletteSize; i++)
    {
        mFontPal.mPal->mPal[i] = RGBConversion::RGBA555ToRGBA888Components(fontFile->mPalette[i]);
    }

    std::vector<u8> newData(fontFile->mWidth * fontFile->mHeight); // TODO *2 was out of bounds?

    // Expand 4bit to 8bit
    std::size_t src = 0;
    std::size_t dst = 0;
    while (dst < newData.size())
    {
        newData[dst++] = (fontFile->mPixelBuffer[src] & 0xF);
        newData[dst++] = ((fontFile->mPixelBuffer[src++] & 0xF0) >> 4);
    }

    mFontContext.mFntResource.mCurPal = mFontPal.mPal;
    mFontContext.mFntResource.mPngPtr = std::make_shared<PngData>();
    mFontContext.mFntResource.mPngPtr->mWidth = fontFile->mWidth;
    mFontContext.mFntResource.mPngPtr->mHeight = fontFile->mHeight;

    mFontContext.mFntResource.mPngPtr->mPixels.resize(fontFile->mWidth * fontFile->mHeight);
    mFontContext.mFntResource.mPngPtr->mPixels = newData;
    mFontContext.mFntResource.mCurPal = mFontContext.mFntResource.mPngPtr->mPal;
    mFont.Load(512, mFontPal, &mFontContext);
    
    /*
    BaseAnimatedWithPhysicsGameObject::MakeArray();
    gBaseGameObjects = relive_new DynamicArrayT<BaseGameObject>(90);
    BaseAnimatedWithPhysicsGameObject::MakeArray(); // Makes drawables
    AnimationBase::CreateAnimationArray();
    CamResource nullCamRes;
    gScreenManager = relive_new ScreenManager(nullCamRes, &gMap.mCameraOffset);

    mLcdScreenParams.mTopLeftX = 100;
    mLcdScreenParams.mBottomRightX = 300;
    mLcdScreenParams.mBottomRightY = 40;
    mLcdScreenParams.mMessageId1 = 3;
    mLcdScreenParams.mMessageId2 = 4;
    Guid g;
    mLcd = std::make_unique<LCDScreen>(&mLcdScreenParams, g);

    mLcdStatusBoardParams.mTopLeftX = 50;
    mLcdStatusBoardParams.mTopLeftY = 110;
    mLcdStatusBoardParams.mNumberOfMuds = 123;
    mLcdStatusBoardParams.mHideBoard = relive::reliveChoice::eNo;
    mLcdStatusBoard = std::make_unique<LCDStatusBoard>(&mLcdStatusBoardParams, g);
    */
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
    DataConversion::DataVersions zeroVersions;

#if 0
    // Dev hack to control which data files to convert (can force reconvert/force skip)
    zeroVersions = DataConversion::DataVersions::LatestVersion();
    zeroVersions.mFmvVersion = 0;
#endif

    if (mGameType == GameType::eAe)
    {
        dataConversion.ConvertDataAE(dataConversion.DataVersionAE().value_or(zeroVersions));
    }
    else
    {
        dataConversion.ConvertDataAO(dataConversion.DataVersionAO().value_or(zeroVersions));
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
        SetDead(true);
    }

    mPoly.SetRGB0(0, 0, 0);
    mPoly.SetRGB1(0, 0, 0);
    mPoly.SetRGB2(0, 0, 0);
    mPoly.SetRGB3(0, 0, 0);

    //mLcdStatusBoard->VUpdate();
    //mLcd->VUpdate();

    mCurMessage = "Data conversion in progress" + mDots;
    mTimer++;

    if (mTimer > 5)
    {
        mTimer = 0;

        mDots += ".";
        
        if (mDots.length() > 3)
        {
            mDots.clear();
        }
    }
}

void DataConversionUI::VRender(OrderingTable& ot)
{
    //mLcdStatusBoard->VRender(ot);
    //mLcd->VRender(ot);

    gFontDrawScreenSpace = true;
    mFont.DrawString(ot, mCurMessage.c_str(), 20, (240) - 15, relive::TBlendModes::eBlend_0, 0, 0, Layer::eLayer_0, 127, 127, 127, 0, FP_FromInteger(1), 640, 0);
    ot.Add(Layer::eLayer_0, &mPoly);
    gFontDrawScreenSpace = false;
}

bool DataConversionUI::ConversionRequired()
{
    DataConversion dataConversion;
    DataConversion::DataVersions zeroVersions;
    if (mGameType == GameType::eAe)
    {
        return dataConversion.DataVersionAE().value_or(zeroVersions).AnyConversionRequired();
    }
    else
    {
        return dataConversion.DataVersionAO().value_or(zeroVersions).AnyConversionRequired();
    }
}
