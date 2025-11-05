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
        "layout (location = 1) in vec2 aTexCoord;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "out vec2 TexCoords;\n"
        "void main() {\n"
        "    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);\n"
        "    TexCoords = aTexCoord;\n"
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

    const char* newFragment2 =
        "#version 410 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   vec2 uv = TexCoords * 8.0;\n"
        "   float checker = mod(floor(uv.x) + floor(uv.y), 2.0);\n"
        "   FragColor = checker == 0.0 ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.1, 0.1, 0.15, 1.0);\n"
        "}\n";

    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::checkerboard;
    newFragmentPair.file = newFragment2;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::checkerboard);

    const char* newFragment3 =
        "#version 410 core\n"
        "uniform vec3 colour;\n"
        "out vec4 frag_colour;\n"
        "void main() {\n"
        "  frag_colour = vec4(colour.x / 255.0, colour.y / 255.0, colour.z / 255.0, 1.0);\n"
        "}\n";

    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::Colour;
    newFragmentPair.file = newFragment3;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::Colour);


    const char* terrainVertex =
        "#version 410 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec2 aTexCoord;\n"
        "\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "\n"
        "out vec2 TexCoords;\n"
        "out float worldY;\n"
        "\n"
        "void main() {\n"
        "    vec4 worldPos = uModel * vec4(aPos, 1.0);\n"
        "    gl_Position = uProj * uView * worldPos;\n"
        "    TexCoords = aTexCoord;\n"
        "    worldY = worldPos.y;\n"
        "}\n";

    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::terrain;
    newVertexPair.file = terrainVertex;
    m_vertexFiles.push_back(newVertexPair);


    const char* terrainFrag =
        "#version 410 core\n"
        "in vec2 TexCoords;\n"
        "in float worldY;\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "uniform float lowHeight = 12.0;\n"
        "uniform float midHeight = 14.0;\n"
        "uniform float highHeight = 20.0;\n"
        "\n"
        "vec4 getHeightColor(float y) {\n"
        "    if (y < midHeight) {\n"
        "        // Blend yellow to green\n"
        "        float t = clamp((y - lowHeight) / (midHeight - lowHeight), 0.0, 1.0);\n"
        "        return mix(vec4(1.0, 1.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 1.0), t);\n"
        "    }\n"
        "    else {\n"
        "        // Blend green to grey\n"
        "        float t = clamp((y - midHeight) / (highHeight - midHeight), 0.0, 1.0);\n"
        "        return mix(vec4(0.0, 1.0, 0.0, 1.0), vec4(0.5, 0.5, 0.5, 1.0), t);\n"
        "    }\n"
        "}\n"
        "\n"
        "void main() {\n"
        "    FragColor = getHeightColor(worldY);\n"
        "}\n";

    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::terrain;
    newFragmentPair.file = terrainFrag;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::terrain, Shader::FragmentShaderType::terrain);

    const char* litVertex =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "layout (location = 2) in vec3 aNormal;\n"

        "uniform vec3 CenterPoint;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"

        "out vec2 TexCoords;\n"
        "out vec3 WorldPos;\n"
        "out vec3 Normal;\n"
        "out float height;\n"

        "void main() {\n"
        "    vec4 worldPos = uModel * vec4(aPos, 1.0);\n"
        "    gl_Position = uProj * uView * worldPos;\n"
        "    WorldPos = worldPos.xyz;\n"
        "    Normal = mat3(transpose(inverse(uModel))) * aNormal;\n"
        "    TexCoords = aTexCoord;\n"
        "    height = sqrt((CenterPoint.x - WorldPos.x) * (CenterPoint.x - WorldPos.x) + (CenterPoint.y - WorldPos.y) * (CenterPoint.y - WorldPos.y) + (CenterPoint.z - WorldPos.z) * (CenterPoint.z - WorldPos.z));\n"
        "}\n";

    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::lit;
    newVertexPair.file = litVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* litFragment =
        "#version 410 core\n"
        "in vec3 WorldPos;\n"
        "in float height;\n"
        "in vec3 Normal;\n"
        "out vec4 FragColor;\n"

        //"uniform vec3 baseColour;\n"
        "uniform vec2 minMax;\n"
        "const int MAX_VEC_SIZE = 32;\n"

        "uniform vec3 heightColours[MAX_VEC_SIZE];\n"
        "uniform float startHeight[MAX_VEC_SIZE];\n"
        //"in float feather;\n"

        "const vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));\n"
        "const vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
        "const vec3 viewPos = vec3(0.0, 0.0, 10.0);\n"

        "void main() {\n"
        "    // Normalize the interpolated normal\n"
        "    vec3 norm = normalize(Normal);\n"
        "    float min = minMax.x;\n"
        "    float max = minMax.y;\n"
        "    float heightPercent = ((height - min) / max);\n"  
        "    vec3 colourOverride = vec3(252.0,15.0,192.0);\n"
        //"    if (heightPercent <= 0.01) colourOverride = vec3(0.0,0.0,255.0);\n"
        "    for (int i = 0; i < MAX_VEC_SIZE; i++) {\n"
        //"       if(startHeight[i] == 0.0) continue;\n"
        "       if(startHeight[i] <= heightPercent){\n"
        "           colourOverride = heightColours[i];\n"
        "       }}\n"
        "    vec3 objectColor = (colourOverride / 255.0);\n"
        "    float ambientStrength = 0.2;\n"
        "    vec3 ambient = ambientStrength * lightColor;\n"
        "    float diff = max(dot(norm, -lightDir), 0.0);\n"
        "    vec3 diffuse = diff * lightColor;\n"
        "    float specularStrength = 0.5;\n"
        "    vec3 viewDir = normalize(viewPos - WorldPos);\n"
        "    vec3 reflectDir = reflect(lightDir, norm);\n"
        "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
        "    vec3 specular = specularStrength * spec * lightColor;\n"
        //"    vec3 result = (ambient + diffuse) * objectColor + specular;\n"
        "    vec3 result = objectColor;\n"  
        "    FragColor = vec4(result, 1.0);\n"
        "}\n";

    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::lit;
    newFragmentPair.file = litFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::lit, Shader::FragmentShaderType::lit);
}

// *** NEVER USE THIS ***
void VertexShaders::LoadShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment)
{
    // find the shader and try to use it
    for (unsigned int i = 0; i < m_shaders.size(); i++)
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
    //glBindTexture To Do
}

// retrieve the pointer to the wanted shader
std::shared_ptr<Shader> VertexShaders::retrieveShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment)
{
    // find the shader and try to use it
    for (unsigned int i = 0; i < m_shaders.size(); i++)
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

    for (unsigned int i = 0; i < m_vertexFiles.size(); i++)
    {
        if (m_vertexFiles.at(i).vertexType == t_vertex)
        {
            vertexNum = i;
            break;
        }
    }
    for (unsigned int i = 0; i < m_fragmentFiles.size(); i++)
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
    GLint success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
        return std::errc::timed_out;
    }


    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &m_fragmentFiles.at(fragNum).file, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
        return std::errc::timed_out;
    }

    glAttachShader(newShader->shaderPair, vs);
    glAttachShader(newShader->shaderPair, fs);

    glLinkProgram(newShader->shaderPair);

    m_shaders.push_back(newShader);
}
