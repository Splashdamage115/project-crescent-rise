


#include "imgui.h"
#include <stdio.h>

int main(int, char**)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    
    
    
    
    io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;

    for (int n = 0; n < 20; n++)
    {
        printf("NewFrame() %d\n", n);
        io.DisplaySize = ImVec2(1920, 1080);
        io.DeltaTime = 1.0f / 60.0f;
        ImGui::NewFrame();

        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::ShowDemoWindow(nullptr);

        ImGui::Render();
    }

    printf("DestroyContext()\n");
    ImGui::DestroyContext();
    return 0;
}
