



























#pragma once
#include "imgui.h"          
#ifndef IMGUI_DISABLE


#if defined(__EMSCRIPTEN__) && !defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN) && !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
#include <emscripten/version.h>
#if (__EMSCRIPTEN_major__ >= 4) && (__EMSCRIPTEN_minor__ >= 0) && (__EMSCRIPTEN_tiny__ >= 10)
#define IMGUI_IMPL_WEBGPU_BACKEND_DAWN
#else
#define IMGUI_IMPL_WEBGPU_BACKEND_WGPU
#endif
#endif

#include <webgpu/webgpu.h>
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
#include <webgpu/webgpu_cpp.h>  
#elif !defined(__EMSCRIPTEN__)
#include <webgpu/wgpu.h>        
#endif


struct ImGui_ImplWGPU_InitInfo
{
    WGPUDevice              Device = nullptr;
    int                     NumFramesInFlight = 3;
    WGPUTextureFormat       RenderTargetFormat = WGPUTextureFormat_Undefined;
    WGPUTextureFormat       DepthStencilFormat = WGPUTextureFormat_Undefined;
    WGPUMultisampleState    PipelineMultisampleState = {};

    ImGui_ImplWGPU_InitInfo()
    {
        PipelineMultisampleState.count = 1;
        PipelineMultisampleState.mask = UINT32_MAX;
        PipelineMultisampleState.alphaToCoverageEnabled = false;
    }
};


IMGUI_IMPL_API bool ImGui_ImplWGPU_Init(ImGui_ImplWGPU_InitInfo* init_info);
IMGUI_IMPL_API void ImGui_ImplWGPU_Shutdown();
IMGUI_IMPL_API void ImGui_ImplWGPU_NewFrame();
IMGUI_IMPL_API void ImGui_ImplWGPU_RenderDrawData(ImDrawData* draw_data, WGPURenderPassEncoder pass_encoder);


IMGUI_IMPL_API bool ImGui_ImplWGPU_CreateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplWGPU_InvalidateDeviceObjects();


IMGUI_IMPL_API void ImGui_ImplWGPU_UpdateTexture(ImTextureData* tex);




struct ImGui_ImplWGPU_RenderState
{
    WGPUDevice                  Device;
    WGPURenderPassEncoder       RenderPassEncoder;
};







bool        ImGui_ImplWGPU_IsSurfaceStatusError(WGPUSurfaceGetCurrentTextureStatus status);
bool        ImGui_ImplWGPU_IsSurfaceStatusSubOptimal(WGPUSurfaceGetCurrentTextureStatus status);    


void        ImGui_ImplWGPU_DebugPrintAdapterInfo(const WGPUAdapter& adapter);
const char* ImGui_ImplWGPU_GetBackendTypeName(WGPUBackendType type);
const char* ImGui_ImplWGPU_GetAdapterTypeName(WGPUAdapterType type);
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
const char* ImGui_ImplWGPU_GetDeviceLostReasonName(WGPUDeviceLostReason type);
const char* ImGui_ImplWGPU_GetErrorTypeName(WGPUErrorType type);
#elif defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU) && !defined(__EMSCRIPTEN__)
const char* ImGui_ImplWGPU_GetLogLevelName(WGPULogLevel level);
#endif


#if defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU) || defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN) && !defined(__EMSCRIPTEN__)
struct ImGui_ImplWGPU_CreateSurfaceInfo
{
    WGPUInstance    Instance;
    const char*     System;      
    void*           RawWindow;   
    void*           RawDisplay;  
    void*           RawSurface;  
    void*           RawInstance; 
};
WGPUSurface ImGui_ImplWGPU_CreateWGPUSurfaceHelper(ImGui_ImplWGPU_CreateSurfaceInfo* info);
#endif

#endif 
