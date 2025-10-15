#include "GroundTile.h"

void GroundTile::Start()
{
    int x = 0;
    int y = 0;

    std::vector<float> vertices;

    int squareCount = 4096;
    int width = 64;
    size = squareCount;
    bool stopHeight = false;
    int disp = 1;

    std::vector<std::vector<float>> heights;

    for (int x = 0; x < width; x++)
    {
        heights.emplace_back();
        for (int y = 0; y < width; y++)
        {
            heights.at(x).emplace_back(static_cast<float>(rand() % 100) / 100.f);
        }
    }

    int iterX = 0;
    int iterY = 0;

    for (int i = 0; i < squareCount; i++)
    {


//                        x                                  y                                 z                                 u                               v
vertices.emplace_back( -0.5f + (x * disp) ); vertices.emplace_back( heights.at((y == 0) ? iterX : iterX - 1).at((x == 0) ? iterY : iterY - 1) ); vertices.emplace_back( -0.5f + (y * disp) ); vertices.emplace_back(  0.0f ); vertices.emplace_back(  1.0f );
vertices.emplace_back( -0.5f + (x * disp) ); vertices.emplace_back( heights.at(iterX).at((x == 0) ? iterY : iterY - 1)); vertices.emplace_back(  0.5f + (y * disp) ); vertices.emplace_back(  0.0f ); vertices.emplace_back(  0.0f );
vertices.emplace_back(  0.5f + (x * disp) ); vertices.emplace_back( heights.at((y == 0) ? iterX : iterX - 1).at(iterY)); vertices.emplace_back( -0.5f + (y * disp) ); vertices.emplace_back(  1.0f ); vertices.emplace_back(  1.0f );
vertices.emplace_back(  0.5f + (x * disp) ); vertices.emplace_back( heights.at(iterX).at(iterY)); vertices.emplace_back(  0.5f + (y * disp) ); vertices.emplace_back(  1.0f ); vertices.emplace_back(  0.0f );

        
iterY++;
x++;
if (x >= width)
{
    iterY = 0;
    iterX++;
    x = 0;
    y++;
}
    }

    // Define indices for two triangles (counter-clockwise)
    std::vector<unsigned int> indices;

    for (int i = 0; i < squareCount; i++)
    {
        int offset = i * 4;
        indices.push_back(0 + offset); indices.push_back(1 + offset); indices.push_back(2 + offset);
        indices.push_back(2 + offset); indices.push_back(3 + offset); indices.push_back(1 + offset);
    }


    // Generate and setup VAO
    glGenVertexArrays(1, &m_body.vao);
    glBindVertexArray(m_body.vao);

    // Generate and setup VBO
    glGenBuffers(1, &m_body.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Generate and setup EBO
    glGenBuffers(1, &m_body.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_body.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Setup vertex attributes
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Get shader and uniform locations
    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::checkerboard);

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
}

void GroundTile::Render()
{
    VertexShaders::LoadShader(m_shader);

    glBindVertexArray(m_body.vao);

    glm::mat4 model = transform ? ToModelMatrix(*transform) : glm::mat4(1.0f);
    glm::mat4 view = Window::Get().GetView();
    glm::mat4 proj = Window::Get().GetProj();


    if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));

    glDrawElements(GL_TRIANGLES, size * 6, GL_UNSIGNED_INT, 0);
}
