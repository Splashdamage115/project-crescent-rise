#pragma once

#include "Library.h"

struct Shader
{
	enum class VertexShaderType {
		standard, terrain
	};
	enum class FragmentShaderType {
		standard, checkerboard, Blue, terrain
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
	static void LoadShader(std::shared_ptr<Shader> t_shader);
	static std::shared_ptr<Shader> retrieveShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment);
private:
	static std::errc mountShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment);
	static std::vector<std::shared_ptr<Shader>> m_shaders;
	static std::vector<ShaderFilesVertex> m_vertexFiles;
	static std::vector<ShaderFilesFragment> m_fragmentFiles;
};

