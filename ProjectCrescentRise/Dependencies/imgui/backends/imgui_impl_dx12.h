



















#pragma once
#include "imgui.h"      
#ifndef IMGUI_DISABLE
#include <dxgiformat.h> 
#include <d3d12.h>      


struct ImGui_ImplDX12_InitInfo
{
    ID3D12Device*               Device;
    ID3D12CommandQueue*         CommandQueue;       
    int                         NumFramesInFlight;
    DXGI_FORMAT                 RTVFormat;          
    DXGI_FORMAT                 DSVFormat;          
    void*                       UserData;

    
    
    ID3D12DescriptorHeap*       SrvDescriptorHeap;
    void                        (*SrvDescriptorAllocFn)(ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle);
    void                        (*SrvDescriptorFreeFn)(ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle);
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    D3D12_CPU_DESCRIPTOR_HANDLE LegacySingleSrvCpuDescriptor; 
    D3D12_GPU_DESCRIPTOR_HANDLE LegacySingleSrvGpuDescriptor;
#endif

    ImGui_ImplDX12_InitInfo()   { memset((void*)this, 0, sizeof(*this)); }
};


IMGUI_IMPL_API bool     ImGui_ImplDX12_Init(ImGui_ImplDX12_InitInfo* info);
IMGUI_IMPL_API void     ImGui_ImplDX12_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplDX12_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplDX12_RenderDrawData(ImDrawData* draw_data, ID3D12GraphicsCommandList* graphics_command_list);

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS



IMGUI_IMPL_API bool     ImGui_ImplDX12_Init(ID3D12Device* device, int num_frames_in_flight, DXGI_FORMAT rtv_format, ID3D12DescriptorHeap* srv_descriptor_heap, D3D12_CPU_DESCRIPTOR_HANDLE font_srv_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE font_srv_gpu_desc_handle);
#endif


IMGUI_IMPL_API bool     ImGui_ImplDX12_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplDX12_InvalidateDeviceObjects();


IMGUI_IMPL_API void     ImGui_ImplDX12_UpdateTexture(ImTextureData* tex);




struct ImGui_ImplDX12_RenderState
{
    ID3D12Device*               Device;
    ID3D12GraphicsCommandList*  CommandList;
};

#endif 
