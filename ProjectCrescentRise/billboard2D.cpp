#include "billboard2D.h"
#include "Window.h"

void billboard2D::Start()
{
    float points[] = {
    0.5f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f,
    -0.5f, 0.5f,  0.0f,
    0.5f, 0.5f,  0.0f
    };

    glGenBuffers(1, &m_body.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_body.vao);
    glBindVertexArray(m_body.vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::standard);
}

void billboard2D::Render()
{
    // Put the shader program, and the VAO, in focus in OpenGL's state machine.
    VertexShaders::LoadShader(m_shader);

    glBindVertexArray(m_body.vao);

    // Draw points 0-3 from the currently bound VAO with current in-use shader.
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
