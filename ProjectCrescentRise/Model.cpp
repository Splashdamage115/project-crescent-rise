#include "Model.h"
#include "ModelStore.h"

void Model::Start()
{
	m_body = MeshStore::getMesh("./Assets/Mesh/bottle.fbx");

	m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::Colour);


	//textureID = TextureStore::RetrieveTexture("./Assets/Images/crate.jpg");


	//heightMapID = TextureStore::RetrieveNormals("./Assets/Images/CrateNormal.jpg");

	heightLoc = glGetUniformLocation(m_shader->shaderPair, "uHeightMap");
	uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
	uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
	uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
	uColourLoc = glGetUniformLocation(m_shader->shaderPair, "colour");
	textureLoc = glGetUniformLocation(m_shader->shaderPair, "uTexture");
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

	glm::mat4 model = transform ? ToModelMatrix(*transform) : glm::mat4(1.0f);
	glm::mat4 view = Window::Get().GetView();
	glm::mat4 proj = Window::Get().GetProj();


	if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
	if (uColourLoc >= 0) glUniform3f(uColourLoc, 40.f, 255.f, 80.f);

	glDrawElements(GL_TRIANGLES, m_body.indexLength, GL_UNSIGNED_INT, 0);
}