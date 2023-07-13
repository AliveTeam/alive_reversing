#include "stdafx.h"
#include "PsxRender.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Psx.hpp"
#include "../relive_lib/Sound/PsxSpuApi.hpp"
#include "../relive_lib/Renderer/IRenderer.hpp"
#include "../relive_lib/FatalError.hpp"

// TODO: Refactor, only used by input to enable controller vibration
s32 gScreenXOffset = 0;
s32 gScreenYOffset = 0;

void OrderingTable::HandlePrimRendering(IRenderer& renderer, const BasePrimitive& any)
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

void OrderingTable::Clear()
{
    for (u32 i = 0; i < mLen; i++)
    {
        mOrderingTable[i] = nullptr;
    }
}

void OrderingTable::DrawOTag()
{
    gScreenXOffset = 0;
    gScreenYOffset = 0;

    IRenderer& renderer = *IRenderer::GetRenderer();

    renderer.StartFrame();

    for (u32 i = 0; i < mLen; i++)
    {
        BasePrimitive* pOtItem = mOrderingTable[i];
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
                    HandlePrimRendering(renderer, *pOtItem);
                    break;
            }

            // To the next item
            pOtItem = pOtItem->mNext;
        }
    }
}

void OrderingTable::Add(Layer layer, BasePrimitive* pPrim)
{
    const u32 otLayer = static_cast<u32>(layer);
    if (otLayer > mLen)
    {
        ALIVE_FATAL("%d layer is out of the ordering table bounds", otLayer);
    }

    BasePrimitive* pOld = mOrderingTable[otLayer];

    // OT points to the new item
    mOrderingTable[otLayer] = pPrim;

    // Item points back to whatever used to be in the OT, either a pointer to the next OT element
    // or the previously added prim.
    pPrim->mNext = pOld;
}

void PSX_DrawOTag(OrderingTable& ot)
{
    // TODO: refactor global - which also makes this func redundant
    if (!gTurnOffRendering)
    {
        ot.DrawOTag();
        ot.Clear();
    }
}
