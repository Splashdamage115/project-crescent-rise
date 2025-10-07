#pragma once

#include "Library.h"

class VertexShaders
{
public:
	enum class VertexShaderType {
		standardBlue
	};

	static void initialise();

	static void LoadShader(VertexShaderType t_type, GLuint& shader_program);
private:
	static std::vector<std::pair<VertexShaderType, const char*>> m_shaders;
};

