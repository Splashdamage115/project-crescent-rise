#include "Particle.h"
#include "TextureStore.h"
#include "AnimatedTextureStore.h"
#include "Game.h"

void Particle::Start()
{
        float points[] = {
        
        0.5f,   0.5f,  0.0f,   0.0f, 0.0f,
        0.5f,  -0.5f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f,   1.0f, 0.0f,
        0.5f,   0.5f,  0.0f,   0.0f, 0.0f
    };


    glGenBuffers(1, &m_body.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_body.vao);
    glBindVertexArray(m_body.vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::Particle, Shader::FragmentShaderType::Particle);

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
    uColourLoc = glGetUniformLocation(m_shader->shaderPair, "colour");
    textureLoc = glGetUniformLocation(m_shader->shaderPair, "uTexture");
	UVPositionLoc = glGetUniformLocation(m_shader->shaderPair, "UVPosition");
    


	initialiseData();
}

void Particle::initialiseData()
{
    if (maxTimeAlive > 0.f)
    {
        timeAliveLeft = maxTimeAlive;
        maxTimeAlive = 0.f;
		active = true;
    }
	currentFrame = 0;
	frameTimeLeft = frameTime;
	frameSize.x = textureSize.x / frameAmt.x;
    frameSize.y = textureSize.y / frameAmt.y;
    AnimatedTextureStore::AddAnimatedTexture(textureLocation, frameSize, textureSize, frameAmt.x * frameAmt.y);
}

void Particle::Update()
{
	if (!active) return;
	frameTimeLeft -= static_cast<float>(Game::deltaTime);
	timeAliveLeft -= static_cast<float>(Game::deltaTime);
    if (frameTimeLeft <= 0.f)
    {
        currentFrame++;
        frameTimeLeft = frameTime;
    }
	if (timeAliveLeft <= 0.f)
    {
        active = false;
    }
}

void Particle::Render()
{
    if (!active) return;

    VertexShaders::LoadShader(m_shader);

    glBindVertexArray(m_body.vao);

    glm::mat4 view = Window::Get().GetView();

    glm::vec3 right = glm::vec3(view[0][0], view[1][0], view[2][0]);
    glm::vec3 up = glm::vec3(view[0][1], view[1][1], view[2][1]);
    glm::vec3 forward = glm::vec3(view[0][2], view[1][2], view[2][2]);

    glm::mat4 model = glm::mat4(glm::vec4(right * positionOverride->scale.x, 0.0f), glm::vec4(up * positionOverride->scale.y, 0.0f), glm::vec4(forward * positionOverride->scale.z, 0.0f), glm::vec4(positionOverride->position, 1.0f));
    
    glm::mat4 proj = Window::Get().GetProj();
    glm::vec4 uv;
	if (AnimatedTextureStore::RetrieveAnimatedTexture(textureLocation, currentFrame, uv))
    {
        if(UVPositionLoc != -1)
			glUniform4f(UVPositionLoc, uv.x, uv.y, uv.z, uv.w);
        if (textureLoc != -1)
            glUniform1i(textureLoc, 0);
    }

    if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
    if (uColourLoc >= 0) glUniform3f(uColourLoc, 1.f, 1.f, 1.f);

    glEnable(GL_ALPHA_TEST);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
