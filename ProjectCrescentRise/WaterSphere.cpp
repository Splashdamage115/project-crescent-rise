#include "WaterSphere.h"
#include "CubeSphereFace.h"
#include "PlayerInput.h"
#include "Globals.h"
#include "stb_image.h"

void WaterSphere::ResetPlanet()
{
    callChange = true;
    shapeGenerator.settings = shapeSettings;
    shapeGenerator.reset();
    std::vector<glm::vec3> direction;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    int vertLength = 8; // x y z NormalX NormalY NormalZ u v
    int faceAmt = 6; // face for each side
    int triIntCount = 6; // top left, top right, bottom left ~ bootom left, top right, bottom right ~ these are the triangle positions
    int pointAmt = (pointsPerRow * pointsPerRow);
    int indiceAmt = (pointsPerRow - 1) * (pointsPerRow - 1);

    vertices.clear();
    vertices.resize(((faceAmt)*pointAmt) * vertLength);
    indices.clear();
    indices.resize(((faceAmt)*indiceAmt) * triIntCount);

    glm::vec3 playerPos = PlayerInput::playerPosition;


    direction.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));   // Up
    direction.emplace_back(glm::vec3(0.0f, -1.0f, 0.0f));  // Down
    direction.emplace_back(glm::vec3(-1.0f, 0.0f, 0.0f));  // Left
    direction.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f));   // Right
    direction.emplace_back(glm::vec3(0.0f, 0.0f, -1.0f));  // Forward
    direction.emplace_back(glm::vec3(0.0f, 0.0f, 1.0f));   // Back

    int closestNum = 0;

    float lowestDist = glm::distance(playerPos, direction.at(0) + transform->position);

    for (int i = 1; i < 6; i++)
    {
        float currentDist = glm::distance(playerPos, direction.at(i) + transform->position);
        if (currentDist < lowestDist)
        {
            lowestDist = currentDist;
            closestNum = i;
        }
    }

    int multiplierSkip = 0;

    for (int i = 0; i < 6; i++)
    {
        //if (i == closestNum); // i is now the closest face

        CubeSphereFace::generateFace(vertices, indices, pointsPerRow, direction.at(i), i, shapeGenerator);

    }
    size = indices.size();

    glBindVertexArray(m_body.vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_body.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
}

void WaterSphere::Start()
{
    // Generate and setup VAO
    glGenVertexArrays(1, &m_body.vao);
    glBindVertexArray(m_body.vao);

    glGenBuffers(1, &m_body.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_body.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_body.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    if (WIRE_FRAME)
        m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::Line, Shader::FragmentShaderType::Line);
    else
        m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::Water, Shader::FragmentShaderType::Water);

    if (WIRE_FRAME)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
    }


    int width, height, channels; unsigned char* data = stbi_load("./Assets/Images/NormalMap.png", &width, &height, &channels, 4);

    if (!data)
    {
        std::cout << "DATA FOR IMAGE COULD NOT BE LOADED\n";
    }

    glGenTextures(1, &waterNormalTextureID);
    glBindTexture(GL_TEXTURE_2D, waterNormalTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);


    data = stbi_load("./Assets/Images/water.png", &width, &height, &channels, 4);

    if (!data)
    {
        std::cout << "DATA FOR IMAGE COULD NOT BE LOADED\n";
    }

    glGenTextures(1, &waterTextureID);
    glBindTexture(GL_TEXTURE_2D, waterTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);


    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
    if (WIRE_FRAME)
    MinMax = glGetUniformLocation(m_shader->shaderPair, "minMax");
    CenterPoint = glGetUniformLocation(m_shader->shaderPair, "CenterPoint");
    ViewPosition = glGetUniformLocation(m_shader->shaderPair, "viewPos");
    time = glGetUniformLocation(m_shader->shaderPair, "uTime");

    WaterNormalMap = glGetUniformLocation(m_shader->shaderPair, "uWaterNormalMap");
    TextureScale = glGetUniformLocation(m_shader->shaderPair, "uTextureScale");
    TextureSpeed = glGetUniformLocation(m_shader->shaderPair, "uTextureSpeed");

    WaterColorMap = glGetUniformLocation(m_shader->shaderPair, "uWaterColorMap");

    // Generate initial mesh data
    ResetPlanet();
    callChange = false;
}

void WaterSphere::Render()
{
    if (!enabled) return;

    if (callChange) ResetPlanet();

    VertexShaders::LoadShader(m_shader);

    glBindVertexArray(m_body.vao);

    glm::mat4 model = transform ? ToModelMatrix(*transform) : glm::mat4(1.0f);
    glm::mat4 view = Window::Get().GetView();
    glm::mat4 proj = Window::Get().GetProj();

    if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));

    if (CenterPoint >= 0) glUniform3f(CenterPoint, (float)transform->position.x, (float)transform->position.y, (float)transform->position.z);
    if (MinMax >= 0) glUniform2f(MinMax, shapeGenerator.elevationMinMax.getMin(), shapeGenerator.elevationMinMax.getMax());
    if (ViewPosition >= 0)
    {
        glm::vec3 playerPos = PlayerInput::playerPosition;
        glUniform3f(ViewPosition, playerPos.x, playerPos.y, playerPos.z);
    }
    if (time >= 0) glUniform1f(time, glfwGetTime());

    if (WaterNormalMap >= 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterNormalTextureID);
        glUniform1i(waterNormalTextureID, 0);
    };
    if (WaterColorMap >= 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, WaterColorMap);
        glUniform1i(WaterColorMap, 0);
    };
    
    if (TextureScale >= 0) glUniform1f(TextureScale, 8.0f);
    if (TextureSpeed >= 0) glUniform1f(TextureSpeed, 0.05f);

    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

}
