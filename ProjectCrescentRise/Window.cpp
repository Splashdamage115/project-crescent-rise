#include "Window.h"
#include "ShaderPrograms.h"

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
    float points[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);





    const char* fragment_shader =
        "#version 410 core\n"
        "out vec4 frag_colour;"
        "void main() {"
        "  frag_colour = vec4( 0.0, 0.0, 0.5, 1.0 );"
        "}";

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);


    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    VertexShaders::LoadShader(VertexShaders::VertexShaderType::standardBlue, shader_program);
    glLinkProgram(shader_program);
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    // Put the shader program, and the VAO, in focus in OpenGL's state machine.
    glUseProgram(shader_program);
    glBindVertexArray(vao);
 
    // Draw points 0-3 from the currently bound VAO with current in-use shader.
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}
