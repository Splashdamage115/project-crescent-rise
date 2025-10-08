#include "Window.h"
#include "ShaderPrograms.h"
#include "GameObjects.h"

Window::Window()
{
    m_window = glfwCreateWindow(640, 480, "Project Crescent Rise", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        return;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);
    glewInit();
    VertexShaders::initialise();
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
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    GameObjects::renderAll();

    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}
