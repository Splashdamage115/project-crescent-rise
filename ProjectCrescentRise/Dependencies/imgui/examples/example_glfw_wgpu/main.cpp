









#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
#include <emscripten/html5_webgpu.h>
#endif
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#else
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
#include <webgpu/webgpu_glfw.h>
#endif
#endif


static WGPUInstance             wgpu_instance = nullptr;
static WGPUDevice               wgpu_device = nullptr;
static WGPUSurface              wgpu_surface = nullptr;
static WGPUQueue                wgpu_queue = nullptr;
static WGPUSurfaceConfiguration wgpu_surface_configuration = {};
static int                      wgpu_surface_width = 1280;
static int                      wgpu_surface_height = 800;


static bool InitWGPU(GLFWwindow* window);

static void glfw_error_callback(int error, const char* description)
{
    printf("GLFW Error %d: %s\n", error, description);
}

static void ResizeSurface(int width, int height)
{
    wgpu_surface_configuration.width = wgpu_surface_width = width;
    wgpu_surface_configuration.height = wgpu_surface_height = height;
    wgpuSurfaceConfigure(wgpu_surface, &wgpu_surface_configuration);
}


int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); 
    wgpu_surface_width *= main_scale;
    wgpu_surface_height *= main_scale;
    GLFWwindow* window = glfwCreateWindow(wgpu_surface_width, wgpu_surface_height, "Dear ImGui GLFW+WebGPU example", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    
    if (!InitWGPU(window))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glfwShowWindow(window);

    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      

    
    ImGui::StyleColorsDark();
    

    
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        
    style.FontScaleDpi = main_scale;        

    
    ImGui_ImplGlfw_InitForOther(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
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

    
#ifdef __EMSCRIPTEN__
    
    
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        
        
        
        
        
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        
        int width, height;
        glfwGetFramebufferSize((GLFWwindow*)window, &width, &height);
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
        ImGui_ImplGlfw_NewFrame();
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
        view_desc.dimension = WGPUTextureViewDimension_2D ;
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
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    wgpuSurfaceUnconfigure(wgpu_surface);
    wgpuSurfaceRelease(wgpu_surface);
    wgpuQueueRelease(wgpu_queue);
    wgpuDeviceRelease(wgpu_device);
    wgpuInstanceRelease(wgpu_instance);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}





#if !defined(__EMSCRIPTEN__) && (defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU) || defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN))

#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#define GLFW_HAS_X11_OR_WAYLAND     1
#else
#define GLFW_HAS_X11_OR_WAYLAND     0
#endif
#ifdef _WIN32
#undef APIENTRY
#ifndef GLFW_EXPOSE_NATIVE_WIN32    
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#elif defined(__APPLE__)
#ifndef GLFW_EXPOSE_NATIVE_COCOA    
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#elif GLFW_HAS_X11_OR_WAYLAND
#ifndef GLFW_EXPOSE_NATIVE_X11      
#define GLFW_EXPOSE_NATIVE_X11
#endif
#ifndef GLFW_EXPOSE_NATIVE_WAYLAND
#if defined(__has_include) && __has_include(<wayland-client.h>)
#define GLFW_EXPOSE_NATIVE_WAYLAND
#endif
#endif
#endif
#include <GLFW/glfw3native.h>
#undef Status                       

WGPUSurface CreateWGPUSurface(const WGPUInstance& instance, GLFWwindow* window)
{
    ImGui_ImplWGPU_CreateSurfaceInfo create_info = {};
    create_info.Instance = instance;
#if defined(GLFW_EXPOSE_NATIVE_COCOA)
    {
        create_info.System = "cocoa";
        create_info.RawWindow = (void*)glfwGetCocoaWindow(window);
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#elif defined(GLFW_EXPOSE_NATIVE_WAYLAND)
    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND)
    {
        create_info.System = "wayland";
        create_info.RawDisplay = (void*)glfwGetWaylandDisplay();
        create_info.RawSurface = (void*)glfwGetWaylandWindow(window);
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#elif defined(GLFW_EXPOSE_NATIVE_X11)
    if (glfwGetPlatform() == GLFW_PLATFORM_X11)
    {
        create_info.System = "x11";
        create_info.RawWindow = (void*)glfwGetX11Window(window);
        create_info.RawDisplay = (void*)glfwGetX11Display();
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#elif defined(GLFW_EXPOSE_NATIVE_WIN32)
    {
        create_info.System = "win32";
        create_info.RawWindow = (void*)glfwGetWin32Window(window);
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
        printf("Request_adapter status=%#.8x message=%.*s\n", status, (int) message.length, message.data);
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
        printf("Request_device status=%#.8x message=%.*s\n", status, (int) message.length, message.data);
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

static bool InitWGPU(GLFWwindow* window)
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
    wgpu_surface = instance.CreateSurface(&surface_desc).MoveToCHandle();
#else
    wgpu_surface = CreateWGPUSurface(instance.Get(), window);
#endif
    if (!wgpu_surface)
        return false;

    
    wgpu_instance = instance.MoveToCHandle();

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
