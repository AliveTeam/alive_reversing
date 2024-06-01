#include "PaletteOverwriter.hpp"

namespace AO {

#define kPalDepth 64

PalleteOverwriter::PalleteOverwriter(AnimationPal& /*pal*/, s16 colour)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::ePalOverwriter);

    gObjListDrawables->Push_Back(this);

    SetDrawable(true);

    for (auto& palBufferEntry : field_A8_palBuffer)
    {
        palBufferEntry = colour;
    }

    field_BA_pal_w = 8;
    field_B8_pal_x_index = 1;
    field_BC_bFirstUpdate = 1;
    field_BE_bDone = false;
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
    if (!field_BE_bDone)
    {
        // TODO: FIX ME - abstraction break, the x value is used as an offset as to how much to overwrite, the width isn't isn't the pal depth in this case
        /*
        const IRenderer::PalRecord palRec{ static_cast<s16>(field_10_pal_xy.x + field_B8_pal_x_index), field_10_pal_xy.y, field_BA_pal_w };
        IRenderer::GetRenderer()->PalSetData(palRec, reinterpret_cast<u8*>(&field_A8_palBuffer[0]));*/

        // TODO: Set the next 8 pal entries + update anim
    }
}

void PalleteOverwriter::VUpdate()
{
    if (field_BC_bFirstUpdate || field_BE_bDone)
    {
        // First time round or when done do nothing
        field_BC_bFirstUpdate = false;
    }
    else
    {
        if (field_B8_pal_x_index == kPalDepth - 1)
        {
            // Got to the end
            field_BE_bDone = true;
        }
        else
        {
            field_B8_pal_x_index += 8;

            if (field_B8_pal_x_index >= kPalDepth - 1)
            {
                field_B8_pal_x_index = kPalDepth - 1;
            }

            if (field_BA_pal_w + field_B8_pal_x_index >= kPalDepth - 1)
            {
                field_BA_pal_w = kPalDepth - field_B8_pal_x_index;
            }
        }
    }
}

}
