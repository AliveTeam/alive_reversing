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

    for (auto& palBufferEntry : field_B8_palBuffer)
    {
        palBufferEntry = colour;
    }

    field_CA_pal_w = 8;
    field_C8_pal_x_index = 1;
    field_CC_bFirstUpdate = 1;
    field_CE_bDone = false;
}

PalleteOverwriter::~PalleteOverwriter()
{
    gObjListDrawables->Remove_Item(this);
}

void PalleteOverwriter::VScreenChanged()
{
    // Stayin' alive
}

void PalleteOverwriter::VUpdate()
{
    if (field_CC_bFirstUpdate || field_CE_bDone)
    {
        // First time round or when done do nothing
        field_CC_bFirstUpdate = false;
    }
    else
    {
        if (field_C8_pal_x_index == kPalDepth - 1)
        {
            // Got to the end
            field_CE_bDone = true;
        }
        else
        {
            field_C8_pal_x_index += 8;

            if (field_C8_pal_x_index >= kPalDepth - 1)
            {
                field_C8_pal_x_index = kPalDepth - 1;
            }

            if (field_C8_pal_x_index + field_CA_pal_w >= kPalDepth - 1)
            {
                field_CA_pal_w = kPalDepth - field_C8_pal_x_index;
            }
        }
    }
}

void PalleteOverwriter::VRender(OrderingTable& /*ot*/)
{
    if (!field_CE_bDone)
    {
        // TODO: FIX ME - abstraction break, the x value is used as an offset as to how much to overwrite, the width isn't isn't the pal depth in this case
        /*
        const IRenderer::PalRecord palRec{ static_cast<s16>(field_20_pal_xy.x + field_C8_pal_x_index), field_20_pal_xy.y, field_CA_pal_w};

        IRenderer::GetRenderer()->PalSetData(palRec, reinterpret_cast<u8*>(&field_B8_palBuffer[0]));
        */

        // TODO: Copy in the 8 new entries

        // TODO: Actually set this pal back on the anim
    }
}
