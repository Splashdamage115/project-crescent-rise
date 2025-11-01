#include "GroundTile.h"

void GroundTile::Start()
{
    int x = 0;
    int y = 0;

    std::vector<float> vertices;

    std::vector<std::vector<float>> heights1;
    std::vector<std::vector<float>> heights2;

    generateVornoi(heights1, width, 32);
    generateVornoi(heights2, width, 128);

    std::vector<std::vector<float>> heights;

    for (int x = 0; x < width; x++)
    {
        heights.emplace_back();
        for (int y = 0; y < width; y++)
        {
            heights.at(x).emplace_back((heights1.at(x).at(y)) + (heights2.at(x).at(y) * 0.5f));
        }
    }


    disp *= scaleFactor;
    tileSize *= scaleFactor;

    //generateVornoi(heights, width);

    int iterX = 0;
    int iterY = 0;
    bool stopHeight = false;
    size = width * width;


    for (int i = 0; i < width * width; i++)
    {
//                        x                                  y                                 z                                 u                               v
// 0
vertices.emplace_back( -tileSize + (x * disp) ); vertices.emplace_back( heights.at((y == 0) ? iterX : iterX - 1).at((x == 0) ? iterY : iterY - 1) ); vertices.emplace_back( -tileSize + (y * disp) ); vertices.emplace_back(  0.0f ); vertices.emplace_back(  1.0f );

// 1
vertices.emplace_back( -tileSize + (x * disp) ); vertices.emplace_back( heights.at(iterX).at((x == 0) ? iterY : iterY - 1)); vertices.emplace_back(tileSize + (y * disp) ); vertices.emplace_back(  0.0f ); vertices.emplace_back(  0.0f );

// 2
vertices.emplace_back(  tileSize + (x * disp) ); vertices.emplace_back( heights.at((y == 0) ? iterX : iterX - 1).at(iterY)); vertices.emplace_back( -tileSize + (y * disp) ); vertices.emplace_back(  1.0f ); vertices.emplace_back(  1.0f );

// 3
vertices.emplace_back(  tileSize + (x * disp) ); vertices.emplace_back( heights.at(iterX).at(iterY)); vertices.emplace_back(tileSize + (y * disp) ); vertices.emplace_back(  1.0f ); vertices.emplace_back(  0.0f );

        
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

    for (int i = 0; i < width * width; i++)
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
    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::terrain, Shader::FragmentShaderType::terrain);

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
}

void GroundTile::Render()
{
    if (!enabled) return;

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

struct pos
{
    pos(float t_x, float t_y) : x(t_x), y(t_y) { }
    float x, y;
};

static float distance(float t_pos1x, float t_pos1y, float t_pos2x, float t_pos2y)
{
    return std::sqrt((t_pos1x - t_pos2x) * (t_pos1x - t_pos2x) + (t_pos1y - t_pos2y) * (t_pos1y - t_pos2y));
}

void GroundTile::generateVornoi(std::vector<std::vector<float>>& t_heights, int width, int pointAmt)
{
    std::vector<pos> vornoiPositions;

    for (int i = 0; i < pointAmt; i++)
    {
        float x = static_cast<float>(rand() % width);
        float y = static_cast<float>(rand() % width);
        vornoiPositions.emplace_back(x, y);
    }
    float height = 0.f;
    for (int x = 0; x < width; x++)
    {
        t_heights.emplace_back();
        height = 0.f;
        for (int y = 0; y < width; y++)
        {
            //int closest = 0;
            float closestDistance = distance(x, y, vornoiPositions.at(0).x, vornoiPositions.at(0).y);
            float secondClosest = distance(x, y, vornoiPositions.at(1).x, vornoiPositions.at(1).y);
            for (int i = 0; i < vornoiPositions.size(); i++)
            {
                float currentDistance = distance(x, y, vornoiPositions.at(i).x, vornoiPositions.at(i).y);
                if (closestDistance > currentDistance)
                {
                    secondClosest = closestDistance;
                    closestDistance = currentDistance;
                }
                else if (secondClosest > currentDistance)
                {
                    secondClosest = currentDistance;
                }
            }
            t_heights.at(x).emplace_back((closestDistance + secondClosest) * heightFactor);
        }
    }
}
