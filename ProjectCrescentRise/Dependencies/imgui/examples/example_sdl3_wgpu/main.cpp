










#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_wgpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
#include <emscripten/html5_webgpu.h>
#endif
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#include <webgpu/webgpu.h>
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
#include <webgpu/webgpu_cpp.h>
#endif


WGPUInstance             wgpu_instance = nullptr;
WGPUDevice               wgpu_device = nullptr;
WGPUSurface              wgpu_surface = nullptr;
WGPUQueue                wgpu_queue = nullptr;
WGPUSurfaceConfiguration wgpu_surface_configuration = {};
int                      wgpu_surface_width = 1280;
int                      wgpu_surface_height = 800;


static bool InitWGPU(SDL_Window* window);

void ResizeSurface(int width, int height)
{
    wgpu_surface_configuration.width  = wgpu_surface_width  = width;
    wgpu_surface_configuration.height = wgpu_surface_height = height;
    wgpuSurfaceConfigure( wgpu_surface, (WGPUSurfaceConfiguration*)&wgpu_surface_configuration );
}


int main(int, char**)
{
    
    
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE;
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL3+WebGPU example", wgpu_surface_width, wgpu_surface_height, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }

    
    InitWGPU(window);

    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      

    
    ImGui::StyleColorsDark();
    

    
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        
    style.FontScaleDpi = main_scale;        

    
    ImGui_ImplSDL3_InitForOther(window);

    ImGui_ImplWGPU_InitInfo init_info;
    init_info.Device = wgpu_device;
    init_info.NumFramesInFlight = 3;
    init_info.RenderTargetFormat = wgpu_surface_configuration.format;
    init_info.DepthStencilFormat = WGPUTextureFormat_Undefined;
    ImGui_ImplWGPU_Init(&init_info);

    
    
    
    
    
    
    
    
    
    
#ifndef IMGUI_DISABLE_FILE_FUNCTIONS
    
    
    
    
    
    
#endif

    
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    
    bool done = false;
#ifdef __EMSCRIPTEN__
    
    
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!done)
#endif
    {
        
        
        
        
        
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        
        
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        if (width != wgpu_surface_width || height != wgpu_surface_height)
            ResizeSurface(width, height);

        
        WGPUSurfaceTexture surface_texture;
        wgpuSurfaceGetCurrentTexture(wgpu_surface, &surface_texture);
        if (ImGui_ImplWGPU_IsSurfaceStatusError(surface_texture.status))
        {
            fprintf(stderr, "Unrecoverable Surface Texture status=%#.8x\n", surface_texture.status);
            abort();
        }
        if (ImGui_ImplWGPU_IsSurfaceStatusSubOptimal(surface_texture.status))
        {
            if (surface_texture.texture)
                wgpuTextureRelease(surface_texture.texture);
            if (width > 0 && height > 0)
                ResizeSurface(width, height);
            continue;
        }

        
        ImGui_ImplWGPU_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                                

            ImGui::Text("This is some useful text.");                     
            ImGui::Checkbox("Demo Window", &show_demo_window);            
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                  
            ImGui::ColorEdit3("clear color", (float*)&clear_color);       

            if (ImGui::Button("Button"))                                  
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);         
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        
        ImGui::Render();

        WGPUTextureViewDescriptor view_desc = {};
        view_desc.format = wgpu_surface_configuration.format;
        view_desc.dimension = WGPUTextureViewDimension_2D;
        view_desc.mipLevelCount = WGPU_MIP_LEVEL_COUNT_UNDEFINED;
        view_desc.arrayLayerCount = WGPU_ARRAY_LAYER_COUNT_UNDEFINED;
        view_desc.aspect = WGPUTextureAspect_All;

        WGPUTextureView texture_view = wgpuTextureCreateView(surface_texture.texture, &view_desc);

        WGPURenderPassColorAttachment color_attachments = {};
        color_attachments.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
        color_attachments.loadOp = WGPULoadOp_Clear;
        color_attachments.storeOp = WGPUStoreOp_Store;
        color_attachments.clearValue = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        color_attachments.view = texture_view;

        WGPURenderPassDescriptor render_pass_desc = {};
        render_pass_desc.colorAttachmentCount = 1;
        render_pass_desc.colorAttachments = &color_attachments;
        render_pass_desc.depthStencilAttachment = nullptr;

        WGPUCommandEncoderDescriptor enc_desc = {};
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(wgpu_device, &enc_desc);

        WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);
        ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
        wgpuRenderPassEncoderEnd(pass);

        WGPUCommandBufferDescriptor cmd_buffer_desc = {};
        WGPUCommandBuffer cmd_buffer = wgpuCommandEncoderFinish(encoder, &cmd_buffer_desc);
        wgpuQueueSubmit(wgpu_queue, 1, &cmd_buffer);

#ifndef __EMSCRIPTEN__
        wgpuSurfacePresent(wgpu_surface);
        
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
        wgpuDeviceTick(wgpu_device);
#endif
#endif
        wgpuTextureViewRelease(texture_view);
        wgpuRenderPassEncoderRelease(pass);
        wgpuCommandEncoderRelease(encoder);
        wgpuCommandBufferRelease(cmd_buffer);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    
    
    ImGui_ImplWGPU_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    wgpuSurfaceUnconfigure(wgpu_surface);
    wgpuSurfaceRelease(wgpu_surface);
    wgpuQueueRelease(wgpu_queue);
    wgpuDeviceRelease(wgpu_device);
    wgpuInstanceRelease(wgpu_instance);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}





#if !defined(__EMSCRIPTEN__) && (defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU) || defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN))

#if defined(SDL_PLATFORM_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#endif

WGPUSurface CreateWGPUSurface(const WGPUInstance& instance, SDL_Window* window)
{
    SDL_PropertiesID propertiesID = SDL_GetWindowProperties(window);

    ImGui_ImplWGPU_CreateSurfaceInfo create_info = {};
    create_info.Instance = instance;
#if defined(SDL_PLATFORM_MACOS)
    {
        create_info.System = "cocoa";
        create_info.RawWindow = (void*)SDL_GetPointerProperty(propertiesID, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0)
    {
        create_info.System = "wayland";
        create_info.RawDisplay = (void*)SDL_GetPointerProperty(propertiesID, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
        create_info.RawSurface = (void*)SDL_GetPointerProperty(propertiesID, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
    else if (!SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11"))
    {
        create_info.System = "x11";
        create_info.RawWindow = (void*)SDL_GetNumberProperty(propertiesID, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        create_info.RawDisplay = (void*)SDL_GetPointerProperty(propertiesID, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#elif defined(SDL_PLATFORM_WIN32)
    {
        create_info.System = "win32";
        create_info.RawWindow = (void*)SDL_GetPointerProperty(propertiesID, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
        create_info.RawInstance = (void*)::GetModuleHandle(NULL);
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#else
#error "Unsupported WebGPU native platform!"
#endif
    return nullptr;
}
#endif

#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
static WGPUAdapter RequestAdapter(wgpu::Instance& instance)
{
    wgpu::Adapter acquired_adapter;
    wgpu::RequestAdapterOptions adapter_options;
    auto onRequestAdapter = [&](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message)
    {
        if (status != wgpu::RequestAdapterStatus::Success)
        {
            printf("Failed to get an adapter: %s\n", message.data);
            return;
        }
        acquired_adapter = std::move(adapter);
    };

    
    wgpu::Future waitAdapterFunc { instance.RequestAdapter(&adapter_options, wgpu::CallbackMode::WaitAnyOnly, onRequestAdapter) };
    wgpu::WaitStatus waitStatusAdapter = instance.WaitAny(waitAdapterFunc, UINT64_MAX);
    IM_ASSERT(acquired_adapter != nullptr && waitStatusAdapter == wgpu::WaitStatus::Success && "Error on Adapter request");
    return acquired_adapter.MoveToCHandle();
}

static WGPUDevice RequestDevice(wgpu::Instance& instance, wgpu::Adapter& adapter)
{
    
    wgpu::DeviceDescriptor device_desc;
    device_desc.SetDeviceLostCallback(wgpu::CallbackMode::AllowSpontaneous,
        [](const wgpu::Device&, wgpu::DeviceLostReason type, wgpu::StringView msg) { fprintf(stderr, "%s error: %s\n", ImGui_ImplWGPU_GetDeviceLostReasonName((WGPUDeviceLostReason)type), msg.data); }
    );
    device_desc.SetUncapturedErrorCallback(
        [](const wgpu::Device&, wgpu::ErrorType type, wgpu::StringView msg) { fprintf(stderr, "%s error: %s\n", ImGui_ImplWGPU_GetErrorTypeName((WGPUErrorType)type), msg.data); }
    );

    wgpu::Device acquired_device;
    auto onRequestDevice = [&](wgpu::RequestDeviceStatus status, wgpu::Device local_device, wgpu::StringView message)
    {
        if (status != wgpu::RequestDeviceStatus::Success)
        {
            printf("Failed to get an device: %s\n", message.data);
            return;
        }
        acquired_device = std::move(local_device);
    };

    
    wgpu::Future waitDeviceFunc { adapter.RequestDevice(&device_desc, wgpu::CallbackMode::WaitAnyOnly, onRequestDevice) };
    wgpu::WaitStatus waitStatusDevice = instance.WaitAny(waitDeviceFunc, UINT64_MAX);
    IM_ASSERT(acquired_device != nullptr && waitStatusDevice == wgpu::WaitStatus::Success && "Error on Device request");
    return acquired_device.MoveToCHandle();
}
#elif defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
#ifdef __EMSCRIPTEN__

EM_ASYNC_JS( void, getAdapterAndDeviceViaJS, (),
{
    if (!navigator.gpu)
        throw Error("WebGPU not supported.");
    const adapter = await navigator.gpu.requestAdapter();
    const device = await adapter.requestDevice();
    Module.preinitializedWebGPUDevice = device;
} );
#else 
static void handle_request_adapter(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void* userdata1, void* userdata2)
{
    if (status == WGPURequestAdapterStatus_Success)
    {
        WGPUAdapter* extAdapter = (WGPUAdapter*)userdata1;
        *extAdapter = adapter;
    }
    else
    {
        printf("Request_adapter status=%#.8x message=%.*s\n", status, (int)message.length, message.data);
    }
}

static void handle_request_device(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, void* userdata1, void* userdata2)
{
    if (status == WGPURequestDeviceStatus_Success)
    {
        WGPUDevice* extDevice = (WGPUDevice*)userdata1;
        *extDevice = device;
    }
    else
    {
        printf("Request_device status=%#.8x message=%.*s\n", status, (int)message.length, message.data);
    }
}

static WGPUAdapter RequestAdapter(WGPUInstance& instance)
{
    WGPURequestAdapterOptions adapter_options = {};

    WGPUAdapter local_adapter;
    WGPURequestAdapterCallbackInfo adapterCallbackInfo = {};
    adapterCallbackInfo.callback = handle_request_adapter;
    adapterCallbackInfo.userdata1 = &local_adapter;

    wgpuInstanceRequestAdapter(instance, &adapter_options, adapterCallbackInfo);
    IM_ASSERT(local_adapter && "Error on Adapter request");
    return local_adapter;
}

static WGPUDevice RequestDevice(WGPUAdapter& adapter)
{
    WGPUDevice local_device;
    WGPURequestDeviceCallbackInfo deviceCallbackInfo = {};
    deviceCallbackInfo.callback = handle_request_device;
    deviceCallbackInfo.userdata1 = &local_device;
    wgpuAdapterRequestDevice(adapter, nullptr, deviceCallbackInfo);
    IM_ASSERT(local_device && "Error on Device request");
    return local_device;
}
#endif 
#endif 

static bool InitWGPU(SDL_Window* window)
{
    WGPUTextureFormat preferred_fmt = WGPUTextureFormat_Undefined;  

    
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
    wgpu::InstanceDescriptor instance_desc = {};
    static constexpr wgpu::InstanceFeatureName timedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
    instance_desc.requiredFeatureCount = 1;
    instance_desc.requiredFeatures = &timedWaitAny;
    wgpu::Instance instance = wgpu::CreateInstance(&instance_desc);

    wgpu::Adapter adapter = RequestAdapter(instance);
    ImGui_ImplWGPU_DebugPrintAdapterInfo(adapter.Get());

    wgpu_device = RequestDevice(instance, adapter);

    
#ifdef __EMSCRIPTEN__
    wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector canvas_desc = {};
    canvas_desc.selector = "#canvas";

    wgpu::SurfaceDescriptor surface_desc = {};
    surface_desc.nextInChain = &canvas_desc;
    wgpu::Surface surface = instance.CreateSurface(&surface_desc);
#else
    wgpu::Surface surface = CreateWGPUSurface(instance.Get(), window);
#endif
    if (!surface)
        return false;

    
    wgpu_instance = instance.MoveToCHandle();
    wgpu_surface = surface.MoveToCHandle();

    WGPUSurfaceCapabilities surface_capabilities = {};
    wgpuSurfaceGetCapabilities(wgpu_surface, adapter.Get(), &surface_capabilities);

    preferred_fmt = surface_capabilities.formats[0];

    
#elif defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
    wgpu_instance = wgpuCreateInstance(nullptr);

#ifdef __EMSCRIPTEN__
    getAdapterAndDeviceViaJS();

    wgpu_device = emscripten_webgpu_get_device();
    IM_ASSERT(wgpu_device != nullptr && "Error creating the Device");

    WGPUSurfaceDescriptorFromCanvasHTMLSelector html_surface_desc = {};
    html_surface_desc.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
    html_surface_desc.selector = "#canvas";

    WGPUSurfaceDescriptor surface_desc = {};
    surface_desc.nextInChain = &html_surface_desc.chain;

    
    wgpu_surface = wgpuInstanceCreateSurface(wgpu_instance, &surface_desc);
    preferred_fmt = wgpuSurfaceGetPreferredFormat(wgpu_surface, {} /* adapter */);
#else 
    wgpuSetLogCallback(
        [](WGPULogLevel level, WGPUStringView msg, void* userdata) { fprintf(stderr, "%s: %.*s\n", ImGui_ImplWGPU_GetLogLevelName(level), (int)msg.length, msg.data); }, nullptr
    );
    wgpuSetLogLevel(WGPULogLevel_Warn);

    WGPUAdapter adapter = RequestAdapter(wgpu_instance);
    ImGui_ImplWGPU_DebugPrintAdapterInfo(adapter);

    wgpu_device = RequestDevice(adapter);

    
    wgpu_surface = CreateWGPUSurface(wgpu_instance, window);
    if (!wgpu_surface)
        return false;

    WGPUSurfaceCapabilities surface_capabilities = {};
    wgpuSurfaceGetCapabilities(wgpu_surface, adapter, &surface_capabilities);

    preferred_fmt = surface_capabilities.formats[0];
#endif 
#endif 

    wgpu_surface_configuration.presentMode = WGPUPresentMode_Fifo;
    wgpu_surface_configuration.alphaMode = WGPUCompositeAlphaMode_Auto;
    wgpu_surface_configuration.usage = WGPUTextureUsage_RenderAttachment;
    wgpu_surface_configuration.width = wgpu_surface_width;
    wgpu_surface_configuration.height = wgpu_surface_height;
    wgpu_surface_configuration.device = wgpu_device;
    wgpu_surface_configuration.format = preferred_fmt;

    wgpuSurfaceConfigure(wgpu_surface, &wgpu_surface_configuration);
    wgpu_queue = wgpuDeviceGetQueue(wgpu_device);

    return true;
}
