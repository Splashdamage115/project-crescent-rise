#include "PlanetSurface.h"
#include "CubeSphereFace.h"
#include "PlayerInput.h"
#include "Globals.h"
#include "stb_image.h"

void PlanetSurface::ResetPlanet()
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
    vertices.resize(((faceAmt) * pointAmt) * vertLength);
    indices.clear();
    indices.resize(((faceAmt) * indiceAmt) * triIntCount);

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
    for (int i = 0; i < planetColour.COLOUR_MAX; i++)
    {
        if (!planetColour.m_needsReloading.at(i)) continue;

        std::string location = "./Assets/Images/floorTextures/";
        location += planetColour.m_textureLocation.at(i);

        int width, height, channels; unsigned char* data = stbi_load(location.c_str(), &width, &height, &channels, 4);

        if (!data)
        {
            std::cout << "DATA FOR IMAGE COULD NOT BE LOADED\n";
        }

        glGenTextures(1, &textureLocations.at(i));
        glBindTexture(GL_TEXTURE_2D, textureLocations.at(i));

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



        glGenTextures(1, &normalLocations.at(i));
        glBindTexture(GL_TEXTURE_2D, normalLocations.at(i));

        location = "./Assets/Images/floorTextures/";
        location += planetColour.m_normalLocation.at(i);

        int w, h, c;
        unsigned char* hData = stbi_load(location.c_str(), &w, &h, &c, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, hData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(hData);

        planetColour.m_needsReloading.at(i) = false;
    }
    size = indices.size();

    glBindVertexArray(m_body.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_body.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
}

void PlanetSurface::Start()
{
    for (int i = 0; i < planetColour.COLOUR_MAX; i++)
    {
        planetColour.m_shaderType.emplace_back(0);
        planetColour.m_colours.emplace_back(255.f);
        planetColour.m_heights.emplace_back(10.0f);
        planetColour.active.emplace_back(false);
        planetColour.m_needsReloading.emplace_back(false);
        planetColour.m_textureLocation.emplace_back("basic.jpg");
        planetColour.m_normalLocation.emplace_back("basicNormal.jpg");
        planetColour.m_textureScale.emplace_back(20.f);
        planetColour.m_normalStrength.emplace_back(0.8f);

        textureLocations.emplace_back(-1);
        std::string location = "./Assets/Images/floorTextures/";
        location += planetColour.m_textureLocation.at(i);

        int width, height, channels; unsigned char* data = stbi_load(location.c_str(), &width, &height, &channels, 4);

        if (!data)
        {
            std::cout << "DATA FOR IMAGE COULD NOT BE LOADED\n";
        }

        glGenTextures(1, &textureLocations.at(i));
        glBindTexture(GL_TEXTURE_2D, textureLocations.at(i));

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



        normalLocations.emplace_back(-1);
        glGenTextures(1, &normalLocations.at(i));
        glBindTexture(GL_TEXTURE_2D, normalLocations.at(i));

        location = "./Assets/Images/floorTextures/";
        location += planetColour.m_normalLocation.at(i);

        int w, h, c;
        unsigned char* hData = stbi_load(location.c_str(), &w, &h, &c, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, hData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(hData);
    }
    // TO DO: change from being static lowest being sand
    planetColour.m_shaderType.at(0) = 1;

    planetColour.m_heights.at(0) = -1.0f;

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
        m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::Planet, Shader::FragmentShaderType::Planet);

    if (WIRE_FRAME)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
    }

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
    if (WIRE_FRAME)
        uColourLoc = glGetUniformLocation(m_shader->shaderPair, "baseColour");
    MinMax = glGetUniformLocation(m_shader->shaderPair, "minMax");
    CenterPoint = glGetUniformLocation(m_shader->shaderPair, "CenterPoint");
    heightColours = glGetUniformLocation(m_shader->shaderPair, "heightColours");
    startHeight = glGetUniformLocation(m_shader->shaderPair, "startHeight");
    shaderType = glGetUniformLocation(m_shader->shaderPair, "shaderType");
    ViewPosition = glGetUniformLocation(m_shader->shaderPair, "viewPos");
    time = glGetUniformLocation(m_shader->shaderPair, "uTime");
    layerTextures = glGetUniformLocation(m_shader->shaderPair, "layerTextures");
    textureScale = glGetUniformLocation(m_shader->shaderPair, "textureScale");
    LayerNormal = glGetUniformLocation(m_shader->shaderPair, "uLayerNormal");
    NormalStrength = glGetUniformLocation(m_shader->shaderPair, "uLayerNormalStrength");

    // Generate initial mesh data
    ResetPlanet();
    callChange = false;
}

void PlanetSurface::Render()
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

    if (WIRE_FRAME && uColourLoc >= 0) glUniform3f(uColourLoc, 255.0f, 255.0f, 255.0f);


    if (CenterPoint >= 0) glUniform3f(CenterPoint, (float)transform->position.x, (float)transform->position.y, (float)transform->position.z);
    if (MinMax >= 0) glUniform2f(MinMax, shapeGenerator.elevationMinMax.getMin(), shapeGenerator.elevationMinMax.getMax()); 
    if (heightColours >= 0 && planetColour.m_colours.size() > 0) {
        //std::cout << planetColour.m_colours[0].x << " " << planetColour.m_colours[0].g << " " << planetColour.m_colours[0].b << "\n";
        glUniform3fv(heightColours, planetColour.m_colours.size(), glm::value_ptr(planetColour.m_colours[0]));
    }
    if (startHeight >= 0 && planetColour.m_heights.size() > 0) glUniform1fv(startHeight, planetColour.m_heights.size(), &planetColour.m_heights[0]);
    if (shaderType >= 0 && planetColour.m_shaderType.size() > 0) glUniform1iv(shaderType, planetColour.m_shaderType.size(), &planetColour.m_shaderType[0]);

    if (layerTextures >= 0 && !textureLocations.empty())
    {
        glBindTextures(0, textureLocations.size(), &textureLocations[0]);

        std::vector<int> units(textureLocations.size());
        for (int i = 0; i < units.size(); i++)
            units[i] = i;

        glUniform1iv(layerTextures, units.size(), units.data());
    }
    if (LayerNormal >= 0 && !normalLocations.empty())
    {
        glBindTextures(planetColour.COLOUR_MAX, normalLocations.size(), &normalLocations[0]);

        std::vector<int> units(normalLocations.size());
        for (int i = 0; i < units.size(); i++)
            units[i] = i + planetColour.COLOUR_MAX;

        glUniform1iv(LayerNormal, units.size(), units.data());
    }
    
    if (textureScale >= 0 && !planetColour.m_textureScale.empty()) glUniform1fv(textureScale, planetColour.m_textureScale.size(), &planetColour.m_textureScale[0]);
    if (NormalStrength >= 0 && !planetColour.m_normalStrength.empty()) glUniform1fv(NormalStrength, planetColour.m_normalStrength.size(), &planetColour.m_normalStrength[0]);

    if (ViewPosition >= 0)
    {
        glm::vec3 playerPos = PlayerInput::playerPosition;
        glUniform3f(ViewPosition, playerPos.x, playerPos.y, playerPos.z);
    }
    if (time >= 0) glUniform1f(time, glfwGetTime());

    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}
