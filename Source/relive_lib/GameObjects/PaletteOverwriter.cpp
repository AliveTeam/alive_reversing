#include "PaletteOverwriter.hpp"

#define kPalDepth 64

// Overwrites a pallete 8 colours at a time one per update
PalleteOverwriter::PalleteOverwriter(AnimationPal& pal, s16 colour)
    : BaseGameObject(false, 0)
{
    SetType(ReliveTypes::ePalOverwriter);

    gObjListDrawables->Push_Back(this);

    mPal = pal;

    u32 palDepth = 1; // account for the first array index which is 0
    for (u32 i = 0; i < ALIVE_COUNTOF(mPal.mPal); i++)
    {
        if (*(reinterpret_cast<u32*>(&mPal.mPal[i])) != 0)
        {
            palDepth++;
        }
    }
    LOG_INFO("pal depth %d", palDepth);

    SetDrawable(true);

    for (auto& palBufferEntry : mPalBuffer)
    {
        palBufferEntry = colour;
    }

    mPalW = 8;
    mPalXIndex = 1;
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
        // TODO: FIX ME - abstraction break, the x value is used as an offset as to how much to overwrite, the width isn't isn't the pal depth in this case
        /*
        const IRenderer::PalRecord palRec{ static_cast<s16>(field_20_pal_xy.x + mPalXIndex), field_20_pal_xy.y, mPalW};

        IRenderer::GetRenderer()->PalSetData(palRec, reinterpret_cast<u8*>(&mPalBuffer[0]));
        */

        // TODO: Copy in the 8 new entries

        // TODO: Actually set this pal back on the anim
    }
}

void PalleteOverwriter::VUpdate()
{
    if (mFirstUpdate || mDone)
    {
        // First time round or when done do nothing
        mFirstUpdate = false;
    }
    else
    {
        if (mPalXIndex == kPalDepth - 1)
        {
            // Got to the end
            mDone = true;
        }
        else
        {
            mPalXIndex += 8;

            if (mPalXIndex >= kPalDepth - 1)
            {
                mPalXIndex = kPalDepth - 1;
            }

            if (mPalXIndex + mPalW >= kPalDepth - 1)
            {
                mPalW = kPalDepth - mPalXIndex;
            }
        }
    }
}
