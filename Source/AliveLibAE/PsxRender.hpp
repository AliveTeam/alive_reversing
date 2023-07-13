#pragma once

#include "../relive_lib/Primitives.hpp"

struct BasePrimitive;
class IRenderer;
enum class Layer : s16;

class OrderingTable final
{
public:
    void Clear();
    void DrawOTag();

    void Add(Layer layer, BasePrimitive* pPrim);
private:
    void HandlePrimRendering(IRenderer& renderer, const BasePrimitive& any);

    static constexpr u32 mLen = 256;
    BasePrimitive* mOrderingTable[mLen] = {};
};

void PSX_DrawOTag(OrderingTable& ot);

extern s32 gScreenXOffset;
extern s32 gScreenYOffset;
