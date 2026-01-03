#include "Skybox.h"
#include "stb_image.h"
#include "PlayerInput.h"
#include "Globals.h"

void Skybox::Start()
{
	float points[] = {

		-0.5f,-0.5f, 0.5f,
		 0.5f,-0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		-0.5f,-0.5f,-0.5f,
		 0.5f,-0.5f,-0.5f,
		 0.5f, 0.5f,-0.5f,
		-0.5f, 0.5f,-0.5f,

		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f,-0.5f,

		 0.5f,-0.5f,-0.5f,
		 0.5f,-0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f,-0.5f,

		-0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f,-0.5f,
		-0.5f, 0.5f,-0.5f,


		-0.5f,-0.5f, 0.5f,
		 0.5f,-0.5f, 0.5f,
		 0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Generate and setup EBO
	glGenBuffers(1, &m_body.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_body.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::skyBox, Shader::FragmentShaderType::skyBox);

	std::vector<std::string> faces;
	faces.resize(6);

	if (LOW_RES)
	{
		faces.at(0) = "./Assets/Images/SkyBox/LowRes_px.png"; // +X
		faces.at(1) = "./Assets/Images/SkyBox/LowRes_nx.png"; // -X
		faces.at(2) = "./Assets/Images/SkyBox/LowRes_py.png"; // +Y
		faces.at(3) = "./Assets/Images/SkyBox/LowRes_ny.png"; // -Y
		faces.at(4) = "./Assets/Images/SkyBox/LowRes_pz.png"; // +Z
		faces.at(5) = "./Assets/Images/SkyBox/LowRes_nz.png"; // -Z
	}
	else
	{
		faces.at(0) = "./Assets/Images/SkyBox/HighRes_px.png"; // +X
		faces.at(1) = "./Assets/Images/SkyBox/HighRes_nx.png"; // -X
		faces.at(2) = "./Assets/Images/SkyBox/HighRes_py.png"; // +Y
		faces.at(3) = "./Assets/Images/SkyBox/HighRes_ny.png"; // -Y
		faces.at(4) = "./Assets/Images/SkyBox/HighRes_pz.png"; // +Z
		faces.at(5) = "./Assets/Images/SkyBox/HighRes_nz.png"; // -Z
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, channels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 3);
		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load cubemap face: " << faces[i] << "\n";
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
	uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
	uSkyLoc = glGetUniformLocation(m_shader->shaderPair, "uSky");
	timeID = glGetUniformLocation(m_shader->shaderPair, "time");

}

void Skybox::Render()
{
	transform->position = PlayerInput::playerPosition;
	glDepthFunc(GL_LEQUAL); 
	glDepthMask(GL_FALSE);

	VertexShaders::LoadShader(m_shader);

	glBindVertexArray(m_body.vao);

	if (textureID != -1 && uSkyLoc != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glUniform1i(uSkyLoc, 0);
	}


	glm::mat4 view = glm::mat4(glm::mat3(Window::Get().GetView()));
	glm::mat4 proj = Window::Get().GetProj();

	glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));

	glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}