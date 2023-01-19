#pragma once

#include <string>
#include "../relive_lib/Renderer/IRenderer.hpp"

void VGA_Shutdown();
void VGA_EndFrame();
void VGA_CreateRenderer(IRenderer::Renderers rendererToCreate, const std::string& windowTitleBase);
