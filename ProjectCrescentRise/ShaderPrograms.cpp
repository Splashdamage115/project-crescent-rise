#include "ShaderPrograms.h"

std::vector<std::shared_ptr<Shader>> VertexShaders::m_shaders;
std::vector<ShaderFilesVertex> VertexShaders::m_vertexFiles;
std::vector<ShaderFilesFragment> VertexShaders::m_fragmentFiles;

void VertexShaders::initialise()
{
    const char* newVertex =
        "#version 410 core\n"
        "in vec3 vp;"
        "void main() {"
        "  gl_Position = vec4( vp, 1.0 );"
        "}";

    ShaderFilesVertex newVertexPair;
    newVertexPair.vertexType = Shader::VertexShaderType::standardBlue;
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
    newFragmentPair.file = newVertex;
    m_fragmentFiles.push_back(newFragmentPair);

    std::shared_ptr< Shader > newShader = std::make_shared<Shader>();
    newShader->vertexType = m_vertexFiles.at(0).vertexType;
    newShader->fragmentType = m_fragmentFiles.at(0).fragmentType;

    newShader->shaderPair = glCreateProgram();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &m_vertexFiles.at(0).file, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &m_fragmentFiles.at(0).file, NULL);
    glCompileShader(fs);

    glAttachShader(newShader->shaderPair, vs);
    glAttachShader(newShader->shaderPair, fs);

    glLinkProgram(newShader->shaderPair);

    m_shaders.push_back(newShader);
}

void VertexShaders::LoadShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment)
{
    for (int i = 0; i < m_shaders.size(); i++)
    {
        if (m_shaders.at(i)->vertexType == t_vertex &&
            m_shaders.at(i)->fragmentType == t_fragment)
        {
            glUseProgram(m_shaders.at(i)->shaderPair);
            return;
        }
    }
    glUseProgram(m_shaders.at(0)->shaderPair);
}
