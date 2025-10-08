#include "ShaderPrograms.h"

std::vector<std::shared_ptr<Shader>> VertexShaders::m_shaders;
std::vector<ShaderFilesVertex> VertexShaders::m_vertexFiles;
std::vector<ShaderFilesFragment> VertexShaders::m_fragmentFiles;

// load fragment and vertex shader files
void VertexShaders::initialise()
{
    const char* newVertex =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "void main() {\n"
        "    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);\n"
        "}\n";


    ShaderFilesVertex newVertexPair;
    newVertexPair.vertexType = Shader::VertexShaderType::standard;
    newVertexPair.file = newVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* newFragment =
        "#version 410 core\n"
        "out vec4 frag_colour;"
        "void main() {"
        "  frag_colour = vec4( 1.0, 0.0, 0.49, 1.0 );"
        "}";

    ShaderFilesFragment newFragmentPair;
    newFragmentPair.fragmentType = Shader::FragmentShaderType::standard;
    newFragmentPair.file = newFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::standard);
}

// *** NEVER USE THIS ***
void VertexShaders::LoadShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment)
{
    // find the shader and try to use it
    for (int i = 0; i < m_shaders.size(); i++)
    {
        if (m_shaders.at(i)->vertexType == t_vertex &&
            m_shaders.at(i)->fragmentType == t_fragment)
        {
            glUseProgram(m_shaders.at(i)->shaderPair);
            return;
        }
    }


    // if the combo requested doesnt exist yet, try to mount it, and use it
    try {
        mountShader(t_vertex, t_fragment);
        glUseProgram(m_shaders.at(m_shaders.size() - 1)->shaderPair);
    }
    catch(std::errc)
    {
        glUseProgram(m_shaders.at(0)->shaderPair); // otherwise revert to basic shader
    }

}

// try to keep the used shader in memory somewhere, otherwise call the other type
// we use this to speed up execution
void VertexShaders::LoadShader(std::shared_ptr<Shader> t_shader)
{
    glUseProgram(t_shader->shaderPair);
}

// retrieve the pointer to the wanted shader
std::shared_ptr<Shader> VertexShaders::retrieveShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment)
{
    // find the shader and try to use it
    for (int i = 0; i < m_shaders.size(); i++)
    {
        if (m_shaders.at(i)->vertexType == t_vertex &&
            m_shaders.at(i)->fragmentType == t_fragment)
        {
            return m_shaders.at(i);
        }
    }


    // if the combo requested doesnt exist yet, try to mount it, and use it
    try {
        mountShader(t_vertex, t_fragment);
        return m_shaders.at(m_shaders.size() - 1);
    }
    catch (std::errc)
    {
        return m_shaders.at(0);
    }

}

// mount any non existent shaders, and make a new shader for it
std::errc VertexShaders::mountShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment)
{
    std::shared_ptr< Shader > newShader = std::make_shared<Shader>();

    // find the file location (if either doesnt exist, exit)
    int vertexNum = -1;
    int fragNum = -1;

    for (int i = 0; i < m_vertexFiles.size(); i++)
    {
        if (m_vertexFiles.at(i).vertexType == t_vertex)
        {
            vertexNum = i;
            break;
        }
    }
    for (int i = 0; i < m_fragmentFiles.size(); i++)
    {
        if (m_fragmentFiles.at(i).fragmentType == t_fragment)
        {
            fragNum = i;
            break;
        }
    }

    if (vertexNum == -1 || fragNum == -1) return std::errc::timed_out;


    // initialise the new shader
    newShader->vertexType = m_vertexFiles.at(vertexNum).vertexType;
    newShader->fragmentType = m_fragmentFiles.at(fragNum).fragmentType;

    newShader->shaderPair = glCreateProgram();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &m_vertexFiles.at(vertexNum).file, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &m_fragmentFiles.at(fragNum).file, NULL);
    glCompileShader(fs);

    glAttachShader(newShader->shaderPair, vs);
    glAttachShader(newShader->shaderPair, fs);

    glLinkProgram(newShader->shaderPair);

    m_shaders.push_back(newShader);
}
