#include "Model.h"
#include "ModelStore.h"
#include "Game.h"
#include "TextureStore.h"

void Model::Start()
{
	renderPriority = followCam ? RenderPriority::Viewmodel : RenderPriority::Cull;
	m_body = MeshStore::getMesh(loadLocation);

	if (textureLoc1.empty())
	{
		m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::Colour);
	}
	else
	{
		m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::litTexture, Shader::FragmentShaderType::litTexture);
		textureID = TextureStore::RetrieveTexture(textureLoc1);
		heightMapID = TextureStore::RetrieveNormals("./Assets/Images/metal.jpg");
		colour = glm::vec3(1.0f);
	}





	heightLoc = glGetUniformLocation(m_shader->shaderPair, "uHeightMap");
	uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
	uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
	uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
	uColourLoc = glGetUniformLocation(m_shader->shaderPair, "colour");
	textureLoc = glGetUniformLocation(m_shader->shaderPair, "uTexture");
}

void Model::Update()
{
	if (useOffsetMover)
	{
		modelOffset.moveAlongForward(velocity * static_cast<float>(Game::deltaTime));
		modelOffset.rotation += rotation * static_cast<float>(Game::deltaTime);
		modelOffset.scale += scale * static_cast<float>(Game::deltaTime);
	}
}

void Model::Render()
{
	VertexShaders::LoadShader(m_shader);

	glDisable(GL_CULL_FACE); 
	glFrontFace(GL_CW);

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

	glm::mat4 model, view, proj;

	if (followCam)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		model = ToModelMatrix(modelOffset);
		view = glm::mat4(1.0f);
		proj = Window::Get().GetProj();
	}
	else
	{
		Transform localOffset = modelOffset;
		if (useCameraOffset)
		{
			localOffset.position.y += cameraHeightOffset;
		}

		if (transform)
		{
			model = useOffset
				? ToModelMatrix(*transform) * ToModelMatrix(localOffset)
				: ToModelMatrix(*transform + localOffset);
		}
		else
		{
			model = ToModelMatrix(localOffset);
		}
		view = Window::Get().GetView();
		proj = Window::Get().GetProj();
	}

	if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
	if (uColourLoc >= 0) glUniform3f(uColourLoc, colour.x, colour.y, colour.z);

	glDrawElements(GL_TRIANGLES, m_body.indexLength, GL_UNSIGNED_INT, 0);
}