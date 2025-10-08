#pragma once

#include "ScriptObject.h"
#include "ShaderPrograms.h"

struct renderObject {
	Shader::FragmentShaderType fragType;
	Shader::VertexShaderType vertexType;

	GLuint vbo;
	GLuint vao;
};

class RenderableObject : public ScriptObject
{
public:
	virtual void Start()override;
	virtual void Render() override {}
protected:
	renderObject m_body;
	std::shared_ptr<Shader> m_shader;
};

