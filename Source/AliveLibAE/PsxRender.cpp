#include "stdafx.h"
#include "PsxRender.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Psx.hpp"
#include "../relive_lib/Sound/PsxSpuApi.hpp"
#include "../relive_lib/Renderer/IRenderer.hpp"
#include "../relive_lib/FatalError.hpp"

static void DrawOTag_HandlePrimRendering(IRenderer& renderer, const BasePrimitive& any)
{
    switch (any.mType)
    {
        case PrimitivesTypes::eLineG2:
            renderer.Draw(static_cast<const Line_G2&>(any));
            break;

        case PrimitivesTypes::eLineG4:
            renderer.Draw(static_cast<const Line_G4&>(any));
            break;

        case PrimitivesTypes::ePolyG3:
            renderer.Draw(static_cast<const Poly_G3&>(any));
            break;

        case PrimitivesTypes::ePolyFT4:
            renderer.Draw(static_cast<const Poly_FT4&>(any));
            break;

        case PrimitivesTypes::ePolyG4:
            renderer.Draw(static_cast<const Poly_G4&>(any));
            break;

        default:
            ALIVE_FATAL("Unknown prim type %d", static_cast<s32>(any.mType));
            break;
    }
}


s32 gScreenXOffset = 0;
s32 gScreenYOffset = 0;

static void DrawOTagImpl(BasePrimitive** ppOt, u32 len)
{
    gScreenXOffset = 0;
    gScreenYOffset = 0;

    IRenderer& renderer = *IRenderer::GetRenderer();

    renderer.StartFrame();

    for (u32 i = 0; i < len; i++)
    {
        BasePrimitive* pOtItem = ppOt[i];
        while (pOtItem)
        {
            SsSeqCalledTbyT();

            switch (pOtItem->mType)
            {
                case PrimitivesTypes::eScissorRect:
                    renderer.SetClip(*static_cast<const Prim_ScissorRect*>(pOtItem));
                    break;

                // Always the lowest command in the list (due to where its added to the OT)
                case PrimitivesTypes::eScreenOffset:
                {
                    auto pScreenOffSet = static_cast<const Prim_ScreenOffset*>(pOtItem);
                    renderer.SetScreenOffset(*pScreenOffSet);
                    gScreenXOffset = pScreenOffSet->field_C_xoff * 2;
                    gScreenYOffset = pScreenOffSet->field_E_yoff;
                    break;
                }

                case PrimitivesTypes::eLaughingGas:
                    renderer.Draw(*static_cast<const Prim_GasEffect*>(pOtItem));
                    break;

                default:
                    DrawOTag_HandlePrimRendering(renderer, *pOtItem);
                    break;
            }

            // To the next item
            pOtItem = pOtItem->mNext;
        }
    }
}

void PSX_DrawOTag(BasePrimitive** ppOt, u32 len)
{
    if (!gTurnOffRendering)
    {
        DrawOTagImpl(ppOt, len);
    }
}
