















#pragma once
#include "imgui.h"      
#ifndef IMGUI_DISABLE

struct ID3D10Device;
struct ID3D10SamplerState;
struct ID3D10Buffer;


IMGUI_IMPL_API bool     ImGui_ImplDX10_Init(ID3D10Device* device);
IMGUI_IMPL_API void     ImGui_ImplDX10_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplDX10_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplDX10_RenderDrawData(ImDrawData* draw_data);


IMGUI_IMPL_API bool     ImGui_ImplDX10_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplDX10_InvalidateDeviceObjects();


IMGUI_IMPL_API void     ImGui_ImplDX10_UpdateTexture(ImTextureData* tex);




struct ImGui_ImplDX10_RenderState
{
    ID3D10Device*           Device;
    ID3D10SamplerState*     SamplerDefault;
    ID3D10Buffer*           VertexConstantBuffer;
};

#endif 
