#include "data_conversion_ui.hpp"
#include "Primitives.hpp"
#include "data_conversion.hpp"
#include <functional>
#include "../../AliveLibAE/Resources.hpp"
#include "../../AliveLibAE/Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

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

    /*
    mFontContext.LoadFontType(FontType::LcdFont);
    mFontPal.mPal = mFontContext.field_C_resource_id.mTgaPtr->mPal;
    mFont.Load(512, mFontPal, &mFontContext);
    */

    mFontContext.field_8_atlas_array = sDebugFontAtlas;

    mFontPal.mPal = std::make_shared<AnimationPal>();
    
    auto fontFile = reinterpret_cast<File_Font*>(sDebugFont);
    for (s32 i = 0; i < fontFile->field_6_palette_size; i++)
    {
        mFontPal.mPal->mPal[i] = RGBConversion::RGBA555ToRGBA888Components(fontFile->field_8_palette[i]);
    }

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
    mFontContext.field_C_resource_id.mCurPal = mFontContext.field_C_resource_id.mTgaPtr->mPal;
    mFont.Load(512, mFontPal, &mFontContext);
    
    /*
    BaseAnimatedWithPhysicsGameObject::MakeArray();
    gBaseGameObjects = relive_new DynamicArrayT<BaseGameObject>(90);
    BaseAnimatedWithPhysicsGameObject::MakeArray(); // Makes drawables
    AnimationBase::CreateAnimationArray();
    CamResource nullCamRes;
    pScreenManager = relive_new ScreenManager(nullCamRes, &gMap.field_24_camera_offset);

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

    SetRGB0(&mPoly, 0, 0, 0);
    SetRGB1(&mPoly, 0, 0, 0);
    SetRGB2(&mPoly, 0, 0, 0);
    SetRGB3(&mPoly, 0, 0, 0);

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

void DataConversionUI::VRender(PrimHeader** ppOt)
{
    //mLcdStatusBoard->VRender(ppOt);
    //mLcd->VRender(ppOt);

    sFontDrawScreenSpace = 1;
    mFont.DrawString(ppOt, mCurMessage.c_str(), 20, (240)-15, TPageAbr::eBlend_0, 0, 0, Layer::eLayer_0, 127, 127, 127, 0, FP_FromInteger(1), 640, 0);
    OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_0), &mPoly.mBase.header);
    sFontDrawScreenSpace = 0;
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
