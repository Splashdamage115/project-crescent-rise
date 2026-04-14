
















































#include "imgui.h"

#ifndef IMGUI_DISABLE
#include "imgui_impl_wgpu.h"
#include <limits.h>
#include <stdio.h>
#include <webgpu/webgpu.h>


#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN) == defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
#error Exactly one of IMGUI_IMPL_WEBGPU_BACKEND_DAWN or IMGUI_IMPL_WEBGPU_BACKEND_WGPU must be defined!
#endif



#if defined(__EMSCRIPTEN__) && defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
#define IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN
#endif

#ifdef IMGUI_IMPL_WEBGPU_BACKEND_DAWN


using WGPUProgrammableStageDescriptor = WGPUComputeState;
#endif


extern ImGuiID ImHashData(const void* data_p, size_t data_size, ImU32 seed);
#define MEMALIGN(_SIZE,_ALIGN)        (((_SIZE) + ((_ALIGN) - 1)) & ~((_ALIGN) - 1))    


struct ImGui_ImplWGPU_Texture
{
    WGPUTexture         Texture = nullptr;
    WGPUTextureView     TextureView = nullptr;
};

struct RenderResources
{
    WGPUSampler         Sampler = nullptr;              
    WGPUBuffer          Uniforms = nullptr;             
    WGPUBindGroup       CommonBindGroup = nullptr;      
    ImGuiStorage        ImageBindGroups;                
    WGPUBindGroupLayout ImageBindGroupLayout = nullptr; 
};

struct FrameResources
{
    WGPUBuffer  IndexBuffer;
    WGPUBuffer  VertexBuffer;
    ImDrawIdx*  IndexBufferHost;
    ImDrawVert* VertexBufferHost;
    int         IndexBufferSize;
    int         VertexBufferSize;
};

struct Uniforms
{
    float MVP[4][4];
    float Gamma;
};

struct ImGui_ImplWGPU_Data
{
    ImGui_ImplWGPU_InitInfo initInfo;
    WGPUDevice              wgpuDevice = nullptr;
    WGPUQueue               defaultQueue = nullptr;
    WGPUTextureFormat       renderTargetFormat = WGPUTextureFormat_Undefined;
    WGPUTextureFormat       depthStencilFormat = WGPUTextureFormat_Undefined;
    WGPURenderPipeline      pipelineState = nullptr;

    RenderResources         renderResources;
    FrameResources*         pFrameResources = nullptr;
    unsigned int            numFramesInFlight = 0;
    unsigned int            frameIndex = UINT_MAX;
};



static ImGui_ImplWGPU_Data* ImGui_ImplWGPU_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplWGPU_Data*)ImGui::GetIO().BackendRendererUserData : nullptr;
}





static const char __shader_vert_wgsl[] = R"(
struct VertexInput {
    @location(0) position: vec2<f32>,
    @location(1) uv: vec2<f32>,
    @location(2) color: vec4<f32>,
};

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) color: vec4<f32>,
    @location(1) uv: vec2<f32>,
};

struct Uniforms {
    mvp: mat4x4<f32>,
    gamma: f32,
};

@group(0) @binding(0) var<uniform> uniforms: Uniforms;

@vertex
fn main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    out.position = uniforms.mvp * vec4<f32>(in.position, 0.0, 1.0);
    out.color = in.color;
    out.uv = in.uv;
    return out;
}
)";

static const char __shader_frag_wgsl[] = R"(
struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) color: vec4<f32>,
    @location(1) uv: vec2<f32>,
};

struct Uniforms {
    mvp: mat4x4<f32>,
    gamma: f32,
};

@group(0) @binding(0) var<uniform> uniforms: Uniforms;
@group(0) @binding(1) var s: sampler;
@group(1) @binding(0) var t: texture_2d<f32>;

@fragment
fn main(in: VertexOutput) -> @location(0) vec4<f32> {
    let color = in.color * textureSample(t, s, in.uv);
    let corrected_color = pow(color.rgb, vec3<f32>(uniforms.gamma));
    return vec4<f32>(corrected_color, color.a);
}
)";

static void SafeRelease(ImDrawIdx*& res)
{
    if (res)
        delete[] res;
    res = nullptr;
}
static void SafeRelease(ImDrawVert*& res)
{
    if (res)
        delete[] res;
    res = nullptr;
}
static void SafeRelease(WGPUBindGroupLayout& res)
{
    if (res)
        wgpuBindGroupLayoutRelease(res);
    res = nullptr;
}
static void SafeRelease(WGPUBindGroup& res)
{
    if (res)
        wgpuBindGroupRelease(res);
    res = nullptr;
}
static void SafeRelease(WGPUBuffer& res)
{
    if (res)
        wgpuBufferRelease(res);
    res = nullptr;
}
static void SafeRelease(WGPUPipelineLayout& res)
{
    if (res)
        wgpuPipelineLayoutRelease(res);
    res = nullptr;
}
static void SafeRelease(WGPURenderPipeline& res)
{
    if (res)
        wgpuRenderPipelineRelease(res);
    res = nullptr;
}
static void SafeRelease(WGPUSampler& res)
{
    if (res)
        wgpuSamplerRelease(res);
    res = nullptr;
}
static void SafeRelease(WGPUShaderModule& res)
{
    if (res)
        wgpuShaderModuleRelease(res);
    res = nullptr;
}
static void SafeRelease(RenderResources& res)
{
    SafeRelease(res.Sampler);
    SafeRelease(res.Uniforms);
    SafeRelease(res.CommonBindGroup);
    SafeRelease(res.ImageBindGroupLayout);
};

static void SafeRelease(FrameResources& res)
{
    SafeRelease(res.IndexBuffer);
    SafeRelease(res.VertexBuffer);
    SafeRelease(res.IndexBufferHost);
    SafeRelease(res.VertexBufferHost);
}

static WGPUProgrammableStageDescriptor ImGui_ImplWGPU_CreateShaderModule(const char* wgsl_source)
{
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();

#if !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN)
    WGPUShaderSourceWGSL wgsl_desc = {};
    wgsl_desc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgsl_desc.code = { wgsl_source, WGPU_STRLEN };
#else
    WGPUShaderModuleWGSLDescriptor wgsl_desc = {};
    wgsl_desc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    wgsl_desc.code = wgsl_source;
#endif

    WGPUShaderModuleDescriptor desc = {};
    desc.nextInChain = (WGPUChainedStruct*)&wgsl_desc;

    WGPUProgrammableStageDescriptor stage_desc = {};
    stage_desc.module = wgpuDeviceCreateShaderModule(bd->wgpuDevice, &desc);

#if !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN)
    stage_desc.entryPoint = { "main", WGPU_STRLEN };
#else
    stage_desc.entryPoint = "main";
#endif
    return stage_desc;
}

static WGPUBindGroup ImGui_ImplWGPU_CreateImageBindGroup(WGPUBindGroupLayout layout, WGPUTextureView texture)
{
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();
    WGPUBindGroupEntry image_bg_entries[] = { { nullptr, 0, 0, 0, 0, 0, texture } };

    WGPUBindGroupDescriptor image_bg_descriptor = {};
    image_bg_descriptor.layout = layout;
    image_bg_descriptor.entryCount = sizeof(image_bg_entries) / sizeof(WGPUBindGroupEntry);
    image_bg_descriptor.entries = image_bg_entries;
    return wgpuDeviceCreateBindGroup(bd->wgpuDevice, &image_bg_descriptor);
}

static void ImGui_ImplWGPU_SetupRenderState(ImDrawData* draw_data, WGPURenderPassEncoder ctx, FrameResources* fr)
{
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();

    
    
    {
        float L = draw_data->DisplayPos.x;
        float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
        float T = draw_data->DisplayPos.y;
        float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
        float mvp[4][4] =
        {
            { 2.0f/(R-L),   0.0f,           0.0f,       0.0f },
            { 0.0f,         2.0f/(T-B),     0.0f,       0.0f },
            { 0.0f,         0.0f,           0.5f,       0.0f },
            { (R+L)/(L-R),  (T+B)/(B-T),    0.5f,       1.0f },
        };
        wgpuQueueWriteBuffer(bd->defaultQueue, bd->renderResources.Uniforms, offsetof(Uniforms, MVP), mvp, sizeof(Uniforms::MVP));
        float gamma;
        switch (bd->renderTargetFormat)
        {
        case WGPUTextureFormat_ASTC10x10UnormSrgb:
        case WGPUTextureFormat_ASTC10x5UnormSrgb:
        case WGPUTextureFormat_ASTC10x6UnormSrgb:
        case WGPUTextureFormat_ASTC10x8UnormSrgb:
        case WGPUTextureFormat_ASTC12x10UnormSrgb:
        case WGPUTextureFormat_ASTC12x12UnormSrgb:
        case WGPUTextureFormat_ASTC4x4UnormSrgb:
        case WGPUTextureFormat_ASTC5x5UnormSrgb:
        case WGPUTextureFormat_ASTC6x5UnormSrgb:
        case WGPUTextureFormat_ASTC6x6UnormSrgb:
        case WGPUTextureFormat_ASTC8x5UnormSrgb:
        case WGPUTextureFormat_ASTC8x6UnormSrgb:
        case WGPUTextureFormat_ASTC8x8UnormSrgb:
        case WGPUTextureFormat_BC1RGBAUnormSrgb:
        case WGPUTextureFormat_BC2RGBAUnormSrgb:
        case WGPUTextureFormat_BC3RGBAUnormSrgb:
        case WGPUTextureFormat_BC7RGBAUnormSrgb:
        case WGPUTextureFormat_BGRA8UnormSrgb:
        case WGPUTextureFormat_ETC2RGB8A1UnormSrgb:
        case WGPUTextureFormat_ETC2RGB8UnormSrgb:
        case WGPUTextureFormat_ETC2RGBA8UnormSrgb:
        case WGPUTextureFormat_RGBA8UnormSrgb:
            gamma = 2.2f;
            break;
        default:
            gamma = 1.0f;
        }
        wgpuQueueWriteBuffer(bd->defaultQueue, bd->renderResources.Uniforms, offsetof(Uniforms, Gamma), &gamma, sizeof(Uniforms::Gamma));
    }

    
    wgpuRenderPassEncoderSetViewport(ctx, 0, 0, draw_data->FramebufferScale.x * draw_data->DisplaySize.x, draw_data->FramebufferScale.y * draw_data->DisplaySize.y, 0, 1);

    
    wgpuRenderPassEncoderSetVertexBuffer(ctx, 0, fr->VertexBuffer, 0, fr->VertexBufferSize * sizeof(ImDrawVert));
    wgpuRenderPassEncoderSetIndexBuffer(ctx, fr->IndexBuffer, sizeof(ImDrawIdx) == 2 ? WGPUIndexFormat_Uint16 : WGPUIndexFormat_Uint32, 0, fr->IndexBufferSize * sizeof(ImDrawIdx));
    wgpuRenderPassEncoderSetPipeline(ctx, bd->pipelineState);
    wgpuRenderPassEncoderSetBindGroup(ctx, 0, bd->renderResources.CommonBindGroup, 0, nullptr);

    
    WGPUColor blend_color = { 0.f, 0.f, 0.f, 0.f };
    wgpuRenderPassEncoderSetBlendConstant(ctx, &blend_color);
}



void ImGui_ImplWGPU_RenderDrawData(ImDrawData* draw_data, WGPURenderPassEncoder pass_encoder)
{
    
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0 || draw_data->CmdLists.Size == 0)
        return;

    
    
    if (draw_data->Textures != nullptr)
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
                ImGui_ImplWGPU_UpdateTexture(tex);

    
    
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();
    bd->frameIndex = bd->frameIndex + 1;
    FrameResources* fr = &bd->pFrameResources[bd->frameIndex % bd->numFramesInFlight];

    
    if (fr->VertexBuffer == nullptr || fr->VertexBufferSize < draw_data->TotalVtxCount)
    {
        if (fr->VertexBuffer)
        {
            wgpuBufferDestroy(fr->VertexBuffer);
            wgpuBufferRelease(fr->VertexBuffer);
        }
        SafeRelease(fr->VertexBufferHost);
        fr->VertexBufferSize = draw_data->TotalVtxCount + 5000;

        WGPUBufferDescriptor vb_desc =
        {
            nullptr,
#if !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN)
            { "Dear ImGui Vertex buffer", WGPU_STRLEN, },
#else
            "Dear ImGui Vertex buffer",
#endif
            WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex,
            MEMALIGN(fr->VertexBufferSize * sizeof(ImDrawVert), 4),
            false
        };
        fr->VertexBuffer = wgpuDeviceCreateBuffer(bd->wgpuDevice, &vb_desc);
        if (!fr->VertexBuffer)
            return;

        fr->VertexBufferHost = new ImDrawVert[fr->VertexBufferSize];
    }
    if (fr->IndexBuffer == nullptr || fr->IndexBufferSize < draw_data->TotalIdxCount)
    {
        if (fr->IndexBuffer)
        {
            wgpuBufferDestroy(fr->IndexBuffer);
            wgpuBufferRelease(fr->IndexBuffer);
        }
        SafeRelease(fr->IndexBufferHost);
        fr->IndexBufferSize = draw_data->TotalIdxCount + 10000;

        WGPUBufferDescriptor ib_desc =
        {
            nullptr,
#if !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN)
            { "Dear ImGui Index buffer", WGPU_STRLEN, },
#else
            "Dear ImGui Index buffer",
#endif
            WGPUBufferUsage_CopyDst | WGPUBufferUsage_Index,
            MEMALIGN(fr->IndexBufferSize * sizeof(ImDrawIdx), 4),
            false
        };
        fr->IndexBuffer = wgpuDeviceCreateBuffer(bd->wgpuDevice, &ib_desc);
        if (!fr->IndexBuffer)
            return;

        fr->IndexBufferHost = new ImDrawIdx[fr->IndexBufferSize];
    }

    
    ImDrawVert* vtx_dst = (ImDrawVert*)fr->VertexBufferHost;
    ImDrawIdx* idx_dst = (ImDrawIdx*)fr->IndexBufferHost;
    for (const ImDrawList* draw_list : draw_data->CmdLists)
    {
        memcpy(vtx_dst, draw_list->VtxBuffer.Data, draw_list->VtxBuffer.Size * sizeof(ImDrawVert));
        memcpy(idx_dst, draw_list->IdxBuffer.Data, draw_list->IdxBuffer.Size * sizeof(ImDrawIdx));
        vtx_dst += draw_list->VtxBuffer.Size;
        idx_dst += draw_list->IdxBuffer.Size;
    }
    int64_t vb_write_size = MEMALIGN((char*)vtx_dst - (char*)fr->VertexBufferHost, 4);
    int64_t ib_write_size = MEMALIGN((char*)idx_dst - (char*)fr->IndexBufferHost, 4);
    wgpuQueueWriteBuffer(bd->defaultQueue, fr->VertexBuffer, 0, fr->VertexBufferHost, vb_write_size);
    wgpuQueueWriteBuffer(bd->defaultQueue, fr->IndexBuffer,  0, fr->IndexBufferHost,  ib_write_size);

    
    ImGui_ImplWGPU_SetupRenderState(draw_data, pass_encoder, fr);

    
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    ImGui_ImplWGPU_RenderState render_state;
    render_state.Device = bd->wgpuDevice;
    render_state.RenderPassEncoder = pass_encoder;
    platform_io.Renderer_RenderState = &render_state;

    
    
    int global_vtx_offset = 0;
    int global_idx_offset = 0;
    ImVec2 clip_scale = draw_data->FramebufferScale;
    ImVec2 clip_off = draw_data->DisplayPos;
    for (const ImDrawList* draw_list : draw_data->CmdLists)
    {
        for (int cmd_i = 0; cmd_i < draw_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &draw_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != nullptr)
            {
                
                
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_ImplWGPU_SetupRenderState(draw_data, pass_encoder, fr);
                else
                    pcmd->UserCallback(draw_list, pcmd);
            }
            else
            {
                
                ImTextureID tex_id = pcmd->GetTexID();
                ImGuiID tex_id_hash = ImHashData(&tex_id, sizeof(tex_id), 0);
                WGPUBindGroup bind_group = (WGPUBindGroup)bd->renderResources.ImageBindGroups.GetVoidPtr(tex_id_hash);
                if (!bind_group)
                {
                    bind_group = ImGui_ImplWGPU_CreateImageBindGroup(bd->renderResources.ImageBindGroupLayout, (WGPUTextureView)tex_id);
                    bd->renderResources.ImageBindGroups.SetVoidPtr(tex_id_hash, bind_group);
                }
                wgpuRenderPassEncoderSetBindGroup(pass_encoder, 1, (WGPUBindGroup)bind_group, 0, nullptr);

                
                ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

                
                if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
                if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
                if (clip_max.x > fb_width) { clip_max.x = (float)fb_width; }
                if (clip_max.y > fb_height) { clip_max.y = (float)fb_height; }
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                
                wgpuRenderPassEncoderSetScissorRect(pass_encoder, (uint32_t)clip_min.x, (uint32_t)clip_min.y, (uint32_t)(clip_max.x - clip_min.x), (uint32_t)(clip_max.y - clip_min.y));
                wgpuRenderPassEncoderDrawIndexed(pass_encoder, pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0);
            }
        }
        global_idx_offset += draw_list->IdxBuffer.Size;
        global_vtx_offset += draw_list->VtxBuffer.Size;
    }

    
    ImGuiStorage& image_bind_groups = bd->renderResources.ImageBindGroups;
    for (int i = 0; i < image_bind_groups.Data.Size; i++)
    {
        WGPUBindGroup bind_group = (WGPUBindGroup)image_bind_groups.Data[i].val_p;
        SafeRelease(bind_group);
    }
    image_bind_groups.Data.resize(0);

    platform_io.Renderer_RenderState = nullptr;
}

static void ImGui_ImplWGPU_DestroyTexture(ImTextureData* tex)
{
    if (ImGui_ImplWGPU_Texture* backend_tex = (ImGui_ImplWGPU_Texture*)tex->BackendUserData)
    {
        IM_ASSERT(backend_tex->TextureView == (WGPUTextureView)(intptr_t)tex->TexID);
        wgpuTextureViewRelease(backend_tex->TextureView);
        wgpuTextureRelease(backend_tex->Texture);
        IM_DELETE(backend_tex);

        
        tex->SetTexID(ImTextureID_Invalid);
        tex->BackendUserData = nullptr;
    }
    tex->SetStatus(ImTextureStatus_Destroyed);
}

void ImGui_ImplWGPU_UpdateTexture(ImTextureData* tex)
{
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();
    if (tex->Status == ImTextureStatus_WantCreate)
    {
        
        
        IM_ASSERT(tex->TexID == ImTextureID_Invalid && tex->BackendUserData == nullptr);
        IM_ASSERT(tex->Format == ImTextureFormat_RGBA32);
        ImGui_ImplWGPU_Texture* backend_tex = IM_NEW(ImGui_ImplWGPU_Texture)();

        
        WGPUTextureDescriptor tex_desc = {};
#if !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN)
        tex_desc.label = { "Dear ImGui Texture", WGPU_STRLEN };
#else
        tex_desc.label = "Dear ImGui Texture";
#endif
        tex_desc.dimension = WGPUTextureDimension_2D;
        tex_desc.size.width = tex->Width;
        tex_desc.size.height = tex->Height;
        tex_desc.size.depthOrArrayLayers = 1;
        tex_desc.sampleCount = 1;
        tex_desc.format = WGPUTextureFormat_RGBA8Unorm;
        tex_desc.mipLevelCount = 1;
        tex_desc.usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding;
        backend_tex->Texture = wgpuDeviceCreateTexture(bd->wgpuDevice, &tex_desc);

        
        WGPUTextureViewDescriptor tex_view_desc = {};
        tex_view_desc.format = WGPUTextureFormat_RGBA8Unorm;
        tex_view_desc.dimension = WGPUTextureViewDimension_2D;
        tex_view_desc.baseMipLevel = 0;
        tex_view_desc.mipLevelCount = 1;
        tex_view_desc.baseArrayLayer = 0;
        tex_view_desc.arrayLayerCount = 1;
        tex_view_desc.aspect = WGPUTextureAspect_All;
        backend_tex->TextureView = wgpuTextureCreateView(backend_tex->Texture, &tex_view_desc);

        
        tex->SetTexID((ImTextureID)(intptr_t)backend_tex->TextureView);
        tex->BackendUserData = backend_tex;
        
    }

    if (tex->Status == ImTextureStatus_WantCreate || tex->Status == ImTextureStatus_WantUpdates)
    {
        ImGui_ImplWGPU_Texture* backend_tex = (ImGui_ImplWGPU_Texture*)tex->BackendUserData;
        IM_ASSERT(tex->Format == ImTextureFormat_RGBA32);

        
        const int upload_x = (tex->Status == ImTextureStatus_WantCreate) ? 0 : tex->UpdateRect.x;
        const int upload_y = (tex->Status == ImTextureStatus_WantCreate) ? 0 : tex->UpdateRect.y;
        const int upload_w = (tex->Status == ImTextureStatus_WantCreate) ? tex->Width : tex->UpdateRect.w;
        const int upload_h = (tex->Status == ImTextureStatus_WantCreate) ? tex->Height : tex->UpdateRect.h;

        
        
#if !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN)
        WGPUTexelCopyTextureInfo dst_view = {};
#else
        WGPUImageCopyTexture dst_view = {};
#endif
        dst_view.texture = backend_tex->Texture;
        dst_view.mipLevel = 0;
        dst_view.origin = { (uint32_t)upload_x, (uint32_t)upload_y, 0 };
        dst_view.aspect = WGPUTextureAspect_All;
#if !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN)
        WGPUTexelCopyBufferLayout layout = {};
#else
        WGPUTextureDataLayout layout = {};
#endif
        layout.offset = 0;
        layout.bytesPerRow = tex->Width * tex->BytesPerPixel;
        layout.rowsPerImage = upload_h;
        WGPUExtent3D write_size = { (uint32_t)upload_w, (uint32_t)upload_h, 1 };
        wgpuQueueWriteTexture(bd->defaultQueue, &dst_view, tex->GetPixelsAt(upload_x, upload_y), (uint32_t)(tex->Width * upload_h * tex->BytesPerPixel), &layout, &write_size);
        tex->SetStatus(ImTextureStatus_OK);
    }
    if (tex->Status == ImTextureStatus_WantDestroy && tex->UnusedFrames > 0)
        ImGui_ImplWGPU_DestroyTexture(tex);
}

static void ImGui_ImplWGPU_CreateUniformBuffer()
{
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();
    WGPUBufferDescriptor ub_desc =
    {
        nullptr,
#if !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN)
        { "Dear ImGui Uniform buffer", WGPU_STRLEN, },
#else
        "Dear ImGui Uniform buffer",
#endif
        WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform,
        MEMALIGN(sizeof(Uniforms), 16),
        false
    };
    bd->renderResources.Uniforms = wgpuDeviceCreateBuffer(bd->wgpuDevice, &ub_desc);
}

bool ImGui_ImplWGPU_CreateDeviceObjects()
{
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();
    if (!bd->wgpuDevice)
        return false;
    if (bd->pipelineState)
        ImGui_ImplWGPU_InvalidateDeviceObjects();

    
    WGPURenderPipelineDescriptor graphics_pipeline_desc = {};
    graphics_pipeline_desc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    graphics_pipeline_desc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
    graphics_pipeline_desc.primitive.frontFace = WGPUFrontFace_CW;
    graphics_pipeline_desc.primitive.cullMode = WGPUCullMode_None;
    graphics_pipeline_desc.multisample = bd->initInfo.PipelineMultisampleState;

    
    WGPUBindGroupLayoutEntry common_bg_layout_entries[2] = {};
    common_bg_layout_entries[0].binding = 0;
    common_bg_layout_entries[0].visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    common_bg_layout_entries[0].buffer.type = WGPUBufferBindingType_Uniform;
    common_bg_layout_entries[1].binding = 1;
    common_bg_layout_entries[1].visibility = WGPUShaderStage_Fragment;
    common_bg_layout_entries[1].sampler.type = WGPUSamplerBindingType_Filtering;

    WGPUBindGroupLayoutEntry image_bg_layout_entries[1] = {};
    image_bg_layout_entries[0].binding = 0;
    image_bg_layout_entries[0].visibility = WGPUShaderStage_Fragment;
    image_bg_layout_entries[0].texture.sampleType = WGPUTextureSampleType_Float;
    image_bg_layout_entries[0].texture.viewDimension = WGPUTextureViewDimension_2D;

    WGPUBindGroupLayoutDescriptor common_bg_layout_desc = {};
    common_bg_layout_desc.entryCount = 2;
    common_bg_layout_desc.entries = common_bg_layout_entries;

    WGPUBindGroupLayoutDescriptor image_bg_layout_desc = {};
    image_bg_layout_desc.entryCount = 1;
    image_bg_layout_desc.entries = image_bg_layout_entries;

    WGPUBindGroupLayout bg_layouts[2];
    bg_layouts[0] = wgpuDeviceCreateBindGroupLayout(bd->wgpuDevice, &common_bg_layout_desc);
    bg_layouts[1] = wgpuDeviceCreateBindGroupLayout(bd->wgpuDevice, &image_bg_layout_desc);

    WGPUPipelineLayoutDescriptor layout_desc = {};
    layout_desc.bindGroupLayoutCount = 2;
    layout_desc.bindGroupLayouts = bg_layouts;
    graphics_pipeline_desc.layout = wgpuDeviceCreatePipelineLayout(bd->wgpuDevice, &layout_desc);

    
    WGPUProgrammableStageDescriptor vertex_shader_desc = ImGui_ImplWGPU_CreateShaderModule(__shader_vert_wgsl);
    graphics_pipeline_desc.vertex.module = vertex_shader_desc.module;
    graphics_pipeline_desc.vertex.entryPoint = vertex_shader_desc.entryPoint;

    
    WGPUVertexAttribute attribute_desc[] =
    {
#ifdef IMGUI_IMPL_WEBGPU_BACKEND_DAWN
        { nullptr, WGPUVertexFormat_Float32x2, (uint64_t)offsetof(ImDrawVert, pos), 0 },
        { nullptr, WGPUVertexFormat_Float32x2, (uint64_t)offsetof(ImDrawVert, uv),  1 },
        { nullptr, WGPUVertexFormat_Unorm8x4,  (uint64_t)offsetof(ImDrawVert, col), 2 },
#else
        { WGPUVertexFormat_Float32x2, (uint64_t)offsetof(ImDrawVert, pos), 0 },
        { WGPUVertexFormat_Float32x2, (uint64_t)offsetof(ImDrawVert, uv),  1 },
        { WGPUVertexFormat_Unorm8x4,  (uint64_t)offsetof(ImDrawVert, col), 2 },
#endif
    };

    WGPUVertexBufferLayout buffer_layouts[1];
    buffer_layouts[0].arrayStride = sizeof(ImDrawVert);
    buffer_layouts[0].stepMode = WGPUVertexStepMode_Vertex;
    buffer_layouts[0].attributeCount = 3;
    buffer_layouts[0].attributes = attribute_desc;

    graphics_pipeline_desc.vertex.bufferCount = 1;
    graphics_pipeline_desc.vertex.buffers = buffer_layouts;

    
    WGPUProgrammableStageDescriptor pixel_shader_desc = ImGui_ImplWGPU_CreateShaderModule(__shader_frag_wgsl);

    
    WGPUBlendState blend_state = {};
    blend_state.alpha.operation = WGPUBlendOperation_Add;
    blend_state.alpha.srcFactor = WGPUBlendFactor_One;
    blend_state.alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blend_state.color.operation = WGPUBlendOperation_Add;
    blend_state.color.srcFactor = WGPUBlendFactor_SrcAlpha;
    blend_state.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;

    WGPUColorTargetState color_state = {};
    color_state.format = bd->renderTargetFormat;
    color_state.blend = &blend_state;
    color_state.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragment_state = {};
    fragment_state.module = pixel_shader_desc.module;
    fragment_state.entryPoint = pixel_shader_desc.entryPoint;
    fragment_state.targetCount = 1;
    fragment_state.targets = &color_state;

    graphics_pipeline_desc.fragment = &fragment_state;

    
    WGPUDepthStencilState depth_stencil_state = {};
    depth_stencil_state.format = bd->depthStencilFormat;
#if !defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU_EMSCRIPTEN)
    depth_stencil_state.depthWriteEnabled = WGPUOptionalBool_False;
#else
    depth_stencil_state.depthWriteEnabled = false;
#endif
    depth_stencil_state.depthCompare = WGPUCompareFunction_Always;
    depth_stencil_state.stencilFront.compare = WGPUCompareFunction_Always;
    depth_stencil_state.stencilFront.failOp = WGPUStencilOperation_Keep;
    depth_stencil_state.stencilFront.depthFailOp = WGPUStencilOperation_Keep;
    depth_stencil_state.stencilFront.passOp = WGPUStencilOperation_Keep;
    depth_stencil_state.stencilBack.compare = WGPUCompareFunction_Always;
    depth_stencil_state.stencilBack.failOp = WGPUStencilOperation_Keep;
    depth_stencil_state.stencilBack.depthFailOp = WGPUStencilOperation_Keep;
    depth_stencil_state.stencilBack.passOp = WGPUStencilOperation_Keep;

    
    graphics_pipeline_desc.depthStencil = (bd->depthStencilFormat == WGPUTextureFormat_Undefined) ? nullptr :  &depth_stencil_state;

    bd->pipelineState = wgpuDeviceCreateRenderPipeline(bd->wgpuDevice, &graphics_pipeline_desc);

    ImGui_ImplWGPU_CreateUniformBuffer();

    
    
    WGPUSamplerDescriptor sampler_desc = {};
    sampler_desc.minFilter = WGPUFilterMode_Linear;
    sampler_desc.magFilter = WGPUFilterMode_Linear;
    sampler_desc.mipmapFilter = WGPUMipmapFilterMode_Linear;
    sampler_desc.addressModeU = WGPUAddressMode_ClampToEdge;
    sampler_desc.addressModeV = WGPUAddressMode_ClampToEdge;
    sampler_desc.addressModeW = WGPUAddressMode_ClampToEdge;
    sampler_desc.maxAnisotropy = 1;
    bd->renderResources.Sampler = wgpuDeviceCreateSampler(bd->wgpuDevice, &sampler_desc);

    
    WGPUBindGroupEntry common_bg_entries[] =
    {
        { nullptr, 0, bd->renderResources.Uniforms, 0, MEMALIGN(sizeof(Uniforms), 16), 0, 0 },
        { nullptr, 1, 0, 0, 0, bd->renderResources.Sampler, 0 },
    };
    WGPUBindGroupDescriptor common_bg_descriptor = {};
    common_bg_descriptor.layout = bg_layouts[0];
    common_bg_descriptor.entryCount = sizeof(common_bg_entries) / sizeof(WGPUBindGroupEntry);
    common_bg_descriptor.entries = common_bg_entries;
    bd->renderResources.CommonBindGroup = wgpuDeviceCreateBindGroup(bd->wgpuDevice, &common_bg_descriptor);
    bd->renderResources.ImageBindGroupLayout = bg_layouts[1];

    SafeRelease(vertex_shader_desc.module);
    SafeRelease(pixel_shader_desc.module);
    SafeRelease(graphics_pipeline_desc.layout);
    SafeRelease(bg_layouts[0]);

    return true;
}

void ImGui_ImplWGPU_InvalidateDeviceObjects()
{
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();
    if (!bd->wgpuDevice)
        return;

    SafeRelease(bd->pipelineState);
    SafeRelease(bd->renderResources);

    
    for (ImTextureData* tex : ImGui::GetPlatformIO().Textures)
        if (tex->RefCount == 1)
            ImGui_ImplWGPU_DestroyTexture(tex);

    for (unsigned int i = 0; i < bd->numFramesInFlight; i++)
        SafeRelease(bd->pFrameResources[i]);
}

bool ImGui_ImplWGPU_Init(ImGui_ImplWGPU_InitInfo* init_info)
{
    ImGuiIO& io = ImGui::GetIO();
    IMGUI_CHECKVERSION();
    IM_ASSERT(io.BackendRendererUserData == nullptr && "Already initialized a renderer backend!");

    
    ImGui_ImplWGPU_Data* bd = IM_NEW(ImGui_ImplWGPU_Data)();
    io.BackendRendererUserData = (void*)bd;
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
#if defined(__EMSCRIPTEN__)
    io.BackendRendererName = "imgui_impl_wgpu (Dawn, Emscripten)"; 
#else
    io.BackendRendererName = "imgui_impl_wgpu (Dawn, Native)";
#endif
#elif defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
#if defined(__EMSCRIPTEN__)
    io.BackendRendererName = "imgui_impl_wgpu (WGPU, Emscripten)"; 
#else
    io.BackendRendererName = "imgui_impl_wgpu (WGPU, Native)";
#endif
#endif
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  
    io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;   

    bd->initInfo = *init_info;
    bd->wgpuDevice = init_info->Device;
    bd->defaultQueue = wgpuDeviceGetQueue(bd->wgpuDevice);
    bd->renderTargetFormat = init_info->RenderTargetFormat;
    bd->depthStencilFormat = init_info->DepthStencilFormat;
    bd->numFramesInFlight = init_info->NumFramesInFlight;
    bd->frameIndex = UINT_MAX;

    bd->renderResources.Sampler = nullptr;
    bd->renderResources.Uniforms = nullptr;
    bd->renderResources.CommonBindGroup = nullptr;
    bd->renderResources.ImageBindGroups.Data.reserve(100);
    bd->renderResources.ImageBindGroupLayout = nullptr;

    
    bd->pFrameResources = new FrameResources[bd->numFramesInFlight];
    for (unsigned int i = 0; i < bd->numFramesInFlight; i++)
    {
        FrameResources* fr = &bd->pFrameResources[i];
        fr->IndexBuffer = nullptr;
        fr->VertexBuffer = nullptr;
        fr->IndexBufferHost = nullptr;
        fr->VertexBufferHost = nullptr;
        fr->IndexBufferSize = 10000;
        fr->VertexBufferSize = 5000;
    }

    return true;
}

void ImGui_ImplWGPU_Shutdown()
{
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();
    IM_ASSERT(bd != nullptr && "No renderer backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

    ImGui_ImplWGPU_InvalidateDeviceObjects();
    delete[] bd->pFrameResources;
    bd->pFrameResources = nullptr;
    wgpuQueueRelease(bd->defaultQueue);
    bd->wgpuDevice = nullptr;
    bd->numFramesInFlight = 0;
    bd->frameIndex = UINT_MAX;

    io.BackendRendererName = nullptr;
    io.BackendRendererUserData = nullptr;
    io.BackendFlags &= ~(ImGuiBackendFlags_RendererHasVtxOffset | ImGuiBackendFlags_RendererHasTextures);
    platform_io.ClearRendererHandlers();
    IM_DELETE(bd);
}

void ImGui_ImplWGPU_NewFrame()
{
    ImGui_ImplWGPU_Data* bd = ImGui_ImplWGPU_GetBackendData();
    if (!bd->pipelineState)
        if (!ImGui_ImplWGPU_CreateDeviceObjects())
            IM_ASSERT(0 && "ImGui_ImplWGPU_CreateDeviceObjects() failed!");
}






bool ImGui_ImplWGPU_IsSurfaceStatusError(WGPUSurfaceGetCurrentTextureStatus status)
{
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
    return (status == WGPUSurfaceGetCurrentTextureStatus_Error);
#else
    return (status == WGPUSurfaceGetCurrentTextureStatus_OutOfMemory || status == WGPUSurfaceGetCurrentTextureStatus_DeviceLost);
#endif
}

bool ImGui_ImplWGPU_IsSurfaceStatusSubOptimal(WGPUSurfaceGetCurrentTextureStatus status)
{
#if defined(__EMSCRIPTEN__) && !defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
    return (status == WGPUSurfaceGetCurrentTextureStatus_Timeout || status == WGPUSurfaceGetCurrentTextureStatus_Outdated || status == WGPUSurfaceGetCurrentTextureStatus_Lost);
#else
    return (status == WGPUSurfaceGetCurrentTextureStatus_Timeout || status == WGPUSurfaceGetCurrentTextureStatus_Outdated || status == WGPUSurfaceGetCurrentTextureStatus_Lost || status == WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal);
#endif
}


#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
const char* ImGui_ImplWGPU_GetErrorTypeName(WGPUErrorType type)
{
    switch (type)
    {
    case WGPUErrorType_Validation: return "Validation";
    case WGPUErrorType_OutOfMemory: return "OutOfMemory";
    case WGPUErrorType_Unknown: return "Unknown";
    case WGPUErrorType_Internal: return "Internal";
    default: return "Unknown";
    }
}
const char* ImGui_ImplWGPU_GetDeviceLostReasonName(WGPUDeviceLostReason type)
{
    switch (type)
    {
    case WGPUDeviceLostReason_Unknown: return "Unknown";
    case WGPUDeviceLostReason_Destroyed: return "Destroyed";
    case WGPUDeviceLostReason_CallbackCancelled: return "CallbackCancelled";
    case WGPUDeviceLostReason_FailedCreation: return "FailedCreation";
    default: return "Unknown";
    }
}
#elif !defined(__EMSCRIPTEN__)
const char* ImGui_ImplWGPU_GetLogLevelName(WGPULogLevel level)
{
    switch (level)
    {
    case WGPULogLevel_Error: return "Error"; 
    case WGPULogLevel_Warn: return "Warn";
    case WGPULogLevel_Info: return "Info";
    case WGPULogLevel_Debug: return "Debug";
    case WGPULogLevel_Trace: return "Trace";
    default: return "Unknown";
    }
}
#endif

const char* ImGui_ImplWGPU_GetBackendTypeName(WGPUBackendType type)
{
    switch (type)
    {
    case WGPUBackendType_WebGPU: return "WebGPU";
    case WGPUBackendType_D3D11: return "D3D11";
    case WGPUBackendType_D3D12: return "D3D12";
    case WGPUBackendType_Metal: return "Metal";
    case WGPUBackendType_Vulkan: return "Vulkan";
    case WGPUBackendType_OpenGL: return "OpenGL";
    case WGPUBackendType_OpenGLES: return "OpenGLES";
    default: return "Unknown";
    }
}

const char* ImGui_ImplWGPU_GetAdapterTypeName(WGPUAdapterType type)
{
    switch (type)
    {
    case WGPUAdapterType_DiscreteGPU: return "DiscreteGPU";
    case WGPUAdapterType_IntegratedGPU: return "IntegratedGPU";
    case WGPUAdapterType_CPU: return "CPU";
    default: return "Unknown";
    }
}

void ImGui_ImplWGPU_DebugPrintAdapterInfo(const WGPUAdapter& adapter)
{
    WGPUAdapterInfo info = {};
    wgpuAdapterGetInfo(adapter, &info);
    printf("description: \"%.*s\"\n", (int)info.description.length, info.description.data);
    printf("vendor: \"%.*s\", vendorID: %x\n", (int)info.vendor.length, info.vendor.data, info.vendorID);
    printf("architecture: \"%.*s\"\n", (int) info.architecture.length, info.architecture.data);
    printf("device: \"%.*s\", deviceID: %x\n", (int)info.device.length, info.device.data, info.deviceID);
    printf("backendType: \"%s\"\n", ImGui_ImplWGPU_GetBackendTypeName(info.backendType));
    printf("adapterType: \"%s\"\n", ImGui_ImplWGPU_GetAdapterTypeName(info.adapterType));
    wgpuAdapterInfoFreeMembers(info);
}

#if defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU) || defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN) && !defined(__EMSCRIPTEN__)

#if defined(__APPLE__)


#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#endif

WGPUSurface ImGui_ImplWGPU_CreateWGPUSurfaceHelper(ImGui_ImplWGPU_CreateSurfaceInfo* info)
{
    WGPUSurfaceDescriptor surface_descriptor = {};
    WGPUSurface surface = {};
#if defined(__APPLE__)
    if (strcmp(info->System, "cocoa") == 0)
    {
        IM_ASSERT(info->RawWindow != nullptr);
        NSWindow* ns_window = (NSWindow*)info->RawWindow;
        id metal_layer = [CAMetalLayer layer];
        [ns_window.contentView setWantsLayer : YES] ;
        [ns_window.contentView setLayer : metal_layer] ;
        WGPUSurfaceSourceMetalLayer surface_src_metal = {};
        surface_src_metal.chain.sType = WGPUSType_SurfaceSourceMetalLayer;
        surface_src_metal.layer = metal_layer;
        surface_descriptor.nextInChain = &surface_src_metal.chain;
        surface = wgpuInstanceCreateSurface(info->Instance, &surface_descriptor);
    }
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    if (strcmp(info->System, "wayland") == 0)
    {
        IM_ASSERT(info->RawDisplay != nullptr && info->RawSurface != nullptr);
        WGPUSurfaceSourceWaylandSurface surface_src_wayland = {};
        surface_src_wayland.chain.sType = WGPUSType_SurfaceSourceWaylandSurface;
        surface_src_wayland.display = info->RawDisplay;
        surface_src_wayland.surface = info->RawSurface;
        surface_descriptor.nextInChain = &surface_src_wayland.chain;
        surface = wgpuInstanceCreateSurface(info->Instance, &surface_descriptor);
    }
    else if (strcmp(info->System, "x11") == 0)
    {
        IM_ASSERT(info->RawDisplay != nullptr && info->RawWindow != nullptr);
        WGPUSurfaceSourceXlibWindow surface_src_xlib = {};
        surface_src_xlib.chain.sType = WGPUSType_SurfaceSourceXlibWindow;
        surface_src_xlib.display = info->RawDisplay;
        surface_src_xlib.window = (uint64_t)info->RawWindow;
        surface_descriptor.nextInChain = &surface_src_xlib.chain;
        surface = wgpuInstanceCreateSurface(info->Instance, &surface_descriptor);
    }
#elif defined(_WIN32)
    if (strcmp(info->System, "win32") == 0)
    {
        IM_ASSERT(info->RawWindow != nullptr && info->RawInstance != nullptr);
        WGPUSurfaceSourceWindowsHWND surface_src_hwnd = {};
        surface_src_hwnd.chain.sType = WGPUSType_SurfaceSourceWindowsHWND;
        surface_src_hwnd.hinstance = info->RawInstance;
        surface_src_hwnd.hwnd = info->RawWindow;
        surface_descriptor.nextInChain = &surface_src_hwnd.chain;
        surface = wgpuInstanceCreateSurface(info->Instance, &surface_descriptor);
    }
#else
#error "Unsupported WebGPU native platform!"
#endif
    return surface;
}
#endif



#endif 
