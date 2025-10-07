#include "ShaderPrograms.h"

std::vector<std::pair<VertexShaders::VertexShaderType, const char*>> VertexShaders::m_shaders;

void VertexShaders::initialise()
{
    const char* newShader =
        "#version 410 core\n"
        "in vec3 vp;"
        "void main() {"
        "  gl_Position = vec4( vp, 1.0 );"
        "}";

    std::pair<VertexShaderType, const char*> newPair;
    newPair.first = VertexShaderType::standardBlue;
    newPair.second = newShader;

    m_shaders.push_back(newPair);
}

void VertexShaders::LoadShader(VertexShaderType t_type, GLuint& shader_program)
{
    for (int i = 0; i < m_shaders.size(); i++)
    {
        if (m_shaders.at(i).first == t_type)
        {
            GLuint vs = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs, 1, &m_shaders.at(i).second, NULL);
            glCompileShader(vs);

            glAttachShader(shader_program, vs);
        }
    }

}
