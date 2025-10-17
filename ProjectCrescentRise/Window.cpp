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
}

bool Window::windowClosed()
{
    return (glfwWindowShouldClose(m_window));
}

void Window::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    GameObjects::renderAll();

    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}
