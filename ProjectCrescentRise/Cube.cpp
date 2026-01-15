#include "Cube.h"
#include "TextureStore.h"

void Cube::Start()
{
	float points[] = {
		//   pos                    uv        normal
		-0.5f,-0.5f, 0.5f,     0.0f,0.0f,   0.0f,0.0f,1.0f,
		 0.5f,-0.5f, 0.5f,     1.0f,0.0f,   0.0f,0.0f,1.0f,
		 0.5f, 0.5f, 0.5f,     1.0f,1.0f,   0.0f,0.0f,1.0f,
		-0.5f, 0.5f, 0.5f,     0.0f,1.0f,   0.0f,0.0f,1.0f,

		-0.5f,-0.5f,-0.5f,     1.0f,0.0f,   0.0f,0.0f,-1.0f,
		 0.5f,-0.5f,-0.5f,     0.0f,0.0f,   0.0f,0.0f,-1.0f,
		 0.5f, 0.5f,-0.5f,     0.0f,1.0f,   0.0f,0.0f,-1.0f,
		-0.5f, 0.5f,-0.5f,     1.0f,1.0f,   0.0f,0.0f,-1.0f,

		-0.5f,-0.5f,-0.5f,     0.0f,0.0f,  -1.0f,0.0f,0.0f,
		-0.5f,-0.5f, 0.5f,     1.0f,0.0f,  -1.0f,0.0f,0.0f,
		-0.5f, 0.5f, 0.5f,     1.0f,1.0f,  -1.0f,0.0f,0.0f,
		-0.5f, 0.5f,-0.5f,     0.0f,1.0f,  -1.0f,0.0f,0.0f,

		0.5f,-0.5f,-0.5f,     1.0f,0.0f,   1.0f,0.0f,0.0f,
		0.5f,-0.5f, 0.5f,     0.0f,0.0f,   1.0f,0.0f,0.0f,
		0.5f, 0.5f, 0.5f,     0.0f,1.0f,   1.0f,0.0f,0.0f,
		0.5f, 0.5f,-0.5f,     1.0f,1.0f,   1.0f,0.0f,0.0f,

		-0.5f, 0.5f, 0.5f,     0.0f,1.0f,   0.0f,1.0f,0.0f,
		 0.5f, 0.5f, 0.5f,     1.0f,1.0f,   0.0f,1.0f,0.0f,
		 0.5f, 0.5f,-0.5f,     1.0f,0.0f,   0.0f,1.0f,0.0f,
		-0.5f, 0.5f,-0.5f,     0.0f,0.0f,   0.0f,1.0f,0.0f,

		-0.5f,-0.5f, 0.5f,     0.0f,0.0f,   0.0f,-1.0f,0.0f,
		 0.5f,-0.5f, 0.5f,     1.0f,0.0f,   0.0f,-1.0f,0.0f,
		 0.5f,-0.5f,-0.5f,     1.0f,1.0f,   0.0f,-1.0f,0.0f,
		-0.5f,-0.5f,-0.5f,     0.0f,1.0f,   0.0f,-1.0f,0.0f
	};


	std::vector<unsigned int> indices = {
		0,  1,  2,    2,  3,  0,
		4,  5,  6,    6,  7,  4,
		8,  9,  10,   10, 11, 8,
		12, 13, 14,   14, 15, 12,
		16, 17, 18,   18, 19, 16,
		20, 21, 22,   22, 23, 20
	};


	glGenBuffers(1, &m_body.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_body.vao);
	glBindVertexArray(m_body.vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &m_body.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_body.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::litTexture, Shader::FragmentShaderType::litTexture);


	textureID = TextureStore::RetrieveTexture("./Assets/Images/crate.jpg");


	heightMapID = TextureStore::RetrieveNormals("./Assets/Images/CrateNormal.jpg");

	heightLoc = glGetUniformLocation(m_shader->shaderPair, "uHeightMap");
	uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
	uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
	uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
	uColourLoc = glGetUniformLocation(m_shader->shaderPair, "colour");
	textureLoc = glGetUniformLocation(m_shader->shaderPair, "uTexture");
}

void Cube::Render()
{
	VertexShaders::LoadShader(m_shader);

	glBindVertexArray(m_body.vao);
	if (textureLoc != -1 && heightMapID != -1 && heightLoc != -1 && textureID != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(textureLoc, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, heightMapID);
		glUniform1i(heightLoc, 1);
	}

	glm::mat4 model = transform ? ToModelMatrix(*transform) : glm::mat4(1.0f);
	glm::mat4 view = Window::Get().GetView();
	glm::mat4 proj = Window::Get().GetProj();


	if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
	if (uColourLoc >= 0) glUniform3f(uColourLoc, 1.f, 1.f, 1.f);

	glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
}