#pragma once

#include "ScriptObject.h"
#include "ShaderPrograms.h"
#include "Window.h"
#include <glm/gtc/type_ptr.hpp>

struct renderObject {
	Shader::FragmentShaderType fragType;
	Shader::VertexShaderType vertexType;

	GLuint ebo;
	GLuint vbo;
	GLuint vao;
};

class RenderableObject : public ScriptObject
{
public:
	virtual void Start()override;
	virtual void Render() override = 0;
protected:
	renderObject m_body;
	std::shared_ptr<Shader> m_shader;
	
	GLint uModelLoc = -1;
	GLint uViewLoc = -1;
	GLint uProjLoc = -1;
};

