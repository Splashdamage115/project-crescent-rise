#include "Window.h"
#include "ShaderPrograms.h"
#include "GameObjects.h"
#include "Update.h"

Window::Window()
{
    m_window = glfwCreateWindow(m_width, m_height, "Project Crescent Rise", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        return;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    VertexShaders::initialise();
    KeyScan::init();
    glfwSetMouseButtonCallback(m_window, KeyScan::mouseClickCallback);
    glfwSetKeyCallback(m_window, KeyScan::key_callback);
    glfwSetCursorPosCallback(m_window, KeyScan::cursorCallback);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    Update::append([this]() { this->Update(); });

    clickIntoWindow = std::make_shared<mouseKeyInput>();
    clickIntoWindow->active = true;
    clickIntoWindow->function = [this]() { this->ClickIn(); };
    clickIntoWindow->keyCode = KeyScan::MouseKeyCode::LeftMouse;
    KeyScan::append(clickIntoWindow, true);

    if (guiActive)
    {
        std::cout << "=== GUI ACTIVE ===\n";
        
        // Set window hints for floating, transparent, draggable window
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);           // Always on top
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);         // No title bar/borders
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); // Transparent background
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);         // Fixed size for simplicity
        
        // Create smaller GUI window (adjust size as needed)
        int guiWidth = 600;
        int guiHeight = 800;
        guiWindow = glfwCreateWindow(guiWidth, guiHeight, "GUI WINDOW", NULL, NULL);
        if (!guiWindow)
        {
            glfwTerminate();
            return;
        }
        
        // Position the window in top-right corner
        glfwSetWindowPos(guiWindow, m_width - guiWidth - 50, 50);
        
        // Set window opacity (0.9 = 90% opaque, 10% transparent)
        glfwSetWindowOpacity(guiWindow, 0.9f);
        
        /* Make the window's context current */
        glfwMakeContextCurrent(guiWindow);
        glewInit();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        
        // Make ImGui background more transparent
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors->w = 0.8f; // 80% opaque background
        
        ImGui_ImplGlfw_InitForOpenGL(guiWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        
        // Set up mouse callbacks for dragging
        glfwSetWindowUserPointer(guiWindow, this);
    }
    else
    {
        std::cout << "=== GUI INACTIVE ===\n";
    }
    glfwMakeContextCurrent(m_window);
}

void Window::Update()
{
    if (escDown && !KeyScan::isKeyDown(KeyScan::KeyCode::ESC))
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        tabbedOut = true;
    }
    escDown = KeyScan::isKeyDown(KeyScan::KeyCode::ESC);
    if (!tabbedOut) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void Window::ClickIn()
{
    tabbedOut = false;
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}

bool Window::windowClosed()
{
    return (glfwWindowShouldClose(m_window));
}

void Window::render()
{
    if (guiActive)
    {
        // render all im gui stuff
        glfwMakeContextCurrent(guiWindow);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set alpha to 0 for transparent background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        planetGen.guiRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(guiWindow);
    }


    glfwMakeContextCurrent(m_window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    GameObjects::renderAll();

    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}

void Window::PassPlanet(std::shared_ptr<CubeSphere> t_planet)
{
    planetGen.init(t_planet);
}
