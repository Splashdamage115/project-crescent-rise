#pragma once

#include "Library.h"

struct Shader
{
	enum class VertexShaderType {
		standardBlue
	};
	enum class FragmentShaderType {
		standard
	};
	VertexShaderType vertexType;
	FragmentShaderType fragmentType;
	GLuint shaderPair;
};

struct ShaderFilesVertex 
{
	Shader::VertexShaderType vertexType;
	const char* file;
};

struct ShaderFilesFragment
{
	Shader::FragmentShaderType fragmentType;
	const char* file;
};

class VertexShaders
{
public:
	static void initialise();

	static void LoadShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment);
private:
	static std::vector<std::shared_ptr<Shader>> m_shaders;
	static std::vector<ShaderFilesVertex> m_vertexFiles;
	static std::vector<ShaderFilesFragment> m_fragmentFiles;
};

