#include "ExternalAssets.hpp"
#include <map>

// We're creating a map for all render commands. This is because we need to tie them to the original
// polygon data. This gets checked inside the renderer, and if a polygon has a render command tied to it,
// it will use that render command instead of the original poly render pipeline. (PSX emu stuff)
// The only reason that we're doing this is because we can't break ABI compatibility at the moment.
std::map<void*, CustomRenderSpriteFormat> g_RenderCommands;

CustomRenderSpriteFormat* CustomRender_GetCommand(void* ptr)
{
    auto it = g_RenderCommands.find(ptr);
    if (it != g_RenderCommands.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CustomRender_AddCommand(void* ptr, CustomRenderSpriteFormat command)
{
    command.originalPoly = ptr;
    g_RenderCommands[ptr] = command;
}

void CustomRender_RemoveCommand(void* ptr)
{
    auto it = g_RenderCommands.find(ptr);
    if (it != g_RenderCommands.end())
    {
        g_RenderCommands.erase(it);
    }
}

void CustomRender_ClearCommands()
{
    g_RenderCommands.clear();
}