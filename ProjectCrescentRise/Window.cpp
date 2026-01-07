#include "Window.h"
#include "ShaderPrograms.h"
#include "GameObjects.h"
#include "Update.h"
#include "CommandInterpreter.h"
#include "PlanetSurface.h"
#include <glm/gtc/quaternion.hpp>

Window::Window()
{
    CommandInterpreter::append([this](std::string t) { this->openGui(t); }, "/GUI");
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Update::append([this]() { this->Update(); });

    clickIntoWindow = std::make_shared<mouseKeyInput>();
    clickIntoWindow->active = true;
    clickIntoWindow->function = [this]() { this->ClickIn(); };
    clickIntoWindow->keyCode = KeyScan::MouseKeyCode::RightMouse;
    KeyScan::append(clickIntoWindow, true);

    if (guiActive)
    {
        //std::cout << "=== GUI ACTIVE ===\n";
        //
        //// Set window hints for floating, transparent, draggable window
        //glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);           // Always on top
        //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);         // No title bar/borders
        //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); // Transparent background
        //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);         // Fixed size for simplicity
        //
        //// Create smaller GUI window (adjust size as needed)
        //
        //guiWindow = glfwCreateWindow(planetGen.guiWidth, planetGen.guiHeight, "GUI WINDOW", NULL, NULL);
        //if (!guiWindow)
        //{
        //    glfwTerminate();
        //    return;
        //}
        //
        //// Position the window in top-right corner
        //glfwSetWindowPos(guiWindow, m_width - planetGen.guiWidth - 50, 50);
        //
        //// Set window opacity (0.9 = 90% opaque, 10% transparent)
        //glfwSetWindowOpacity(guiWindow, 0.9f);
        //
        ///* Make the window's context current */
        //glfwMakeContextCurrent(guiWindow);
        //glewInit();
        //glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LESS);

        initGui();
    }
    else
    {
        std::cout << "=== GUI INACTIVE ===\n";
    }
    glfwMakeContextCurrent(m_window);
}

void Window::initGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Make ImGui background more transparent
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors->w = 0.8f; // 80% opaque background

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Set up mouse callbacks for dragging
    glfwSetWindowUserPointer(m_window, this);
}

void Window::SetCameraFromTransform(Transform& t) {
    // Use planet-relative up when available so camera follows spherical surface orientation
    glm::vec3 pos = t.position;
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    if (PlanetSurface::s_instance && PlanetSurface::s_instance->getTransform()) {
        up = glm::normalize(pos - PlanetSurface::s_instance->getTransform()->position);
    }

    // Build a tangent-space forward using yaw and pitch around local axes
    float yaw = glm::radians(t.rotation.y);
    float pitch = glm::radians(t.rotation.x);

    // pick an arbitrary vector not parallel to 'up'
    glm::vec3 arbitrary = glm::vec3(0.0f, 0.0f, 1.0f);
    if (fabs(glm::dot(up, arbitrary)) > 0.999f) arbitrary = glm::vec3(0.0f, 1.0f, 0.0f);

    // build orthonormal tangent basis
    glm::vec3 right = glm::normalize(glm::cross(up, arbitrary));
    glm::vec3 forwardBasis = glm::normalize(glm::cross(right, up));

    // apply yaw around up first
    glm::quat qYaw = glm::angleAxis(yaw, up);
    glm::vec3 fYaw = qYaw * forwardBasis;

    // recompute right after yaw in a consistent way
    glm::vec3 rightYaw = glm::normalize(glm::cross(up, fYaw));

    // apply pitch around the post-yaw right axis
    glm::quat qPitch = glm::angleAxis(pitch, rightYaw);
    glm::vec3 forward = qPitch * fYaw;

    // if forward becomes nearly parallel to up, fallback to yaw-only to avoid degenerate lookAt
    if (fabs(glm::dot(glm::normalize(forward), up)) > 0.999f)
    {
        forward = fYaw; // yaw-only
    }

    glm::vec3 target = pos + forward;

    m_camera.setPosition(pos);
    m_camera.setTarget(target);
    m_camera.setUp(up);
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

    if (newPlanetWaitingInBuffer.load())
    {
        std::lock_guard<std::mutex> lock(planetMutex);

        planetGen.setNewPlanet(mostRecentPlanet);
        newPlanetWaitingInBuffer.store(false);

    }
}

void Window::ClickIn()
{
    tabbedOut = false;
}

Window::~Window()
{
    if (guiActive)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

bool Window::windowClosed()
{
    return (glfwWindowShouldClose(m_window));
}

void Window::render()
{
    glfwMakeContextCurrent(m_window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GameObjects::renderAll();

    if (guiActive)
    {
        // render all im gui stuff
        //glfwMakeContextCurrent(guiWindow);
        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set alpha to 0 for transparent background
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        planetGen.guiRender();

        if (guiActive)
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            //glfwSwapBuffers(m_window);
        }
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}

void Window::PassPlanet(std::shared_ptr<PlanetSurface> t_planet, std::shared_ptr<WaterSphere> t_water)
{
    planetGen.init(t_planet, t_water);
}

void Window::passNewPlanetSettings(PlanetPayload t_payload)
{
    std::lock_guard<std::mutex> lock(planetMutex);

    mostRecentPlanet.planetColour.m_colours = t_payload.planetColour.m_colours;
    mostRecentPlanet.planetColour.m_heights = t_payload.planetColour.m_heights;

    mostRecentPlanet.shapeSettings.noiseLayers = t_payload.shapeSettings.noiseLayers;
    mostRecentPlanet.shapeSettings.planetRadius = t_payload.shapeSettings.planetRadius;

    mostRecentPlanet.planetPointCount = t_payload.planetPointCount;

    newPlanetWaitingInBuffer.store(true);
}

void Window::closeGUI()
{
    guiActive = false;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(guiWindow);
    glfwMakeContextCurrent(m_window);
}

void Window::openGui(std::string t_arguement)
{
    guiActive = !guiActive;

    if (guiActive)initGui();
    else closeGUI();
}
