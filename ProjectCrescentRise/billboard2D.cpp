#include "billboard2D.h"
#include "Window.h"
#include <glm/gtc/type_ptr.hpp>

void billboard2D::Start()
{
    float points[] = {
        // positions          // texture coords
        0.5f,   0.5f,  0.0f,   1.0f, 1.0f,  
        0.5f,  -0.5f,  0.0f,   1.0f, 0.0f,  
        -0.5f, -0.5f,  0.0f,   0.0f, 0.0f,  
        -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 
        -0.5f,  0.5f,  0.0f,   0.0f, 1.0f,
        0.5f,   0.5f,  0.0f,   1.0f, 1.0f
    };

    glGenBuffers(1, &m_body.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_body.vao);
    glBindVertexArray(m_body.vao);
    
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture coordinate attribute (location = 1)
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::checkerboard);

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");

}

void billboard2D::Render()
{
    VertexShaders::LoadShader(m_shader);

    glBindVertexArray(m_body.vao);

    glm::mat4 model = transform ? ToModelMatrix(*transform) : glm::mat4(1.0f);
    glm::mat4 view = Window::Get().GetView();
    glm::mat4 proj = Window::Get().GetProj();


    if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));

    // Draw points 0-3 from the currently bound VAO with current in-use shader.
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
