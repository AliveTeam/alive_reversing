#include "PaletteOverwriter.hpp"

// Overwrites a pallete 8 colours at a time one per update
PalleteOverwriter::PalleteOverwriter(AnimationPal& pal, const RGBA32& colour)
    : BaseGameObject(false, 0), mPal(pal)
{
    SetType(ReliveTypes::ePalOverwriter);

    gObjListDrawables->Push_Back(this);

    //mPal = pal;

    mPalDepth = 256;

    // This object was only ever given 64 color pals in the past, since the VUpdate would overwrite
    // 8 entries per loop it took 64/8=8 calls to VUpdate to complete the overwrite.
    // No matter what we have the pal depth set to we have to complete the overwrite in 8 updates, hence
    // we divide the depth by 8 in order to know how many colours to overwrite per VUpdate to finish in 8 iterations.
    // The reason we stick to 8 ticks is to keep the gaming logic timing the same without it being changed depending
    // on what assets are being used.
    mColoursToUpdatePerIteration = 32;

    SetDrawable(true);

    mColour = colour;

    mNumEntriesToOverwrite = mColoursToUpdatePerIteration;
    mCurPalIndex = 1;
    mFirstUpdate = true;
    mDone = false;
}

PalleteOverwriter::~PalleteOverwriter()
{
    gObjListDrawables->Remove_Item(this);
}

void PalleteOverwriter::VScreenChanged()
{
    // Stayin' alive
}

void PalleteOverwriter::VRender(OrderingTable& /*ot*/)
{
    if (!mDone)
    {
        for (u32 i = mCurPalIndex; i < mCurPalIndex + mNumEntriesToOverwrite; i++)
        {
            mPal.mPal[i] = mColour;
        }
    }
}

void PalleteOverwriter::VUpdate()
{
    if (mFirstUpdate || mDone)
    {
        // First time round or when done do nothing
        mFirstUpdate = false;
        return;
    }

    if (mCurPalIndex == mPalDepth - 1)
    {
        // Got to the end
        mDone = true;
        LOG_INFO("pal overwriter done");
        return;
    }

    mCurPalIndex += mColoursToUpdatePerIteration;

    // Don't go out of bounds (overflow)
    if (mCurPalIndex >= mPalDepth - 1)
    {
        mCurPalIndex = mPalDepth - 1;
    }

    // Don't let the amount of entries to overwrite go out of bounds (overflow)
    if (mCurPalIndex + mNumEntriesToOverwrite >= mPalDepth - 1)
    {
        mNumEntriesToOverwrite = mPalDepth - mCurPalIndex;
    }
}
