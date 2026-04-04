#include "ScreenOverlay.h"
#include "ShaderPrograms.h"
#include "Game.h"

void ScreenOverlay::Start()
{
	renderPriority = RenderPriority::GUI;

	m_fadeTimeLeft = m_maxFadeTime;

	float points[] = {      
		1.0f,   1.0f,  0.0f,
		1.0f,  -1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f,
		-1.0f,  1.0f,  0.0f,
		1.0f,   1.0f,  0.0f
	};

	glGenBuffers(1, &m_body.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_body.vao);
	glBindVertexArray(m_body.vao);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::Colour, Shader::FragmentShaderType::Colour);

	uColourLoc = glGetUniformLocation(m_shader->shaderPair, "colour");
	uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
	uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
	uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
}

void ScreenOverlay::Update()
{
	if (Game::deltaTime > 3.0f) return;

	if (m_fadeTimeLeft > 0.f)
	{
		m_fadeTimeLeft -= Game::deltaTime;

		m_opacity = (m_fadeTimeLeft / m_maxFadeTime);
	}
	else
	{
		m_opacity = 0.f;
	}
}

void ScreenOverlay::Render()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VertexShaders::LoadShader(m_shader);
	glBindVertexArray(m_body.vao);

	glm::mat4 identity = glm::mat4(1.0f);
	glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(identity));
	glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(identity));
	glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(identity));

	glUniform3f(uColourLoc, colour.x, colour.y, colour.z);

	glBlendColor(0.0f, 0.0f, 0.0f, m_opacity);
	glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	if (staticOverlayActive)
	{
		glUniform3f(uColourLoc, staticOverlay.x, staticOverlay.y, staticOverlay.z);

		glBlendColor(0.0f, 0.0f, 0.0f, 0.4f);
		glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void ScreenOverlay::sendMessage(const std::string& t_messageString)
{
	if (t_messageString == "DAMAGE")
	{
		m_fadeTimeLeft = m_maxFadeTime / 2.0f;
		colour = glm::vec3(255.0f, 0.0f, 0.0f);
	}
	if (t_messageString == "UNDERWATER")
	{
		staticOverlayActive = !staticOverlayActive;
		colour = glm::vec3(30.0f, 100.0f, 255.0f);
	}
}
