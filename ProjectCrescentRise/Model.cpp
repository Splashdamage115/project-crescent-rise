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
		m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::Colour, Shader::FragmentShaderType::Colour);
	}
	else
	{
		m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::litTexture, Shader::FragmentShaderType::litTexture);
		textureID = TextureStore::RetrieveTexture(textureLoc1);
		heightMapID = TextureStore::RetrieveNormals("./Assets/Images/metal.jpg");
		colour = glm::vec3(1.0f);
		if (!textureLoc2.empty())
			textureID2 = TextureStore::RetrieveTexture(textureLoc2);
	}





	heightLoc = glGetUniformLocation(m_shader->shaderPair, "uHeightMap");
	uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
	uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
	uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
	uColourLoc = glGetUniformLocation(m_shader->shaderPair, "colour");
	textureLoc = glGetUniformLocation(m_shader->shaderPair, "uTexture");
	uTexture2Loc = glGetUniformLocation(m_shader->shaderPair, "uTexture2");
	highlightLoc = glGetUniformLocation(m_shader->shaderPair, "uHighlight");
	viewPosLoc = glGetUniformLocation(m_shader->shaderPair, "viewPos");

	m_outlineShader = VertexShaders::retrieveShader(Shader::VertexShaderType::outline, Shader::FragmentShaderType::outline);
	outlineModelLoc = glGetUniformLocation(m_outlineShader->shaderPair, "uModel");
	outlineViewLoc  = glGetUniformLocation(m_outlineShader->shaderPair, "uView");
	outlineProjLoc  = glGetUniformLocation(m_outlineShader->shaderPair, "uProj");
	outlineWidthLoc = glGetUniformLocation(m_outlineShader->shaderPair, "uOutlineWidth");
	outlineColorLoc = glGetUniformLocation(m_outlineShader->shaderPair, "outlineColor");
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

		if (uTexture2Loc != -1 && textureID2 != -1)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, textureID2);
			glUniform1i(uTexture2Loc, 2);
		}
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
	if (highlightLoc >= 0)
	{
		glUniform1i(highlightLoc, highlight);
	}
	if (viewPosLoc >= 0)
	{
		glm::vec3 camPos = Window::Get().GetCameraPosition();
		glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);
	}

	glDrawElements(GL_TRIANGLES, m_body.indexLength, GL_UNSIGNED_INT, 0);

	if (highlight)
	{
		VertexShaders::LoadShader(m_outlineShader);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		if (outlineModelLoc >= 0) glUniformMatrix4fv(outlineModelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (outlineViewLoc  >= 0) glUniformMatrix4fv(outlineViewLoc,  1, GL_FALSE, glm::value_ptr(view));
		if (outlineProjLoc  >= 0) glUniformMatrix4fv(outlineProjLoc,  1, GL_FALSE, glm::value_ptr(proj));
		if (outlineWidthLoc >= 0) glUniform1f(outlineWidthLoc, outlineWidth);
		if (outlineColorLoc >= 0) glUniform3f(outlineColorLoc, outlineColor.x / 255.f, outlineColor.y / 255.f, outlineColor.z / 255.f);

		glDrawElements(GL_TRIANGLES, m_body.indexLength, GL_UNSIGNED_INT, 0);

		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}
}